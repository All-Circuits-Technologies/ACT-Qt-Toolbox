// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "afilelogsstrategy.hpp"

#include <QDebug>
#include <QDir>
#include <QFile>


AFileLogsStrategy::AFileLogsStrategy(const QString &folderPath, QObject *parent) :
    QObject(parent),
    _folderPath(folderPath)
{
}

bool AFileLogsStrategy::start()
{
    _loggingStarted = true;
    return true;
}

bool AFileLogsStrategy::stop()
{
    _loggingStarted = false;
    return true;
}
