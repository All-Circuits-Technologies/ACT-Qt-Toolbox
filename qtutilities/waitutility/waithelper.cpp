// SPDX-FileCopyrightText: 2018 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "waithelper.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QEventLoop>
#include <QThread>
#include <QTimer>

#include "definesutility/definesutility.hpp"


bool WaitHelper::pseudoWait(const bool &taskDone, int timeToWaitInMs, WaitMethod waitMethod)
{
    return pseudoWait([&taskDone](){ return taskDone; }, timeToWaitInMs, waitMethod);
}

bool WaitHelper::pseudoWait(const std::function<bool ()> &testFunc,
                            int timeToWaitInMs,
                            WaitMethod waitMethod)
{
    if(Q_UNLIKELY(timeToWaitInMs < -1))
    {
        qWarning() << "Illegal wait duration" << timeToWaitInMs << " leading to no-wait at all.";
    }

    bool success = false;

    switch(waitMethod)
    {
        case WaitMethod::UseLocalEventLoop:
            success = eventLoopPseudoWait(&testFunc, timeToWaitInMs);
            break;

        case WaitMethod::UseThreadSleep:
            success = sleepPseudoWait(&testFunc, timeToWaitInMs);
            break;
    }

    if(!success)
    {
        qWarning() << "A problem occurred while waiting";
        return false;
    }

    const bool finalConditionTest = testFunc();

    if(!finalConditionTest)
    {
        qWarning() << "Timeout raised before condition matches";
    }

    return finalConditionTest;
}

bool WaitHelper::pseudoSleep(int timeToWaitInMs, WaitMethod waitMethod)
{
    if(Q_UNLIKELY(timeToWaitInMs < 0))
    {
        qWarning() << "Illegal wait duration" << timeToWaitInMs << ", we can't wait forever.";
        return false;
    }

    bool success = false;
    switch(waitMethod)
    {
        case WaitMethod::UseLocalEventLoop:
            success = eventLoopPseudoWait(nullptr, timeToWaitInMs);
            break;

        case WaitMethod::UseThreadSleep:
            success = sleepPseudoWait(nullptr, timeToWaitInMs);
            break;
    }

    return success;
}

bool WaitHelper::eventLoopPseudoWait(const std::function<bool ()> *testFunc, int timeToWaitInMs)
{
    if(Q_UNLIKELY(testFunc == nullptr && timeToWaitInMs < 0))
    {
        qWarning() << "We can't start a pseudo wait without a function to test and with an "
                   << "inifinite timeout. It will create an infinite loop.";
        return false;
    }

    QEventLoop loop;
    QTimer *testTicTimer = nullptr;
    QTimer *timeout = nullptr;

    auto doQuit = [&loop, testTicTimer, timeout]() {
        if(loop.isRunning())
        {
            if(testTicTimer != nullptr)
            {
                testTicTimer->stop();
            }

            if(timeout != nullptr)
            {
                timeout->stop();
            }

            loop.quit();
        }
    };

    if(timeToWaitInMs >= 0)
    {
        // Configure timeout timer
        timeout = new QTimer();
        timeout->setInterval(timeToWaitInMs);
        timeout->setSingleShot(true);
        timeout->setTimerType(Qt::PreciseTimer);
        QObject::connect(timeout, &QTimer::timeout, doQuit);
        timeout->start();
    }

    if(testFunc != nullptr)
    {
        testTicTimer = new QTimer();

        auto testIfCanQuit = [testFunc, &doQuit, testTicTimer]() {
            if((*testFunc)()) {
                doQuit();
            } else {
                testTicTimer->start();
            }
        };

        // Configure function test timer
        testTicTimer->setInterval(microSleepsDurationMs);
        testTicTimer->setSingleShot(true);
        testTicTimer->setTimerType(Qt::PreciseTimer);
        QObject::connect(testTicTimer, &QTimer::timeout, testIfCanQuit);
        // We can call start here, because even if the interval is 0, the event will only be
        // processed after we leave the synchrone activity. Therefore, it will be processed in the
        // next exec.
        testTicTimer->start();
    }

    loop.exec(QEventLoop::ExcludeUserInputEvents);

    delete testTicTimer;
    delete timeout;

    return true;
}

bool WaitHelper::sleepPseudoWait(const std::function<bool ()> *testFunc, int timeToWaitInMs)
{
    const bool isInfiniteWait { (timeToWaitInMs < 0) };

    if(Q_UNLIKELY(testFunc == nullptr && isInfiniteWait))
    {
        qWarning() << "We can't start a pseudo wait without a function to test and with an "
                   << "inifinite timeout. It will create an infinite loop.";
        return false;
    }

    int elapsedTime = 0;

    while((isInfiniteWait || elapsedTime < timeToWaitInMs) &&
           (testFunc == nullptr || !(*testFunc)()))
    {
        QThread::msleep(microSleepsDurationMs);
        elapsedTime += microSleepsDurationMs;
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    return true;
}
