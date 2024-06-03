// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "loggingstrategy.hpp"

#include <QMetaEnum>


void LoggingStrategy::RegisterMetaType()
{
    qRegisterMetaType<LoggingStrategy::Enum>("LoggingStrategy::Enum");
}

QString LoggingStrategy::toString(LoggingStrategy::Enum value)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(value));
}
