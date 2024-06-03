// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "onefileperobjectlogsstrategy.hpp"

#include <QDateTime>
#include <QDebug>


OneFilePerObjectLogsStrategy::OneFilePerObjectLogsStrategy(
                                                        const QString &folderPath,
                                                        const QString &fileSuffix,
                                                        LoggingStrategyOption::Enums folderStrategy,
                                                        qint64 maxFolderLimitInMo,
                                                        QObject *parent) :
    AOneFileLogsStrategy(folderPath, folderStrategy, maxFolderLimitInMo, parent)
{
    setFileSuffix(fileSuffix);
}

void OneFilePerObjectLogsStrategy::setFileSuffix(const QString &fileSuffix)
{
    QString tmp = fileSuffix.section('.', -1);

    if(!tmp.isNull())
    {
        _fileSuffix = tmp;
    }
}

bool OneFilePerObjectLogsStrategy::manageFileCreationIfNeeded(const LoggingOptions &options)
{
    if(options.isEmpty())
    {
        qWarning() << "Need the object identifier to create a new log file";
        return false;
    }

    QString id;
    if(!LoggingOption::getId(options, id))
    {
        return false;
    }

    if(_objectIdentifier == id)
    {
        // Same object, keep the same file
        return true;
    }

    _objectIdentifier = id;

    return createFile(QString("%1-%2.%3").arg(
                                        QDateTime::currentDateTimeUtc().toString(DateTimeFormat),
                                        id,
                                        _fileSuffix));
}

QStringList OneFilePerObjectLogsStrategy::getLogFilenameFilters()
{
    return { "*." + _fileSuffix };
}
