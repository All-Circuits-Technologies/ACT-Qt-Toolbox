// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "loggingstrategyoption.hpp"

#include <QDebug>
#include <QMetaEnum>

LoggingStrategyOption::Enums LoggingStrategyOption::AllElements = {};
QVector<LoggingStrategyOption::Enum> LoggingStrategyOption::EnumList = {};
QHash<LoggingStrategy::Enum, QVector<LoggingStrategyOption::Enum>>
                                                        LoggingStrategyOption::StrategyOptions = {
    { LoggingStrategy::SaveLogsInFiles,         { File_OneFilePerDay,
                                                  File_OneFilePerObject,
                                                  File_SetMaxSizeLimit,
                                                  File_StoreInYearFolder,
                                                  File_StoreInMonthFolder,
                                                  File_StoreInDayFolder,
                                                  Glob_DisplayLogContext,
                                                  Glob_DisplayLogLevel,
                                                  Glob_DisplayDateTime   } },
    { LoggingStrategy::DisplayLogsInConsole,    { Glob_DisplayLogContext,
                                                  Glob_DisplayLogLevel,
                                                  Glob_DisplayDateTime   } }
};


void LoggingStrategyOption::RegisterMetaType()
{
    qRegisterMetaType<LoggingStrategyOption::Enum>("LoggingStrategies::Enum");
    qRegisterMetaType<LoggingStrategyOption::Enums>("LoggingStrategies::Enums");
}

QString LoggingStrategyOption::toString(Enum value)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(value));
}

const LoggingStrategyOption::Enums &LoggingStrategyOption::getAllFlags()
{
    if(AllElements == 0)
    {
        const QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

        for(int idx = 0; idx < metaEnum.keyCount(); idx++)
        {
            AllElements |= static_cast<Enum>(metaEnum.value(idx));
        }
    }

    return AllElements;
}

LoggingStrategyOption::Enums LoggingStrategyOption::getAllFlags(LoggingStrategy::Enum mainStrategy)
{
    if(!StrategyOptions.contains(mainStrategy))
    {
        qWarning() << "The strategy: " << LoggingStrategy::toString(mainStrategy)
                   << ", is not known as main strategy";
        return {};
    }

    Enums filteredStrategies = Enums();
    const QVector<LoggingStrategyOption::Enum> &options = StrategyOptions[mainStrategy];
    for(auto citer = options.cbegin(); citer != options.cend(); ++citer)
    {
        filteredStrategies.setFlag(*citer);
    }

    return filteredStrategies;
}

const QVector<LoggingStrategyOption::Enum> &LoggingStrategyOption::getAllEnums()
{
    if(EnumList.isEmpty())
    {
        const QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

        for(int idx = 0; idx < metaEnum.keyCount(); idx++)
        {
            EnumList.append(static_cast<Enum>(metaEnum.value(idx)));
        }
    }

    return EnumList;
}

LoggingStrategyOption::Enums LoggingStrategyOption::filterOptionsForSpecificStrategy(
                                                            LoggingStrategy::Enum mainStrategy,
                                                            LoggingStrategyOption::Enums strategies)
{
    if(!StrategyOptions.contains(mainStrategy))
    {
        qWarning() << "The strategy: " << LoggingStrategy::toString(mainStrategy)
                   << ", is not known as main strategy";
        return {};
    }

    Enums filteredStrategies = Enums();

    const QVector<LoggingStrategyOption::Enum> &options = StrategyOptions[mainStrategy];
    for(auto citer = options.cbegin(); citer != options.cend(); ++citer)
    {
        if(strategies.testFlag(*citer))
        {
            filteredStrategies.setFlag(*citer);
        }
    }

    return filteredStrategies;
}
