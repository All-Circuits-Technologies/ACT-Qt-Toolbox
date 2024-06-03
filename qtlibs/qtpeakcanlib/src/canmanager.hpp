// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include "handlerutility/handlerclassmembersmixin.hpp"

#include "src/definescan.hpp"

#include "src/models/candeviceinfo.hpp"
#include "src/pcanapi/pcanbusitf.hpp"

class CanDeviceConfig;
class CanDeviceIntf;


/** @brief This class manages the @ref CanDeviceIntf creation and getting */
class CAN_EXPORT CanManager : public QObject,
                                     HandlerClassMembersMixin<PCanBusItf::Enum, CanDeviceIntf>
{
    Q_OBJECT

    private:
        /** @brief Class constructor
            @param parent The class parent */
        explicit CanManager(QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~CanManager() override;

    public:
        /** @brief Return the singleton instance of the Can manager
            @note If no instance created, getInsantce force Can manager instance creation
            @return The Can manager instance */
        static CanManager &getInstance();

    public:
        /** @brief Get a @ref CanDeviceIntf thanks to its CAN interface key
            @note The CanDeviceIntf has to be created before in order to be got by this method
            @note This method is thread safe but required an event loop in the caller method
            @param canIntfKey The CAN interface key is the value stored in the @ref CanDeviceIntf
                                 class
            @return The CanDeviceIntf found or a nullptr */
        QSharedPointer<CanDeviceIntf> getCanIntf(PCanBusItf::Enum canIntfKey);

        /** @brief Create or get a @ref CanDeviceIntf thanks to the @ref CanDeviceConfig info given
            @note If the interface already exists, it returns the existent CAN device interface
            @note This method is thread safe but required an event loop in the caller method
            @param config The configuration info to create the CAN device interface
            @return The CanDeviceIntf created or a nullptr */
        QSharedPointer<CanDeviceIntf> createOrGetCanIntf(const CanDeviceConfig &config);

    private:
        /** @brief Get a @ref CanDeviceIntf thanks to its CAN interface key
            @note The CanDeviceIntf has to be created before in order to be got by this method
            @param canIntfKey The CAN interface key is the value stored in the @ref CanDeviceIntf
                                 class
            @return The CanDeviceIntf found or a nullptr */
        QSharedPointer<CanDeviceIntf> getCanIntfPriv(PCanBusItf::Enum canIntfKey);

        /** @brief Create or get a @ref CanDeviceIntf thanks to the @ref CanDeviceConfig info given
            @note If the interface already exists, it returns the existent CAN device interface
            @param config The configuration info to create the CAN device interface
            @return The CanDeviceIntf created or a nullptr */
        QSharedPointer<CanDeviceIntf> createOrGetCanIntfPriv(const CanDeviceConfig &config);

    public:
        /** @brief Find a CAN device matching all non-empty rules
            @note Given strings are case sensitive but can be a subset of CAN device contents.
                  Example a rule deviceName="CAN" will be OK for a real deviceName "PEAK CAN".
            @param canIntfKey The CAN interface key to use
            @param deviceName The device name (set it to null to ignore this rule)
            @param isCanFd Says if we communicate with CAN FD or not (set it to nullptr to ignore
                           this rule)
            @return First matching CAN device info, or a default-constructed (null) CAN device
                    info */
        static CanDeviceInfo findBusDeviceInfo(PCanBusItf::Enum canIntfKey = PCanBusItf::Unknown,
                                               const QString &deviceName = {},
                                               const bool *isCanFd = nullptr);

        /** @brief Get the current available CAN devices
            @return The list of CAN devices */
        static QVector<CanDeviceInfo> getAvailableDevices();

    private:
        /** @brief Register all the meta types linked to the lib
            @note This is called when the CAN Manager instance is created */
        static void registerMetaTypes();

    private:
        /** @brief This is the timeout used for the mutex try lock */
        static const constexpr int MutexTimeoutInMs = 1000;

    private:
        static CanManager *_instance;
};
