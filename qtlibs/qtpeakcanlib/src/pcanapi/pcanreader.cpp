// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pcanreader.hpp"

#include <QDebug>
#include <QMutex>

#include "src/pcanapi/pcanapi.hpp"
#include "src/pcanapi/pcanframedlc.hpp"

#include "src/pcanapi/import_pcanbasic.hpp"


PCanReader::PCanReader(PCanBusItf::Enum canBusItf, bool isCanFd, QObject *parent)
    : QObject{parent},
    _isCanFd{isCanFd},
    _canBusItf{canBusItf},
    _readMutex{new QMutex()}
{
}

PCanReader::~PCanReader()
{
    waitForProcessEnd();

    delete _readMutex;
    _readMutex = nullptr;
}

bool PCanReader::waitForProcessEnd(int timeoutInMs)
{
    if(_readMutex == nullptr)
    {
        qWarning() << "The PCan reader mutex is null, this should never happen. There is nothing "
                   << "to wait, the object is destroyed.";
        return true;
    }

    if(!_readMutex->tryLock(timeoutInMs)) {
        qWarning() << "A process is still blocking the PCan reader mutex and the timeout has "
                   << "raised, we abandon the waiting";
        return false;
    }

    _readMutex->unlock();
    return true;
}

void PCanReader::cancelReading()
{
    qDebug() << "Ask for read cancelling";
    _cancel = true;
}

void PCanReader::readMessages()
{
    if(_readMutex == nullptr || !_readMutex->tryLock())
    {
        qWarning() << "We are already trying to read messages or the object is destroyed";
        return;
    }

    manageMsgReading();

    _readMutex->unlock();
}

bool PCanReader::manageMsgReading()
{
    const quint16 canBusItfHandle = PCanBusItf::toTPCanHandle(_canBusItf);

    HANDLE eventBuffer = CreateEvent(nullptr, FALSE, FALSE, L"");

    const TPCANStatus setValueResult = CAN_SetValue(canBusItfHandle,
                                                    PCAN_RECEIVE_EVENT,
                                                    &eventBuffer,
                                                    sizeof(eventBuffer));

    if(setValueResult  != PCAN_ERROR_OK)
    {
        qWarning() << "A problem occurred: " << PCanApi::getErrorText(setValueResult)
                   << ", when tried to register the listening of read CAN FD event, with CAN "
                   << "BUS: " << PCanBusItf::toString(_canBusItf);
        return false;
    }

    while(!_cancel)
    {
        if(WaitForSingleObject(eventBuffer, ReadWaitingTimeoutInMs) == WAIT_OBJECT_0)
        {
            if(!processReceivedMessages())
            {
                qWarning() << "A fatal error occurred in the CAN reading process, we stop reading";
                break;
            }
        }
    }

    CloseHandle(eventBuffer);
    eventBuffer = nullptr;

    const TPCANStatus unsetValueResult = CAN_SetValue(canBusItfHandle,
                                                      PCAN_RECEIVE_EVENT,
                                                      &eventBuffer,
                                                      sizeof(eventBuffer));

    if(unsetValueResult  != PCAN_ERROR_OK)
    {
        qWarning() << "A problem occurred: " << PCanApi::getErrorText(unsetValueResult)
                   << ", when tried to unregister the listening of read CAN FD event, with CAN "
                   << "BUS: " << PCanBusItf::toString(_canBusItf);
        return false;
    }

    return true;
}

bool PCanReader::processReceivedMessages()
{
    TPCANStatus status = PCAN_ERROR_OK;

    while(isItOkToContinueMessageProcessing(status) && !_cancel)
    {
        status = _isCanFd ? processCanFdMessages() : processCanMessages();

        processFramesQueueEmitting();
    }

    // We send all we got before waiting for new event
    processFramesQueueEmitting(true);

    return !isReadErrorFatal(status);
}

quint32 PCanReader::processCanMessages()
{
    TPCANMsg canMsg;
    TPCANTimestamp canTimeStamp;
    
    const TPCANStatus canStatus = CAN_Read(PCanBusItf::toTPCanHandle(_canBusItf),
                                           &canMsg,
                                           &canTimeStamp);

    if(canStatus == PCAN_ERROR_QRCVEMPTY)
    {
        // The empty queue isn't considered as an error
        // Nothing has to be done here
        return canStatus;
    }

    if(canStatus != PCAN_ERROR_OK)
    {
        qWarning() << "A problem occurred: " << PCanApi::getErrorText(canStatus)
                   << ", when tried to read a can message, with bus: "
                   << PCanBusItf::toString(_canBusItf);
        return canStatus;
    }

    // The following code is inspired by the message management done by Qt in their libs
    if(Q_UNLIKELY(canMsg.MSGTYPE & PCAN_MESSAGE_STATUS))
    {
        // Filter out PCAN status frames, to avoid turning them
        // into QCanBusFrame::DataFrames with random canId
        return PCAN_ERROR_OK;
    }

    const int size = static_cast<int>(canMsg.LEN);
    QCanBusFrame frame(canMsg.ID, QByteArray(reinterpret_cast<const char *>(canMsg.DATA), size));

    const quint64 millis = canTimeStamp.millis +
                           (MillisOverflowCoeff * canTimeStamp.millis_overflow);

    const quint64 micros = (MilliToMicroCoeff * millis) + canTimeStamp.micros;
    frame.setTimeStamp(QCanBusFrame::TimeStamp::fromMicroSeconds(static_cast<qint64>(micros)));

    frame.setExtendedFrameFormat(canMsg.MSGTYPE & PCAN_MESSAGE_EXTENDED);
    frame.setFrameType((canMsg.MSGTYPE & PCAN_MESSAGE_RTR) ? QCanBusFrame::RemoteRequestFrame :
                                                             QCanBusFrame::DataFrame);

    qDebug() << "A frame has been received from CAN bus: " <<  PCanBusItf::toString(_canBusItf)
             << ", the frame: " << frame.toString();

    _framesQueue.append(frame);
    return PCAN_ERROR_OK;
}

