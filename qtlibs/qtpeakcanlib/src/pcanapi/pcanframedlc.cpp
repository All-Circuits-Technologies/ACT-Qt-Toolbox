// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pcanframedlc.hpp"

const QHash<PCanFrameDlc::Enum, qint32> PCanFrameDlc::DlcSizes = {
    { PCanFrameDlc::Dlc0,  0 },
    { PCanFrameDlc::Dlc1,  1 },
    { PCanFrameDlc::Dlc2,  2 },
    { PCanFrameDlc::Dlc3,  3 },
    { PCanFrameDlc::Dlc4,  4 },
    { PCanFrameDlc::Dlc5,  5 },
    { PCanFrameDlc::Dlc6,  6 },
    { PCanFrameDlc::Dlc7,  7 },
    { PCanFrameDlc::Dlc8,  8 },
    { PCanFrameDlc::Dlc12, 12 },
    { PCanFrameDlc::Dlc16, 16 },
    { PCanFrameDlc::Dlc20, 20 },
    { PCanFrameDlc::Dlc24, 24 },
    { PCanFrameDlc::Dlc32, 32 },
    { PCanFrameDlc::Dlc48, 48 },
    { PCanFrameDlc::Dlc64, 64 },
};


PCanFrameDlc::Enum PCanFrameDlc::parseFromByte(quint8 byte)
{
    if(byte >= _LastValue)
    {
        return Unknown;
    }

    return static_cast<PCanFrameDlc::Enum>(byte);
}

quint8 PCanFrameDlc::toByte(Enum value)
{
    if(value == _LastValue || value == Unknown)
    {
        // This is used as default value when we don't know the value
        return Dlc0;
    }

    return value;
}

PCanFrameDlc::Enum PCanFrameDlc::parseFromSize(qint32 size)
{
    return DlcSizes.key(size, Unknown);
}

qint32 PCanFrameDlc::toSize(Enum value)
{
    return DlcSizes.value(value, UnknownSize);
}
