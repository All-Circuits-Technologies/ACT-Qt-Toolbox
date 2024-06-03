// SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

/* ------------------------------------------------------------------
 * crchelper.cpp - COPYRIGHT (C) 2019 ALL CIRCUITS
 * ------------------------------------------------------------------
 */

#include "crchelper.hpp"

#include <endianesshelper.hpp>


CrcHelper *CrcHelper::_instance = Q_NULLPTR;


CrcHelper::CrcHelper()
{
}

CrcHelper &CrcHelper::getInstance()
{
    if(_instance == Q_NULLPTR)
    {
        _instance = new CrcHelper();
    }

    return *_instance;
}

quint16 CrcHelper::calculateCrcMcrf4xx(const QByteArray &data, quint16 init)
{
    quint16 crc = init;

    for(char byte : data)
    {
        quint8 tmp = static_cast<quint8>(byte);

        tmp ^= EndianessHelper::getByteFromLsbToMsb(crc, 0);
        tmp ^= (tmp << 4) & 0xF0;

        crc = static_cast<quint16>(tmp << 8) | EndianessHelper::getByteFromLsbToMsb(crc, 1);
        crc ^= ((tmp >> 4) & 0x0F);
        crc ^= (tmp << 3);
    }

    return crc;
}

quint32 CrcHelper::calculateCrc32(const QByteArray &data, quint32 polynom, quint32 init)
{
    getInstance().createCrc32Table(polynom);

    quint32 crc = init;

    for(int idx = 0; idx < data.length(); idx++)
    {
        quint8 tableIndex = ((static_cast<quint8>(data.at(idx)) ^ crc) & 0xFF);

        crc = (crc >> 8) ^ getInstance()._crc32Table[tableIndex];
    }

    return ~crc;
}

void CrcHelper::createCrc32Table(quint32 polynom)
{
    if(_crc32Table != nullptr)
    {
        // Table already created
        return;
    }

    _crc32Table = new quint32[CrcConstants::Crc32::tableSize]();

    quint32 element = 0;

    for(quint32 idxTab = 0; idxTab < CrcConstants::Crc32::tableSize; idxTab++)
    {
        element = idxTab;
        for(quint32 idxIterNb = 0;
            idxIterNb < CrcConstants::Crc32::tableCreationIterNb;
            idxIterNb++)
        {
            if((element & 0x01) == 0x01)
            {
                element = (element >> 0x01) ^ polynom;
            }
            else
            {
                element = element >> 0x01;
            }
        }

        _crc32Table[idxTab] = element;
    }
}
