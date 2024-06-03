// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QDebug>
#include <QHash>
#include <QVariant>
#include <QVector>


/** @brief This class contains helper methods for QVector class */
class QVectorHelper
{
    public:
        /** @brief Deleted constructor */
        explicit QVectorHelper() = delete;

    public:
        /** @brief Cast the elements of a specific type to QVariant
            @note Of course, the specific type has to be convertible to QVariant, or the function
                  will return false
            @note The elements are set in the same order
            @param vector The elements to cast
            @param castedVector The elements casted
            @return True if no problem occurs */
        template<class T>
        static bool castToQVariantVector(const QVector<T> &vector, QVector<QVariant> &castedVector);

        /** @brief Cast the elements of a specific type to QVariant
            @note Of course, the specific type has to be convertible to QVariant, or the function
                  will return false
            @note The elements are set in the same order
            @param vector The elements to cast
            @param castedVector The elements casted
            @return True if no problem occurs */
        template<class T>
        static bool castToQVariantVector(const QList<T> &list, QVector<QVariant> &castedVector);

        /** @brief Cast the elements from a QVariant to the given type
            @note Of course, the QVariant has to be convertible to the type given, or the function
                  will return false
            @note The elements are set in the same order
            @param vector The elements to cast
            @param castedVector The elements casted
            @return True if no problem occurs */
        template<class T>
        static bool castFromQVariantVector(const QVector<QVariant> &vector,
                                           QVector<T> &castedVector);

        /** @brief Cast the element from QObject to the wanted type
            @note Of course the wanted type has to derive from QObject (if not, this fire an error
                  on build)
            @note The elements are set in the same order
            @param vector The elements to cast
            @param castedVector The elements casted
            @return True if no problem occurs */
        template<class T>
        static QVector<T*> castFromQObjectVector(const QVector<QObject*> &vector);

        /** @brief Cast the @ref baseVectorList to the targetted derived class
            @note If the real class doesn't derived from the base class, this will return an empty list with the ok par */
        template<class T, class Y>
        static QVector<T*> castFromBaseClass(const QVector<Y*> &baseVectorList, bool *ok = nullptr);

        /** @brief Cast const the elements stored in a QSharedPointer
            @note The elements are set in the same order
            @param vector The elements to cast
            @return True The elements casted */
        template<class T>
        static QVector<QSharedPointer<const T>> getConstCast(
                                                        const QVector<QSharedPointer<T>> &vector);

        /** @brief Compare two vectors together
            @note If the list is unordered, the test will count the occurrence number of elements
                  for each list and compare them
            @param firstList The first list to compare
            @param secondList The second list to compare with
            @param orderedList - If true, we consider that the lists are ordered, therefore:
                                    firstList[0] == secondList[0] , etc...
                               - If false, we consider that the lists are unordered, for instance:
                                 [ A, A, B, C, B] and [ B, A, C, B, A ] are equals
            @return True if the list are equals */
        template<class T>
        static bool compareList(const QVector<T> &firstList,
                                const QVector<T> &secondList,
                                bool orderedList = true);

        /** @brief Count the occurrence of each different element contains in the vector
            @param listToCountElement The list to count the elements occurrence
            @return The occurrences of elements in the list */
        template<class T>
        static QHash<T, int> countElements(const QVector<T> &listToCountElement);

        /** @brief Joins all the string list's strings into a single string with each element
                   separated by the given separator (which can be an empty string).
            @note The methods acts like the QStringList::join method
            @param stringList The list to join its elements
            @param separator The separator to add between values (it can be empty)
            @return The result of the join method */
        static QString join(const QVector<QString> &stringList, const QString &separator);

    private:
        /** @brief Test if the type given can be used by a QVariant
            @return True if no problem occurs */
        template<class T>
        static bool testIfTypeCanBeCasted();
};

template<class T>
bool QVectorHelper::castToQVariantVector(const QList<T> &list, QVector<QVariant> &castedVector)
{
    if(!testIfTypeCanBeCasted<T>())
    {
        return false;
    }

    castedVector.reserve(list.length());

    typename QList<T>::const_iterator iter = list.cbegin();

    for(;iter != list.cend(); ++iter)
    {
        castedVector.append(QVariant::fromValue<T>(*iter));
    }

    return true;
}

