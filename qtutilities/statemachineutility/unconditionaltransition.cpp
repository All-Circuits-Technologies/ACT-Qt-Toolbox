// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "unconditionaltransition.hpp"


UnconditionalTransition::UnconditionalTransition(QAbstractState *target, QState *sourceState) :
    QAbstractTransition(sourceState)
{
    setTargetState(target);
}

UnconditionalTransition::UnconditionalTransition(QState *sourceState) :
    QAbstractTransition(sourceState)
{
}

bool UnconditionalTransition::eventTest(QEvent *event)
{
    Q_UNUSED(event)

    return true;
}

void UnconditionalTransition::onTransition(QEvent *event)
{
    Q_UNUSED(event)
}
