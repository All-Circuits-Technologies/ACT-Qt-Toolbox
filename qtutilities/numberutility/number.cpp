// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "number.hpp"

#include <cmath>
#include <QMetaType>
#include <QRegularExpression>

#include "byteutility/bytearrayhelper.hpp"

const Number Number::PositiveInfinite = Number(std::numeric_limits<quint64>::max());
const Number Number::NegativeInfinite = Number(std::numeric_limits<quint64>::max(), 0, false);
const Number Number::Invalid = Number();
const QRegularExpression Number::DecNumberRegExpression =
    QRegularExpression("^(-)?([0-9]+)([\\.,]([0-9]+))?$");
const QRegularExpression Number::HexNumberRegExpression =
    QRegularExpression("^0((x)|(X))([0-9a-fA-F]+)$");
const QString Number::PositiveStrInfinite = "+Inf";
const QString Number::NegativeStrInfinite = "-Inf";


Number::Number(const QString &value)
{
    *this = fromString(value);
}

Number::Number(const Number &otherNumber) :
    _value(otherNumber._value),
    _commaShift(otherNumber._commaShift),
    _positive(otherNumber._positive),
    _valid(otherNumber._valid)
{
}

Number::~Number()
{
}

Number::Number(quint64 value, quint8 commaShift, bool positiveNumber)
{
    if(commaShift > MaxAllowedCommaShift)
    {
        qWarning() << "The commaShift can't be superior to: " << MaxAllowedCommaShift;
        _valid = false;
        return;
    }

    _positive = positiveNumber;

    _valid = trimNumber(value, commaShift, _value, _commaShift);
}

bool Number::isValid() const
{
    return _valid;
}

void Number::clear()
{
    _valid = false;
}

quint64 Number::getDecimalPart() const
{
    quint64 powerOfTen = 1;

    if(!calculatePowerOfTen(_commaShift, powerOfTen))
    {
        qWarning() << "The commaShift has an incorrect value: " << _commaShift;
        return 0;
    }

    return _value % powerOfTen;
}

quint64 Number::getDecimalDivisor() const
{
    quint64 powerOfTen = 1;

    if(!calculatePowerOfTen(_commaShift, powerOfTen))
    {
        qWarning() << "The commaShift has an incorrect value: " << _commaShift;
        return 0;
    }

    return powerOfTen;
}

quint64 Number::getIntegerPart() const
{
    quint64 powerOfTen = 1;

    if(!calculatePowerOfTen(_commaShift, powerOfTen))
    {
        qWarning() << "The commaShift has an incorrect value: " << _commaShift;
        return 0;
    }

    // Truncation is operated
    return _value / powerOfTen;
}

quint8 Number::toUint8(bool *ok, quint8 commaShiftToRound) const
{
    return toInteger<quint8>(ok, commaShiftToRound);
}

qint8 Number::toInt8(bool *ok, quint8 commaShiftToRound) const
{
    return toInteger<qint8>(ok, commaShiftToRound);
}

quint16 Number::toUint16(bool *ok, quint8 commaShiftToRound) const
{
    return toInteger<quint16>(ok, commaShiftToRound);
}

qint16 Number::toInt16(bool *ok, quint8 commaShiftToRound) const
{
    return toInteger<qint16>(ok, commaShiftToRound);
}

quint32 Number::toUint32(bool *ok, quint8 commaShiftToRound) const
{
    return toInteger<quint32>(ok, commaShiftToRound);
}

qint32 Number::toInt32(bool *ok, quint8 commaShiftToRound) const
{
    return toInteger<qint32>(ok, commaShiftToRound);
}

quint64 Number::toUint64(bool *ok, quint8 commaShiftToRound) const
{
    return toInteger<quint64>(ok, commaShiftToRound);
}

qint64 Number::toInt64(bool *ok, quint8 commaShiftToRound) const
{
    return toInteger<qint64>(ok, commaShiftToRound);
}

