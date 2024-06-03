// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tictype.hpp"

#include <QMetaEnum>

#include "definesutility/definesutility.hpp"


void TicType::registerMetaTypes()
{
    qRegisterMetaType<TicType::Enum>("TicType::Enum");
}

int TicType::getPeriodInMs(TicType::Enum ticType)
{
    switch(ticType)
    {
        case TicType::TwoHz:
            return 1000 / 2;
        case TicType::ThreeHz:
            return 1000 / 3;
        case TicType::Unknown:
            return 0;
    }

    return 0;
}

QString TicType::toString(TicType::Enum type)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(type));
}
