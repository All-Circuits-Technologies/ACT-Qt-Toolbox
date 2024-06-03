// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QSharedPointer>

class AVisacom;


/** @brief This class is used to manage the asynchronous communication, of the VISA
           communication */
class VisaAsyncManager : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        explicit VisaAsyncManager(QObject *parent = nullptr);

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

    private:
        /** @brief Call this method to read a response from a device
            @param cmd The cmd to write
            @return True if no problem occurs */
        bool asyncReadPriv(QByteArray &ouput);

    signals:
        /** @brief Emitted when a new message is received from device
            @param buffer the buffer containing the message */
        void messageReceived(const QByteArray &buffer);

        /** @brief Emitted when an error occured during communication */
        void errorOccured();

    private slots:
        /** @brief Call the synchronous read method
            @note This method has to be called outside of the write methods, to be totally useful */
        void waitResult();

    private:
        AVisacom* _avisacom {nullptr};
};
