// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "bytearrayhelper.hpp"

#include <QDebug>
#include <QString>
#include <QStringRef>

const QString ByteArrayHelper::hexPrefixC = QString("0x");
const QString ByteArrayHelper::hexPrefixTi = QString("0h");
const QString ByteArrayHelper::hexSuffix = QString("h");


bool ByteArrayHelper::parseFromHex(const QString &hex, QByteArray &byteArray)
{
    return parseFromHex(QStringRef(&hex), byteArray);
}

bool ByteArrayHelper::parseFromHex(const QStringRef &hex, QByteArray &byteArray)
{
    QByteArray utf8;

    QStringRef ref;
    int usableLength;

    if(extractHexPart(hex, true, hexPrefixC, ref) ||
        extractHexPart(hex, true, hexPrefixTi, ref) ||
        extractHexPart(hex, false, hexSuffix, ref))
    {
        utf8 = ref.toUtf8();
        usableLength= utf8.length();
    }
    else
    {
        // Convert all to utf8
        utf8 = hex.toUtf8();
        usableLength = utf8.length();
    }

    byteArray = QByteArray::fromHex(utf8);

    // Divide the usable length by 2 in order to have the right length of hex
    if(byteArray.length() != ((usableLength + 1) / 2))
    {
        byteArray.clear();
        qWarning() << "The given string: " << hex << ", can't be parsed to hexadecimal byte array";
        return false;
    }

    return true;
}

void ByteArrayHelper::convertToStringHex(const QByteArray &byteArray,
                                         QString &hex,
                                         bool prependSuffix,
                                         int atLeastByteNb)
{
    int arrayLength = byteArray.length();

    hex = QString(prependSuffix ? hexPrefixC : "");

    if(atLeastByteNb > arrayLength)
    {
        int zerosPairToAdd = atLeastByteNb - arrayLength;

        hex.append(QString(zerosPairToAdd * 2, '0'));
    }

    hex.append(byteArray.toHex());
}

QString ByteArrayHelper::toStringHex(const QByteArray &byteArray,
                                     bool prependSuffix,
                                     int atLeastByteNb)
{
    QString hexStr;
    convertToStringHex(byteArray, hexStr, prependSuffix, atLeastByteNb);
    return hexStr;
}

bool ByteArrayHelper::toUintHex(const QByteArray &byteArray, quint16 &hexValue, bool lsbFirst)
{
    QVector<quint8> list;
    if(!fillByteListLsbFirst(byteArray, lsbFirst, EndianessHelper::ByteNbInUint16, list))
    {
        qWarning() << "The given byte array can't be set in a quint16";
        hexValue = 0;
        return false;
    }

    hexValue = EndianessHelper::setBytesFromLsbToMsb(list[0],
                                                     list[1]);
    return true;
}

bool ByteArrayHelper::toUintHex(const QByteArray &byteArray, quint32 &hexValue, bool lsbFirst)
{
    QVector<quint8> list;
    if(!fillByteListLsbFirst(byteArray, lsbFirst, EndianessHelper::ByteNbInUint32, list))
    {
        qWarning() << "The given byte array can't be set in a quint32";
        hexValue = 0;
        return false;
    }

    hexValue = EndianessHelper::setBytesFromLsbToMsb(list[0],
                                                     list[1],
                                                     list[2],
                                                     list[3]);
    return true;
}

bool ByteArrayHelper::toUintHex(const QByteArray &byteArray, quint64 &hexValue, bool lsbFirst)
{
    QVector<quint8> list;
    if(!fillByteListLsbFirst(byteArray, lsbFirst, EndianessHelper::ByteNbInUint64, list))
    {
        qWarning() << "The given byte array can't be set in a quint64";
        hexValue = 0;
        return false;
    }

    hexValue = EndianessHelper::setBytesFromLsbToMsb(list[0],
                                                     list[1],
                                                     list[2],
                                                     list[3],
                                                     list[4],
                                                     list[5],
                                                     list[6],
                                                     list[7]);
    return true;
}