quint32 PCanReader::processCanFdMessages()
{
    TPCANMsgFD canFdMsg;
    TPCANTimestampFD canTimeStamp;
    
    const TPCANStatus canStatus = CAN_ReadFD(PCanBusItf::toTPCanHandle(_canBusItf),
                                             &canFdMsg,
                                             &canTimeStamp);

    if(canStatus == PCAN_ERROR_QRCVEMPTY)
    {
        // The empty queue isn't considered as an error
        // Nothing has to be done here
        return canStatus;
    }

    if(canStatus != PCAN_ERROR_OK)
    {
        qWarning() << "A problem occurred: " << PCanApi::getErrorText(canStatus)
                   << ", when tried to read a FD can message, with bus: "
                   << PCanBusItf::toString(_canBusItf);
        return  canStatus;
    }

    // The following code is inspired by the message management done by Qt in their libs
    if(Q_UNLIKELY(canFdMsg.MSGTYPE & PCAN_MESSAGE_STATUS))
    {
        // Filter out PCAN status frames, to avoid turning them
        // into QCanBusFrame::DataFrames with random canId
        return PCAN_ERROR_OK;
    }

    const PCanFrameDlc::Enum frameDlc = PCanFrameDlc::parseFromByte(canFdMsg.DLC);

    if(frameDlc == PCanFrameDlc::Unknown)
    {
        qWarning() << "A problem occurred when tried to parse the DLC value of the CAN FD message, "
                   << "we can't parse the message and we will ignore it. The CAN FD msg id is: "
                   << canFdMsg.ID;
        return PCAN_ERROR_OK;
    }

    QCanBusFrame frame(canFdMsg.ID, QByteArray(reinterpret_cast<const char *>(canFdMsg.DATA),
                                               PCanFrameDlc::toSize(frameDlc)));

    frame.setTimeStamp(QCanBusFrame::TimeStamp::fromMicroSeconds(
                                                                static_cast<qint64>(canTimeStamp)));
    frame.setExtendedFrameFormat(canFdMsg.MSGTYPE & PCAN_MESSAGE_EXTENDED);
    frame.setFrameType((canFdMsg.MSGTYPE & PCAN_MESSAGE_RTR)
                           ? QCanBusFrame::RemoteRequestFrame : QCanBusFrame::DataFrame);
    frame.setFlexibleDataRateFormat(canFdMsg.MSGTYPE & PCAN_MESSAGE_FD);
    frame.setBitrateSwitch(canFdMsg.MSGTYPE & PCAN_MESSAGE_BRS);
    frame.setErrorStateIndicator(canFdMsg.MSGTYPE & PCAN_MESSAGE_ESI);

    qDebug() << "A frame has been received from FD CAN bus: " <<  PCanBusItf::toString(_canBusItf)
             << ", the frame: " << frame.toString();
    _framesQueue.append(frame);

    return canStatus;
}

void PCanReader::processFramesQueueEmitting(bool forceSendAll)
{
    const int frameQueueSize = _framesQueue.size();
    if(frameQueueSize == 0 || (frameQueueSize < MaxFramesNbToEmit && !forceSendAll))
    {
        // Nothing to do
        return;
    }

    emit framesReceived(_framesQueue);
    _framesQueue.clear();
}

bool PCanReader::isItOkToContinueMessageProcessing(quint32 errorStatus)
{
    if(errorStatus == PCAN_ERROR_OK)
    {
        // In that case, everything is ok, we can continue to process message
        return true;
    }

    if(errorStatus == PCAN_ERROR_QOVERRUN)
    {
        // In that case, the message was read too late, we pop it and continue to read
        return true;
    }

    return false;
}

bool PCanReader::isReadErrorFatal(quint32 errorStatus)
{
    // This has to be completed with fatal error when we meet them (a fatal error is an error where
    // the reading is no more possible and we have to recreate all the connection)
    return (errorStatus == PCAN_ERROR_INITIALIZE ||
            errorStatus == PCAN_ERROR_ILLOPERATION ||
            errorStatus == PCAN_ERROR_ILLDATA ||
            errorStatus == PCAN_ERROR_RESOURCE ||
            errorStatus == PCAN_ERROR_ILLHW);
}