bool Number::isInfinite() const
{
    return((_commaShift == 0) && _valid && (_value == std::numeric_limits<quint64>::max()));
}

bool Number::isPositiveInfinite() const
{
    return isInfinite() && isPositive();
}

bool Number::isNegativeInfinite() const
{
    return isInfinite() && isNegative();
}

QString Number::toString() const
{
    if(!_valid)
    {
        return {};
    }

    if(isInfinite())
    {
        if(isPositive())
        {
            return PositiveStrInfinite;
        }

        return NegativeStrInfinite;
    }

    QString value = QString::number(_value, Number::DeciBase);

    if(_commaShift > 0)
    {
        //: Character used as decimal separator
        value.insert(value.length() - _commaShift, QObject::tr(","));
    }

    if(!_positive)
    {
        value.prepend('-');
    }

    return value;
}

Number &Number::round(quint8 commaShiftToRound)
{
    if(!isValid())
    {
        qWarning() << "The number is invalid, can't round it";
        return *this;
    }

    if(isInfinite())
    {
        qWarning() << "Can't round infinite";
        return *this;
    }

    if(commaShiftToRound > MaxAllowedCommaShift)
    {
        qWarning() << "Cannot round to the commaShift: " << commaShiftToRound << ", the limit max "
                   << "is: " << MaxAllowedCommaShift;
        _valid = false;
        return *this;
    }

    // When we are, we know that the function trimNumber has previously already been called
    if(_commaShift <= commaShiftToRound)
    {
        // Nothing to round here
        return *this;
    }

    quint64 powerOfTen;
    if(!calculatePowerOfTen((_commaShift - commaShiftToRound), powerOfTen))
    {
        _valid = false;
        return *this;
    }

    _value = static_cast<quint64>(((_value + (powerOfTen / 2)) / powerOfTen));
    _commaShift = commaShiftToRound;

    return trimNumber();
}

Number &Number::trimNumber()
{
    if(!isValid())
    {
        qWarning() << "The number is invalid, can't trim it";
        return *this;
    }

    if(!trimNumber(_value, _commaShift, _value, _commaShift))
    {
        _valid = false;
    }

    return *this;
}

Number &Number::operator=(const Number &otherNumber)
{
    _value      = otherNumber._value;
    _commaShift = otherNumber._commaShift;
    _positive   = otherNumber._positive;
    _valid      = otherNumber._valid;

    return *this;
}

Number Number::operator-() const
{
    if(!isValid())
    {
        qWarning() << "The number is invalid, can't inverse its sign";
        return {};
    }

    return Number(_value, _commaShift, !_positive);
}

Number Number::operator+(const Number &otherNumber) const
{
    return operatorAddOrSub(*this, otherNumber, true);
}

void Number::operator+=(const Number &otherNumber)
{
    *this = operatorAddOrSub(*this, otherNumber, true);
}

Number Number::operator-(const Number &otherNumber) const
{
    return operatorAddOrSub(*this, otherNumber, false);
}

void Number::operator-=(const Number &otherNumber)
{
    *this = operatorAddOrSub(*this, otherNumber, false);
}

Number Number::operator/(const Number &otherNumber) const
{
    return operatorDivide(*this, otherNumber);
}

void Number::operator/=(const Number &otherNumber)
{
    *this = operatorDivide(*this, otherNumber);
}

Number Number::operator*(const Number &otherNumber) const
{
    return operatorMultiply(*this, otherNumber);
}

void Number::operator*=(const Number &otherNumber)
{
    *this = operatorMultiply(*this, otherNumber);
}

Number Number::operator%(const Number &otherNumber) const
{
    return operatorModulo(*this, otherNumber);
}

void Number::operator%=(const Number &otherNumber)
{
    *this = operatorModulo(*this, otherNumber);
}

bool Number::operator>(const Number &otherNumber) const
{
    return compare(*this, otherNumber, true, false);
}

