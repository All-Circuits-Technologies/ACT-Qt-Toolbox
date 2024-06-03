// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include <QTimer>

#include "ticutility/tictype.hpp"

class TicManager;
class TicTimer;


/** @brief This a TicHandler used to receive tics on a specific frequency */
class TicHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(TicType::Enum ticType READ getTicType WRITE setTicType NOTIFY ticTypeModified)
    Q_PROPERTY(quint8 triggerEveryNthOccurrence
               READ getTriggerEveryNthOccurrence
               WRITE setTriggerEveryNthOccurrence
               NOTIFY nthOccurrenceModified)

    public:
        /** @brief Default class constructor
            @note If you use it, you will have to set the TicType by its setter:
                  @ref TicHandler::setTicType
            @param ticManager The tic manager linked to the handler
            @param parent The instance parent class */
        explicit TicHandler(QObject *parent = nullptr);

        /** @brief Class constructor
            @param ticManager The tic manager linked to the handler
            @param ticType The tic type linked to the handler
            @param triggerEveryNthOccurrence This handler will emit a signal every nth occurrence
                                            (nth is the param here) of the base frequence tic.
                                            For instance:
                                                If you want to receive a signal every 2 seconds,
                                                and you are using the TicType::TwoHz type. You have
                                                to set triggerEveryNthOccurrence to 4.
                                                Because 2Hz => 500ms, 4*500ms = 2s
            @param parent The instance parent class */
        explicit TicHandler(TicManager &ticManager,
                            TicType::Enum ticType,
                            quint8 triggerEveryNthOccurrence,
                            QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~TicHandler() override;

    public:
        /** @brief Get the tic type linked to the handler */
        TicType::Enum getTicType() const { return _ticType; }

        /** @brief Set the tic type linked to the handler
            @note This methods takes care to unregister and register to the right frequency */
        void setTicType(TicType::Enum ticType);

        /** @brief Return the coeff which is used to filter the base frequency occurrences
            @note This allow to receive @ref TicHandler::ticTriggered signal */
        quint8 getTriggerEveryNthOccurrence() const { return _triggerEveryNthOccurrence; }

        /** @brief Set the coeff which is used to filter the base frequency occurrences
            @note This allow to receive @ref TicHandler::ticTriggered signal */
        void setTriggerEveryNthOccurrence(quint8 triggerEveryNthOccurrence);

    signals:
        /** @brief Emitted when the tic type is modified */
        void ticTypeModified(TicType::Enum newTicType);

        /** @brief Emitted when the nth occurrence is modified */
        void nthOccurrenceModified(quint8 triggerEveryNthOccurrence);

        /** @brief Emitted when a tic is raised
            @param occurrence This value is the occurrence returned by the TicTimer divided by the
                              @ref _triggerEveryNthOccurrence.
                              This allows to have a counter on the TicTriggered occurrence and all
                              the handler with the same frequency and
                              @ref _triggerEveryNthOccurrence value have the same occurrence
                              value */
        void ticTriggered(quint32 occurrence);

    private slots:
        /** @brief Called at each tic of the linked @ref TicTimer
            @note This method manages the filtering
            @param occurrence The tic occurrence of the linked TicTimer */
        void onTicCalled(quint32 occurrence);

    private:
        /** @brief Utility method to connect or disconnect the handler with a given TicTimer
            @param timer The timer linked to the targetted frequency
            @param toConnect True to connect to the timer signals */
        void manageTimerConnection(const TicTimer *timer, bool toConnect);

        /** @brief Utility method to register/unregister the handler with the manager
            @note This method calls the @ref registerHandler and @ref unregisterHandler methods
            @param registerHand True to register the handler into the TicManger */
        void manageCurrentTypeRegistration(bool registerHand);

    private:
        /** @brief Register the handler with the TicManager
            @param type The TicType to register with
            @return The TicTimer linked to the wanted frequency */
        const TicTimer *registerHandler(TicType::Enum type);

        /** @brief Unregister the handler with the TicManager
            @param type The TicType to unregister with */
        void unregisterHandler(TicType::Enum type);

    private:
        TicType::Enum _ticType{TicType::Unknown};
        quint8 _triggerEveryNthOccurrence;

        TicManager *_ticManager{nullptr};

        const TicTimer *_linkedTimer{nullptr};
};
