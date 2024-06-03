// SPDX-FileCopyrightText: 2018 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QMutex>


/*! @brief Thread helper which helps to wait the running of thread before to do anything in other
           thread.

    @note ThreadRunningHelper::waitForThread method has to be called in the caller thread
    @note ThreadRunningHelper::onThreadReady method has to be called in the thread managed, in order
          to indicate that the thread is ready */
class ThreadRunningHelper : public QObject
{
    Q_OBJECT

    public:
        /*! @brief Class constructor
            @param parent The class parent */
        explicit ThreadRunningHelper(QObject *parent = nullptr);

        /*! @brief Test if the the wanted thread is running, if it's running return true, if not
                   wait some times the running.
            @return True if the thread is running. false if a problem occurred and the timeout has
                    raised. */
        bool waitForThread();

    public slots:
        /*! @brief Method to call in the thread, in order to indicate that the thread is running and
                   has finished the initialisation. */
        void onThreadReady();

    private:
        QMutex _mutex;
        bool _running;

    private:
        static const constexpr int timeoutInMs{1000};
};
