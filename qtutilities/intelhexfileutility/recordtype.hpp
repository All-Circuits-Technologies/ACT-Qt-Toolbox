// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Represents the record type in a hex line */
class RecordType : public QObject
{
    Q_OBJECT

    public:
        enum Enum: quint8
        {
            Data        = 0x00,
            EndOfFile   = 0x01,
            Unknown     = std::numeric_limits<quint8>::max()
        };
        Q_ENUM(Enum)

    public:
        /** @brief Parse the RecordType from number
            @param value The value to parse
            @return The enum parsed from number */
        static Enum parseFromNumber(quint8 value);
};
