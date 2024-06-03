// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QCanBusDevice>
#include <QCanBusFrame>

#include "src/models/candeviceconfig.hpp"

class ExpectedCanFrameMask;
class PCanReadThread;


/** @brief This class represents a device which communicates through CAN
    @note The object lives a dedicated Thread
    @note The reading of messages is also done in another dedicated Thread */
class CanDevice : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param config The CAN device config linked to this device
            @param parent The object parent */
        explicit CanDevice(const CanDeviceConfig &config, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~CanDevice() override;

    public:
        /** @brief Initialize the CAN device and begins to listen for CAN messages
            @note If the device is already initialized, the method will return true.
            @return True if no problem occurred */
        bool initialize();

        /** @brief Unitialize the CAN device and stop to listen for CAN messages
            @note The method first stops to listen the messages before closing the channel.
                  Therefore, this method may take some times to return.
            @return True if no problem occurred */
        bool unInitialize();

        /** @brief Test if the device is already initialized or not
            @return True if it's initialized */
        bool isInitialized() const;

        /** @brief Get the value of the BUS Off auto reset parameter
            @note From doc: This parameter instructs the PCAN driver to automatically reset the CAN
                            controller of a PCAN Channel when a bus-off state is detected.
            @note The @ref autoReset parameter is a pointer to be used with the
                  @ref ThreadConcurrentRun::run method
            @param autoReset The auto reset method parameter to set with value got from PCAN
            @return True if no problem occurred */
        bool getParamBusOffAutoReset(bool *autoReset);

        /** @brief Set the value of the BUS Off auto reset parameter
            @note From doc: This parameter instructs the PCAN driver to automatically reset the CAN
                            controller of a PCAN Channel when a bus-off state is detected.
            @param autoReset The BUS Off auto reset parameter value to set in PCAN
            @return True if no problem occurred */
        bool setParamBusOffAutoReset(bool autoReset);

        /** @brief Write a CAN bus frame
            @param frame The frame to write
            @return True if no problem occurred */
        bool write(const QCanBusFrame &frame);

        /** @brief Write a CAN bus frame and wait for an answer
            @note The method begins to listen before the writting of message; therefore, if the
                  answer is sent before the writing, you may receive this answer.
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
            @param expectedCanMsgId The id of the expected CAN message
            @param timeoutInMs If different of -1, the method will wait the CAN message for this
                               duration and returns an empty QVector, if nothing has been received
                               If equals to -1, the method will wait forever the reception of the
                               CAN message
            @return The expected CAN message, if empty, it means that a problem occurred.
                    The vector returned contains at most one element. */
        QVector<QCanBusFrame> waitCanMsgById(quint32 answerId, int timeoutInMs = -1);

        /** @brief Only waits to receive one of the given CAN messages from a distant device
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
            @param expectedCanMsgsIds The ids of the expected CAN messages
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
            @param expectedCanMsgsIds The ids of the expected CAN messages
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

    private:
        /** @brief Write and wait for CAN messages
            @note The method begins to listen before the write method; therefore, if one of
                  the expected messages is sent before the writing, you may receive this message.
            @param frame The frame to write
            @param expectedCanMsgIds The ids of the expected CAN messages
            @param areWeWaitingForAllMsgIds True if we are waiting for all the elements contain
                                            in the @ref expectedCanMsgIds param.
                                            False if we are only waiting for one of the elements
                                            contains in the @ref expectedCanMsgIds param.
            @param timeoutInMs If different of -1, the method will wait the CAN messages for this
                               duration and returns an empty QVector, if nothing has been received
                               If equals to -1, the method will wait forever the reception of the
                               CAN messages
            @return The expected CAN messages, if empty, it means that a problem occurred.
                    If @ref areWeWaitingForAllMsgIds is equals to true, the vector returned
                    contains the same length of elements as @ref expectedCanMsgIds.
                    If @ref areWeWaitingForAllMsgIds is equals to false, the vector returned
                    contains at most one element. */
        QVector<QCanBusFrame> writeAndwaitFrameAndProcessIfNeeded(
            const QCanBusFrame &frame,
            const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
            bool areWeWaitingForAllMsgIds,
            int timeoutInMs = -1);

        /** @brief Wait for CAN messages and call the @ref process method (if given)
            @note The method begins to listen before the call of the process method; therefore, if
                  the expected message is sent before the process, you may receive this message.
            @param expectedCanMsgIds The ids of the expected CAN messages
            @param process The process to call while waiting for the specific CAN messages.
                           If equals to nullptr, nothing is called and we just wait for the messages
            @param timeoutInMs If different of -1, the method will wait the CAN messages for this
                               duration and returns an empty QVector, if nothing has been received
                               If equals to -1, the method will wait forever the reception of the
                               CAN messages
            @return The expected CAN messages, if empty, it means that a problem occurred.
                    If @ref areWeWaitingForAllMsgIds is equals to true, the vector returned
                    contains the same length of elements as @ref expectedCanMsgIds.
                    If @ref areWeWaitingForAllMsgIds is equals to false, the vector returned
                    contains at most one element. */
        QVector<QCanBusFrame> waitFrameAndProcessIfNeeded(
            const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
            bool areWeWaitingForAllMsgIds,
            const std::function<bool ()> *process = nullptr,
            int timeoutInMs = -1);

    signals:
        /** @brief Emitted when frames are received
            @param frames The received frames */
        void framesReceived(const QVector<QCanBusFrame> &frames);

    private:
        CanDeviceConfig _config;
        PCanReadThread *_readThread{nullptr};
};
