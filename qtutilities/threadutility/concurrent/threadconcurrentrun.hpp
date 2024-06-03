// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <functional>
#include <QDebug>
#include <QObject>
#include <QMutex>

#include "waitutility/waithelper.hpp"


/** @brief This class contains functions which help to call synchrone class method
           (from a different thread) in the class thread.
    @note Those methods are thread safe
    @note If you want to call a method in an another thread (no matter of the thread), it's better
          to use @ref Qt::ConcurrentRun.
    @note Run functions limitations:
            - The run function can only be used with Q_OBJECT class
            - The method to call has to be synchrone
            - The run function only works with class methods
            - The run function waits until the class method is processed, there is no
              way to cancel the call
            - You cannot choose the thread where the class method will be called; the method
              will be called in the given class thread. */
class ThreadConcurrentRun
{
    public:
        /** @brief Call a synchrone class method in the class thread, and return the returned value
                   of the method
            @note The method is thread-safe
            @note This function blocks the current thread until the method is done in the other
                  thread. When blocking the event loop is not freezed, but periodically processed
            @note The class method arguments have to be given after the method
            @note This function is for calling non-const method, like this one:
                        T fn();
            @param object The object which has the method given, the method will be called in its
                          thread. The object has to be a Q_OBJECT
            @param fn The method (of the class given) to call in the class thread. The method has
                      to be synchrone, the returned value is returned by this function
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return The returned value by the method called */
        template<typename R, typename ObjClass, typename FnClass>
        static R run(ObjClass &object,
                     R (FnClass::*fn)(),
                     int timeoutInMs = -1,
                     WaitHelper::WaitMethod waitMethod = WaitHelper::WaitMethod::UseLocalEventLoop);

        /** @brief Call a synchrone class method in the class thread, and return the returned value
                   of the method
            @note The method is thread-safe
            @note This function blocks the current thread until the method is done in the other
                  thread. When blocking the event loop is not freezed, but periodically processed
            @note The class method arguments have to be given after the method
            @note This function is for calling const method, like this one:
                        T fn() const;
            @param object The object which has the method given, the method will be called in its
                          thread. The object has to be a Q_OBJECT
            @param fn The method (of the class given) to call in the class thread. The method has
                      to be synchrone, the returned value is returned by this function
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return The returned value by the method called */
        template<typename R, typename ObjClass, typename FnClass>
        static R run(const ObjClass &object,
                     R (FnClass::*fn)() const,
                     int timeoutInMs = -1,
                     WaitHelper::WaitMethod waitMethod = WaitHelper::WaitMethod::UseLocalEventLoop);

        /** @brief Call a synchrone class method in the class thread, and return the returned value
                   of the method
            @note The method is thread-safe
            @note This function blocks the current thread until the method is done in the other
                  thread. When blocking the event loop is not freezed, but periodically processed
            @note The class method arguments have to be given after the method
            @note This function is for calling non-const method, like this one:
                        T fn(const Arg1 &arg1);
            @param object The object which has the method given, the method will be called in its
                          thread. The object has to be a Q_OBJECT
            @param fn The method (of the class given) to call in the class thread. The method has
                      to be synchrone, the returned value is returned by this function
            @param arg1 The first argument to give when calling the method
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return The returned value by the method called */
        template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1>
        static R run(ObjClass &object,
                     R (FnClass::*fn)(Param1),
                     const Arg1 &arg1,
                     int timeoutInMs = -1,
                     WaitHelper::WaitMethod waitMethod = WaitHelper::WaitMethod::UseLocalEventLoop);

