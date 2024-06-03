// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "jsontypedefs.hpp"


/** @brief This class allowes to compare JsonObject and JsonArray */
class JsonComparator
{
    public:
        /** @brief Test if the two objects are equals
            @note For the JsonArray, we don't care about the order of the elements in the list
            @note For the JsonObject, we don't care about the order of the attributes in the object
            @param firstObject The first object to compare
            @param secondObject The second object to compare with
            @return True if the two objects are equals */
        static bool isEqual(const JsonObject &firstObject, const JsonObject &secondObject);

        /** @brief Test if the two arrays are equals
            @note For the JsonArray, we don't care about the order of the elements in the list
            @note For the JsonObject, we don't care about the order of the attributes in the object
            @param firstArray The first array to compare
            @param secondArray The second array to compare with
            @return True if the two arrays are equals */
        static bool isEqual(const JsonArray &firstArray, const JsonArray &secondArray);

    private:
        /** @brief Test if the two QVariant are equals
            @note For the JsonArray, we don't care about the order of the elements in the list
            @note For the JsonObject, we don't care about the order of the attributes in the object
            @note The method test if the QVariant user type are strictly the same, except for
                  Number types (@ref JsonComparator::NumberMetaUserTypes) and if a converter
                  function has been defined to convert properly the two variant
            @param firstValue The first value to compare
            @param secondValue The second value to compare
            @return True if the two QVariant are equals */
        static bool isEqual(const QVariant &firstValue, const QVariant &secondValue);

    private:
        /** @brief Find an element equivalent to the one given in the JsonArray
            @param elementToFindEquivalent The element to find equivalent in the array given
            @param secondArray The array to search in, in order to find the same element in
            @param elementFoundIter The iterator (in the second array) which corresponds to the
                                    the element given
            @return True if no problem occurs */
        static bool findEquivalent(const QVariant &elementToFindEquivalent,
                                   JsonArray &secondArray,
                                   JsonArray::iterator &elementFoundIter);

        /** @brief Search for the first element which has the same type given or can be properly
                   converted to the type wanted
            @note When the type wanted is contained in the Number list, the method will return the
                  first element found with a type in this same Number list
            @param jsonArray The array to search element in
            @param searchFromThisIter The finding begins from this iteration
            @param userTypeToCompareWith The user type to search in all the JsonArray values
            @param elementFoundIter The first element found is set in it
            @return True if no problem occurs*/
        static bool findFirstElementInJsonArray(JsonArray &jsonArray,
                                                const JsonArray::iterator &searchFromThisIter,
                                                int userTypeToCompareWith,
                                                JsonArray::iterator &elementFoundIter);
};
