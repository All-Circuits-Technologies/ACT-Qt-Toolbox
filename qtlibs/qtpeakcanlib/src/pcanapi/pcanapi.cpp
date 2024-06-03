// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pcanapi.hpp"

#include <QCanBusFrame>
#include <QDebug>

#include "src/models/candeviceconfig.hpp"
#include "src/models/candeviceconfigdetails.hpp"
#include "src/models/candevicefdconfigdetails.hpp"
#include "src/pcanapi/pcanframedlc.hpp"

#include "import_pcanbasic.hpp"


PCanApi::PCanApi()
{
}

bool PCanApi::initializeCan(const CanDeviceConfig &config)
{
    if(!config.isValid())
    {
        qWarning() << "Impossible to initialize the CAN: the CAN device config given isn't valid";
        return false;
    }

    if(config.isCanFd())
    {
        qWarning() << "Impossible to initialize the CAN: the CAN device config given doesn't "
                   << "contain the CAN details config";
        return false;
    }

    const CanDeviceConfigDetails &configDetails = config.getDetails();

    PCanBaudRate::Enum baudRate = configDetails.getBaudRate();

    bool ok = false;
    quint16 apiCode = PCanBaudRate::toApiCode(baudRate, &ok);

    if(!ok)
    {
        qWarning() << "Impossible to initialize the CAN: the stored baudrate is unknown";
        return false;
    }

    const TPCANStatus status = CAN_Initialize(config.getPCanBusItfHandle(), apiCode);

    if(status != PCAN_ERROR_OK)
    {
        qWarning() << "A problem occurred: " << getErrorText(status) << ", when tried to "
                   << "initialize the CAN, with baud rate value: "
                   << PCanBaudRate::toRealValueString(baudRate);
        return false;
    }

    return true;
}

bool PCanApi::initializeCanFd(const CanDeviceConfig &config)
{
    if(!config.isValid())
    {
        qWarning() << "Impossible to initialize the CAN FD; the CAN device config given isn't "
                   << "valid";
        return false;
    }

    if(!config.isCanFd())
    {
        qWarning() << "Impossible to initialize the CAN FD: the CAN device config given doesn't "
                   << "contain the FD details config";
        return false;
    }

    const CanDeviceFdConfigDetails &fdConfig = config.getFdDetails();

    const QString tmpBitrateFd = QString(BitrateFd).arg(fdConfig.getFClockInMHz())
                                                   .arg(fdConfig.getNomBrp())
                                                   .arg(fdConfig.getNomTseg1())
                                                   .arg(fdConfig.getNomTseg2())
                                                   .arg(fdConfig.getNomSjw())
                                                   .arg(fdConfig.getDataBrp())
                                                   .arg(fdConfig.getDataTseg1())
                                                   .arg(fdConfig.getDataTseg2())
                                                   .arg(fdConfig.getDataSjw());

    const TPCANStatus status = CAN_InitializeFD(config.getPCanBusItfHandle(),
                                                tmpBitrateFd.toStdString().data());

    if(status != PCAN_ERROR_OK)
    {
        qWarning() << "A problem occurred: " << getErrorText(status) << ", when tried to "
                   << "initialize the CAN FD, with values: " << tmpBitrateFd;
        return false;
    }

    return true;
}

bool PCanApi::unInitializeCan(PCanBusItf::Enum pCanBusItf)
{
    if(pCanBusItf == PCanBusItf::Unknown)
    {
        qWarning() << "We can't uninitialize an unknown can bus interface";
        return false;
    }

    const TPCANStatus status = CAN_Uninitialize(PCanBusItf::toTPCanHandle(pCanBusItf));

    if(status != PCAN_ERROR_OK)
    {
        qWarning() << "A problem occurred: " << getErrorText(status) << ", when tried to "
                   << "uninitialize the CAN FD, with CAN BUS: " << PCanBusItf::toString(pCanBusItf);
        return false;
    }

    return true;
}

