// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "logsmanager.hpp"

#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QTimer>

#include "logsutility/saveloginfilesthread.hpp"

LogsManager *LogsManager::_instance = nullptr;


LogsManager::LogsManager(QObject *parent) :
    QObject(parent)
{
    _strategies[LoggingStrategy::DisplayLogsInConsole] =
                        LoggingStrategyOption::getAllFlags(LoggingStrategy::DisplayLogsInConsole);

    _defaultMsgHandler = qInstallMessageHandler(nullptr);
    qInstallMessageHandler(LogsManager::staticLogHandler);
}

LogsManager::~LogsManager()
{
    qInstallMessageHandler(_defaultMsgHandler);
}

bool LogsManager::setSavingLogFileStrategy(LoggingStrategyOption::Enums strategies,
                                           const QString &folderPath,
                                           const QString &fileNameFormatOrSuffix,
                                           LogMsgType::Enum logCriticityToSaveInFile,
                                           int maxFolderSizeLimitInMo)
{
    strategies = LoggingStrategyOption::filterOptionsForSpecificStrategy(
                                                        LoggingStrategy::SaveLogsInFiles,
                                                        strategies);

    _strategies[LoggingStrategy::SaveLogsInFiles] = strategies;

    if(strategies == 0)
    {
        // Do nothing, the log won't be no more sent to the log strategy
        return true;
    }

    if(_saveLogInFilesThread == nullptr)
    {
        _saveLogInFilesThread = new SaveLogInFilesThread(this);

        if(!_saveLogInFilesThread->startThreadAndWaitToBeReady())
        {
            _saveLogInFilesThread->stopAndDeleteThread();
            _saveLogInFilesThread = nullptr;
            return false;
        }
    }

    if(strategies.testFlag(LoggingStrategyOption::File_SetMaxSizeLimit) &&
       maxFolderSizeLimitInMo <= 0)
    {
        qWarning() << "Strategy: "
                   << LoggingStrategyOption::toString(LoggingStrategyOption::File_SetMaxSizeLimit)
                   << ", can't be enabled with a wrong maxFolderSizeLimitInMo parameter value: "
                   << maxFolderSizeLimitInMo;
        return false;
    }

    if(!strategies.testFlag(LoggingStrategyOption::File_SetMaxSizeLimit) &&
       maxFolderSizeLimitInMo != -1)
    {
        qWarning() << "The maxFolderSizeLimitInMo parameter can't have a value different of -1 when"
                   << " the strategy: "
                   << LoggingStrategyOption::toString(LoggingStrategyOption::File_SetMaxSizeLimit)
                   << " is enabled";
        return false;
    }

    _saveLogInFilesThread->setLogCriticity(logCriticityToSaveInFile);

    if(strategies.testFlag(LoggingStrategyOption::File_OneFilePerDay))
    {
        if(!_saveLogInFilesThread->resetToOneFilePerDayStrategy(folderPath,
                                                                fileNameFormatOrSuffix,
                                                                strategies,
                                                                maxFolderSizeLimitInMo))
        {
            return false;
        }
    }
    else if(strategies.testFlag(LoggingStrategyOption::File_OneFilePerObject))
    {
        if(!_saveLogInFilesThread->resetToOneFilePerObjectStrategy(folderPath,
                                                                   fileNameFormatOrSuffix,
                                                                   strategies,
                                                                   maxFolderSizeLimitInMo))
        {
            return false;
        }
    }
    else
    {
        qWarning() << "No right strategy given for saving logs in file";
        return false;
    }

    return true;
}

bool LogsManager::setLogConsoleStrategy(LoggingStrategyOption::Enums strategies,
                                        LogMsgType::Enum logCritictyToDisplayInConsole)
{
    strategies = LoggingStrategyOption::filterOptionsForSpecificStrategy(
                                                            LoggingStrategy::DisplayLogsInConsole,
                                                            strategies);

    _strategies[LoggingStrategy::DisplayLogsInConsole] = strategies;

    _consoleLogCriticity = logCritictyToDisplayInConsole;

    return true;
}

void LogsManager::RegisterMetaType()
{
    LoggingStrategy::RegisterMetaType();
    LoggingStrategyOption::RegisterMetaType();
}

LogsManager &LogsManager::Instance()
{
    if(_instance == nullptr)
    {
        _instance = new LogsManager();
    }

    return *_instance;
}

void LogsManager::staticLogHandler(QtMsgType type,
                                   const QMessageLogContext &context,
                                   const QString &msg)
{
    LogsManager::Instance().logHandler(type, context, msg);
}

QString LogsManager::formatLog(const LoggingStrategyOption::Enums &strategies,
                               LogMsgType::Enum type,
                               const QMessageLogContext &context,
                               const QString &msg)
{
    QString log;

    if(strategies.testFlag(LoggingStrategyOption::Glob_DisplayDateTime))
    {
        log.append(QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs) + " ");
    }

    if(strategies.testFlag(LoggingStrategyOption::Glob_DisplayLogLevel))
    {
        log.append(LogMsgType::toLogString(type) + " ");
    }

    log.append(msg);

    if(strategies.testFlag(LoggingStrategyOption::Glob_DisplayLogContext))
    {
        log.append(QString(" (%1:%2)").arg(QFileInfo(context.file).fileName()).arg(context.line));
    }

    return log;
}

void LogsManager::logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    LogMsgType::Enum logMsgType = LogMsgType::parseCriticityFromQt(type);

    if(_saveLogInFilesThread != nullptr &&
       LogMsgType::isEqualOrAboveCriticity(logMsgType, _saveLogInFilesThread->getLogCriticity()))
    {
        const LoggingStrategyOption::Enums &saveFileStrats =
                                                    _strategies[LoggingStrategy::SaveLogsInFiles];

        if(saveFileStrats != 0)
        {
            _saveLogInFilesThread->writeLog(formatLog(saveFileStrats, logMsgType, context, msg));
        }
    }

    if(!LogMsgType::isEqualOrAboveCriticity(logMsgType, _consoleLogCriticity))
    {
        return;
    }

    LoggingStrategyOption::Enums consoleStrats = _strategies[LoggingStrategy::DisplayLogsInConsole];

    if(consoleStrats != 0)
    {
        _defaultMsgHandler(type, context, formatLog(consoleStrats, logMsgType, context, msg));
    }
}
