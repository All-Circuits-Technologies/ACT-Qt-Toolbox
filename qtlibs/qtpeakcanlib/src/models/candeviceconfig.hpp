// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "src/definescan.hpp"
#include "src/pcanapi/pcanbusitf.hpp"

class CanDeviceConfigDetails;
class CanDeviceFdConfigDetails;


/** @brief This is the config to initialize the @ref CanDevice */
class CAN_EXPORT CanDeviceConfig : public QObject
{
    Q_OBJECT

    public:
        /** @brief Default class constructor which generates an invalid object
            @note This generates an invalid config
            @param parent The parent instance */
        explicit CanDeviceConfig(QObject *parent = nullptr);

        /** @brief Class constructor
            @param isCanFd True if we configure the CAN FD
            @param parent The parent instance */
        explicit CanDeviceConfig(bool isCanFd, QObject *parent = nullptr);

        /** @brief Copy constructor
            @param copy The element to copy */
        explicit CanDeviceConfig(const CanDeviceConfig& copy);

        /** @brief Class destructor */
        virtual ~CanDeviceConfig() override;

    public:
        /** @brief Get the CAN bus interface */
        PCanBusItf::Enum getCanBusItf() const { return _canBusItf; }

        /** @brief Get the interface handle linked to the can bus Interface
            @note The interface handle is needed by the PEAK CAN lib
            @note This method is like calling @ref getCanBusItf and then
                  @ref PCanBusItf::toPCanBusItf
            @return The handle which can be used with the PEAK CAN lib*/
        quint16 getPCanBusItfHandle() const;

        /** @brief Get the name of the can bus interface
            @note This method is like calling @ref getCanBusItf and then
                  @ref PCanBusItf::toString
            @return The string representation of the CAN bus interface */
        QString getCanBusItfName() const;

        /** @brief Set the CAN bus interface linked to the config
            @param canBusItf The CAN bus interface to set */
        void setCanBusItf(const PCanBusItf::Enum &canBusItf) { _canBusItf = canBusItf; }

        /** @brief Test if the config is done for a CAN FD or not
            @return True if the config is for a CAN FD */
        bool isCanFd() const { return _canFdConfig != nullptr; }

        /** @brief Say if the CAN has a flexible data rate
            @note If we change the FD state of the CAN, the linked config details will change: the
                  previous config details will be removed and new fresh one will be created.
                  Therefore you may lose information.
            @param isCanFd True if the CAN has a flexible data rate */
        void setCanFd(bool isCanFd);

        /** @brief Get the CAN config details (if the config is NOT FOR CAN FD)
            @warning This returns a reference to the linked @ref _canConfig member, but if the
                     config is for CAN FD, the class member will be null and the app might crash
            @return The constant reference to the CAN details */
        const CanDeviceConfigDetails& getDetails() const { return *_canConfig; }

        /** @brief Access the CAN config details (if the config is NOT FOR CAN FD)
            @warning This returns a reference to the linked @ref _canConfig member, but if the
                     config is for CAN FD, the class member will be null and the app might crash
            @return The reference to the CAN details */
        CanDeviceConfigDetails& accessDetails() const { return *_canConfig; }

        /** @brief Get the CAN FD config details (if the config is FOR CAN FD)
            @warning This returns a reference to the linked @ref _canFdConfig member, but if the
                     config is for CAN, the class member will be null and the app might crash
            @return The constant reference to the CAN FD details */
        const CanDeviceFdConfigDetails& getFdDetails() const { return *_canFdConfig; }

        /** @brief Access the CAN FD config details (if the config is FOR CAN FD)
            @warning This returns a reference to the linked @ref _canFdConfig member, but if the
                     config is for CAN, the class member will be null and the app might crash
            @return The reference to the CAN FD details */
        CanDeviceFdConfigDetails& accessFdDetails() const { return *_canFdConfig; }

        /** @brief Test if the config and the details configs are valids
            @return True if the class is valid */
        bool isValid() const;

    public:
        /** @brief Equal operator
            @param otherConfig The config to copy
            @return The current object instance */
        CanDeviceConfig &operator=(const CanDeviceConfig &otherConfig);

    private:
        PCanBusItf::Enum _canBusItf{PCanBusItf::Unknown};

        CanDeviceConfigDetails *_canConfig{nullptr};
        CanDeviceFdConfigDetails *_canFdConfig{nullptr};
};

Q_DECLARE_METATYPE(CanDeviceConfig)
