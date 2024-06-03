// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "visaasyncthread.hpp"

#include <QTimer>

#include "threadrunninghelper.hpp"
#include "concurrent/threadconcurrentrun.hpp"

#include "visaasyncmanager.hpp"
#include "avisacom.hpp"


VisaAsyncThread::VisaAsyncThread(QObject *parent) :
    QThread(parent),
    _threadRunningHelper(new ThreadRunningHelper(this))
{
    //Start the thread
    start();
}

VisaAsyncThread::~VisaAsyncThread()
{
    stopThread();
}

bool VisaAsyncThread::stopThread()
{
    if(isFinished())
    {
        // Thread already finished
        return true;
    }

    connect(_visaAsyncManager, &VisaAsyncManager::destroyed, this, &VisaAsyncThread::quit);
    QTimer::singleShot(0, _visaAsyncManager, &VisaAsyncManager::deleteLater);
    return true;
}

bool VisaAsyncThread::stopAndDeleteThread()
{
    connect(this, &VisaAsyncThread::finished,
            this, &VisaAsyncThread::deleteLater, Qt::UniqueConnection);

    if(isFinished())
    {
        // The thread is already finished; therefore it's useless to wait for finished signal
        deleteLater();
        return true;
    }

    return stopThread();
}

bool VisaAsyncThread::asyncWrite(const QByteArray &cmd)
{
    if(!_threadRunningHelper->waitForThread())
    {
        qWarning() << "A problem occured when waiting for the thread to start";
        return false;
    }

    return ThreadConcurrentRun::run(*_visaAsyncManager, &VisaAsyncManager::asyncWrite, cmd);
}

bool VisaAsyncThread::asyncQuery(const QByteArray &cmd)
{
    if(!_threadRunningHelper->waitForThread())
    {
        qWarning() << "A problem occured when waiting for the thread to start";
        return false;
    }

    return ThreadConcurrentRun::run(*_visaAsyncManager, &VisaAsyncManager::asyncQuery, cmd);
}

void VisaAsyncThread::run()
{
    _visaAsyncManager = new VisaAsyncManager();
    _visaAsyncManager->setVisacom(_avisacom);

    connect(_visaAsyncManager, &VisaAsyncManager::messageReceived,
            this,             &VisaAsyncThread::messageReceived);
    connect(_visaAsyncManager, &VisaAsyncManager::errorOccured,
            this,             &VisaAsyncThread::errorOccured);

    QTimer::singleShot(0, _threadRunningHelper, &ThreadRunningHelper::onThreadReady);
    exec();
}
