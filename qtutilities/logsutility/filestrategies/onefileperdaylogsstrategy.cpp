// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "onefileperdaylogsstrategy.hpp"

#include <QDebug>
#include <QDir>
#include <QFile>


OneFilePerDayLogsStrategy::OneFilePerDayLogsStrategy(const QString &folderPath,
                                                     const QString &fileNameFormat,
                                                     LoggingStrategyOption::Enums folderStrategy,
                                                     qint64 maxFolderLimitInMo,
                                                     QObject *parent) :
    AOneFileLogsStrategy(folderPath, folderStrategy, maxFolderLimitInMo, parent),
    _fileNameFormat(fileNameFormat)
{
}

bool OneFilePerDayLogsStrategy::manageFileCreationIfNeeded(const LoggingOptions &options)
{
    Q_UNUSED(options)

    QDate currentDate = QDate::currentDate();

    if(_currentFileDate == currentDate)
    {
        return true;
    }

    QString filename = QString("%1-%2").arg(currentDate.toString("yyyyMMdd"), _fileNameFormat);

    if(!AOneFileLogsStrategy::createFile(filename))
    {
        return false;
    }

    _currentFileDate = currentDate;

    return true;
}

QStringList OneFilePerDayLogsStrategy::getLogFilenameFilters()
{
    QStringList nameFilters;

    QString suffix = QFileInfo(_fileNameFormat).suffix();

    if(!suffix.isEmpty())
    {
        nameFilters.append("*." + suffix);
    }

    return nameFilters;
}
