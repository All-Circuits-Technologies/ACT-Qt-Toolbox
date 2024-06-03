// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "jsoncomparator.hpp"

#include "jsonconstants.hpp"


// The method is called recursively to test the children elements
// NOLINTNEXTLINE(misc-no-recursion)
bool JsonComparator::isEqual(const JsonObject &firstObject, const JsonObject &secondObject)
{
    if(firstObject.count() != secondObject.count())
    {
        return false;
    }

    JsonObject::const_iterator iter = firstObject.cbegin();

    for(; iter != firstObject.cend(); iter++)
    {
        if(!secondObject.contains(iter.key()))
        {
            return false;
        }

        if(!isEqual(iter.value(), secondObject.value(iter.key())))
        {
            return false;
        }
    }

    return true;
}

// The method is called recursively to test the children elements
// NOLINTNEXTLINE(misc-no-recursion)
bool JsonComparator::isEqual(const JsonArray &firstArray, const JsonArray &secondArray)
{
    if(firstArray.length() != secondArray.length())
    {
        return false;
    }

    JsonArray tempSecArray = secondArray;

    JsonArray::const_iterator iter = firstArray.cbegin();
    for(; iter != firstArray.cend(); ++iter)
    {
        JsonArray::iterator iterSecArray;

        if(!findEquivalent(*iter, tempSecArray, iterSecArray))
        {
            return false;
        }

        tempSecArray.erase(iterSecArray);
    }

    return true;
}

// The method is called recursively to test the children elements
// NOLINTNEXTLINE(misc-no-recursion)
bool JsonComparator::isEqual(const QVariant &firstValue, const QVariant &secondValue)
{
    if(firstValue.canConvert<JsonObject>())
    {
        if(!secondValue.canConvert<JsonObject>())
        {
            return false;
        }

        return isEqual(firstValue.value<JsonObject>(), secondValue.value<JsonObject>());
    }

    if(firstValue.canConvert<JsonArray>())
    {
        if(!secondValue.canConvert<JsonArray>())
        {
            return false;
        }

        return isEqual(firstValue.value<JsonArray>(), secondValue.value<JsonArray>());
    }

    int firstValueUserType = firstValue.userType();
    int secValueUserType = secondValue.userType();

    // Test numbers
    bool isFirstNumber = JsonConstants::numberMetaUserTypes.contains(firstValueUserType);
    bool isSecNumber = JsonConstants::numberMetaUserTypes.contains(secValueUserType);

    // If the types aren't equals, we considere that values aren't equal (because some types can
    // have strange behavior with '==' and '!=' operator)
    // It exists an exception for numbers: if both types are numbers, we don't test if the types
    // are equals. For a json object it only exists one number type
    // It exists an other exception if a converter function has been defined for converting the
    // types together
    if((!isFirstNumber || !isSecNumber) &&
       firstValueUserType != secValueUserType &&
       !QMetaType::hasRegisteredConverterFunction(firstValueUserType, secValueUserType) &&
       !QMetaType::hasRegisteredConverterFunction(secValueUserType, firstValueUserType))
    {
        // The types are not equals
        return false;
    }

    return (firstValue == secondValue);
}

// The method is called recursively to test the children elements
// NOLINTNEXTLINE(misc-no-recursion)
bool JsonComparator::findEquivalent(const QVariant &elementToFindEquivalent,
                                    JsonArray &secondArray,
                                    JsonArray::iterator &elementFoundIter)
{
    JsonArray::iterator secIter = secondArray.begin();
    JsonArray::iterator objFound;

    while(findFirstElementInJsonArray(secondArray,
                                      secIter,
                                      elementToFindEquivalent.userType(),
                                      objFound))
    {
        if(isEqual(elementToFindEquivalent, *objFound))
        {
            // An equivalent object has been found, no need to look for this object again
            elementFoundIter = objFound;
            return true;
        }

        if(objFound != secondArray.end())
        {
            secIter = objFound + 1;
        }
    }

    return false;
}

bool JsonComparator::findFirstElementInJsonArray(JsonArray &jsonArray,
                                                 const JsonArray::iterator &searchFromThisIter,
                                                 int userTypeToCompareWith,
                                                 JsonArray::iterator &elementFoundIter)
{
    JsonArray::iterator iter = searchFromThisIter;
    bool isUserTypeNumber = JsonConstants::numberMetaUserTypes.contains(userTypeToCompareWith);

    for(; iter != jsonArray.end(); iter++)
    {
        int iterUserType = iter->userType();

        if((isUserTypeNumber && JsonConstants::numberMetaUserTypes.contains(iterUserType))  ||
           (userTypeToCompareWith == iterUserType)                                          ||
           QMetaType::hasRegisteredConverterFunction(iterUserType, userTypeToCompareWith)   ||
           QMetaType::hasRegisteredConverterFunction(userTypeToCompareWith, iterUserType))
        {
            elementFoundIter = iter;
            return true;
        }
    }

    return false;
}
