// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tichandler.hpp"

#include <QDebug>

#include "definesutility/definesutility.hpp"
#include "managersutility/aglobalmanager.hpp"
#include "ticutility/tichandleraccesskey.hpp"
#include "ticutility/ticmanager.hpp"
#include "ticutility/tictimer.hpp"


TicHandler::TicHandler(QObject *parent):
    QObject(parent)
{
}

TicHandler::TicHandler(TicManager &ticManager,
                       TicType::Enum ticType,
                       quint8 triggerEveryNthOccurrence,
                       QObject *parent):
    QObject(parent),
    _ticType(ticType),
    _triggerEveryNthOccurrence(triggerEveryNthOccurrence),
    _ticManager(&ticManager)
{
    manageCurrentTypeRegistration(true);
}

TicHandler::~TicHandler()
{
    manageCurrentTypeRegistration(false);
}

void TicHandler::setTicType(TicType::Enum ticType)
{
    if(ticType != _ticType)
    {
        manageCurrentTypeRegistration(false);

        _ticType = ticType;

        manageCurrentTypeRegistration(true);

        emit ticTypeModified(ticType);
    }
}

void TicHandler::setTriggerEveryNthOccurrence(quint8 triggerEveryNthOccurrence)
{
    if(_triggerEveryNthOccurrence != triggerEveryNthOccurrence)
    {
        _triggerEveryNthOccurrence = triggerEveryNthOccurrence;
        emit nthOccurrenceModified(triggerEveryNthOccurrence);
    }
}

void TicHandler::onTicCalled(quint32 occurrence)
{
    if((occurrence % _triggerEveryNthOccurrence) != 0) {
        // Nothing to trigger
        return;
    }

    emit ticTriggered(occurrence / _triggerEveryNthOccurrence);
}

const TicTimer *TicHandler::registerHandler(TicType::Enum type)
{
    if(type == TicType::Unknown || _ticManager == nullptr)
    {
        // Can't take one if the type is Unknown or the _ticManager linked is null
        return nullptr;
    }

    return &_ticManager->takeOne(TicHandlerAccessKey(), type);
}

void TicHandler::unregisterHandler(TicType::Enum type)
{
    if(type == TicType::Unknown || _ticManager == nullptr)
    {
        // Nothing to do here
        return;
    }

    _ticManager->releaseOne(TicHandlerAccessKey(), type);
}

void TicHandler::manageTimerConnection(const TicTimer *timer, bool toConnect)
{
    if(timer == nullptr)
    {
        return;
    }

    if(toConnect)
    {
        connect(timer, &TicTimer::ticTimeout, this, &TicHandler::onTicCalled);
    }
    else
    {
        disconnect(timer, &TicTimer::ticTimeout, this, &TicHandler::onTicCalled);
    }
}

void TicHandler::manageCurrentTypeRegistration(bool registerHand)
{
    if(registerHand)
    {
        _linkedTimer = registerHandler(_ticType);
        manageTimerConnection(_linkedTimer, true);
    }
    else
    {
        manageTimerConnection(_linkedTimer, false);
        unregisterHandler(_ticType);
        _linkedTimer = nullptr;
    }

}
