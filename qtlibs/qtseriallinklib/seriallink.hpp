// SPDX-FileCopyrightText: 2019 - 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QSerialPort>

class QSerialPortInfo;


/** @brief This class holds a single serial line with a few dedicated helpers */
class SerialLink : public QObject
{
    Q_OBJECT

    public:
        /** @brief Create an instance
            @param portInfo Details of serial port to wrap
            @param parent Optional Qt parentship */
        explicit SerialLink(const QSerialPortInfo &portInfo, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~SerialLink() override;

    public:
        /** @brief Serial port getter
            @return wrapped serial port */
        const QSerialPort &getSerialPort() const;

        /** @brief Serial port accessor
            @return wrapped serial port */
        QSerialPort &accessSerialPort();

    public slots:
        /** @brief Send data to serial port (asynchronously)
            @note This method ensure thread uncoupling but requires an event loop
            @param data Data to send
            @param forceFlush If true force the immediate flush of data into the serial link
            @return False upon error */
        bool send(const QByteArray &data, bool forceFlush = false);

        /** @brief Trash RX buffer (asynchronously) */
        void flushRx();

    private slots:
        /** @brief React on serial port ready-read event */
        void onReadyRead();

    signals:
        /** @brief Signal fired whenever data is received from serial port
            @param data Received data chunk
            @note This signal only fires when serial port is opened. */
        void dataReceived(const QByteArray &data);

    private:
        /** @brief Encapsulated serial port */
        QSerialPort _serial;
};
