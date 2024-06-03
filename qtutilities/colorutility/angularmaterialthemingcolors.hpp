// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QColor>
#include <QHash>
#include <QVector>


/** @brief This utility class provides Color themes from Angular Material themes
    @link https://material.io/archive/guidelines/style/color.html#color-color-palette */
class AngularMaterialThemingColors : public QObject
{
    Q_OBJECT

    public:
        /** @brief The main Angular Material themes color */
        enum ThemeColor
        {
            Red,
            Pink,
            Purple,
            DeepPurple,
            Indigo,
            Blue,
            LightBlue,
            Cyan,
            Teal,
            Green,
            LightGreen,
            Lime,
            Yellow,
            Amber,
            Orange,
            DeepOrange,
            Brown,      //!< @brief Do not have A tone colors (A100, A200, A400 and A700)
            Grey,       //!< @brief Do not have A tone colors (A100, A200, A400 and A700)
            BlueGrey    //!< @brief Do not have A tone colors (A100, A200, A400 and A700)
        };
        Q_ENUM(ThemeColor)

        /** @brief The available tones of color theme
            @note The Brown, Grey and Blue Grey don't have the A tone colors (A100, A200, A400,
                  A700) */
        enum Tone
        {
            Tone50,
            Tone100,
            Tone200,
            Tone300,
            Tone400,
            Tone500,
            Tone600,
            Tone700,
            Tone800,
            Tone900,
            ToneA100,
            ToneA200,
            ToneA400,
            ToneA700
        };
        Q_ENUM(Tone)

    private:
        /** @brief Contains the details of a theme tone color */
        struct ColorInfo
        {
            QString color;              //!< @brief The theme tone color
            bool useWhiteTextWithIt;    /*!< @brief True if it's better to use a text white with
                                                    this background color */
        };

    private:
        /** @brief Class constructor */
        AngularMaterialThemingColors() {}

    public:
        /** @brief Get the theme color with the right tone
            @note If the color wanted is not known, return an invalid color
            @param themeColor The theme color to get
            @param tone The tone of the color to get
            @return The theme color to get, or invalid color if the colors is not known */
        static QColor getThemeColor(ThemeColor themeColor, Tone tone);

        /** @brief Get the text color depending of the background color
            @note The text can be black or white
            @note If the background color targeted is not known, return an invalid color
            @param bgThemeColor The theme of the background color
            @param bgTone The tone of the background color
            @return The text color to get, or invalid color if the background color is not known */
        static QColor getTextColorDependingOfBackgroundColor(ThemeColor bgThemeColor, Tone bgTone);

        /** @brief Get the theme color name
            @param themeColor The theme color to get name from
            @return The theme color name */
        static QString getThemeColorName(ThemeColor themeColor);

        /** @brief Get the tone color name
            @param tone The tone color to get name from
            @return The tone color name */
        static QString getToneName(Tone tone);

    private:
        /** @brief Get color info
            @note If the color hasn't been found, ok is equal to false and the color info returns is
                  @ref defaultColorInfo
            @param themeColor The theme color to get info from
            @param tone The tone color to get info from
            @param ok True if the color info has been found, False if not
            @return The color info found, or the @ref defaultColorInfo if nothing has been found */
        static ColorInfo &getColorInfo(ThemeColor themeColor, Tone tone, bool &ok);

    public:
        /** @brief Default black color of the theme*/
        static const QColor Black;

        /** @brief Default white color of the theme */
        static const QColor White;

    private:
        /** @brief The default color information
            @note Not constant, in order to be used by reference */
        static ColorInfo defaultColorInfo;

    private:
        /** @brief The Angular material themes and tone
            @note Not constant, in order to use the value by reference */
        static QHash<ThemeColor, QHash<Tone, ColorInfo>> _colorThemes;
};
