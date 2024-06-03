// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "seriallinkthread.hpp"

#include "seriallink.hpp"

#include "definesutility/definesutility.hpp"

#include <QDebug>
#include <QSerialPortInfo>
#include <QTimer>


SerialLinkThread::SerialLinkThread(QObject *parent)
    : BaseThread{parent}
{
}

SerialLinkThread::~SerialLinkThread()
{
}

bool SerialLinkThread::initSerialLink(const QSerialPortInfo &serialPortInfo)
{
    if(_valid)
    {
        qInfo() << "The serial link has already been initialized, do nothing";
        return true;
    }

    if(!_serialPortInfo.isNull())
    {
        qWarning() << "The serial link thread has already been initialized";
        return false;
    }

    _serialPortInfo = serialPortInfo;

    RETURN_IF_FALSE(startThreadAndWaitToBeReady());

    _valid = true;

    return true;
}

bool SerialLinkThread::stopThread()
{
    _valid = false;

    if(_serialLink != nullptr)
    {
        QTimer::singleShot(0, _serialLink, &SerialLink::deleteLater);
        _serialLink = nullptr;
    }

    return BaseThread::stopThread();
}

void SerialLinkThread::run()
{
    _serialLink = new SerialLink(_serialPortInfo);

    BaseThread::run();
}
