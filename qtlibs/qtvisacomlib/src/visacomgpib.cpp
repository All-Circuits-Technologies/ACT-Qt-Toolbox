// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "visacomgpib.hpp"

#include "visa.h"


VisacomGpib::VisacomGpib(const QString &interfaceId,
                         VisacomManager &visaManager,
                         QObject *parent):
    AVisacom(interfaceId, visaManager, parent)
{
}

QString VisacomGpib::generateInterfaceId(quint16 gpibNumber, quint16 address)
{
    return QString(gpibItf).arg(gpibNumber).arg(address);
}

bool VisacomGpib::setEOI(bool statusEOI)
{
    return AVisacom::setAttribute(VI_ATTR_SEND_END_EN, statusEOI);
}

bool VisacomGpib::setSecondaryAddr(quint16 secondAddr)
{
    return AVisacom::setAttribute(VI_ATTR_GPIB_SECONDARY_ADDR, secondAddr);
}

bool VisacomGpib::getEOI(bool &statusEOI)
{
    return AVisacom::getAttribute(VI_ATTR_SEND_END_EN, statusEOI);
}

bool VisacomGpib::getSecondaryAddr(quint16 &secondAddr)
{
    return AVisacom::getAttribute(VI_ATTR_GPIB_SECONDARY_ADDR, secondAddr);
}
