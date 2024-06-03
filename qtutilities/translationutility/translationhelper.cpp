// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "translationhelper.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QLocale>


QTranslator *TranslationHelper::translate(const QString &filename,
                                          const QString &filepath,
                                          QObject *parent)
{
    QTranslator * translator = new QTranslator(parent);

    if(!translator->load(QLocale(), filename, "-", filepath))
    {
        qWarning() << "Can't find or install the translator file in folder: " << filepath
                   << ", with the name: " << filename << ", and for locale: " << QLocale().name();
        delete translator;
        return nullptr;
    }

    QCoreApplication::installTranslator(translator);
    return translator;
}
