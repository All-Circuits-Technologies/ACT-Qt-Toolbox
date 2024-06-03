// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "candevice.hpp"

#include <QCanBus>
#include <QDebug>

#include "definesutility/definesutility.hpp"
#include "waitutility/waithelper.hpp"

#include "src/models/candeviceconfig.hpp"
#include "src/models/expectedcanframemask.hpp"
#include "src/pcanapi/pcanapi.hpp"
#include "src/pcanapi/pcanreadthread.hpp"


CanDevice::CanDevice(const CanDeviceConfig &config, QObject *parent)
    : QObject{parent},
    _config{config}
{
}

CanDevice::~CanDevice()
{
    unInitialize();
}

bool CanDevice::initialize()
{
    if(_readThread != nullptr)
    {
        qInfo() << "The CAN device: " << _config.getCanBusItfName() << ", is already initialized "
                << "we can't process";
        return true;
    }

    if(_config.isCanFd())
    {
        RETURN_IF_FALSE(PCanApi::initializeCanFd(_config));
    }
    else
    {
        RETURN_IF_FALSE(PCanApi::initializeCan(_config));
    }

    _readThread = new PCanReadThread(_config.getCanBusItf(), _config.isCanFd());

    connect(_readThread, &PCanReadThread::framesReceived, this, &CanDevice::framesReceived);

    if(!_readThread->startThreadAndWaitToBeReady())
    {
        qWarning() << "A problem occurred when tried to start the CAN read thread";
        _readThread->stopAndDeleteThread();
        _readThread = nullptr;
        return false;
    }

    qDebug() << "The CAN device: " << _config.getCanBusItfName() << ", is initialized";
    return true;
}

bool CanDevice::unInitialize()
{
    if(_readThread == nullptr)
    {
        qInfo() << "The CAN device: " << _config.getCanBusItfName() << ", is not initialized "
                << "we can't uninitialize it";
        return true;
    }

    // This will waits the read thread to leave properly
    _readThread->stopAndDeleteThread();
    _readThread = nullptr;

    RETURN_IF_FALSE(PCanApi::unInitializeCan(_config.getCanBusItf()));

    qDebug() << "The CAN device: " << _config.getCanBusItfName() << ", is uninitialized";

    return true;
}

bool CanDevice::isInitialized() const
{
    return (_readThread != nullptr);
}

bool CanDevice::getParamBusOffAutoReset(bool *autoReset)
{
    if(_readThread == nullptr)
    {
        qWarning() << "We can't get the bus off auto reset value, for CAN bus intf: "
                   << _config.getCanBusItfName() << ", because the can device hasn't "
                   << "been initialized";
        return false;
    }

    if(autoReset == nullptr)
    {
        qWarning() << "We can't get the bus off auto reset value, for CAN bus intf: "
                   << _config.getCanBusItfName() << ", because the auto reset param pointer is "
                   << "nullptr";
        return false;
    }

    return PCanApi::getParamBusOffAutoReset(_config.getCanBusItf(), *autoReset);
}

bool CanDevice::setParamBusOffAutoReset(bool autoReset)
{
    if(_readThread == nullptr)
    {
        qWarning() << "We can't set the bus off auto reset value, for CAN bus intf: "
                   << _config.getCanBusItfName() << ", because the can device hasn't "
                   << "been initialized";
        return false;
    }

    return PCanApi::setParamBusOffAutoReset(_config.getCanBusItf(), autoReset);
}

bool CanDevice::write(const QCanBusFrame &frame)
{
    if(_readThread == nullptr)
    {
        qWarning() << "We can't write the given frame: " << frame.toString() << ", for CAN bus "
                   << "intf: " << _config.getCanBusItfName() << ", because the can device hasn't "
                   << "been initialized";
        return false;
    }

    if(_config.isCanFd())
    {
        return PCanApi::writeCanFdMsgProcess(_config.getCanBusItf(), frame);
    }

    return PCanApi::writeCanMsgProcess(_config.getCanBusItf(), frame);
}

