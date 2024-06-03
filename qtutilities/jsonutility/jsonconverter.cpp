// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "jsonconverter.hpp"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>


// The method is called recursively to test the children elements
// NOLINTNEXTLINE(misc-no-recursion)
JsonObject JsonConverter::convertJsonObject(const QJsonObject &object)
{
    JsonObject jsonObject;

    jsonObject.reserve(object.length());

    for(auto citer = jsonObject.cbegin(); citer != jsonObject.cend(); ++citer)
    {
        QString key = citer.key();
        jsonObject.insert(key, convertValue(object.value(key)));
    }

    return jsonObject;
}

// The method is called recursively to test the children elements
// NOLINTNEXTLINE(misc-no-recursion)
JsonArray JsonConverter::convertJsonArray(const QJsonArray &array)
{
    JsonArray valueArray;

    valueArray.reserve(array.count());
    QJsonArray::const_iterator constIter = array.constBegin();

    for(; constIter != array.constEnd(); ++constIter)
    {
        valueArray.append(convertValue(*constIter));
    }

    return valueArray;
}

// The method is called recursively to test the children elements
// NOLINTNEXTLINE(misc-no-recursion)
QVariant JsonConverter::convertValue(const QJsonValue &value)
{
    if(value.isNull())
    {
        return QVariant::fromValue(nullptr);
    }

    if(value.isBool())
    {
        return value.toBool();
    }

    if(value.isDouble())
    {
        return value.toDouble();
    }

    if(value.isString())
    {
        return value.toString();
    }

    if(value.isObject())
    {
        return QVariant::fromValue(convertJsonObject(value.toObject()));
    }

    if(value.isArray())
    {
        return QVariant::fromValue(convertJsonArray(value.toArray()));
    }

    return {};
}
