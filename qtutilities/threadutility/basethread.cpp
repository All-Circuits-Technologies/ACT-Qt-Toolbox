// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "basethread.hpp"

#include "threadrunninghelper.hpp"

#include <QTimer>


BaseThread::BaseThread(QObject *parent) :
    QThread(parent),
    _threadRunningHelper(new ThreadRunningHelper(this))
{
}

BaseThread::~BaseThread()
{
    stopThread();
}

bool BaseThread::startThreadAndWaitToBeReady()
{
    start();
    return _threadRunningHelper->waitForThread();
}

bool BaseThread::stopThread()
{
    QTimer::singleShot(0, this, &BaseThread::quit);
    return true;
}

bool BaseThread::stopAndDeleteThread()
{
    connect(this, &BaseThread::finished,
            this, &BaseThread::deleteLater, Qt::UniqueConnection);

    if(isFinished())
    {
        // The thread is already finished; therefore it's useless to wait for finished signal
        deleteLater();
        return true;
    }

    return stopThread();
}

bool BaseThread::waitForThread() const
{
    return _threadRunningHelper->waitForThread();
}

void BaseThread::run()
{
    QTimer::singleShot(0, this, &BaseThread::onThreadReady);
    exec();
}

void BaseThread::onThreadReady()
{
    _threadRunningHelper->onThreadReady();
    emit ready();
}
