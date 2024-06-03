// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "logmsgtype.hpp"

#include <QMetaEnum>

QVector<LogMsgType::Enum> LogMsgType::EnumList = {};


bool LogMsgType::isEqualOrAboveCriticity(Enum logType, Enum criticityLimit)
{
    return (logType >= criticityLimit);
}

LogMsgType::Enum LogMsgType::parseCriticityFromString(const QString &logCriticity)
{
    QString lowerLogCrit = logCriticity.toLower();
    LogMsgType::Enum msg = Unknown;

    if(lowerLogCrit.contains("debug"))
    {
        msg = Debug;
    }
    else if(lowerLogCrit.contains("info"))
    {
        msg = Info;
    }
    else if(lowerLogCrit.contains("warning"))
    {
        msg = Warning;
    }
    else if(lowerLogCrit.contains("critical"))
    {
        msg = Critical;
    }
    else if(lowerLogCrit.contains("fatal"))
    {
        msg = Fatal;
    }

    return msg;
}

LogMsgType::Enum LogMsgType::parseCriticityFromQt(QtMsgType msgType)
{
    switch(msgType)
    {
        case QtMsgType::QtDebugMsg:
            return Debug;
        case QtMsgType::QtInfoMsg:
            return Info;
        case QtMsgType::QtWarningMsg:
            return Warning;
        case QtMsgType::QtCriticalMsg:
            return Critical;
        case QtMsgType::QtFatalMsg:
            return Fatal;
    }

    return Unknown;
}

const QVector<LogMsgType::Enum> &LogMsgType::getAvailableElements()
{
    if(EnumList.isEmpty())
    {
        const QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

        for(int idx = 0; idx < metaEnum.keyCount(); idx++)
        {
            EnumList.append(static_cast<Enum>(metaEnum.value(idx)));
        }
    }

    return EnumList;
}

QtMsgType LogMsgType::toQtMsgType(Enum msgType, bool *ok, QtMsgType defaultValue)
{
    auto manageOk = [ok](bool isOk)
    {
        if(ok != nullptr)
        {
            *ok = isOk;
        }
    };

    switch(msgType)
    {
        case Debug:
            manageOk(true);
            return QtMsgType::QtDebugMsg;

        case Info:
            manageOk(true);
            return QtMsgType::QtInfoMsg;

        case Warning:
            manageOk(true);
            return QtMsgType::QtWarningMsg;

        case Critical:
            manageOk(true);
            return QtMsgType::QtCriticalMsg;

        case Fatal:
            manageOk(true);
            return QtMsgType::QtFatalMsg;

        case Unknown:
            break;
    }

    manageOk(false);
    return defaultValue;
}

QString LogMsgType::toString(Enum logType)
{
    switch(logType)
    {
        case Debug:
            return DebugMsgStr;

        case Info:
            return InfoMsgStr;

        case Warning:
            return WarningMsgStr;

        case Critical:
            return CriticalMsgStr;

        case Fatal:
            return FatalMsgStr;

        case Unknown:
            return {};
    }

    return {};
}

QString LogMsgType::toTrString(Enum logType)
{
    switch(logType)
    {
        case Debug:
            return tr(TrDebugMsgStr);

        case Info:
            return tr(TrInfoMsgStr);

        case Warning:
            return tr(TrWarningMsgStr);

        case Critical:
            return tr(TrCriticalMsgStr);

        case Fatal:
            return tr(TrFatalMsgStr);

        case Unknown:
            return {};
    }

    return {};
}

QString LogMsgType::toOneLetterTrString(Enum logType)
{
    switch(logType)
    {
        case Debug:
            return tr(ShortTrDebugMsgStr);

        case Info:
            return tr(ShortTrInfoMsgStr);

        case Warning:
            return tr(ShortTrWarningMsgStr);

        case Critical:
            return tr(ShortTrCriticalMsgStr);

        case Fatal:
            return tr(ShortTrFatalMsgStr);

        case Unknown:
            return {};
    }

    return {};
}

QString LogMsgType::toLogString(Enum logType)
{
    switch(logType)
    {
        case Debug:
            return LogDebugMsgStr;

        case Info:
            return LogInfoMsgStr;

        case Warning:
            return LogWarningMsgStr;

        case Critical:
            return LogCriticalMsgStr;

        case Fatal:
            return LogFatalMsgStr;

        case Unknown:
            return {};
    }

    return {};
}

void LogMsgType::registerMetaType()
{
    qRegisterMetaType<LogMsgType::Enum>("LogMsgType::Enum");
}