bool Number::operator>=(const Number &otherNumber) const
{
    return compare(*this, otherNumber, true, true);
}

bool Number::operator<(const Number &otherNumber) const
{
    return compare(*this, otherNumber, false, false);
}

bool Number::operator<=(const Number &otherNumber) const
{
    return compare(*this, otherNumber, false, true);
}

bool Number::operator==(const Number &otherNumber) const
{
    return ((_valid      == otherNumber._valid)       &&
            (_value      == otherNumber._value)       &&
            (_commaShift == otherNumber._commaShift)  &&
            (_positive   == otherNumber._positive));
}

bool Number::operator!=(const Number &otherNumber) const
{
    return ((_valid      != otherNumber._valid)       ||
            (_value      != otherNumber._value)       ||
            (_commaShift != otherNumber._commaShift)  ||
            (_positive   != otherNumber._positive));
}

Number &Number::pow(qint8 power)
{
    *this = operatorPow(*this, power);
    return *this;
}

Number &Number::invert()
{
    *this = operatorDivide(Number(1), *this);
    return * this;
}

Number &Number::multiplyWithPowerOfTen(qint8 power)
{
    *this = multiplyWithPowerOfTen(*this, power);
    return *this;
}

Number Number::fromString(const QString &number)
{
    if(number.isEmpty())
    {
        return {};
    }

    if(number == PositiveStrInfinite)
    {
        return PositiveInfinite;
    }

    if(number == NegativeStrInfinite)
    {
        return NegativeInfinite;
    }

    QRegularExpressionMatch hexMatch = HexNumberRegExpression.match(number);

    if(hexMatch.hasMatch())
    {
        return fromHexString(number);
    }

    return fromDecString(number);
}

Number Number::fromHex(const QByteArray &number, bool isLsbFirst)
{
    const int numberSize = number.size();
    if(numberSize > sizeof(quint64))
    {
        qWarning() << "A problem occurred when tryied to parse the QByteArray: " << number
                   << ", to hex number ; the size of QByteArray overflows the max size of number";
        return {};
    }

    quint64 value = 0;
    if(!ByteArrayHelper::toUintHex(number, value, isLsbFirst))
    {
        qWarning() << "A problem occurred when tryied to parse the QByteArray: " << number
                   << ", to unit64";
        return {};
    }

    Number result;
    result._valid = true;
    result._commaShift = 0;
    result._value = value;

    return result.trimNumber();
}

Number Number::fromDecString(const QString &number)
{
    QRegularExpressionMatchIterator matchIterator = DecNumberRegExpression.globalMatch(number);

    QStringList numberParts;

    while(matchIterator.hasNext())
    {
        QRegularExpressionMatch match = matchIterator.next();

        // First result it's all pattern
        for(int idx = 1; idx <= match.lastCapturedIndex(); idx++)
        {
            QString captured = match.captured(idx);

            if(!captured.isEmpty())
            {
                numberParts << captured;
            }
        }
    }

    // Can be:
    // 1 elems:  1234
    // 2 elems: -1234
    // 3 elems:  1234.567
    // 4 elems: -1234.567
    if(numberParts.length() < 1 || numberParts.length() > 4)
    {
        qWarning() << "The string given isn't a number: " << number;
        return {};
    }

    bool ok = false;

    Number result;
    result._valid = true;
    result._commaShift = 0;

    if(numberParts.at(0) == "-")
    {
        result._positive = false;
        numberParts.removeFirst();
    }

    const QString &strIntPart = numberParts.at(0);

    result._value = strIntPart.toULongLong(&ok);

    if(!ok)
    {
        qWarning() << "A problem occurred when trying to parse the integer part: " << strIntPart;
        return {};
    }

    // There is a decimal part
    if(numberParts.length() == 3)
    {
        // Index 0: it's integer part
        // Index 1: it's the dot + decimal part
        // Index 2: its the decimal part only
        // This is due to the RegexEx
        const QString &strDecPart = numberParts.at(2);

        if(strDecPart.length() > MaxAllowedCommaShift)
        {
            qWarning() << "There are too much numbers after the comma, can't proceed, the number: "
                       << number;
            return {};
        }

        result._commaShift = static_cast<quint8>(strDecPart.length());

        quint64 decPart = strDecPart.toUInt(&ok);

        if(!ok)
        {
            qWarning() << "A problem occurred when trying to parse the decimal part: " << strDecPart;
            return {};
        }

        if(!multiplyWithPowerOfTen(result._value, result._commaShift, result._value) ||
            !testIfPossibleToAdd(result._value, decPart))
        {
            qWarning() << "The number is too big: " << number;
            return {};
        }

        result._value += decPart;
    }

    return result.trimNumber();
}

