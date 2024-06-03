// SPDX-FileCopyrightText: 2022 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "managersutility/abstractmanager.hpp"

#include <QByteArray>

class QDir;
class QFile;
class QIODevice;
class QSettings;
class YqProcessCaller;


/** @brief Manages YAML files and convert them to JSON files which can be used by Qt

           The manager uses the yq program to convert YAML to JSON files.

           The JSON files converted are stored in a working directory. The manager looks if the
           YAML already exists in its working directory before trying to generate a new JSON file.

           At each start, if a generated JSON file hasn't been used after some times, it will be
           removed */
class YamlToJsonManager : public AbstractManager
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance */
        explicit YamlToJsonManager(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~YamlToJsonManager() override;

    public:
        /** @brief Init the manager
            @param yqProgramPath The path of the yq program
            @param yamlWorkerDirPath The path of the directory where the generated JSON files are
                                     stored
            @param tmpJsonFileLifeDurationInS The life duration of the generated JSON files which
                                              haven't been used
            @return True if no problem occurred */
        bool initManager(const QString &yqProgramPath,
                         const QString &yamlWorkerDirPath,
                         int tmpJsonFileLifeDurationInS = YamlToJsonManager::JsonFileLifeDurationInS);

        /** @brief Load a given YAML file and fills the given JSON file with it
            @param yamlFilePath The path of the YAML file to load
            @param jsonFile The JSON file to fill with the content loaded
                            The fileName of the given file is overwritten
            @return True if no problem occurred */
        bool loadYamlAndConvertToJson(const QString &yamlFilePath, QFile &jsonFile);

        /** @brief Load a given YAML file and fills the given JSON file with it
            @param yamlFile The YAML file to load
            @param jsonFile The JSON file to fill with the content loaded
                            The fileName of the given file is overwritten
            @return True if no problem occurred */
        bool loadYamlAndConvertToJson(QFile &yamlFile, QFile &jsonFile);

        /** @brief Say if the YAML managed is valid (if it has been correctly initialized)
            @return True if the manager can be used */
        bool isValid() const;

    private:
        /** @brief This method creates the directory which stores the generated JSON file, but also
                   the setting file
            @param yamlWorkerDirPath The storage directory path
            @return True if no problem occurred */
        bool manageYamlWorkerDir(const QString &yamlWorkerDirPath);

        /** @brief Clean the temporary folder and removes the useless JSON files
            @return True if no problem occurred */
        bool cleanTmpFolder();

    private:
        /** @brief Name of the YAML setting */
        static const constexpr char* YamlConfigFileName = "yaml_config.ini";

        /** @brief Name of the JSON suffix to use when generating JSON file */
        static const constexpr char* JsonSuffix = "%1.json";

        /** @brief Represents one month in second : 60 * 60 * 24 * 30 */
        static const constexpr qint64 JsonFileLifeDurationInS = 31557600;

        /** @brief Base64 options for generating hash and name of temporary files */
        static const QByteArray::Base64Options Base64Options;

    private:
        YqProcessCaller *_yqProcess{nullptr};
        QDir *_yamlWorkerDir{nullptr};
        QSettings *_yamlSettings{nullptr};
        int _tmpJsonFileLifeDurationInS{YamlToJsonManager::JsonFileLifeDurationInS};
};