bool ByteArrayHelper::calculateMaxUnsignedValue(quint8 byteCount, quint64 &maxUnsignedValue)
{
    if(byteCount > EndianessHelper::ByteNbInUint64)
    {
        qWarning() << "The byte size: " << byteCount << ", overflows the quint64 max size";
        maxUnsignedValue = 0;
        return false;
    }

    if(byteCount == EndianessHelper::ByteNbInUint64)
    {
        maxUnsignedValue = std::numeric_limits<quint64>::max();
        return true;
    }

    maxUnsignedValue = ((1ULL << (8 * byteCount)) - 1);

    return true;
}

QByteArray ByteArrayHelper::operatorOr(const QByteArray &a1, const QByteArray &a2)
{
    return operatorOr(QVector<QByteArray>{a1, a2}, {0, 0}).first;
}

QPair<QByteArray, quint8> ByteArrayHelper::operatorOr(const QByteArray &a1,
                                                      const QByteArray &a2,
                                                      quint8 offsetA1,
                                                      quint8 offsetA2)
{
    return operatorOr(QVector<QByteArray>{a1, a2}, {offsetA1, offsetA2});
}

QPair<QByteArray, quint8> ByteArrayHelper::operatorOr(const QVector<QByteArray> &aList,
                                                      const QVector<quint8> &offsetAList)
{
    QPair<QByteArray, quint8> final;

    // Because the operator method always return true, the logic operator method will always
    // return true, no need to test its result
    logicOperator(aList,
        offsetAList,
        0x00u,
        [](quint8 a1, quint8 a2, quint8 &result) {
            result = a1 | a2;
            return true;
        },
        &final);
    return final;
}

QByteArray ByteArrayHelper::operatorXor(const QByteArray &a1, const QByteArray &a2)
{
    return operatorXor(QVector<QByteArray>{a1, a2}, {0, 0}).first;
}

QPair<QByteArray, quint8> ByteArrayHelper::operatorXor(const QByteArray &a1,
                                                       const QByteArray &a2,
                                                       quint8 offsetA1,
                                                       quint8 offsetA2)
{
    return operatorXor(QVector<QByteArray>{a1, a2}, {offsetA1, offsetA2});
}

QPair<QByteArray, quint8> ByteArrayHelper::operatorXor(const QVector<QByteArray> &aList,
                                                       const QVector<quint8> &offsetAList)
{
    QPair<QByteArray, quint8> final;

    // Because the operator method always return true, the logic operator method will always
    // return true, no need to test its result
    logicOperator(
        aList,
        offsetAList,
        0x00u,
        [](quint8 a1, quint8 a2, quint8 &result) {
            result = a1 ^ a2;
            return true;
        },
        &final);

    return final;
}

QByteArray ByteArrayHelper::operatorAnd(const QByteArray &a1, const QByteArray &a2)
{
    return operatorAnd(QVector<QByteArray>{a1, a2}, {0, 0}).first;
}

QPair<QByteArray, quint8> ByteArrayHelper::operatorAnd(const QByteArray &a1,
                                                       const QByteArray &a2,
                                                       quint8 offsetA1,
                                                       quint8 offsetA2)
{
    return operatorAnd(QVector<QByteArray>{a1, a2}, {offsetA1, offsetA2});
}

QPair<QByteArray, quint8> ByteArrayHelper::operatorAnd(const QVector<QByteArray> &aList,
                                                       const QVector<quint8> &offsetAList)
{
    QPair<QByteArray, quint8> final;

    // Because the operator method always return true, the logic operator method will always
    // return true, no need to test its result
    logicOperator(
        aList,
        offsetAList,
        0xFFu,
        [](quint8 a1, quint8 a2, quint8 &result) {
            result = a1 & a2;
            return true;
        },
        &final);

    return final;
}

QByteArray ByteArrayHelper::operatorNot(const QByteArray &byteArray)
{
    QByteArray final;
    final.reserve(byteArray.length());

    for(int idx = 0; idx < byteArray.length(); ++idx)
    {
        final[idx] = ~byteArray.at(idx);
    }

    return final;
}

bool ByteArrayHelper::testIfCollidingBits(const QByteArray &a1, const QByteArray &a2)
{
    return testIfCollidingBits(QVector<QByteArray>{ a1, a2 }, { 0, 0 });
}

bool ByteArrayHelper::testIfCollidingBits(const QByteArray &a1,
                                          const QByteArray &a2,
                                          quint8 offsetA1,
                                          quint8 offsetA2)
{
    return testIfCollidingBits(QVector<QByteArray>{ a1, a2 }, { offsetA1, offsetA2 });
}

