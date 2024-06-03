// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "hexline.hpp"

#include <QDebug>

#include "byteutility/bytearrayhelper.hpp"
#include "hexchecksumhelper.hpp"


HexLine::HexLine(QObject *parent) : QObject(parent)
{
}

HexLine::HexLine(quint16 addressMsbFirst,
                         RecordType::Enum recordType,
                         const QByteArray &data,
                         QObject *parent) :
    QObject(parent),
    _addressMsbFirst(addressMsbFirst),
    _recordType(recordType),
    _data(data)
{
    calculateLastAddress();
    _valid = HexChecksumHelper::calculateChecksum(addressMsbFirst, recordType, data, _checksum);
}

bool HexLine::isContained(quint16 addressMsbFirst) const
{
    return _valid &&
           (_recordType == RecordType::Data) &&
            (_addressMsbFirst <= addressMsbFirst) && (addressMsbFirst <= _lastAddressMsbFirst);
}

bool HexLine::getData(quint16 addressMsbFirst, quint8 size, QByteArray &data) const
{
    if(!_valid)
    {
        qWarning() << "Can't read data at address: 0x" << QString::number(addressMsbFirst,
                                                                          HexLine::HexBase)
                   << " of an invalid line";
        return false;
    }

    if(!isContained(addressMsbFirst))
    {
        qWarning() << "The begin address: 0x" << QString::number(addressMsbFirst,
                                                                 HexLine::HexBase)
                   << ", isn't contained in the hex file line, address: 0x"
                   << QString::number(_addressMsbFirst, HexLine::HexBase) << ", size: "
                   << _data.length() << ", can't read the line";
        return false;
    }

    quint16 lastAddr = addressMsbFirst + (size -1);

    if(!isContained(lastAddr))
    {
        qWarning() << "The last address: 0x" << QString::number(lastAddr, HexLine::HexBase)
                   << ", isn't contained in the hex "
                   << "file line, address: 0x"
                   << QString::number(_addressMsbFirst, HexLine::HexBase) << ", size: "
                   << _data.length() << ", can't read the line";
        return false;
    }

    int relativeIdx = addressMsbFirst - _addressMsbFirst;
    data = _data.mid(relativeIdx, size);

    return true;
}

bool HexLine::setData(quint16 addressMsbFirst, const QByteArray &data)
{
    if(!_valid)
    {
        qWarning() << "Can't write data at address: 0x"
                   << QString::number(addressMsbFirst, HexLine::HexBase) << " of an invalid line";
        return false;
    }

    if(!isContained(addressMsbFirst))
    {
        qWarning() << "The begin address: 0x" << QString::number(addressMsbFirst, HexLine::HexBase)
                   << ", isn't contained in the hex file line, address: 0x"
                   << QString::number(_addressMsbFirst, HexLine::HexBase) << ", size: "
                   << _data.length() << ", can't write the line";
        return false;
    }

    quint16 lastAddr = addressMsbFirst + (static_cast<quint16>(data.length()) -1);

    if(!isContained(lastAddr))
    {
        qWarning() << "The last address: 0x" << QString::number(lastAddr, HexLine::HexBase)
                   << ", isn't contained in the hex "
                   << "file line, address: 0x"
                   << QString::number(_addressMsbFirst, HexLine::HexBase) << ", size: "
                   << _data.length() << ", can't write the line";
        return false;
    }

    int relativeIdx = addressMsbFirst - _addressMsbFirst;
    for(int idx = 0; idx < data.length(); ++idx)
    {
        _data[idx + relativeIdx] = data.at(idx);
    }

    return recalculateChecksum();
}

bool HexLine::toHexLine(QString &line)
{
    if(!_valid)
    {
        qWarning() << "Can't generate the string line, the HexFileLine is invalid";
        return false;
    }

    QByteArray tmp;
    if(!toHexLine(tmp))
    {
        return false;
    }

    line = ByteArrayHelper::toStringHex(tmp, false);
    line.prepend(StartCodeChar);

    return true;
}

