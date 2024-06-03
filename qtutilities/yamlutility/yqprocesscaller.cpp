// SPDX-FileCopyrightText: 2022 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "yqprocesscaller.hpp"

#include <QDebug>
#include <QFile>

#include "definesutility/definesutility.hpp"
#include "processutility/processcaller.hpp"


YqProcessCaller::YqProcessCaller(QObject *parent)
    : QObject{parent},
      _processCaller(new ProcessCaller(YqProcessCaller::YqProgramName, this))
{
}

YqProcessCaller::~YqProcessCaller()
{
}

bool YqProcessCaller::initProcess(const QString &yqPrgmPath)
{
    RETURN_IF_FALSE(_processCaller->initProcess(yqPrgmPath, YqProcessCaller::DefaultTimeoutInMs));

    QString stdOutput;
    RETURN_IF_FALSE(_processCaller->call({ YqProcessCaller::VersionCommand }, &stdOutput));
    qInfo() << "Version of the yq program used is: " << stdOutput;

    return true;
}

bool YqProcessCaller::call(const QStringList &arguments, QString *stdOutput)
{
    return _processCaller->call(arguments, stdOutput);
}

bool YqProcessCaller::convertYamlFileToJsonFile(const QString &yamlFilePath,
                                                const QString &jsonFilePath,
                                                int exportIndent)
{
    if(exportIndent < 0)
    {
        qWarning() << "The export indent can't be negative: " << exportIndent << ", when trying to "
                   << "convert yaml file: " << yamlFilePath << ", to json file: " << jsonFilePath;
        return false;
    }

    QFile jsonFile(jsonFilePath);

    if(!_processCaller->call({QString(YqProcessCaller::OutputCmd).arg(YqProcessCaller::OutputJsonValue),
                              QString(YqProcessCaller::IndentOutputCmd).arg(exportIndent),
                               QString(YqProcessCaller::DefaultEvaluateExpression),
                              yamlFilePath
                             },
                             jsonFile))
    {
        qWarning() << "A problem occurred when tried to convert the yaml file: "
                   << yamlFilePath << ", to JSON file: " << jsonFilePath;
        return false;
    }

    return true;
}
