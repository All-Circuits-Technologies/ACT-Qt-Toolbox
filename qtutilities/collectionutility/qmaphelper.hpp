// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QMap>
#include <QSharedPointer>
#include <QVector>


/** @brief This class contains helper methods for QMap class */
class QMapHelper
{
    public:
        /** @brief Deleted constructor */
        explicit QMapHelper() = delete;

    public:
        /** @brief Get the QMap keys
            @note This method does the same thing (in the same way) that QMap::keys method but
                  create a QVector instead of a QList
            @param map The QMap to get keys from
            @return The QMap keys */
        template<class Key, class Y>
        static QVector<Key> getKeys(const QMap<Key, Y> &map);

        /** @brief Get the QMap keys in const
            @note This method does the same thing (in the same way) that QMap::keys method but
                  create a QVector instead of a QList
            @note Change the constness of the element
            @param map The QMap to get keys from
            @return The QMap keys in constant */
        template<class Key, class Y>
        static QVector<const Key> getConstKeys(const QMap<Key, Y> &map);

        /** @brief Get the QMap keys and change the QSharedPointer element to constant
            @note This method does the same thing (in the same way) that QMap::keys method but
                  create a QVector instead of a QList.
            @note Change the constness of the element
            @param map The QMap to get keys from
            @return The QMap keys in constant */
        template<class T, class Y>
        static QVector<QSharedPointer<const T>> getConstKeys(
                                                            const QMap<QSharedPointer<T>, Y> &map);

        /** @brief Get the QMap values
            @note This method does the same thing (in the same way) that QMap::values method but
                  create a QVector instead of a QList.
            @param map The QMap to get values from
            @return The QMap values */
        template<class Key, class Y>
        static QVector<Y> getValues(const QMap<Key, Y> &map);

        /** @brief Get the QMap values in const
            @note This method does the same thing (in the same way) that QMap::values method but
                  create a QVector instead of a QList.
            @note Change the constness of the element
            @param map The QMap to get values from
            @return The QMap values in constant */
        template<class Key, class Y>
        static QVector<const Y> getConstValues(const QMap<Key, Y> &map);
};

template<class Key, class Y>
QVector<Key> QMapHelper::getKeys(const QMap<Key, Y> &map)
{
    QVector<Key> res;
    res.reserve(map.size());

    typename QMap<Key,Y>::const_iterator iter = map.cbegin();

    while(iter != map.end())
    {
        res.append(iter.key());
        ++iter;
    }

    return res;
}

template<class Key, class Y>
QVector<const Key> QMapHelper::getConstKeys(const QMap<Key, Y> &map)
{
    QVector<const Key> res;
    res.reserve(map.size());

    typename QHash<Key,Y>::const_iterator iter = map.cbegin();

    while(iter != map.cend())
    {
        res.append(iter.key());
        ++iter;
    }

    return res;
}

template<class T, class Y>
QVector<QSharedPointer<const T>> QMapHelper::getConstKeys(const QMap<QSharedPointer<T>, Y> &map)
{
    QVector<QSharedPointer<const T>> res;
    res.reserve(map.size());

    typename QMap<QSharedPointer<T>,Y>::const_iterator iter = map.cbegin();

    while(iter != map.cend())
    {
        res.append(qSharedPointerConstCast<const T>(iter.key()));
        ++iter;
    }

    return res;
}

template<class Key, class Y>
QVector<Y> QMapHelper::getValues(const QMap<Key, Y> &map)
{
    QVector<Y> res;
    res.reserve(map.size());

    typename QMap<Key,Y>::const_iterator iter = map.cbegin();

    while(iter != map.cend())
    {
        res.append(iter.value());
        ++iter;
    }

    return res;
}

template<class Key, class Y>
QVector<const Y> QMapHelper::getConstValues(const QMap<Key, Y> &map)
{
    QVector<const Y> res;
    res.reserve(map.size());

    typename QMap<Key,Y>::const_iterator iter = map.cbegin();

    while(iter != map.cend())
    {
        res.append(iter.value());
        ++iter;
    }

    return res;
}
