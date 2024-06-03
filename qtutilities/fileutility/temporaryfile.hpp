// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QFile>

#include "fileutility/filehelper.hpp"


/** @brief Helpful class to define a temporary file
    @note In some cases, the @ref QTemporaryFile class can't be used because it keeps the file
          opens and so the file can't be modified by other programs.
          It's in those cases that this class is useful.
    @note The file is generated in the @ref QDir::temp folder with an unique name
    @note The file is removed when this class is destroyed (except if we explicitly disable the
          auto remove) */
class TemporaryFile : public QFile
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @note The file is generated in the @ref QDir::temp folder with an unique name
            @note An unique id is added between the baseName and the suffix
            @param baseName The base name of the file to create
            @param suffix The suffix to use for the file to create
            @param parent The parent instance class */
        explicit TemporaryFile(const QString &baseName,
                               const QString &suffix = FileHelper::TmpSuffixFileName,
                               QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~TemporaryFile() override;

    public:
        /** @brief Set the file to enable/disable the auto remove feature
            @note By default the autoremove is enabled
            @param autoRemove True to auto remove the file when the instance is destroyed */
        void setAutoRemoveFile(bool autoRemove) { _autoRemoveFile = autoRemove; }

    private:
        /** @brief Generate an unique file name to be used to create the temporary file
            @note The file name is generated with the @ref QDir::temp as folder path
            @note An unique id is added between the baseName and the suffix
            @param baseName The base name of the file name
            @param suffix The suffix to add at the end of the file name
            @return The unique file name generated */
        static QString generateUniqueFileName(const QString &baseName, const QString &suffix);

    private:
        bool _autoRemoveFile{true};
};
