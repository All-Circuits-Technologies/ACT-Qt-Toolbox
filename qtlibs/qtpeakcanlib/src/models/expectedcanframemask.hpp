#pragma once

// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include <QObject>

#include "src/definescan.hpp"

class QCanBusFrame;


/** @brief This represents the expected CAN frame and may contain mask information to wait a
           specific frame.
    @note If @ref _mask is empty, it means that we only wait for an answer thanks to its id
          If @ref _mask is not empty, it means that we want to receive a specific answer and we
          won't stop the listenning until we receive it */
class CAN_EXPORT ExpectedCanFrameMask
{
    public:
        /** @brief Class constructor
            @param receivedMsgId The CAN id of the expected answer
            @param maskIdx This is the index in the message payload where the mask to apply begins
            @param mask This is the mask to apply to the message received
            @param expectedMaskResult This is the expected value after having apply the mask on the
                                      received message
            @param waitUntilReceived If true, it means that we have to wait the expected answer
                                     before returning the message to be tested */
        explicit ExpectedCanFrameMask(quint32 receivedMsgId,
                                      quint8 maskIdx,
                                      const QByteArray &mask,
                                      const QByteArray &expectedMaskResult,
                                      bool waitUntilReceiveExpected =
                                        DefaultWaitUntilReceiveExpectedFrame);

        /** @brief Class constructor
            @param receivedMsgId The CAN id of the expected answer */
        explicit ExpectedCanFrameMask(quint32 receivedMsgId);

        /** @brief Copy constructor
            @param copy The element to copy */
        ExpectedCanFrameMask(const ExpectedCanFrameMask& copy);

        /** @brief Class destructor */
        virtual ~ExpectedCanFrameMask();

    public:
        /** @brief Get the id of the received message */
        quint32 getReceivedMsgId() const { return _receivedMsgId; }

        /** @brief Say if we are waiting for the expected answer before returning the message to be
                   tested */
        bool hasToWaitUntilReceivedExpected() const { return _waitUntilReceiveExpected; }

        /** @brief Check if the message given is valid
            @param messageReceived The message to test
            @param silent If false, we test the message received and display warnings if the message
                          isn't what we expect. This can be used when you only want to verify if the
                          message is really formatted as you expect
                          If true, we test if the message received is the expected one, if not, no
                          warnings are displayed in logs. This can be used when you want to test the
                          received message to find the expected one.
            @return True if no problem occurred */
        bool checkIfMessageReceivedIsValid(const QCanBusFrame &messageReceived,
                                           bool silent = false) const;

        /** @brief Assignment operator
            @param otherElement The element to copy */
        ExpectedCanFrameMask &operator=(const ExpectedCanFrameMask &otherElement);

        /** @brief Returns the object stringified */
        QString toString() const;

    public:
        /** @brief Searches in the given @ref expectedFrameMasks, the first element which matches
                   the message id and, if we have to wait until received element, we check the
                   @ref frame received
            @param expectedFrameMasks The list of elements to search in
            @param frame The frame to test and search from
            @param from The index to start the search from
            @return The index of the found @ref ExpectedCanFrameMask element or -1 isn't found */
        static int indexOf(const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
                           const QCanBusFrame &frame,
                           int from = 0);

        /** @brief Convert the answer ids to a @ref ExpectedCanFrameMask list
            @param answersId The list of ids to convert
            @return The @ref ExpectedCanFrameMask list converted */
        static QVector<ExpectedCanFrameMask> convert(const QVector<quint32> &answersId);

        /** @brief Check if the message given is valid
            @param messageReceived The message to test
            @param maskIdx This is the index in the message payload where the mask to apply begins
            @param mask This is the mask to apply to the message received
            @param expectedMaskResult This is the expected value after having apply the mask on the
                                      received message
            @param silent If false, we test the message received and display warnings if the message
                          isn't what we expect. This can be used when you only want to verify if the
                          message is really formatted as you expect
                          If true, we test if the message received is the expected one, if not, no
                          warnings are displayed in logs. This can be used when you want to test the
                          received message to find the expected one.
            @return True if no problem occurred */
        static bool checkMessageReceivedValidity(const QCanBusFrame &messageReceived,
                                                 quint8 maskIdx,
                                                 const QByteArray &mask,
                                                 const QByteArray &expectedMaskResult,
                                                 bool silent = false);

    public:
        static const constexpr bool DefaultWaitUntilReceiveExpectedFrame = false;
        static const constexpr char* ToStringPattern = "ExpectedCanFrameMask, id: %1";
        static const constexpr char* ToStringPatternExtended = "%1, mask idx: %1, pattern: %3, "
                                                               "result: %4, wait until "
                                                               "received: %5";

    private:
        quint32 _receivedMsgId{0};
        quint8 _maskIdx{0};
        QByteArray _mask{};
        QByteArray _expectedMaskResult{};

        bool _waitUntilReceiveExpected{DefaultWaitUntilReceiveExpectedFrame};
};

/** @brief Allow to display the object instance in debug console */
QDebug operator<<(QDebug stream, const ExpectedCanFrameMask &expected);
