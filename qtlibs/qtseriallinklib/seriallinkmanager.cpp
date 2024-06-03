// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "seriallinkmanager.hpp"

#include <QDebug>
#include <QMutex>
#include <QSerialPortInfo>

#include "threadutility/concurrent/threadconcurrentrun.hpp"

#include "seriallinkintf.hpp"

SerialLinkManager* SerialLinkManager::_instance = nullptr;


SerialLinkManager::SerialLinkManager(QObject *parent)
    : QObject{parent}
{
}

SerialLinkManager::~SerialLinkManager()
{
}

SerialLinkManager &SerialLinkManager::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new SerialLinkManager();
    }

    return *_instance;
}

QSharedPointer<SerialLinkIntf> SerialLinkManager::getSerialLink(const QString &interfaceName)
{
    return ThreadConcurrentRun::run(*this, &SerialLinkManager::getSerialLinkPriv, interfaceName);
}

QSharedPointer<SerialLinkIntf> SerialLinkManager::createOrGetSerialLink(
                                                                    const QSerialPortInfo &portInfo)
{
    return ThreadConcurrentRun::run(*this, &SerialLinkManager::createOrGetSerialLinkPriv, portInfo);
}

QSharedPointer<SerialLinkIntf> SerialLinkManager::getSerialLinkPriv(const QString &interfaceName)
{
    return getHandler(interfaceName);
}

QSharedPointer<SerialLinkIntf> SerialLinkManager::createOrGetSerialLinkPriv(
    const QSerialPortInfo &portInfo)
{
    const QString interfaceName = portInfo.portName();

    auto createCanIntf = [&portInfo](const QString &key)
    {
        SerialLinkIntf *serialLinkIntf = new SerialLinkIntf(key);

        if(!serialLinkIntf->initSerialLink(portInfo))
        {
            qWarning() << "A problem occurred when tried to initialize the serial link: " << key;
            delete serialLinkIntf;
            // We don't directly return nullptr here, because the linter doesn't understand that
            // the return type is CanDeviceIntf * and not nullptr
            serialLinkIntf = nullptr;
        }

        return QSharedPointer<SerialLinkIntf>(serialLinkIntf, &QObject::deleteLater);
    };

    return createOrGetHandler(interfaceName, createCanIntf);
}

QVector<QSerialPortInfo> SerialLinkManager::findAllSerialPort(const QString &portName,
                                                              const QString &serialNumber,
                                                              quint16 usbVID,
                                                              quint16 usbPID,
                                                              const QString &description,
                                                              const QString &manufacturer,
                                                              const QString &location)
{
    QVector<QSerialPortInfo> portsFound;
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    for(auto citer = availablePorts.cbegin(); citer != availablePorts.cend(); ++citer)
    {
        if((portName.isNull()     || citer->portName().contains(portName)) &&
           (serialNumber.isNull() || citer->serialNumber().contains(serialNumber)) &&
           ((usbVID == 0x0000)    || (citer->vendorIdentifier() == usbVID)) &&
           ((usbPID == 0x0000)    || (citer->productIdentifier() == usbPID)) &&
           (description.isNull()  || citer->description().contains(description)) &&
           (manufacturer.isNull() || citer->manufacturer().contains(manufacturer)) &&
           (location.isNull()     || citer->systemLocation().contains(location)))
        {
            portsFound.append(*citer);
        }
    }

    return portsFound;
}
