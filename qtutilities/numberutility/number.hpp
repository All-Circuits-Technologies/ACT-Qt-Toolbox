// SPDX-FileCopyrightText: 2019 - 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QDebug>
#include <QMetaType>
#include <QtGlobal>

#include "definesutility/warningsutility.hpp"


/** @brief Useful class in order to represent a decimal number
    @note The number can't have a comma shift higher to 6
    @note The number can be positive or negative
    @note The max limit of quint64 represents infinity */
class Number
{
public:
    /** @brief Class constructor
        @note The class is invalid */
    Number() = default;

    /** @brief Class constructor
        @param value The string representation of the number, must be formatted like this:
                     *  123456
                     *  12345.566
                     *  12345,666
                     *  -123456
                     *  -123456.256
                     *  -123456,256
        @note If the string is not well formatted, the number will be invalid */
    explicit Number(const QString &value);

    /** @brief Class constructor
        @param value The complete value without comma
        @param commaShift The shift of the comma in the previous unsigned integer given
        @param positiveNumber Give the sign of the number */
    explicit Number(quint64 value, quint8 commaShift = 0, bool positiveNumber = true);

    /** @brief Copy constructor */
    Number(const Number &otherNumber);

    /** @brief Class destructor */
    virtual ~Number();

    /** @brief Say if the number is valid or not */
    bool isValid() const;

    /** @brief Set the number invalid */
    void clear();

    /** @brief Get the shift number after comma */
    quint8 getShiftNbAfterComma() const { return _commaShift; }

    /** @brief Returns the decimal part of the number
        @note For instance:
                If the number is 123456.789, the method will return: 789
        @return The decimal part of the number */
    quint64 getDecimalPart() const;

    /** @brief Returns the divisor to obtain, from an integer, the floating decimal part
        @note For instance:
                If the number is 123456.00789, the method will return: 100000
        @return The divisor to obtain the floating decimal part from an integer*/
    quint64 getDecimalDivisor() const;

    /** @brief Returns the integer part of the number
        @note Number sign is handled separately (see @ref isPositive),
                hence integer part is always positive.
        @note For instance:
                - If the number is 123456.789, the method will return 123456
                - If the number is -123456.789, the method will also return 123456
        @return The integer part of the number */
    quint64 getIntegerPart() const;

    /** @brief Transform the number to uint8
        @param ok If a problem occurred when trying to transform the integer, and if the param is
                  not null, ok will be set to false (or true if no problem occurred)
        @param commaShiftToRound The comma shift to apply on Number
                                 Example :
                                    If Number: 1234.56789, and commaShift = 0; result = 1235
                                    If Number: 1234.56789, and commaShift = 1; result = 12346
                                    If Number: -1234.5678, and commaShift = 0; result = -1235
                                    If Number: -1234.5678, and commaShift = 1; result = -12346
                                    If Number: 123.45, and commaShift = 1; result = 1235
                                    If Number: 123, and commaShift = 1; result = 1230
        @return The number transformed, or 0 if a problem occurred */
    quint8 toUint8(bool *ok = nullptr, quint8 commaShiftToRound = 0) const;

    /** @brief Transform the number to int8
        @param ok If a problem occurred when trying to transform the integer, and if the param is
                  not null, ok will be set to false (or true if no problem occurred)
        @param commaShiftToRound The comma shift to apply on Number
                                 Example :
                                    If Number: 1234.56789, and commaShift = 0; result = 1235
                                    If Number: 1234.56789, and commaShift = 1; result = 12346
                                    If Number: -1234.5678, and commaShift = 0; result = -1235
                                    If Number: -1234.5678, and commaShift = 1; result = -12346
                                    If Number: 123.45, and commaShift = 1; result = 1235
                                    If Number: 123, and commaShift = 1; result = 1230
        @return The number transformed, or 0 if a problem occurred */
    qint8 toInt8(bool *ok = nullptr, quint8 commaShiftToRound = 0) const;

    /** @brief Transform the number to uint16
        @param ok If a problem occurred when trying to transform the integer, and if the param is
                  not null, ok will be set to false (or true if no problem occurred)
        @param commaShiftToRound The comma shift to apply on Number
                                 Example :
                                    If Number: 1234.56789, and commaShift = 0; result = 1235
                                    If Number: 1234.56789, and commaShift = 1; result = 12346
                                    If Number: -1234.5678, and commaShift = 0; result = -1235
                                    If Number: -1234.5678, and commaShift = 1; result = -12346
                                    If Number: 123.45, and commaShift = 1; result = 1235
                                    If Number: 123, and commaShift = 1; result = 1230
        @return The number transformed, or 0 if a problem occurred */
    quint16 toUint16(bool *ok = nullptr, quint8 commaShiftToRound = 0) const;

