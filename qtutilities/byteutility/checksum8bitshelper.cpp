// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "checksum8bitshelper.hpp"

#include <QDebug>

#include "byteutility/endianesshelper.hpp"
#include "byteutility/mathhelper.hpp"


bool Checksum8BitsHelper::calculateChecksum(const QByteArray &data, quint8 &checksum)
{
    if(data.isEmpty())
    {
        qWarning() << "Can not calculate checksum of empty byte array !";
        return false;
    }

    quint32 checksumComputation = 0;

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
