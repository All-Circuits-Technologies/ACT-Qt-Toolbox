// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "ticmanager.hpp"

#include <QDebug>
#include <QTime>

#include "ticutility/tichandler.hpp"
#include "ticutility/tichandleraccesskey.hpp"


TicManager::TicManager(QObject *parent) : AbstractManager(parent)
{
}

TicHandler *TicManager::createTicHandler(TicType::Enum ticType,
                                         quint8 triggerEveryNthOccurrence,
                                         QObject *parent)
{
    return new TicHandler(*this, ticType, triggerEveryNthOccurrence, parent);
}

const TicTimer &TicManager::takeOne(const TicHandlerAccessKey &key, TicType::Enum type)
{
    Q_UNUSED(key)

    TicTypeInfos &infos = getOrCreateTypeInfo(type);

    infos.handlersNb++;

    if(infos.handlersNb == 1)
    {
        atFirstHandler(infos);
    }

    return *infos.timer;
}

void TicManager::releaseOne(const TicHandlerAccessKey &key, TicType::Enum type)
{
    Q_UNUSED(key)

    TicTypeInfos &infos = getOrCreateTypeInfo(type);

    if(infos.handlersNb == 0)
    {
        qWarning() << "A tic handler tries to release: " << TicType::toString(type) << ", but no "
                   << "handler was currently known, this can't logically happen (except if there "
                   << "is an algo problem)";
        return;
    }

    infos.handlersNb--;

    if(infos.handlersNb == 0)
    {
        whenNoMoreHandlers(infos);
    }
}

void TicManager::registerMetaTypes()
{
    TicType::registerMetaTypes();
}

void TicManager::atFirstHandler(TicTypeInfos &infos)
{
    // Get the current time (got from operating system) and start the timer at the next second
    // This way to do doesn't  certify that we precisely begins at the next second, because it
    // doesn't take into account the code operation time.
    // But it allows to be closer than doing nothing
    QTimer::singleShot(1000 - QTime::currentTime().msec(), infos.timer, &TicTimer::start);
}

void TicManager::whenNoMoreHandlers(TicTypeInfos &infos)
{
    infos.timer->stop();
}

TicManager::TicTypeInfos &TicManager::getOrCreateTypeInfo(TicType::Enum ticType)
{
    if(!_ticTypeInfos.contains(ticType))
    {
        _ticTypeInfos.insert(ticType, { new TicTimer(ticType, this) });
    }

    return _ticTypeInfos[ticType];
}
