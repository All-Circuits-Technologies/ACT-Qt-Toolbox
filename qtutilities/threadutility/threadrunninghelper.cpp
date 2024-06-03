// SPDX-FileCopyrightText: 2018 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "threadrunninghelper.hpp"

#include <QDebug>

#include "waitutility/waithelper.hpp"


ThreadRunningHelper::ThreadRunningHelper(QObject *parent) :
    QObject(parent),
    _running(false)
{
    _mutex.lock();
}

bool ThreadRunningHelper::waitForThread()
{
    if(!_running)
    {
        auto mutexTest = [this]()
        {
            bool locked = false;

            if(_mutex.tryLock())
            {
                locked = true;
                _mutex.unlock();
            }

            return locked;
        };

        if(!WaitHelper::pseudoWait(mutexTest, timeoutInMs))
        {
            qWarning() << "A problem occurred, thread isn't ready ; timeout raised.";
            return false;
        }
    }

    return true;
}

void ThreadRunningHelper::onThreadReady()
{
    _mutex.unlock();
    _running = true;
}
