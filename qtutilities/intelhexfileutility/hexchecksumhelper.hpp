// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QtGlobal>

#include "recordtype.hpp"


/** @brief This class contains helper methods to manage checksum in intel hex file */
class HexChecksumHelper
{
    public:
        /** @brief Calculate the checksum on a line
            @note For intel hex file the checksum is particular (from Wikipedia):
                    A record's checksum byte is the two's complement of the least significant byte
                    (LSB) of the sum of all decoded byte values in the record preceding the
                    checksum. It is computed by summing the decoded byte values and extracting the
                    LSB of the sum (i.e., the data checksum), and then calculating the two's
                    complement of the LSB (e.g., by inverting its bits and adding one).

                    For example, in the case of the record :0300300002337A1E, the sum of the
                    decoded byte values is 03 + 00 + 30 + 00 + 02 + 33 + 7A = E2, which has LSB
                    value E2.
                    The two's complement of E2 is 1E, which is the checksum byte appearing at the
                    end of the record.
            @param addressMsbFirst The address written on the line (MSB first)
            @param recordType The type of line record
            @param data The data contains in the line
            @param checksum The calculated checksum
            @return True if no problem occurs */
        static bool calculateChecksum(quint16 addressMsbFirst,
                                      RecordType::Enum recordType,
                                      const QByteArray &data,
                                      quint8 &checksum);
};
