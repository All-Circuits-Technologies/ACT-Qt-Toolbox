// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "threadutility/basethread.hpp"

#include <QObject>

#include "src/pcanapi/pcanapi.hpp"

class PCanReader;


/** @brief This is the Thread used to read CAN frames from the PEAK CAN lib.
    @note The CAN frames reading has its own thread because the reading process is blocking */
class PCanReadThread : public BaseThread
{
    Q_OBJECT

    public:
    /** @brief Class constructor
            @param canBusIntf The CAN Bus interface key
            @param isCanFd Say if we use the CAN FD to read messages
            @param parent The class parent */
        explicit PCanReadThread(PCanBusItf::Enum canBusItf,
                                bool isCanFd,
                                QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~PCanReadThread() override;

    public slots:
        /** @brief Call to stop the thread
            @return True if no problem occurs */
        virtual bool stopThread() override;

    protected:
        /** @copydoc BaseThread::run */
        virtual void run() override;

    signals:
        /** @brief Emitted when new frames are received
            @param frames The received frame */
        void framesReceived(const QVector<QCanBusFrame> &frames);

    private:
        PCanBusItf::Enum _canBusItf;
        bool _isCanFd;
        PCanReader *_reader{nullptr};
};
