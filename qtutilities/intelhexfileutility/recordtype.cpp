// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "recordtype.hpp"


RecordType::Enum RecordType::parseFromNumber(quint8 value)
{
    switch (value)
    {
        case Data:
        case EndOfFile:
        case Unknown:
            return static_cast<Enum>(value);
    }

    return Unknown;
}
