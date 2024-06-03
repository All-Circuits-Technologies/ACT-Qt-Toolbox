// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "src/definescan.hpp"
#include "src/pcanapi/pcanbaudrate.hpp"


/** @brief This is the config details for the CAN
    @todo This class has been created but doesn't contain the needed information for creating a
          CAN connection */
class CAN_EXPORT CanDeviceConfigDetails : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class contructor
            @note This generates an invalid config
            @param parent The parent instance */
        explicit CanDeviceConfigDetails(QObject *parent = nullptr);

        /** @brief Copy constructor
            @param copy The element to copy */
        explicit CanDeviceConfigDetails(const CanDeviceConfigDetails& copy);

        /** @brief Class destructor */
        virtual ~CanDeviceConfigDetails() override;

    public:
        /** @brief Get the Baud rate value
            @return The value got */
        PCanBaudRate::Enum getBaudRate() const { return _baudRate; }

        /** @brief Set the baudRate value
            @param value The new value to set */
        void setBaudRate(PCanBaudRate::Enum value) { _baudRate = value; }

        /** @brief Test if the config is valid
            @return True if the class is valid */
        bool isValid() const;

    public:
        /** @brief Equal operator
            @param otherConfig The config to copy
            @return The current object instance */
        CanDeviceConfigDetails &operator=(const CanDeviceConfigDetails &otherConfig);

    private:
        PCanBaudRate::Enum _baudRate{PCanBaudRate::Unknown};
};
