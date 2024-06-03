// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QHash>

#include "src/models/candeviceinfo.hpp"
#include "src/pcanapi/pcanbusitf.hpp"

class CanDeviceConfig;
class QCanBusFrame;


/** @brief This class wraps PCANLib methods to use them with Qt
    @note The class contains static methods to call */
class PCanApi
{
    private:
        /** @brief Private class constructor */
        explicit PCanApi();

    public:
        /** @brief Intialize a CAN channel
            @param config The config to initialize the CAN
            @return True if no problem occurred */
        static bool initializeCan(const CanDeviceConfig &config);

        /** @brief Intialize a CAN FD channel
            @param config The config to initialize the CAN FD
            @return True if no problem occurred */
        static bool initializeCanFd(const CanDeviceConfig &config);

        /** @brief Unitialize the CAN channel
            @param pCanBusItf The CAN Bus interface key
            @return True if no problem occurred */
        static bool unInitializeCan(PCanBusItf::Enum pCanBusItf);

        /** @brief Get the error text
            @param errorCode The error code to get its text
            @return The string representation of the error */
        static QString getErrorText(quint32 errorCode);

        /** @brief Write a CAN FD message through the CAN BUB targetted by the @ref pCanBusItf
                   given
            @param pCanBusItf The CAN Bus interface key
            @param frame The frame to write
            @return True if no problem occurred */
        static bool writeCanFdMsgProcess(PCanBusItf::Enum pCanBusItf, const QCanBusFrame &frame);

        /** @brief Write a CAN message through the CAN BUB targetted by the @ref pCanBusItf
                   given
            @param pCanBusItf The CAN Bus interface key
            @param frame The frame to write
            @return True if no problem occurred */
        static bool writeCanMsgProcess(PCanBusItf::Enum pCanBusItf, const QCanBusFrame &frame);

        /** @brief Get the list of currently available devices
            @return The info list of the available devices */
        static QVector<CanDeviceInfo> getAvailableDevices();

    public:
        /** @brief Get the value of the BUS Off auto reset parameter
            @note From doc: This parameter instructs the PCAN driver to automatically reset the CAN
                            controller of a PCAN Channel when a bus-off state is detected.
            @param pCanBusItf The CAN Bus interface key
            @param autoReset The auto reset method parameter to set with value got from PCAN
            @return True if no problem occurred */
        static bool getParamBusOffAutoReset(PCanBusItf::Enum pCanBusItf, bool &autoReset);

        /** @brief Set the value of the BUS Off auto reset parameter
            @note From doc: This parameter instructs the PCAN driver to automatically reset the CAN
                            controller of a PCAN Channel when a bus-off state is detected.
            @param pCanBusItf The CAN Bus interface key
            @param autoReset The BUS Off auto reset parameter value to set in PCAN
            @return True if no problem occurred */
        static bool setParamBusOffAutoReset(PCanBusItf::Enum pCanBusItf, bool autoReset);

    private:
        /** @brief Get the value of the targetted PCAN parameter
            @param pCanBusItf The CAN Bus interface key
            @param paramType The type of the PCAN parameter, @ref TPCANParameter
            @param value The value got from PCAN lib of the parameter
            @return True if no problem occurred */
        static bool getBooleanParam(PCanBusItf::Enum pCanBusItf, quint8 paramType, bool &value);

        /** @brief Set the value of the targetted PCAN parameter
            @param pCanBusItf The CAN Bus interface key
            @param paramType The type of the PCAN parameter, @ref TPCANParameter
            @param value The value to set to the PCAN lib parameter
            @return True if no problem occurred */
        static bool setBooleanParam(PCanBusItf::Enum pCanBusItf, quint8 paramType, bool value);

    private:
        /** @brief The string pattern to complete in order to send the Bitrate FD to the lib */
        static const constexpr char* BitrateFd = "f_clock_mhz=%1, "
                                                 "nom_brp=%2, nom_tseg1=%3, "
                                                 "nom_tseg2=%4, nom_sjw=%5, "
                                                 "data_brp=%6, data_tseg1=%7, "
                                                 "data_tseg2=%8, data_sjw=%9";

        /** @brief The english language code used to get the error text */
        static const constexpr quint16 EnLanguageCode = 0x09;
};
