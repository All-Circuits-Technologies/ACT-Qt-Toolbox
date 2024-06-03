// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "definesutility/warningsutility.hpp"


/** @brief Contains usefull methods to execute mathematical functions on numbers */
namespace MathHelper
{
    /** @brief Allow to calculate the two's complement of an unsigned integer
        @param value The value to calculate it's two's complement
        @return The two's complement of the value given */
    template<class T>
    T twosComplement(T value);
};

template<class T>
T MathHelper::twosComplement(T value)
{
    static_assert (std::is_same<quint64, T>() ||
                   std::is_same<quint32, T>() ||
                   std::is_same<quint16, T>() ||
                   std::is_same<quint8, T>(), "The value is not an unsigned integer");

    // The cast to quint64 prevents the app to cast the value to an integer before applying the
    // minus operator
    WARNING_INT_MINUS_PUSH
    return static_cast<T>(-static_cast<quint64>(value));
    WARNING_INT_MINUS_POP
}
