// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "logsutility/loggingstrategyoption.hpp"
#include "logsutility/logmsgtype.hpp"

#include <QHash>

class SaveLogInFilesThread;


/** @brief Singleton to manage logs received from the Qt logging system */
class LogsManager : public QObject
{
    Q_OBJECT

    private:
        /** @brief Private class constructor
            @param parent The parent class */
        explicit LogsManager(QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~LogsManager() override;

        /** @brief Allow to specify a strategy for saving logs in files
            @param strategies The optional strategies to apply
            @param folderPath The path of the main logs folder
            @param fileNameFormatOrSuffix If using OneFilePerDay strategy:
                                                        the name format to apply to the log file
                                  If using OneFilePerObject strategy:
                                                        the suffix to apply to the log file
            @param logCriticityToSaveInFile The log level from which we want to save logs in files
                                            Save in file if the level msg is equal or above this one
            @param maxFolderLimitInMo Defines a max limit in Mo for the main logs folder
                                      If the folder overflows this limit, the last logs will be
                                      removed
                                      If equals to -1 the folder is not test
            @return True if no problem occurs */
        bool setSavingLogFileStrategy(LoggingStrategyOption::Enums strategies,
                                      const QString &folderPath,
                                      const QString &fileNameFormatOrSuffix,
                                      LogMsgType::Enum logCriticityToSaveInFile,
                                      int maxFolderSizeLimitInMo = -1);

        /** @brief Allow to specify a strategy for writting logs in console
            @param strategies The optional strategies to apply
            @param logCritictyToDisplayInConsole The log level from which we want to write logs in
                                                 console. Write in console if the level msg is equal
                                                 or above this on
            @return True if no problem occurs */
        bool setLogConsoleStrategy(LoggingStrategyOption::Enums strategies,
                                   LogMsgType::Enum logCritictyToDisplayInConsole);

        /** @brief Register meta types linked to this logging system */
        static void RegisterMetaType();

    public:
        /** @brief Singleton class instance */
        static LogsManager &Instance();

    public:
        /** @brief Static method called by the Qt system each time the Qt logging system is used
            @param type The criticity log level
            @param context The log message context, saw in Qt doc:
                           ote: By default, this information is recorded only in debug builds.
                                You can overwrite this explicitly by defining QT_MESSAGELOGCONTEXT
                                or QT_NO_MESSAGELOGCONTEXT.
            @param msg The msg to log */
        static void staticLogHandler(QtMsgType type,
                                     const QMessageLogContext &context,
                                     const QString &msg);

    private:
        /** @brief Called in the log manager instance context
            @see LogsManager::staticLogHandler */
        void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    private:
        /** @brief Format the log msg to display
            @note Display the log in this way:
                        timestamp [type] msg (file:lineInFile)
                  ex:
                        123456789 [Dbug] Log message (class.cpp:52)
            @see LogsManager::staticLogHandler
            @return The log msg formatted */
        static QString formatLog(const LoggingStrategyOption::Enums &strategies,
                                 LogMsgType::Enum type,
                                 const QMessageLogContext &context,
                                 const QString &msg);

    private:
        static LogsManager *_instance; ///< @brief Singleton instance

    private:
        QtMessageHandler _defaultMsgHandler;
        QHash<LoggingStrategy::Enum, LoggingStrategyOption::Enums> _strategies;
        LogMsgType::Enum _consoleLogCriticity {LogMsgType::Debug};
        SaveLogInFilesThread *_saveLogInFilesThread{nullptr};
};
