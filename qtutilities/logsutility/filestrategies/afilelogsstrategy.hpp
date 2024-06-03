// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QString>
#include <QVector>

#include "loggingoption.hpp"

class QFile;


/** @brief Abstract class which allow to define a strategy to save logs in files */
class AFileLogsStrategy : public QObject
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param folderPath The path of the main logs folder
            @param parent The class parent */
        explicit AFileLogsStrategy(const QString &folderPath, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AFileLogsStrategy() override = default;

    public:
        /** @brief Say if the logging has been started */
        bool isStarted() const { return _loggingStarted; }

    public slots:
        /** @brief Start logging in files
            @return True if no problem occurs */
        virtual bool start();

        /** @brief Stop logging in files
            @return True if no problem occurs */
        virtual bool stop();

        /** @brief Call to write the log message in the files
            @param msg The msg to write in files
            @param options Optional logging arguments, the usage depends of the strategy chosen
            @return True if no problem occurs */
        virtual bool writeLog(const QString &msg,
                              const LoggingOptions &options = LoggingOptions()) = 0;

        /** @brief Append a message to the log file base name
            @param toAppend The suffix to add to the log file base name
            @param options Optional logging arguments, the usage depends of the strategy chosen
            @return True if no problem occurs */
        virtual bool appendToBaseName(const QString &toAppend,
                                      const LoggingOptions &options = LoggingOptions()) = 0;

    protected:
        /** @brief Get the path of the main logs folder */
        const QString &getFolderPath() const { return _folderPath; }

    private:
        const QString _folderPath;
        bool _loggingStarted{false};
};
