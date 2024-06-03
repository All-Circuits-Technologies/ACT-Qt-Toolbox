// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QString>

class QCanBusFrame;


/** @brief Contains useful method to manipulate the  @ref QCanBusFrame */
class CanBusFrameHelper
{
    private:
        /** @brief Private constructor */
        explicit CanBusFrameHelper();

    public:
        /** @brief Stringiy a QCanBusFrame
            @note Only the frame id, the payload and its length are written in the string (not
                  the options)
            @note To separate the elements, the separator: @ref StrSeparator is used
            @warning The cmd id and payload are written in base 16 and the payload size is written
                     in base 10
            @param frame The frame to stringify
            @return The frame stringified */
        static QString stringifyFrame(const QCanBusFrame &frame);

        /** @brief Parse a stringified frame to a @ref QCanBusFrame
            @note The @ref strFrame must have the same format as the string returned by the method
                  @ref stringifyFrame
            @param strFrame The frame representation to parse
            @param frame The frame parsed
            @return True if no problem occurred */
        static bool parseStrFrame(const QString &strFrame, QCanBusFrame &frame);

        /** @brief The method copies the information of the @ref source object to the @ref target
                   object
            @note The class @ref QCanBusFrame doesn't have a copy constructor an equals operator,
                  this method does the job
            @param source The CAN bus frame to copy
            @param target The CAN bus frame to fill with the @ref source information */
        static void copyCanBusFrame(const QCanBusFrame &source, QCanBusFrame &target);

    private:
        /** @brief The number of expected elements in the stringified frame */
        static const constexpr int PartsNb = 3;

        /** @brief The index of the frame id in the stringified frame */
        static const constexpr int IdPartIdx = 0;

        /** @brief The index of the frame length in the stringified frame */
        static const constexpr int LengthPartIdx = 1;

        /** @brief The index of the payload in the stringified frame */
        static const constexpr int PayloadPartIdx = 2;

        /** @brief The separator character in the stringified frame */
        static const constexpr char StrSeparator = '/';

        /** @brief The format used to represent the stringified frame */
        static const constexpr char* CanBusFormat = "%1/%2/%3";
};
