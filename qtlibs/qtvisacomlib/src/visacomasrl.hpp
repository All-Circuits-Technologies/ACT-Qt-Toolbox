// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "avisacom.hpp"

#include <QDebug>

#include "serial/flowcontrol.hpp"
#include "serial/parity.hpp"
#include "serial/stopbits.hpp"


/** @brief Useful class used to manage serial communication */
class VISACOM_EXPORT VisacomAsrl: public AVisacom
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param interface the interface needed to address the correct instrument
            @param visaManager The visa com manager
            @param parent The parent class */
        explicit VisacomAsrl(const QString &interfaceId,
                             VisacomManager &visaManager,
                             QObject *parent = nullptr);

        /** @brief generate a interface id for session creation
            @param comPort the com port used for serial communication
            @return SERIAL interface id with QString format */
        static QString generateInterfaceId(quint16 comPort);

        /** @brief Set the baudrate value for serial communication
            @param baud the baudrate parameter */
        bool setBaudrate(quint32 baud);

        /** @brief Set the data bits value for serial communication
            @param dataBits the data bits parameter */
        bool setDataBits(quint16 dataBits);

        /** @brief Set the flow control value for serial communication
            @param flowcontrol the flow control parameter
            @note flowcontrol must be defined with Enum value */
        bool setFlowControl(FlowControl::Enum flowcontrol);

        /** @brief Set the parity value for serial communication
            @param parity the parity parameter
            @note parity must be defined with Enum value */
        bool setParity(Parity::Enum parity);

        /** @brief Set the stop bits value for serial communication
            @param stopBits the stop bits parameter
            @note stopBits must be defined with Enum value */
        bool setStopBits(StopBits::Enum stopBits);

        /** @brief Return the baudrate value from serial communication */
        bool getBaudrate(quint32 &baud);

        /** @brief Return the dataBits value from serial communication */
        bool getDataBits(quint16 &dataBits);

        /** @brief Return the flowcontrol value from serial communication */
        bool getFlowControl(quint16 &flowcontrol);

        /** @brief Return the parity value from serial communication */
        bool getParity(quint16 &parity);

        /** @brief Return the stopBits value from serial communication */
        bool getStopBits(quint16 &stopBits);

    private:
        static const constexpr char* asrlItf = "ASRL%1::INSTR";
};