QString PCanApi::getErrorText(quint32 errorCode)
{
    QByteArray errorTxtData(256, 0);
    const TPCANStatus status = CAN_GetErrorText(errorCode, EnLanguageCode, errorTxtData.data());

    if(status != PCAN_ERROR_OK)
    {
        qWarning() << "A problem occurred when tried to get the error text from the PCAN lib, "
                   << "the error: " << status << ", we tried to get the error code of the error: "
                   << errorCode;
        return {};
    }

    return QString::fromLatin1(errorTxtData);
}

bool PCanApi::writeCanFdMsgProcess(PCanBusItf::Enum pCanBusItf, const QCanBusFrame &frame)
{
    const QByteArray payload = frame.payload();
    const qint32 payloadSize = payload.size();
    const PCanFrameDlc::Enum frameDlc = PCanFrameDlc::parseFromSize(payloadSize);

    if(frameDlc == PCanFrameDlc::Unknown)
    {
        qWarning() << "We can't write the CAN FD message, the payload size is not managed by the "
                      "DLC";
        return false;
    }

    TPCANMsgFD message = {};
    message.ID = frame.frameId();
    message.DLC = PCanFrameDlc::toByte(frameDlc);
    message.MSGTYPE = frame.hasExtendedFrameFormat() ? PCAN_MESSAGE_EXTENDED
                                                     : PCAN_MESSAGE_STANDARD;

    if (frame.hasFlexibleDataRateFormat())
    {
        message.MSGTYPE |= PCAN_MESSAGE_FD;
    }

    if (frame.hasBitrateSwitch())
    {
        message.MSGTYPE |= PCAN_MESSAGE_BRS;
    }

    if (frame.frameType() == QCanBusFrame::RemoteRequestFrame)
    {
        // In that case, we do not care about the payload
        message.MSGTYPE |= PCAN_MESSAGE_RTR;
    }
    else
    {
        memcpy(message.DATA, payload.constData(), payloadSize);
    }

    const TPCANStatus status = CAN_WriteFD(PCanBusItf::toTPCanHandle(pCanBusItf), &message);

    if(status != PCAN_ERROR_OK)
    {
        qWarning() << "A problem occurred when tried to write the CAN FD frame: , "
                   << frame.toString() << ", with CAN BUS: " << PCanBusItf::toString(pCanBusItf)
                   << ", the error is : " << getErrorText(status) ;
        return false;
    }

    qDebug() << "A frame has been written from FD CAN bus: " <<  PCanBusItf::toString(pCanBusItf)
             << ", the frame: " << frame.toString();

    return true;
}

bool PCanApi::writeCanMsgProcess(PCanBusItf::Enum pCanBusItf, const QCanBusFrame &frame)
{
    const QByteArray payload = frame.payload();
    const qint32 payloadSize = payload.size();

    TPCANMsg message = {};
    message.ID = frame.frameId();
    message.LEN = static_cast<quint8>(payloadSize);
    message.MSGTYPE = frame.hasExtendedFrameFormat() ? PCAN_MESSAGE_EXTENDED
                                                     : PCAN_MESSAGE_STANDARD;

    if (frame.frameType() == QCanBusFrame::RemoteRequestFrame)
    {
        // In that case, we do not care about the payload
        message.MSGTYPE |= PCAN_MESSAGE_RTR;
    }
    else
    {
        memcpy(message.DATA, payload.constData(), payloadSize);
    }

    const TPCANStatus status = CAN_Write(PCanBusItf::toTPCanHandle(pCanBusItf), &message);

    if(status != PCAN_ERROR_OK)
    {
        qWarning() << "A problem occurred when tried to write the CAN frame: , "
                   << frame.toString() << ", with CAN BUS: " << PCanBusItf::toString(pCanBusItf)
                   << ", the error is : " << getErrorText(status) ;
        return false;
    }

    qDebug() << "A frame has been written from CAN bus: " <<  PCanBusItf::toString(pCanBusItf)
             << ", the frame: " << frame.toString();

    return true;
}

