// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QHash>
#include <QTimer>

#include "managersutility/abstractmanager.hpp"
#include "ticutility/tictimer.hpp"
#include "ticutility/tictype.hpp"

class TicHandler;
class TicHandlerAccessKey;


/** @brief This manager is used to generate tic at a specific frequency
    @note Tics are only generated when someone has a handler on the frequency (to avoid to have
          useless running timer)
    @note Every handler on a specific frequency uses the same timer
    @note Each frequency is independent from others, it means that they aren't synchronized.
          Therefore a gap may appear between the frequencies tics */
class TicManager : public AbstractManager
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent class instance */
        explicit TicManager(QObject *parent = nullptr);

    public:
        /** @brief Create the tic handler wanted
            @note The handler listens on a specific frequency, it's possible to use multiple of the
                  chosen period using the @ref triggerEveryNthOccurrence param.
                  For instance:
                    If you want to receive a signal every 2 seconds, we have to register to the
                    TicType::TwoHz type and then set triggerEveryNthOccurrence to 4.
                    Because 2Hz => 500ms, 4*500ms = 2s
            @param ticType The type of tic to create a handler on
            @param triggerEveryNthOccurrence This param allows you to set a multiple for the chosen
                                            period
            @param parent The TicHandler parent instance
            @return The TicHandler created */
        Q_INVOKABLE TicHandler *createTicHandler(TicType::Enum ticType,
                                                 quint8 triggerEveryNthOccurrence = 1,
                                                 QObject *parent = nullptr);

    public:
        /** @brief This method allows TicHandler instances to register them to the TicManager
            @attention To correctly manage the TicTimer lifes, the TicHandlers have to call this
                       method when they want to use a specific tic type (and received tic)
            @see TicManager::releaseOne
            @param key This access key prevents other classes than @ref TicHandler to use this
                       method
            @param type The type of the handler which is registering
            @return The tic timer linked to the TicType given */
        const TicTimer &takeOne(const TicHandlerAccessKey &key, TicType::Enum type);

        /** @brief This method allows TicHandler instance to unregister them from the TicManager
            @attention To correctly manage the TicTimer lifes, the TicHandlers have to call this
                       method when they don't want to listen a frequency anymore
            @see TicManager::releaseOne
            @param key This access key prevents other classes than @ref TicHandler to use this
                       method
            @param type The type of the handler which is unregistering */
        void releaseOne(const TicHandlerAccessKey &key, TicType::Enum type);

    public:
        /** @brief Register meta type linked to this enum */
        static void registerMetaTypes();

    private:
        /** @brief Informations about a TicType */
        struct TicTypeInfos
        {
            TicTimer *timer{nullptr};   //!< @brief The timer linked to the TicType
            quint32 handlersNb{0};      /*!< @brief The currently number of handlers which are using
                                                    the frequency */
        };

    private:
        /** @brief Called when the first handler wants to listen to a specific frequency
            @note This method tries to begin to start the timer at 0ms of the next second.
                  Nevertheless, the timer isn't enough precise to begin at the exact 0 (and there is
                  also operations which takes time)
            @param infos The TicTypeInfos linked to the frequency to start */
        void atFirstHandler(TicTypeInfos &infos);

        /** @brief Called when there is no more handler listening a specific frequency
            @note This allows to stop the timer (if nobody is listening)
            @param infos The TicTypeInfos linked to the frequency to stop */
        void whenNoMoreHandlers(TicTypeInfos &infos);

        /** @brief If the wanted TicType is not known in the _ticTypeInfos QHash, this creates and
                   insert the element on the map.
                   If the wanted TicType is already contained in the _ticTypeInfos QHash, this
                   simply returns the value
            @param ticType The tic type to get or create
            @return The TicTypeInfos linked to the TicType */
        TicTypeInfos &getOrCreateTypeInfo(TicType::Enum ticType);

    private:
        QHash<TicType::Enum, TicTypeInfos> _ticTypeInfos;
};
