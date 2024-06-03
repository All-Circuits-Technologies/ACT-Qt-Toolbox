// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "threadutility/basethread.hpp"

#include <QObject>

#include "src/models/candeviceconfig.hpp"

class CanDevice;


/** @brief The thread linked to the CAN device communication
    @note Each CAN device has its own thread, to not be blocking if the writing or reading take
          times */
class CanDeviceThread : public BaseThread
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent class */
        explicit CanDeviceThread(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~CanDeviceThread() override;

    public:
        /** @brief Create and initialize the @ref CanDevice
            @param config The information needed to create the CanDevice
            @return True if no problem occurred */
        bool createCanDevice(const CanDeviceConfig &config);

        /** @brief Access the @ref CanDevice created in the thread
            @warning The CanDevice is created in the run of this thread; therefore, the caller of
                     this method and the object pointer you got with this method aren't in the same
                     thread */
        CanDevice *accessCanDevice() const { return _canDevice; }

        /** @brief Test if the thread is valid (if it's started and not stopped)
            @return True if the object is valid */
        bool isValid() const { return _valid; }

    public slots:
        /** @brief Call to stop the thread
                @return True if no problem occurs */
        virtual bool stopThread() override;

    protected:
        /** @see BaseThread::run */
        virtual void run() override;

    private:
        CanDevice *_canDevice{nullptr};
        CanDeviceConfig _canConfig;
        bool _valid{false};
};
