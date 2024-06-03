// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "expectedcanframemask.hpp"

#include <QDebug>
#include <QCanBusFrame>

#include "byteutility/bytearrayhelper.hpp"


ExpectedCanFrameMask::ExpectedCanFrameMask(quint32 receivedMsgId,
                                           quint8 maskIdx,
                                           const QByteArray &mask,
                                           const QByteArray &expectedMaskResult,
                                           bool waitUntilReceiveExpected)
    : _receivedMsgId{receivedMsgId},
    _maskIdx{maskIdx},
    _mask{mask},
    _expectedMaskResult{expectedMaskResult},
    _waitUntilReceiveExpected{waitUntilReceiveExpected}
{
}

ExpectedCanFrameMask::ExpectedCanFrameMask(quint32 receivedMsgId)
    : _receivedMsgId{receivedMsgId}
{
}

ExpectedCanFrameMask::ExpectedCanFrameMask(const ExpectedCanFrameMask &copy)
    : _receivedMsgId{copy._receivedMsgId},
    _maskIdx{copy._maskIdx},
    _mask{copy._mask},
    _expectedMaskResult{copy._expectedMaskResult},
    _waitUntilReceiveExpected{copy._waitUntilReceiveExpected}
{
}

ExpectedCanFrameMask::~ExpectedCanFrameMask()
{
}

bool ExpectedCanFrameMask::checkIfMessageReceivedIsValid(const QCanBusFrame &messageReceived,
                                                         bool silent) const
{
    return checkMessageReceivedValidity(messageReceived,
                                        _maskIdx,
                                        _mask,
                                        _expectedMaskResult,
                                        silent);
}

ExpectedCanFrameMask &ExpectedCanFrameMask::operator=(const ExpectedCanFrameMask &otherElement)
{
    _receivedMsgId = otherElement._receivedMsgId;
    _maskIdx = otherElement._maskIdx;
    _mask = otherElement._mask;
    _expectedMaskResult = otherElement._expectedMaskResult;
    _waitUntilReceiveExpected = otherElement._waitUntilReceiveExpected;

    return *this;
}

QString ExpectedCanFrameMask::toString() const
{
    QString toStr;

    toStr = QString(ToStringPattern).arg(_receivedMsgId);

    if(_mask.isEmpty())
    {
        return toStr;
    }

    QString maskHex;
    QString resultHex;
    ByteArrayHelper::convertToStringHex(_mask, maskHex);
    ByteArrayHelper::convertToStringHex(_expectedMaskResult, resultHex);

    toStr = QString(ToStringPatternExtended)
                .arg(toStr)
                .arg(_maskIdx)
                .arg(maskHex, resultHex)
                .arg(_waitUntilReceiveExpected);

    return toStr;
}

int ExpectedCanFrameMask::indexOf(const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
                                  const QCanBusFrame &frame,
                                  int from)
{
    const int length = expectedFrameMasks.length();
    for(int idx = from; idx < length; ++idx)
    {
        const ExpectedCanFrameMask &expected = expectedFrameMasks[idx];
        if(expected.getReceivedMsgId() != frame.frameId())
        {
            continue;
        }

        if(expected.hasToWaitUntilReceivedExpected() &&
            !expected.checkIfMessageReceivedIsValid(frame, true))
        {
            // In that case, we haven't received the expected message and we want it
            continue;
        }

        return idx;
    }

    return -1;
}

QVector<ExpectedCanFrameMask> ExpectedCanFrameMask::convert(const QVector<quint32> &answersId)
{
    QVector<ExpectedCanFrameMask> expectedFrames;
    expectedFrames.reserve(answersId.length());

    for(auto citer = answersId.cbegin(); citer != answersId.cend(); ++citer)
    {
        expectedFrames.append(ExpectedCanFrameMask{ *citer });
    }

    return expectedFrames;
}

bool ExpectedCanFrameMask::checkMessageReceivedValidity(const QCanBusFrame &messageReceived,
                                                        quint8 maskIdx,
                                                        const QByteArray &mask,
                                                        const QByteArray &expectedMaskResult,
                                                        bool silent)
{
    const QByteArray receivedPayload = messageReceived.payload();
    const int expectedAnswerLengthFromMask = maskIdx + mask.length();

    // First check the size of the payload, with the mask size
    if(receivedPayload.length() < expectedAnswerLengthFromMask)
    {
        if(!silent)
        {
            qWarning() << "The expected mask: " << mask << ", at idx: " << maskIdx << ", isn't "
                       << "contained in the CAN answer received: " << messageReceived.toString();
        }

        return false;
    }

    for(int idx = maskIdx; idx < expectedAnswerLengthFromMask; ++idx)
    {
        const char payloadChar = receivedPayload.at(idx);
        const char maskChar = mask.at(idx - maskIdx);
        const char expectedMaskResultChar = expectedMaskResult.at(idx - maskIdx);

        if((payloadChar & maskChar) != expectedMaskResultChar)
        {
            if(!silent)
            {
                qWarning() << "The mask: " << QString::number(maskChar, 16) << ", at idx: " << idx
                           << ", applied to value: " << QString::number(payloadChar, 16)
                           << ", doesn't match the expected mask result: "
                           << QString::number(expectedMaskResultChar, 16);
            }

            return false;
        }
    }

    return true;
}

QDebug operator<<(QDebug stream, const ExpectedCanFrameMask &expected)
{
    QDebugStateSaver saver(stream);
    Q_UNUSED(saver)

    return stream.noquote() << expected.toString();
}
