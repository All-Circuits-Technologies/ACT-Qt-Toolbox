// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QColor>


/** @brief The class contains useful methods in order to manage colors */
class ColorHelper
{
    private:
        /** @brief Class constructor */
        ColorHelper() {}

    public:
        /** @brief Get the text color depending of the background color
            @note The text can be black or white
            @note Returns black if the background color luminance is higher than
                  @ref LimitLuminanceForUsingBlackOrWhiteText, or white if lesser.
            @param backgroundColor The background color
            @return The color of text to get, or invalid color if the background color is not
                    known */
        static QColor getTextColorDependingOfBackgroundLuminance(const QColor &backgroundColor);

        /** @brief Get the luminance of a color (value between 0 and 1)
            @link https://www.w3.org/TR/WCAG20/#relativeluminancedef
            @note the relative brightness of any point in a colorspace, normalized to 0 for darkest
                  black and 1 for lightest white

                  Note 1: For the sRGB colorspace, the relative luminance of a color is defined as
                        L = 0.2126 * R + 0.7152 * G + 0.0722 * B where R, G and B are defined as:

                    - if RsRGB <= 0.03928 then R = RsRGB/12.92 else R = ((RsRGB+0.055)/1.055) ^ 2.4
                    - if GsRGB <= 0.03928 then G = GsRGB/12.92 else G = ((GsRGB+0.055)/1.055) ^ 2.4
                    - if BsRGB <= 0.03928 then B = BsRGB/12.92 else B = ((BsRGB+0.055)/1.055) ^ 2.4

                  and RsRGB, GsRGB, and BsRGB are defined as:

                    - RsRGB = R8bit/255
                    - GsRGB = G8bit/255
                    - BsRGB = B8bit/255

                  The "^" character is the exponentiation operator. (Formula taken from [sRGB] and
                  [IEC-4WD]).
            @param color The color to get the luminance from
            @return The luminance of color, between 0 and 1 */
        static qreal getRelativeLuminance(const QColor &color);

    private:
        /** @brief The luminance threshold of the background color to choose between white and black
            @note From StackOverflow:
                  The formula given for contrast in the W3C Recommendations is
                  (L1 + 0.05) / (L2 + 0.05), where L1 is the luminance of the lightest color and
                  L2 is the luminance of the darkest on a scale of 0.0-1.0. The luminance of black
                  is 0.0 and white is 1.0, so substituting those values lets you determine the one
                  with the highest contrast. If the contrast for black is greater than the contrast
                  for white, use black, otherwise use white. Given the luminance of the color
                  you're testing as L the test becomes:

                  if (L + 0.05) / (0.0 + 0.05) > (1.0 + 0.05) / (L + 0.05)
                                                                      use #000000 else use #ffffff

                  This simplifies down algebraically to:

                  if L > sqrt(1.05 * 0.05) - 0.05

                  Or approximately:

                  if L > 0.179 use #000000 else use #ffffff
            @link https://stackoverflow.com/questions/3942878/how-to-decide-font-color-in-white-or\
                  -black-depending-on-background-color */
        static const constexpr qreal LimitLuminanceForUsingBlackOrWhiteText = 0.179 ;

        /** @see ColorHelper::getRelativeLuminance algorithm */
        static const constexpr qreal ColorLimitForTest = 0.03928;

        /** @see ColorHelper::getRelativeLuminance algorithm */
        static const constexpr qreal ColorDividorWhenUnderLimit = 12.92;

        /** @see ColorHelper::getRelativeLuminance algorithm */
        static const constexpr qreal AddConstWhenAboveLimit = 0.055;

        /** @see ColorHelper::getRelativeLuminance algorithm */
        static const constexpr qreal DivConsWhenAboveLimit = 1.055;

        /** @see ColorHelper::getRelativeLuminance algorithm */
        static const constexpr qreal PowConstWhenAboveLimit = 2.4;

        /** @see ColorHelper::getRelativeLuminance algorithm */
        static const constexpr qreal RedConstMultiple = 0.2126;

        /** @see ColorHelper::getRelativeLuminance algorithm */
        static const constexpr qreal GreenConstMultiple = 0.7152;

        /** @see ColorHelper::getRelativeLuminance algorithm */
        static const constexpr qreal BlueConstMultiple = 0.0722;
};
