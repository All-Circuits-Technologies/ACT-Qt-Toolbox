// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QMetaType>
#include <QVariantHash>
#include <QVector>


/** @brief Special types to manage differently the Qt objets : QJsonObject, QJsonArray and
           QJsonValue
    @note The QJsonObject is replaced by a QVariantHash
    @note qRegisterMetaType(JsonArray) has to be used if you want to use the object in
          signals/slots */

using JsonObject = QVariantHash;

using JsonArray = QVector<QVariant>;

Q_DECLARE_METATYPE(JsonArray)
