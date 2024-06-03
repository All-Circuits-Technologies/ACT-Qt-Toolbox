// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "aonefilelogsstrategy.hpp"

#include <QDate>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "fileutility/filehelper.hpp"


AOneFileLogsStrategy::AOneFileLogsStrategy(const QString &folderPath,
                                   LoggingStrategyOption::Enums options,
                                   qint64 maxFolderLimitInMo,
                                   QObject *parent) :
    AFileLogsStrategy(folderPath, parent)
{
    setStrategyOptions(options);

    if(maxFolderLimitInMo > 0)
    {
        _maxFolderLimitInBytes = maxFolderLimitInMo * MegaBytesToBytesFactor;
    }
    else
    {
        _maxFolderLimitInBytes = -1;
    }

    connect(this, &AOneFileLogsStrategy::_askForLogFolderCleaning,
            this, &AOneFileLogsStrategy::removeOldFilesIfNeeded, Qt::QueuedConnection);
}

bool AOneFileLogsStrategy::stop()
{
    if(_logsFile != nullptr)
    {
        _logsFile->close();
        delete _logsFile;
        _logsFile = nullptr;
    }

    return AFileLogsStrategy::stop();
}

bool AOneFileLogsStrategy::writeLog(const QString &msg, const LoggingOptions &options)
{
    if(!isStarted())
    {
        // The strategy log is not started, so we don't process the writing
        return false;
    }

    if(!manageFileCreationIfNeeded(options) || (_logsFile == nullptr))
    {
        // The new file creation has failed
        return false;
    }

    QString toWrite = QString("%1\r\n").arg(msg);

    bool addAtStart = false;
    if(!LoggingOption::isAtStart(options, addAtStart, true))
    {
        return false;
    }

    if(!addAtStart)
    {
        if(!_logsFile->isOpen() && !_logsFile->open(QIODevice::Append))
        {
            // The file was not open, we tried to open it, but that failed
            // I don't add a log here because if each console log it also added to files, this can
            // quickly become exponential
            return false;
        }

        qint64 dataWritten = _logsFile->write(toWrite.toLatin1());

        if(toWrite.length() != dataWritten)
        {
            // A problem occurred when writting logs
            return false;
        }
    }
    else
    {
        // This method (as explains in its doc) closes the file given at the start of the process
        // That's why we reopen it afterwards
        if(!FileHelper::prependData(toWrite.toLatin1(), *_logsFile))
        {
            qWarning() << "A problem occurred when trying to prepend data to log file: "
                       << _logsFile->fileName() << ", we will try to reopen the file";
            return false;
        }

        if(!_logsFile->open(QIODevice::Append))
        {
            qWarning() << "Can't reopen the log file: " << _logsFile->fileName() << ", after "
                       << "prepend data";
            return false;
        }
    }

    return _logsFile->flush();
}

bool AOneFileLogsStrategy::appendToBaseName(const QString &toAppend, const LoggingOptions &options)
{
    // For now we don't hav to use options
    Q_UNUSED(options)

    if(_logsFile == nullptr)
    {
        qWarning() << "Can't rename an unexistant logs file";
        return false;
    }

    QString newFileName = FileHelper::appendSuffixToBaseName(toAppend, *_logsFile);

    _logsFile->close();

    QString oldName = _logsFile->fileName();

    if(!_logsFile->rename(newFileName))
    {
        qWarning() << "The logs file: " << oldName << ", can't be renamed to: " << newFileName
                   << ", stop logging";
        stop();
        return false;
    }

    // Try to reopen the log file
    if(!_logsFile->open(QIODevice::Append))
    {
        qWarning() << "A problem occurred after to have renamed the file: " << oldName << ", to: "
                   << newFileName << ", impossible to open the file. Stop logging.";
        stop();
        return false;
    }

    return true;
}

void AOneFileLogsStrategy::removeOldFilesIfNeeded()
{
    if(_maxFolderLimitInBytes == -1)
    {
        return;
    }

    QDir logFolder(getFolderPath());
    qint64 currentSizeInBytes = 0;
    bool currentFound = false;
    QStringList fileWhichCantBeRemoved;

    removeFilesIfNeededImpl(logFolder,
                            getLogFilenameFilters(),
                            currentFound,
                            currentSizeInBytes,
                            fileWhichCantBeRemoved);

    if(!fileWhichCantBeRemoved.isEmpty())
    {
        qWarning() << "At least those files or folders can't be removed: "
                   << fileWhichCantBeRemoved.join(", ");
    }
}

