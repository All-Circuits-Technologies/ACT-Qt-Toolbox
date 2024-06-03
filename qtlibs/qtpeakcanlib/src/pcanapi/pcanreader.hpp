// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QCanBusFrame>

#include "src/pcanapi/pcanbusitf.hpp"

class QMutex;


/** @brief This class contains useful methods to continuously read the CAN frames received
    @note You can only call @ref readMessages once.
    @note If you want to restart the reading process after having call @ref cancelReading, you
          have to create a new instance */
class PCanReader : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param canBusIntf The CAN Bus interface key
            @param isCanFd Say if we use the CAN FD to read messages
            @param parent The class parent */
        explicit PCanReader(PCanBusItf::Enum canBusItf, bool isCanFd, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~PCanReader() override;

    public:
        /** @brief Wait for the read process end
            @note This method waits the cancellation of the read process before returning
            @param timeoutInMs The waiting timeout
            @return True if the process is ended, false if the timeout raised before the end of
                    process */
        bool waitForProcessEnd(int timeoutInMs = -1);

        /** @brief This cancels the current reading process */
        void cancelReading();

    public slots:
        /** @brief This method starts the reading process */
        void readMessages();

    private:
        /** @brief Manage the CAN message reading process
            @return True if no problem occurred */
        bool manageMsgReading();

        /** @brief The method processes the received messages
            @return True if no problem occurred */
        bool processReceivedMessages();

        /** @brief The method processes the CAN message received
            @return The PEAK Can lib error code of the process */
        quint32 processCanMessages();

        /** @brief The method processes the CAN FD message received
            @return The PEAK Can lib error code of the process */
        quint32 processCanFdMessages();

        /** @brief Helpful method to emit the frames queued in @ref _framesQueue, if the
                   @ref MaxFramesNbToEmit limit is raised
            @note This is useful to not emit the CAN frame one by one and not a big list if a lot is
                  received. But to group the read frames by packets
            @param forceSendAll The method doesn't check the number of queued frames and send all */
        void processFramesQueueEmitting(bool forceSendAll = false);

    signals:
        /** @brief Emitted when new frames are received
            @param frames The received frame */
        void framesReceived(QVector<QCanBusFrame> frames);

    private:
        /** @brief Test if it's ok to continue the message processing thanks to the @ref errorStatus
                   given.
            @param errorStatus The error status to test
            @return True if we can continue the message process */
        static bool isItOkToContinueMessageProcessing(quint32 errorStatus);

        /** @brief Test the @ref errorStatus given and says if it's a fatal error (which has to stop
                   the read process)
            @param errorStatus The error status to test
            @return True if the error status given is a fatal error */
        static bool isReadErrorFatal(quint32 errorStatus);

    private:
        /** @brief This defines the read timeout when waiting for a read event */
        static const constexpr quint32 ReadWaitingTimeoutInMs = 100;

        /** @brief This is max frames number to have before emitting a signal */
        static const constexpr int MaxFramesNbToEmit = 10;

        /** @brief This is the coefficient to use in order to manage the millisecond overflow */
        static const constexpr quint64 MillisOverflowCoeff = Q_UINT64_C(0x100000000);

        /** @brief This is the coefficient to use in order to transform milli to micro */
        static const constexpr quint64 MilliToMicroCoeff = Q_UINT64_C(1000);

    private:
        bool _cancel{false};
        bool _isCanFd{false};
        PCanBusItf::Enum _canBusItf;
        QMutex *_readMutex{nullptr};
        QVector<QCanBusFrame> _framesQueue{};
};