QVector<QCanBusFrame> CanDevice::writeAndWaitAnswer(const QCanBusFrame &frame,
                                                    const ExpectedCanFrameMask &expectedFrameMask,
                                                    int timeoutInMs)
{
    if(_readThread == nullptr)
    {
        qWarning() << "We can't write the given frame: " << frame.toString() << " and wait its "
                   << "answer, for CAN bus intf: " << _config.getCanBusItfName()
                   << ", because the can device hasn't been initialized";
        return {};
    }

    return writeAndwaitFrameAndProcessIfNeeded(frame, { expectedFrameMask }, true, timeoutInMs);
}

QVector<QCanBusFrame> CanDevice::writeAndWaitAnswerById(const QCanBusFrame &frame,
                                                        quint32 answerId,
                                                        int timeoutInMs)
{
    return writeAndWaitAnswer(frame, ExpectedCanFrameMask{ answerId }, timeoutInMs);
}

QVector<QCanBusFrame> CanDevice::writeAndWaitOneOfAnswers(
    const QCanBusFrame &frame,
    const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
    int timeoutInMs)
{
    if(_readThread == nullptr)
    {
        qWarning() << "We can't write the given frame: " << frame.toString() << " and wait one of "
                   << "the expected answers, for CAN bus intf: " << _config.getCanBusItfName()
                   << ", because the can device hasn't been initialized";
        return {};
    }

    return writeAndwaitFrameAndProcessIfNeeded(frame, expectedFrameMasks, false, timeoutInMs);
}

QVector<QCanBusFrame> CanDevice::writeAndWaitOneOfAnswersByIds(const QCanBusFrame &frame,
                                                               const QVector<quint32> &answersIds,
                                                               int timeoutInMs)
{
    return writeAndWaitOneOfAnswers(frame,
                                    ExpectedCanFrameMask::convert(answersIds),
                                    timeoutInMs);
}

QVector<QCanBusFrame> CanDevice::writeAndWaitAllTheAnswers(
    const QCanBusFrame &frame,
    const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
    int timeoutInMs)
{
    if(_readThread == nullptr)
    {
        qWarning() << "We can't write the given frame: " << frame.toString() << " and wait for all "
                   << "the expected answers, for CAN bus intf: " << _config.getCanBusItfName()
                   << ", because the can device hasn't been initialized";
        return {};
    }

    return writeAndwaitFrameAndProcessIfNeeded(frame, expectedFrameMasks, false, timeoutInMs);
}

QVector<QCanBusFrame> CanDevice::writeAndWaitAllTheAnswersByIds(const QCanBusFrame &frame,
                                                                const QVector<quint32> &answersIds,
                                                                int timeoutInMs)
{
    return writeAndWaitAllTheAnswers(frame,
                                     ExpectedCanFrameMask::convert(answersIds),
                                     timeoutInMs);
}

QVector<QCanBusFrame> CanDevice::waitCanMsg(const ExpectedCanFrameMask &expectedFrameMask,
                                            int timeoutInMs)
{
    if(_readThread == nullptr)
    {
        qWarning() << "We can't wait the given frame with id: " << expectedFrameMask.toString()
                   << " for CAN bus intf: " << _config.getCanBusItfName()
                   << ", because the can device hasn't been initialized";
        return {};
    }

    return waitFrameAndProcessIfNeeded({ expectedFrameMask }, true, nullptr, timeoutInMs);
}

QVector<QCanBusFrame> CanDevice::waitCanMsgById(quint32 answerId, int timeoutInMs)
{
    return waitCanMsg(ExpectedCanFrameMask{ answerId }, timeoutInMs);
}