void AOneFileLogsStrategy::removeFilesIfNeededImpl(const QDir &directory,
                                                   const QStringList &namesFilter,
                                                   bool &currentFileFound,
                                                   qint64 &currentSizeInBytes,
                                                   QStringList &cantBeRemovedFiles)
{
    QFileInfoList logFiles = directory.entryInfoList( namesFilter,
                                                      QDir::Files | QDir::AllDirs |
                                                      QDir::NoSymLinks | QDir::NoDotAndDotDot,
                                                      QDir::Name | QDir::Reversed );

    QFileInfoList::const_iterator removeAllFromHere;
    QFileInfoList::const_iterator iter;

    if(currentSizeInBytes <= _maxFolderLimitInBytes)
    {
        // Here, we are still under the limit
        removeAllFromHere = logFiles.cend();
        iter = logFiles.cbegin();
    }
    else
    {
        // Here all folders and files have to be removed
        removeAllFromHere = logFiles.cbegin();
        iter = logFiles.cend();
    }

    for(; iter != logFiles.cend(); ++iter)
    {
        if(!iter->isDir())
        {
            currentSizeInBytes += iter->size();
        }
        else
        {
            // The element is a folder
            QDir tmpDir(iter->absoluteFilePath());
            removeFilesIfNeededImpl(tmpDir,
                                    namesFilter,
                                    currentFileFound,
                                    currentSizeInBytes,
                                    cantBeRemovedFiles);
        }

        if(_logsFile != nullptr &&
           !currentFileFound &&
           QFileInfo(iter->filePath()).canonicalFilePath() == QFileInfo(
                                                        _logsFile->fileName()).canonicalFilePath())
        {
            currentFileFound = true;
        }
        else if(currentSizeInBytes > _maxFolderLimitInBytes)
        {
            removeAllFromHere = iter;
            break;
        }
    }

    // Remove extra files
    for(iter = removeAllFromHere; iter != logFiles.cend(); ++iter)
    {
        if(iter->isDir())
        {
            // Here manage folders
            QDir folder = iter->dir();
            removeFilesIfNeededImpl(folder,
                                    namesFilter,
                                    currentFileFound,
                                    currentSizeInBytes,
                                    cantBeRemovedFiles);

            if(folder.isEmpty() && !folder.removeRecursively())
            {
                if(cantBeRemovedFiles.length() <= maxCantBeRemovedFilesToDisplay)
                {
                    cantBeRemovedFiles.append(folder.absolutePath());
                }

                qDebug() << "The directory can't be removed: " << folder.absolutePath();
             }
        }
        else if(_logsFile != nullptr &&
                !currentFileFound &&
                QFileInfo(iter->filePath()).canonicalFilePath() == QFileInfo(
                                                        _logsFile->fileName()).canonicalFilePath())
        {
            // We don't try to remove the current file
            currentFileFound = true;
        }
        // From here, we are trying to remove a file
        else if(!QFile::remove(iter->absoluteFilePath()))
        {
            if(cantBeRemovedFiles.length() <= maxCantBeRemovedFilesToDisplay)
            {
                cantBeRemovedFiles.append(iter->absoluteFilePath());
            }

            qDebug() << "The file can't be removed: " << iter->absoluteFilePath();
        }
    }
}

void AOneFileLogsStrategy::setStrategyOptions(LoggingStrategyOption::Enums strategyOptions)
{
    _strategyOptions.setFlag(
                        LoggingStrategyOption::File_StoreInDayFolder,
                        strategyOptions.testFlag(LoggingStrategyOption::File_StoreInDayFolder));

    _strategyOptions.setFlag(
                        LoggingStrategyOption::File_StoreInMonthFolder,
                        strategyOptions.testFlag(LoggingStrategyOption::File_StoreInMonthFolder));

    _strategyOptions.setFlag(
                        LoggingStrategyOption::File_StoreInYearFolder,
                        strategyOptions.testFlag(LoggingStrategyOption::File_StoreInYearFolder));
}

bool AOneFileLogsStrategy::createFile(const QString &filename)
{
    // No matter if a problem occurred, we test if it's necessary to do some cleaning in the folder
    emit _askForLogFolderCleaning();

    QDir folder;
    if(!createAndGetCurrentRightFolder(folder))
    {
        return false;
    }

    QFileInfo filePath(folder, filename);

    QFile *logsFile = new QFile(filePath.filePath(), this);

    if(!logsFile->open(QIODevice::Append))
    {
        delete logsFile;

        return false;
    }

    QFile *oldFile = _logsFile;

    _logsFile = logsFile;

    if(oldFile != nullptr)
    {
        oldFile->close();
        delete oldFile;
    }

    return true;
}

bool AOneFileLogsStrategy::createAndGetCurrentRightFolder(QDir &dir)
{
    dir = QDir(getFolderPath());

    if(!dir.makeAbsolute())
    {
        qWarning() << "Can't make the path: " << getFolderPath() << ", absolute";
    }

    if(!dir.mkpath("."))
    {
        qWarning() << "Can't create the directory: " << dir.absolutePath();
        return false;
    }

    QDate currentDate = QDate::currentDate();

    auto mkdirAndCd = [&dir](const QString &folderName)
    {
        if(!dir.exists(folderName) && !dir.mkdir(folderName))
        {
            qWarning() << "Can't create the folder: " << folderName << ", in directory: "
                       << dir.absolutePath();
            return false;
        }

        if(!dir.cd(folderName))
        {
            qWarning() << "Can't cd to folder: " << folderName << ", in directory: "
                       << dir.absolutePath();
            return false;
        }

        return true;
    };

    if(_strategyOptions.testFlag(LoggingStrategyOption::File_StoreInYearFolder) &&
       !mkdirAndCd(currentDate.toString("yyyy")))
    {
        return false;
    }

    if(_strategyOptions.testFlag(LoggingStrategyOption::File_StoreInMonthFolder) &&
       !mkdirAndCd(currentDate.toString("yyyyMM")))
    {
        return false;
    }

    if(_strategyOptions.testFlag(LoggingStrategyOption::File_StoreInDayFolder) &&
       !mkdirAndCd(currentDate.toString("yyyyMMdd")))
    {
        return false;
    }

    return true;
}
