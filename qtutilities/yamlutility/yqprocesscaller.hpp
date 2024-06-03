// SPDX-FileCopyrightText: 2022 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

class ProcessCaller;


/** @brief Manage the call of th yq program */
class YqProcessCaller : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance */
        explicit YqProcessCaller(QObject *parent = nullptr);

        /** @brief Class destuctor */
        virtual ~YqProcessCaller() override;

    public:
        /** @brief Init the yq process
            @param yqPrgmPath The yq program path
            @return True if no problem occurred */
        bool initProcess(const QString &yqPrgmPath);

        /** @brief Call the yq process
            @param arguments The arguments to use when calling the yq process
            @param stdOutput If not null, the standard output is written in this string
            @return True if no problem occurred */
        bool call(const QStringList &arguments, QString* stdOutput = nullptr);

        /** @brief Convert a YAML to JSON file
            @param yamlFilePath The path of the YAML file to convert
            @param jsonFilePath The path of the JSON file to create and fill with the converted YAML
            @param exportIndent The indent number to use in the file to export
            @return True if no problem occurred */
        bool convertYamlFileToJsonFile(const QString &yamlFilePath,
                                       const QString &jsonFilePath,
                                       int exportIndent = 2);

    private:
        /** @brief Print help for yq, --help */
        static const constexpr char* HelpCommand = "-h";

        /** @brief Print version information and quit, --version */
        static const constexpr char* VersionCommand = "-V";

        /** @brief Sets indent level for output (default 2), --indent int */
        static const constexpr char* IndentOutputCmd = "-I=%1";

        /** @brief [yaml|y|json|j|props|p|xml|x] output format type. (default "yaml"),
                   --output-format string */
        static const constexpr char* OutputCmd = "-o=%1";

        /** @brief The output format type to use with the yq program in order to export json file */
        static const constexpr char* OutputJsonValue = "json";

        /** @brief The default expression used to evaluate the given yaml file
                   When no command is given to the yq tool, the "eval" command is used */
        static const constexpr char* DefaultEvaluateExpression = ".";

        /** @brief Name of the program */
        static const constexpr char* YqProgramName = "yq";

        /** @brief The default timeout in MS for calling yq */
        static const constexpr int DefaultTimeoutInMs = 1500;

        /** @brief This regexp is used to verify the format of the version command return */
        static const constexpr char *VersionFormatRegExp =
                                        R"(yq \(https:\/\/.*\) version v[0-9]+\.[0-9]+\.[0-9]+)";

    private:
        ProcessCaller *_processCaller{nullptr};
};
