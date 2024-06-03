// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "intelhexdevice.hpp"

#include <array>
#include <QFile>
#include <QDebug>

#include "hexline.hpp"


IntelHexDevice::IntelHexDevice(QObject *parent) : QObject(parent)
{
}

IntelHexDevice::~IntelHexDevice()
{
    if(_hasDeviceOwnership)
    {
        delete _hexDevice;
    }
}

bool IntelHexDevice::linkToHexFile(const QString &fileName)
{
    if(!QFile::exists(fileName))
    {
        qWarning() << "The file doesn't exist: " << fileName << ", can't parse it as hex file";
        return false;
    }

    return linkToHexDevice(fileName, new QFile(fileName, this));
}

bool IntelHexDevice::linkToHexDevice(const QString &deviceName,
                              QIODevice *device,
                              bool giveDeviceOwnership)
{
    if(device == nullptr)
    {
        qWarning() << "Can't link a null device to the class";
        return false;
    }

    if(device->isSequential())
    {
        qWarning() << "Can't link to a sequential device";

        if(giveDeviceOwnership)
        {
            delete device;
        }

        return false;
    }

    if(_hexDevice != nullptr)
    {
        if(_hasDeviceOwnership)
        {
            delete _hexDevice;
        }

        qDeleteAll(_fileLines);
        _fileLines.clear();
    }

    _hasDeviceOwnership = giveDeviceOwnership;
    _deviceName = deviceName;
    _hexDevice = device;

    return true;
}

bool IntelHexDevice::loadDevice()
{
    if(_hexDevice == nullptr)
    {
        qWarning() << "There is no device linked, we can't load an unknown device";
        return false;
    }

    if(_fileLines.length() > 0)
    {
        // File is already loaded, do nothing
        return true;
    }

    if(!_hexDevice->isOpen() && !_hexDevice->open(QIODevice::ReadOnly))
    {
        qWarning() << "Failed to open the device: " << _deviceName;
        return false;
    }

    bool success = readAllDataFromDevice();

    if(!_fileLines.isEmpty() && _fileLines.last()->getRecordType() != RecordType::EndOfFile)
    {
        qWarning() << "The last element is not an End of file line, can't process file";
        success = false;
    }

    if(!success)
    {
        qWarning() << "A problem occured when trying to load the hex device lines: " << _deviceName;
        qDeleteAll(_fileLines);
        _fileLines.clear();
    }

    _hexDevice->close();
    return success;
}

bool IntelHexDevice::getData(quint16 address, int length, QByteArray &data)
{
    data.clear();

    if(length > std::numeric_limits<quint16>::max())
    {
        qWarning() << "The length overflows what can be read: " << length
                   << ", it must be a quint16";
        return false;
    }

    if(_hexDevice == nullptr)
    {
        qWarning() << "There is no device linked, in order to read the wanted data at address: "
                   << address;
        return false;
    }

    if(_fileLines.isEmpty())
    {
        qWarning() << "The intel hex device: " << _deviceName << ", hasn't been loaded in memory";
        return false;
    }

    data.reserve(length);

    bool success = actOnLines(address,
                              length,
                              [&data](
                                  quint16 addrToActWith,
                                  quint16 sizeToActWith,
                                  int lineIdx,
                                  HexLine *line)
    {
        Q_UNUSED(lineIdx)

        QByteArray tmp;
        if(!line->getData(addrToActWith, static_cast<quint8>(sizeToActWith), tmp))
        {
            qWarning() << "A problem occured when tryied to read the data from the lin";
            return false;
        }

        data.append(tmp);
        return true;
    });

    if(!success)
    {
        data.clear();
    }

    return success;
}

