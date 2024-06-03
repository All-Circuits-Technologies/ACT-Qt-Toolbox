// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QThread>

class ThreadRunningHelper;


/** @brief Useful class to derive in order to manage threading
    @note This class owns an instance of @ref ThreadRunningHelper
    @note This class is useful to manage the call of methods in other threads, to be sure the
          thread is running, call the @ref waitForThread method
    @warning You can't keep the BaseThread in a QSharedPointer, because the class is removed
             whithout stopping the thread (which leads to a crash). To do it, you can use the
             class: @ref BaseThreadHandler */
class BaseThread : public QThread
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance */
        explicit BaseThread(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~BaseThread() override;

    public:
        /** @brief Start the thread and waits it becomes ready
            @return True if no problem occurs */
        bool startThreadAndWaitToBeReady();

    public slots:
        /** @brief Call to properly stop the thread
            @attention When overriding the method, don't forget to call this one at the end
            @note Overrides the method in order to delete the objects owns by the thread
            @return True if no problem occurs */
        virtual bool stopThread();

        /** @brief Call to stop the thread and then delete properly the object
            @attention When overriding the method, don't forget to call this one at the end
            @return True if no problem occurs */
        virtual bool stopAndDeleteThread();

    signals:
        /** @brief Emitted when the thread is ready and it's in exec() method and all has be done
                   in Run() method */
        void ready();

    protected:
        /** @brief Useful method to call in order to be sure that the thread is running
            @note This method is only useful at the thread start. Therefore, if possible, it's
                  better to manage the thread starting by the method
                  @ref startThreadAndWaitToBeReady and then not call this one
            @return True if no problem occurs */
        bool waitForThread() const;

        /** @see QThread::run
            @attention When overriding the method, don't forget to call this one at the end */
        virtual void run() override;

    protected slots:
        /** @brief Called when the thread is ready and when its going to enter in the event loop */
        virtual void onThreadReady();

    private:
        ThreadRunningHelper *_threadRunningHelper{nullptr};
};
