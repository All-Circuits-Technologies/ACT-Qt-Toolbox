// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QHash>
#include <QVector>

#include "loggingstrategy.hpp"


/** @brief Specify the strategy options for main strategies
    @note Some options are particular to somme specific main strategies
          Some options are generic to all the main strategies */
class LoggingStrategyOption : public QObject
{
    Q_OBJECT

    public:
        enum Enum : qint32
        {
            // Strategy options for the main strategy: saving logs in files
            // *** The following options are sub main strategies for saving logs in files, they are
            //     not compatible between each others
            File_OneFilePerDay          = 0x00000001, /*!< @brief In that mode, we create one file
                                                                  per day */
            File_OneFilePerObject       = 0x00000002, /*!< @brief In that mode, we create one file
                                                                  per object */

            // *** Generic options for saving logs in files strategies, thoses options can be
            //     combined
            File_SetMaxSizeLimit        = 0x00000004, /*!< @brief Set a max size limit for the main
                                                                  logs folder*/
            File_StoreInYearFolder      = 0x00000008, /*!< @brief Create a sub folder in the main
                                                                  logs directory for the current log
                                                                  file year
                                                           @note The folder will be named yyyy,
                                                                 ex: "2020" */
            File_StoreInMonthFolder     = 0x00000010, /*!< @brief Create a sub folder in the main
                                                                  logs directory for the current log
                                                                  file month
                                                           @note The folder will be named yyyyMM,
                                                                 ex: "202001" */
            File_StoreInDayFolder       = 0x00000020, /*!< @brief Create a sub folder in the main
                                                                  logs directory for the current log
                                                                  file day
                                                           @note The folder will be named yyyyMMdd,
                                                                 ex: "20200125" */

            // Global format log
            Glob_DisplayLogContext      = 0x00000040, /*!< @brief Add to the log message, the log
                                                                 context (the line file and file
                                                                 name from where the log has been
                                                                 created) */
            Glob_DisplayLogLevel        = 0x00000080, /*!< @brief Add to the log message, the log
                                                                  level (Warning, Info, etc...) */
            Glob_DisplayDateTime        = 0x00000100  /*!< @brief Add to the log message, the
                                                                  timestamp when the log has been
                                                                  created */
        };
        Q_ENUM(Enum)
        Q_DECLARE_FLAGS(Enums, Enum)

    public:
        /** @brief Register meta type linked to this enum */
        static void RegisterMetaType();

        /** @brief Get a string representation of the enum
            @param value The enum value to stringify
            @return The string representation of the enum */
        static QString toString(Enum value);

        /** @brief Get all the available flags */
        static const Enums &getAllFlags();

        /** @brief Get all the available flags linked to the main strategy targeted
            @param mainStrategy The main strategy to discriminate the strategy options
            @return All the available flags for the main strategy wanted */
        static Enums getAllFlags(LoggingStrategy::Enum mainStrategy);

        /** @brief Get all the available flags
            @note Return the same flags as @ref LoggingStrategyOption::getAllFlags method */
        static const QVector<Enum> &getAllEnums();

        /** @brief Filter the given strategies to only keep those which can be used with the
                   main strategy targeted
            @param mainStrategy The main strategy to discriminate options from
            @param strategies The option strategies to filter
            @return The strategies filtered */
        static Enums filterOptionsForSpecificStrategy(LoggingStrategy::Enum mainStrategy,
                                                      Enums strategies);

    private:
        static Enums AllElements;
        static QVector<Enum> EnumList;
        static QHash<LoggingStrategy::Enum, QVector<Enum>> StrategyOptions;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(LoggingStrategyOption::Enums)
Q_DECLARE_METATYPE(LoggingStrategyOption::Enums)
