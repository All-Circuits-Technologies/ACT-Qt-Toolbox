// SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

/* ------------------------------------------------------------------
 * crcconstants.hpp - COPYRIGHT (C) 2019 ALL CIRCUITS
 * ------------------------------------------------------------------
 */

#pragma once

#include <QtGlobal>


namespace CrcConstants
{
    namespace Crc32
    {
        /** @brief Init value for CRC32 */
        static constexpr const quint32 defaultInit = 0x00000000;

        /** @brief Init value for CRC32 */
        static constexpr const quint32 reversedInit = 0xFFFFFFFF;

        /** @brief CRC32 normal polynom */
        static constexpr const quint32 normalPolynom = 0x04C11DB7;

        /** @brief CRC32 reversed polynom */
        static constexpr const quint32 reversedPolynom = 0xEDB88320;

        /** @brief CRC32 reciprocal polynom */
        static constexpr const quint32 reciprocalPolynom = 0xDB710641;

        /** @brief CRC32 reversed reciprocal polynom */
        static constexpr const quint32 reversedReciprocalPolynom = 0x82608EDB;

        /** @brief Size of the CRC32 table  */
        static constexpr const int tableSize = 256;

        /** @brief CRC32 table creation iteration number */
        static constexpr const int tableCreationIterNb = 8;
    }

    namespace Crc16Mcrf4xx
    {
        constexpr const quint16 defaultInit = 0xFFFF;
    }
}
