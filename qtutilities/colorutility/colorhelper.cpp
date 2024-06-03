// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "colorhelper.hpp"

#include <cmath>
#include <QDebug>


QColor ColorHelper::getTextColorDependingOfBackgroundLuminance(const QColor &backgroundColor)
{
    if(getRelativeLuminance(backgroundColor) > LimitLuminanceForUsingBlackOrWhiteText)
    {
        return {Qt::black};
    }

    return {Qt::white};
}

qreal ColorHelper::getRelativeLuminance(const QColor &color)
{
    auto calculateAverageColor = [](qreal color)
    {
        qreal avColor = 0;
        if(color <= ColorLimitForTest)
        {
            avColor = color / ColorDividorWhenUnderLimit;
        }
        else
        {
            avColor = pow(((color + AddConstWhenAboveLimit) / DivConsWhenAboveLimit),
                          PowConstWhenAboveLimit);
        }

        return avColor;
    };

    return ((calculateAverageColor(color.redF()) * RedConstMultiple) +
            (calculateAverageColor(color.greenF()) * GreenConstMultiple) +
            (calculateAverageColor(color.blueF()) * BlueConstMultiple));
}