    /** @brief Transform the number to int16
        @param ok If a problem occurred when trying to transform the integer, and if the param is
                  not null, ok will be set to false (or true if no problem occurred)
        @param commaShiftToRound The comma shift to apply on Number
                                 Example :
                                    If Number: 1234.56789, and commaShift = 0; result = 1235
                                    If Number: 1234.56789, and commaShift = 1; result = 12346
                                    If Number: -1234.5678, and commaShift = 0; result = -1235
                                    If Number: -1234.5678, and commaShift = 1; result = -12346
                                    If Number: 123.45, and commaShift = 1; result = 1235
                                    If Number: 123, and commaShift = 1; result = 1230
        @return The number transformed, or 0 if a problem occurred */
    qint16 toInt16(bool *ok = nullptr, quint8 commaShiftToRound = 0) const;

    /** @brief Transform the number to uint32
        @param ok If a problem occurred when trying to transform the integer, and if the param is
                  not null, ok will be set to false (or true if no problem occurred)
        @param commaShiftToRound The comma shift to apply on Number
                                 Example :
                                    If Number: 1234.56789, and commaShift = 0; result = 1235
                                    If Number: 1234.56789, and commaShift = 1; result = 12346
                                    If Number: -1234.5678, and commaShift = 0; result = -1235
                                    If Number: -1234.5678, and commaShift = 1; result = -12346
                                    If Number: 123.45, and commaShift = 1; result = 1235
                                    If Number: 123, and commaShift = 1; result = 1230
        @return The number transformed, or 0 if a problem occurred */
    quint32 toUint32(bool *ok = nullptr, quint8 commaShiftToRound = 0) const;

    /** @brief Transform the number to int32
        @param ok If a problem occurred when trying to transform the integer, and if the param is
                  not null, ok will be set to false (or true if no problem occurred)
        @param commaShiftToRound The comma shift to apply on Number
                                 Example :
                                    If Number: 1234.56789, and commaShift = 0; result = 1235
                                    If Number: 1234.56789, and commaShift = 1; result = 12346
                                    If Number: -1234.5678, and commaShift = 0; result = -1235
                                    If Number: -1234.5678, and commaShift = 1; result = -12346
                                    If Number: 123.45, and commaShift = 1; result = 1235
                                    If Number: 123, and commaShift = 1; result = 1230
        @return The number transformed, or 0 if a problem occurred */
    qint32 toInt32(bool *ok = nullptr, quint8 commaShiftToRound = 0) const;

    /** @brief Transform the number to uint64
        @param ok If a problem occurred when trying to transform the integer, and if the param is
                  not null, ok will be set to false (or true if no problem occurred)
        @param commaShiftToRound The comma shift to apply on Number
                                 Example :
                                    If Number: 1234.56789, and commaShift = 0; result = 1235
                                    If Number: 1234.56789, and commaShift = 1; result = 12346
                                    If Number: -1234.5678, and commaShift = 0; result = -1235
                                    If Number: -1234.5678, and commaShift = 1; result = -12346
                                    If Number: 123.45, and commaShift = 1; result = 1235
                                    If Number: 123, and commaShift = 1; result = 1230
        @return The number transformed, or 0 if a problem occurred */
    quint64 toUint64(bool *ok = nullptr, quint8 commaShiftToRound = 0) const;

    /** @brief Transform the number to int64
        @param ok If a problem occurred when trying to transform the integer, and if the param is
                  not null, ok will be set to false (or true if no problem occurred)
        @param commaShiftToRound The comma shift to apply on Number
                                 Example :
                                    If Number: 1234.56789, and commaShift = 0; result = 1235
                                    If Number: 1234.56789, and commaShift = 1; result = 12346
                                    If Number: -1234.5678, and commaShift = 0; result = -1235
                                    If Number: -1234.5678, and commaShift = 1; result = -12346
                                    If Number: 123.45, and commaShift = 1; result = 1235
                                    If Number: 123, and commaShift = 1; result = 1230
        @return The number transformed, or 0 if a problem occurred */
    qint64 toInt64(bool *ok = nullptr, quint8 commaShiftToRound = 0) const;

    /** @brief Return True if the number is negative */
    bool isNegative() const { return !_positive; }

    /** @brief Return True if the number is positive */
    bool isPositive() const { return _positive; }

    /** @brief Return True if the number is infinite */
    bool isInfinite() const;

    /** @brief Return True if the number is psotiive infinite */
    bool isPositiveInfinite() const;

    /** @brief Return True if the number is negative infinite */
    bool isNegativeInfinite() const;

    /** @brief Return a string representation of the number
        @note The separator is a comma */
    QString toString() const;

