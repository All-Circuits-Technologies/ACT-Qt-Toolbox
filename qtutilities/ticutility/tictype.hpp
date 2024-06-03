// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief The type of tic which represents a specific frequency */
class TicType : public QObject
{
    Q_OBJECT

    public:
        enum Enum {
            TwoHz,      //!< @brief Equals to 500ms
            ThreeHz,    //!< @brief Equals to 333ms
            Unknown
        };
        Q_ENUM(Enum)

    public:
        /** @brief Register meta type linked to this enum */
        static void registerMetaTypes();

        /** @brief Get the period in milliseconds linked to the tic type */
        static int getPeriodInMs(Enum ticType);

        /** @brief Returns a string representation of the tic type */
        static QString toString(Enum type);
};
