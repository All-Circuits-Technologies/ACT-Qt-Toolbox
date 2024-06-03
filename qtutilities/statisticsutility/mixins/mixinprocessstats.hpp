// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QDebug>
#include <QHash>
#include <QString>

#include "models/statscategory.hpp"


/** @brief This mixin has to be used to add statistics to a process
    @note It's also useful to get a string representation which can be displayed in sequence logs */
class MixinProcessStats
{
    protected:
        /** @brief Class constructor
            @param description This is the description to display at start of statistics logs */
        explicit MixinProcessStats(const QString &description = DefaultStatsDescriptionLogsDisplay);

    public:
        /** @brief Class destructor */
        virtual ~MixinProcessStats();

    protected:
        /** @brief Test if the parent process has registered elements to follow
            @return True if there is at least one statistic element registered in the mixin */
        bool isThereRegisteredStats() const;

        /** @brief Register a new counter
            @param key The key of the counter to register
            @param description The counter description */
        CounterStatsCategory &registerCounter(const QString &key,
                                              const QString &description = QString());

        /** @brief Unregister an existent counter
            @param key The key of the counter to unregister */
        void unregisterCounter(const QString &key);

        /** @brief Access the counter category
            @param key The key of the category to access
            @return The counter category found, nullptr if none exist */
        CounterStatsCategory* accessCounterCategory(const QString &key);

        /** @brief Format the statistics to be displayed in logs
            @return The formatted statistics */
        QString formatStatsToBeDisplayedInLogs();

    private:
        /** @brief Register a new statistic category
            @note If the element already exists, nothing is created and the existent element is
                  returned
            @param key The key of the category to register
            @param factory The factory used to create a new stats category
            @param stats The statistic categories where to find the element
            @return The registered statistic category */
        template<class T>
        T &registerStats(const QString &key,
                         const std::function<T*()> &factory,
                         QHash<QString, T*> &stats);

        /** @brief Unregister an existent statistic category
            @param key The key of the category to unregister
            @param stats The satistic categories map */
        template<class T>
        void unregisterStats(const QString &key, QHash<QString, T*> &stats);

        /** @brief Format the statistics category map to be displayed
            @param statsTitle The statistics title to display in logs
            @param stats The statistics category map
            @return The formatted statistics categories */
        template<class T>
        QVector<QString> formatStatsType(const QString &statsTitle,
                                         const QHash<QString, T*> &stats);

    private:
        static const constexpr char *NoStatsLogsDisplay = "No statistics";
        static const constexpr char *DefaultStatsDescriptionLogsDisplay = "Statistics results";

        static const constexpr int StatsCategoryTypeTabNbBefore = 1;
        static const constexpr char *StatsCategoryTypeFormatLogsDisplay = "%1%2:";
        static const constexpr char *StatsGlobalFormatLogsDisplay = "%1:";
        static const constexpr char *CategorySeparatorLogsDisplay = "\n";
        static const constexpr int SpacesByTab = 2;

        static const constexpr char *CounterStatsCategoryName = "Counter statistics";

    private:
        QHash<QString, CounterStatsCategory*> _counters;
        QString _description;
};

template<class T>
T &MixinProcessStats::registerStats(const QString &key,
                                    const std::function<T*()> &factory,
                                    QHash<QString, T*> &stats)
{
    if(stats.contains(key))
    {
        qInfo() << "A statistic already exists with the key: " << key << ", we return the "
                << "existent one";
        return *stats[key];
    }

    T *category = factory();
    stats[key] = category;

    return *category;
}

template<class T>
void MixinProcessStats::unregisterStats(const QString &key, QHash<QString, T *> &stats)
{
    if(!stats.contains(key))
    {
        qInfo() << "The statistics category: " << key << ", is not known; therefore no need to "
                << "unregister it.";
        return;
    }

    delete stats[key];
    stats.remove(key);
}

template<class T>
QVector<QString> MixinProcessStats::formatStatsType(const QString &statsTitle,
                                                    const QHash<QString, T *> &stats)
{
    QVector<QString> lines;

    lines.append(QString(StatsCategoryTypeFormatLogsDisplay)
        .arg(QString{SpacesByTab * StatsCategoryTypeTabNbBefore, QChar::Space}, statsTitle));

    for(auto citer = stats.cbegin(); citer != stats.cend(); ++citer)
    {
        lines.append(citer.value()->toDisplayStringLines(SpacesByTab));
    }

    return lines;
}
