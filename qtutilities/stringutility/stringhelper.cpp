// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "stringhelper.hpp"

#include <QStringList>


StringHelper::StringHelper()
{
}

quint8 StringHelper::toQuint8(const QString &value, bool *ok, int base)
{
    quint64 tmpValue = value.toULongLong(ok, base);
    if(tmpValue > std::numeric_limits<quint8>::max())
    {
        if(ok != nullptr)
        {
            *ok = false;
        }
        tmpValue = 0;
    }

    return static_cast<quint8>(tmpValue);
}

qint8 StringHelper::toQint8(const QString &value, bool *ok, int base)
{
    qint64 tmpValue = value.toLongLong(ok, base);
    if(tmpValue < std::numeric_limits<qint8>::min() ||
        tmpValue > std::numeric_limits<qint8>::max())
    {
        if(ok != nullptr)
        {
            *ok = false;
        }
        tmpValue = 0;
    }

    return static_cast<qint8>(tmpValue);
}

bool StringHelper::toBool(const QString &value, bool *ok)
{
    QString lowerValue = value.toLower();
    bool tmpValue = false;

    if(lowerValue == StrTrueValue || lowerValue == StrNumberTrueValue)
    {
        tmpValue = true;

        if(ok != nullptr)
        {
            *ok = true;
        }

    }
    else if(lowerValue == StrFalseValue || lowerValue == StrNumberFalseValue)
    {
        if(ok != nullptr)
        {
            *ok = true;
        }
    }
    else if(ok != nullptr)
    {
        *ok = false;
    }

    return tmpValue;
}

QStringList StringHelper::splitAndKeepSepOnEnd(const QString &source,
                                               const QString &sep,
                                               Qt::SplitBehavior behavior,
                                               Qt::CaseSensitivity cs)
{
    if(sep.isEmpty() || source.isEmpty())
    {
        // In those cases, we keep the split normal behavior
        return source.split(sep, behavior, cs);
    }

    // We first apply the Qt method and keeps the empty parts.
    // In the second iteration we will remove the empty parts.
    QStringList list = source.split(sep, Qt::KeepEmptyParts, cs);

    for(int idx = 0; idx < list.length() - 1; ++idx)
    {
        // We don't append the separator on the last element
        list[idx].append(sep);
    }

    if(behavior == Qt::SkipEmptyParts && list.last().isEmpty())
    {
        // In that case, we don't keep the last part because it will still be empty
        list.removeLast();
    }

    return list;
}

QStringList StringHelper::splitAndKeepSepOnStart(const QString &source,
                                                 const QString &sep,
                                                 Qt::SplitBehavior behavior,
                                                 Qt::CaseSensitivity cs)
{
    if(sep.isEmpty() || source.isEmpty())
    {
        // In those cases, we keep the split normal behavior
        return source.split(sep, behavior, cs);
    }

    // We first apply the Qt method and keeps the empty parts.
    // In the second iteration we will remove the empty parts.
    QStringList list = source.split(sep, Qt::KeepEmptyParts, cs);

    for(int idx = 1; idx < list.length(); ++idx)
    {
        // We don't prepend the separator on the first element
        list[idx].prepend(sep);
    }

    if(behavior == Qt::SkipEmptyParts && list.first().isEmpty())
    {
        // In that case, we don't keep the first part because it will still be empty
        list.removeFirst();
    }

    return list;
}
