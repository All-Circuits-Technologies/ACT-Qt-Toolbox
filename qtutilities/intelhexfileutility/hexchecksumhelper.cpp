// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "hexchecksumhelper.hpp"

#include <QDebug>

#include "byteutility/endianesshelper.hpp"
#include "byteutility/mathhelper.hpp"


bool HexChecksumHelper::calculateChecksum(quint16 addressMsbFirst,
                                         RecordType::Enum recordType,
                                         const QByteArray &data,
                                         quint8 &checksum)
{
    if(recordType == RecordType::Unknown)
    {
        qWarning() << "The type of the record is unknown, can't calculate the checksum with it";
        checksum = 0;
        return false;
    }

    int dataSize = data.length();

    if(dataSize > std::numeric_limits<quint8>::max())
    {
        qWarning() << "The size of data: " << dataSize << ", are overflowing the lenght of a "
                   << "hex file: " << std::numeric_limits<quint8>::max();
        checksum = 0;
        return false;
    }

    // The adding of all the elements can't overflow a quint32, cause:
    //  - You can't have more than 0xFF(255) bytes on a line, each byte can't be no more than 0xFF.
    //    Therefore in the worst case, all the bytes data added are equals to:
    //          0xFF * 0xFF = 0xFE01 (65 025)
    //  - We also add the byte (for the data count), two bytes for the address and one byte for the
    //    the record type => (1 + 2 + 1) * 0xFF = 0x03FC
    // Therefore, we have 4 bytes on top of the bytes dat => 0xFE01 + 0x03FC = 0x0101FD (which is
    // lesser than a quint32 but greather than a quint16
    quint32 checksumComputation = static_cast<quint8>(dataSize) +
                        EndianessHelper::getByteFromLsbToMsb(addressMsbFirst, 1) +
                        EndianessHelper::getByteFromLsbToMsb(addressMsbFirst, 0) +
                        recordType;

    QByteArray::const_iterator citer = data.cbegin();

    for(; citer != data.cend(); ++citer)
    {
        checksumComputation += static_cast<quint8>(*citer);
    }

    // The checksum is equal to the two's complement of the line count LSB value
    quint8 checksumComputationLsbValue = EndianessHelper::getByteFromLsbToMsb(checksumComputation,
                                                                              0);

    checksum = MathHelper::twosComplement(checksumComputationLsbValue);
    return true;
}
