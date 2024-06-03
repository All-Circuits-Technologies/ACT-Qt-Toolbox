// SPDX-FileCopyrightText: 2018 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

/* ------------------------------------------------------------------
 * asyncwaterfallhelper.cpp - COPYRIGHT (C) 2018 BMS CIRCUITS
 * ------------------------------------------------------------------
 */

#include "asyncwaterfallhelper.hpp"


AsyncWaterfallHelper::AsyncWaterfallHelper(QList<AsyncTypes::CallbackFunc> callbacks,
                                           AsyncTypes::Next finalCallback,
                                           QObject *parent) :
    QObject(parent),
    _callbacks(callbacks),
    _finalCallback(finalCallback)
{
    connect(this, SIGNAL(nextStep(bool)), this, SLOT(manageAsync(bool)));
}

void AsyncWaterfallHelper::next(bool anErrorOccured)
{
    emit nextStep(anErrorOccured);
}

void AsyncWaterfallHelper::manageAsync(bool anErrorOccured)
{
    if(anErrorOccured)
    {
        // An error occured, stop right now and go to the final callback
        _finalCallback(anErrorOccured);
        deleteLater();
    }
    else if(_callbacksIndex >= _callbacks.length())
    {
        // All the callbacks have been called
        _finalCallback(false);
        deleteLater();
    }
    else
    {
        int index = _callbacksIndex;
        _callbacksIndex++;

        _callbacks[index](AsyncTypes::castNextCallback(*this, &AsyncWaterfallHelper::next));
    }
}

void AsyncWaterfallHelper::manage(const QList<AsyncTypes::CallbackFunc> &callbacks,
                                  AsyncTypes::Next finalCallback)
{
    AsyncWaterfallHelper *asyncHelper = new AsyncWaterfallHelper(callbacks, finalCallback);
    asyncHelper->manageAsync(false);
}