        /** @brief Call a synchrone class method in the class thread, and return the returned value
                   of the method
            @note The method is thread-safe
            @note This function blocks the current thread until the method is done in the other
                  thread. When blocking the event loop is not freezed, but periodically processed
            @note The class method arguments have to be given after the method
            @note This function is for calling const method, like this one:
                        T fn(const Arg1 &arg1) const;
            @param object The object which has the method given, the method will be called in its
                          thread. The object has to be a Q_OBJECT
            @param fn The method (of the class given) to call in the class thread. The method has
                      to be synchrone, the returned value is returned by this function
            @param arg1 The first argument to give when calling the method
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return The returned value by the method called */
        template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1>
        static R run(const ObjClass &object,
                     R (FnClass::*fn)(Param1) const,
                     const Arg1 &arg1,
                     int timeoutInMs = -1,
                     WaitHelper::WaitMethod waitMethod = WaitHelper::WaitMethod::UseLocalEventLoop);

        /** @brief Call a synchrone class method in the class thread, and return the returned value
                   of the method
            @note The method is thread-safe
            @note This function blocks the current thread until the method is done in the other
                  thread. When blocking the event loop is not freezed, but periodically processed
            @note The class method arguments have to be given after the method
            @note This function is for calling non-const method, like this one:
                        T fn(const Arg1 &arg1, const Arg2 &arg2);
            @param object The object which has the method given, the method will be called in its
                          thread. The object has to be a Q_OBJECT
            @param fn The method (of the class given) to call in the class thread. The method has
                      to be synchrone, the returned value is returned by this function
            @param arg1 The first argument to give when calling the method
            @param arg2 The second argument to give when calling the method
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return The returned value by the method called */
        template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
                 typename Param2, typename Arg2>
        static R run(ObjClass &object,
                     R (FnClass::*fn)(Param1, Param2),
                     const Arg1 &arg1,
                     const Arg2 &arg2,
                     int timeoutInMs = -1,
                     WaitHelper::WaitMethod waitMethod = WaitHelper::WaitMethod::UseLocalEventLoop);

        /** @brief Call a synchrone class method in the class thread, and return the returned value
                   of the method
            @note The method is thread-safe
            @note This function blocks the current thread until the method is done in the other
                  thread. When blocking the event loop is not freezed, but periodically processed
            @note The class method arguments have to be given after the method
            @note This function is for calling const method, like this one:
                        T fn(const Arg1 &arg1, const Arg2 &arg2) const;
            @param object The object which has the method given, the method will be called in its
                          thread. The object has to be a Q_OBJECT
            @param fn The method (of the class given) to call in the class thread. The method has
                      to be synchrone, the returned value is returned by this function
            @param arg1 The first argument to give when calling the method
            @param arg2 The second argument to give when calling the method
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return The returned value by the method called */
        template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
                 typename Param2, typename Arg2>
        static R run(const ObjClass &object,
                     R (FnClass::*fn)(Param1, Param2) const,
                     const Arg1 &arg1,
                     const Arg2 &arg2,
                     int timeoutInMs = -1,
                     WaitHelper::WaitMethod waitMethod = WaitHelper::WaitMethod::UseLocalEventLoop);

        /** @brief Call a synchrone class method in the class thread, and return the returned value
                   of the method
            @note The method is thread-safe
            @note This function blocks the current thread until the method is done in the other
                  thread. When blocking the event loop is not freezed, but periodically processed
            @note The class method arguments have to be given after the method
            @note This function is for calling non-const method, like this one:
                        T fn(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3);
            @param object The object which has the method given, the method will be called in its
                          thread. The object has to be a Q_OBJECT
            @param fn The method (of the class given) to call in the class thread. The method has
                      to be synchrone, the returned value is returned by this function
            @param arg1 The first argument to give when calling the method
            @param arg2 The second argument to give when calling the method
            @param arg3 The third argument to give when calling the method
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return The returned value by the method called */
        template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
                 typename Param2, typename Arg2, typename Param3, typename Arg3>
        static R run(ObjClass &object,
                     R (FnClass::*fn)(Param1, Param2, Param3),
                     const Arg1 &arg1,
                     const Arg2 &arg2,
                     const Arg3 &arg3,
                     int timeoutInMs = -1,
                     WaitHelper::WaitMethod waitMethod = WaitHelper::WaitMethod::UseLocalEventLoop);

