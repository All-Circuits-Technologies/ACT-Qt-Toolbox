// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tictimer.hpp"

#include <QDebug>


TicTimer::TicTimer(TicType::Enum ticType, QObject *parent) :
    QObject(parent),
    _ticType(ticType)
{
    initTimer();
}

void TicTimer::start()
{
    _timer.start();
}

void TicTimer::stop()
{
    _timer.stop();
}

void TicTimer::onTimeout()
{
    emit ticTimeout(_occurrence);

    if(_occurrence == std::numeric_limits<quint32>::max())
    {
        _occurrence = 0;
    }
    else
    {
        _occurrence++;
    }
}

void TicTimer::initTimer()
{
    if(_ticType == TicType::Unknown)
    {
        return;
    }

    _timer.setInterval(TicType::getPeriodInMs(_ticType));
    _timer.setSingleShot(false);
    _timer.setTimerType(Qt::PreciseTimer);

    connect(&_timer, &QTimer::timeout,
            this,    &TicTimer::onTimeout);
}