    /** @brief The method does round the number
        @param commaShiftToRound The commaShift to round
                                    If Number: 1234.56789, and commaShift = 0; result = 1235
                                    If Number: 1234.56789, and commaShift = 1; result = 1234.6
                                    If Number: -1234.5678, and commaShift = 0; result = -1235
                                    If Number: -1234.5678, and commaShift = 1; result = -1234.6
                                    If Number: 1234, and commaShift = 2; result = 1234 (do
                                        nothing)
                                    If Number: -1234, and commaShift = 2; result = -1234 (do
                                        nothing)
        @return The number rounded */
    Number &round(quint8 commaShiftToRound);

    /** @brief Trim the current number value with commaShift in order that: 2.000 and 2 are the
               same number.
        @return The current number */
    Number &trimNumber();

    /** @brief Assign the values of an other number to this one
        @param otherNumber The other number to copy
        @return Return this modified element */
    Number &operator=(const Number &otherNumber);

    /** @brief Inverse the sign of this number
        @return Same number with the opposite sign */
    Number operator-() const;

    /** @brief Add an other number with this one and returns the result
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, this number will be set to invalid
        @param otherNumber The other number to add with this number
        @return The result of the addition */
    Number operator+(const Number &otherNumber) const;

    /** @brief Add an other number to this one, and set the result to this number
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, this number will be set to invalid
        @param otherNumber The other number to add with this number */
    void operator+=(const Number &otherNumber);

    /** @brief Substract an other number to this one and returns the result
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, this number will be set to invalid
        @param otherNumber The other number to substract to this number
        @return The result of the substraction */
    Number operator-(const Number &otherNumber) const;

    /** @brief Substract an other number to this one, and set the result to this number
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, this number will be set to invalid
        @param otherNumber The other number to substract to this number */
    void operator-=(const Number &otherNumber);

    /** @brief Divide this number with the other number given
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, this number will be set to invalid
        @param otherNumber The other number to divide this number with
        @return The result of the division */
    Number operator/(const Number &otherNumber) const;

    /** @brief Divide this number with the other number given, and set the result to this number
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, this number will be set to invalid
        @param otherNumber The other number to divide this number with */
    void operator/=(const Number &otherNumber);

    /** @brief Multiply this number with the other number given
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, this number will be set to invalid
        @param otherNumber The other number to multiply this number with
        @return The result of the multiplication */
    Number operator*(const Number &otherNumber) const;

    /** @brief Multiply this number with the other number given, and set the result to this
               number
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, this number will be set to invalid
        @param otherNumber The other number to multiply this number with */
    void operator*=(const Number &otherNumber);

    /** @brief Get the modulo of the dividing between this number and the other number given
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, this number will be set to invalid
        @param otherNumber The other number to divide this number with, and get the modulo of
                           the operation
        @return The modulo of the division */
    Number operator%(const Number &otherNumber) const;

    /** @brief Get the modulo of the division between this number and the other number given,
               and set the result to this number
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, this number will be set to invalid
        @param otherNumber The other number to divide this number with, and get the modulo of
                           the operation */
    void operator%=(const Number &otherNumber);

    /** @brief Compare this number and the other number, if this number is strictly higher than
               the other number, return true
        @note If a problem occurs or if one of the numbers is invalid, returns False
        @param otherNumber The other number to compare this number with
        @return True if this number is strictly higher than the other one */
    bool operator>(const Number &otherNumber) const;

    /** @brief Compare this number and the other number, if this number is higher than
               (or equal) the other number, return true
        @note If a problem occurs or if one of the numbers is invalid, returns False
        @param otherNumber The other number to compare this number with
        @return True if this number is higher than (or equal) the other one */
    bool operator>=(const Number &otherNumber) const;

    /** @brief Compare this number and the other number, if this number is strictly lower than
               the other number, return true
        @note If a problem occurs or if one of the numbers is invalid, returns False
        @param otherNumber The other number to compare this number with
        @return True if this number is strictly lower than the other one */
    bool operator<(const Number &otherNumber) const;

    /** @brief Compare this number and the other number, if this number is lower than
               (or equal) the other number, return true
        @note If a problem occurs or if one of the numbers is invalid, returns False
        @param otherNumber The other number to compare this number with
        @return True if this number is lower than (or equal) the other one */
    bool operator<=(const Number &otherNumber) const;

    /** @brief Test equality between the two numbers
        @param otherNumber The other number to compare this number with
        @return True if the numbers are equals */
    bool operator==(const Number &otherNumber) const;

    /** @brief Test equality between the two numbers
        @param otherNumber The other number to compare this number with
        @return True if the numbers are not equals */
    bool operator!=(const Number &otherNumber) const;

    /** @brief Cast operator toward QString
        @note This operator defines the possibility to easily cast a Number to a QString
              using standard C++ calls
        @return Stringification of the current instance */
    operator QString() const { return toString(); }

