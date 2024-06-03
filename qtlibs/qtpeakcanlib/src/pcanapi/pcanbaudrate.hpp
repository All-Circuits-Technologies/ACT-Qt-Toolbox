// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QHash>

#include "numberutility/number.hpp"

#include "src/definescan.hpp"


/** @brief Contains a representation of the baudrate which can be used in the PEAK CAN bus
           configuration */
class CAN_EXPORT PCanBaudRate : public QObject
{
    Q_OBJECT

    public:
        /** @brief The PEAK CAN bus interface enum
            @note From lib doc:
                  Baud rate codes = BTR0/BTR1 register values for the CAN controller. ou can define
                  your own Baud rate with the BTROBTR1 register.
                  Take a look at www.peak-system.com for our free software "BAUDTOOL" to calculate
                  the BTROBTR1 register for every bit rate and sample point. */
        enum Enum {
            Baud1M,     //   1 MBit/s
            Baud800k,   // 800 kBit/s
            Baud500k,   // 500 kBit/s
            Baud250k,   // 250 kBit/s
            Baud125k,   // 125 kBit/s
            Baud100k,   // 100 kBit/s
            Baud95k,    //  95,238 kBit/s
            Baud83k,    //  83,333 kBit/s
            Baud50k,    //  50 kBit/s
            Baud47k,    //  47,619 kBit/s
            Baud33k,    //  33,333 kBit/s
            Baud20k,    //  20 kBits/s
            Baud10k,    //  10 kBit/s
            Baud5k,     //   5 kBit/s
            Unknown
        };
        Q_ENUM(Enum)

    private:
        /** @brief Contains the baud rate information */
        struct BaudRateInfo
        {
            /** @brief This is the PEAK CAN api code linked to the baud rate */
            quint16 apiCode;

            /** @brief This is the representation of the baudrate
                @note Exemple of 1MBit/s baudrate, this is equals to 1.000.000 */
            Number realValue;

            /** @brief Structure constructor
                @param tmpApiCode The PEAK lib api code
                @param tmpRealValue The real representation value */
            BaudRateInfo(quint16 tmpApiCode = DefaultApiCodeError,
                         const Number &tmpRealValue = Number::Invalid) :
                apiCode{tmpApiCode},
                realValue{tmpRealValue}
            {
            }

            /** @brief Copy constructor
                @param info The info to copy */
            BaudRateInfo(const BaudRateInfo& info) :
                apiCode{info.apiCode},
                realValue{info.realValue}
            {
            }

            /** @brief This is the assignment operator
                @param info The info to set
                @return The reference to this structure */
            BaudRateInfo& operator=(const BaudRateInfo& info)
            {
                apiCode = info.apiCode;
                realValue = info.realValue;

                return *this;
            }

            /** @brief This is the equal operator
                @param info The element to compare with
                @return True if the structures are equal */
            bool operator==(const BaudRateInfo& info) const
            {
                return (info.apiCode == apiCode) && (info.realValue == realValue);
            }
        };

    public:
        /** @brief Convert the baudrate enum to the real value
            @note Exemple If enum is equals to Baud1M, the method will return 1.000.000
            @param value The value to convert
            @return The baudrate value */
        static const Number &toRealValue(Enum value);

        /** @brief Convert the baudrate enum to the PEAK CAN api code
            @param value The value to convert
            @param ok If not nullptr, the method write the method result in it
            @return The api code linked to the enum */
        static quint16 toApiCode(Enum value, bool *ok = nullptr);

        /** @brief Convert the baudrate enum to the stringified real value
            @note Exemple If enum is equals to Baud1M, the method will return "1.000.000"
            @param value The value to convert
            @return The baudrate value */
        static QString toRealValueString(Enum value);

        /** @brief Parse the baudrate enum from the real value
            @param realBaudRateValue The real value to parse
            @return The baudrate representation */
        static Enum parseFromRealValue(const Number &realBaudRateValue);

        /** @brief Get the available elements
            @param excludeUnknown True to exclude the Unknown enum from the list of all elements
            @return The enum list */
        static QVector<Enum> getAvailableElements(bool excludeUnknown = false);

    private:
        /** @brief This is the default api code returned when an error occurred in parsing */
        static const constexpr quint16 DefaultApiCodeError = 0x0000U;

        /** @brief This is the default unknown string */
        static const constexpr char *DefaultUnknownString = "Unknown";

    private:
        static QHash<Enum, BaudRateInfo> BaudRateInfos;
};
