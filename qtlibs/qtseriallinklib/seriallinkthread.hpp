// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "threadutility/basethread.hpp"

#include <QSerialPortInfo>

class SerialLink;


/** @brief The thread linked to the serial link
    @note Each serial port has its own thread, to not be blocking if the writing or reading take
          times */
class SerialLinkThread : public BaseThread
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance */
        explicit SerialLinkThread(QObject *parent = nullptr);

        /** @brief The class destructor */
        virtual ~SerialLinkThread() override;

    public:
        /** @brief Initialize the serial link to work
            @param serialPortInfo The information needed to create the serial port
            @return True if no problem occurred */
        bool initSerialLink(const QSerialPortInfo &serialPortInfo);

        /** @brief Access the serial link created in the thread
            @warning The serial link is created in the run of this thread; therefore, the caller of
                     this method and the object pointer you got with this method aren't in the same
                     thread */
        SerialLink *accessSerialLink() const { return _serialLink; }

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
        SerialLink *_serialLink{nullptr};
        QSerialPortInfo _serialPortInfo;
        bool _valid{false};
};