Number Number::fromHexString(const QString &number)
{
    bool ok = false;
    quint64 value = number.toLower().toULongLong(&ok, Number::HexBase);

    if(!ok)
    {
        qWarning() << "A problem occurred when tryied to parse the string: " << number << ", to hex "
                   << "number";
        return {};
    }

    Number result;
    result._valid = true;
    result._commaShift = 0;
    result._value = value;

    return result.trimNumber();
}

void Number::RegisterMetaTypes()
{
    qRegisterMetaType<Number>();

    if(!QMetaType::registerConverter<Number,QString>(&Number::toString))
    {
        qWarning() << "Register converter from Number to QString failed";
    }

    if(!QMetaType::registerConverter<QString,Number>(&Number::fromString))
    {
        qWarning() << "Register converter from QString to Number failed";
    }

    if(!QMetaType::registerConverter<float,Number>(qOverload<float>(&Number::fromFloat)))
    {
        qWarning() << "Register converter from float to Number failed";
    }

    if(!QMetaType::registerConverter<double,Number>(qOverload<double>(&Number::fromDouble)))
    {
        qWarning() << "Register converter from double to Number failed";
    }

    if(!QMetaType::registerConverter<qint8,Number>(&Number::fromInt8))
    {
        qWarning() << "Register converter from qint8 to Number failed";
    }

    if(!QMetaType::registerConverter<quint8,Number>(&Number::fromUInt8))
    {
        qWarning() << "Register converter from quint8 to Number failed";
    }

    if(!QMetaType::registerConverter<qint16,Number>(&Number::fromInt16))
    {
        qWarning() << "Register converter from qint16 to Number failed";
    }

    if(!QMetaType::registerConverter<quint16,Number>(&Number::fromUInt16))
    {
        qWarning() << "Register converter from quint16 to Number failed";
    }

    if(!QMetaType::registerConverter<qint32,Number>(&Number::fromInt32))
    {
        qWarning() << "Register converter from qint32 to Number failed";
    }

    if(!QMetaType::registerConverter<quint32,Number>(&Number::fromUInt32))
    {
        qWarning() << "Register converter from quint32 to Number failed";
    }

    if(!QMetaType::registerConverter<qint64,Number>(&Number::fromInt64))
    {
        qWarning() << "Register converter from qint64 to Number failed";
    }

    if(!QMetaType::registerConverter<quint64,Number>(&Number::fromUInt64))
    {
        qWarning() << "Register converter from quint64 to Number failed";
    }

    if(!QMetaType::registerComparators<Number>())
    {
        qWarning() << "Register comparators for number failed";
    }
}

bool Number::multiplyWithPowerOfTen(quint64 value, quint8 powerOfTen, quint64 &result)
{
    quint64 multiple = 1;

    if(!calculatePowerOfTen(powerOfTen, multiple))
    {
        return false;
    }

    if(!testIfPossibleToMultiply(value, multiple))
    {
        qWarning() << "Cannot multiply the two values (exceeds the quint64 limits): " << value
                   << " and " << multiple;
        return false;
    }

    result = multiple * value;

    return true;
}

