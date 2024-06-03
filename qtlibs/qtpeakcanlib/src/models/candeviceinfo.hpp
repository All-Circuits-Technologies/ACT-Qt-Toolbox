// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "src/definescan.hpp"
#include "src/pcanapi/pcanbusitf.hpp"


/** @brief This is the information lined to the Can device */
class CAN_EXPORT CanDeviceInfo : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param pCanBusIntf The CAN bus interface linked to the CAN device
            @param deviceName The name of the CAN device
            @param isCanFd True if we communicate through CAN FD
            @param parent The parent instance */
        explicit CanDeviceInfo(PCanBusItf::Enum pCanBusIntf,
                               const QString &deviceName,
                               bool isCanFd,
                               QObject *parent = nullptr);

        /** @brief Default class constructor
            @note This generates a null info
            @param parent The parent instance */
        explicit CanDeviceInfo(QObject *parent = nullptr);

        /** @brief Copy constructor
            @param otherElement The element to copy */
        explicit CanDeviceInfo(const CanDeviceInfo &otherElement);

        /** @brief Class destructor */
        virtual ~CanDeviceInfo() override;

    public:
        /** @brief Get the CAN bus interface key */
        PCanBusItf::Enum getPCanBusIntf() const { return _pCanBusIntf; }

        /** @brief Get the device name */
        QString getDeviceName() const { return _deviceName; }

        /** @brief Say if we communication through CAN FD or not */
        bool isCanFd() const { return _isCanFd; }

        /** @brief Test if the info is null */
        bool isNull() const { return _pCanBusIntf == PCanBusItf::Unknown; }

    public:
        /** @brief Equal operator
            @param otherConfig The config to copy
            @return The current object instance */
        CanDeviceInfo &operator=(const CanDeviceInfo &otherElement);

    private:
        PCanBusItf::Enum _pCanBusIntf{PCanBusItf::Unknown};
        QString _deviceName;
        bool _isCanFd;
};
