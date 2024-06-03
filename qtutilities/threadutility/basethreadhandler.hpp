// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "basethread.hpp"


/** @brief This class is useful when you want to keep a @ref BaseThread as a class member
    @note The generic class has to be derivated from @ref BaseThread
    @note To remove a @ref BaseThread instance, you have to stop the thread first and then remove
          the class. To help doing that, the @ref BaseThread::stopAndDeleteThread method exists.
          Sometimes, classes are removed automatically like when they are kept in a QSharedPointer,
          but, as explained above, in those cases a problem will appear with the class
          @ref BaseThread.
          That's why @ref BaseThreadHandler exists: it manages the proper deletion of the
          @ref BaseThread in its destructor */
template<class Thread>
class BaseThreadHandler
{
    public:
        /** @brief The class constructor
            @note The class will take the ownership of the thread
            @param thread The thread to manage */
        explicit BaseThreadHandler(Thread *thread);

        /** @brief The class constructor
            @param threadFactory This method allowes to create a thread which will be managed by the
                                 class. The method may return nullptr if a problem occurred in the
                                 creation process. */
        explicit BaseThreadHandler(const std::function<Thread*()> &threadFactory);

        /** @brief Class destructor */
        virtual ~BaseThreadHandler();

    public:
        /** @brief Get the thread managed by the class
            @attention This class may return a nullptr if the class has been created with a nullptr
                       pointer, if the thread factory failed or if the class is being deleted.
            @return The thread managed by the class */
        const Thread *getThread() const { return _baseThread; }

        /** @brief Access the thread managed by the class
            @attention This class may return a nullptr if the class has been created with a nullptr
                       pointer, if the thread factory failed or if the class is being deleted.
            @return The thread managed by the class */
        Thread *accessThread() const { return _baseThread; }

        /** @brief Test if the managed thread is nullptr
            @note This class may handle a nullptr thread if the class has been created with a
                  nullptr pointer, if the thread factory failed or if the class is being deleted.
            @return True if the managed thread is nullptr */
        bool isThreadNull() const { return _baseThread == nullptr; }

    private:
        Thread *_baseThread{nullptr};
};

template<class Thread>
BaseThreadHandler<Thread>::BaseThreadHandler(Thread *thread) :
    _baseThread{thread}
{
    static_assert(std::is_base_of<BaseThread, Thread>::value,
                  "The linked class is not based on Base thread, the base thread handler can't be "
                  "used");
}

template<class Thread>
BaseThreadHandler<Thread>::BaseThreadHandler(const std::function<Thread*()> &threadFactory)
{
    static_assert(std::is_base_of<BaseThread, Thread>::value,
                  "The linked class is not based on Base thread, the base thread handler can't be "
                  "used");

    _baseThread = threadFactory();
}

template<class Thread>
BaseThreadHandler<Thread>::~BaseThreadHandler()
{
    if(_baseThread != nullptr)
    {
        _baseThread->stopAndDeleteThread();
        _baseThread = nullptr;
    }
}
