// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QHash>


/** @brief This is the CAN FD DLC representation in the PEAK Can lib */
class PCanFrameDlc : public QObject
{
    Q_OBJECT

    public:
        /** @brief Contains all the available DLC
            @note The DLC value is the byte representation of the DLC in the PEAK lib
                  (to have a DLC equals to 12, you have to send a 0x09) */
        enum Enum : quint8 {
            Dlc0 = 0x00,
            Dlc1,
            Dlc2,
            Dlc3,
            Dlc4,
            Dlc5,
            Dlc6,
            Dlc7,
            Dlc8,
            Dlc12,
            Dlc16,
            Dlc20,
            Dlc24,
            Dlc32,
            Dlc48,
            Dlc64,
            _LastValue, //!< @brief This is only used to get the last DLC index
            Unknown = 0xFF,
        };
        Q_ENUM(Enum)

    public:
        /** @brief Parse the frame DLC enum from the byte received or sent to the PEAK Can lib
            @param byte The byte to parse
            @return The DLC enum parsed, this returns Unknown if no match has been found */
        static Enum parseFromByte(quint8 byte);

        /** @brief Generate the byte to send to the PEAK CAN Lib from the frame DLC enum
            @param value The value to parse
            @return The byte to send.
                    If the enum is equal to @ref _LastValue or @ref Unknown, the method returns
                    @ref Dlc0 */
        static quint8 toByte(Enum value);

        /** @brief Parse the DLC from the payload size
            @param size The size to parse
            @return The DLC enum parsed, this returns Unknown if no match has been found */
        static Enum parseFromSize(qint32 size);

        /** @brief Generate the size of message payload from the DLC enum
            @param value The value to parse
            @return The linked length, if no size is found @ref UnknownSize is used */
        static qint32 toSize(Enum value);

    private:
        /** @brief The size of an Unknown DLC */
        static const constexpr qint32 UnknownSize = -1;

    private:
        static const QHash<Enum, qint32> DlcSizes;
};