        /** @brief Call a synchrone class method in the class thread, and return the returned value
                   of the method
            @note The method is thread-safe
            @note This function blocks the current thread until the method is done in the other
                  thread. When blocking the event loop is not freezed, but periodically processed
            @note The class method arguments have to be given after the method
            @note This function is for calling const method, like this one:
                        T fn(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3) const;
            @param object The object which has the method given, the method will be called in its
                          thread. The object has to be a Q_OBJECT
            @param fn The method (of the class given) to call in the class thread. The method has
                      to be synchrone, the returned value is returned by this function
            @param arg1 The first argument to give when calling the method
            @param arg2 The second argument to give when calling the method
            @param arg3 The third argument to give when calling the method
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return The returned value by the method called */
        template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
                 typename Param2, typename Arg2, typename Param3, typename Arg3>
        static R run(const ObjClass &object,
                     R (FnClass::*fn)(Param1, Param2, Param3) const,
                     const Arg1 &arg1,
                     const Arg2 &arg2,
                     const Arg3 &arg3,
                     int timeoutInMs = -1,
                     WaitHelper::WaitMethod waitMethod = WaitHelper::WaitMethod::UseLocalEventLoop);

        /** @brief Call a synchrone class method in the class thread, and return the returned value
                   of the method
            @note The method is thread-safe
            @note This function blocks the current thread until the method is done in the other
                  thread. When blocking the event loop is not freezed, but periodically processed
            @note The class method arguments have to be given after the method
            @note This function is for calling non-const method, like this one:
                        T fn(const Arg1 &arg1,
                             const Arg2 &arg2,
                             const Arg3 &arg3,
                             const Arg4 &arg4);
            @param object The object which has the method given, the method will be called in its
                          thread. The object has to be a Q_OBJECT
            @param fn The method (of the class given) to call in the class thread. The method has
                      to be synchrone, the returned value is returned by this function
            @param arg1 The first argument to give when calling the method
            @param arg2 The second argument to give when calling the method
            @param arg3 The third argument to give when calling the method
            @param arg4 The fourth argument to give when calling the method
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return The returned value by the method called */
        template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
                 typename Param2, typename Arg2, typename Param3, typename Arg3,
                 typename Param4, typename Arg4>
        static R run(ObjClass &object,
                     R (FnClass::*fn)(Param1, Param2, Param3, Param4),
                     const Arg1 &arg1,
                     const Arg2 &arg2,
                     const Arg3 &arg3,
                     const Arg4 &arg4,
                     int timeoutInMs = -1,
                     WaitHelper::WaitMethod waitMethod = WaitHelper::WaitMethod::UseLocalEventLoop);

        /** @brief Call a synchrone class method in the class thread, and return the returned value
                   of the method
            @note The method is thread-safe
            @note This function blocks the current thread until the method is done in the other
                  thread. When blocking the event loop is not freezed, but periodically processed
            @note The class method arguments have to be given after the method
            @note This function is for calling const method, like this one:
                        T fn(const Arg1 &arg1,
                             const Arg2 &arg2,
                             const Arg3 &arg3,
                             const Arg4 &arg4) const;
            @param object The object which has the method given, the method will be called in its
                          thread. The object has to be a Q_OBJECT
            @param fn The method (of the class given) to call in the class thread. The method has
                      to be synchrone, the returned value is returned by this function
            @param arg1 The first argument to give when calling the method
            @param arg2 The second argument to give when calling the method
            @param arg3 The third argument to give when calling the method
            @param arg4 The fourth argument to give when calling the method
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return The returned value by the method called */
        template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
                 typename Param2, typename Arg2, typename Param3, typename Arg3,
                 typename Param4, typename Arg4>
        static R run(const ObjClass &object,
                     R (FnClass::*fn)(Param1, Param2, Param3, Param4) const,
                     const Arg1 &arg1,
                     const Arg2 &arg2,
                     const Arg3 &arg3,
                     const Arg4 &arg4,
                     int timeoutInMs = -1,
                     WaitHelper::WaitMethod waitMethod = WaitHelper::WaitMethod::UseLocalEventLoop);

