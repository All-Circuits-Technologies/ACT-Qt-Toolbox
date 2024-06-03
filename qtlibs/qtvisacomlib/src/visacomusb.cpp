// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "visacomusb.hpp"

#include "visa.h"


VisacomUsb::VisacomUsb(const QString &interfaceId, VisacomManager &visaManager, QObject *parent):
    AVisacom(interfaceId, visaManager, parent)
{
}

QString VisacomUsb::generateInterfaceId(quint16 usbNumber,
                                        quint32 vid,
                                        quint32 pid,
                                        const QString &serialNumber,
                                        quint16 interfaceNumber)
{
    return QString(usbItf).arg(usbNumber).arg(vid).arg(pid).arg(serialNumber).arg(interfaceNumber);
}
