// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "angularmaterialthemingcolors.hpp"

#include <QDebug>
#include <QMetaEnum>

const QColor AngularMaterialThemingColors::Black = QColor(Qt::black);
const QColor AngularMaterialThemingColors::White = QColor(Qt::white);
AngularMaterialThemingColors::ColorInfo
                            AngularMaterialThemingColors::defaultColorInfo = { "#000000", false };

QHash<AngularMaterialThemingColors::ThemeColor,
            QHash<AngularMaterialThemingColors::Tone, AngularMaterialThemingColors::ColorInfo>>
                                                    AngularMaterialThemingColors::_colorThemes = {
    { ThemeColor::Red, {        { Tone50,   { "#FFEBEE", false } },
                                { Tone100,  { "#FFCDD2", false } },
                                { Tone200,  { "#EF9A9A", false } },
                                { Tone300,  { "#E57373", false } },
                                { Tone400,  { "#EF5350", true  } },
                                { Tone500,  { "#F44336", true  } },
                                { Tone600,  { "#E53935", true  } },
                                { Tone700,  { "#D32F2F", true  } },
                                { Tone800,  { "#C62828", true  } },
                                { Tone900,  { "#B71C1C", true  } },
                                { ToneA100, { "#FF8A80", false } },
                                { ToneA200, { "#FF5252", true  } },
                                { ToneA400, { "#FF1744", true  } },
                                { ToneA700, { "#D50000", true  } } } },
    { ThemeColor::Pink, {       { Tone50,   { "#FCE4EC", false } },
                                { Tone100,  { "#F8BBD0", false } },
                                { Tone200,  { "#F48FB1", false } },
                                { Tone300,  { "#F06292", false } },
                                { Tone400,  { "#EC407A", true  } },
                                { Tone500,  { "#E91E63", true  } },
                                { Tone600,  { "#D81B60", true  } },
                                { Tone700,  { "#C2185B", true  } },
                                { Tone800,  { "#AD1457", true  } },
                                { Tone900,  { "#880E4F", true  } },
                                { ToneA100, { "#FF80AB", false } },
                                { ToneA200, { "#FF4081", true  } },
                                { ToneA400, { "#F50057", true  } },
                                { ToneA700, { "#C51162", true  } } } },
    { ThemeColor::Purple, {     { Tone50,   { "#F3E5F5", false } },
                                { Tone100,  { "#E1BEE7", false } },
                                { Tone200,  { "#CE93D8", false } },
                                { Tone300,  { "#BA68C8", true  } },
                                { Tone400,  { "#AB47BC", true  } },
                                { Tone500,  { "#9C27B0", true  } },
                                { Tone600,  { "#8E24AA", true  } },
                                { Tone700,  { "#7B1FA2", true  } },
                                { Tone800,  { "#6A1B9A", true  } },
                                { Tone900,  { "#4A148C", true  } },
                                { ToneA100, { "#EA80FC", false } },
                                { ToneA200, { "#E040FB", true  } },
                                { ToneA400, { "#D500F9", true  } },
                                { ToneA700, { "#AA00FF", true  } } } },
    { ThemeColor::DeepPurple, { { Tone50,   { "#EDE7F6", false } },
                                { Tone100,  { "#D1C4E9", false } },
                                { Tone200,  { "#B39DDB", false } },
                                { Tone300,  { "#9575CD", true  } },
                                { Tone400,  { "#7E57C2", true  } },
                                { Tone500,  { "#673AB7", true  } },
                                { Tone600,  { "#5E35B1", true  } },
                                { Tone700,  { "#512DA8", true  } },
                                { Tone800,  { "#4527A0", true  } },
                                { Tone900,  { "#311B92", true  } },
                                { ToneA100, { "#B388FF", false } },
                                { ToneA200, { "#7C4DFF", true  } },
                                { ToneA400, { "#651FFF", true  } },
                                { ToneA700, { "#6200EA", true  } } } },
    { ThemeColor::Indigo,     { { Tone50,   { "#E8EAF6", false } },
                                { Tone100,  { "#C5CAE9", false } },
                                { Tone200,  { "#9FA8DA", false } },
                                { Tone300,  { "#7986CB", true  } },
                                { Tone400,  { "#5C6BC0", true  } },
                                { Tone500,  { "#3F51B5", true  } },
                                { Tone600,  { "#3949AB", true  } },
                                { Tone700,  { "#303F9F", true  } },
                                { Tone800,  { "#283593", true  } },
                                { Tone900,  { "#1A237E", true  } },
                                { ToneA100, { "#8C9EFF", false } },
                                { ToneA200, { "#536DFE", true  } },
                                { ToneA400, { "#3D5AFE", true  } },
                                { ToneA700, { "#304FFE", true  } } } },
    { ThemeColor::Blue,       { { Tone50,   { "#E3F2FD", false } },
                                { Tone100,  { "#BBDEFB", false } },
                                { Tone200,  { "#90CAF9", false } },
                                { Tone300,  { "#64B5F6", false } },
                                { Tone400,  { "#42A5F5", false } },
                                { Tone500,  { "#2196F3", false } },
                                { Tone600,  { "#1E88E5", true  } },
                                { Tone700,  { "#1976D2", true  } },
                                { Tone800,  { "#1565C0", true  } },
                                { Tone900,  { "#0D47A1", true  } },
                                { ToneA100, { "#82B1FF", false } },
                                { ToneA200, { "#448AFF", true  } },
                                { ToneA400, { "#2979FF", true  } },
                                { ToneA700, { "#2962FF", true  } } } },
    { ThemeColor::LightBlue,  { { Tone50,   { "#E1F5FE", false } },
                                { Tone100,  { "#B3E5FC", false } },
                                { Tone200,  { "#81D4FA", false } },
                                { Tone300,  { "#4FC3F7", false } },
                                { Tone400,  { "#29B6F6", false } },
                                { Tone500,  { "#03A9F4", false } },
                                { Tone600,  { "#039BE5", false } },
                                { Tone700,  { "#0288D1", true  } },
                                { Tone800,  { "#0277BD", true  } },
                                { Tone900,  { "#01579B", true  } },
                                { ToneA100, { "#80D8FF", false } },
                                { ToneA200, { "#40C4FF", false } },
                                { ToneA400, { "#00B0FF", false } },
                                { ToneA700, { "#0091EA", true  } } } },
    { ThemeColor::Cyan,       { { Tone50,   { "#E0F7FA", false } },
                                { Tone100,  { "#B2EBF2", false } },
                                { Tone200,  { "#80DEEA", false } },
                                { Tone300,  { "#4DD0E1", false } },
                                { Tone400,  { "#26C6DA", false } },
                                { Tone500,  { "#00BCD4", false } },
                                { Tone600,  { "#00ACC1", false } },
                                { Tone700,  { "#0097A7", true  } },
                                { Tone800,  { "#00838F", true  } },
                                { Tone900,  { "#006064", true  } },
                                { ToneA100, { "#84FFFF", false } },
                                { ToneA200, { "#18FFFF", false } },
                                { ToneA400, { "#00E5FF", false } },
                                { ToneA700, { "#00B8D4", false } } } },
    { ThemeColor::Teal,       { { Tone50,   { "#E0F2F1", false } },
                                { Tone100,  { "#B2DFDB", false } },
                                { Tone200,  { "#80CBC4", false } },
                                { Tone300,  { "#4DB6AC", false } },
                                { Tone400,  { "#26A69A", false } },
                                { Tone500,  { "#009688", true  } },
                                { Tone600,  { "#00897B", true  } },
                                { Tone700,  { "#00796B", true  } },
                                { Tone800,  { "#00695C", true  } },
                                { Tone900,  { "#004D40", true  } },
                                { ToneA100, { "#A7FFEB", false } },
                                { ToneA200, { "#64FFDA", false } },
                                { ToneA400, { "#1DE9B6", false } },
                                { ToneA700, { "#00BFA5", false } } } },
    { ThemeColor::Green,      { { Tone50,   { "#E8F5E9", false } },
                                { Tone100,  { "#C8E6C9", false } },
                                { Tone200,  { "#A5D6A7", false } },
                                { Tone300,  { "#81C784", false } },
                                { Tone400,  { "#66BB6A", false } },
                                { Tone500,  { "#4CAF50", false } },
                                { Tone600,  { "#43A047", true  } },
                                { Tone700,  { "#388E3C", true  } },
                                { Tone800,  { "#2E7D32", true  } },
                                { Tone900,  { "#1B5E20", true  } },
                                { ToneA100, { "#B9F6CA", false } },
                                { ToneA200, { "#69F0AE", false } },
                                { ToneA400, { "#00E676", false } },
                                { ToneA700, { "#00C853", false } } } },
    { ThemeColor::LightGreen, { { Tone50,   { "#F1F8E9", false } },
                                { Tone100,  { "#DCEDC8", false } },
                                { Tone200,  { "#C5E1A5", false } },
                                { Tone300,  { "#AED581", false } },
                                { Tone400,  { "#9CCC65", false } },
                                { Tone500,  { "#8BC34A", false } },
                                { Tone600,  { "#7CB342", false } },
                                { Tone700,  { "#689F38", false } },
                                { Tone800,  { "#558B2F", true  } },
                                { Tone900,  { "#33691E", true  } },
                                { ToneA100, { "#CCFF90", false } },
                                { ToneA200, { "#B2FF59", false } },
                                { ToneA400, { "#76FF03", false } },
                                { ToneA700, { "#64DD17", false } } } },
    { ThemeColor::Lime,       { { Tone50,   { "#CDDC39", false } },
                                { Tone100,  { "#F9FBE7", false } },
                                { Tone200,  { "#E6EE9C", false } },
                                { Tone300,  { "#DCE775", false } },
                                { Tone400,  { "#D4E157", false } },
                                { Tone500,  { "#CDDC39", false } },
                                { Tone600,  { "#C0CA33", false } },
                                { Tone700,  { "#AFB42B", false } },
                                { Tone800,  { "#9E9D24", false } },
                                { Tone900,  { "#827717", true  } },
                                { ToneA100, { "#F4FF81", false } },
                                { ToneA200, { "#EEFF41", false } },
                                { ToneA400, { "#C6FF00", false } },
                                { ToneA700, { "#AEEA00", false } } } },
    { ThemeColor::Yellow,     { { Tone50,   { "#FFFDE7", false } },
                                { Tone100,  { "#FFF9C4", false } },
                                { Tone200,  { "#FFF59D", false } },
                                { Tone300,  { "#FFF176", false } },
                                { Tone400,  { "#FFEE58", false } },
                                { Tone500,  { "#FFEB3B", false } },
                                { Tone600,  { "#FDD835", false } },
                                { Tone700,  { "#FBC02D", false } },
                                { Tone800,  { "#F9A825", false } },
                                { Tone900,  { "#F57F17", false } },
                                { ToneA100, { "#FFFF8D", false } },
                                { ToneA200, { "#FFFF00", false } },
                                { ToneA400, { "#FFEA00", false } },
                                { ToneA700, { "#FFD600", false } } } },
    { ThemeColor::Amber,      { { Tone50,   { "#FFF8E1", false } },
                                { Tone100,  { "#FFECB3", false } },
                                { Tone200,  { "#FFE082", false } },
                                { Tone300,  { "#FFD54F", false } },
                                { Tone400,  { "#FFCA28", false } },
                                { Tone500,  { "#FFC107", false } },
                                { Tone600,  { "#FFB300", false } },
                                { Tone700,  { "#FFA000", false } },
                                { Tone800,  { "#FF8F00", false } },
                                { Tone900,  { "#FF6F00", false } },
                                { ToneA100, { "#FFE57F", false } },
                                { ToneA200, { "#FFD740", false } },
                                { ToneA400, { "#FFC400", false } },
                                { ToneA700, { "#FFAB00", false } } } },
    { ThemeColor::Orange,     { { Tone50,   { "#FFF3E0", false } },
                                { Tone100,  { "#FFE0B2", false } },
                                { Tone200,  { "#FFCC80", false } },
                                { Tone300,  { "#FFB74D", false } },
                                { Tone400,  { "#FFA726", false } },
                                { Tone500,  { "#FF9800", false } },
                                { Tone600,  { "#FB8C00", false } },
                                { Tone700,  { "#F57C00", false } },
                                { Tone800,  { "#EF6C00", false } },
                                { Tone900,  { "#E65100", true  } },
                                { ToneA100, { "#FFD180", false } },
                                { ToneA200, { "#FFAB40", false } },
                                { ToneA400, { "#FF9100", false } },
                                { ToneA700, { "#FF6D00", false } } } },
    { ThemeColor::DeepOrange, { { Tone50,   { "#FBE9E7", false } },
                                { Tone100,  { "#FFCCBC", false } },
                                { Tone200,  { "#FFAB91", false } },
                                { Tone300,  { "#FF8A65", false } },
                                { Tone400,  { "#FF7043", false } },
                                { Tone500,  { "#FF5722", false } },
                                { Tone600,  { "#F4511E", true  } },
                                { Tone700,  { "#E64A19", true  } },
                                { Tone800,  { "#D84315", true  } },
                                { Tone900,  { "#BF360C", true  } },
                                { ToneA100, { "#FF9E80", false } },
                                { ToneA200, { "#FF6E40", false } },
                                { ToneA400, { "#FF3D00", true  } },
                                { ToneA700, { "#DD2C00", true  } } } },
    { ThemeColor::Brown     , { { Tone50,   { "#EFEBE9", false } },
                                { Tone100,  { "#D7CCC8", false } },
                                { Tone200,  { "#BCAAA4", false } },
                                { Tone300,  { "#A1887F", true  } },
                                { Tone400,  { "#8D6E63", true  } },
                                { Tone500,  { "#795548", true  } },
                                { Tone600,  { "#6D4C41", true  } },
                                { Tone700,  { "#5D4037", true  } },
                                { Tone800,  { "#4E342E", true  } },
                                { Tone900,  { "#3E2723", true  } } } },
    { ThemeColor::Grey      , { { Tone50,   { "#FAFAFA", false } },
                                { Tone100,  { "#F5F5F5", false } },
                                { Tone200,  { "#EEEEEE", false } },
                                { Tone300,  { "#E0E0E0", false } },
                                { Tone400,  { "#BDBDBD", false } },
                                { Tone500,  { "#9E9E9E", false } },
                                { Tone600,  { "#757575", true  } },
                                { Tone700,  { "#616161", true  } },
                                { Tone800,  { "#424242", true  } },
                                { Tone900,  { "#212121", true  } } } },
    { ThemeColor::BlueGrey  , { { Tone50,   { "#ECEFF1", false } },
                                { Tone100,  { "#CFD8DC", false } },
                                { Tone200,  { "#B0BEC5", false } },
                                { Tone300,  { "#90A4AE", false } },
                                { Tone400,  { "#78909C", true  } },
                                { Tone500,  { "#607D8B", true  } },
                                { Tone600,  { "#546E7A", true  } },
                                { Tone700,  { "#455A64", true  } },
                                { Tone800,  { "#37474F", true  } },
                                { Tone900,  { "#263238", true  } } } }
};