        /** @brief Call a synchrone class method in the class thread, and return the returned value
                   of the method
            @note The method is thread-safe
            @note This function blocks the current thread until the method is done in the other
                  thread. When blocking the event loop is not freezed, but periodically processed
            @note The class method arguments have to be given after the method
            @note This function is for calling non-const method, like this one:
                        T fn(const Arg1 &arg1,
                             const Arg2 &arg2,
                             const Arg3 &arg3,
                             const Arg4 &arg4,
                             const Arg5 &arg5);
            @param object The object which has the method given, the method will be called in its
                          thread. The object has to be a Q_OBJECT
            @param fn The method (of the class given) to call in the class thread. The method has
                      to be synchrone, the returned value is returned by this function
            @param arg1 The first argument to give when calling the method
            @param arg2 The second argument to give when calling the method
            @param arg3 The third argument to give when calling the method
            @param arg4 The fourth argument to give when calling the method
            @param arg5 The fifth argument to give when calling the method
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return The returned value by the method called */
        template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
                 typename Param2, typename Arg2, typename Param3, typename Arg3,
                 typename Param4, typename Arg4, typename Param5, typename Arg5>
        static R run(ObjClass &object,
                     R (FnClass::*fn)(Param1, Param2, Param3, Param4, Param5),
                     const Arg1 &arg1,
                     const Arg2 &arg2,
                     const Arg3 &arg3,
                     const Arg4 &arg4,
                     const Arg5 &arg5,
                     int timeoutInMs = -1,
                     WaitHelper::WaitMethod waitMethod = WaitHelper::WaitMethod::UseLocalEventLoop);

        /** @brief Call a synchrone class method in the class thread, and return the returned value
                   of the method
            @note The method is thread-safe
            @note This function blocks the current thread until the method is done in the other
                  thread. When blocking the event loop is not freezed, but periodically processed
            @note The class method arguments have to be given after the method
            @note This function is for calling const method, like this one:
                        T fn(const Arg1 &arg1,
                             const Arg2 &arg2,
                             const Arg3 &arg3,
                             const Arg4 &arg4,
                             const Arg5 &arg5) const;
            @param object The object which has the method given, the method will be called in its
                          thread. The object has to be a Q_OBJECT
            @param fn The method (of the class given) to call in the class thread. The method has
                      to be synchrone, the returned value is returned by this function
            @param arg1 The first argument to give when calling the method
            @param arg2 The second argument to give when calling the method
            @param arg3 The third argument to give when calling the method
            @param arg4 The fourth argument to give when calling the method
            @param arg5 The fifth argument to give when calling the method
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return The returned value by the method called */
        template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
                 typename Param2, typename Arg2, typename Param3, typename Arg3,
                 typename Param4, typename Arg4,typename Param5, typename Arg5>
        static R run(const ObjClass &object,
                     R (FnClass::*fn)(Param1, Param2, Param3, Param4, Param5) const,
                     const Arg1 &arg1,
                     const Arg2 &arg2,
                     const Arg3 &arg3,
                     const Arg4 &arg4,
                     const Arg5 &arg5,
                     int timeoutInMs = -1,
                     WaitHelper::WaitMethod waitMethod = WaitHelper::WaitMethod::UseLocalEventLoop);

    //private:
        /** @brief Call the function given in the object thread given
            @param object The function will be called in the thread of the object given
            @param func The function to call in the object thread
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return The returned value by the function called*/
        template<typename R>
        static R runImpl(QObject *object,
                         const std::function<R ()> &func,
                         int timeoutInMs = -1,
                         WaitHelper::WaitMethod waitMethod =
                         WaitHelper::WaitMethod::UseLocalEventLoop);

