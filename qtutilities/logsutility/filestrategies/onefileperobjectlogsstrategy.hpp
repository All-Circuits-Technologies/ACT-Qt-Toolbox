// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "filestrategies/aonefilelogsstrategy.hpp"


/** @brief This strategy is used to create one logs file per object */
class OneFilePerObjectLogsStrategy : public AOneFileLogsStrategy
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param folderPath The path of the main logs folder
            @param fileSuffix The suffix to append to the log file (ex: ".log")
            @param folderStrategy The strategy options which defines the way of logging for this
                                  file
            @param maxFolderLimitInMo Defines a max limit in Mo for the main logs folder
                                      If the folder overflows this limit, the last logs will be
                                      removed
                                      If equals to -1 the folder is not test
            @param parent The object parent */
        explicit OneFilePerObjectLogsStrategy(const QString &folderPath,
                                              const QString &fileSuffix,
                                              LoggingStrategyOption::Enums folderStrategy,
                                              qint64 maxFolderLimitInMo = -1,
                                              QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~OneFilePerObjectLogsStrategy() override = default;

    public:
        /** @brief Set the suffix to append at the end of the log file (ex: ".log") */
        void setFileSuffix(const QString &fileSuffix);

    protected:
        /** @see AOneFileLogsStrategy::manageFileCreationIfNeeded */
        virtual bool manageFileCreationIfNeeded(const LoggingOptions &options) override;

        /** @see AOneFileLogsStrategy::getLogFilenameFilters */
        virtual QStringList getLogFilenameFilters() override;

    private:
        /** @brief Timestamp format for the file name */
        static const constexpr char *DateTimeFormat = "yyyyMMdd'T'HHmmss";

    private:
        QString _fileSuffix;
        QString _objectIdentifier;
};
