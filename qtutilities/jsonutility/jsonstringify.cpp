// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "jsonstringify.hpp"

#include "jsonconstants.hpp"


// The method is called recursively to test the children elements
// NOLINTNEXTLINE(misc-no-recursion)
bool JsonStringify::toCompactString(const JsonObject &jsonObject, QByteArray &compactStr)
{
    compactStr += "{";

    JsonObject::const_iterator iter = jsonObject.cbegin();

    for(; iter != jsonObject.cend(); ++iter)
    {
        if(iter != jsonObject.cbegin())
        {
            // We are doing this way to fix the usage of the deprecated iterator operator+ on
            // Qt 5.15
            compactStr += ",";
        }

        compactStr += QString("\"%1\":").arg(iter.key()).toLatin1();

        if(!convertValue(*iter, compactStr))
        {
            return false;
        }
    }

    compactStr += "}";

    return true;
}

// The method is called recursively to test the children elements
// NOLINTNEXTLINE(misc-no-recursion)
bool JsonStringify::toCompactString(const JsonArray &jsonArray, QByteArray &compactStr)
{
    compactStr += "[";

    JsonArray::const_iterator iter = jsonArray.cbegin();

    for(; iter != jsonArray.cend(); ++iter)
    {
        if(!convertValue(*iter, compactStr))
        {
            return false;
        }

        if((iter+1) != jsonArray.cend())
        {
            compactStr += ",";
        }
    }

    compactStr += "]";

    return true;
}

// The method is called recursively to test the children elements
// NOLINTNEXTLINE(misc-no-recursion)
bool JsonStringify::convertValue(const QVariant &value, QByteArray &compactStr)
{
    int type = value.userType();

    if(value.isNull())
    {
        compactStr += "null";
    }
    else if(value.canConvert<JsonObject>())
    {
        if(!toCompactString(value.value<JsonObject>(), compactStr))
        {
            return false;
        }
    }
    else if(value.canConvert<JsonArray>())
    {
        if(!toCompactString(value.value<JsonArray>(), compactStr))
        {
            return false;
        }
    }
    else if(JsonConstants::numberMetaUserTypes.contains(type))
    {
        compactStr += value.toString().toLatin1();
    }
    else if(type == QMetaType::Bool)
    {
        compactStr += (value.toBool() ? "true" : "false");
    }
    else
    {
        compactStr += QString("\"%1\"").arg(value.toString()).toLatin1();
    }

    return true;
}