        /** @brief Call the constant function given in the object thread given
            @param object The function will be called in the thread of the object given
            @param func The function to call in the object thread
            @param timeoutInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return The returned value by the function called*/
        template<typename R>
        static R runImpl(const QObject *object,
                         const std::function<R ()> &func,
                         int timeoutInMs = -1,
                         WaitHelper::WaitMethod waitMethod =
                         WaitHelper::WaitMethod::UseLocalEventLoop);
};

template<typename R, typename ObjClass, typename FnClass>
R ThreadConcurrentRun::run(ObjClass &object,
                           R (FnClass::*fn)(),
                           int timeoutInMs,
                           WaitHelper::WaitMethod waitMethod)
{
    static_assert(std::is_base_of<QObject, ObjClass>::value, "Targetted object must be a QObject");
    static_assert(std::is_base_of<FnClass, ObjClass>::value, "Object must implement given method");
    std::function<R()> func = std::bind(fn, &object);
    return runImpl(&object, func, timeoutInMs, waitMethod);
}

template<typename R, typename ObjClass, typename FnClass>
R ThreadConcurrentRun::run(const ObjClass &object,
                           R (FnClass::*fn)() const,
                           int timeoutInMs,
                           WaitHelper::WaitMethod waitMethod)
{
    static_assert(std::is_base_of<QObject, ObjClass>::value, "Targetted object must be a QObject");
    static_assert(std::is_base_of<FnClass, ObjClass>::value, "Object must implement given method");
    std::function<R()> func = std::bind(fn, &object);
    return runImpl(&object, func, timeoutInMs, waitMethod);
}

template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1>
R ThreadConcurrentRun::run(ObjClass &object,
                           R (FnClass::*fn)(Param1),
                           const Arg1 &arg1,
                           int timeoutInMs,
                           WaitHelper::WaitMethod waitMethod)
{
    static_assert(std::is_base_of<QObject, ObjClass>::value, "Targetted object must be a QObject");
    static_assert(std::is_base_of<FnClass, ObjClass>::value, "Object must implement given method");
    std::function<R()> func = std::bind(fn, &object, arg1);
    return runImpl(&object, func, timeoutInMs, waitMethod);
}

template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1>
R ThreadConcurrentRun::run(const ObjClass &object,
                           R (FnClass::*fn)(Param1) const,
                           const Arg1 &arg1,
                           int timeoutInMs,
                           WaitHelper::WaitMethod waitMethod)
{
    static_assert(std::is_base_of<QObject, ObjClass>::value, "Targetted object must be a QObject");
    static_assert(std::is_base_of<FnClass, ObjClass>::value, "Object must implement given method");
    std::function<R()> func = std::bind(fn, &object, arg1);
    return runImpl(&object, func, timeoutInMs, waitMethod);
}

template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
         typename Param2, typename Arg2>
R ThreadConcurrentRun::run(ObjClass &object,
                           R (FnClass::*fn)(Param1, Param2),
                           const Arg1 &arg1,
                           const Arg2 &arg2,
                           int timeoutInMs,
                           WaitHelper::WaitMethod waitMethod)
{
    static_assert(std::is_base_of<QObject, ObjClass>::value, "Targetted object must be a QObject");
    static_assert(std::is_base_of<FnClass, ObjClass>::value, "Object must implement given method");
    std::function<R()> func = std::bind(fn, &object, arg1, arg2);
    return runImpl(&object, func, timeoutInMs, waitMethod);
}

template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
         typename Param2, typename Arg2>
R ThreadConcurrentRun::run(const ObjClass &object,
                           R (FnClass::*fn)(Param1, Param2) const,
                           const Arg1 &arg1,
                           const Arg2 &arg2,
                           int timeoutInMs,
                           WaitHelper::WaitMethod waitMethod)
{
    static_assert(std::is_base_of<QObject, ObjClass>::value, "Targetted object must be a QObject");
    static_assert(std::is_base_of<FnClass, ObjClass>::value, "Object must implement given method");
    std::function<R()> func = std::bind(fn, &object, arg1, arg2);
    return runImpl(&object, func, timeoutInMs, waitMethod);
}

