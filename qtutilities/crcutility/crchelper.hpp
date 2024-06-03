// SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

/* ------------------------------------------------------------------
 * crchelper.hpp - COPYRIGHT (C) 2019 ALL CIRCUITS
 * ------------------------------------------------------------------
 */

#pragma once

#include "crcutility/crcconstants.hpp"


/** @brief Helpful methods to calculate CRC */
class CrcHelper
{
    private:
        /** @brief Class constructor */
        explicit CrcHelper();

    public:
        /** @brief Calculate a specific 16bits CRC, the CRC MCRF4XX
            @param data Calculate the CRC from those data
            @return The CRC calculated */
        static quint16 calculateCrcMcrf4xx(const QByteArray &data,
                                           quint16 init = CrcConstants::Crc16Mcrf4xx::defaultInit);

        /** @brief Calculate the 32bits CRC
            @param data Calculate the CRC from those data
            @return The CRC calculated */
        static quint32 calculateCrc32(const QByteArray &data,
                                      quint32 polynom = CrcConstants::Crc32::normalPolynom,
                                      quint32 init = CrcConstants::Crc32::defaultInit);

    private:
        /** @brief Create Crc32 table if not already done */
        void createCrc32Table(quint32 polynom);

    private:
        /** @brief Get Class instance */
        static CrcHelper &getInstance();

    private:
        static CrcHelper *_instance;

    private:
        quint32 *_crc32Table = nullptr;
};
