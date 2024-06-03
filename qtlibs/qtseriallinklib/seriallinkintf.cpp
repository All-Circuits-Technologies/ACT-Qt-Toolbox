// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "seriallinkintf.hpp"

#include "definesutility/definesutility.hpp"
#include "threadutility/concurrent/threadconcurrentrun.hpp"

#include "seriallink.hpp"
#include "seriallinkthread.hpp"

#include <QDebug>


SerialLinkIntf::SerialLinkIntf(const QString &interfaceName, QObject *parent)
    : QObject{parent},
      _serialLinkThread(new SerialLinkThread()),
      _interfaceName(interfaceName)
{
}

SerialLinkIntf::~SerialLinkIntf()
{
    _serialLinkThread->stopAndDeleteThread();
}

bool SerialLinkIntf::initSerialLink(const QSerialPortInfo &serialPortInfo)
{
    RETURN_IF_FALSE(_serialLinkThread->initSerialLink(serialPortInfo));

    connect(_serialLinkThread->accessSerialLink(),  &SerialLink::dataReceived,
            this,                                   &SerialLinkIntf::dataReceived);

    return true;
}

bool SerialLinkIntf::send(const QByteArray &data, bool forceFlush)
{
    if(!_serialLinkThread->isValid())
    {
        qWarning() << "Can't send data, the serial link thread: " << _interfaceName
                   << ", isn't valid, may be the thread hasn't be initialized or it's stopped";
        return false;
    }

    return ThreadConcurrentRun::run(*_serialLinkThread->accessSerialLink(),
                                    &SerialLink::send,
                                    data,
                                    forceFlush);

}

void SerialLinkIntf::flushRx()
{
    if(!_serialLinkThread->isValid())
    {
        qWarning() << "Can't flush RX, the serial link thread: " << _interfaceName
                   << " isn't valid, may be the thread hasn't be initialized or it's stopped";
        return;
    }

    ThreadConcurrentRun::run(*_serialLinkThread->accessSerialLink(), &SerialLink::flushRx);
}

bool SerialLinkIntf::open(QIODevice::OpenMode mode)
{
    if(!_serialLinkThread->isValid())
    {
        qWarning() << "Can't open the serial port, the serial link thread: " << _interfaceName
                   << " isn't valid, may be the thread hasn't be initialized or it's stopped";
        return false;
    }

    return ThreadConcurrentRun::run(_serialLinkThread->accessSerialLink()->accessSerialPort(),
                                    &QSerialPort::open,
                                    mode);
}

bool SerialLinkIntf::isOpen() const
{
    if(!_serialLinkThread->isValid())
    {
        qWarning() << "Can't test if serial port is open, the serial link: " << _interfaceName
                   << " thread isn't valid, may be the thread hasn't be initialized or it's stopped";
        return false;
    }

    return ThreadConcurrentRun::run(_serialLinkThread->accessSerialLink()->accessSerialPort(),
                                    &QSerialPort::isOpen);
}

void SerialLinkIntf::close()
{
    if(!_serialLinkThread->isValid())
    {
        qWarning() << "Can't close the serial port, the serial link thread: " << _interfaceName
                   << " isn't valid, may be the thread hasn't be initialized or it's stopped";
        return;
    }

    ThreadConcurrentRun::run(_serialLinkThread->accessSerialLink()->accessSerialPort(),
                             &QSerialPort::close);
}

bool SerialLinkIntf::setFlowControl(QSerialPort::FlowControl flowControl)
{
    if(!_serialLinkThread->isValid())
    {
        qWarning() << "Can't set the flow control of the serial port, the serial link thread: "
                   << _interfaceName << " isn't valid, may be the thread hasn't be initialized or "
                   << "it's stopped";
        return false;
    }

    return ThreadConcurrentRun::run(_serialLinkThread->accessSerialLink()->accessSerialPort(),
                                    &QSerialPort::setFlowControl,
                                    flowControl);
}

bool SerialLinkIntf::setBaudRate(qint32 baudRate, QSerialPort::Directions directions)
{
    if(!_serialLinkThread->isValid())
    {
        qWarning() << "Can't set the baudrate of the serial port, the serial link thread: "
                   << _interfaceName << " isn't valid, may be the thread hasn't be initialized or "
                   << "it's stopped";
        return false;
    }

    return ThreadConcurrentRun::run(_serialLinkThread->accessSerialLink()->accessSerialPort(),
                                    &QSerialPort::setBaudRate,
                                    baudRate,
                                    directions);
}
