// SPDX-FileCopyrightText: 2022 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "yamltojsonmanager.hpp"

#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>

#include "definesutility/definesutility.hpp"
#include "yamlutility/yqprocesscaller.hpp"

const QByteArray::Base64Options YamlToJsonManager::Base64Options = QByteArray::Base64UrlEncoding |
                                                             QByteArray::OmitTrailingEquals;


YamlToJsonManager::YamlToJsonManager(QObject *parent)
    : AbstractManager{parent},
      _yqProcess(new YqProcessCaller(this))
{
}

YamlToJsonManager::~YamlToJsonManager()
{
    delete _yamlWorkerDir;
    _yamlWorkerDir = nullptr;
}

bool YamlToJsonManager::initManager(const QString &yqProgramPath,
                              const QString &yamlWorkerDirPath,
                              int tmpJsonFileLifeDurationInS)
{
    if(tmpJsonFileLifeDurationInS < 0)
    {
        qWarning() << "The temporary JSON life duration can't be negative: "
                   << tmpJsonFileLifeDurationInS;
        return false;
    }

    _tmpJsonFileLifeDurationInS = tmpJsonFileLifeDurationInS;

    RETURN_IF_FALSE(_yqProcess->initProcess(yqProgramPath));

    RETURN_IF_FALSE(manageYamlWorkerDir(yamlWorkerDirPath));

    RETURN_IF_FALSE(cleanTmpFolder());

    return true;
}

bool YamlToJsonManager::loadYamlAndConvertToJson(const QString &yamlFilePath, QFile &jsonFile)
{
    QFile yamlFile(yamlFilePath);

    if(!yamlFile.exists())
    {
        qWarning() << "The YAML file: " << yamlFilePath << ", doesn't exist";
        return false;
    }

    return loadYamlAndConvertToJson(yamlFile, jsonFile);
}

bool YamlToJsonManager::loadYamlAndConvertToJson(QFile &yamlFile, QFile &jsonFile)
{
    if(!isValid())
    {
        qWarning() << "Can't load the YAML file, the YAML manager hasn't been initialized";
        return false;
    }

    QCryptographicHash hash(QCryptographicHash::Sha256);

    if(!yamlFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "The yam file can't be open: " << yamlFile.fileName();
        return false;
    }

    if(!hash.addData(&yamlFile))
    {
        qWarning() << "The yaml file given can't be read: " << yamlFile.fileName();
        yamlFile.close();
        return false;
    }

    yamlFile.close();

    const QString hashStr = hash.result().toBase64(YamlToJsonManager::Base64Options);

    const QFileInfo jsonFileInfo(*_yamlWorkerDir, QString(YamlToJsonManager::JsonSuffix).arg(hashStr));

    if(!_yamlSettings->contains(hashStr))
    {
        // The yaml file has never been loaded
        RETURN_IF_FALSE(_yqProcess->convertYamlFileToJsonFile(yamlFile.fileName(),
                                                              jsonFileInfo.absoluteFilePath()));
    }

    jsonFile.setFileName(jsonFileInfo.absoluteFilePath());

    _yamlSettings->setValue(hashStr, QDateTime::currentSecsSinceEpoch());

    return true;
}

bool YamlToJsonManager::isValid() const
{
    return _yamlSettings != nullptr && _yamlWorkerDir != nullptr;
}

bool YamlToJsonManager::manageYamlWorkerDir(const QString &yamlWorkerDirPath)
{
    if(isValid())
    {
        // The YAML worker dir is already managed
        return true;
    }

    _yamlWorkerDir = new QDir(yamlWorkerDirPath);

    if(!_yamlWorkerDir->makeAbsolute())
    {
        qWarning() << "Can't make the path: " << yamlWorkerDirPath << ", absolute";
    }

    if(!_yamlWorkerDir->mkpath("."))
    {
        qWarning() << "Can't create the directory: " << _yamlWorkerDir->absolutePath();
        delete _yamlWorkerDir;
        _yamlWorkerDir = nullptr;
        return false;
    }

    const QFileInfo yamlSettingFile(*_yamlWorkerDir, YamlToJsonManager::YamlConfigFileName);

    _yamlSettings = new QSettings(yamlSettingFile.absoluteFilePath(), QSettings::IniFormat, this);

    return true;
}

bool YamlToJsonManager::cleanTmpFolder()
{
    if(!isValid())
    {
        qWarning() << "Can't clean the tmp folder, the yaml manager isn't initialized";
        return false;
    }

    const QFileInfoList jsonElements = _yamlWorkerDir->entryInfoList(
        { QString(YamlToJsonManager::JsonSuffix).arg("*") },
                                             QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    QStringList settingsKeys = _yamlSettings->allKeys();

    for(auto citer = jsonElements.cbegin(); citer != jsonElements.cend(); ++citer)
    {
        const QString hash = citer->baseName();

        bool toRemove = true;
        bool containedInSettingsFile = settingsKeys.contains(hash);

        if(containedInSettingsFile)
        {
            const QString strTimestampInS = _yamlSettings->value(hash).toString();

            bool ok = false;
            const qint64 tsInS = strTimestampInS.toLongLong(&ok);

            if(ok &&
                (QDateTime::currentSecsSinceEpoch() - tsInS) <= YamlToJsonManager::JsonFileLifeDurationInS)
            {
                toRemove = false;
            }

            // We remove the found keys; in order to only keep the keys which haven't been searched
            settingsKeys.removeAll(hash);
        }

        if(toRemove)
        {
            if(!QFile::remove(citer->absoluteFilePath()))
            {
                qWarning() << "A problem occurred when tried to remove the old JSON file: "
                           << citer->absoluteFilePath();
                continue;
            }

            if(containedInSettingsFile)
            {
                _yamlSettings->remove(hash);
            }
        }
    }

    // We remove all the keys in the settings file which have no file linked to them
    for(auto citer = settingsKeys.cbegin(); citer != settingsKeys.cend(); ++citer)
    {
        _yamlSettings->remove(*citer);
    }

    qInfo() << "The YAML tmp folder has been cleaned with old JSON files";

    return true;
}