    /** @brief Apply pow on the current number, if the calculation failed, this will set the
                   current number as invalid
            @param power The power to apply
            @return A reference on the current number */
    Number &pow(qint8 power);

    /** @brief Invert the current number, if the method failed, this will set the current number
               as invalid
        @return A reference on the current number */
    Number &invert();

    /** @brief Multiply the current number with a power of ten, if the method failed, this will
               set the current number as invalid
        @param power The power of ten to apply on number
        @return A reference on the current number */
    Number &multiplyWithPowerOfTen(qint8 power);

public:
    /** @brief Parse a number from a string
        @note To be considered as an hex number, the string representation has to follow the
              regex: @ref Number::HexNumberRegExpression
        @note The string representation of the number, must be formatted like this:
                     *  123456
                     *  12345.566
                     *  12345,666
                     *  -123456
                     *  -123456.256
                     *  -123456,256
                     *  0xAC0G
        @note If the string is not well formatted, the number returned will be invalid
        @param number The number string representation to parse
        @return The number parsed thanks to the string given */
    static Number fromString(const QString &number);

    /** @brief convert QByteArray to number
        @param number The number QByteArray representation to convert
        @param isLsbFirst set true if LSB first on QByteArray
        @return The number converted thanks to the QByteArray given */
    static Number fromHex(const QByteArray &number, bool isLsbFirst);

    /** @brief Parse a number from a float
        @note If the float overflows the limits, the number returned will be invalid
        @note Same that calling fromFloat(float, quint8) with a commaShift equals to
              maxAllowedCommaShift
        @param number The float number to parse
        @return The number parsed */
    static Number fromFloat(float number)
    { return fromFloatOrDouble<float>(number, MaxAllowedCommaShift); }

    /** @brief Parse a number from a float
        @note If the float overflows the limits, the number returned will be invalid
        @param number The float number to parse
        @param commaShift Give the number of digits we want after the comma
        @return The number parsed */
    static Number fromFloat(float number, quint8 commaShift)
    { return fromFloatOrDouble<float>(number, commaShift); }

    /** @brief Parse a number from a double
        @note If the double overflows the limits, the number returned will be invalid
        @note Same that calling fromDouble(double, quint8) with a commaShift equals to
              maxAllowedCommaShift
        @param number The double number to parse
        @param commaShift Give the number of digits we want after the comma
        @return The number parsed */
    static Number fromDouble(double number)
    { return fromFloatOrDouble<double>(number, MaxAllowedCommaShift); }

    /** @brief Parse a number from a double
        @note If the double overflows the limits, the number returned will be invalid
        @param number The double number to parse
        @param commaShift Give the number of digits we want after the comma
        @return The number parsed */
    static Number fromDouble(double number, quint8 commaShift)
    { return fromFloatOrDouble<double>(number, commaShift); }

    /** @brief Parse a number from a qint8
        @param number The qint8 number
        @return The number parsed */
    static Number fromInt8(qint8 number)
    { return fromInteger<qint8>(number); }

    /** @brief Parse a number from a quint8
        @param number The quint8 number
        @return The number parsed */
    static Number fromUInt8(quint8 number)
    { return fromInteger<quint8>(number); }

    /** @brief Parse a number from a qint16
        @param number The qint16 number
        @return The number parsed */
    static Number fromInt16(qint16 number)
    { return fromInteger<qint16>(number); }

    /** @brief Parse a number from a quint16
        @param number The quint16 number
        @return The number parsed */
    static Number fromUInt16(quint16 number)
    { return fromInteger<quint16>(number); }

    /** @brief Parse a number from a qint32
        @param number The qint32 number
        @return The number parsed */
    static Number fromInt32(qint32 number)
    { return fromInteger<qint32>(number); }

    /** @brief Parse a number from a quint32
        @param number The quint32 number
        @return The number parsed */
    static Number fromUInt32(quint32 number)
    { return fromInteger<quint32>(number); }

    /** @brief Parse a number from a qint64
        @param number The qint64 number
        @return The number parsed */
    static Number fromInt64(qint64 number)
    { return fromInteger<qint64>(number); }

    /** @brief Parse a number from a quint64
        @param number The quint64 number
        @return The number parsed */
    static Number fromUInt64(quint64 number)
    { return fromInteger<quint64>(number); }

    /** @brief Register all required stuff to use Number through QMetaTypes
        @note Especially registers Number to QString QMetaType conversion,
              allowing to easily convert a QVariant<Number> into a string. */
    static void RegisterMetaTypes();

    /** @brief The zero number representation */
    static Number zero() { return Number(0); }

