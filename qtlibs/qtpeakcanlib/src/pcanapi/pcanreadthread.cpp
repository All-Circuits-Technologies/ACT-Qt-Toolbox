// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pcanreadthread.hpp"

#include <QDebug>
#include <QTimer>

#include "src/pcanapi/pcanreader.hpp"


PCanReadThread::PCanReadThread(PCanBusItf::Enum canBusItf, bool isCanFd, QObject *parent)
    : BaseThread{parent},
    _canBusItf{canBusItf},
    _isCanFd(isCanFd)
{
}

PCanReadThread::~PCanReadThread()
{
}

bool PCanReadThread::stopThread()
{
    if(_reader != nullptr)
    {
        // We cancel the reading and wait for the process to be ended
        // This is necessary before trying to uninitialize CAN
        _reader->cancelReading();
        _reader->waitForProcessEnd();
        qDebug() << "After waiting process";

        QTimer::singleShot(0, _reader, &PCanReader::deleteLater);
        _reader = nullptr;
    }

    return BaseThread::stopThread();
}

void PCanReadThread::run()
{
    _reader = new PCanReader(_canBusItf, _isCanFd);

    connect(_reader,    &PCanReader::framesReceived,
            this,       &PCanReadThread::framesReceived);
    connect(this,    &PCanReadThread::ready,
            _reader, &PCanReader::readMessages, Qt::QueuedConnection);

    BaseThread::run();
}
