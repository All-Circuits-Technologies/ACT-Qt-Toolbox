// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "candevicethread.hpp"

#include <QDebug>
#include <QTimer>

#include "definesutility/definesutility.hpp"

#include "src/candevice/candevice.hpp"


CanDeviceThread::CanDeviceThread(QObject *parent)
    : BaseThread{parent}
{
}

CanDeviceThread::~CanDeviceThread()
{
}

bool CanDeviceThread::createCanDevice(const CanDeviceConfig &config)
{
    if(_valid)
    {
        qInfo() << "The can device has already been initialized, do nothing";
        return true;
    }

    if(_canConfig.isValid())
    {
        qWarning() << "The CAN device thread has already been initialized";
        return false;
    }

    _canConfig = config;

    RETURN_IF_FALSE(startThreadAndWaitToBeReady());

    _valid = true;

    return true;
}

bool CanDeviceThread::stopThread()
{
    _valid = false;

    if(_canDevice != nullptr)
    {
        QTimer::singleShot(0, _canDevice, &CanDevice::deleteLater);
        _canDevice = nullptr;
    }

    return BaseThread::stopThread();
}

void CanDeviceThread::run()
{
    _canDevice = new CanDevice(_canConfig);

    BaseThread::run();
}
