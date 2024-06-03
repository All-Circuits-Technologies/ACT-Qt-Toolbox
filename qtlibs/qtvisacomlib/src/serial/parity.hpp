// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include "src/visacomglobal.hpp"


/** @brief List of the value parity */
class VISACOM_EXPORT Parity : public QObject
{
    Q_OBJECT

    public:
        enum Enum:quint16
        {
            NoParity = 0,   /*VI_ASRL_PAR_NONE (0)*/
            Odd = 1,        /*VI_ASRL_PAR_ODD (1)*/
            Even = 2,       /*VI_ASRL_PAR_EVEN (2)*/
            Mark = 3,       /*VI_ASRL_PAR_MARK (3)*/
            Space = 4,       /*VI_ASRL_PAR_SPACE (4)*/
            Unknown = std::numeric_limits<quint16>::max()
        };

        Q_ENUM(Enum)

    public:
        /** @brief Parse the enum from string
            @param strValue The string to parse
            @return The enum parsed */
        static Enum parseFromString(const QString &strValue);

        /** @brief Return a string representation of the Parity
            @param parity The type to get the string representation from
            @return The string representation */
        static QString toString(Enum parity);

        /** @brief Get all the parity enum values
            @return The list of memory types */
        static const QVector<Enum> &getAllEnums();

    private:
        static QVector<Enum> enumList;
};
