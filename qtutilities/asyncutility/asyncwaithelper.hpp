// SPDX-FileCopyrightText: 2018 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

/* ------------------------------------------------------------------
 * asyncwaithelper.hpp - COPYRIGHT (C) 2018 BMS CIRCUITS
 * ------------------------------------------------------------------
 */

#pragma once

#include <QObject>

#include <QSet>
#include <QThread>

#include "asynctypes.hpp"


/*! @brief Defines an async wait helper for inter-thread calling.
    @note The thread, where the pseudoWait is called, will wait the end of the method wanted.
          Therefore, for the caller thread, the method is asynchrone
          The method targeted will be called in its instance thread. This method is asynchrone and
          so doesn't block this thread. */
class AsyncWaitHelper : public QObject
{
    Q_OBJECT

    private:
        /*! @brief State of the process */
        enum ProcessState
        {
            InProgress,         //!< @brief The process in in progress
            EndedWithError,     //!< @brief The method has ended with error
            EndedWithoutError   //!< @brief The method has ended with no error
        };

    public:
        virtual ~AsyncWaitHelper();

    private:
        /*! @brief Default constructor
            @param methodToCall The method to call
            @param parent Object parent */
        explicit AsyncWaitHelper(AsyncTypes::CallbackFunc methodToCall,
                                 QObject *parent = nullptr);

    private:
        /*! @brief Get the current state of process
            @return The process state */
        ProcessState getProcessState() const { return _processState; }

        /*! @brief Ask the method calling
            @note Call @ref AsyncWaitHelper::callMethod via @ref QTimer::singleShot
            @note Useful in order to call from a thread to another one. */
        void askCalling();

    private slots:
        /*! @brief Call the wanted method. */
        void callMethod();

    public:
        /*! @brief Wait until callback has ended, or timeout expires. This method makes event loop
                   working.
            @note Execute the method in caller thread.
            @note Doesn't block the caller thread
            @param caller The instance object which own the method given has an async method pattern
            @param method The method to call in the caller thread. Its parameter has to be called
                          in order to signify the end of process.
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite) */
        template<class T, class Z=AsyncTypes::CallbackMeth<T>>
        static bool pseudoWait(T &caller, Z method, int timeToWaitInMs = -1);

    private:
        /*! @brief Wait the method end
            @param waitHelper The AsyncWaitHelper instance used for waiting
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite) */
        static bool useSyncWaitHelper(const AsyncWaitHelper &waitHelper, int timeToWaitInMs);

    private:
        AsyncTypes::CallbackFunc _methodToCall;
        ProcessState _processState{InProgress};

    private:
        static QSet<AsyncWaitHelper*> currentWaitHelpers;
};

template<class T, class Z>
bool AsyncWaitHelper::pseudoWait(T &caller, Z method, int timeToWaitInMs)
{
    static_assert(std::is_base_of<QObject, T>::value, "T must inherit from QObject");

    AsyncWaitHelper *waitHelper = new AsyncWaitHelper(AsyncTypes::castMethCallback(caller, method));

    if(caller.thread() != QThread::currentThread())
    {
        waitHelper->moveToThread(caller.thread());
    }

    waitHelper->askCalling();

    bool noError = AsyncWaitHelper::useSyncWaitHelper(*waitHelper, timeToWaitInMs);

    waitHelper->deleteLater();

    return noError;
}
