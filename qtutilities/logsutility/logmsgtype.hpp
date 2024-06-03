// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief This class has for goal to replace the @ref QtMsgType enum
    @note The enum is ordered in the right order of criticity (at the opposite of @ref QtMsgType,
          where QtInfoMsg has been added after the others and it's not in the right place)
    @note Because the enum is stored in a QObject class it allowes to use the Q_ENUM macro and so
          to be easily used in QML
    @note We have added the Unknown enum value to manage particular case */
class LogMsgType : public QObject
{
    Q_OBJECT

    public:
        /** @brief Contains the same log criticity level as @ref QtMsgType enum */
        enum Enum
        {
            Debug       = 0,
            Info        = 1,
            Warning     = 2,
            Critical    = 3,
            System      = Critical,
            Fatal       = 4,
            Unknown
        };
        Q_ENUM(Enum)

    public:
        /** @brief Test if the Msg type is equal or greater than the criticity limit
            @note Test and return true if: logType >= criticityLimit
            @param logType The log type to compare with limit
            @param criticityLimit The limit to compare the log type with
            @return True if no problem occurs */
        static bool isEqualOrAboveCriticity(Enum logType, Enum criticityLimit);

        /** @brief Parse LogMsgType from a string representation
            @param logCriticity The MsgType to parse
            @return The msg type parsed from string or Unknown if parse failed */
        static Enum parseCriticityFromString(const QString &logCriticity);

        /** @brief Parse LogMsgType from a QtMsgType enum value
            @param msgType The MsgType to convert
            @return The msg type parsed from QtMsgType */
        static Enum parseCriticityFromQt(QtMsgType msgType);

        /** @brief Get all the available elements
            @note Unknown enum is included
            @return The list of all enum values */
        static const QVector<Enum> &getAvailableElements();

        /** @brief Convert the LogMsgType::Enum to QtMsgType
            @param msgType The msg type to convert
            @param ok If not null, set to true if the converting is succes or false if failed
            @param defaultValue The default value to return if the converting failed
            @return The msg type converted */
        static QtMsgType toQtMsgType(Enum msgType,
                                     bool *ok = nullptr,
                                     QtMsgType defaultValue = QtMsgType::QtWarningMsg);

        /** @brief Convert the LogMsgType::Enum to string
            @param logType The log type to stringifyed
            @return String representation of the LogMsgType::Enum given */
        static QString toString(Enum logType);

        /** @brief Convert the LogMsgType::Enum to a translated string
            @param logType The log type to stringify
            @return A translated string representation of the LogMsgType::Enum given */
        static QString toTrString(Enum logType);

        /** @brief Convert the LogMsgType::Enum to a one letter translated string
            @param logType The log type to stringify
            @return A one letter translated string representation of the LogMsgType::Enum given */
        static QString toOneLetterTrString(Enum logType);

        /** @brief Convert the LogMsgType::Enum to a string representation which can be used in a
                   log file
            @note The log type is formatted like this: [xxxx], ex: [Dbug]
            @param logType The log type to stringified
            @return The string representation of the log type */
        static QString toLogString(Enum logType);

        /** @brief Allows to register the meta type of the LogMsgType::Enum */
        static void registerMetaType();

    private:
        static const constexpr char* DebugMsgStr =      "LogDebugMsg";
        static const constexpr char* InfoMsgStr =       "LogInfoMsg";
        static const constexpr char* WarningMsgStr =    "LogWarningMsg";
        static const constexpr char* CriticalMsgStr =   "LogCriticalMsg";
        static const constexpr char* FatalMsgStr =      "LogFatalMsg";

        static const constexpr char* TrDebugMsgStr =    QT_TR_NOOP("(TR) LogDebugMsg");
        static const constexpr char* TrInfoMsgStr =     QT_TR_NOOP("(TR) LogInfoMsg");
        static const constexpr char* TrWarningMsgStr =  QT_TR_NOOP("(TR) LogWarningMsg");
        static const constexpr char* TrCriticalMsgStr = QT_TR_NOOP("(TR) LogCriticalMsg");
        static const constexpr char* TrFatalMsgStr =    QT_TR_NOOP("(TR) LogFatalMsg");

        static const constexpr char* ShortTrDebugMsgStr =    QT_TR_NOOP("(TR) D");
        static const constexpr char* ShortTrInfoMsgStr =     QT_TR_NOOP("(TR) I");
        static const constexpr char* ShortTrWarningMsgStr =  QT_TR_NOOP("(TR) W");
        static const constexpr char* ShortTrCriticalMsgStr = QT_TR_NOOP("(TR) C");
        static const constexpr char* ShortTrFatalMsgStr =    QT_TR_NOOP("(TR) F");

        static const constexpr char* LogDebugMsgStr =       "[Dbug]";
        static const constexpr char* LogInfoMsgStr =        "[Info]";
        static const constexpr char* LogWarningMsgStr =     "[Warn]";
        static const constexpr char* LogCriticalMsgStr =    "[Crit]";
        static const constexpr char* LogFatalMsgStr =       "[Fatl]";

    private:
        static QVector<Enum> EnumList;
};
