// SPDX-FileCopyrightText: 2018 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

/* ------------------------------------------------------------------
 * asyncwaterfallhelper.hpp - COPYRIGHT (C) 2018 BMS CIRCUITS
 * ------------------------------------------------------------------
 */

#pragma once

#include <QObject>

#include <QVector>

#include "asynctypes.hpp"


/*! @brief Useful with asynchronous methods or functions to call in waterfall effect */
class AsyncWaterfallHelper : public QObject
{
    Q_OBJECT

    private:
        /*! @brief Private class constructor
            @param callbacks The callbacks to call in the order given
            @param finalCallback The final callback to call at the end of process */
        explicit AsyncWaterfallHelper(QList<AsyncTypes::CallbackFunc> callbacks,
                                      AsyncTypes::Next finalCallback,
                                      QObject *parent = nullptr);

    public:
        /*! @brief Call the function/methods in the order given.
            @note In order to call the next function/method, the developper has to call the
                  AsyncNext callback given in parameter.
            @note If an error occured in a function/method given and you want to go to the
                  final callback (without passing in the others function/methods given).
                  Call the AsyncNext callback with true as parameter.
            @note The final function/method is called when all the others functions/methods have
                  been called. In that case, the parameter value of the final function/method is
                  equal to false for: "no error during process"
            @note The final function/method is called if an error has been highlighted by one of the
                  method/function. In that case, no others functions/methods of the list will be
                  called and the parameter value of the final function/method is equal to true for:
                  "an error occured during process"
            @attention Note for developpers: in the methods/functions you create and give as
                       callbacks, it's important to call, for each function/method end, the
                       AsyncNext callback given
            @param caller The class instance of all the methods given in the callbacks list
                          parameter and also the class instance of the method given as finalCallback
            @param callbacks The class methods called one after the other
            @param finalCallback The class method called at the end of process */
        template<class T, class Z=AsyncTypes::CallbackMeth<T>, class Y=AsyncTypes::NextMeth<T>>
        static void manage(T &caller, const QList<Z> &callbacks, Y finalCallback);

        /*! @brief Call the function/methods in the order given.
            @note In order to call the next function/method, the developper has to call the
                  AsyncNext callback given in parameter.
            @note If an error occured in a function/method given and you want to go to the
                  final callback (without passing in the others function/methods given).
                  Call the AsyncNext callback with true as parameter.
            @note The final function/method is called when all the others functions/methods have
                  been called. In that case, the parameter value of the final function/method is
                  equal to false for: "no error during process"
            @note The final function/method is called if an error has been highlighted by one of the
                  method/function. In that case, no others functions/methods of the list will be
                  called and the parameter value of the final function/method is equal to true for:
                  "an error occured during process"
            @attention Note for developpers: in the methods/functions you create and give as
                       callbacks, it's important to call, for each function/method end, the
                       AsyncNext callback given
            @param caller The class instance of all the methods given in the callbacks list
                          parameter
            @param callbacks The class methods called one after the other
            @param finalCallback The function called at the end of process */
        template<class T, class Z=AsyncTypes::CallbackMeth<T>>
        static void manage(T &caller,
                           const QList<Z> &callbacks,
                           AsyncTypes::Next finalCallback);

        /*! @brief Call the function/methods in the order given.
            @note In order to call the next function/method, the developper has to call the
                  AsyncNext callback given in parameter.
            @note If an error occured in a function/method given and you want to go to the
                  final callback (without passing in the others function/methods given).
                  Call the AsyncNext callback with true as parameter.
            @note The final function/method is called when all the others functions/methods have
                  been called. In that case, the parameter value of the final function/method is
                  equal to false for: "no error during process"
            @note The final function/method is called if an error has been highlighted by one of the
                  method/function. In that case, no others functions/methods of the list will be
                  called and the parameter value of the final function/method is equal to true for:
                  "an error occured during process"
            @attention Note for developpers: in the methods/functions you create and give as
                       callbacks, it's important to call, for each function/method end, the
                       AsyncNext callback given
            @param caller The class instance of the method given as finalCallback
            @param callbacks The functions called one after the other
            @param finalCallback The method called at the end of process */
        template<class T, class Y=AsyncTypes::NextMeth<T>>
        static void manage(T &caller,
                           const QList<AsyncTypes::CallbackFunc> &callbacks,
                           Y finalCallback);

        /*! @brief Call the function/methods in the order given.
            @note In order to call the next function/method, the developper has to call the
                  AsyncNext callback given in parameter.
            @note If an error occured in a function/method given and you want to go to the
                  final callback (without passing in the others function/methods given).
                  Call the AsyncNext callback with true as parameter.
            @note The final function/method is called when all the others functions/methods have
                  been called. In that case, the parameter value of the final function/method is
                  equal to false for: "no error during process"
            @note The final function/method is called if an error has been highlighted by one of the
                  method/function. In that case, no others functions/methods of the list will be
                  called and the parameter value of the final function/method is equal to true for:
                  "an error occured during process"
            @attention Note for developpers: in the methods/functions you create and give as
                       callbacks, it's important to call, for each function/method end, the
                       AsyncNext callback given
            @param callbacks The functions called one after the other
            @param finalCallback The function called at the end of process */
        static void manage(const QList<AsyncTypes::CallbackFunc> &callbacks,
                           AsyncTypes::Next finalCallback);

    private:
        /*! @brief Method given to callback and so called at the end of callback
            @note emit nextSte psignal
            @param anErrorOccured True if an error occured in callback */
        void next(bool anErrorOccured);

    private slots:
        /*! @brief Connected to nextStep signal and call the next callback or the final callback.
            @note The final function/method is called when all the others functions/methods have
                  been called. In that case, the parameter value of the final function/method is
                  equal to false for: "no error during process"
            @note The final function/method is called if an error has been highlighted by one of the
                  method/function. In that case, no others functions/methods of the list will be
                  called and the parameter value of the final function/method is equal to true for:
                  "an error occured during process"
            @param enErrorOccured True fi an error occured in process*/
        void manageAsync(bool anErrorOccured);

    signals:
        /*! @brief Emitted in order to call the next callback
            @param anErrorOccured True if an error occured in process */
        void nextStep(bool anErrorOccured);

    private:
        const QList<AsyncTypes::CallbackFunc> _callbacks;
        int _callbacksIndex{0};
        const AsyncTypes::Next _finalCallback;
};

template<class T, class Z, class Y>
void AsyncWaterfallHelper::manage(T &caller, const QList<Z> &callbacks, Y finalCallback)
{
    manage(caller, callbacks, AsyncTypes::castNextCallback(caller, finalCallback));
}

template<class T, class Z>
void AsyncWaterfallHelper::manage(T &caller,
                                  const QList<Z> &callbacks,
                                  AsyncTypes::Next finalCallback)
{
    QList<AsyncTypes::CallbackFunc> privCallbacks;

    for(int idx = 0 ; idx < callbacks.length() ; idx++)
    {
        privCallbacks.append(AsyncTypes::castMethCallback(caller, callbacks[idx]));
    }

    manage(privCallbacks, finalCallback);
}

template<class T, class Y>
void AsyncWaterfallHelper::manage(T &caller,
                                  const QList<AsyncTypes::CallbackFunc> &callbacks,
                                  Y finalCallback)
{
    manage(callbacks, AsyncTypes::castNextCallback(caller, finalCallback));
}
