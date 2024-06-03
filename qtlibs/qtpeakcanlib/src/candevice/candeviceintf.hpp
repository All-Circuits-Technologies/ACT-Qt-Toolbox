// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QCanBusFrame>

#include "src/definescan.hpp"
#include "src/models/candeviceconfig.hpp"

class CanDevice;
class CanDeviceThread;
class ExpectedCanFrameMask;
class QCanBusDevice;


/** @brief This is the interface to communicate to a CAN device
    @note This class creates a @ref CanDeviceThread and let all the CAN management in this thread
          The calls of all methods in the class are thread safe. */
class CAN_EXPORT CanDeviceIntf : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param canIntfKey The CAN interface key used to communicate to the device through the
                              CAN
            @param parent The parent class */
        explicit CanDeviceIntf(const CanDeviceConfig &config, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~CanDeviceIntf() override;

    public:
        /** @brief Get the CAN interface key used to communicate with the device */
        PCanBusItf::Enum getCanIntfKey() const { return _config.getCanBusItf(); }

        /** @brief Get the CAN device config linked to the interface */
        const CanDeviceConfig &getConfig() const { return _config; }

        /** @brief Initialize the communication to the device. Also starts the listenning of the
                   CAN messages.
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @return True if no problem occurred */
        bool initDevice();

        /** @brief Unitialize the communcation with the device
            @note The method first stops to listen the messages before closing the channel.
                  Therefore, this method may take some times to return.
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @return True if no problem occurred */
        bool unInitialize();

        /** @brief Test if the CAN device is initialized or not
            @return True if the CAN device is initialized */
        bool isInitialized() const;

        /** @brief Get the value of the BUS Off auto reset parameter
            @note From doc: This parameter instructs the PCAN driver to automatically reset the CAN
                            controller of a PCAN Channel when a bus-off state is detected.
            @param autoReset The auto reset method parameter to set with value got from PCAN
            @return True if no problem occurred */
        bool getParamBusOffAutoReset(bool &autoReset);

        /** @brief Set the value of the BUS Off auto reset parameter
            @note From doc: This parameter instructs the PCAN driver to automatically reset the CAN
                            controller of a PCAN Channel when a bus-off state is detected.
            @param autoReset The BUS Off auto reset parameter value to set in PCAN
            @return True if no problem occurred */
        bool setParamBusOffAutoReset(bool autoReset);

        /** @brief Write a CAN bus frame
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param frame The frame to write
            @return True if no problem occurred */
        bool write(const QCanBusFrame &frame);

        /** @brief Write a CAN bus frame and wait for an answer
            @note The method begins to listen before the writting of message; therefore, if the
                  answer is sent before the writing, you may receive this answer.
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param frame The frame to write
            @param expectedFrameMask The information which describes the expected answer
            @param timeoutInMs If different of -1, the method will wait the anwer for this
                               duration and returns an empty QVector, if nothing has been received
                               If equals to -1, the method will wait forever the reception of the
                               answer
            @return The expected answer, if empty, it means that a problem occurred.
                    The vector returned contains at most one element. */
        QVector<QCanBusFrame> writeAndWaitAnswer(const QCanBusFrame &frame,
                                                 const ExpectedCanFrameMask &expectedFrameMask,
                                                 int timeoutInMs = -1);

        /** @brief Write a CAN bus frame and wait for an answer
            @note The method begins to listen before the writting of message; therefore, if the
                  answer is sent before the writing, you may receive this answer.
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param frame The frame to write
            @param answerId The id of the expected answer
            @param timeoutInMs If different of -1, the method will wait the anwer for this
                               duration and returns an empty QVector, if nothing has been received
                               If equals to -1, the method will wait forever the reception of the
                               answer
            @return The expected answer, if empty, it means that a problem occurred.
                    The vector returned contains at most one element. */
        QVector<QCanBusFrame> writeAndWaitAnswerById(const QCanBusFrame &frame,
                                                     quint32 answerId,
                                                     int timeoutInMs = -1);

        /** @brief Write a CAN bus frame and wait for one of the answers given
            @note The method begins to listen before the writting of message; therefore, if one the
                  answer is sent before the writing, you may receive this answer.
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param frame The frame to write
            @param expectedFrameMasks The information which describes the expected answers
            @param timeoutInMs If different of -1, the method will wait one of the anwer for this
                               duration and returns an empty QVector, if nothing has been received
                               If equals to -1, the method will wait forever the reception of the
                               answers
            @return One of the expected answers, if empty, it means that a problem occurred.
                    The vector returned contains at most one element. */
        QVector<QCanBusFrame> writeAndWaitOneOfAnswers(
            const QCanBusFrame &frame,
            const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
            int timeoutInMs = -1);

        /** @brief Write a CAN bus frame and wait for one of the answers given
            @note The method begins to listen before the writting of message; therefore, if one the
                  answer is sent before the writing, you may receive this answer.
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param frame The frame to write
            @param answersIds The ids of the expected answers
            @param timeoutInMs If different of -1, the method will wait one of the anwer for this
                               duration and returns an empty QVector, if nothing has been received
                               If equals to -1, the method will wait forever the reception of the
                               answers
            @return One of the expected answers, if empty, it means that a problem occurred.
                    The vector returned contains at most one element. */
        QVector<QCanBusFrame> writeAndWaitOneOfAnswersByIds(
            const QCanBusFrame &frame,
            const QVector<quint32> &answersIds,
            int timeoutInMs = -1);

        /** @brief Write a CAN bus frame and wait for all the answers given
            @note The method begins to listen before the writting of message; therefore, if one the
                  answer is sent before the writing, you will get this answer.
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param frame The frame to write
            @param expectedFrameMasks The information which describes the expected answers
            @param timeoutInMs If different of -1, the method will wait the anwers for this
                               duration and returns an empty QVector, if nothing has been received
                               If equals to -1, the method will wait forever the reception of the
                               answers
            @return The expected answer, if empty, it means that a problem occurred.
                    The vector returned contains the same length of elements as @ref answersIds. */
        QVector<QCanBusFrame> writeAndWaitAllTheAnswers(
            const QCanBusFrame &frame,
            const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
            int timeoutInMs = -1);

        /** @brief Write a CAN bus frame and wait for all the answers given
            @note The method begins to listen before the writting of message; therefore, if one the
                  answer is sent before the writing, you will get this answer.
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param frame The frame to write
            @param answersIds The ids of the expected answers
            @param timeoutInMs If different of -1, the method will wait the anwers for this
                               duration and returns an empty QVector, if nothing has been received
                               If equals to -1, the method will wait forever the reception of the
                               answers
            @return The expected answer, if empty, it means that a problem occurred.
                    The vector returned contains the same length of elements as @ref answersIds. */
        QVector<QCanBusFrame> writeAndWaitAllTheAnswersByIds(
            const QCanBusFrame &frame,
            const QVector<quint32> &answersIds,
            int timeoutInMs = -1);

        /** @brief Only waits to receive a specific CAN message from a distant device
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param expectedFrameMask The information which describes the expected answer
            @param timeoutInMs If different of -1, the method will wait the CAN message for this
                               duration and returns an empty QVector, if nothing has been received
                               If equals to -1, the method will wait forever the reception of the
                               CAN message
            @return The expected CAN message, if empty, it means that a problem occurred.
                    The vector returned contains at most one element. */
        QVector<QCanBusFrame> waitCanMsg(const ExpectedCanFrameMask &expectedFrameMask,
                                         int timeoutInMs = -1);

        /** @brief Only waits to receive a specific CAN message from a distant device
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param answerId The id of the expected CAN message id
            @param timeoutInMs If different of -1, the method will wait the CAN message for this
                               duration and returns an empty QVector, if nothing has been received
                               If equals to -1, the method will wait forever the reception of the
                               CAN message
            @return The expected CAN message, if empty, it means that a problem occurred.
                    The vector returned contains at most one element. */
        QVector<QCanBusFrame> waitCanMsgById(quint32 answerId, int timeoutInMs = -1);

        /** @brief Only waits to receive one of the given CAN messages from a distant device
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param expectedFrameMasks The information which describes the expected answers
            @param timeoutInMs If different of -1, the method will wait one of the CAN messages for
                               this duration and returns an empty QVector, if nothing has been
                               received.
                               If equals to -1, the method will wait forever the reception of the
                               CAN messages
            @return One of the expected CAN messages, if empty, it means that a problem occurred.
                    The vector returned contains at most one element. */
        QVector<QCanBusFrame> waitOneOfTheCanMsgs(
            const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
            int timeoutInMs = -1);

        /** @brief Only waits to receive one of the given CAN messages from a distant device
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param answersIds The ids of the expected CAN messages
            @param timeoutInMs If different of -1, the method will wait one of the CAN messages for
                               this duration and returns an empty QVector, if nothing has been
                               received.
                               If equals to -1, the method will wait forever the reception of the
                               CAN messages
            @return One of the expected CAN messages, if empty, it means that a problem occurred.
                    The vector returned contains at most one element. */
        QVector<QCanBusFrame> waitOneOfTheCanMsgsByIds(
            const QVector<quint32> &answersIds,
            int timeoutInMs = -1);

        /** @brief Only waits to receive all the given CAN messages from a distant device
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param expectedFrameMasks The information which describes the expected answers
            @param timeoutInMs If different of -1, the method will wait all the CAN messages for
                               this duration and returns an empty QVector, if nothing has been
                               received.
                               If equals to -1, the method will wait forever the reception of the
                               CAN messages
            @return All the expected CAN messages, if empty, it means that a problem occurred.
                    The vector returned contains the same length of elements as
                    @ref expectedCanMsgsIds. */
        QVector<QCanBusFrame> waitAllTheCanMsgs(
            const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
            int timeoutInMs = -1);

        /** @brief Only waits to receive all the given CAN messages from a distant device
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param answersIds The ids of the expected CAN messages
            @param timeoutInMs If different of -1, the method will wait all the CAN messages for
                               this duration and returns an empty QVector, if nothing has been
                               received.
                               If equals to -1, the method will wait forever the reception of the
                               CAN messages
            @return All the expected CAN messages, if empty, it means that a problem occurred.
                    The vector returned contains the same length of elements as
                    @ref expectedCanMsgsIds. */
        QVector<QCanBusFrame> waitAllTheCanMsgsByIds(
            const QVector<quint32> &answersIds,
            int timeoutInMs = -1);

    signals:
        /** @brief Emitted when frames are received
            @param frames The received frames */
        void framesReceived(const QVector<QCanBusFrame> &frames);

    private:
        /** @brief Useful method to access the @ref CanDevice contains in the @ref CanDeviceThread
            @note The method ensures the linked thread to be ready.
            @param action Thanks to this parameter the caller precises the actions is doing to
                          contextualize the method logs
            @return The @ref CanDevice contains in the linked thread or null if a problem
                    occurred */
        CanDevice *accessDeviceThroughThread(const QString &action) const;

    private:
        CanDeviceConfig _config;

        CanDeviceThread *_canDeviceThread{nullptr};
};
