// SPDX-FileCopyrightText: 2022 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "processcaller.hpp"

#include <QDebug>
#include <QFile>
#include <QProcess>

#include "definesutility/definesutility.hpp"
#include "waitutility/waithelper.hpp"


ProcessCaller::ProcessCaller(const QString &programName, QObject *parent)
    : QObject{parent},
      _programName{programName}
{
}

ProcessCaller::~ProcessCaller()
{
}

bool ProcessCaller::initProcess(const QString &processPath,
                                int defaultTimeout,
                                const QStringList &defaultArguments)
{
    if(!_processPath.isEmpty())
    {
        // Already initialized
        return true;
    }

    if(!QFile::exists(processPath))
    {
        qWarning() << "The program: "<< _programName
                   << ", can't be found where is expected to be: " << processPath;
        return false;
    }

    _processPath = processPath;

    // If negative, the defaultTimeout keeps its default value which is: -1
    if(defaultTimeout >= 0)
    {
        _defaultTimeout = defaultTimeout;
    }

    _defaultArguments = defaultArguments;

    return true;
}

bool ProcessCaller::call(const QStringList &arguments,
                         QString *stdOutput,
                         bool logProcessError,
                         int overrideTimeoutInMs,
                         bool *processExitProperly)
{
    return callPriv(arguments,
                    logProcessError,
                    overrideTimeoutInMs,
                    nullptr,
                    stdOutput,
                    processExitProperly);
}

bool ProcessCaller::call(const QStringList &arguments,
                         QFile &stdOutputFile,
                         bool logProcessError,
                         int overrideTimeoutInMs,
                         bool *processExitProperly)
{
    return callPriv(arguments,
                    logProcessError,
                    overrideTimeoutInMs,
                    &stdOutputFile,
                    nullptr,
                    processExitProperly);
}

bool ProcessCaller::call(const QString &programName,
                         const QString &processPath,
                         const QStringList &arguments,
                         QString *stdOutput,
                         bool logProcessError,
                         int timeoutInMs,
                         bool *processExitProperly)
{
    ProcessCaller process(programName);

    RETURN_IF_FALSE(process.initProcess(processPath));

    return process.callPriv(arguments,
                            logProcessError,
                            timeoutInMs,
                            nullptr,
                            stdOutput,
                            processExitProperly);
}

bool ProcessCaller::call(const QString &programName,
                         const QString &processPath,
                         const QStringList &arguments,
                         QFile &stdOutputFile,
                         bool logProcessError,
                         int timeoutInMs,
                         bool *processExitProperly)
{
    ProcessCaller process(programName);

    RETURN_IF_FALSE(process.initProcess(processPath));

    return process.callPriv(arguments,
                            logProcessError,
                            timeoutInMs,
                            &stdOutputFile,
                            nullptr,
                            processExitProperly);
}

bool ProcessCaller::callPriv(const QStringList &arguments,
                             bool logProcessError,
                             int overrideTimeoutInMs,
                             QFile* stdOutputFile,
                             QString* stdOutput,
                             bool *processExitProperly)
{
    if(_processPath.isEmpty())
    {
        qWarning() << "The path of the program: " << _programName << ", is empty; therefore we "
                   << "can't call the process";
        return false;
    }

    int timeoutInMs = _defaultTimeout;

    if(processExitProperly != nullptr)
    {
        *processExitProperly  = false;
    }

    if(overrideTimeoutInMs >= 0)
    {
        // Use the timeout overloaded
        timeoutInMs = overrideTimeoutInMs;
    }

    QStringList tmpArguments{_defaultArguments};

    tmpArguments.append(arguments);

    qDebug() << "Try to call program: " << _programName << " " << tmpArguments.join(" ");

    QProcess process;

    bool waitForStarted = false;
    bool waitForFinished = false;

    auto tokenStart = connect(&process, &QProcess::started,
                              this, [&waitForStarted]() { waitForStarted = true; });
    auto tokenFinish = connect(&process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
                               this, [&waitForFinished]() { waitForFinished = true; });

    if(stdOutputFile != nullptr)
    {
        process.setStandardOutputFile(stdOutputFile->fileName());
    }
    else if(stdOutput == nullptr)
    {
        // There is no expected standart output
        process.setStandardOutputFile(QProcess::nullDevice());
    }

    process.start(_processPath, tmpArguments);

    if(!WaitHelper::pseudoWait(waitForStarted, timeoutInMs))
    {
        disconnect(tokenStart);
        disconnect(tokenFinish);

        qWarning() << "Program: " << _programName << ", not started";
        qWarning() << "StandartOutput: "<< process.readAllStandardOutput()
                                                            .right(ProcessLogCharLimitToDisplay);
        qWarning() << "ErrorOutput: " << process.readAllStandardError()
                                                            .right(ProcessLogCharLimitToDisplay);
        return false;
    }

    disconnect(tokenStart);

    if(!WaitHelper::pseudoWait(waitForFinished, timeoutInMs))
    {
        disconnect(tokenFinish);

        if(logProcessError)
        {
            qWarning() << "Program: " << _programName
                       << ", didn't end as expected, timeout occurred: " << process.error();
            qWarning() << "StandartOutput: "<< process.readAllStandardOutput()
                                                            .right(ProcessLogCharLimitToDisplay);
            qWarning() << "ErrorOutput: " << process.readAllStandardError()
                                                            .right(ProcessLogCharLimitToDisplay);
        }

        return false;
    }

    disconnect(tokenFinish);

    if((process.exitStatus() != QProcess::NormalExit) || (process.exitCode() != 0))
    {
        if(processExitProperly == nullptr && logProcessError)
        {
            // If processExitProperly isn't equal to nullptr, we can deduce that's an expected case
            // (and managed) therefore it's useless to log the error output
            qWarning() << "Program: " << _programName << " didn't end as expected : "
                       << process.error() << ", exit code: " << process.exitCode();
            qWarning() << "StandartOutput: "<< process.readAllStandardOutput()
                                                            .right(ProcessLogCharLimitToDisplay);
            qWarning() << "ErrorOutput: " << process.readAllStandardError()
                                                            .right(ProcessLogCharLimitToDisplay);
        }

        return (processExitProperly != nullptr);
    }

    if(processExitProperly != nullptr)
    {
        *processExitProperly  = true;
    }

    if(stdOutput != nullptr)
    {
        if(stdOutputFile == nullptr)
        {
            stdOutput->append(process.readAllStandardOutput());
        }
        else
        {
            // If a std output file is given, the QProcess doesn't return the content by the method
            // readAllStandardOutput
            if(!stdOutputFile->open(QIODevice::ReadOnly))
            {
                qWarning() << "A problem occurred when tried to read the content of the standard "
                           << "output file, created by program: " << _programName;
                return false;
            }

            stdOutput->append(stdOutputFile->readAll());

            stdOutputFile->close();
        }
    }

    return true;
}