    /** @brief The max quint8 representation */
    static Number maxUint8() { return Number(std::numeric_limits<quint8>::max()); }

    /** @brief The min quint8 representation */
    static Number minUint8() { return Number(std::numeric_limits<quint8>::min()); }

    /** @brief The max qint8 representation */
    static Number maxInt8() { return Number(std::numeric_limits<qint8>::max()); }

    /** @brief The min qint8 representation */
    static Number minInt8() { return Number::fromInt8(std::numeric_limits<qint8>::min()); }

    /** @brief The max quint16 representation */
    static Number maxUint16() { return Number(std::numeric_limits<quint16>::max()); }

    /** @brief The min quint16 representation */
    static Number minUint16() { return Number(std::numeric_limits<quint16>::min()); }

    /** @brief The max qint16 representation */
    static Number maxInt16() { return Number(std::numeric_limits<qint16>::max()); }

    /** @brief The min qint16 representation */
    static Number minInt16() { return Number::fromInt16(std::numeric_limits<qint16>::min()); }

    /** @brief The max quint32 representation */
    static Number maxUint32() { return Number(std::numeric_limits<quint32>::max()); }

    /** @brief The min quint32 representation */
    static Number minUint32() { return Number(std::numeric_limits<quint32>::min()); }

    /** @brief The max qint32 representation */
    static Number maxInt32() { return Number(std::numeric_limits<qint32>::max()); }

    /** @brief The min qint32 representation */
    static Number minInt32() { return Number::fromInt32(std::numeric_limits<qint32>::min()); }

    /** @brief The min qint64 representation */
    static Number maxInt64() { return Number(std::numeric_limits<qint64>::max()); }

    /** @brief The max qint64 representation */
    static Number minInt64() { return Number::fromInt64(std::numeric_limits<qint64>::min()); }

private:
    /** @brief Transform the number to integer
        @param ok If a problem occurred when trying to transform the integer, and if the param is
                  not null, ok will be set to false (or true if no problem occurred)
        @param commaShiftToRound The comma shift to apply on Number
                                 Example :
                                    If Number: 1234.56789, and commaShift = 0; result = 1235
                                    If Number: 1234.56789, and commaShift = 1; result = 12346
                                    If Number: -1234.5678, and commaShift = 0; result = -1235
                                    If Number: -1234.5678, and commaShift = 1; result = -12346
                                    If Number: 123.45, and commaShift = 1; result = 1235
                                    If Number: 123, and commaShift = 1; result = 1230
        @return The number transformed, or 0 if a problem occurred */
    template<class T>
    T toInteger(bool *ok = nullptr, quint8 commaShiftToRound = 0) const;

private:
    /** @brief Parse a decimal QString to a Number
        @note The string representation of the number, must be formatted like this:
                     *  123456
                     *  12345.566
                     *  12345,666
                     *  -123456
                     *  -123456.256
                     *  -123456,256
        @note If the string is not well formatted, the number returned will be invalid
        @param number The number string representation to parse
        @return The number parsed thanks to the string given */
    static Number fromDecString(const QString &number);

    /** @brief Parse an hexadecimal QString to a Number
        @note The string representation of the number is parsed by QString methods,
        @note If the string is not well formatted, the number returned will be invalid
        @param number The number string representation to parse
        @return The number parsed thanks to the string given */
    static Number fromHexString(const QString &number);

    /** @brief Multiply the value given with the power of ten:
                result = value * 10^powerOfTen
        @param value The value to multiply
        @param powerOfTen The power of ten to multiply the value with
        @param result The result of the multiplication will be set to this param
        @return False if the operation overflows the limits */
    static bool multiplyWithPowerOfTen(quint64 value, quint8 powerOfTen, quint64 &result);

    /** @brief Multiply the number with the power of ten given:
                result = value * 10^powerOfTen
        @note This method supports negative power of ten
        @param base The value to multiply
        @param powerOfTen The power of ten to multiply the value with
        @return The result of the operation, or invalid if a problem occurred */
    static Number multiplyWithPowerOfTen(const Number& base, qint8 powerOfTen);

    /** @brief Calculate the power of ten:
                result = 10^powerOfTen
        @param powerOfTen The power of ten to get its value from
        @param result The result of the calculation
        @return False if the operation overflows the limits */
    static bool calculatePowerOfTen(quint8 powerOfTen, quint64 &result);

    /** @brief Test if it's possible to multiply the two numbers without exceeds the limits
        @param number The first number of the multiplication
        @param otherNumber The second number of the multiplication
        @return True if the two numbers can be multiplied together without exceeds the limits */
    static bool testIfPossibleToMultiply(quint64 number, quint64 otherNumber);

