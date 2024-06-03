// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "canbusframehelper.hpp"

#include <QCanBusFrame>
#include <QDebug>

#include "byteutility/bytearrayhelper.hpp"


CanBusFrameHelper::CanBusFrameHelper()
{
}

QString CanBusFrameHelper::stringifyFrame(const QCanBusFrame &frame)
{
    QString id = QString::number(frame.frameId(), 16).toUpper();
    QString length = QString::number(frame.payload().size());

    QString payload;
    ByteArrayHelper::convertToStringHex(frame.payload(), payload, false);

    return QString(CanBusFormat).arg(id, length, payload);
}

bool CanBusFrameHelper::parseStrFrame(const QString &strFrame, QCanBusFrame &frame)
{
    const QStringList frameParts = strFrame.split(StrSeparator);

    if(frameParts.length() != PartsNb)
    {
        qWarning() << "We were trying to parse the stringified CAN frame: " << strFrame << ", but "
                   << "the parts number of the frame doesn't match the expected one: " << PartsNb;
        return false;
    }

    QByteArray hexId;
    if(!ByteArrayHelper::parseFromHex(frameParts[IdPartIdx], hexId))
    {
        qWarning() << "We were trying to parse the stringified CAN frame: " << strFrame << ", but "
                   << "the id part can't be parsed";
        return false;
    }

    quint32 id;
    if(!ByteArrayHelper::toUintHex(hexId, id))
    {
        qWarning() << "We were trying to parse the stringified CAN frame: " << strFrame << ", but "
                   << "the id part can't be converted to a quint32";
        return false;
    }

    frame.setFrameId(id);

    bool ok = false;
    const int payloadSize = frameParts[LengthPartIdx].toInt(&ok);

    if(!ok)
    {
        qWarning() << "We were trying to parse the stringified CAN frame: " << strFrame << ", but "
                   << "the length part can't be converted to a qint32";
        return false;
    }

    QByteArray payload(payloadSize, 0x00);
    if(!ByteArrayHelper::parseFromHex(frameParts[PayloadPartIdx], payload))
    {
        qWarning() << "We were trying to parse the stringified CAN frame: " << strFrame << ", but "
                   << "the payload part can't be parsed";
        return false;
    }

    if(payload.size() != payloadSize)
    {
        qWarning() << "We were trying to parse the stringified CAN frame: " << strFrame << ", but "
                   << "the payload size isn't the expecting one";
        return false;
    }

    frame.setPayload(payload);

    return true;
}

void CanBusFrameHelper::copyCanBusFrame(const QCanBusFrame &source, QCanBusFrame &target)
{
    target.setBitrateSwitch(source.hasBitrateSwitch());
    target.setError(source.error());
    target.setErrorStateIndicator(source.hasErrorStateIndicator());
    target.setExtendedFrameFormat(source.hasExtendedFrameFormat());
    target.setFlexibleDataRateFormat(source.hasFlexibleDataRateFormat());
    target.setFrameId(source.frameId());
    target.setFrameType(source.frameType());
    target.setLocalEcho(source.hasLocalEcho());
    target.setPayload(source.payload());
    target.setTimeStamp(source.timeStamp());
}