bool HexLine::toHexLine(QByteArray &line)
{
    if(!_valid)
    {
        qWarning() << "Can't generate the hex line, the HexFileLine is invalid";
        return false;
    }

    line.clear();
    line.reserve(FixedBytesNbInLine + _data.length());

    line[ByteNbIndex] = static_cast<qint8>(_data.length());
    line[AddressIndex] = static_cast<qint8>(
                             EndianessHelper::getByteFromLsbToMsb(_addressMsbFirst, 1));
    line[AddressIndex + 1] = static_cast<qint8>(
                             EndianessHelper::getByteFromLsbToMsb(_addressMsbFirst, 0));
    line[RecordTypeIndex] = static_cast<qint8>(_recordType);
    line.append(_data);
    line.append(static_cast<qint8>(_checksum));

    return true;
}

bool HexLine::getStringSize(int &size)
{
    if(!_valid)
    {
        qWarning() << "Can't get the string size, the HexFileLine is invalid";
        size = 0;
        return false;
    }

    // We multiplicate by 2 because a string needs two char to display a byte
    // And we add one because the start code is written on one byte
    size = ((FixedBytesNbInLine + _data.length()) * 2) + 1;

    return true;
}

HexLine *HexLine::createEndOfLineFile(QObject *parent)
{
    return new HexLine(0, RecordType::EndOfFile, QByteArray(), parent);
}

HexLine *HexLine::parseHexLine(const QString &line, QObject *parent)
{
    if(line.at(0) != StartCodeChar)
    {
        qWarning() << "The line given doesn't start with the expected start code character: '"
                   << StartCodeChar << "'";
        return nullptr;
    }

    QByteArray lineBytes;

    if(!ByteArrayHelper::parseFromHex(line.rightRef(line.length() - 1), lineBytes))
    {
        qWarning() << "The line given, from the intel hex file isn't full of bytes: " << line
                   << " (The start code character isn't parsed)";
        return nullptr;
    }

    return parseHexLine(lineBytes, parent);
}

HexLine *HexLine::parseHexLine(const QByteArray &bytesLine, QObject *parent)
{
    // First get the byte number
    quint8 bytesNb = static_cast<quint8>(bytesLine.at(ByteNbIndex));

    int realDataNb = bytesLine.length() - FixedBytesNbInLine;
    if(realDataNb != bytesNb)
    {
        qWarning() << "The bytes nb written in the line: " << bytesNb << ", doesn't match the "
                   << "real data number in the line: " << realDataNb;
        return nullptr;
    }

    quint16 address = EndianessHelper::setBytesFromLsbToMsb(
                          static_cast<quint8>(bytesLine.at(AddressIndex + 1)),
                          static_cast<quint8>(bytesLine.at(AddressIndex)));

    RecordType::Enum recordType = RecordType::parseFromNumber(
                            static_cast<quint8>(bytesLine.at(RecordTypeIndex)));

    if(recordType == RecordType::Unknown)
    {
        qWarning() << "The record type: " << static_cast<quint8>(bytesLine.at(RecordTypeIndex))
                   << " isn't managed in this version of the app";
        return nullptr;
    }

    QByteArray data = bytesLine.mid(DataIndex, realDataNb);

    quint8 checksum = static_cast<quint8>(bytesLine.at(bytesLine.length() - 1));

    HexLine *line = new HexLine(address, recordType, data, parent);

    if(!line->_valid)
    {
        qWarning() << "A problem occured with the checksum calculation of the line: " << bytesLine;
        delete line;
        return nullptr;
    }

    if(checksum != line->_checksum)
    {
        qWarning() << "The calculated checksum: 0x"
                   << QString::number(line->_checksum, HexLine::HexBase)
                   << ", isn't equal to the given checksum: 0x"
                   << QString::number(checksum, HexLine::HexBase)
                   << ", there is a problem with the line: "
                   << bytesLine;
        delete line;
        return nullptr;
    }

    return line;
}

bool HexLine::recalculateChecksum()
{
    _valid = HexChecksumHelper::calculateChecksum(_addressMsbFirst, _recordType, _data, _checksum);
    return _valid;
}

void HexLine::calculateLastAddress()
{
    int dataLength = _data.length();

    if(dataLength > 0)
    {
        _lastAddressMsbFirst = _addressMsbFirst + (static_cast<quint16>(dataLength) - 1);
    }
    else
    {
        _lastAddressMsbFirst = 0;
    }
}