    /** @brief Test if it's possible to add the two numbers without exceeds the limits
        @param number The first number of the addition
        @param otherNumber The second number of the addition
        @return True if the two numbers can be added together without exceeds the limits */
    static bool testIfPossibleToAdd(quint64 number, quint64 otherNumber);

    /** @brief Add or substract two numbers
        @note Test if the numbers are valids and the case with infinity
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, the number returned will be set to invalid
        @see Number::addOrSubMethod
        @param base The first number of the operation
        @param other The number to add or substract
        @param add True if the operation is an addition,
                   False if the operation is a substraction
        @return The result of the operation */
    static Number operatorAddOrSub(const Number &base,
                                   const Number &other,
                                   bool add = true);

    /** @brief Divides two numbers
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, the number returned will be set to invalid
        @param base The first number of the operation
        @param other The number to divide
        @return The result of the operation */
    static Number operatorDivide(const Number &base,
                                 const Number &other);

    /** @brief Multiplies two numbers
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, the number returned will be set to invalid
        @param base The first number of the operation
        @param other The number to multiply
        @return The result of the operation */
    static Number operatorMultiply(const Number &base,
                                   const Number &other);

    /** @brief Get the modulo value of the two numbers dividing
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, the number returned will be set to invalid
        @param base The first number of the operation
        @param other The number which divides
        @return The result of the operation */
    static Number operatorModulo(const Number &base,
                                 const Number &other);

    /** @brief Apply pow on the given number
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, the number returned will be set to invalid
        @param base The number to apply the pow on
        @param power The power to apply on the number
        @return The result of the operation */
    static Number operatorPow(const Number &base, qint8 power);

    /** @brief Compare two numbers together, do the following test: <, =<, > and >=
        @param base The left part of the comparison
        @param other The rigth part of the comparison
        @param baseGreatherThanOther If set to True, one of those operations can be done:
                                     base >= other or base > other
                                     If set to False, one of those operations can be done:
                                     base <= other or base < other
        @param canBeEqual If set to True, one of those operations can be done:
                          base >= other or base <= other
                          If set to False, one of those operations can be done:
                          base > other or base < other
        @return False if a problem occurs or if the comparison is wrong */
    static bool compare(const Number &base,
                        const Number &other,
                        bool baseGreatherThanOther,
                        bool canBeEqual);

    /** @brief Add or substract two valid and not infinite numbers
        @note If we overflow the limits, the numbers are already invalids or if a problem
              occurred, the number returned will be set to invalid
        @see Number::operatorAddOrSub
        @param base The first number of the operation
        @param other The number to add or substract
        @param add True if the operation is an addition,
                   False if the operation is a substraction
        @param result The result of the operation
        @return False if a problem occurs or if the limits overflow */
    static bool addOrSubMethod(const Number &base,
                               const Number &other,
                               Number &result,
                               bool add = true);

    /** @brief Try to change the number of digits after the comma
        @param number The number to use its value and commaShift
        @param numberAfterComma The new value for number after comma, this number has to be
                                higher than the current comma shift
        @param newValue The new value linked to comma shift asked
        @return False if we overflow the limits */
    static bool changeValuePrecision(const Number &number,
                                     quint8 numberAfterComma,
                                     quint64 &newValue);

    /** @brief Try to change the number of digits after the comma
        @param commaShift The commaShift to deal with
        @param value The value to deal with
        @param numberAfterComma The new value for number after comma, this number has to be
                                higher than the current comma shift
        @param newValue The new value linked to comma shift asked
        @return False if we overflow the limits */
    static bool changeValuePrecision(quint8 commaShift,
                                     quint64 value,
                                     quint8 numberAfterComma,
                                     quint64 &newValue);

    /** @brief Parse a number from a float or a double
        @note If the float or the double overflows the limits, the number returned will be
              invalid
        @param number The float or double number to parse
        @param commaShift Give the number of digits we want after the comma
        @return The number parsed */
    template<class T>
    static Number fromFloatOrDouble(T number, quint8 commaShift);

    /** @brief Parse a number from a qint8, quint8, qint16, quint16, qint32, quint32,
               qint64, quint64
        @param number The integer number
        @return The number parsed */
    template<class T>
    static Number fromInteger(T number);

    /** @brief Transform a number to a qint8, quint8, qint16, quint16, qint32, quint32,
               qint64, quint64
        @param number The number to get integer from
        @param commaShiftToRound The comma shift to apply on Number
                                 Example :
                                    If Number: 1234.56789, and commaShift = 0; result = 1235
                                    If Number: 1234.56789, and commaShift = 1; result = 1234,6
                                    If Number: -1234.5678, and commaShift = 0; result = -1235
                                    If Number: -1234.5678, and commaShift = 1; result = -1234,6
                                    If Number: 123.45, and commaShift = 1; result = 1235
                                    If Number: 123, and commaShift = 1; result = 1230
        @param result The result returned
        @return True if no problem occurs */
    template<class T>
    static bool toInteger(const Number &number, quint8 commaShiftToRound, T &result);

