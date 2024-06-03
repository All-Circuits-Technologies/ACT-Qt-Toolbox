// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QAbstractTransition>


/** @brief This transition is used to create an "always true" transition. When the node is entered
           the state machine is directly led to the next one */
class UnconditionalTransition : public QAbstractTransition
{
    public:
        /** @brief Class constructor
            @param target The target of the transition
            @param sourceState The state which is at the source of the transition */
        explicit UnconditionalTransition(QAbstractState *target,
                                         QState *sourceState = nullptr);

        /** @brief Class constructor
            @param sourceState The state which is at the source of the transition  */
        explicit UnconditionalTransition(QState *sourceState = nullptr);

        /** @brief Class destructor */
        virtual ~UnconditionalTransition() override = default;

    protected:
        /** @brief Override this method to always returns true
            @param event The event to test in order to know if the transition is valid
            @return True if we can go to the next state */
        virtual bool eventTest(QEvent *event) override;

        /** @brief Override this method, because is pure virtual in QAbstractTransition, but nothing
                   to do here.
            @param event The event of the transition */
        virtual void onTransition(QEvent *event) override;
};
