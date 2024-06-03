// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QDebug>
#include <QHash>
#include <QMutex>
#include <QSharedPointer>


/** @brief Contains instance handles which can be got from specific key
    @note The instances are kept in QWeakPointer.
          When the caller creates an element a QSharedPointer is returned. If the caller doesn't
          keep the QSharedPointer, the element is automatically destroyed (because we only keep
          QWeakPointer in this class; the goal is to avoid memory leak).
    @warning The QWeakPointer objects are removed from the map when we try to access them and if
             their class instance kept in the QWeakPointer has been destroyed.
             Therefore, if you never call @ref removeHandler and if the keys are never the same,
             the list can grow endlessly.
    @note The methods calls are protected by a mutex (to protect calls when they are coming from
          different threads)
    @note The class has only protected methods because we consider that most of the time, the
          derivated class wants to wrap the method, especially the @ref createOrGetHandler method
    @note The key has to be a class which can be used as QHash key (which needs to define some
          methods) */
template<class Key, class Handler>
class HandlerClassMembersMixin
{
    protected:
        /** @brief Get a specific handler thanks to the given key
            @note The handler is destroyed if no more class keeps a QSharedPointer to the handler
            @param key The unique key to get the handler
            @return The handler got if it has been created or if it still alive */
        QSharedPointer<Handler> getHandler(const Key &key);

        /** @brief Create or get a specific handler thanks to the given key
            @note The handler is destroyed if no more class keeps a QSharedPointer to the handler
            @param key The unique key to create or get the handler with
            @param instanceFactory This method will be used to ask the instance creation. The method
                                   may returns nullptr if a problem occurred in the creation
                                   process.
            @return The handler created (or already existent if one already exists with the same
                    key), or nullptr if a problem occurred */
        QSharedPointer<Handler> createOrGetHandler(
            const Key &key,
            const std::function<QSharedPointer<Handler> (const Key&)> &instanceFactory);

        /** @brief Remove the handler from the class
            @note By default, the QWeakPointer of the handler is destroyed when we try to access it
                  but it already been destroyed.
            @note This method doesn't need to be called if the handler keys are not generated and
                  if the keys are reused at each sequence iteration.
            @attention This class needs to be called if we can create multiple handlers and the keys
                       are never the same (which will may grow endlessly the list)
            @note The parameter @ref force is only relevant to use when you know the QSharedPointer
                  you kept is the only one and will be destroyed shortly after this method
                  calls.
                  For instance, when the QSharedPointer is a class member and this method is called
                  in the class destroyer. */
        bool removeHandler(const Key &key, bool force = false);

    private:
        /** @brief This is the mutex timeout in milliseconds used with handlers */
        static const constexpr int HandlerMutexTimeoutInMs = 1000;

    private:
        QMutex _mutex;
        QHash<Key, QWeakPointer<Handler>> _handlerInstances;
};

template<class Key, class Handler>
QSharedPointer<Handler> HandlerClassMembersMixin<Key, Handler>::getHandler(const Key &key)
{
    if(!_mutex.tryLock(HandlerMutexTimeoutInMs))
    {
        qWarning() << "Handler mutex already locked, and it hasn't released in the expected time";
        return nullptr;
    }

    if(!_handlerInstances.contains(key))
    {
        qWarning() << "The handler name: " << key << ", is not known, it hasn't been "
                   << "created or setup";
        _mutex.unlock();
        return nullptr;
    }

    QSharedPointer<Handler> pointer = _handlerInstances[key].toStrongRef();

    if(pointer == nullptr)
    {
        _handlerInstances.remove(key);
        qWarning() << "The handler name: " << key << ", was known but it has been released since";
        _mutex.unlock();
        return nullptr;
    }

    _mutex.unlock();
    return pointer;
}

template<class Key, class Handler>
QSharedPointer<Handler> HandlerClassMembersMixin<Key, Handler>::createOrGetHandler(
    const Key &key,
    const std::function<QSharedPointer<Handler> (const Key &)> &instanceFactory)
{
    if(!_mutex.tryLock(HandlerMutexTimeoutInMs))
    {
        qWarning() << "Handler mutex already locked, and it hasn't released in the expected time";
        return nullptr;
    }

    if(_handlerInstances.contains(key))
    {
        QSharedPointer<Handler> pointer = _handlerInstances[key].toStrongRef();

        if(pointer != nullptr)
        {
            qInfo() << "The handler name: " << key << ", has already been created, we return it";
            _mutex.unlock();
            return pointer;
        }

        // If we are here, it means that the instance has been removed since
        _handlerInstances.remove(key);
    }

    QSharedPointer<Handler> sharedHandler = instanceFactory(key);

    if(sharedHandler == nullptr)
    {
        qWarning() << "A problem occurred when tried to create the handler: " << key;
        _mutex.unlock();
        return nullptr;
    }

    _handlerInstances.insert(key, sharedHandler.toWeakRef());

    _mutex.unlock();

    return sharedHandler;
}

template<class Key, class Handler>
bool HandlerClassMembersMixin<Key, Handler>::removeHandler(const Key &key, bool force)
{
    if(!_mutex.tryLock(HandlerMutexTimeoutInMs))
    {
        qWarning() << "Handler mutex already locked, and it hasn't released in the expected time";
        return false;
    }

    if(_handlerInstances.contains(key))
    {
        QSharedPointer<Handler> pointer = _handlerInstances[key].toStrongRef();

        if(pointer != nullptr && !force)
        {
            qWarning() << "The handler name: " << key << ", is still alive in a class; therefore "
                       << "we can't remove it. ";
            _mutex.unlock();
            return false;
        }

        _handlerInstances.remove(key);
    }

    _mutex.unlock();
    return true;
}