template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
         typename Param2, typename Arg2, typename Param3, typename Arg3>
R ThreadConcurrentRun::run(ObjClass &object,
                           R (FnClass::*fn)(Param1, Param2, Param3),
                           const Arg1 &arg1,
                           const Arg2 &arg2,
                           const Arg3 &arg3,
                           int timeoutInMs,
                           WaitHelper::WaitMethod waitMethod)
{
    static_assert(std::is_base_of<QObject, ObjClass>::value, "Targetted object must be a QObject");
    static_assert(std::is_base_of<FnClass, ObjClass>::value, "Object must implement given method");
    std::function<R()> func = std::bind(fn, &object, arg1, arg2, arg3);
    return runImpl(&object, func, timeoutInMs, waitMethod);
}

template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
         typename Param2, typename Arg2,typename Param3, typename Arg3>
R ThreadConcurrentRun::run(const ObjClass &object,
                           R (FnClass::*fn)(Param1, Param2, Param3) const,
                           const Arg1 &arg1,
                           const Arg2 &arg2,
                           const Arg3 &arg3,
                           int timeoutInMs,
                           WaitHelper::WaitMethod waitMethod)
{
    static_assert(std::is_base_of<QObject, ObjClass>::value, "Targetted object must be a QObject");
    static_assert(std::is_base_of<FnClass, ObjClass>::value, "Object must implement given method");
    std::function<R()> func = std::bind(fn, &object, arg1, arg2, arg3);
    return runImpl(&object, func, timeoutInMs, waitMethod);
}

template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
         typename Param2, typename Arg2, typename Param3, typename Arg3,
         typename Param4, typename Arg4>
R ThreadConcurrentRun::run(ObjClass &object,
                           R (FnClass::*fn)(Param1, Param2, Param3, Param4),
                           const Arg1 &arg1,
                           const Arg2 &arg2,
                           const Arg3 &arg3,
                           const Arg4 &arg4,
                           int timeoutInMs,
                           WaitHelper::WaitMethod waitMethod)
{
    static_assert(std::is_base_of<QObject, ObjClass>::value, "Targetted object must be a QObject");
    static_assert(std::is_base_of<FnClass, ObjClass>::value, "Object must implement given method");
    std::function<R()> func = std::bind(fn, &object, arg1, arg2, arg3, arg4);
    return runImpl(&object, func, timeoutInMs, waitMethod);
}

template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
         typename Param2, typename Arg2, typename Param3, typename Arg3,
         typename Param4, typename Arg4>
R ThreadConcurrentRun::run(const ObjClass &object,
                           R (FnClass::*fn)(Param1, Param2, Param3, Param4) const,
                           const Arg1 &arg1,
                           const Arg2 &arg2,
                           const Arg3 &arg3,
                           const Arg4 &arg4,
                           int timeoutInMs,
                           WaitHelper::WaitMethod waitMethod)
{
    static_assert(std::is_base_of<QObject, ObjClass>::value, "Targetted object must be a QObject");
    static_assert(std::is_base_of<FnClass, ObjClass>::value, "Object must implement given method");
    std::function<R()> func = std::bind(fn, &object, arg1, arg2, arg3, arg4);
    return runImpl(&object, func, timeoutInMs, waitMethod);
}

template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
         typename Param2, typename Arg2, typename Param3, typename Arg3,
         typename Param4, typename Arg4, typename Param5, typename Arg5>