QVector<QCanBusFrame> CanDevice::waitOneOfTheCanMsgs(
    const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
    int timeoutInMs)
{
    if(_readThread == nullptr)
    {
        qWarning() << "We can't wait for one of the given frames with ids: " << expectedFrameMasks
                   << " for CAN bus intf: " << _config.getCanBusItfName()
                   << ", because the can device hasn't been initialized";
        return {};
    }

    return waitFrameAndProcessIfNeeded(expectedFrameMasks, false, nullptr, timeoutInMs);
}

QVector<QCanBusFrame> CanDevice::waitOneOfTheCanMsgsByIds(const QVector<quint32> &answersIds,
                                                          int timeoutInMs)
{
    return waitOneOfTheCanMsgs(ExpectedCanFrameMask::convert(answersIds), timeoutInMs);
}

QVector<QCanBusFrame> CanDevice::waitAllTheCanMsgs(
    const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
    int timeoutInMs)
{
    if(_readThread == nullptr)
    {
        qWarning() << "We can't wait for all the given frames with ids: " << expectedFrameMasks
                   << " for CAN bus intf: " << _config.getCanBusItfName()
                   << ", because the can device hasn't been initialized";
        return {};
    }

    return waitFrameAndProcessIfNeeded(expectedFrameMasks, true, nullptr, timeoutInMs);
}

QVector<QCanBusFrame> CanDevice::waitAllTheCanMsgsByIds(const QVector<quint32> &answersIds,
                                                   int timeoutInMs)
{
    return waitAllTheCanMsgs(ExpectedCanFrameMask::convert(answersIds), timeoutInMs);
}

QVector<QCanBusFrame> CanDevice::writeAndwaitFrameAndProcessIfNeeded(
    const QCanBusFrame &frame,
    const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
    bool areWeWaitingForAllMsgIds,
    int timeoutInMs)
{
    const std::function<bool ()> writeMsg = [this, &frame]() {
        return write(frame);
    };

    return waitFrameAndProcessIfNeeded(expectedFrameMasks,
                                       areWeWaitingForAllMsgIds,
                                       &writeMsg,
                                       timeoutInMs);
}

QVector<QCanBusFrame> CanDevice::waitFrameAndProcessIfNeeded(
    const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
    bool areWeWaitingForAllMsgIds,
    const std::function<bool ()> *process,
    int timeoutInMs)
{
    QVector<ExpectedCanFrameMask> waitingFrames(expectedFrameMasks);
    QVector<QCanBusFrame> foundFrames;

    auto waitingConn = connect(_readThread,
                               &PCanReadThread::framesReceived,
                               this,
                               [&foundFrames, &waitingFrames, areWeWaitingForAllMsgIds](
                                   const QVector<QCanBusFrame> &frames)
                               {
                                   if(waitingFrames.isEmpty())
                                   {
                                       // Nothing to do, we already received the expected frame(s)
                                       return;
                                   }

                                   for(auto citer = frames.cbegin();
                                       citer != frames.cend();
                                       ++citer)
                                   {
                                       int indexOf = ExpectedCanFrameMask::indexOf(waitingFrames,
                                                                                   *citer);
                                       if(indexOf >= 0)
                                       {
                                           foundFrames.append(*citer);

                                           if(areWeWaitingForAllMsgIds)
                                           {
                                               waitingFrames.removeAt(indexOf);
                                           }
                                           else
                                           {
                                               // We only wait for one element, we receive it;
                                               // therefore nothing more is waiting
                                               waitingFrames.clear();
                                               break;
                                           }
                                       }
                                   }
                               });

    // We call the process method if not null
    if(process != nullptr && !(*process)())
    {
        disconnect(waitingConn);
        return {};
    }

    if(!WaitHelper::pseudoWait([&waitingFrames]() { return waitingFrames.isEmpty(); }, timeoutInMs))
    {
        qWarning() << "A problem occurred when waiting for the received of a specific CAN message "
                   << "after processing";
        disconnect(waitingConn);
        return {};
    }

    disconnect(waitingConn);
    return foundFrames;
}