bool ByteArrayHelper::testIfCollidingBits(const QVector<QByteArray> &aList,
                                          const QVector<quint8> &offsetAList)
{
    return !logicOperator(
        aList,
        offsetAList,
        0x00u,
        [](quint8 a1, quint8 a2, quint8 &result)
        {
            result = a1 | a2;
            return (result == (a1 ^ a2));
        });
}

bool ByteArrayHelper::equalsToZero(const QByteArray &byteArray)
{
    for(auto citer = byteArray.cbegin(); citer != byteArray.cend(); ++citer)
    {
        if((*citer) != 0x00)
        {
            return false;
        }
    }

    return true;
}

bool ByteArrayHelper::logicOperator(
    const QVector<QByteArray> &aList,
    const QVector<quint8> &offsetAList,
    quint8 initialValue,
    const std::function<bool (quint8, quint8, quint8 &)> &operatorFunc,
    QPair<QByteArray, quint8> *finalResult)
{
    const int listLength = aList.length();
    const int offsetListLength = offsetAList.length();
    QVector<quint8> tmpOffsetList(offsetAList);
    tmpOffsetList.reserve(listLength);
    tmpOffsetList.insert(offsetListLength, listLength - offsetListLength, 0);

    const quint8 minOffset = *std::min_element(offsetAList.cbegin(), offsetAList.cend());
    QVector<int> arrayLength;
    arrayLength.reserve(listLength);
    for(int listIdx = 0; listIdx < listLength; ++listIdx)
    {
        arrayLength.append(aList.at(listIdx).length()
                           + tmpOffsetList.at(listIdx)
                           - minOffset);
    }

    const int maxLength = *std::max_element(arrayLength.cbegin(), arrayLength.cend());

    if(finalResult != nullptr)
    {
        finalResult->first.resize(maxLength);
        finalResult->second = minOffset;
    }

    for(int idx = minOffset; idx < (maxLength + minOffset); ++idx)
    {
        quint8 value = initialValue;
        for(int listIdxBis = 0; listIdxBis < listLength; ++listIdxBis)
        {
            quint8 tmpOffset = tmpOffsetList.at(listIdxBis);
            if((tmpOffset <= idx && idx < (arrayLength.at(listIdxBis) + tmpOffset)) &&
                !operatorFunc(value, aList.at(listIdxBis).at(idx - tmpOffset), value))
            {
                return false;
            }
        }

        if(finalResult != nullptr)
        {
            finalResult->first[idx - minOffset] = value;
        }
    }

    return true;
}

bool ByteArrayHelper::extractHexPart(const QStringRef &hex,
                                     bool testStart,
                                     const QString &extraElem,
                                     QStringRef &hexPart)
{
    if(testStart && hex.startsWith(extraElem, Qt::CaseInsensitive))
    {
        hexPart = hex.right(hex.length() - extraElem.length());
        return true;
    }

    if(!testStart && hex.endsWith(extraElem, Qt::CaseInsensitive))
    {
        hexPart = hex.left(hex.length() - extraElem.length());
        return true;
    }

    return false;
}

bool ByteArrayHelper::fillByteListLsbFirst(const QByteArray &byteArray,
                                           bool lsbFirst,
                                           int lengthWanted,
                                           QVector<quint8> &lsbFirstList)
{
    if(byteArray.length() > lengthWanted)
    {
        qWarning() << "The given byte array is too long: " << byteArray.length()
                   << ", according to the wanted length: " << lengthWanted;
        return false;
    }

    lsbFirstList.reserve(lengthWanted);

    QByteArray::const_iterator citer;
    if(lsbFirst)
    {
        for(citer = byteArray.constBegin(); citer != byteArray.constEnd(); ++citer)
        {
            lsbFirstList.append(static_cast<quint8>(*citer));
        }
    }
    else
    {
        citer = byteArray.constEnd();
        while(citer != byteArray.constBegin())
        {
            --citer;
            lsbFirstList.append(static_cast<quint8>(*citer));
        }
    }

    int lengthToAdd = (lengthWanted - lsbFirstList.length());
    for(int idx = 0; idx < lengthToAdd; ++idx)
    {
        lsbFirstList.append(static_cast<quint8>(0));
    }

    return true;
}
