// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QMetaType>
#include <QVector>


/** @brief Contains Json constants */
namespace JsonConstants
{
    /** @brief Contains QMetaTypes which can be considered as "Number" (like uint8, int64,
               float, etc...) */
    const QVector<int> numberMetaUserTypes {
        QMetaType::Char,
        QMetaType::SChar,
        QMetaType::UChar,
        QMetaType::Short,
        QMetaType::UShort,
        QMetaType::Int,
        QMetaType::UInt,
        QMetaType::Long,
        QMetaType::ULong,
        QMetaType::LongLong,
        QMetaType::ULongLong,
        QMetaType::Double,
        QMetaType::Float
    };
}
