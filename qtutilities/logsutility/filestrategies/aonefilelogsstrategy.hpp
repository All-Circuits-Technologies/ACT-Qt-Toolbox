// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "filestrategies/afilelogsstrategy.hpp"

#include <QDir>

#include "loggingstrategyoption.hpp"

class QFile;


/** @brief Abstract strategy for logging into one file at the same time */
class AOneFileLogsStrategy : public AFileLogsStrategy
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param folderPath The path of the main logs folder
            @param options The strategy options which defines the way of logging for this file
            @param maxFolderLimitInMo Defines a max limit in Mo for the main logs folder
                                      If the folder overflows this limit, the last logs will be
                                      removed
                                      If equals to -1 the folder is not test
            @param parent The object parent */
        explicit AOneFileLogsStrategy(const QString &folderPath,
                                      LoggingStrategyOption::Enums options,
                                      qint64 maxFolderLimitInMo = -1,
                                      QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AOneFileLogsStrategy() override = default;

    public slots:
        /** @see AFileLogsStrategy::stop */
        virtual bool stop() override;

        /** @see AFileLogsStrategy::writeLog */
        virtual bool writeLog(const QString &msg,
                              const LoggingOptions &options = LoggingOptions()) override;

        /** @see AFileLogsStrategy::appendToBaseName */
        virtual bool appendToBaseName(const QString &toAppend,
                                      const LoggingOptions &options = LoggingOptions()) override;

        /** @brief Call to remove old files, if the current size of all the logs files in the folder
                   is greater than the max folder limit
            @note The folder can exceed the max limit without firing this method, because we only
                  calculate the total logs files size and not the folder size
            @note If the max folder limit is equal to -1, this method does nothing
            @note The method will try to remove files even if a problem occurred on one, in order to
                  prevent that no logs are removed if one can't be.
            @note The method will display the first files which can't be removed in only one warning
                  log.
            @note The method will display the path of all the files which can't be removed in debug
                  level, in order to avoid to saturate the logs files */
        void removeOldFilesIfNeeded();

        /** @brief Set the options to apply with this strategy
            @param strategyOptions The options to apply */
        void setStrategyOptions(LoggingStrategyOption::Enums strategyOptions);

    signals:
        /** @brief Emitted in order to fire the call of the method:
                   @ref AOneFileLogsStrategy::removeOldFilesIfNeeded */
        void _askForLogFolderCleaning();

    protected:
        /** @brief Manage the creation of the log file, if needed
            @note The firing of the file creation depends of the strategy
            @param options Optional logging arguments, the usage depends of the strategy chosen
            @return True if no problem occurs */
        virtual bool manageFileCreationIfNeeded(const LoggingOptions &options) = 0;

        /** @brief Get from strategy the filename filters to apply, in order to get the logs
                   manage by the strategy
            @return The filter to apply on filename logs, in order to get them */
        virtual QStringList getLogFilenameFilters() = 0;

    protected:
        /** @brief Create a log file thanks to filename given
            @note If the path of the log file doesn't exist, the method will create it
            @param filename The path and name of the file
            @return True if no problem occurs */
        bool createFile(const QString &filename);

        /** @brief Get the current logs file or nullptr if the logs file hasn't been created */
        const QFile *getLogsFile() const { return _logsFile; }

    private:
        /** @brief Get the current directory where to insert logs file, if the folder doesn't
                   already exist the method creates it.
            @note Try to create the folder from the folder path given in constructor
            @param dir The folder where to add logs file
            @return True if no problem occurs */
        bool createAndGetCurrentRightFolder(QDir &dir);

        /** @brief Remove old logs files, if the current size of all the logs files in the folder
                   is greater than the max folder limit. This method allow to remove files
                   recursively in folders
            @attention Because this method is an utility method to remove logs file, its better to
                       call @ref AOneFileLogsStrategy::removeOldFilesIfNeeded method instead of
                       this one.
            @note The folder can exceed the max limit without firing this method, because we only
                  calculate the total logs files size and not the folder size
            @note If a folder is empty, try to remove it
            @note The method will try to remove files even if a problem occurred on one, in order to
                  prevent that no logs are removed if one can't be.
            @note The method will display the path of all the files which can't be removed in debug
                  level, in order to avoid to saturate the logs files
            @param directory The current directory to iterate on files in it
            @param namesFilter The names filter to apply in order to only get logs file managed by
                               the strategy
            @param currentFileFound Set to true when the current file has been found
            @param currentSizeInBytes The current size in bytes calculated
            @param cantBeRemovedFiles Contains the first files which can't be removed, because an
                                      error occurred,
                                      @ref AOneFileLogsStrategy::maxCantBeRemovedFilesToDisplay */
        void removeFilesIfNeededImpl(const QDir &directory,
                                     const QStringList &namesFilter,
                                     bool &currentFileFound,
                                     qint64 &currentSizeInBytes,
                                     QStringList &cantBeRemovedFiles);

    private:
        /** @brief Used when trying to remove logs files, this constant precises how many logs files
                   name can be displayed in the warning log, when files removing fails */
        static const constexpr int maxCantBeRemovedFilesToDisplay = 5;

        /** @brief Used to convert a number in Mega bytes to bytes */
        static const constexpr qint64 MegaBytesToBytesFactor = 1'000'000;

    private:
        qint64 _maxFolderLimitInBytes;
        LoggingStrategyOption::Enums _strategyOptions;
        QFile *_logsFile{nullptr};
};
