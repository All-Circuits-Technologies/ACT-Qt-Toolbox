// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "avisacom.hpp"

#include <QString>

/** @brief Useful class used to manage usb communication */
class VISACOM_EXPORT VisacomUsb: public AVisacom
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param interface the interface needed to address the correct instrument
            @param visaManager The visa com manager
            @param parent The parent class */
        explicit VisacomUsb(const QString &interfaceId,
                            VisacomManager &visaManager,
                            QObject *parent = nullptr);

        /** @brief generate a interface id for session creation
            @param usbNumber the device number used for usb communication
            @param vid the vid of the device
            @param pid the pid of the device
            @param serialNumber the serial number of the device
            @param interfaceNumber the usb interface used for usb communication
            @return USB interface id with QString format */
        static QString generateInterfaceId(quint16 usbNumber,
                                           quint32 vid,
                                           quint32 pid,
                                           const QString &serialNumber,
                                           quint16 interfaceNumber);

    private:
        static const constexpr char *usbItf = "USB%1::%2::%3::%4::%5::INSTR";

};
