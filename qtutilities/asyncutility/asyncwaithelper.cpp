// SPDX-FileCopyrightText: 2018 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

/* ------------------------------------------------------------------
 * asyncwaithelper.cpp - COPYRIGHT (C) 2018 BMS CIRCUITS
 * ------------------------------------------------------------------
 */

#include "asyncwaithelper.hpp"

#include <QDebug>
#include <QTimer>

#include <waithelper.hpp>

QSet<AsyncWaitHelper*> AsyncWaitHelper::currentWaitHelpers = {};


AsyncWaitHelper::AsyncWaitHelper(AsyncTypes::CallbackFunc methodToCall,
                                 QObject *parent) :
    QObject(parent),
    _methodToCall(methodToCall)
{
    AsyncWaitHelper::currentWaitHelpers.insert(this);
}

AsyncWaitHelper::~AsyncWaitHelper()
{
    AsyncWaitHelper::currentWaitHelpers.remove(this);
}

void AsyncWaitHelper::askCalling()
{
    // The use of function pointer doesn't work here when calling from an exterior thread
    QTimer::singleShot(0, this, SLOT(callMethod()));
}

void AsyncWaitHelper::callMethod()
{
    _methodToCall([=](bool error)
    {
        if(!AsyncWaitHelper::currentWaitHelpers.contains(this))
        {
            qWarning() << "Useless to go further, the AsyncWaitHelper instance doesn't exist "
                          "anymore.";
            return;
        }

        if(error)
        {
            this->_processState = ProcessState::EndedWithError;
        }
        else
        {
            this->_processState = ProcessState::EndedWithoutError;
        }
    });
}

bool AsyncWaitHelper::useSyncWaitHelper(const AsyncWaitHelper &waitHelper, int timeToWaitInMs)
{
    bool ok = WaitHelper::pseudoWait([&]()
                             { return (waitHelper.getProcessState() != ProcessState::InProgress); },
                                     timeToWaitInMs);

    return (ok && (waitHelper.getProcessState() == ProcessState::EndedWithoutError));
}
