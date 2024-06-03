// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QTimer>

#include "ticutility/tictype.hpp"


/** @brief This timer is used to generate tic on a specific frequency
    @note It also returns an occurrence value at each tic */
class TicTimer : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param ticType The tic type managed by the timer
            @param parent The parent instance class */
        explicit TicTimer(TicType::Enum ticType, QObject *parent = nullptr);

    public slots:
        /** @brief Call to start the timer */
        void start();

        /** @brief Call to stop the timer */
        void stop();

    signals:
        /** @brief Emitted at each tic occurrence
            @param occurrence This occurrence is incremented at each tic */
        void ticTimeout(quint32 occurrence);

    private slots:
        /** @brief Called at each QTimer timeout */
        void onTimeout();

    private:
        /** @brief Utility method to initialize the timer and connect to the right signals */
        void initTimer();

    private:
        quint32 _occurrence{0};
        TicType::Enum _ticType{TicType::Unknown};

        QTimer _timer;
};