Number Number::multiplyWithPowerOfTen(const Number &base, qint8 powerOfTen)
{
    if(!base.isValid())
    {
        qWarning() << "Can't multiply with power of ten, the number is invalid";
        return {};
    }

    if(base._value == 0 || base.isInfinite() || powerOfTen == 0)
    {
        return base;
    }

    if(powerOfTen > 0)
    {
        quint8 posPowerTen = static_cast<quint8>(powerOfTen);
        Number result(base);

        if(base._commaShift >= powerOfTen)
        {
            // In that case we only change the comma shift
            result._commaShift = base._commaShift - posPowerTen;
            return result;
        }

        quint8 truePowerAfterCommaShift = posPowerTen - base._commaShift;
        if(!multiplyWithPowerOfTen(base._value, truePowerAfterCommaShift, result._value))
        {
            qWarning() << "Can't multiply the number: " << base << ", with power of ten: "
                       << powerOfTen;
            return {};
        }

        return result;
    }

    quint8 posPowerTen = static_cast<quint8>(-powerOfTen);

    quint64 powerOfTenValue = 1;
    if(!calculatePowerOfTen(posPowerTen, powerOfTenValue))
    {
        qWarning() << "The power of ten: " << powerOfTen << ", isn't supported";
        return {};
    }

    return operatorDivide(base, Number(powerOfTenValue));
}

bool Number::calculatePowerOfTen(quint8 powerOfTen, quint64 &result)
{
    if(powerOfTen > MaxPowerOfTenSupported)
    {
        qWarning() << "Cannot calculate the power of ten of: " << powerOfTen << ", the limit max "
                   << "is: " << MaxPowerOfTenSupported;
        return false;
    }

    result = 1;

    for(int idx = 1; idx <= powerOfTen; idx++)
    {
        result *= Number::DeciBase;
    }

    return true;
}

bool Number::testIfPossibleToMultiply(quint64 number, quint64 otherNumber)
{
    return (std::numeric_limits<quint64>::max() / otherNumber) >= number;
}

bool Number::testIfPossibleToAdd(quint64 number, quint64 otherNumber)
{
    return ((std::numeric_limits<quint64>::max() - number) >= otherNumber);
}

Number Number::operatorAddOrSub(const Number &base, const Number &other, bool add)
{
    if(!base.isValid() || !other.isValid())
    {
        qWarning() << "Can't added the numbers, one is invalid";
        return {};
    }

    if(base.isInfinite() && other.isInfinite())
    {
        // For add method:
        // x = Inf + (-Inf) or x = (-Inf) + Inf -> indeterminate
        // For sub method:
        // x = Inf - Inf or x = (-Inf) - (-Inf) -> indeterminate
        if((add  && (base.isPositive() != other.isPositive())) ||
            (!add && (base.isPositive() == other.isPositive())))
        {
            qWarning() << "Can't added the number, the sign of the infinites numbers will give an"
                          " indeterminate state";
            return {};
        }

        return base;
    }

    if(base.isInfinite())
    {
        return base;
    }

    if(other.isInfinite())
    {
        return other;
    }

    Number result;

    if(!addOrSubMethod(base, other, result, add))
    {
        return {};
    }

    return result;
}

