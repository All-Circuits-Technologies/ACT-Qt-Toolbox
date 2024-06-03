// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QDir>
#include <QString>

class QFile;
class QIODevice;


/** @brief Contains helper methods for dealing with files */
class FileHelper
{
    public:
        /** @brief Prepend the data given to the start of file
            @note The method closes the file given
            @note The method will create temporary files to deal with this functionality
            @param data The data to add at file start
            @param file The file to add data at its start
            @return True if no problem occurs */
        static bool prependData(const QByteArray &data, QFile &file);

        /** @brief Append a suffix to the file base name
            @param toAppendToBaseName The suffix to append to the file base name
            @param file The file to append a suffix to its base name
            @return The new file path renamed (an absolute path) */
        static QString appendSuffixToBaseName(const QString &toAppendToBaseName, const QFile &file);

        /** @brief Create an unique temporary file name from the filename given
            @note If the filename contains a path, the method doesn't change it
            @param filename The filename to extend in order to make it unique
            @return An unique tmp file name */
        static QString createUniqueTmpFileName(const QString &fileName);

        /** @brief Create an unique backup file name from the filename given
            @note If the filename contains a path, the method doesn't change it
            @param filename The filename to extend in order to make it unique
            @return An unique backup file name */
        static QString createUniqueBkpFileName(const QString &fileName);

        /** @brief Create an unique file name from the fileName given
            @note If the filename contains a path, the method doesn't modify it
            @param fileName The file name to extend in order to make it unique
            @param suffix The suffix to append to the file name
            @return An unique file name */
        static QString createUniqueFileName(const QString &fileName, const QString &suffix);

        /** @brief Append the source device data to the target file
            @note The target file has to be already opened in QIODevice::Append mode
            @note The source device has to be readable
            @note The method uses transaction for reading from the device source; therefore it can't
                  be currently in transaction
            @param targetFile The file to write in it
            @param deviceSource The device to get data from
            @return True if no problem occurs */
        static bool append(QFile &targetFile, QIODevice &deviceSource);

        /** @brief Compare two path and determine the unique part of the first one
            @param baseDir The QDir to return the unique part from
            @param dirToCompareWith The QDir to compare with
            @param uniquePath The unique part of the first path */
        static void getUniquePartOfAPath(const QDir &baseDir,
                                         const QDir &dirToCompareWith,
                                         QString &uniquePath);

        /** @brief Get an absolute file path which can be used in QML
            @note When using QML, all the paths coming from QRC has to be prepended with the
                  @ref QrcPrefix
            @param fileInfo The file info to get its path from
            @return The absolute path which can be used in QML file */
        static QString getQmlAbsoluteFilePath(const QFileInfo &fileInfo);

        /** @brief Get a clean string which can be used as file name. The name is based on the given
                   @ref fileName
            @note The qhash parameter: @ref replacementOverride, is first called before using the
                  parameter @ref blackListCharPattern
            @param fileName The string to clean in order to use it as file name
            @param blackListCharPattern This is a regexp which lists all the elements we don't want
                                        in the file name
            @param defaultReplacement This contains the string to use in order to replace the
                                      blacklisted characters.
                                      If you want to override the default replacement character for
                                      specifics chars, you can set it in the
                                      @ref replacementOverride parameter
            @param replacementOverride Contains characters to replace with given replacement string
            @return The file name cleaned */
        static QString cleanFileName(
            const QString &fileName,
            const QString &blackListCharPattern = CleanFileNameDefaultPattern,
            const QString &defaultReplacement = CleanFileNameDefaultReplacement,
            const QHash<char, QString> &replacementOverride = {});

    public:
        static const constexpr char *TmpSuffixFileName = ".tmp";
        static const constexpr char *BckpSuffixFileName = ".bkp";
        static const constexpr char *IntelHexSuffixFileName = ".hex";

        static const constexpr char *QrcPrefix = "qrc";

        static const constexpr char *FilePrefix = "file:///";

        static const constexpr char PathSeparator = '/';

        static const constexpr char *CleanFileNameDefaultReplacement = "";

        static const constexpr char *CleanFileNameDefaultPattern = R"([^\w\-. ])";

    private:
        static const constexpr char *SuffixFileName = "-%1%2";

        //! @brief Use 4096 because it's a very common IO block under Linux file systems
        static const constexpr qint32 DefaultBlockSizeInByte = 4096;

        /** @brief Instead of getting block by block (which can be slow) we take several blocks by
                   several blocks in order to optimize.
            @note The following value is arbitrariness, but seems to be the good compromise */
        static const constexpr qint32 BlockNbToGetAtEachIter = 8;
};