bool IntelHexDevice::setData(quint16 address, const QByteArray &data, bool flushToDevice)
{
    if(_hexDevice == nullptr)
    {
        qWarning() << "There is no device linked, in order to write the given data at address: "
                   << address;
        return false;
    }

    if(_fileLines.isEmpty())
    {
        qWarning() << "The intel hex device: " << _deviceName << ", hasn't been loaded in memory";
        return false;
    }

    int firstLineIdx = -1;
    int lastLineIdx = -1;

    bool result = actOnLines(
                      address,
                      data.length(),
                      [&data, &address, &flushToDevice, &firstLineIdx, &lastLineIdx](
                          quint16 addrToActWith,
                          quint16 sizeToActWith,
                          int lineIdx,
                          HexLine *line)
    {
        if(!line->setData(addrToActWith, data.mid(addrToActWith - address, sizeToActWith)))
        {
            qWarning() << "A problem occured when tryied to set data, with address: " << address
                       << ", and datasize: " << data.length();
            return false;
        }

        if(!flushToDevice)
        {
            // No need to go after
            return true;
        }

        if(firstLineIdx == -1 || lineIdx < firstLineIdx)
        {
            firstLineIdx = lineIdx;
        }

        if(lineIdx > lastLineIdx)
        {
            lastLineIdx = lineIdx;
        }

        return true;
    });

    if(!result || !flushToDevice || firstLineIdx == -1)
    {
        // No need to go after
        return result;
    }

    return writeLinesToDevice(firstLineIdx, lastLineIdx);
}

int IntelHexDevice::indexOfEndOfLine(const QString &buffer, int fromIndex)
{
    if(!_endOfLine.isEmpty())
    {
        return buffer.indexOf(_endOfLine, fromIndex);
    }

    int endOfLineIdx = findEndOfLine(buffer, crLf, fromIndex);

    if(endOfLineIdx > 0)
    {
        return endOfLineIdx;
    }

    endOfLineIdx = findEndOfLine(buffer, lineFeed, fromIndex);

    if(endOfLineIdx > 0)
    {
        return endOfLineIdx;
    }

    return findEndOfLine(buffer, carriageReturn, fromIndex);
}

int IntelHexDevice::findEndOfLine(const QString &buffer, const QString &expectedEndOfLine, int fromIndex)
{
    int endOfLineIdx = buffer.indexOf(expectedEndOfLine, fromIndex);

    if(endOfLineIdx > 0)
    {
        _endOfLine = expectedEndOfLine;
    }

    return endOfLineIdx;
}

bool IntelHexDevice::readAllDataFromDevice()
{
    QString tmpBuffer("");
    std::array<char, bufferSize> buf{};
    int readData = static_cast<int>(_hexDevice->read(buf.data(), bufferSize));

    while(readData > 0)
    {
        // Because the buffer size is a qint32, readData can't be upper than qint32::max(), that's
        // why can do the cast without testing the limits
        QString bufStr = QString::fromLatin1(buf.data(), readData);

        int lastEndOfLineIdx = 0;
        int endOfLineIdx = indexOfEndOfLine(bufStr);

        while(endOfLineIdx > 0 || ((readData < bufferSize) && (lastEndOfLineIdx < readData)))
        {
            tmpBuffer += bufStr.midRef(lastEndOfLineIdx, endOfLineIdx - lastEndOfLineIdx);

            HexLine *line = HexLine::parseHexLine(tmpBuffer, this);

            if(line == nullptr)
            {
                qWarning() << "An error occured when trying to parse the line: "
                           << _fileLines.length();
                return false;
            }

            _fileLines.append(line);
            tmpBuffer.clear();

            // Continue to search for the end of line
            lastEndOfLineIdx = endOfLineIdx + _endOfLine.length();
            endOfLineIdx = indexOfEndOfLine(bufStr, lastEndOfLineIdx);
        }

        if(lastEndOfLineIdx < readData)
        {
            // If we are here it means that we have a none complete line
            // Therefore we add it to tmpBuffer
            tmpBuffer = bufStr.mid(lastEndOfLineIdx);
        }

        // Continue the reading
        readData = static_cast<int>(_hexDevice->read(buf.data(), bufferSize));
    }

    if (readData == -1)
    {
        qWarning() << "An error occured when tryied to read the device content: " << _deviceName;
        return false;
    }

    return true;
}