    /** @brief Trim the number value with commaShift in order that: 2.000 and 2 are the same
               number.
        @param value The value to deal with
        @param commaShift The commaShift to deal with
        @param valueTrimmed The value cleaned
        @param commaShift The commaShift to deal with
        @return True If no problem occurs */
    static bool trimNumber(quint64 value,
                           quint8 commaShift,
                           quint64 &valueTrimmed,
                           quint8 &commaShiftTrimmed);

public:
    /** @brief The positive infinite number representation */
    static const Number PositiveInfinite;

    /** @brief The positive infinite number representation */
    static const Number NegativeInfinite;

    /** @brief The invalid number representation */
    static const Number Invalid;

    /** @brief Max allowed comma shift, to avoid to have the integer part too small */
    static const constexpr quint8 MaxAllowedCommaShift = 6;

private:
    /** @brief The max power of ten supported (after it will overflow the limits) */
    static const constexpr quint8 MaxPowerOfTenSupported = 18;

    /** @brief Used to parse a decimal and float number from a string */
    static const QRegularExpression DecNumberRegExpression;

    /** @brief Used to parse an hex number from a string */
    static const QRegularExpression HexNumberRegExpression;

    /** @brief Text representation of positive infinite  */
    static const QString PositiveStrInfinite;

    /** @brief Text representation of negative infinite  */
    static const QString NegativeStrInfinite;

    static const constexpr int DeciBase = 10;
    static const constexpr int HexBase = 16;

private:
    quint64 _value{0};
    quint8 _commaShift{0};
    bool _positive{true};
    bool _valid{false};
};

template<class T>
T Number::toInteger(bool *ok, quint8 commaShiftToRound) const
{
    static_assert(std::is_same<quint8, T>::value   || std::is_same<qint8, T>::value   ||
                  std::is_same<quint16, T>::value  || std::is_same<qint16, T>::value  ||
                  std::is_same<quint32, T>::value  || std::is_same<qint32, T>::value  ||
                  std::is_same<quint64, T>::value  || std::is_same<qint64, T>::value,
                  "T must be a signed or unsigned integer");

    T result = 0;

    bool success = toInteger(*this, commaShiftToRound, result);

    if(ok)
    {
        *ok = success;
    }

    return result;
}

template<class T>
Number Number::fromInteger(T number)
{
    static_assert(std::is_same<quint8, T>::value   || std::is_same<qint8, T>::value   ||
                  std::is_same<quint16, T>::value  || std::is_same<qint16, T>::value  ||
                  std::is_same<quint32, T>::value  || std::is_same<qint32, T>::value  ||
                  std::is_same<quint64, T>::value  || std::is_same<qint64, T>::value,
                  "T must be a signed or unsigned integer");

    Number result;
    result._valid = true;

    if(number >= 0)
    {
        result._value = static_cast<quint64>(number);
        result._positive = true;
    }
    // The qAbs can cause problems in the limit cases, example:
    // With qint8, if you try to change the sigdness of -128, the templated method qAbs will
    // return -128, because 128 doesn't exist in qint8
    else if((std::is_same<qint32, T>::value || std::is_same<qint64, T>::value)
             && (number == std::numeric_limits<T>::min()))
    {
        result._value = static_cast<quint64>(std::numeric_limits<T>::max() + 1ULL);
        result._positive = false;
    }
    else
    {
        WARNING_INT_MINUS_PUSH
        result._value = static_cast<quint64>(-number);
        WARNING_INT_MINUS_POP
        result._positive = false;
    }

    result._commaShift = 0;

    return result;
}

