// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "temporaryfile.hpp"

#include <QDebug>
#include <QDir>


TemporaryFile::TemporaryFile(const QString &baseName,
                             const QString &suffix,
                             QObject *parent) :
    QFile(generateUniqueFileName(baseName, suffix), parent)
{
}

TemporaryFile::~TemporaryFile()
{
    if(!_autoRemoveFile || !exists())
    {
        return;
    }

    if(!remove())
    {
        qWarning() << "The temporary file: " << fileName() << ", can't be removed with the "
                   << "instance of the TemporaryFile";
    }
}

QString TemporaryFile::generateUniqueFileName(const QString &baseName, const QString &suffix)
{
    QFileInfo fileName(QDir::temp(), FileHelper::createUniqueFileName(baseName, suffix));
    return fileName.absoluteFilePath();
}