Number Number::operatorDivide(const Number &base, const Number &other)
{
    if(!base.isValid() || !other.isValid() || other.isInfinite() || (other._value == 0))
    {
        qWarning() << "Can't divide the numbers, one of them is invalid, or the divisor is equal "
                      "to zero or infinite";
        return {};
    }

    if(base._value == 0 || base.isInfinite())
    {
        return base;
    }

    quint64 baseValue   = base._value;
    bool basePositive = base._positive;
    quint64 otherValue  = other._value;
    bool otherPositive = other._positive;

    quint8 maxCommaShift = qMax(base._commaShift, other._commaShift);

    Number result;

    if(maxCommaShift > base._commaShift)
    {
        if(!changeValuePrecision(base, maxCommaShift, baseValue))
        {
            return {};
        }
    }
    else if(maxCommaShift > other._commaShift)
    {
        if(!changeValuePrecision(other, maxCommaShift, otherValue))
        {
            return {};
        }
    }

    quint8 commaShiftIdx = 0;
    result._value = baseValue / otherValue;

    // Loop until the maximum precision is reached or until we find an empty division rest
    while(baseValue % otherValue != 0 && commaShiftIdx < MaxAllowedCommaShift)
    {
        // Increase the required precision, and compute the next numerator
        commaShiftIdx++;
        result._value *= Number::DeciBase;
        result._value += ((baseValue % otherValue) * Number::DeciBase) / otherValue;
        baseValue *= Number::DeciBase;

        if(commaShiftIdx == MaxAllowedCommaShift &&
            (((baseValue % otherValue) * Number::DeciBase) / otherValue)
                >= static_cast<quint8>(MaxAllowedCommaShift - 1))
        {
            // The next digit is >=5, so we have to round up
            result._value += 1;
        }

        result._commaShift = commaShiftIdx;
    }

    // If (a is + and b is +) or (a is - and b is -) therefore result is +
    // If (a is + and b is -) or (a is - and b is +) therefore result is -
    result._positive = (basePositive == otherPositive);

    result._valid = true;

    return result.trimNumber();
}

Number Number::operatorMultiply(const Number &base, const Number &other)
{
    if(!base.isValid() || !other.isValid())
    {
        qWarning() << "Can't multiply the two numbers, one of them is invalid";
        return {};
    }

    if((base.isInfinite() && other._value == 0) || (base._value == 0 && other.isInfinite()))
    {
        qWarning() << "Can't multiply the two numbers, one is equal to zero and the other one is "
                      "infinite";
        return {};
    }

    if(other._value == 0 || base._value == 0)
    {
        return Number(0, 0);
    }

    bool basePositive = base._positive;
    bool otherPositive = other._positive;

    Number result;

    // If (a is + and b is +) or (a is - and b is -) therefore result is +
    // If (a is + and b is -) or (a is - and b is +) therefore result is -
    result._positive = (basePositive == otherPositive);

    if(base.isInfinite() || other.isInfinite())
    {
        result._value = PositiveInfinite._value;
        result._valid = true;
        return result;
    }

    if(!testIfPossibleToMultiply(base._value, other._value))
    {
        qWarning() << "Can't multiply the two numbers, the result overflows the limits";
        return {};
    }

    result._value = base._value * other._value;

    quint8 realFinalCommaShift = base._commaShift + other._commaShift;
    result._commaShift = realFinalCommaShift;

    if(realFinalCommaShift > MaxAllowedCommaShift)
    {
        result._commaShift = MaxAllowedCommaShift;

        quint64 powerOfTenToDivideWith;
        if(!calculatePowerOfTen(realFinalCommaShift - result._commaShift, powerOfTenToDivideWith))
        {
            // Can't be, because it's tested by maxCommaShift
            return {};
        }

        result._value = ((result._value + (powerOfTenToDivideWith - 1)) / powerOfTenToDivideWith);
    }

    result._valid = true;
    return result.trimNumber();
}