bool IntelHexDevice::actOnLines(quint16 address,
                         int length,
                         const std::function<bool (quint16 addrToActWith,
                                                   quint16 sizeToActWith,
                                                   int lineIdx,
                                                   HexLine *line)> &actOnData)
{
    quint16 lastAddress = address + (static_cast<quint16>(length) - 1);
    quint16 nextAddress = address;
    int lineIdx = 0;

    while(nextAddress <= lastAddress)
    {
        HexLine *line = findLine(nextAddress, lineIdx, lineIdx);

        if(line == nullptr)
        {
            qWarning() << "We can't find a line which contains the address: " << nextAddress;
            return false;
        }

        quint16 size;

        if(line->getLastAddressMsbFirst() > lastAddress)
        {
            // It means that the line contains the end of the wanted data
            size = (lastAddress + 1) - nextAddress;
        }
        else
        {
            size = (line->getLastAddressMsbFirst() + 1) - nextAddress;
        }

        if(!actOnData(nextAddress, size, lineIdx, line))
        {
            qWarning() << "A problem occured when acting with the data on the line: "
                       << (lineIdx + 1);
            return false;
        }

        nextAddress += size;
    }

    return true;
}

HexLine *IntelHexDevice::findLine(quint16 address, int &lineIdx, int fromLineIdx)
{
    int idx = fromLineIdx;
    int linesLength = _fileLines.length();
    int lastIdx = (fromLineIdx == 0) ? (linesLength - 1) : (fromLineIdx - 1);

    while(idx != lastIdx)
    {
        HexLine *line = _fileLines[idx];
        if(line->isContained(address))
        {
            lineIdx = idx;
            return line;
        }

        idx++;

        if(idx == linesLength)
        {
            idx = 0;
        }
    }

    lineIdx = 0;
    return nullptr;
}

bool IntelHexDevice::writeLinesToDevice(int firstLineIdx, int lastLineIdx)
{
    qint64 pos;
    if(!calculateFirstPos(firstLineIdx, pos))
    {
        return false;
    }

    if(!_hexDevice->open(QIODevice::ReadWrite))
    {
        qWarning() << "Can't open the intel hex device: " << _deviceName << ", in write "
                   << "only mode";
        return false;
    }

    if(!_hexDevice->seek(pos))
    {
        qWarning() << "Can't go to the expected position: " << pos << ", in order to write data "
                   << "in device: " << _deviceName;
        _hexDevice->close();
        return false;
    }

    for(int idx = firstLineIdx; idx <= lastLineIdx; ++idx)
    {
        HexLine *hexLine = _fileLines.at(idx);
        QString toWrite;
        if(!hexLine->toHexLine(toWrite))
        {
            qWarning() << "An error occured when tried to get string representation of the hex "
                       << "line needed at line: " << (idx + 1);
            _hexDevice->close();
            return false;
        }

        toWrite += _endOfLine;

        if(_hexDevice->write(toWrite.toLatin1()) != toWrite.length())
        {
            qWarning() << "A problem occured when tried to write the hex line: " << (idx + 1);
            _hexDevice->close();
            return false;
        }
    }

    _hexDevice->close();
    return true;
}

bool IntelHexDevice::calculateFirstPos(int lineIdx, qint64 &pos)
{
    pos = 0;

    for(int idx = 0; idx < lineIdx; ++idx)
    {
        int tmpSize = 0;
        if(!_fileLines.at(idx)->getStringSize(tmpSize))
        {
            qWarning() << "A problem occured can't get the position of the wanted line: "
                       << lineIdx + 1;
            return false;
        }

        pos += tmpSize + _endOfLine.length();
    }

    return true;
}