R ThreadConcurrentRun::run(ObjClass &object,
                           R (FnClass::*fn)(Param1, Param2, Param3, Param4, Param5),
                           const Arg1 &arg1,
                           const Arg2 &arg2,
                           const Arg3 &arg3,
                           const Arg4 &arg4,
                           const Arg5 &arg5,
                           int timeoutInMs,
                           WaitHelper::WaitMethod waitMethod)
{
    static_assert(std::is_base_of<QObject, ObjClass>::value, "Targetted object must be a QObject");
    static_assert(std::is_base_of<FnClass, ObjClass>::value, "Object must implement given method");
    std::function<R()> func = std::bind(fn, &object, arg1, arg2, arg3, arg4, arg5);
    return runImpl(&object, func, timeoutInMs, waitMethod);
}

template<typename R, typename ObjClass, typename FnClass, typename Param1, typename Arg1,
         typename Param2, typename Arg2, typename Param3, typename Arg3,
         typename Param4, typename Arg4, typename Param5, typename Arg5>
R ThreadConcurrentRun::run(const ObjClass &object,
                           R (FnClass::*fn)(Param1, Param2, Param3, Param4, Param5) const,
                           const Arg1 &arg1,
                           const Arg2 &arg2,
                           const Arg3 &arg3,
                           const Arg4 &arg4,
                           const Arg5 &arg5,
                           int timeoutInMs,
                           WaitHelper::WaitMethod waitMethod)
{
    static_assert(std::is_base_of<QObject, ObjClass>::value, "Targetted object must be a QObject");
    static_assert(std::is_base_of<FnClass, ObjClass>::value, "Object must implement given method");
    std::function<R()> func = std::bind(fn, &object, arg1, arg2, arg3, arg4, arg5);
    return runImpl(&object, func, timeoutInMs, waitMethod);
}

template<typename R>
R ThreadConcurrentRun::runImpl(QObject *object,
                               const std::function<R ()> &func,
                               int timeoutInMs,
                               WaitHelper::WaitMethod waitMethod)
{
    QMutex mutex;
    R returnValue;
    bool valueSet = false;

    auto testIfSet = [&valueSet, &mutex]()
    {
        mutex.lock();
        bool isSet = valueSet;
        mutex.unlock();

        return isSet;
    };

    // Invoke on an object's thread
    QMetaObject::invokeMethod(object, [&valueSet, &returnValue, &mutex, &func]()
    {
        returnValue = func();

        mutex.lock();
        valueSet = true;
        mutex.unlock();
    });

    if(!WaitHelper::pseudoWait(testIfSet, timeoutInMs, waitMethod))
    {
        qWarning() << "A problem occurred when waiting the result of the method";
    }

    return returnValue;
}

template<>
inline void ThreadConcurrentRun::runImpl(QObject *object,
                                         const std::function<void()> &func,
                                         int timeoutInMs,
                                         WaitHelper::WaitMethod waitMethod)
{
    QMutex mutex;
    bool valueSet = false;

    auto testIfSet = [&valueSet, &mutex]()
    {
        mutex.lock();
        bool isSet = valueSet;
        mutex.unlock();

        return isSet;
    };

    // Invoke on an object's thread
    QMetaObject::invokeMethod(object, [&valueSet, &mutex, &func]()
    {
        func();

        mutex.lock();
        valueSet = true;
        mutex.unlock();
    });

    if(!WaitHelper::pseudoWait(testIfSet, timeoutInMs, waitMethod))
    {
        qWarning() << "A problem occurred when waiting the result of the method";
    }
}

template<typename R>
R ThreadConcurrentRun::runImpl(const QObject *object,
                               const std::function<R ()> &func,
                               int timeoutInMs,
                               WaitHelper::WaitMethod waitMethod)
{
    QObject *tmpObject = const_cast<QObject *>(object);
    return runImpl(tmpObject, func, timeoutInMs, waitMethod);
}
template<>
inline void ThreadConcurrentRun::runImpl<void>(const QObject *object,
                                               const std::function<void()> &func,
                                               int timeoutInMs,
                                               WaitHelper::WaitMethod waitMethod)
{
    QObject *tmpObject = const_cast<QObject *>(object);
    return runImpl<void>(tmpObject, func, timeoutInMs, waitMethod);
}