Number Number::operatorModulo(const Number &base, const Number &other)
{
    if(!base.isValid() || !other.isValid() || base.isInfinite() || other.isInfinite()||
        (other._value == 0))
    {
        qWarning() << "Can't do the modulo of the numbers, one of them is invalid, infinite or the "
                   << "divisor is equal to 0";
        return {};
    }

    if((base._value == other._value) || (other._value == 1) || (base._value == 0))
    {
        return Number(0);
    }

    quint8 maxCommaShift = qMax(base._commaShift, other._commaShift);

    Number result;

    quint64 baseValue   = base._value;
    quint64 otherValue  = other._value;

    if(maxCommaShift > base._commaShift)
    {
        if(!changeValuePrecision(base, maxCommaShift, baseValue))
        {
            return {};
        }
    }
    else if(maxCommaShift > other._commaShift)
    {
        if(!changeValuePrecision(other, maxCommaShift, otherValue))
        {
            return {};
        }
    }

    result._value = baseValue % otherValue;
    result._commaShift = maxCommaShift;

    // If (a is + and b is +) or (a is - and b is -) therefore result is +
    // If (a is + and b is -) or (a is - and b is +) therefore result is -
    result._positive = (base._positive == other._positive);

    result._valid = true;

    return result.trimNumber();
}

Number Number::operatorPow(const Number &base, qint8 power)
{
    if(!base.isValid())
    {
        qWarning() << "Can't pow the number, because it's invalid";
        return {};
    }

    if(power == 0)
    {
        return Number(1);
    }

    if(base._value == 0 || base.isPositiveInfinite())
    {
        return base;
    }

    if(base.isNegativeInfinite())
    {
        if((power % 2) == 0)
        {
            // This will produce a positive infinite
            return PositiveInfinite;
        }

        return NegativeInfinite;
    }

    Number ref(base);

    qint8 abstractPower;

    if(power > 0)
    {
        abstractPower = power;
    }
    else
    {
        abstractPower = -power;
        if(!ref.invert().isValid())
        {
            qWarning() << "A problem occurred when tried to apply the power: " << power << ", on "
                       << "the number: " << base;
            return {};
        }
    }

    Number tmp(ref);

    for(qint8 idx = 1; idx < abstractPower; ++idx)
    {
        tmp = operatorMultiply(tmp, ref);
        if(!tmp.isValid())
        {
            qWarning() << "Can't apply a pow: " << power << ", on the number: " << base
                       << ", a problem occurred at pow: " << (idx + 1);
            return {};
        }
    }

    return tmp;
}

bool Number::compare(const Number &base,
                     const Number &other,
                     bool baseGreatherThanOther,
                     bool canBeEqual)
{
    if((base._valid == other._valid)        &&
        (base._value == other._value)        &&
        (base._positive == other._positive)  &&
        (base._commaShift == other._commaShift))
    {
        return canBeEqual;
    }

    if(!base.isValid() || !other.isValid())
    {
        qWarning() << "Can't compare, if one element is invalid";
        return false;
    }

    // If the two numbers have a different sign, we can determine if it's greather or not
    if(base.isPositive() != other.isPositive())
    {
        return (baseGreatherThanOther == base.isPositive());
    }

    if(base.isInfinite() && other.isInfinite())
    {
        qWarning() << "Can't compare, two infinites number with the sign";
        return false;
    }

    // If the two elements are negatives, we exchange the type of test (because here we only play
    // with positive numbers)
    if(!base.isPositive())
    {
        baseGreatherThanOther = !baseGreatherThanOther;
    }

    if(base.isInfinite() || other.isInfinite())
    {
        return (baseGreatherThanOther == base.isInfinite());
    }

    quint64 baseInt = base.getIntegerPart();
    quint64 otherInt = other.getIntegerPart();

    if(baseInt == otherInt)
    {
        quint64 baseDec = base.getDecimalPart();
        quint64 otherDec = other.getDecimalPart();

        if(other._commaShift > base._commaShift)
        {
            if(!changeValuePrecision(base._commaShift, baseDec, other._commaShift, baseDec))
            {
                return false;
            }
        }
        else if(base._commaShift > other._commaShift)
        {
            if(!changeValuePrecision(other._commaShift, otherDec, base._commaShift, otherDec))
            {
                return false;
            }
        }

        return (baseGreatherThanOther == (baseDec > otherDec));
    }

    return (baseGreatherThanOther == (baseInt > otherInt));
}

