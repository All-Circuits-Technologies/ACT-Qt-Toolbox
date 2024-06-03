// SPDX-FileCopyrightText: 2022 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

class QFile;


/** @brief Useful class to manage external process calling */
class ProcessCaller : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param programName The name of the program to call, only used in logs
            @param parent The parent instance */
        explicit ProcessCaller(const QString &programName, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ProcessCaller() override;

    public:
        /** @brief Init the process calling
            @param processPath The path of the process to call
            @param defaultTimeout The calling timeout to apply by default
                                  If set to -1, there is no timeout
            @param defaultArguments The default arguments to set at each calls
            @return True if no problem occurred */
        bool initProcess(const QString &processPath,
                         int defaultTimeout = -1,
                         const QStringList &defaultArguments = {});

        /** @brief Call the external process
            @note Init process should have been called before this method
            @see ProcessCaller::callPriv
            @param arguments The arguments to call the process with
            @param stdOutput If not null, the standard output is written in this string
            @param logProcessError If true and if an error occurred, they will be written in logs
            @param overrideTimeoutInMs This param is used to override the timeout sets by the user
                                       (if not equals to -1)
            @param processExitProperly If not null, this will say if the process exits properly,
                                       the param is equals to false except when the process succeeds
                                       and no error occurs
            @return True if no problem occurred */
        bool call(const QStringList &arguments,
                  QString* stdOutput = nullptr,
                  bool logProcessError = true,
                  int overrideTimeoutInMs = -1,
                  bool *processExitProperly = nullptr);

        /** @brief Call the external process
            @note Init process should have been called before this method
            @see ProcessCaller::callPriv
            @param arguments The arguments to call the process with
            @param stdOutputFile If not null, the standard output is written in this file
            @param logProcessError If true and if an error occurred, they will be written in logs
            @param overrideTimeoutInMs This param is used to override the timeout sets by the user
                                       (if not equals to -1)
            @param processExitProperly If not null, this will say if the process exits properly,
                                       the param is equals to false except when the process succeeds
                                       and no error occurs
            @return True if no problem occurred */
        bool call(const QStringList &arguments,
                  QFile& stdOutputFile,
                  bool logProcessError = true,
                  int overrideTimeoutInMs = -1,
                  bool *processExitProperly = nullptr);

    public:
        /** @brief Call the external process
            @see ProcessCaller::callPriv
            @param programName The name of the program to call, only used in logs
            @param processPath The path of the process to call
            @param arguments The arguments to call the process with
            @param stdOutput If not null, the standard output is written in this string
            @param logProcessError If true and if an error occurred, they will be written in logs
            @param timeoutInMs This param is used set a timeout on the process call
                               If -1 no timeout is set
            @param processExitProperly If not null, this will say if the process exits properly,
                                       the param is equals to false except when the process succeeds
                                       and no error occurs
            @return True if no problem occurred */
        static bool call(const QString &programName,
                         const QString &processPath,
                         const QStringList &arguments,
                         QString* stdOutput = nullptr,
                         bool logProcessError = true,
                         int timeoutInMs = -1,
                         bool *processExitProperly = nullptr);

        /** @brief Call the external process
            @see ProcessCaller::callPriv
            @param programName The name of the program to call, only used in logs
            @param processPath The path of the process to call
            @param arguments The arguments to call the process with
            @param stdOutputFile If not null, the standard output is written in this file
            @param logProcessError If true and if an error occurred, they will be written in logs
            @param timeoutInMs This param is used set a timeout on the process call
                               If -1 no timeout is set
            @param processExitProperly If not null, this will say if the process exits properly,
                                       the param is equals to false except when the process succeeds
                                       and no error occurs
            @return True if no problem occurred */
        static bool call(const QString &programName,
                         const QString &processPath,
                         const QStringList &arguments,
                         QFile& stdOutputFile,
                         bool logProcessError = true,
                         int timeoutInMs = -1,
                         bool *processExitProperly = nullptr);

    private:
        /** @brief Call the external process, private method
            @note  The @ref stdOutputFile parameter is prioritary on the @ref stdOutput parameter,
                   both can't be set at the same time
            @param arguments The arguments to call the process with
            @param logProcessError If true and if an error occurred, they will be written in logs
            @param overrideTimeoutInMs This param is used to override the timeout sets by the user
                                       (if not equals to -1)
            @param stdOutputFile If not null, the standard output is written in this file
            @param stdOutput If not null, the standard output is written in this string
            @param processExitProperly If not null, this will say if the process exits properly,
                                       the param is equals to false except when the process succeeds
                                       and no error occurs
            @return True if no problem occurred */
        bool callPriv(const QStringList &arguments,
                      bool logProcessError = true,
                      int overrideTimeoutInMs = -1,
                      QFile* stdOutputFile = nullptr,
                      QString* stdOutput = nullptr,
                      bool *processExitProperly = nullptr);

    private:
        /** @brief Character number limit, in order to not display all the output of Program output
                   to debug, only the last chars */
        static const constexpr int ProcessLogCharLimitToDisplay { 900 };

    private:
        QString _programName;

        QString _processPath;
        int _defaultTimeout{-1};
        QStringList _defaultArguments{};
};
