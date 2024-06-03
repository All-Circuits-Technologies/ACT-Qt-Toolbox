// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "parity.hpp"

#include <QMetaEnum>

QVector<Parity::Enum> Parity::enumList = {};


Parity::Enum Parity::parseFromString(const QString &strValue)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

    for(int idx = 0; idx < metaEnum.keyCount(); idx++)
    {
        QString value(metaEnum.key(idx));

        if(value.toLower() == strValue.toLower())
        {
            return static_cast<Enum>(metaEnum.value(idx));
        }
    }

    return Unknown;
}

QString Parity::toString(Parity::Enum parity)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(parity));
}

const QVector<Parity::Enum> &Parity::getAllEnums()
{
    if(enumList.isEmpty())
    {
        const QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

        for(int idx = 0; idx < metaEnum.keyCount(); idx++)
        {
            Enum value = static_cast<Enum>(metaEnum.value(idx));
            enumList.append(value);
        }
    }

    return enumList;
}
