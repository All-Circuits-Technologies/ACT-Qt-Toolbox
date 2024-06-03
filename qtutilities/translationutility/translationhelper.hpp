// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QTranslator>

class QTranslator;


/** @brief Helping methods to load and install a QTranslator into the QCoreApplication */
namespace TranslationHelper
{
    /** @brief Load the translate file given and install the translator linked to the current
               language
        @param filename The basic filename which will be completed with a suffix dependant of
                        the language
        @param filepath The path of the translation folder
        @param parent The parent of the translator
        @return true if translation went well, false if not */
    QTranslator *translate(const QString &filename,
                           const QString &filepath,
                           QObject *parent = nullptr);
};
