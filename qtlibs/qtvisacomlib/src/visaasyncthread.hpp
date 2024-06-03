// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QThread>

#include <QSharedPointer>

class ThreadRunningHelper;
class VisaAsyncManager;
class AVisacom;


/** @brief Useful thread used to manage the synchronous reading and writing in an another thread to
           not block the current sequence thread */
class VisaAsyncThread : public QThread
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        explicit VisaAsyncThread(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~VisaAsyncThread() override;

    public slots:
        /** @brief Call to stop the thread
            @return True if no problem occurs */
        bool stopThread();

        /** @brief Call to stop the thread and then delete properly the object
            @return True if no problem occurs */
        bool stopAndDeleteThread();

    public:
        /** @brief Set the VISA com interface to used inder to communicate with the instrument */
        void setVisacom(AVisacom *avisacom) { _avisacom = avisacom; }

        /** @brief Call this method to write cmd to the instrument without waiting a response
            @param cmd The cmd to write
            @return True if no problem occurs */
        bool asyncWrite(const QByteArray &cmd);

        /** @brief Call this method to write the cmd and wait a response from device
            @param cmd The cmds to write
            @return True if no problem occurs */
        bool asyncQuery(const QByteArray &cmd);

    signals:
        /** @brief Emitted when a new message is received from device
            @param buffer the buffer containing the message */
        void messageReceived(const QByteArray &buffer);

        /** @brief Emitted when an error occured during communication */
        void errorOccured();

    protected:
        /** @brief Thread run method */
        virtual void run() override;

    private:
        VisaAsyncManager *_visaAsyncManager {nullptr};

        ThreadRunningHelper * _threadRunningHelper {nullptr};

        AVisacom *_avisacom {nullptr};
};