template<class T>
bool QVectorHelper::castToQVariantVector(const QVector<T> &vector, QVector<QVariant> &castedVector)
{
    if(!testIfTypeCanBeCasted<T>())
    {
        return false;
    }

    castedVector.reserve(vector.length());

    typename QVector<T>::const_iterator iter = vector.cbegin();

    for(;iter != vector.cend(); ++iter)
    {
        castedVector.append(QVariant::fromValue<T>(*iter));
    }

    return true;
}

template<class T>
bool QVectorHelper::castFromQVariantVector(const QVector<QVariant> &vector,
                                           QVector<T> &castedVector)
{
    if(!testIfTypeCanBeCasted<T>())
    {
        return false;
    }

    castedVector.reserve(vector.length());

    typename QVector<QVariant>::const_iterator iter = vector.cbegin();

    for(;iter != vector.cend(); ++iter)
    {
        castedVector.append(iter->value<T>());
    }

    return true;
}

template<class T>
QVector<T*> QVectorHelper::castFromQObjectVector(const QVector<QObject *> &vector)
{
    static_assert(std::is_base_of<QObject, T>::value, "Targetted object must be a QObject");

    QVector<T*> castedVector;

    castedVector.reserve(vector.length());

    typename QVector<QObject *>::const_iterator iter = vector.cbegin();

    for(;iter != vector.cend(); ++iter)
    {
        castedVector.append(qobject_cast<T*>(*iter));
    }

    return castedVector;
}

template<class T, class Y>
QVector<T*> QVectorHelper::castFromBaseClass(const QVector<Y*> &baseVectorList, bool *ok)
{
    static_assert(std::is_base_of<Y, T>::value, "Targetted object must be derived from the base "
                                                "type given");

    QVector<T*> castedVector;

    castedVector.reserve(baseVectorList.length());

    typename QVector<Y*>::const_iterator iter = baseVectorList.cbegin();

    for(;iter != baseVectorList.cend(); ++iter)
    {
        T* castedObject = qobject_cast<T*>(*iter);

        if(Q_UNLIKELY(castedObject == nullptr && *iter != nullptr))
        {
            qWarning() << "One element stored in the base list hadn't the targetted type";

            if(ok != nullptr)
            {
                *ok = false;
            }

            return {};
        }

        castedVector.append(castedObject);
    }

    if(ok != nullptr)
    {
        *ok = true;
    }

    return castedVector;
}

template<class T>
QVector<QSharedPointer<const T>> QVectorHelper::getConstCast(
                                                        const QVector<QSharedPointer<T>> &vector)
{
    QVector<QSharedPointer<const T>> castedVector;
    castedVector.reserve(vector.length());

    typename QVector<QSharedPointer<T>>::const_iterator iter = vector.cbegin();

    for(;iter != vector.cend(); ++iter)
    {
        castedVector.append(qSharedPointerConstCast<const T>(*iter));
    }

    return castedVector;
}

template<class T>
bool QVectorHelper::compareList(const QVector<T> &firstList,
                                const QVector<T> &secondList,
                                bool orderedList)
{
    if(orderedList)
    {
        return (firstList == secondList);
    }

    if(firstList.length() != secondList.length())
    {
        return false;
    }

    return (countElements(firstList) == countElements(secondList));
}

template<class T>
QHash<T, int> QVectorHelper::countElements(const QVector<T> &listToCountElement)
{
    QHash<T, int> counter;
    typename QVector<T>::const_iterator iter = listToCountElement.cbegin();

    for(; iter != listToCountElement.cend(); ++iter)
    {
        counter[*iter]++;
    }

    return counter;
}

template<class T>
bool QVectorHelper::testIfTypeCanBeCasted()
{
    if(qMetaTypeId<T>() == QMetaType::UnknownType)
    {
        qWarning() << "The type: " << typeid(T).name() << ", hasn't been registered with "
                   << "Q_DECLARE_METATYPE, can't cast to QVariant";
        return false;
    }

    return true;
}
