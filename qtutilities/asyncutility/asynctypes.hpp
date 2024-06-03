// SPDX-FileCopyrightText: 2018 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

/* ------------------------------------------------------------------
 * asynctypes - COPYRIGHT (C) 2018 BMS CIRCUITS
 * ------------------------------------------------------------------
 */

#pragma once

#include <functional>


/*! @brief Definition of useful functions to use with Async methods */
class AsyncTypes
{
    public:
        /*! @brief Callback to call in order to go to the next function/method.
            @note This callback is given in parameter to function/method called
            @param bool True if an error occured */
        typedef std::function<void(bool)> Next;

        /*! @brief Useful function patterns for Async methods
            @note If you want to use a Class method, the context of the function has to be binded
                  before to be given */
        typedef std::function<void(Next)> CallbackFunc;

        /*! @brief Class method pattern for Async methods .
            @note Pattern used for class methods, the method has to be given like this:
                  &Class::method
            @param bool True if an error occured*/
        template<class T>
        using NextMeth = std::function<void(T*, bool)>;

        /*! @brief Class method pattern for Async methods
            @note Pattern used for class methods, the method has to be given like this:
                  &Class::method */
        template<class T>
        using CallbackMeth = std::function<void(T*, Next)>;

    public:
        /*! @brief Cast AsyncNext callback from an instance method a to function with the caller
                   binded
            @param caller The class instance of the method given as callback
            @param callback The class method to cast
            @return A function callback */
        template<class T, class Y=NextMeth<T>>
        static Next castNextCallback(T &caller, Y callback);

        /*! @brief Cast AsyncCallback from an instance method to a function with the caller binded
            @param caller The class instance of the method given as callback
            @param callback The class method to cast
            @return A function callback */
        template<class T, class Z=CallbackMeth<T>>
        static CallbackFunc castMethCallback(T &caller, Z callback);
};

template<class T, class Y>
AsyncTypes::Next AsyncTypes::castNextCallback(T &caller, Y callback)
{
    using namespace std::placeholders;
    return std::bind(callback, &caller, _1);
}

template<class T, class Z>
AsyncTypes::CallbackFunc AsyncTypes::castMethCallback(T &caller, Z callback)
{
    using namespace std::placeholders;
    return std::bind(callback, &caller, _1);
}