template<class T>
bool Number::toInteger(const Number &number, quint8 commaShiftToRound, T &result)
{
    static_assert(std::is_same<quint8, T>::value   || std::is_same<qint8, T>::value   ||
                  std::is_same<quint16, T>::value  || std::is_same<qint16, T>::value  ||
                  std::is_same<quint32, T>::value  || std::is_same<qint32, T>::value  ||
                  std::is_same<quint64, T>::value  || std::is_same<qint64, T>::value,
                  "T must be a signed or unsigned integer");

    if(commaShiftToRound > MaxAllowedCommaShift)
    {
        qWarning() << "The commaShift: " << commaShiftToRound << ", is not supported";
        return false;
    }

    if(!number._valid)
    {
        qWarning() << "The number is invalid";
        return false;
    }

    if(number._value == 0)
    {
        // Shortcut for 0
        result = 0;
        return true;
    }

    if(number.isNegative() && (std::is_same<quint8, T>::value   ||
                               std::is_same<quint16, T>::value  ||
                               std::is_same<quint32, T>::value  ||
                               std::is_same<quint64, T>::value))
    {
        qWarning() << "The number is negative and so it can't parsed to unsigned integer";
        return false;
    }

    bool toDivide;
    quint8 commaShiftOfResult;

    if(number._commaShift >= commaShiftToRound)
    {
        toDivide = true;
        commaShiftOfResult = number._commaShift - commaShiftToRound;
    }
    else
    {
        toDivide = false;
        commaShiftOfResult = commaShiftToRound - number._commaShift;
    }

    quint64 tmpResult = number._value;

    if(commaShiftOfResult > 0)
    {
        quint64 commaShiftResult = 0;

        if(!calculatePowerOfTen(commaShiftOfResult, commaShiftResult))
        {
            return false;
        }

        if(toDivide)
        {
            tmpResult = (tmpResult + (commaShiftResult / 2)) / commaShiftResult;
        }
        else if(!testIfPossibleToMultiply(tmpResult, commaShiftResult))
        {
            qWarning() << "Can't round to this comma shift, we overflow the number limit";
            return false;
        }
        else
        {
            tmpResult = tmpResult * commaShiftResult;
        }
    }

    WARNING_SIGNED_MISMATCH_PUSH
    if(number.isPositive() && tmpResult > std::numeric_limits<T>::max())
    WARNING_SIGNED_MISMATCH_POP
    {
        qWarning() << "The number overflows the numeric max limits";
        return false;
    }
    else if(number.isNegative() &&
             (tmpResult > static_cast<quint64>(std::numeric_limits<T>::max() + 1ULL)))
    {
        // If we are here (because of the previous test), we are dealing with signed integer
        qWarning() << "The number overflows the numeric min limits";
        return false;
    }

    if(number.isNegative())
    {
        WARNING_INT_MINUS_PUSH
        result = static_cast<T>(- tmpResult);
        WARNING_INT_MINUS_POP
        return true;
    }

    result = static_cast<T>(tmpResult);
    return true;
}

template<class T>
Number Number::fromFloatOrDouble(T number, quint8 commaShift)
{
    static_assert(std::is_same<double, T>::value || std::is_same<float, T>::value,
                  "T must be a float or a double");

    if(commaShift > MaxAllowedCommaShift)
    {
        qWarning() << "The commaShift: " << commaShift << ", is not supported";
        return {};
    }

    if(qIsNaN(number))
    {
        return {};
    }

    if(qIsNull(number))
    {
        return Number(0, 0);
    }

    if(qIsInf(number))
    {
        return (number > 0) ? PositiveInfinite : NegativeInfinite;
    }

    Number result;

    result._valid = true;

    if(qAbs(number) > std::numeric_limits<qint64>::max())
    {
        qWarning() << "The float is too big: " << number;
        return {};
    }

    quint64 powerOfTen;

    if(!calculatePowerOfTen(commaShift, powerOfTen))
    {
        qWarning() << "The power of ten asked is too big: " << commaShift;
        return {};
    }

    qint64 intPart = qint64(number);
    T fDecPart = number - intPart;

    quint64 decPart = static_cast<quint64>(qAbs(qRound64(fDecPart * powerOfTen)));

    quint8 newCommaShift = 0;
    if(!trimNumber(decPart, commaShift, decPart, newCommaShift))
    {
        qWarning() << "A problem occurred when tryied to trim the decimal part of the double/float "
                   << "number: " << number;
        return {};
    }

    if(newCommaShift != commaShift && !calculatePowerOfTen(newCommaShift, powerOfTen))
    {
        // Recalculate the power of ten
        qWarning() << "The new power of ten asked is too big: " << newCommaShift;
        return {};
    }

    result._commaShift = newCommaShift;
    result._value = static_cast<quint64>(qAbs(intPart));

    if(!testIfPossibleToMultiply(result._value, powerOfTen))
    {
        qWarning() << "The float is too big: " << number;
        return {};
    }

    result._value *= powerOfTen;

    if(!testIfPossibleToAdd(result._value, decPart))
    {
        qWarning() << "The float is too big: " << number;
        return {};
    }

    result._value += decPart;

    result._positive = (number >= 0);

    return result.trimNumber();
}

/** @brief Return a qHash for Number
    @param number The class to get the qHash from
    @param seed The seed to add in the qHash
    @return The hash created */
inline uint qHash(const Number &number, uint seed)
{
    return qHash(number.toString(), seed);
}

/** @brief Allow to display the object instance in debug console */
QDebug operator<<(QDebug stream, const Number &number);

Q_DECLARE_METATYPE(Number)
Q_DECLARE_METATYPE(QList<Number>)
Q_DECLARE_METATYPE(QVector<Number>)
