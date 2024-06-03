// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "filestrategies/aonefilelogsstrategy.hpp"

#include <QDate>


/** @brief This strategy is used to create one logs file per day */
class OneFilePerDayLogsStrategy : public AOneFileLogsStrategy
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param folderPath The path of the main logs folder
            @param fileNameFormat The name format to apply to the log file, the strategy will use
                                  the name given and preprend it with day date string representation
            @param folderStrategy The strategy options which defines the way of logging for this
                                  file
            @param maxFolderLimitInMo Defines a max limit in Mo for the main logs folder
                                      If the folder overflows this limit, the last logs will be
                                      removed
                                      If equals to -1 the folder is not test
            @param parent The object parent */
        explicit OneFilePerDayLogsStrategy(const QString &folderPath,
                                           const QString &fileNameFormat,
                                           LoggingStrategyOption::Enums folderStrategy,
                                           qint64 maxFolderLimitInMo = -1,
                                           QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~OneFilePerDayLogsStrategy() override = default;

    public:
        /** @brief Set the file name format to apply to the log file, the strategy will use the name
                   given and preprend it with day date string representation */
        void setFileNameFormat(const QString &fileNameFormat) { _fileNameFormat = fileNameFormat; }

    protected:
        /** @see AOneFileLogsStrategy::manageFileCreationIfNeeded */
        virtual bool manageFileCreationIfNeeded(const LoggingOptions &options) override;

        /** @see AOneFileLogsStrategy::getLogFilenameFilters */
        virtual QStringList getLogFilenameFilters() override;

    private:
        QString _fileNameFormat;
        QDate _currentFileDate;
};