QColor AngularMaterialThemingColors::getThemeColor(
                                                AngularMaterialThemingColors::ThemeColor themeColor,
                                                AngularMaterialThemingColors::Tone tone)
{
    bool ok = false;
    ColorInfo &colorInfo = getColorInfo(themeColor, tone, ok);

    if(!ok)
    {
        return {};
    }

    return {colorInfo.color};
}

QColor AngularMaterialThemingColors::getTextColorDependingOfBackgroundColor(
                                            AngularMaterialThemingColors::ThemeColor bgThemeColor,
                                            AngularMaterialThemingColors::Tone bgTone)
{
    bool ok = false;
    ColorInfo &colorInfo = getColorInfo(bgThemeColor, bgTone, ok);

    if(!ok)
    {
        return {};
    }

    return colorInfo.useWhiteTextWithIt;
}

QString AngularMaterialThemingColors::getThemeColorName(
                                                AngularMaterialThemingColors::ThemeColor themeColor)
{
    return QString::fromLatin1(QMetaEnum::fromType<ThemeColor>().valueToKey(themeColor));
}

QString AngularMaterialThemingColors::getToneName(AngularMaterialThemingColors::Tone tone)
{
    return QString::fromLatin1(QMetaEnum::fromType<Tone>().valueToKey(tone));
}

AngularMaterialThemingColors::ColorInfo &AngularMaterialThemingColors::getColorInfo(
                                                AngularMaterialThemingColors::ThemeColor themeColor,
                                                AngularMaterialThemingColors::Tone tone,
                                                bool &ok)
{
    if(!_colorThemes.contains(themeColor))
    {
        qWarning() << "The theme is not known: " << getThemeColorName(themeColor);
        ok = false;
        return defaultColorInfo;
    }

    QHash<Tone, ColorInfo> &theme = _colorThemes[themeColor];

    if(!theme.contains(tone))
    {
        qWarning() << "The tone is not known: " << getToneName(tone)
                   << " of the theme: " << getThemeColorName(themeColor);
        ok = false;
        return defaultColorInfo;
    }

    ok = true;
    return theme[tone];
}