QVector<CanDeviceInfo> PCanApi::getAvailableDevices()
{
    quint32 channelCount = 0;
    const TPCANStatus countStatus = CAN_GetValue(0,
                                                 PCAN_ATTACHED_CHANNELS_COUNT,
                                                 &channelCount,
                                                 sizeof(channelCount));

    if(countStatus != PCAN_ERROR_OK)
    {
        qWarning() << "A problem occurred when tried to get the attached channels count: , "
                   << getErrorText(countStatus) ;
        return {};
    }

    std::vector<TPCANChannelInformation> channelsInfo(channelCount);
    const TPCANStatus attachedStatus = CAN_GetValue(0,
                                                    PCAN_ATTACHED_CHANNELS,
                                                    channelsInfo.data(),
                                                    channelCount * sizeof(TPCANChannelInformation));

    if(attachedStatus != PCAN_ERROR_OK)
    {
        qWarning() << "A problem occurred when tried to get the attached channels infos: , "
                   << getErrorText(attachedStatus) ;
        return {};
    }

    QVector<CanDeviceInfo> devicesInfo;

    std::vector<TPCANChannelInformation>::const_iterator citer = channelsInfo.cbegin();
    for(; citer != channelsInfo.cend(); ++citer)
    {
        const PCanBusItf::Enum canBusIntf = PCanBusItf::parseFromUShort(citer->channel_handle);

        if(canBusIntf == PCanBusItf::Unknown)
        {
            qWarning() << "We tried to get the available devices and one is unknown, we continue";
            continue;
        }

        const QString deviceName = QString::fromLatin1(citer->device_name);

        const bool isCanFd = (citer->device_features & FEATURE_FD_CAPABLE);

        devicesInfo.append(CanDeviceInfo(canBusIntf, deviceName, isCanFd));
    }

    return devicesInfo;
}

bool PCanApi::getParamBusOffAutoReset(PCanBusItf::Enum pCanBusItf, bool &autoReset)
{
    return getBooleanParam(pCanBusItf, PCAN_BUSOFF_AUTORESET, autoReset);
}

bool PCanApi::setParamBusOffAutoReset(PCanBusItf::Enum pCanBusItf, bool autoReset)
{
    return setBooleanParam(pCanBusItf, PCAN_BUSOFF_AUTORESET, autoReset);
}

bool PCanApi::getBooleanParam(PCanBusItf::Enum pCanBusItf, quint8 paramType, bool &value)
{
    quint32 buffer = PCAN_PARAMETER_OFF;
    const TPCANStatus status = CAN_GetValue(PCanBusItf::toTPCanHandle(pCanBusItf),
                                            paramType,
                                            &buffer,
                                            sizeof(quint32));

    if(status != PCAN_ERROR_OK)
    {
        qWarning() << "A problem occurred when tried to get a boolean parameter: " << paramType
                   << ", from the attached channel: " << PCanBusItf::toString(pCanBusItf)
                   << ", error: " << getErrorText(status) ;
        return false;
    }

    value = (buffer != PCAN_PARAMETER_OFF);

    return true;
}

bool PCanApi::setBooleanParam(PCanBusItf::Enum pCanBusItf, quint8 paramType, bool value)
{
    quint32 buffer = value ? PCAN_PARAMETER_ON : PCAN_PARAMETER_OFF;
    const TPCANStatus status = CAN_SetValue(PCanBusItf::toTPCanHandle(pCanBusItf),
                                            paramType,
                                            &buffer,
                                            sizeof(quint32));

    if(status != PCAN_ERROR_OK)
    {
        qWarning() << "A problem occurred when tried to set a boolean parameter: " << paramType
                   << ", its value: " << value << " to the attached channel: "
                   << PCanBusItf::toString(pCanBusItf) << ", error: " << getErrorText(status) ;
        return false;
    }

    return true;
}
