// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "loggingoption.hpp"

#include <QMetaEnum>


void LoggingOption::RegisterMetaTypes()
{
    qRegisterMetaType<LoggingOption::Enum>("LoggingOption::Enum");
    qRegisterMetaType<LoggingOptions>("LoggingOptions");
}

QString LoggingOption::toString(LoggingOption::Enum value)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(value));
}

bool LoggingOption::isAtStart(const QHash<LoggingOption::Enum, QVariant> &options,
                              bool &value,
                              bool canBeEmpty)
{
    return getOptionValue(options, LoggingOption::AtStart, value, canBeEmpty);
}

bool LoggingOption::getId(const QHash<LoggingOption::Enum, QVariant> &options,
                          QString &value,
                          bool canBeEmpty)
{
    return getOptionValue(options, LoggingOption::Id, value, canBeEmpty);
}
