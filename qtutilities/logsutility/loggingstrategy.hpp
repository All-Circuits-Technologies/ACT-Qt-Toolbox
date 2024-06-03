// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Specify the main strategy categories */
class LoggingStrategy : public QObject
{
    Q_OBJECT

    public:
        enum Enum
        {
            SaveLogsInFiles,        //!< @brief Strategy used to save logs in files
            DisplayLogsInConsole    //!< @brief Strategy used to display logs in console
        };
        Q_ENUM(Enum)

    public:
        /** @brief Register meta type linked to this enum */
        static void RegisterMetaType();

        /** @brief Get a string representation of the enum
            @param value The enum value to stringify
            @return The string representation of the enum */
        static QString toString(Enum value);
};
