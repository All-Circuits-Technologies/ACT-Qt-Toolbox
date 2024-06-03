// SPDX-FileCopyrightText: 2019 - 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "seriallink.hpp"

#include <QByteArray>
#include <QDebug>
#include <QSerialPortInfo>

#include "seriallibconstants.hpp"


SerialLink::SerialLink(const QSerialPortInfo &portInfo, QObject *parent) :
    QObject(parent),
    _serial(QSerialPort(portInfo))
{
    connect(&_serial, &QSerialPort::readyRead, this, &SerialLink::onReadyRead);

    connect(&_serial,
            &QSerialPort::errorOccurred,
            this,
            [](QSerialPort::SerialPortError serialPortError) { qDebug() << serialPortError; });
}

SerialLink::~SerialLink()
{
}

const QSerialPort &SerialLink::getSerialPort() const
{
    return _serial;
}

QSerialPort &SerialLink::accessSerialPort()
{
    return _serial;
}

bool SerialLink::send(const QByteArray &data, bool forceFlush)
{
    if(Q_UNLIKELY(SerialLibConstants::Debug::Stream))
    {
        qDebug() << _serial.portName() << " <<< " << data;
    }

    const qint64 writtenBytes = _serial.write(data.constData());

    if(Q_UNLIKELY(writtenBytes == -1))
    {
        qWarning() << "An error occurred with the serial port: " << _serial.portName() << ", when "
                   << "trying to write data. The error is: " << _serial.errorString();
        return false;
    }

    if(Q_UNLIKELY(writtenBytes != data.length()))
    {
        qWarning() << _serial.portName() << "Write fail:" << writtenBytes << "bytes written out of"
                   << data.length();
        return false;
    }

    if(forceFlush && Q_UNLIKELY(!_serial.flush()))
    {
        qWarning() << "Flush failed, when tried to send data on the serial port: "
                   << _serial.portName();
        return false;
    }

    return true;
}

void SerialLink::flushRx()
{
    const QByteArray rxData = _serial.readAll();

    if(Q_UNLIKELY(SerialLibConstants::Debug::Stream))
    {
        qDebug() << _serial.portName() << " Flushed RX: " << rxData;
    }
}

void SerialLink::onReadyRead()
{
    const QByteArray data = _serial.readAll();

    if(!data.isEmpty())
    {
        if(Q_UNLIKELY(SerialLibConstants::Debug::Stream))
        {
            qDebug() << _serial.portName() << " >>> " << data;
        }
        emit dataReceived(data);
    }
}