bool Number::addOrSubMethod(const Number &base, const Number &other, Number &result, bool add)
{
    quint64 baseValue   = base._value;
    bool basePositive = base._positive;
    quint64 otherValue  = other._value;
    bool otherPositive = other._positive;

    quint8 maxCommaShift = qMax(base._commaShift, other._commaShift);


    if(maxCommaShift > base._commaShift)
    {
        if(!changeValuePrecision(base, maxCommaShift, baseValue))
        {
            return false;
        }
    }
    else if(maxCommaShift > other._commaShift)
    {
        if(!changeValuePrecision(other, maxCommaShift, otherValue))
        {
            return false;
        }
    }

    result._commaShift = maxCommaShift;

    // x = (-a) + b or x = (-a) - (-b)
    // Change it to: x = b - a
    if((!basePositive && otherPositive && add) || (!basePositive && !otherPositive && !add))
    {
        quint64 temp = baseValue;
        baseValue = otherValue;
        otherValue = temp;
        add = false;
    }
    // x = (-a) + (-b) or x = (-a) - b
    // Change it to x = - (a + b)
    else if((!basePositive && !otherPositive && add) || (!basePositive && otherPositive && !add))
    {
        add = true;
        result._positive = false;
    }
    // x = a + (-b) or x = a - b
    // Change it to x = a - b
    else if((basePositive && !otherPositive && add) || (basePositive && otherPositive && !add))
    {
        add = false;
    }
    // x = a + b or x = a - (-b)
    // Change it to x = a + b
    else if((basePositive && otherPositive && add) || (basePositive && !otherPositive && !add))
    {
        add = true;
    }

    // Manage addition
    if(add)
    {
        if(!testIfPossibleToAdd(baseValue, otherValue))
        {
            qWarning() << "The values are too bigs, can't add them: " << baseValue << " and "
                       << otherValue;
            return false;
        }

        result._value = baseValue + otherValue;
    }
    else if(baseValue >= otherValue)
    {
        result._value = baseValue - otherValue;
    }
    else
    {
        result._value = otherValue - baseValue;
        result._positive = false;
    }

    result._valid = true;
    result.trimNumber();
    return true;
}

bool Number::changeValuePrecision(const Number &number, quint8 numberAfterComma, quint64 &newValue)
{
    return changeValuePrecision(number._commaShift, number._value, numberAfterComma, newValue);
}

bool Number::changeValuePrecision(quint8 commaShift,
                                  quint64 value,
                                  quint8 numberAfterComma,
                                  quint64 &newValue)
{
    if(numberAfterComma < commaShift)
    {
        qWarning() << "If not possible to have a comma shift inferior to the current comma shift";
        return false;
    }

    quint8 commaShiftDiff = numberAfterComma - commaShift;

    return multiplyWithPowerOfTen(value, commaShiftDiff, newValue);
}

bool Number::trimNumber(quint64 value,
                        quint8 commaShift,
                        quint64 &valueTrimmed,
                        quint8 &commaShiftTrimmed)
{
    if(commaShift > MaxPowerOfTenSupported)
    {
        qWarning() << "Cannot trim the commaShift: " << commaShift << ", the limit max "
                   << "is: " << MaxPowerOfTenSupported;
        return false;
    }

    quint32 powerOfTen = 1;
    qint16 idx = commaShift;

    for(; idx > 0; idx--)
    {
        if((value % (powerOfTen * Number::DeciBase)) != 0)
        {
            break;
        }

        powerOfTen *= Number::DeciBase;
    }

    valueTrimmed = value / powerOfTen;
    commaShiftTrimmed = static_cast<quint8>(idx);
    return true;
}

QDebug operator<<(QDebug stream, const Number &number)
{
    QDebugStateSaver saver(stream);
    Q_UNUSED(saver)

    return stream.noquote() << number.toString();
}
