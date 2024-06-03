// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QString>


/** @brief Contains useful method to manipulate @ref QString */
class StringHelper
{
    private:
        /** @brief Private constructor */
        explicit StringHelper();

    public:
        /** @brief Parse the value and try to convert to a quint8 number
            @param value The value to parse and convert
            @param ok If given, the method sets the result of the method in the parameter
            @param base The numeric base to apply to the number to parse
            @return The quint8 parsed, if a problem occurred the value is equals to 0 */
        static quint8 toQuint8(const QString &value, bool *ok = nullptr, int base = 10);

        /** @brief Parse the value and try to convert to a qint8 number
            @param value The value to parse and convert
            @param ok If given, the method sets the result of the method in the parameter
            @param base The numeric base to apply to the number to parse
            @return The qint8 parsed, if a problem occurred the value is equals to 0 */
        static qint8 toQint8(const QString &value, bool *ok = nullptr, int base = 10);

        /** @brief Parse the value and try to convert to a boolean
            @param value The value to parse and convert
            @param ok If given, the method sets the result of the method in the parameter
            @return The boolean parsed, if a problem occurred the value is equals to false */
        static bool toBool(const QString &value, bool *ok = nullptr);

        /** @brief Splits the string into substrings wherever sep occurs, and returns the list of
                   those strings, with the separator included in at the end of the first element.
                   For instance:
                    If your source is: "a//b/c"
                    The list will be: [ "a/", "/", "b/", "c" ]
            @note If sep does not match anywhere in the string, split() returns a single-element
                  list containing this string.
            @note If sep is empty, split() returns an empty string, followed by each of the string's
                  characters, followed by another empty string.
            @note The behavior of this method is based on the method @ref QString::split, with the
                  adding of the separator
            @param source The source to split elements from
            @param sep The separator used to split the @ref source string
            @param behavior Detail what to do with empty string parts.
                            In this method, the only case is when the @ref source string ends with
                            the separator, for instance:
                                If your source is: "a//b/"
                                If you keep empty parts, the result will be: [ "a/", "/", "b/", "" ]
                                If you don't keep them, the result will be:  [ "a/", "/", "b/" ]
            @param cs Says if the search of the separator is case sensitive
            @return The list of splitted elements from the @ref source string */
        static QStringList splitAndKeepSepOnEnd(
            const QString &source,
            const QString &sep,
            Qt::SplitBehavior behavior = Qt::KeepEmptyParts,
            Qt::CaseSensitivity cs = Qt::CaseSensitive);

        /** @brief Splits the string into substrings wherever sep occurs, and returns the list of
                   those strings, with the separator included in at the begin of the second element.
                   For instance:
                    If your source is: "a//b/c"
                    The list will be: [ "a", "/", "/b/", "/c" ]
            @note If sep does not match anywhere in the string, split() returns a single-element
                  list containing this string.
            @note If sep is empty, split() returns an empty string, followed by each of the string's
                  characters, followed by another empty string.
            @note The behavior of this method is based on the method @ref QString::split, with the
                  adding of the separator
            @param source The source to split elements from
            @param sep The separator used to split the @ref source string
            @param behavior Detail what to do with empty string parts.
                            In this method, the only case is when the @ref source string begins with
                            the separator, for instance:
                                If your source is: "/a//b"
                                If you keep empty parts, the result will be: [ "", "/a", "/", "/b" ]
                                If you don't keep them, the result will be:  [ "/a", "/", "/b" ]
            @param cs Says if the search of the separator is case sensitive
            @return The list of splitted elements from the @ref source string */
        static QStringList splitAndKeepSepOnStart(
            const QString &source,
            const QString &sep,
            Qt::SplitBehavior behavior = Qt::KeepEmptyParts,
            Qt::CaseSensitivity cs = Qt::CaseSensitive);

    private:
        static const constexpr char* StrTrueValue = "true";
        static const constexpr char* StrNumberTrueValue = "1";
        static const constexpr char* StrFalseValue = "false";
        static const constexpr char* StrNumberFalseValue = "0";
};
