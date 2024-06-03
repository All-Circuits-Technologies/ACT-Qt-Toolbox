// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QHash>
#include <QSharedPointer>
#include <QVector>


/** @brief This class contains helper methods for QHash class */
class QHashHelper
{
    public:
        /** @brief Deleted constructor */
        explicit QHashHelper() = delete;

    public:
        /** @brief Get the QHash keys
            @note This method does the same thing (in the same way) that QHash::keys method but
                  create a QVector instead of a QList
            @param hash The QHash to get keys from
            @return The QHash keys */
        template<class Key, class Y>
        static QVector<Key> getKeys(const QHash<Key, Y> &hash);

        /** @brief Get the QHash keys in const
            @note This method does the same thing (in the same way) that QHash::keys method but
                  create a QVector instead of a QList
            @note Change the constness of the element
            @param hash The QHash to get keys from
            @return The QHash keys in constant */
        template<class Key, class Y>
        static QVector<const Key> getConstKeys(const QHash<Key, Y> &hash);

        /** @brief Get the QHash keys and change the QSharedPointer element to constant
            @note This method does the same thing (in the same way) that QHash::keys method but
                  create a QVector instead of a QList.
            @note Change the constness of the element
            @param hash The QHash to get keys from
            @return The QHash keys in constant */
        template<class T, class Y>
        static QVector<QSharedPointer<const T>> getConstKeys(
                                                        const QHash<QSharedPointer<T>, Y> &hash);

        /** @brief Get the QHash values
            @note This method does the same thing (in the same way) that QHash::values method but
                  create a QVector instead of a QList.
            @param hash The QHash to get values from
            @return The QHash values */
        template<class Key, class Y>
        static QVector<Y> getValues(const QHash<Key, Y> &hash);

        /** @brief Get the QHash values in const
            @note This method does the same thing (in the same way) that QHash::values method but
                  create a QVector instead of a QList.
            @note Change the constness of the element
            @param hash The QHash to get values from
            @return The QHash values in constant */
        template<class Key, class Y>
        static QVector<const Y> getConstValues(const QHash<Key, Y> &hash);

        /** @brief This method compares the QHash keys
            @note The QHash keys aren't ordered, therefore we only test if there are the same
                  elements (and so the same element number) in the two QHash
            @param firstHash The first hash to test
            @param secondHash The second hash to test
            @return True if the two QHash have the same key elements */
        template<class Key, class Y, class Z>
        static bool compareKeys(const QHash<Key, Y> &firstHash, const QHash<Key, Z> &secondHash);
};

template<class Key, class Y>
QVector<Key> QHashHelper::getKeys(const QHash<Key, Y> &hash)
{
    QVector<Key> res;
    res.reserve(hash.size());

    typename QHash<Key,Y>::const_iterator iter = hash.cbegin();

    while(iter != hash.end())
    {
        res.append(iter.key());
        ++iter;
    }

    return res;
}

template<class Key, class Y>
QVector<const Key> QHashHelper::getConstKeys(const QHash<Key, Y> &hash)
{
    QVector<const Key> res;
    res.reserve(hash.size());

    typename QHash<Key,Y>::const_iterator iter = hash.cbegin();

    while(iter != hash.end())
    {
        res.append(iter.key());
        ++iter;
    }

    return res;
}

template<class T, class Y>
QVector<QSharedPointer<const T>> QHashHelper::getConstKeys(const QHash<QSharedPointer<T>, Y> &hash)
{
    QVector<QSharedPointer<const T>> res;
    res.reserve(hash.size());

    typename QHash<QSharedPointer<T>,Y>::const_iterator iter = hash.cbegin();

    while(iter != hash.end())
    {
        res.append(qSharedPointerConstCast<const T>(iter.key()));
        ++iter;
    }

    return res;
}

template<class Key, class Y>
QVector<Y> QHashHelper::getValues(const QHash<Key, Y> &hash)
{
    QVector<Y> res;
    res.reserve(hash.size());

    typename QHash<Key,Y>::const_iterator iter = hash.cbegin();

    while(iter != hash.end())
    {
        res.append(iter.value());
        ++iter;
    }

    return res;
}

template<class Key, class Y>
QVector<const Y> QHashHelper::getConstValues(const QHash<Key, Y> &hash)
{
    QVector<const Y> res;
    res.reserve(hash.size());

    typename QHash<Key,Y>::const_iterator iter = hash.cbegin();

    while(iter != hash.end())
    {
        res.append(iter.value());
        ++iter;
    }

    return res;
}

template<class Key, class Y, class Z>
bool QHashHelper::compareKeys(const QHash<Key, Y> &firstHash,
                              const QHash<Key, Z> &secondHash)
{
    if(firstHash.count() != secondHash.count())
    {
        // The keys list isn't equal
        return false;
    }

    QVector<Key> secondKeys = getKeys(secondHash);

    typename QHash<Key, Y>::const_iterator citer = firstHash.cbegin();
    for(; citer != firstHash.cend(); ++citer)
    {
        bool found = false;
        typename QVector<Key>::iterator secIter = secondKeys.begin();
        while(!found && (secIter != secondKeys.end()))
        {
            if(*secIter == *citer)
            {
                // Element is found, we remove it from the temporary list
                secIter = secondKeys.erase(secIter);
                found = true;
            }
            else
            {
                ++secIter;
            }
        }

        if(!found)
        {
            // The key hasn't been found, therefore the keys list are not identical
            return false;
        }
    }

    return true;
}
