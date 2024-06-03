// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "filehelper.hpp"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QStorageInfo>
#include <QUuid>


bool FileHelper::prependData(const QByteArray &data, QFile &file)
{
    QString fileName = file.fileName();
    QString tmpFileName = createUniqueTmpFileName(fileName);

    bool fileExist = file.exists();

    file.close();

    QFile tmpFile(tmpFileName);

    if(!tmpFile.open(QIODevice::Append))
    {
        qWarning() << "Can't open the temporary file: " << tmpFileName << ", which is the copy of: "
                   << fileName;
        return false;
    }

    qint64 byteWritten = tmpFile.write(data);

    if(byteWritten != data.length())
    {
        qWarning() << "A problem occurred when writting to the tmp file: " << tmpFileName << ", "
                   << byteWritten << " bytes written, expected: " << data.length();
        return false;
    }

    QString bkpFileName;

    if(fileExist)
    {
        // If the file doesn't exist this is useless

        bkpFileName = createUniqueBkpFileName(fileName);

        if(!file.rename(bkpFileName))
        {
            qWarning() << "Can't rename the file to backup file: " << bkpFileName;
            return false;
        }

        if(!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Can't open the file: " << fileName << ", in read only mode";
            return false;
        }

        if(!append(tmpFile, file))
        {
            qWarning() << "A problem occurred when tryied to copy data from the file: " << fileName
                       << ", to: " << tmpFileName;
            return false;
        }

        file.close();
    }

    tmpFile.close();

    if(!tmpFile.rename(fileName))
    {
        qWarning() << "Can't rename the temporary file: " << tmpFileName << ", to: " << fileName;
        return false;
    }

    if(fileExist && !QFile::remove(bkpFileName))
    {
        qWarning() << "A problem occurred when trying to remove the backup file: " << bkpFileName;
        return false;
    }

    file.setFileName(fileName);

    return true;
}

QString FileHelper::appendSuffixToBaseName(const QString &toAppendToBaseName, const QFile &file)
{
    QFileInfo fileInfo(file);

    QDir fileDir = fileInfo.absoluteDir();

    // The complete suffix doesn't contain the first "."
    QString newBaseName = fileInfo.baseName() + toAppendToBaseName + "." +
                          fileInfo.completeSuffix();

    return QFileInfo(fileDir, newBaseName).absoluteFilePath();
}

QString FileHelper::createUniqueTmpFileName(const QString &fileName)
{
    return createUniqueFileName(fileName, TmpSuffixFileName);
}

QString FileHelper::createUniqueBkpFileName(const QString &fileName)
{
    return createUniqueFileName(fileName, BckpSuffixFileName);
}

QString FileHelper::createUniqueFileName(const QString &fileName, const QString &suffix)
{
    QUuid uuid = QUuid::createUuid();
    quint32 uuidHash = qHash(uuid);

    QString uniqueFileName(fileName);
    uniqueFileName.append(QString(SuffixFileName).arg(uuidHash).arg(suffix));

    return uniqueFileName;
}

bool FileHelper::append(QFile &targetFile, QIODevice &sourceFile)
{
    if(!sourceFile.isOpen() || !sourceFile.isReadable())
    {
        qWarning() << "A problem occurred, the source file has to be open and readable";
        return false;
    }

    if(sourceFile.isTransactionStarted())
    {
        qWarning() << "Can't read the source file, a transaction is already started";
        return false;
    }

    if(!targetFile.openMode().testFlag(QIODevice::Append))
    {
        qWarning() << "A problem occurred, the target file: " << targetFile.fileName() << ", is not "
                   << "opened in the append open mode";
        return false;
    }

    sourceFile.startTransaction();

    if(!sourceFile.isSequential() && sourceFile.pos() != 0)
    {
        if(!sourceFile.seek(0))
        {
            qWarning() << "A problem occurred when trying to reset the position of source file";
        }
    }

    QStorageInfo storageInfo(targetFile.fileName());

    qint64 blockSize = storageInfo.blockSize();

    if(blockSize == -1)
    {
        qInfo() << "Can't determine the best block size for appending on the storage of file: "
                << targetFile.fileName() << ", use default block size: "
                << DefaultBlockSizeInByte;

        blockSize = DefaultBlockSizeInByte;
    }

    while(!sourceFile.atEnd())
    {
        QByteArray data = sourceFile.read(blockSize * BlockNbToGetAtEachIter);
        if(targetFile.write(data) != data.length())
        {
            qWarning() << "A problem occurred when writting data in the target file: "
                       << targetFile.fileName();

            sourceFile.commitTransaction();
            return false;
        }
    }

    sourceFile.commitTransaction();
    return true;
}

void FileHelper::getUniquePartOfAPath(const QDir &baseDir,
                                      const QDir &dirToCompareWith,
                                      QString &uniquePath)
{
    uniquePath = baseDir.absolutePath();
    QString pathToCompareWith = dirToCompareWith.absolutePath();

    int minSize = qMin(uniquePath.size(), pathToCompareWith.size());
    int lastCommonCharIdx = 0;

    for(int idx = 0; idx < minSize; idx++)
    {
        if(uniquePath.at(idx) == PathSeparator)
        {
            lastCommonCharIdx = idx;
        }

        if(uniquePath.at(idx) != pathToCompareWith.at(idx))
        {
            break;
        }
    }

    uniquePath.remove(0, lastCommonCharIdx);
}

QString FileHelper::getQmlAbsoluteFilePath(const QFileInfo &fileInfo)
{
    QString absoluteFilePath = fileInfo.absoluteFilePath();

    if(!fileInfo.isNativePath() && !absoluteFilePath.startsWith(QrcPrefix))
    {
        absoluteFilePath.prepend(QrcPrefix);
    }
    else if(fileInfo.isNativePath() && !absoluteFilePath.startsWith(FilePrefix))
    {
        absoluteFilePath.prepend(FilePrefix);
    }

    return absoluteFilePath;
}

QString FileHelper::cleanFileName(const QString &fileName,
                                  const QString &blackListCharPattern,
                                  const QString &defaultReplacement,
                                  const QHash<char, QString> &replacementOverride)
{
    QString tmpFileName{fileName};

    for(auto citer = replacementOverride.cbegin(); citer != replacementOverride.cend(); ++citer)
    {
        tmpFileName.replace(citer.key(), citer.value());
    }

    tmpFileName.replace(QRegularExpression(blackListCharPattern), defaultReplacement);

    return tmpFileName;
}
