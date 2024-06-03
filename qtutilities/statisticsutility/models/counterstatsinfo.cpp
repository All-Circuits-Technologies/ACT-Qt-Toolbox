// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "counterstatsinfo.hpp"


CounterStatsInfo::CounterStatsInfo(QObject *parent) :
    AStatsInfo{parent}
{
}

CounterStatsInfo::~CounterStatsInfo()
{
}

QVector<QString> CounterStatsInfo::toDisplayStringLines() const
{
    return { QString::number(_counter) };
}
