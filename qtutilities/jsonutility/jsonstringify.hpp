// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "jsontypedefs.hpp"


/** @brief This class helps to stringify a JsonObject or JsonArray to a JSON */
class JsonStringify
{
    public:
        /** @brief Deleted constructor */
        explicit JsonStringify() = delete;

    public:
        /** @brief Transform a JsonObject to a JSON compacted string
            @note Work in waterfall effect
            @param jsonObject The JsonObject to stringify
            @param compactStr The object stringified
            @return True if no problem occurs */
        static bool toCompactString(const JsonObject &jsonObject, QByteArray &compactStr);

        /** @brief Transform a JsonArray to JSON compacted string
            @note Work in waterfall effect
            @param jsonArray The JsonArray to stringify
            @param compactStr The object stringified
            @return True if no problem occurs */
        static bool toCompactString(const JsonArray &jsonArray, QByteArray &compactStr);

    private:
        /** @brief Stringify a QVariant
            @param value The value to stringify
            @param compactStr The value stringified
            @return True if no problem occurs */
        static bool convertValue(const QVariant &value, QByteArray &compactStr);
};
