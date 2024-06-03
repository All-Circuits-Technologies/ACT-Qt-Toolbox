// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "flowcontrol.hpp"

#include <QMetaEnum>

QVector<FlowControl::Enum> FlowControl::enumList = {};


FlowControl::Enum FlowControl::parseFromString(const QString &strValue)
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

QString FlowControl::toString(FlowControl::Enum flowControl)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(flowControl));
}

const QVector<FlowControl::Enum> &FlowControl::getAllEnums()
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
