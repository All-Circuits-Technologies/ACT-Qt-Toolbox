// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QDebug>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QMutex>

#include "visacomglobal.hpp"

class VisacomManager;
class VisaAsyncThread;

/** @brief Useful class used to manage multiprotocol communication such as Gpib / Serial / USB */
class VISACOM_EXPORT AVisacom: public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param interface the interface needed to address the correct instrument
            @param visaManager The visa com manager
            @param parent The parent class */
        explicit AVisacom(const QString &interfaceId,
                          VisacomManager &visaManager,
                          QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AVisacom() override;

        /** @brief Open visa instrument session
            @return return a boolean value. if false => An error occured */
        virtual bool open();

        /** @brief Write and send message to instrument using visa
            @note This function use Latin1 standard
            @note Use setTimeout() to define a new timeout value, otherwise the timeout = 2000ms
            @param command the message to send to the instrument
            @return return a boolean value. if false => An error occured */
        bool write(const QString &command);

        /** @brief Write and send message to instrument using visa
            @note Use setTimeout() to define a new timeout value, otherwise the timeout = 2000ms
            @param command the message to send to the instrument
            @return return a boolean value. if false => An error occured */
        bool write(const QByteArray &command);

        /** @brief Write and send message list to instrument using visa
            @note This function use Latin1 standard
            @note Use setTimeout() to define a new timeout value, otherwise the timeout = 2000ms
            @param command the message list to send to the instrument
            @return return a boolean value. if false => An error occured */
        bool write(const QVector<QString> &command);

        /** @brief Read message from instrument using visa
            @note This function use Latin1 standard
            @note Use setTimeout() to define a new timeout value, otherwise the timeout = 2000ms
            @param outputBuffer the buffer to read / store the instrument message
            @return return a boolean value. if false => An error occured */
        virtual bool read(QString &outputBuffer);

        /** @brief Read message from instrument using visa
            @note Use setTimeout() to define a new timeout value, otherwise the timeout = 2000ms
            @param outputBuffer the buffer to read / store the instrument message
            @return return a boolean value. if false => An error occured */
        virtual bool read(QByteArray &outputBuffer);

        /** @brief Close visa instrument session
            @return return a boolean value. if false => An error occured */
        virtual bool close();

        /** @brief Clear visa instrument session (reset read & write buffer)
            @return return a boolean value. if false => An error occured */
        bool clear();

        /** @brief Set the timeout value for visa communication

            @param timeout the timeout parameter
            @note If setTimeout() Note used, default timeout = 2000ms
            @param timeout the timeout parameter
            @return return a boolean value. if false => An error occured */
        virtual bool setTimeout(qint32 timeout);

        /** @brief Return the timeout value for gpib communication
            @note If setTimeout() Note used, default timeout = 2000ms
            @param timeout the timeout value
            @return return a boolean value. if false => An error occured */
        bool getTimeout(quint32 &timeout);

        /** @brief return if a session is currently open */
        bool isOpen() const { return _isOpen; }

        /** @brief Get the token linked to visa com */
        const QString &getInterfaceId() const { return _interfaceId; }

    //Public Asynchronous Function
    public:
        /** @brief Call this method to write cmd to the instrument without waiting a response
            @note Use setTimeout() to define a new timeout value, otherwise the timeout = 2000ms
            @param cmd The QByteArray cmd to write
            @return True if no problem occurs */
        bool asyncWrite(const QByteArray &cmd);

        /** @brief Call this method to write cmd to the instrument without waiting a response
            @note Use setTimeout() to define a new timeout value, otherwise the timeout = 2000ms
            @param cmd The QString cmd to write
            @return True if no problem occurs */
        bool asyncWrite(const QString &cmd);

        /** @brief Call this method to write the cmd and wait a response from device
            @note Use setTimeout() to define a new timeout value, otherwise the timeout = 2000ms
            @param cmd The QByteArray cmd to write
            @return True if no problem occurs */
        virtual bool asyncQuery(const QByteArray &cmd);

        /** @brief Call this method to write the cmd and wait a response from device
            @note Use setTimeout() to define a new timeout value, otherwise the timeout = 2000ms
            @param cmd The QString cmd to write
            @return True if no problem occurs */
        virtual bool asyncQuery(const QString &cmd);

    protected:
        /** @brief set attribute to visa corresponding to instrument configuation
            @param attr the attribute to modify in visa config
            @param value the attribute value to modify
            @return return a boolean value. if false => An error occured */
        bool setAttribute(quint32 attr, qint32 value);

        /** @brief set attribute to visa corresponding to instrument configuation
            @param attr the attribute to modify in visa config
            @param value the attribute value to modify
            @return return a boolean value. if false => An error occured */
        bool setAttribute(quint32 attr, quint64 value);

        /** @brief set attribute to visa corresponding to instrument configuation
            @param attr the attribute to modify in visa config
            @param value the attribute value to modify
            @return return a boolean value. if false => An error occured */
        bool setAttribute(quint32 attr, quint32 value);

        /** @brief set attribute to visa corresponding to instrument configuation
            @param attr the attribute to modify in visa config
            @param value the attribute value to modify
            @return return a boolean value. if false => An error occured */
        bool setAttribute(quint32 attr, quint16 value);

        /** @brief set attribute to visa corresponding to instrument configuation
            @param attr the attribute to modify in visa config
            @param value the attribute value to modify
            @return return a boolean value. if false => An error occured */
        bool setAttribute(quint32 attr, bool value);

        /** @brief get attribute from visa corresponding to instrument configuation
            @param attr the attribute to modify in visa config
            @param value the attribute value to modify
            @return return a boolean value. if false => An error occured */
        bool getAttribute(quint32 attr, quint64 &output);

        /** @brief get attribute from corresponding to instrument configuation
            @param attr the attribute to modify in visa config
            @param value the attribute value to modify
            @return return a boolean value. if false => An error occured */
        bool getAttribute(quint32 attr, quint32 &output);

        /** @brief get attribute from corresponding to instrument configuation
            @param attr the attribute to modify in visa config
            @param value the attribute value to modify
            @return return a boolean value. if false => An error occured */
        bool getAttribute(quint32 attr, quint16 &output);

        /** @brief get attribute from corresponding to instrument configuation
            @param attr the attribute to modify in visa config
            @param value the attribute value to modify
            @return return a boolean value. if false => An error occured */
        bool getAttribute(quint32 attr, bool &output);

        /** @brief return the instrument session*/
        unsigned long getInstrumentSession();

    private:
        /** @brief set attribute to visa corresponding to instrument configuation
            @param attr the attribute to modify in visa config
            @param value the attribute value to modify
            @return return a boolean value. if false => An error occured */
        bool getAttributePriv(quint32 attr, void *output);

    protected:
        /** @brief Write and send message to instrument using visa.
            @note Use setTimeout() to define a new timeout value, otherwise the timeout = 2000ms
            @note This function is a generic write function and don't uses mutex.
            @param command the message to send to the instrument.
            @return return a boolean value. if false => An error occured */
        bool writePriv(const QByteArray &command);

        /** @brief Read message from instrument using visa
            @note Use setTimeout() to define a new timeout value, otherwise the timeout = 2000ms
            @note This function is a generic read function and don't uses mutex.
            @param outputBuffer the buffer to read / store the instrument message
            @return return a boolean value. if false => An error occured */
        bool readPriv(QByteArray &outputBuffer);

        /** @brief Lock mutex before write or read function
            @note Must be used in public write / read functions
            @return return a boolean value. if false => An error occured */
        bool lockMutex();

        /** @brief Unlock mutex after write or read function
            @note Must be used in public write / read functions */
        void unlockMutex();

    signals:
        /** @brief Emitted when a new message is received from device
            @param buffer the buffer containing the message */
        void messageReceived(const QByteArray &buffer);

        /** @brief Emitted when an error occured during communication */
        void errorOccured();

    public:
        static const constexpr int descriptionBufferSize = 256;

    private:
        static const constexpr unsigned long bufferSize = 1024;

        static const constexpr char *lineFeed = "\n";

        static const constexpr qint32 mutexTimeout = 5000;

    private:
        /** @brief VisacomManager instance */
        VisacomManager &_visaManager;

        /** @brief Visa ressource manager session for default visa communication */
        /*ViSession*/ unsigned long  _defaultRM;

        /** @brief Visa instrument session */
        /*ViSession*/ unsigned long  _instr{0};

        /** @brief Visa function status (status<0 = function error) */
        /*ViStatus*/  qint32         _status{0};

        /** @brief Flag set to true when init()  | set to false when close() */
        bool _isOpen = false;

        /** @brief Interface ID used as token and used to open instrument session */
        QString _interfaceId;

        /** @brief Mutex used to manage access to write and read function */
        QMutex _mutex;

        /** @brief Thread used to manage asynchronous communication */
        VisaAsyncThread *_visaAsyncThread{nullptr};
};

