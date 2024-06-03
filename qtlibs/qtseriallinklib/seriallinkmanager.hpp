// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include "handlerutility/handlerclassmembersmixin.hpp"

#include "definesseriallink.hpp"

class QMutex;
class QSerialPortInfo;
class SerialLinkIntf;


/** @brief This class manages the serial link creation and getting */
class SERIALLINK_EXPORT SerialLinkManager : public QObject,
                                                   HandlerClassMembersMixin<QString, SerialLinkIntf>

{
    Q_OBJECT

    private:
        /** @brief Private class constructor
            @param parent The parent instance */
        explicit SerialLinkManager(QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~SerialLinkManager() override;

    public:
        /** @brief Return the singleton instance of the Visacom manager
            @note If no instance created, getInsantce force Visacom manager instance creation
            @return The Visacom manager instance */
        static SerialLinkManager &getInstance();

    public:
        /** @brief Get a serial link thanks to its interface name
            @note The interface has to be created before to be got by this method
            @note This method is thread safe but required an event loop in the caller method
            @param interfaceName The interface name is the name stored in the @ref SerialLinkIntf
                                 class
            @return Get the serial link found or a nullptr */
        QSharedPointer<SerialLinkIntf> getSerialLink(const QString &interfaceName);

        /** @brief Create or get a serial link thanks to the serial port info given
            @note If the interface already exists, it returns the serial port interface
            @note This method is thread safe but required an event loop in the caller method
            @param portInfo The interface port info to create the serial link interface
            @return The serial port created or a nullptr */
        QSharedPointer<SerialLinkIntf> createOrGetSerialLink(const QSerialPortInfo& portInfo);

    private:
        /** @brief Get a serial link thanks to its interface name
            @note The interface has to be created before to be got by this method
            @param interfaceName The interface name is the name stored in the @ref SerialLinkIntf
                                 class
            @return Get the serial link found or a nullptr */
        QSharedPointer<SerialLinkIntf> getSerialLinkPriv(const QString &interfaceName);

        /** @brief Create or get a serial link thanks to the serial port info given
            @note If the interface already exists, it returns the serial port interface
            @param portInfo The interface port info to create the serial link interface
            @return The serial port created or a nullptr */
        QSharedPointer<SerialLinkIntf> createOrGetSerialLinkPriv(const QSerialPortInfo& portInfo);

    public:
        /** @brief Find all the serial ports matching all non-empty rules
            @note Given strings are case sensitive but can be a subset of serial port contents.
                  Example a rule manufatcurer="APS" will be OK for a real manufacturer "APS Corp".
            @param portName Serial port name (ex: "COM1")
            @param serialNumber USB serial number
            @param usbVID USB Vendor ID (set it to 0x0000 to ignore this rule)
            @param usbPID USB product ID (set it to 0x0000 to ignore this rule)
            @param description USB description
            @param manufacturer USB manufacturer
            @param location System location (ex: "1.1.2")
            @return All matching serial ports */
        static QVector<QSerialPortInfo> findAllSerialPort(const QString &portName,
                                                          const QString &serialNumber = {},
                                                          quint16 usbVID = 0x0000,
                                                          quint16 usbPID = 0x0000,
                                                          const QString &description = {},
                                                          const QString &manufacturer = {},
                                                          const QString &location = {});

    private:
        static SerialLinkManager *_instance;
};
