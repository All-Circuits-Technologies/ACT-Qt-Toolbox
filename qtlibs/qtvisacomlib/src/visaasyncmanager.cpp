// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "visaasyncmanager.hpp"

#include <QTimer>

#include "avisacom.hpp"

VisaAsyncManager::VisaAsyncManager(QObject *parent) : QObject(parent)
{ 
}

bool VisaAsyncManager::asyncWrite(const QByteArray &cmd)
{
    if(cmd.isEmpty())
    {
        qWarning() << "There is nothing to write to VisaCom, continue";
        return false;
    }

    if(!_avisacom->isOpen())
    {
        qInfo() << "The instrument session: " << _avisacom->getInterfaceId()
                << ", wan't open, try to open it";
    }

    // Before writing cmd, clears the buffer
    _avisacom->clear();

    qDebug() << "Write to visa cmd: " << cmd;

    if(!_avisacom->write(cmd))
    {
        qWarning() << "Failed to write cmd: " << cmd << ", to the device";
        return false;
    }

    return true;
}

bool VisaAsyncManager::asyncQuery(const QByteArray &cmd)
{
    if(!asyncWrite(cmd))
    {
        qWarning() << "An error occured during asynchrounous write with cmd: " << cmd;
        return false;
    }

    QTimer::singleShot(0, this, &VisaAsyncManager::waitResult);

    return true;
}

bool VisaAsyncManager::asyncReadPriv(QByteArray &output)
{
    if(!_avisacom->read(output))
    {
        qWarning() << "No data has been received after the cmd has been written, to interface id: "
                   << _avisacom->getInterfaceId();

        return false;
    }

    return true;
}

void VisaAsyncManager::waitResult()
{
    QByteArray output;

    if(!asyncReadPriv(output))
    {
        emit errorOccured();
    }
    else
    {
        emit messageReceived(output);
    }
}
