// SPDX-FileCopyrightText: 2018 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <functional>


/*! @brief This namespace brings useful wait tools */
class WaitHelper
{
    public:
        /*! @brief Because there are multiple ways to wait for a condition to be true, this enum
                   contains the different method which can be used*/
        enum WaitMethod
        {
            UseLocalEventLoop,  /*!< @brief We use the local event loop wait method, which is faster
                                            and doesn't freeze the current thread. */
            UseThreadSleep,     /*!< @brief We use the thread sleep wait method, which is the more
                                            portable solution */
        };

    public:
        /*! @brief Wait until a boolean reference becomes true.
            This method makes event loop working and regulary read given reference to test for
            completeness.
            User is expected to alter referenced boolean upon completion of a task.
            He can alter it from inside a slot to be safe against reading a mutating variable,
            but since this variable is actually a boolean, we only look for exact "true" state
            which makes this threat quite irrelevent.
            @param taskDone A reference to a boolean which will triger the end of the wait when true
            @param timeToWaitInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return Value of taskDone at the end of this call, used to provide the result */
        static bool pseudoWait(const bool &taskDone,
                               int timeToWaitInMs = -1,
                               WaitMethod waitMethod = WaitMethod::UseLocalEventLoop);

        /*! @brief Wait until testFunc returns true, or timeout expires.
            This method makes event loop working and regulary executes testFunc to test for
            completeness.
            User will likely want to alter a member of its class from slot, and test it in the
            lambda.
            @param testFunc A lambda which returns true to stop waiting
            @param timeToWaitInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return A final testFunc call is used to provide the result */
        static bool pseudoWait(const std::function<bool ()> &testFunc,
                               int timeToWaitInMs = -1,
                               WaitMethod waitMethod = WaitMethod::UseLocalEventLoop);

        /*! @brief Wait until the class instance method given returns true, or timeout expires.
            This method makes event loop working and regulary executes testFunc to test for
            completeness.
            User will likely want to alter a member of its class from slot, and test it in the
            lambda.
            @note Useful to call const class method
            @param caller The class instance caller of the method given
            @param fn A class method of the caller given, which returns true to stop waiting
            @param timeToWaitInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return A final method call is used to provide the result */
        template<typename Class>
        static bool pseudoWait(const Class &caller,
                               bool (Class::*fn)() const,
                               int timeToWaitInMs = -1,
                               WaitMethod waitMethod = WaitMethod::UseLocalEventLoop);

        /*! @brief Wait until the class instance method given returns true, or timeout expires.
            This method makes event loop working and regulary executes testFunc to test for
            completeness.
            User will likely want to alter a member of its class from slot, and test it in the
            lambda.
            @note Useful to call non-const class method
            @param caller The class instance caller of the method given
            @param fn A class method of the caller given, which returns true to stop waiting
            @param timeToWaitInMs Maximum wait duration in milliseconds (-1 means infinite)
            @param waitMethod This allow to choose the wait method you want to use
            @return A final method call is used to provide the result */
        template<typename Class>
        static bool pseudoWait(Class &caller,
                               bool (Class::*fn)(),
                               int timeToWaitInMs = -1,
                               WaitMethod waitMethod = WaitMethod::UseLocalEventLoop);

        /*! @brief Wait until the timeout expires
            @note If you are sure to give a positive number to @ref timeToWaitInMs, the method
                  will always return true.
            @param timeToWaitInMs Maximum wait duration in milliseconds, the time given must be
                                  positive
            @param waitMethod This allow to choose the wait method you want to use
            @return True if no problem occurred */
        static bool pseudoSleep(int timeToWaitInMs,
                                WaitMethod waitMethod = WaitMethod::UseLocalEventLoop);
    private:
        /** @brief Wait the @ref testFunc to return true, or the timeout to rise
            @note This method uses a local event loop, which is the fastest and cleanest way to do
                  it.
                  Indeed: we don't block the thread and let the events live.
            @param testFunc A lambda which returns true to stop waiting,
                            If null, it means that we just wait the @ref timeToWaitInMs given
            @param timeToWaitInMs Maximum wait duration in milliseconds (-1 means infinite)
                                  If @ref testFunc is null, no infinite waiting can be asked
            @return True if no problem occurred */
        static bool eventLoopPseudoWait(const std::function<bool ()> *testFunc,
                                        int timeToWaitInMs = -1);

        /** @brief Wait the @ref testFunc to return true, or the timeout to rise
            @note This method use thread sleeps, which freeze the thread and its event loop.
                  Therefore, the events may take time to be processed and so the method test
                  function to return true.
                  However, it's the more portable solution because it doesn't depend of Qt events
            @param testFunc A lambda which returns true to stop waiting
                            If null, it means that we just wait the @ref timeToWaitInMs given
            @param timeToWaitInMs Maximum wait duration in milliseconds (-1 means infinite)
                                  If @ref testFunc is null, no infinite waiting can be asked
            @return True if no problem occurred */
        static bool sleepPseudoWait(const std::function<bool ()> *testFunc,
                                    int timeToWaitInMs = -1);

    private:
        /** @brief Interval between end-of-wait checks.
            @note Depending on wait method, this is either a real thread sleep or a background
                  check interval */
        static constexpr const int microSleepsDurationMs {30};
};

template<typename Class>
bool WaitHelper::pseudoWait(const Class &caller,
                            bool (Class::*fn)() const,
                            int timeToWaitInMs,
                            WaitMethod waitMethod)
{
    std::function<bool()> func = std::bind(fn, &caller);
    return pseudoWait(func, timeToWaitInMs, waitMethod);
}

template<typename Class>
bool WaitHelper::pseudoWait(Class &caller,
                            bool (Class::*fn)(),
                            int timeToWaitInMs,
                            WaitMethod waitMethod)
{
    std::function<bool()> func = std::bind(fn, &caller);
    return pseudoWait(func, timeToWaitInMs, waitMethod);
}
