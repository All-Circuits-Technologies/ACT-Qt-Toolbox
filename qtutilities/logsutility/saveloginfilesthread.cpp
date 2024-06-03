// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "saveloginfilesthread.hpp"

#include <QTimer>

#include "filestrategies/afilelogsstrategy.hpp"
#include "filestrategies/onefileperdaylogsstrategy.hpp"
#include "filestrategies/onefileperobjectlogsstrategy.hpp"
#include "threadutility/concurrent/threadconcurrentrun.hpp"


SaveLogInFilesThread::SaveLogInFilesThread(QObject *parent) :
    BaseThread(parent)
{
}

SaveLogInFilesThread::~SaveLogInFilesThread()
{
}

bool SaveLogInFilesThread::startLogging()
{
    if(waitForThread())
    {
        if(_strategy == nullptr)
        {
            qWarning() << "Can't start the savings of logs: no strategy set for saving logs in "
                       << "files";
            return false;
        }

        return ThreadConcurrentRun::run(*_strategy, &AFileLogsStrategy::start);
    }

    return false;
}

bool SaveLogInFilesThread::isLoggingStarted()
{
    if(waitForThread())
    {
        if(_strategy == nullptr)
        {
            // If there is no strategy, can't be started
            return false;
        }

        return ThreadConcurrentRun::run(*_strategy, &AFileLogsStrategy::isStarted);
    }

    return false;
}

bool SaveLogInFilesThread::stopLogging()
{
    if(waitForThread())
    {
        if(_strategy == nullptr)
        {
            qWarning() << "Can't stop the savings of logs: no strategy set for saving logs in "
                       << "files";
            return false;
        }

        return ThreadConcurrentRun::run(*_strategy, &AFileLogsStrategy::stop);
    }

    return false;
}

bool SaveLogInFilesThread::resetToOneFilePerDayStrategy(const QString &folderPath,
                                                        const QString &fileNameFormat,
                                                        LoggingStrategyOption::Enums folderStrategy,
                                                        int maxFolderSizeLimitInMo)
{
    if(waitForThread())
    {
        OneFilePerDayLogsStrategy *strategy = new OneFilePerDayLogsStrategy(folderPath,
                                                                            fileNameFormat,
                                                                            folderStrategy,
                                                                            maxFolderSizeLimitInMo);

        strategy->moveToThread(this);

        if(!manageStrategyReset(strategy))
        {
            strategy->deleteLater();
            return false;
        }

        return true;
    }

    return false;
}

bool SaveLogInFilesThread::resetToOneFilePerObjectStrategy(
                                                        const QString &folderPath,
                                                        const QString &fileSuffix,
                                                        LoggingStrategyOption::Enums folderStrategy,
                                                        int maxFolderSizeLimitInMo)
{
    if(waitForThread())
    {
        OneFilePerObjectLogsStrategy *strategy = new OneFilePerObjectLogsStrategy(
                                                                            folderPath,
                                                                            fileSuffix,
                                                                            folderStrategy,
                                                                            maxFolderSizeLimitInMo);

        strategy->moveToThread(this);

        if(!manageStrategyReset(strategy))
        {
            strategy->deleteLater();
            return false;
        }

        return true;
    }

    return false;
}

void SaveLogInFilesThread::writeLog(const QString &msg, const LoggingOptions &options)
{
    if(waitForThread())
    {
        emit _writeLog(msg, options);
    }
}

void SaveLogInFilesThread::writeLog(const QString &msg)
{
    writeLog(msg, LoggingOptions());
}

void SaveLogInFilesThread::writeLog(const QString &msg, const QString &id, bool addAtEnd)
{
    writeLog(msg, { { LoggingOption::Id, id },
                    { LoggingOption::AtStart, !addAtEnd } });
}

bool SaveLogInFilesThread::appendToBaseName(const QString &toAppend, const LoggingOptions &options)
{
    if(!waitForThread())
    {
        return false;
    }

    if(_strategy == nullptr)
    {
        qWarning() << "Can't modify the name of log files: no strategy chosen";
        return false;
    }

    if(!ThreadConcurrentRun::run(*_strategy,
                                 &AFileLogsStrategy::appendToBaseName,
                                 toAppend,
                                 options))
    {
        qWarning() << "A problem occurred when trying to append a string to the log base name";
        return false;
    }

    return true;
}

bool SaveLogInFilesThread::stopThread()
{
    qDebug() << "---------------------> Stop thread for logs";
    if(_strategy != nullptr)
    {
        if(!ThreadConcurrentRun::run(*_strategy, &AFileLogsStrategy::stop))
        {
            qWarning() << "A problem occurred when trying to stop the saving log strategy";
            return false;
        }

        QTimer::singleShot(0, _strategy, &AFileLogsStrategy::deleteLater);
        _strategy = nullptr;
    }

    return BaseThread::stopThread();
}

bool SaveLogInFilesThread::manageStrategyReset(AFileLogsStrategy *newStrategyToApply)
{
    if(!ThreadConcurrentRun::run(*newStrategyToApply, &AFileLogsStrategy::start))
    {
        return false;
    }

    connect(this,               &SaveLogInFilesThread::_writeLog,
            newStrategyToApply, &AFileLogsStrategy::writeLog);

    if(_strategy != nullptr)
    {
        disconnect(this,        &SaveLogInFilesThread::_writeLog,
                   _strategy,   &AFileLogsStrategy::writeLog);

        if(!ThreadConcurrentRun::run(*_strategy, &AFileLogsStrategy::stop))
        {
            return false;
        }

        QTimer::singleShot(0, _strategy, &AFileLogsStrategy::deleteLater);
    }

    _strategy = newStrategyToApply;

    return true;
}
