// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "canmanager.hpp"

#include <QCanBus>
#include <QCanBusFrame>
#include <QDebug>
#include <QMetaType>
#include <QVector>

#include "threadutility/concurrent/threadconcurrentrun.hpp"

#include "src/candevice/candeviceintf.hpp"
#include "src/models/candeviceconfig.hpp"
#include "src/pcanapi/pcanapi.hpp"

CanManager* CanManager::_instance = nullptr;


CanManager::CanManager(QObject *parent)
    : QObject{parent}
{
}

CanManager::~CanManager()
{
}

QSharedPointer<CanDeviceIntf> CanManager::getCanIntf(PCanBusItf::Enum canIntfKey)
{
    return ThreadConcurrentRun::run(*this, &CanManager::getCanIntfPriv, canIntfKey);
}

QSharedPointer<CanDeviceIntf> CanManager::createOrGetCanIntf(const CanDeviceConfig &config)
{
    return ThreadConcurrentRun::run(*this, &CanManager::createOrGetCanIntfPriv, config);
}

QSharedPointer<CanDeviceIntf> CanManager::getCanIntfPriv(PCanBusItf::Enum canIntfKey)
{
    return getHandler(canIntfKey);
}

QSharedPointer<CanDeviceIntf> CanManager::createOrGetCanIntfPriv(const CanDeviceConfig &config)
{
    const PCanBusItf::Enum canIntfKey = config.getCanBusItf();

    auto createCanIntf = [&config](const PCanBusItf::Enum &key)
    {
        CanDeviceIntf *canIntf = new CanDeviceIntf(config);

        if(!canIntf->initDevice())
        {
            qWarning() << "A problem occurred when tried to initialize the can interface: "
                       << PCanBusItf::toString(key);
            delete canIntf;
            // We don't directly return nullptr here, because the linter doesn't understand that
            // the return type is CanDeviceIntf * and not nullptr
            canIntf = nullptr;
        }

        return QSharedPointer<CanDeviceIntf>(canIntf, &QObject::deleteLater);
    };

    return createOrGetHandler(canIntfKey, createCanIntf);
}

CanDeviceInfo CanManager::findBusDeviceInfo(PCanBusItf::Enum canIntfKey,
                                            const QString &deviceName,
                                            const bool *isCanFd)
{
    QVector<CanDeviceInfo> devices = PCanApi::getAvailableDevices();

    for(auto citer = devices.cbegin(); citer != devices.cend(); ++citer)
    {
        if(((canIntfKey == PCanBusItf::Unknown) || citer->getPCanBusIntf() == canIntfKey) &&
            (deviceName.isNull()                || citer->getDeviceName().contains(deviceName)) &&
            ((isCanFd == nullptr)               || (citer->isCanFd() == *isCanFd))) {
            return CanDeviceInfo(*citer);
        }
    }

    qDebug() << "No CAN bus device found matching the given criteria";
    return CanDeviceInfo();
}

QVector<CanDeviceInfo> CanManager::getAvailableDevices()
{
    return PCanApi::getAvailableDevices();
}

CanManager &CanManager::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new CanManager();
        registerMetaTypes();
    }

    return *_instance;
}

void CanManager::registerMetaTypes()
{
    qRegisterMetaType<CanDeviceConfig>();
    qRegisterMetaType<QVector<QCanBusFrame>>("QVector<QCanBusFrame>");
}
