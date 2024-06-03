// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "mixinprocessstats.hpp"

#include <QDebug>

#include "collectionutility/qvectorhelper.hpp"


MixinProcessStats::MixinProcessStats(const QString &description) :
    _description{description}
{
}

MixinProcessStats::~MixinProcessStats()
{
    qDeleteAll(_counters.values());
}

bool MixinProcessStats::isThereRegisteredStats() const
{
    return !_counters.isEmpty();
}

CounterStatsCategory &MixinProcessStats::registerCounter(const QString &key,
                                                         const QString &description)
{
    return registerStats<CounterStatsCategory>(
        key,
        [key, description]() { return new CounterStatsCategory(key, description); },
        _counters);
}

void MixinProcessStats::unregisterCounter(const QString &key)
{
    unregisterStats(key, _counters);
}

CounterStatsCategory *MixinProcessStats::accessCounterCategory(const QString &key)
{
    if(!_counters.contains(key))
    {
        return nullptr;
    }

    return _counters[key];
}

QString MixinProcessStats::formatStatsToBeDisplayedInLogs()
{
    if(!isThereRegisteredStats())
    {
        // Nothing to return, there is no stats
        return NoStatsLogsDisplay;
    }

    QVector<QString> lines;
    lines.append(QString(StatsGlobalFormatLogsDisplay).arg(_description));

    // Counters
    lines.append(formatStatsType(CounterStatsCategoryName, _counters));

    return QVectorHelper::join(lines, CategorySeparatorLogsDisplay);
}
