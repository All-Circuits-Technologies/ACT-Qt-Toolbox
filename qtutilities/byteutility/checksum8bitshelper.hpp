// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QtGlobal>


class Checksum8BitsHelper
{
public:
    /** @brief Calculate 8-bits checksum
        @note This 8-bit Checksum Calculator can be used to calculate the 8-bit Checksum
              of a sequence of hexadecimal values or bytes.
              8-bit Checksum is also called the 2's compliment of addition of all bytes
        @param data The data for which the checksum is calculated
        @param checksum The calculated checksum
        @return True if no problem occurs */
    static bool calculateChecksum(const QByteArray &data, quint8 &checksum);
};

