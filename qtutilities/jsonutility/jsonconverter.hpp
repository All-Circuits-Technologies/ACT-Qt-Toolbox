// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "jsontypedefs.hpp"

class QJsonObject;


/** @brief Convert the QJsonArray and QJsonObject to QVariantHash and QVector<QVariant>
    @note It already exists a method: toVariantHash in the QJsonObject but for each child it
          uses the toVariant() method of the QJsonValue, which transform eash QJsonObject in
          QVariantMap. Therefore if you use the Qt method, you can have a mix of QVariantHash
          and QVariantMap.
    @note There is no method for QJsonArray to transform it in QVector. There is one to convert
          into a QList.
    @note All this methods are reinterpreting what Qt does in their source code for toVariant()
          and toVariantHash() and toVariantList(), but using QHash and QVector */
namespace JsonConverter
{
    /** @brief Convert, with a waterfall effect, a QJsonObject into a JsonObject
        @param object The object to convert
        @return The object converted */
    JsonObject convertJsonObject(const QJsonObject &object);

    /** @brief Convert, with a waterfall effect, a QJsonArray into a JsonArray
        @param array The array to convert
        @return The array converted */
    JsonArray convertJsonArray(const QJsonArray &array);

    /** @brief Convert, with a waterfall effect, a QJsonValue into a QVariant
        @param value The JsonValue to convert
        @return The value converted */
    QVariant convertValue(const QJsonValue &value);
};
