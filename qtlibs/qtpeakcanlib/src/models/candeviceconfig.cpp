// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "candeviceconfig.hpp"

#include "src/models/candeviceconfigdetails.hpp"
#include "src/models/candevicefdconfigdetails.hpp"


CanDeviceConfig::CanDeviceConfig(QObject *parent)
    : QObject{parent}
{
}

CanDeviceConfig::CanDeviceConfig(bool isCanFd, QObject *parent)
    : QObject{parent}
{
    if(isCanFd)
    {
        _canFdConfig = new CanDeviceFdConfigDetails();
    }
    else
    {
        _canConfig = new CanDeviceConfigDetails();
    }
}

CanDeviceConfig::CanDeviceConfig(const CanDeviceConfig &copy) :
    _canBusItf{copy._canBusItf},
    _canConfig{nullptr},
    _canFdConfig{nullptr}
{
    if(copy._canConfig != nullptr)
    {
        _canConfig = new CanDeviceConfigDetails(*copy._canConfig);
    }

    if(copy._canFdConfig != nullptr)
    {
        _canFdConfig = new CanDeviceFdConfigDetails(*copy._canFdConfig);
    }
}

CanDeviceConfig::~CanDeviceConfig()
{
    delete _canConfig;
    delete _canFdConfig;
}

quint16 CanDeviceConfig::getPCanBusItfHandle() const
{
    return PCanBusItf::toTPCanHandle(_canBusItf);
}

QString CanDeviceConfig::getCanBusItfName() const
{
    return PCanBusItf::toString(_canBusItf);
}

void CanDeviceConfig::setCanFd(bool isCanFd)
{
    if(isCanFd && _canFdConfig == nullptr)
    {
        delete _canConfig;
        _canConfig = nullptr;

        _canFdConfig = new CanDeviceFdConfigDetails();
    }
    else if(!isCanFd && _canConfig == nullptr)
    {
        delete _canFdConfig;
        _canFdConfig = nullptr;

        _canConfig = new CanDeviceConfigDetails();
    }
}

bool CanDeviceConfig::isValid() const
{
    return (_canBusItf != PCanBusItf::Unknown) &&
           (_canConfig != nullptr || _canFdConfig != nullptr) &&
           (_canConfig == nullptr || _canConfig->isValid()) &&
           (_canFdConfig == nullptr || _canFdConfig->isValid());
}

CanDeviceConfig &CanDeviceConfig::operator=(const CanDeviceConfig &otherConfig)
{
    _canBusItf  = otherConfig._canBusItf;

    delete _canConfig;
    if(otherConfig._canConfig != nullptr)
    {
        _canConfig = new CanDeviceConfigDetails(*otherConfig._canConfig);
    }
    else
    {
        _canConfig = nullptr;
    }

    delete _canFdConfig;
    if(otherConfig._canFdConfig != nullptr)
    {
        _canFdConfig = new CanDeviceFdConfigDetails(*otherConfig._canFdConfig);
    }
    else
    {
        _canFdConfig = nullptr;
    }

    return *this;
}
