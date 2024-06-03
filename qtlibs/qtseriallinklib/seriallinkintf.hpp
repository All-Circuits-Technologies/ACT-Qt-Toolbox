// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "definesseriallink.hpp"

#include <QSerialPort>

class SerialLinkThread;


/** @brief Interface to communicate with the serial link contains in the worker thread
    @attention Some methods (those which are communicating with the serial port) are called in
               another thread. It means that the event loop of the caller thread is processing
               while the methods are called. */
class SERIALLINK_EXPORT SerialLinkIntf : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param interfaceName The interface name
            @param parent The parent class */
        explicit SerialLinkIntf(const QString &interfaceName, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~SerialLinkIntf() override;

    public:
        /** @brief Init the serial link
            @param serialPortInfo Information of the serial port
            @return True if no problem occurred */
        bool initSerialLink(const QSerialPortInfo &serialPortInfo);

        /** @brief Get interface name */
        const QString &getIntfName() const { return _interfaceName; }

    public slots:
        /** @brief Send data to serial port (asynchronously)
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param data Data to send
            @param forceFlush If true force the immediate flush of data into the serial link
            @return False upon error */
        bool send(const QByteArray &data, bool forceFlush = false);

        /** @brief Trash RX buffer (asynchronously)
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called. */
        void flushRx();

    public:
        /** @brief This method calls the @ref QSerialPort:open
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @param mode The open mode type
            @return True if no problem occurred */
        bool open(QIODevice::OpenMode mode);

        /** @brief This method calls the @ref QSerialPort:isOpen
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @return True if the serial port is open */
        bool isOpen() const;

        /** @brief This method calls the @ref QSerialPort:close
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called. */
        void close();

        /** @brief This method calls the @ref QSerialPort:setFlowControl
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @param flowControl The flow control to choose for the port
            @return True if no problem occurred */
        bool setFlowControl(QSerialPort::FlowControl flowControl);

        /** @brief This method calls the @ref QSerialPort:setBaudRate
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @param baudRate The baudrate of the serial port to set
            @param directions The directions to set the baudrate
            @return True if no problem occurred */
        bool setBaudRate(qint32 baudRate,
                         QSerialPort::Directions directions = QSerialPort::AllDirections);

    signals:
         /** @brief Signal fired whenever data is received from serial port
             @param data Received data chunk
             @note This signal only fires when serial port is opened. */
         void dataReceived(const QByteArray &data);

    private:
         SerialLinkThread *_serialLinkThread{nullptr};
         QString _interfaceName;
};
