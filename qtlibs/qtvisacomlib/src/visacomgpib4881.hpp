// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "visacomgpib.hpp"

#include <QDebug>
#include <QString>
#include <QTimer>
#include <QElapsedTimer>


/** @brief Useful class used to manage gpib 488.1 asynchronous communication
    @note Class based on the principle of callback raised when an event is detected
    @note VI_EVENT_SERVICE_REQUEST:
          When a query request is emitted, the device returns a service request command.
          When this command is issued, it triggers the VI_EVENT_SERVICE_REQUEST event.
    @note VI_EVENT_IO_COMPLETION:
          When an asynchronous operation is successfully performed,
          it triggers the VI_EVENT_IO_COMPLETION event. */
class VISACOM_EXPORT VisacomGpib4881 : public VisacomGpib
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param interface the interface needed to address the correct instrument
            @param visaManager The visa com manager
            @param parent The parent class */
        explicit VisacomGpib4881(const QString &interfaceId,
                                 VisacomManager &visaManager,
                                 QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~VisacomGpib4881() override;

        /** @brief Open visa instrument session
            @return return a boolean value. if false => An error occured */
        virtual bool open() override;

        /** @brief Synchronous read function can't be used with visacomgpib4881 class
            @param outputBuffer the buffer to read / store the instrument message
            @return return false with visacomgpib4881 */
        virtual bool read(QByteArray &outputBuffer) override;

        /** @brief Synchronous read function can't be used with visacomgpib4881 class
            @param outputBuffer the buffer to read / store the instrument message
            @return return false with visacomgpib4881 */
        virtual bool read(QString &outputBuffer) override;

        /** @brief Call this method to write the cmd and wait a response from device
            @note This function don't use mutex to avoid deadlock and is therefore not thread safe
            @note Call the synchronous write function and wait for VI_EVENT_SERVICE_REQ event first
            @note Call the viAsyncRead function and wait for VI_EVENT_IO_COMPLETION event then
            @param cmd The QByteArray cmd to write
            @return True if no problem occurs */
        virtual bool asyncQuery(const QByteArray &cmd) override;

        /** @brief Call this method to write the cmd and wait a response from device
            @note This function don't use mutex to avoid deadlock and is therefore not thread safe
            @note Call the synchronous write function and wait for VI_EVENT_SERVICE_REQ event first
            @note Call viAsyncRead function and wait for VI_IO_COMPLETION event then
            @param cmd The QString cmd to write
            @return True if no problem occurs */
        virtual bool asyncQuery(const QString &cmd) override;

        /** @brief Close visa instrument session
            @return return a boolean value. if false => An error occured */
        virtual bool close() override;

        /** @brief Set the timeout value for visa communication
            @param timeout the timeout parameter
            @note If setTimeout() Note used, default timeout = 2000ms
            @return True if no problem occurs*/
        virtual bool setTimeout(qint32 timeout) override;

    private:
        /** @brief Install VI_EVENT_SERVICE_REQ handler & VI_EVENT_IO_COMPLETION handler
            @note This function define which callback to use when an event occurs
            @return True if no problem occurs */
        bool installHandler();

        /** @brief Enable VI_EVENT_SERVICE_REQ event & VI_EVENT_IO_COMPLETION event
            @note VI_EVENT_SERVICE_REQ the event type used to activate queryCallback after write
            @note VI_EVENT_IO_COMPLETION the event type used to activate callback after async read
            @note This function must be call at the begining of asyncQuery()
            @return True if no problem occurs */
        bool enableEvent();

        /** @brief Uninstall VI_EVENT_SERVICE_REQ handler & VI_EVENT_IO_COMPLETION handler
            @note This function define which callback to use when an event occurs
            @return True if no problem occurs */
        bool uninstallHandler();

        /** @brief Disable VI_EVENT_SERVICE_REQ event & VI_EVENT_IO_COMPLETION event
            @note This function must be call at the end of asyncQuery()
            @note If event not disabled, write() will work as asyncQuery()
            @return True if no problem occurs */
        bool disableEvent();

        /** @brief Force asynchronous operation to teminate
            @note This function generate an IO_COMPLETION_EVENT
            @return True if no problem occurs */
        bool terminateOperation();

        /** @brief Initialize Qtimer timeout with visa timeout value
            @note If setTimeout() not used before initTimeout(), default timeout value = 2000ms */
        void initTimeout();

        /** @brief Deactivate synchronous read()
            @note Synchronous read must be deactived to use asynchronous operation correctly
            @return True if no problem occurs */
        bool deactivateRead();

        /** @brief Call this method to catch VI_EVENT_SERVIC_REQ event when it occurs
            @param session The callback session
            @return True if no problem occurs */
        bool manageSRQEvent(unsigned long session);

        /** @brief Call this method to catch VI_EVENT_IO_COMPLETION event when it occurs
            @param session The callback session
            @param event The callback event
            @return True if no problem occurs */
        bool manageIOEvent(unsigned long session, unsigned long event);

        /** @brief Call this method to manage asyncQuery errors
            @note Firstly, the function disable events
            @note Secondly, the function unlock the asyncQuery
            @note Finaly, the function emit the signal errorOccured()
            @return True if no problem occurs */
        void manageError();

        /** @brief Used to lock a function (This function don't use mutex and avoid deadlock)
            @note Used at the begining of asyncQuery operation
            @note Used to avoid multiple asyncQuery at the same time
            @return return a boolean value. if false => the asyncQuery() is already locked */
        bool lockFunction();

        /** @brief Used to unlock a function (This function don't use mutex and avoid deadlock)
            @note Used at the end of asyncQuery operation
            @note Used to avoid multiple asyncQuery at the same time
            @return return a boolean value. if false => the asyncQuery() is already unlocked */
        void unlockFunction();

    private:
        /** @brief Callback called by an event when write() finihed and data to read available
            @note Event VI_EVENT_SERVICE_REQ activate callback when Service Request command
                  received from the device
            @note Event VI_EVENT_IO_COMPLETION activate callback when asynchronous operations
                  ended successfuly
            @param session the instrument session
            @param evType the event type occured
            @param event used to specify the unique occurrence of an event.
            @param userHandle used to pass the AVisaCom context
            @return return a ViStatus value.*/
        /*ViStatus*/ static signed long queryCallback(unsigned long session,
                                                      unsigned long eventType,
                                                      unsigned long event,
                                                      void* userHandle);

    private slots:
        /** @brief onTimeout called when QTimer::timeout occured */
        void onTimeout();

    private:
        /** @brief Enum used to differenciate asynchronous operations when timeout error occurs */
        enum AsyncQueryState
        {
            Writing,  //used when wait for Service Request
            Reading,  //used when wait for asynRead output
            Terminating, //used when Reading timeout occured to terminate event
        };

    private:
        static const constexpr unsigned long bufferSize = 1024;

        static const constexpr quint32 defaultTimoutInMs = 2000;

    private:
        /** @brief Visa instrument session */
        /*ViSession*/ unsigned long _instr{0};

        /** @brief Visa function status (status<0 = function error) */
        /*ViStatus*/ qint32 _status{0};

        /** @brief Id of asynchronous operation */
        /*ViJobId*/ unsigned long _jobID{0};

        /** @brief Timer used to generate timeout
            @note ENUM: Reading ->  generate timeout signal for asyncRead
            @note ENUM: Writing -> generate timeout signal when no VI_EVENT_SERVICE_REQ occured */
        QTimer *_timer{nullptr};

        /** @brief Used to store timeout value defined in the override setTimeout function */
        qint32 _tmoValue{defaultTimoutInMs};

        /** @brief Buffer used to store received data from queryCallback */
        QByteArray _buffer;

        /** @brief Used to define timeout action to do (Write timeout / read timeout)*/
        AsyncQueryState _asyncQueryState;

        /** @brief Used to manage access to write and read function */
        bool _isFunctionUsed{false};
};
