// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QDebug>
#include <QHash>
#include <QString>

#include "models/astatsinfo.hpp"
#include "models/counterstatsinfo.hpp"


/** @brief This represents a category of stats information
    @note T has to be derived from @ref AStatsInfo class */
template<class T>
class StatsCategory
{
    public:
        /** @brief Class constructor
            @param key The key of the category
            @param description A specific description of the category */
        explicit StatsCategory(const QString &key, const QString &description = QString());

        /** @brief Class destructor */
        virtual ~StatsCategory();

    public:
        /** @brief Clear the list of stats information */
        void clear();

        /** @brief Test if a particular information is already contained in the category
            @param key The key of the element in the category
            @return True if the element is contained in the category */
        bool isContained(const QString &key) { return _statsElements.contains(key); }

        /** @brief Get the stats information linked to the given @ref key, if the element doesn't
                   exist, it creates a new one with the @ref defaultValue
            @note We considere that the deletion of the @ref defaultValue given is managed by this
                  method, even if, the value is not used because we found an existent one.
            @warning The given @ref defaultValue can't be null or it will crash the app
            @param key The key of the category element
            @param defaultValue The default value to use when the @ref key is not contained in the
                                category.
            @return The element got or created */
        T &getOrCreateValue(const QString &key, T *defaultValue);

        /** @brief This generates a list of displayable strings which represents the category and
                   its elements
            @param spacesNbByTab Specify the number of spaces by tab
            @return The list of lines to display and which represents the category elements */
        QVector<QString> toDisplayStringLines(int spacesNbByTab = 0) const;

    private:
        static const constexpr char *DefaultStatsCategoryDescription = "Statistics category [%1]";
        static const constexpr char *StatsCategoryDescFormat = "%1- %2:";
        static const constexpr int StatsCategoryTabNbBefore = 2;
        static const constexpr char *CategoryElementFormatForOne = "%1|- %2:\t%3";
        static const constexpr int CategoryElementTabNbBefore = 3;
        static const constexpr char *CategoryElementValueWhenNone = "nothing";
        static const constexpr char *CategoryElementFormatForMultiple = "%1|- %2:";
        static const constexpr int CategoryElementValueTabNbBefore = 4;
        static const constexpr char *CategoryElementValueFormatForMultiple = "%1%2";

    private:
        QString _key;
        QString _description;
        QHash<QString, T*> _statsElements;
};

using CounterStatsCategory = StatsCategory<CounterStatsInfo>;

template<class T>
StatsCategory<T>::StatsCategory(const QString &key, const QString &description) :
    _key{key},
    _description{description}
{
    static_assert(std::is_base_of<AStatsInfo, T>::value,
                  "The statistics information stored is not an AStatsInfo");
}

template<class T>
StatsCategory<T>::~StatsCategory()
{
    clear();
}

template<class T>
void StatsCategory<T>::clear()
{
    qDeleteAll(_statsElements.values());
    _statsElements.clear();
}

template<class T>
T &StatsCategory<T>::getOrCreateValue(const QString &key, T *defaultValue)
{
    if(defaultValue == nullptr)
    {
        qCritical() << "The given default value of the statistics category is null, the app is "
                    << "going to crash";
    }

    if(!_statsElements.contains(key))
    {
        _statsElements[key] = defaultValue;
    }
    else
    {
        delete defaultValue;
    }

    return *_statsElements[key];
}

template<class T>
QVector<QString> StatsCategory<T>::toDisplayStringLines(int spacesNbByTab) const
{
    QVector<QString> lines;

    QString firstElement;
    if(!_description.isEmpty())
    {
        firstElement = _description;
    }
    else
    {
        firstElement = QString(DefaultStatsCategoryDescription).arg(_key);
    }

    lines.append(QString(StatsCategoryDescFormat).arg(
        QString{spacesNbByTab * StatsCategoryTabNbBefore, QChar::Space},
        firstElement));

    for(auto citer = _statsElements.cbegin(); citer != _statsElements.cend(); ++citer)
    {
        const QVector<QString> toDisplay = citer.value()->toDisplayStringLines();
        int length = toDisplay.length();

        if(length == 0)
        {
            lines.append(QString(CategoryElementFormatForOne)
                             .arg(QString{spacesNbByTab * CategoryElementTabNbBefore, QChar::Space},
                                  _key,
                                  QString(CategoryElementValueWhenNone)));
        }
        else if(length == 1)
        {
            lines.append(QString(CategoryElementFormatForOne)
                             .arg(QString{spacesNbByTab * CategoryElementTabNbBefore, QChar::Space},
                                  _key,
                                  toDisplay.first()));
        }
        else
        {
            lines.append(QString(CategoryElementFormatForMultiple)
                             .arg(QString{spacesNbByTab * CategoryElementTabNbBefore, QChar::Space},
                                  _key));
            for(auto elemIter = toDisplay.cbegin(); elemIter != toDisplay.cend(); ++elemIter)
            {
                lines.append(QString(CategoryElementValueFormatForMultiple)
                    .arg(QString{spacesNbByTab * CategoryElementValueTabNbBefore, QChar::Space},
                         *elemIter));
            }
        }
    }

    return lines;
}
