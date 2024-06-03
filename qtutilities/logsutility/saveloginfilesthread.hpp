// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "threadutility/basethread.hpp"

#include "loggingoption.hpp"
#include "loggingstrategyoption.hpp"
#include "logmsgtype.hpp"

class AFileLogsStrategy;


/** @brief Allow to manage the log saving in a dedicated thread */
class SaveLogInFilesThread : public BaseThread
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The class parent */
        explicit SaveLogInFilesThread(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~SaveLogInFilesThread() override;

    public:
        /** @brief Call to start logging in files
            @return True if no problem occurs */
        bool startLogging();

        /** @brief Call to test if logging is started
            @return True if the logging is started */
        bool isLoggingStarted();

        /** @brief Call to stop logging in files
            @return True if no problem occurs */
        bool stopLogging();

        /** @brief Get the log criticity limit */
        LogMsgType::Enum getLogCriticity() const { return _logCriticity; }

        /** @brief Set the log criticity limit */
        void setLogCriticity(LogMsgType::Enum logCriticity) { _logCriticity = logCriticity; }

        /** @brief Reset the logging and set the strategy to OneFilePerDay strategy
            @param folderPath The path of the main logs folder
            @param fileNameFormat The name format to apply to the log file, the strategy will use
                                  the name given and preprend it with day date string representation
            @param folderStrategy The strategy options which defines the way of logging for this
                                  file
            @param maxFolderLimitInMo Defines a max limit in Mo for the main logs folder
                                      If the folder overflows this limit, the last logs will be
                                      removed
                                      If equals to -1 the folder is not test
            @return True if no problem occurs */
        bool resetToOneFilePerDayStrategy(const QString &folderPath,
                                          const QString &fileNameFormat,
                                          LoggingStrategyOption::Enums folderStrategy,
                                          int maxFolderSizeLimitInMo = -1);

        /** @brief Reset the logging and set the strategy to OneFilePerObject strategy
            @param folderPath The path of the main logs folder
            @param fileSuffix The suffix to append to the log file (ex: ".log")
            @param folderStrategy The strategy options which defines the way of logging for this
                                  file
            @param maxFolderSizeLimitInMo Defines a max limit in Mo for the main logs folder
                                          If the folder overflows this limit, the last logs will be
                                          removed
                                          If equals to -1 the folder is not test
            @return True if no problem occurs */
        bool resetToOneFilePerObjectStrategy(const QString &folderPath,
                                             const QString &fileSuffix,
                                             LoggingStrategyOption::Enums folderStrategy,
                                             int maxFolderSizeLimitInMo = -1);

    public slots:
        /** @brief Called to write log in files
            @param msg The msg to write in files
            @param options Optional logging arguments, the usage depends of the strategy chosen */
        void writeLog(const QString &msg, const LoggingOptions &options);

        /** @brief Called to write log in files
            @note This method is just a shortcut and call the method
                  writeLog(QString, LoggingOptions)
            @param msg The msg to write in files */
        void writeLog(const QString &msg);

        /** @brief Called to write log in files
            @note This method is just a shortcut and call the method
                  writeLog(QString, LoggingOptions)
            @param msg The msg to write in files
            @param id The id linked to the log
            @param addAtEnd False to add a log at the start of the file */
        void writeLog(const QString &msg, const QString &id, bool addAtEnd = true);

        /** @brief Called to append a suffix to the log file base name
            @param toAppend The suffix to add to the log file base name
            @param options Optional logging arguments, the usage depends of the strategy chosen
            @return True if no problem occurs */
        bool appendToBaseName(const QString &toAppend,
                              const LoggingOptions &options = LoggingOptions());

        /** @brief Call to stop the thread
            @return True if no problem occurs */
        virtual bool stopThread() override;

    signals:
        /** @brief Emitted to write log
            @see SaveLogInFilesThread::writeLog */
        void _writeLog(const QString &msg, const LoggingOptions &options);

    private:
        /** @brief Remove the current strategy and change it for the new one given
            @param newSrategyToApply The new strategy to replace the old one with
            @return True if no problem occurs */
        bool manageStrategyReset(AFileLogsStrategy *newStrategyToApply);

    private:
        AFileLogsStrategy *_strategy{nullptr};
        LogMsgType::Enum _logCriticity{LogMsgType::Warning};
};
