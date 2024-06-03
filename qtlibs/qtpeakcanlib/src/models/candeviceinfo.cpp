// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "candeviceinfo.hpp"


CanDeviceInfo::CanDeviceInfo(PCanBusItf::Enum pCanBusIntf,
                             const QString &deviceName,
                             bool isCanFd,
                             QObject *parent)
    : QObject{parent},
    _pCanBusIntf{pCanBusIntf},
    _deviceName(deviceName),
    _isCanFd(isCanFd)
{
}

CanDeviceInfo::CanDeviceInfo(QObject *parent)
    : QObject{parent}
{
}

CanDeviceInfo::CanDeviceInfo(const CanDeviceInfo &otherElement)
{
    _pCanBusIntf    = otherElement._pCanBusIntf;
    _deviceName     = otherElement._deviceName;
    _isCanFd        = otherElement._isCanFd;
}

CanDeviceInfo::~CanDeviceInfo()
{
}

CanDeviceInfo &CanDeviceInfo::operator=(const CanDeviceInfo &otherElement)
{
    _pCanBusIntf    = otherElement._pCanBusIntf;
    _deviceName     = otherElement._deviceName;
    _isCanFd        = otherElement._isCanFd;

    return *this;
}
