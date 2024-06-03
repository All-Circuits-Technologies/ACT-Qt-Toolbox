// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "avisacom.hpp"

#include <QDebug>
#include <QString>


/** @brief Useful class used to manage gpib 488.2 synchronous & asynchronous communication */
class VISACOM_EXPORT VisacomGpib : public AVisacom
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param interface the interface needed to address the correct instrument
            @param visaManager The visa com manager
            @param parent The parent class */
        explicit VisacomGpib(const QString &interfaceId,
                             VisacomManager &visaManager,
                             QObject *parent = nullptr);

        /** @brief generate a interface id for session creation
            @param gpibNumber the device number used for Gpib communication
            @param address the gpib address used for Gpib communication
            @return GPIB interface id with QString format */
        static QString generateInterfaceId(quint16 gpibNumber, quint16 address);

        /** @brief Set the End of Line value for gpib communication
            @param statusEOI the EOI parameter */
        bool setEOI(bool statusEOI);

        /** @brief Set the secondary address value for gpib communication (if needed)
            @param secondAddr the secondary address parameter
            @note This function must be used only when secondary address needed */
        bool setSecondaryAddr(quint16 secondAddr);

        /** @brief Return the End of Line status for gpib communication */
        bool getEOI(bool &statusEOI);

        /** @brief Return the secondary address value for gpib communication */
        bool getSecondaryAddr(quint16 &secondAddr);

    private:
        static const constexpr char *gpibItf = "GPIB%1::%2::INSTR";
};
