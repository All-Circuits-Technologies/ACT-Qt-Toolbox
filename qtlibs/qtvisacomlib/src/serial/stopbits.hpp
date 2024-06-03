// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include "src/visacomglobal.hpp"


/** @brief List of the value stop bits */
class VISACOM_EXPORT StopBits : public QObject
{
    Q_OBJECT

    public:
        enum Enum:quint16
        {
            One = 10,   /*VI_ASRL_STOP_ONE (10)*/
            One5 = 15,  /*VI_ASRL_STOP_ONE5 (15)*/
            Two = 20,    /*VI_ASRL_STOP_TWO (20)*/
            Unknown = std::numeric_limits<quint16>::max()
        };

        Q_ENUM(Enum)

    public:
        /** @brief Parse the enum from string
            @param strValue The string to parse
            @return The enum parsed */
        static Enum parseFromString(const QString &strValue);

        /** @brief Return a string representation of the Stop Bits
            @param stopBits The type to get the string representation from
            @return The string representation */
        static QString toString(Enum stopBits);

        /** @brief Get all the stopbits enum values
            @return The list of memory types */
        static const QVector<Enum> &getAllEnums();

    private:
        static QVector<Enum> enumList;
};
