// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pcanbaudrate.hpp"

#include <QMetaEnum>

#include "collectionutility/qhashhelper.hpp"

#include "src/pcanapi/import_pcanbasic.hpp"

QHash<PCanBaudRate::Enum, PCanBaudRate::BaudRateInfo> PCanBaudRate::BaudRateInfos = {
    { Baud1M,   { PCAN_BAUD_1M,     Number::fromUInt32(1000000) } },
    { Baud800k, { PCAN_BAUD_800K,   Number::fromUInt32( 800000) } },
    { Baud500k, { PCAN_BAUD_500K,   Number::fromUInt32( 500000) } },
    { Baud250k, { PCAN_BAUD_250K,   Number::fromUInt32( 250000) } },
    { Baud125k, { PCAN_BAUD_125K,   Number::fromUInt32( 125000) } },
    { Baud100k, { PCAN_BAUD_100K,   Number::fromUInt32( 100000) } },
    { Baud95k,  { PCAN_BAUD_95K,    Number::fromUInt32(  95238) } },
    { Baud83k,  { PCAN_BAUD_83K,    Number::fromUInt32(  83333) } },
    { Baud50k,  { PCAN_BAUD_50K,    Number::fromUInt32(  50000) } },
    { Baud47k,  { PCAN_BAUD_47K,    Number::fromUInt32(  47619) } },
    { Baud33k,  { PCAN_BAUD_33K,    Number::fromUInt32(  33333) } },
    { Baud20k,  { PCAN_BAUD_20K,    Number::fromUInt32(  20000) } },
    { Baud10k,  { PCAN_BAUD_10K,    Number::fromUInt32(  10000) } },
    { Baud5k,   { PCAN_BAUD_5K,     Number::fromUInt32(   5000) } }
};


const Number &PCanBaudRate::toRealValue(Enum value)
{
    if(!BaudRateInfos.contains(value))
    {
        qWarning() << "The baudrate value given is unknown, we can't get the real value.";
        return Number::Invalid;
    }

    return BaudRateInfos[value].realValue;
}

quint16 PCanBaudRate::toApiCode(Enum value, bool *ok)
{
    if(!BaudRateInfos.contains(value))
    {
        qWarning() << "The baudrate value given is unknown, we can't get the API code.";

        if(ok != nullptr)
        {
            *ok = false;
        }

        return DefaultApiCodeError;
    }

    if(ok != nullptr)
    {
        *ok = true;
    }

    return BaudRateInfos[value].apiCode;
}

QString PCanBaudRate::toRealValueString(Enum value)
{
    if(!BaudRateInfos.contains(value))
    {
        qWarning() << "The baudrate value given is unknown, we can't display the real value.";
        return DefaultUnknownString;
    }

    return BaudRateInfos[value].realValue.toString();
}

PCanBaudRate::Enum PCanBaudRate::parseFromRealValue(const Number &realBaudRateValue)
{
    if(Q_UNLIKELY(!realBaudRateValue.isValid()))
    {
        qWarning() << "The baud rate value given isn't valid, we can't convert it to "
                   << "PCanBaudRate::Enum";
        return Unknown;
    }

    if(Q_UNLIKELY(realBaudRateValue.isNegative() || realBaudRateValue.isInfinite()))
    {
        qWarning() << "The baud rate value given is negative or infinite: " << realBaudRateValue
                   << ", we can't convert it to PCanBaudRate::Enum";
        return Unknown;
    }

    for(auto citer = BaudRateInfos.cbegin(); citer != BaudRateInfos.cend(); ++citer)
    {
        if(citer.value().realValue == realBaudRateValue)
        {
            return citer.key();
        }
    }

    qWarning() << "The wanted baudrate: " << realBaudRateValue << ", isn't known by the library. "
               << "But it could be added programatically in the plugin.";

    return Unknown;
}

QVector<PCanBaudRate::Enum> PCanBaudRate::getAvailableElements(bool excludeUnknown)
{
    QVector<PCanBaudRate::Enum> valuesList = QHashHelper::getKeys(BaudRateInfos);

    if(!excludeUnknown)
    {
        valuesList.append(Unknown);
    }

    return valuesList;
}
