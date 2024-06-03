// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "candeviceconfigdetails.hpp"

#include <QDebug>

#include "definesutility/definesutility.hpp"


CanDeviceConfigDetails::CanDeviceConfigDetails(QObject *parent)
    : QObject{parent}
{
}

CanDeviceConfigDetails::CanDeviceConfigDetails(const CanDeviceConfigDetails &copy) :
    _baudRate{copy._baudRate}
{
}

CanDeviceConfigDetails::~CanDeviceConfigDetails()
{
}

bool CanDeviceConfigDetails::isValid() const
{
    return _baudRate != PCanBaudRate::Unknown;
}

CanDeviceConfigDetails &CanDeviceConfigDetails::operator=(const CanDeviceConfigDetails &otherConfig)
{
    _baudRate = otherConfig._baudRate;

    return *this;
}
