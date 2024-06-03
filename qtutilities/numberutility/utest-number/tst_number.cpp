// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tst_number.hpp"

#include <QCoreApplication>


NumberTest::NumberTest()
{
}

NumberTest::~NumberTest()
{
}

void NumberTest::test_basicnumberstates()
{
    QVERIFY(!Number().isValid());

    Number positiveInfinite = Number::PositiveInfinite;

    QVERIFY(positiveInfinite.isPositiveInfinite());
    QVERIFY(!positiveInfinite.isNegativeInfinite());
    QVERIFY(positiveInfinite.isInfinite());
    QVERIFY(positiveInfinite.isPositive());
    QVERIFY(!positiveInfinite.isNegative());
    QCOMPARE(positiveInfinite.getIntegerPart(), std::numeric_limits<quint64>::max());

    Number negativeInfinite = Number::NegativeInfinite;

    QVERIFY(!negativeInfinite.isPositiveInfinite());
    QVERIFY(negativeInfinite.isNegativeInfinite());
    QVERIFY(negativeInfinite.isInfinite());
    QVERIFY(!negativeInfinite.isPositive());
    QVERIFY(negativeInfinite.isNegative());
    QCOMPARE(negativeInfinite.getIntegerPart(), std::numeric_limits<quint64>::max());
}

void NumberTest::test_create_basicnumber_data()
{
    QTest::addColumn<Number>("basicValue");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Basic zero")
            << Number::zero()
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Max quint8")
            << Number::maxUint8()
            << static_cast<quint64>(std::numeric_limits<quint8>::max()) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;

    QTest::newRow("Min quint8")
            << Number::minUint8()
            << static_cast<quint64>(std::numeric_limits<quint8>::min()) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;

    QTest::newRow("Max qint8")
            << Number::maxInt8()
            << static_cast<quint64>(std::numeric_limits<qint8>::max()) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;

    QTest::newRow("Min qint8")
            << Number::minInt8()
            << static_cast<quint64>(- std::numeric_limits<qint8>::min()) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << false << true;

    QTest::newRow("Max quint16")
            << Number::maxUint16()
            << static_cast<quint64>(std::numeric_limits<quint16>::max()) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;

    QTest::newRow("Min quint16")
            << Number::minUint16()
            << static_cast<quint64>(std::numeric_limits<quint16>::min()) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;

    QTest::newRow("Max qint16")
            << Number::maxInt16()
            << static_cast<quint64>(std::numeric_limits<qint16>::max()) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;

    QTest::newRow("Min qint16")
            << Number::minInt16()
            << static_cast<quint64>(- std::numeric_limits<qint16>::min()) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << false << true;

    QTest::newRow("Max quint32")
            << Number::maxUint32()
            << static_cast<quint64>(std::numeric_limits<quint32>::max()) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;

    QTest::newRow("Min quint32")
            << Number::minUint32()
            << static_cast<quint64>(std::numeric_limits<quint32>::min()) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;

    QTest::newRow("Max qint32")
            << Number::maxInt32()
            << static_cast<quint64>(std::numeric_limits<qint32>::max()) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;

    QTest::newRow("Min qint32")
            << Number::minInt32()
            << static_cast<quint64>(std::numeric_limits<qint32>::max() + 1ULL) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << false << true;

    QTest::newRow("Max qint64")
            << Number::maxInt64()
            << static_cast<quint64>(std::numeric_limits<qint64>::max()) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;

    QTest::newRow("Min qint64")
            << Number::minInt64()
            << static_cast<quint64>(std::numeric_limits<qint64>::max() + 1ULL) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << false << true;
}

void NumberTest::test_create_basicnumber()
{
    QFETCH(Number, basicValue);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    QCOMPARE(basicValue.getIntegerPart(), expectedIntPart);
    QCOMPARE(basicValue.getDecimalPart(), expectedDecPart);
    QCOMPARE(basicValue.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(basicValue.isPositive(), expectedPositive);
    QCOMPARE(basicValue.isValid(), expectedValid);
}

void NumberTest::test_createobjectcompletevalue_data()
{
    QTest::addColumn<quint64>("value");
    QTest::addColumn<quint8>("commaShift");
    QTest::addColumn<bool>("positive");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << static_cast<quint64>(0) << static_cast<quint8>(0) << true
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Negative zero")
            << static_cast<quint64>(0) << static_cast<quint8>(0) << false
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Default int value: 25")
            << static_cast<quint64>(25) << static_cast<quint8>(0) << true
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Default int value: -25")
            << static_cast<quint64>(25) << static_cast<quint8>(0) << false
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Default decimal value: 100.065")
            << static_cast<quint64>(100065) << static_cast<quint8>(3) << true
            << static_cast<quint64>(100) << static_cast<quint64>(65) << static_cast<quint64>(1000)
                << true << true;
    QTest::newRow("Default decimal value: -134.69")
            << static_cast<quint64>(13469) << static_cast<quint8>(2) << false
            << static_cast<quint64>(134) << static_cast<quint64>(69) << static_cast<quint64>(100)
                << false << true;

    QTest::newRow("Test max limit (with positive numb)")
            << std::numeric_limits<quint64>::max() << static_cast<quint8>(6) << true
            << static_cast<quint64>(18446744073709) << static_cast<quint64>(551615)
                << static_cast<quint64>(1000000) << true << true;
    QTest::newRow("Test min limit (with negative numb)")
            << std::numeric_limits<quint64>::max() << static_cast<quint8>(6) << false
            << static_cast<quint64>(18446744073709) << static_cast<quint64>(551615)
                << static_cast<quint64>(1000000) << false << true;

    QTest::newRow("Overflow comma shift (with positive numb)")
            << static_cast<quint64>(100065) << static_cast<quint8>(7) << true
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
    QTest::newRow("Overflow comma shift (with negative numb)")
            << static_cast<quint64>(13469) << static_cast<quint8>(8) << false
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
}

void NumberTest::test_createobjectcompletevalue()
{
    QFETCH(quint64, value);
    QFETCH(quint8, commaShift);
    QFETCH(bool, positive);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number number(value, commaShift, positive);

    QCOMPARE(number.getIntegerPart(), expectedIntPart);
    QCOMPARE(number.getDecimalPart(), expectedDecPart);
    QCOMPARE(number.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(number.isPositive(), expectedPositive);
    QCOMPARE(number.isValid(), expectedValid);
}

void NumberTest::test_createobjectstring_data()
{
    QTest::addColumn<QString>("value");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");
    QTest::addColumn<QString>("expectedString");

    QTest::newRow("Positive zero")
            << "0"
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true << "0";
    QTest::newRow("Negative zero")
            << "-0"
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true << "-0";
    QTest::newRow("Zero with decimal")
            << "0.000000"
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true << "0";

    QTest::newRow("Default int value: 25")
            << "25"
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true << "25";
    QTest::newRow("Default int value: -25")
            << "-25"
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true << "-25";

    QTest::newRow("Default decimal value: 100.065")
            << "100.065"
            << static_cast<quint64>(100) << static_cast<quint64>(65) << static_cast<quint64>(1000)
                << true << true << "100.065";
    QTest::newRow("Default decimal value: -134.69")
            << "-134.69"
            << static_cast<quint64>(134) << static_cast<quint64>(69) << static_cast<quint64>(100)
                << false << true << "-134.69";
    QTest::newRow("Default decimal value: 8952,000065")
            << "8952,000065"
            << static_cast<quint64>(8952) << static_cast<quint64>(65)
                << static_cast<quint64>(1000000) << true << true  << "8952,000065";
    QTest::newRow("Default decimal value: -8974,63")
            << "-8974,63"
            << static_cast<quint64>(8974) << static_cast<quint64>(63) << static_cast<quint64>(100)
                << false << true << "-8974,63";

    QTest::newRow("Test max limit (with positive numb)")
            << "18446744073709.551615"
            << static_cast<quint64>(18446744073709) << static_cast<quint64>(551615)
                << static_cast<quint64>(1000000) << true << true << "18446744073709.551615";
    QTest::newRow("Test min limit (with negative numb)")
            << "-18446744073709.551615"
            << static_cast<quint64>(18446744073709) << static_cast<quint64>(551615)
                << static_cast<quint64>(1000000) << false << true << "-18446744073709.551615";

    QTest::newRow("Test positive infinite")
            << "+Inf"
            << std::numeric_limits<quint64>::max() << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true << "+Inf";
    QTest::newRow("Test positive infinite (with testing limit)")
            << "18446744073709551615"
            << std::numeric_limits<quint64>::max() << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true << "+Inf";
    QTest::newRow("Test negative infinite")
            << "-Inf"
            << std::numeric_limits<quint64>::max() << static_cast<quint64>(0)
                << static_cast<quint64>(1) << false << true << "-Inf";
    QTest::newRow("Test negative infinite (with testing limit)")
            << "-18446744073709551615"
            << std::numeric_limits<quint64>::max() << static_cast<quint64>(0)
                << static_cast<quint64>(1) << false << true << "-Inf";

    QTest::newRow("Test parsing hex number 0xAAAA")
            << "0xAAAA" << 0xAAAAULL << 0ULL << 1ULL << true << true << "43690";

    QTest::newRow("Test parsing hex number 0XCCCCC")
            << "0XCCCCC" << 0xCCCCCULL << 0ULL << 1ULL << true << true << "838860";

    QTest::newRow("Test parsing limit max hex number 0xFFFFFFFFFFFFFFFF")
            << "0xFFFFFFFFFFFFFFFF"
            << std::numeric_limits<quint64>::max() << 0ULL << 1ULL << true << true << "+Inf";

    QTest::newRow("Test overflow string parsing 0x1FFFFFFFFFFFFFFFF")
            << "0x1FFFFFFFFFFFFFFFF"
            << 0ULL << 0ULL << 1ULL << true << false << "";

    QTest::newRow("Failed test parsing negative hex number -0xFC")
            << "-0xFC"
            << 0ULL << 0ULL << 1ULL << true << false << "";

    QTest::newRow("Wrong hex number 0xFCV")
            << "0xFCV"
            << 0ULL << 0ULL << 1ULL << true << false << "";

    QTest::newRow("Wrong hex number xFC")
            << "xFC"
            << 0ULL << 0ULL << 1ULL << true << false << "";

    QTest::newRow("Overflow comma shift (with positive numb)")
            << "1.0000065"
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false << "";
    QTest::newRow("Overflow comma shift (with negative numb)")
            << "0.00013469"
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false << "";

    QTest::newRow("Overflow value (with positive numb) +1 dec")
            << "18446744073709.551616"
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false << "";
    QTest::newRow("Overflow value (with positive numb) +1 int")
            << "18446744073710.551615"
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false << "";

    QTest::newRow("Overflow value (with negative numb) +1 dec")
            << "18446744073709.551616"
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false << "";
    QTest::newRow("Overflow value (with negative numb) +1 int")
            << "18446744073710.551615"
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false << "";

    QTest::newRow("Test wrong string")
            << "2523l52"
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false << "";
}

void NumberTest::test_createobjectstring()
{
    QFETCH(QString, value);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);
    QFETCH(QString, expectedString);

    Number number(value);

    QCOMPARE(number.getIntegerPart(), expectedIntPart);
    QCOMPARE(number.getDecimalPart(), expectedDecPart);
    QCOMPARE(number.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(number.isPositive(), expectedPositive);
    QCOMPARE(number.isValid(), expectedValid);
    QCOMPARE(number.toString().replace('.', ','), expectedString.replace('.', ','));

    Number numberFromString = Number::fromString(value);

    QCOMPARE(numberFromString.getIntegerPart(), expectedIntPart);
    QCOMPARE(numberFromString.getDecimalPart(), expectedDecPart);
    QCOMPARE(numberFromString.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(numberFromString.isPositive(), expectedPositive);
    QCOMPARE(numberFromString.isValid(), expectedValid);
    QCOMPARE(numberFromString.toString().replace('.', ','), expectedString.replace('.', ','));
}

void NumberTest::test_createobjectfromfloat_data()
{
    QTest::addColumn<float>("value");
    QTest::addColumn<quint8>("commaShift");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << static_cast<float>(0) << static_cast<quint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Negative zero")
            << static_cast<float>(-0) << static_cast<quint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Default int value: 25")
            << static_cast<float>(25) << static_cast<quint8>(0)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Default int value: -25")
            << static_cast<float>(-25) << static_cast<quint8>(0)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Default decimal value: 100.065")
            << static_cast<float>(100.065) << static_cast<quint8>(3)
            << static_cast<quint64>(100) << static_cast<quint64>(65) << static_cast<quint64>(1000)
                << true << true;
    QTest::newRow("Default decimal value: -134.69")
            << static_cast<float>(-134.69) << static_cast<quint8>(2)
            << static_cast<quint64>(134) << static_cast<quint64>(69) << static_cast<quint64>(100)
                << false << true;

    QTest::newRow("Default decimal value with round: 100.0650558")
            << static_cast<float>(100.0650558) << static_cast<quint8>(6)
            << static_cast<quint64>(100) << static_cast<quint64>(65056)
                << static_cast<quint64>(1000000) << true << true;
    QTest::newRow("Default decimal value with round: 100.0650564")
            << static_cast<float>(100.0650564) << static_cast<quint8>(6)
            << static_cast<quint64>(100) << static_cast<quint64>(65056)
                << static_cast<quint64>(1000000) << true << true;
    QTest::newRow("Default decimal value with round: 100.9999")
            << static_cast<float>(100.9999) << static_cast<quint8>(3)
            << static_cast<quint64>(101) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Default decimal value with round: -134.695")
            << static_cast<float>(-134.695007) << static_cast<quint8>(2)
            << static_cast<quint64>(134) << static_cast<quint64>(7) << static_cast<quint64>(10)
                << false << true;
    QTest::newRow("Default decimal value with round: -134.694")
            << static_cast<float>(-134.694) << static_cast<quint8>(2)
            << static_cast<quint64>(134) << static_cast<quint64>(69) << static_cast<quint64>(100)
                << false << true;
    QTest::newRow("Default decimal value with round: -134.999")
            << static_cast<float>(-134.999) << static_cast<quint8>(2)
            << static_cast<quint64>(135) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;

    // The expected value is not identical to the value given cause of the float algorithm
    QTest::newRow("Test max limit (with positive numb)")
            << static_cast<float>(18446744073709.551615) << static_cast<quint8>(6)
            << static_cast<quint64>(18446744027136) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;
    QTest::newRow("Test min limit (with negative numb)")
            << static_cast<float>(-18446744073709.551615) << static_cast<quint8>(6)
            << static_cast<quint64>(18446744027136) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << false << true;

    QTest::newRow("Test NaN")
            << static_cast<float>(Q_SNAN) << static_cast<quint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
    QTest::newRow("Test Null")
            << static_cast<float>(0) << static_cast<quint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Test positive infinity")
            << static_cast<float>(+Q_INFINITY) << static_cast<quint8>(0)
            << std::numeric_limits<quint64>::max() << static_cast<quint64>(0)
            << static_cast<quint64>(1) << true << true;
    QTest::newRow("Test negative infinity")
            << static_cast<float>(-Q_INFINITY) << static_cast<quint8>(0)
            << std::numeric_limits<quint64>::max() << static_cast<quint64>(0)
            << static_cast<quint64>(1) << false << true;

    QTest::newRow("Overflow comma shift (with positive numb)")
            << static_cast<float>(1.0000065)  << static_cast<quint8>(7)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
    QTest::newRow("Overflow comma shift (with negative numb)")
            << static_cast<float>(0.00013469) << static_cast<quint8>(8)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;

    QTest::newRow("Overflow value (with positive numb)")
            << static_cast<float>(28446744073709551615.0) << static_cast<quint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;

    QTest::newRow("Overflow value (with negative numb)")
            << static_cast<float>(-28446744073709551615.0) << static_cast<quint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;

    QTest::newRow("Overflow value (with max positive float)")
            << std::numeric_limits<float>::max() << static_cast<quint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
    QTest::newRow("Overflow value (with max negative float)")
            << -std::numeric_limits<float>::max() << static_cast<quint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
}

void NumberTest::test_createobjectfromfloat()
{
    QFETCH(float, value);
    QFETCH(quint8, commaShift);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number numberFromFloat = Number::fromFloat(value, commaShift);

    QCOMPARE(numberFromFloat.getIntegerPart(), expectedIntPart);
    QCOMPARE(numberFromFloat.getDecimalPart(), expectedDecPart);
    QCOMPARE(numberFromFloat.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(numberFromFloat.isPositive(), expectedPositive);
    QCOMPARE(numberFromFloat.isValid(), expectedValid);
}

void NumberTest::test_createobjectfromdouble_data()
{
    QTest::addColumn<double>("value");
    QTest::addColumn<quint8>("commaShift");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << static_cast<double>(0) << static_cast<quint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Negative zero")
            << static_cast<double>(-0) << static_cast<quint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Default int value: 25")
            << static_cast<double>(25) << static_cast<quint8>(0)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Default int value: -25")
            << static_cast<double>(-25) << static_cast<quint8>(0)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Default decimal value: 100.065")
            << static_cast<double>(100.065) << static_cast<quint8>(3)
            << static_cast<quint64>(100) << static_cast<quint64>(65) << static_cast<quint64>(1000)
                << true << true;
    QTest::newRow("Default decimal value: -134.69")
            << static_cast<double>(-134.69) << static_cast<quint8>(2)
            << static_cast<quint64>(134) << static_cast<quint64>(69) << static_cast<quint64>(100)
                << false << true;

    QTest::newRow("Default decimal value with round: 100.0650558")
            << static_cast<double>(100.0650558) << static_cast<quint8>(6)
            << static_cast<quint64>(100) << static_cast<quint64>(65056)
                << static_cast<quint64>(1000000) << true << true;
    QTest::newRow("Default decimal value with round: 100.0650564")
            << static_cast<double>(100.0650564) << static_cast<quint8>(6)
            << static_cast<quint64>(100) << static_cast<quint64>(65056)
                << static_cast<quint64>(1000000) << true << true;
    QTest::newRow("Default decimal value with round: 100.9999")
            << static_cast<double>(100.9999) << static_cast<quint8>(3)
            << static_cast<quint64>(101) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Default decimal value with round: -134.695")
            << static_cast<double>(-134.695) << static_cast<quint8>(2)
            << static_cast<quint64>(134) << static_cast<quint64>(69) << static_cast<quint64>(100)
                << false << true;
    QTest::newRow("Default decimal value with round: -134.694")
            << static_cast<double>(-134.694) << static_cast<quint8>(2)
            << static_cast<quint64>(134) << static_cast<quint64>(69) << static_cast<quint64>(100)
                << false << true;
    QTest::newRow("Default decimal value with round: -134.999")
            << static_cast<double>(-134.999) << static_cast<quint8>(2)
            << static_cast<quint64>(135) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;

    // The expected value is not identical to the value given cause of the double algorithm
    QTest::newRow("Test max limit (with positive numb)")
            << static_cast<double>(18446744073709.551615) << static_cast<quint8>(6)
            << static_cast<quint64>(18446744073709) << static_cast<quint64>(550781)
                << static_cast<quint64>(1000000) << true << true;
    QTest::newRow("Test min limit (with negative numb)")
            << static_cast<double>(-18446744073709.551615) << static_cast<quint8>(6)
            << static_cast<quint64>(18446744073709) << static_cast<quint64>(550781)
                << static_cast<quint64>(1000000) << false << true;

    QTest::newRow("Test NaN")
            << static_cast<double>(Q_SNAN) << static_cast<quint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
    QTest::newRow("Test Null")
            << static_cast<double>(0) << static_cast<quint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Test positive infinity")
            << static_cast<double>(+Q_INFINITY) << static_cast<quint8>(0)
            << std::numeric_limits<quint64>::max() << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;
    QTest::newRow("Test negative infinity")
            << static_cast<double>(-Q_INFINITY) << static_cast<quint8>(0)
            << std::numeric_limits<quint64>::max() << static_cast<quint64>(0)
                << static_cast<quint64>(1) << false << true;

    QTest::newRow("Overflow comma shift (with positive numb)")
            << static_cast<double>(1.0000065)  << static_cast<quint8>(7)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
    QTest::newRow("Overflow comma shift (with negative numb)")
            << static_cast<double>(0.00013469) << static_cast<quint8>(8)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;

    QTest::newRow("Overflow value (with positive numb) +10000 dec")
            << static_cast<double>(18446744073709.561615) << static_cast<quint8>(6)
            << static_cast<quint64>(18446744073709) << static_cast<quint64>(5625)
            << static_cast<quint64>(10000) << true << true;
    QTest::newRow("Overflow value (with positive numb) +1 int")
            << static_cast<double>(18446744073710.551615) << static_cast<quint8>(6)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;

    QTest::newRow("Overflow value (with negative numb) +10000 dec")
            << static_cast<double>(-18446744073709.561615) << static_cast<quint8>(6)
            << static_cast<quint64>(18446744073709) << static_cast<quint64>(5625)
                << static_cast<quint64>(10000) << false << true;
    QTest::newRow("Overflow value (with negative numb) +1 int")
            << static_cast<double>(-18446744073710.551615) << static_cast<quint8>(6)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;

    QTest::newRow("Overflow value (with max positive float)")
            << std::numeric_limits<double>::max() << static_cast<quint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
    QTest::newRow("Overflow value (with max negative float)")
            << -std::numeric_limits<double>::max() << static_cast<quint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
}

void NumberTest::test_createobjectfromdouble()
{
    QFETCH(double, value);
    QFETCH(quint8, commaShift);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number numberFromDouble = Number::fromDouble(value, commaShift);

    QCOMPARE(numberFromDouble.getIntegerPart(), expectedIntPart);
    QCOMPARE(numberFromDouble.getDecimalPart(), expectedDecPart);
    QCOMPARE(numberFromDouble.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(numberFromDouble.isPositive(), expectedPositive);
    QCOMPARE(numberFromDouble.isValid(), expectedValid);
}

void NumberTest::test_createobjectfromint8_data()
{
    QTest::addColumn<qint8>("value");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << static_cast<qint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Negative zero")
            << static_cast<qint8>(-0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Default int value: 25")
            << static_cast<qint8>(25)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Default int value: -25")
            << static_cast<qint8>(-25)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Test max limit (with positive numb)")
            << std::numeric_limits<qint8>::max()
            << static_cast<quint64>(127) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Test min limit (with negative numb)")
            << std::numeric_limits<qint8>::min()
            << static_cast<quint64>(128) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;
}

void NumberTest::test_createobjectfromint8()
{
    QFETCH(qint8, value);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number numberFromInt = Number::fromInt8(value);

    QCOMPARE(numberFromInt.getIntegerPart(), expectedIntPart);
    QCOMPARE(numberFromInt.getDecimalPart(), expectedDecPart);
    QCOMPARE(numberFromInt.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(numberFromInt.isPositive(), expectedPositive);
    QCOMPARE(numberFromInt.isValid(), expectedValid);
}

void NumberTest::test_createobjectfromuint8_data()
{
    QTest::addColumn<quint8>("value");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << static_cast<quint8>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Default int value: 25")
            << static_cast<quint8>(25)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Test max limit (with positive numb)")
            << std::numeric_limits<quint8>::max()
            << static_cast<quint64>(255) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
}

void NumberTest::test_createobjectfromuint8()
{
    QFETCH(quint8, value);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number numberFromInt = Number::fromUInt8(value);

    QCOMPARE(numberFromInt.getIntegerPart(), expectedIntPart);
    QCOMPARE(numberFromInt.getDecimalPart(), expectedDecPart);
    QCOMPARE(numberFromInt.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(numberFromInt.isPositive(), expectedPositive);
    QCOMPARE(numberFromInt.isValid(), expectedValid);
}

void NumberTest::test_createobjectfromint16_data()
{
    QTest::addColumn<qint16>("value");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << static_cast<qint16>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Negative zero")
            << static_cast<qint16>(-0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Default int value: 25")
            << static_cast<qint16>(25)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Default int value: -25")
            << static_cast<qint16>(-25)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Test max limit (with positive numb)")
            << std::numeric_limits<qint16>::max()
            << static_cast<quint64>(32767) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Test min limit (with negative numb)")
            << std::numeric_limits<qint16>::min()
            << static_cast<quint64>(32768) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;
}

void NumberTest::test_createobjectfromint16()
{
    QFETCH(qint16, value);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number numberFromInt = Number::fromInt16(value);

    QCOMPARE(numberFromInt.getIntegerPart(), expectedIntPart);
    QCOMPARE(numberFromInt.getDecimalPart(), expectedDecPart);
    QCOMPARE(numberFromInt.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(numberFromInt.isPositive(), expectedPositive);
    QCOMPARE(numberFromInt.isValid(), expectedValid);
}

void NumberTest::test_createobjectfromuint16_data()
{
    QTest::addColumn<quint16>("value");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << static_cast<quint16>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Default int value: 25")
            << static_cast<quint16>(25)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Test max limit (with positive numb)")
            << std::numeric_limits<quint16>::max()
            << static_cast<quint64>(65535) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
}

void NumberTest::test_createobjectfromuint16()
{
    QFETCH(quint16, value);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number numberFromInt = Number::fromUInt16(value);

    QCOMPARE(numberFromInt.getIntegerPart(), expectedIntPart);
    QCOMPARE(numberFromInt.getDecimalPart(), expectedDecPart);
    QCOMPARE(numberFromInt.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(numberFromInt.isPositive(), expectedPositive);
    QCOMPARE(numberFromInt.isValid(), expectedValid);
}

void NumberTest::test_createobjectfromint32_data()
{
    QTest::addColumn<qint32>("value");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << static_cast<qint32>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Negative zero")
            << static_cast<qint32>(-0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Default int value: 25")
            << static_cast<qint32>(25)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Default int value: -25")
            << static_cast<qint32>(-25)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Test max limit (with positive numb)")
            << std::numeric_limits<qint32>::max()
            << static_cast<quint64>(2147483647)
            << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Test min limit (with negative numb)")
            << std::numeric_limits<qint32>::min()
            << static_cast<quint64>(2147483648) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << false << true;
}

void NumberTest::test_createobjectfromint32()
{
    QFETCH(qint32, value);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number numberFromInt = Number::fromInt32(value);

    QCOMPARE(numberFromInt.getIntegerPart(), expectedIntPart);
    QCOMPARE(numberFromInt.getDecimalPart(), expectedDecPart);
    QCOMPARE(numberFromInt.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(numberFromInt.isPositive(), expectedPositive);
    QCOMPARE(numberFromInt.isValid(), expectedValid);
}

void NumberTest::test_createobjectfromuint32_data()
{
    QTest::addColumn<quint32>("value");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << static_cast<quint32>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Default int value: 25")
            << static_cast<quint32>(25)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Test max limit (with positive numb)")
            << std::numeric_limits<quint32>::max()
            << static_cast<quint64>(4294967295) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;
}

void NumberTest::test_createobjectfromuint32()
{
    QFETCH(quint32, value);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number numberFromInt = Number::fromUInt32(value);

    QCOMPARE(numberFromInt.getIntegerPart(), expectedIntPart);
    QCOMPARE(numberFromInt.getDecimalPart(), expectedDecPart);
    QCOMPARE(numberFromInt.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(numberFromInt.isPositive(), expectedPositive);
    QCOMPARE(numberFromInt.isValid(), expectedValid);
}

void NumberTest::test_createobjectfromint64_data()
{
    QTest::addColumn<qint64>("value");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << static_cast<qint64>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Negative zero")
            << static_cast<qint64>(-0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Default int value: 25")
            << static_cast<qint64>(25)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Default int value: -25")
            << static_cast<qint64>(-25)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Test max limit (with positive numb)")
            << std::numeric_limits<qint64>::max()
            << static_cast<quint64>(9223372036854775807) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;
    QTest::newRow("Test min limit (with negative numb)")
            << std::numeric_limits<qint64>::min()
            << static_cast<quint64>(9223372036854775808U) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << false << true;
}

void NumberTest::test_createobjectfromint64()
{
    QFETCH(qint64, value);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number numberFromInt = Number::fromInt64(value);

    QCOMPARE(numberFromInt.getIntegerPart(), expectedIntPart);
    QCOMPARE(numberFromInt.getDecimalPart(), expectedDecPart);
    QCOMPARE(numberFromInt.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(numberFromInt.isPositive(), expectedPositive);
    QCOMPARE(numberFromInt.isValid(), expectedValid);
}

void NumberTest::test_createobjectfromuint64_data()
{
    QTest::addColumn<quint64>("value");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << static_cast<quint64>(0)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Default int value: 25")
            << static_cast<quint64>(25)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Test max limit (with positive numb)")
            << std::numeric_limits<quint64>::max()
            << static_cast<quint64>(18446744073709551615U) << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;
}

void NumberTest::test_createobjectfromuint64()
{
    QFETCH(quint64, value);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number numberFromInt = Number::fromUInt64(value);

    QCOMPARE(numberFromInt.getIntegerPart(), expectedIntPart);
    QCOMPARE(numberFromInt.getDecimalPart(), expectedDecPart);
    QCOMPARE(numberFromInt.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(numberFromInt.isPositive(), expectedPositive);
    QCOMPARE(numberFromInt.isValid(), expectedValid);
}

void NumberTest::test_numbertoint8_data()
{
    numberToIntegerData<qint8>();
}

void NumberTest::test_numbertoint8()
{
    numberToIntegerTest<qint8>();
}

void NumberTest::test_numbertouint8_data()
{
    numberToIntegerData<quint8>();
}

void NumberTest::test_numbertouint8()
{
    numberToIntegerTest<quint8>();
}

void NumberTest::test_numbertoint16_data()
{
    numberToIntegerData<qint16>();
}

void NumberTest::test_numbertoint16()
{
    numberToIntegerTest<qint16>();
}

void NumberTest::test_numbertouint16_data()
{
    numberToIntegerData<quint16>();
}

void NumberTest::test_numbertouint16()
{
    numberToIntegerTest<quint16>();
}

void NumberTest::test_numbertoint32_data()
{
    numberToIntegerData<qint32>();
}

void NumberTest::test_numbertoint32()
{
    numberToIntegerTest<qint32>();
}

void NumberTest::test_numbertouint32_data()
{
    numberToIntegerData<quint32>();
}

void NumberTest::test_numbertouint32()
{
    numberToIntegerTest<quint32>();
}

void NumberTest::test_numbertoint64_data()
{
    numberToIntegerData<qint64>();
}

void NumberTest::test_numbertoint64()
{
    numberToIntegerTest<qint64>();
}

void NumberTest::test_numbertouint64_data()
{

    QTest::addColumn<Number>("numberValue");
    QTest::addColumn<quint8>("commaShiftToRound");
    QTest::addColumn<bool>("expectedSuccess");
    QTest::addColumn<quint64>("expectedResult");

    QTest::newRow("Zero number")
            << Number(0) << static_cast<quint8>(0) << true << static_cast<quint64>(0);

    QTest::newRow("Invalid number")
            << Number() << static_cast<quint8>(0) << false << static_cast<quint64>(0);

    QTest::newRow("Wrong comma shift")
            << Number(56, 2) << static_cast<quint8>(7) << false << static_cast<quint64>(0);

    QTest::newRow("Positive number")
            << Number(56) << static_cast<quint8>(0) << true << static_cast<quint64>(56);

    QTest::newRow("Negative number")
            << Number(56, 0, false)
            << static_cast<quint8>(0)
            << false
            << static_cast<quint64>(0);

    QTest::newRow("Positive number to round")
            << Number(15, 1) << static_cast<quint8>(0) << true << static_cast<quint64>(2);

    QTest::newRow("Negative number to round")
            << Number(15, 1, false)
            << static_cast<quint8>(0)
            << false
            << static_cast<quint64>(0);

    QTest::newRow("Positive number to round with multiply")
            << Number(11, 1) << static_cast<quint8>(2) << true << static_cast<quint64>(110);

    QTest::newRow("Negative number to round with multiply")
            << Number(11, 1, false)
            << static_cast<quint8>(2)
            << false
            << static_cast<quint64>(0);

    QTest::newRow("Test max limit")
            << Number(std::numeric_limits<quint64>::max())
            << static_cast<quint8>(0)
            << true
            << static_cast<quint64>(std::numeric_limits<quint64>::max());
}

void NumberTest::test_numbertouint64()
{
    numberToIntegerTest<quint64>();
}

void NumberTest::test_objectassignment()
{
    Number number;

    Number numberToAssign(255, 2, false);

    number = numberToAssign;

    QCOMPARE(number.getIntegerPart(),    numberToAssign.getIntegerPart());
    QCOMPARE(number.getDecimalPart(),    numberToAssign.getDecimalPart());
    QCOMPARE(number.getDecimalDivisor(), numberToAssign.getDecimalDivisor());
    QCOMPARE(number.isPositive(),        numberToAssign.isPositive());
    QCOMPARE(number.isValid(),           numberToAssign.isValid());
}

void NumberTest::test_inverseobject_data()
{
    QTest::addColumn<quint64>("value");
    QTest::addColumn<quint8>("commaShift");
    QTest::addColumn<bool>("positive");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << static_cast<quint64>(0) << static_cast<quint8>(0) << true
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;
    QTest::newRow("Negative zero")
            << static_cast<quint64>(0) << static_cast<quint8>(0) << false
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Default int value: 25")
            << static_cast<quint64>(25) << static_cast<quint8>(0) << true
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;
    QTest::newRow("Default int value: -25")
            << static_cast<quint64>(25) << static_cast<quint8>(0) << false
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Test max limit (with positive numb)")
            << std::numeric_limits<quint64>::max() << static_cast<quint8>(6) << true
            << static_cast<quint64>(18446744073709) << static_cast<quint64>(551615)
                << static_cast<quint64>(1000000) << false << true;
    QTest::newRow("Test max limit (with negative numb)")
            << std::numeric_limits<quint64>::max() << static_cast<quint8>(6) << false
            << static_cast<quint64>(18446744073709) << static_cast<quint64>(551615)
                << static_cast<quint64>(1000000) << true << true;
}

void NumberTest::test_inverseobject()
{
    QFETCH(quint64, value);
    QFETCH(quint8, commaShift);
    QFETCH(bool, positive);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number numberToAssign(value, commaShift, positive);

    numberToAssign = - numberToAssign;

    QCOMPARE(numberToAssign.getIntegerPart(),    expectedIntPart);
    QCOMPARE(numberToAssign.getDecimalPart(),    expectedDecPart);
    QCOMPARE(numberToAssign.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(numberToAssign.isPositive(),        expectedPositive);
    QCOMPARE(numberToAssign.isValid(),           expectedValid);
}

void NumberTest::test_objectaddition_data()
{
    QTest::addColumn<Number>("firstElement");
    QTest::addColumn<Number>("secondElement");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << Number(static_cast<quint64>(0), static_cast<quint8>(0), true)
            << Number(static_cast<quint64>(0), static_cast<quint8>(0), true)
            << static_cast<quint64>(0) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Negative zero")
            << Number(static_cast<quint64>(0), static_cast<quint8>(0), false)
            << Number(static_cast<quint64>(0), static_cast<quint8>(0), false)
            << static_cast<quint64>(0) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Integers adding")
            << Number(static_cast<quint64>(326), static_cast<quint8>(0), true)
            << Number(static_cast<quint64>(63), static_cast<quint8>(0), true)
            << static_cast<quint64>(389) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Negative integers adding")
            << Number(static_cast<quint64>(326), static_cast<quint8>(0), false)
            << Number(static_cast<quint64>(63), static_cast<quint8>(0), false)
            << static_cast<quint64>(389) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Mixed positive and negative integers adding with positive result")
            << Number(static_cast<quint64>(326), static_cast<quint8>(0), true)
            << Number(static_cast<quint64>(63), static_cast<quint8>(0), false)
            << static_cast<quint64>(263) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Mixed positive and negative integers adding with negative result")
            << Number(static_cast<quint64>(450), static_cast<quint8>(0), false)
            << Number(static_cast<quint64>(18), static_cast<quint8>(0), true)
            << static_cast<quint64>(432) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Decimals adding")
            << Number(static_cast<quint64>(326), static_cast<quint8>(1), true)
            << Number(static_cast<quint64>(63), static_cast<quint8>(1), true)
            << static_cast<quint64>(38) << static_cast<quint64>(9) <<  static_cast<quint64>(10)
                << true << true;

    QTest::newRow("Negative decimals adding")
            << Number(785, 3, false)
            << Number(42, 2, false)
            << static_cast<quint64>(1) << static_cast<quint64>(205) <<  static_cast<quint64>(1000)
                << false << true;

    QTest::newRow("Mixed positive and negative decimal adding with positive result")
            << Number(569, 2, false)
            << Number(322, 1, true)
            << static_cast<quint64>(26) << static_cast<quint64>(51) << static_cast<quint64>(100)
                << true << true;

    QTest::newRow("Mixed positive and negative decimal adding with negative result")
            << Number(450, 2, false)
            << Number(18, 1, true)
            << static_cast<quint64>(2) << static_cast<quint64>(7) << static_cast<quint64>(10)
                << false << true;

    QTest::newRow("Operate on positive infinite adding")
            << Number::PositiveInfinite
            << Number(1, 6, true)
            << Number::PositiveInfinite.getIntegerPart() << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;

    QTest::newRow("Operate on negative infinite adding")
            << Number::NegativeInfinite
            << Number(1, 6, false)
            << Number::NegativeInfinite.getIntegerPart() << static_cast<quint64>(0)
                << static_cast<quint64>(1) << false << true;

    QTest::newRow("Test positive limit")
            << Number(std::numeric_limits<quint64>::max() - 1, 0, true)
            << Number(25, 0, true)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
    QTest::newRow("Test negative limit")
            << Number(std::numeric_limits<quint64>::max() - 1, 0, false)
            << Number(25, 0, false)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;

    QTest::newRow("Indeterminate infinite operation ")
            << Number::NegativeInfinite
            << Number::PositiveInfinite
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
    QTest::newRow("Indeterminate infinite operation (inverse)")
            << Number::PositiveInfinite
            << Number::NegativeInfinite
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;

    QTest::newRow("Add with invalid value")
            << Number()
            << Number(1, 6, false)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
}

void NumberTest::test_objectaddition()
{
    QFETCH(Number, firstElement);
    QFETCH(Number, secondElement);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number result = firstElement + secondElement;

    QCOMPARE(result.getIntegerPart(),    expectedIntPart);
    QCOMPARE(result.getDecimalPart(),    expectedDecPart);
    QCOMPARE(result.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(result.isPositive(),        expectedPositive);
    QCOMPARE(result.isValid(),           expectedValid);

    firstElement += secondElement;

    QCOMPARE(firstElement.getIntegerPart(),    expectedIntPart);
    QCOMPARE(firstElement.getDecimalPart(),    expectedDecPart);
    QCOMPARE(firstElement.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(firstElement.isPositive(),        expectedPositive);
    QCOMPARE(firstElement.isValid(),           expectedValid);
}

void NumberTest::test_objectsubstraction_data()
{
    QTest::addColumn<Number>("firstElement");
    QTest::addColumn<Number>("secondElement");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << Number(0, 0, true)
            << Number(0, 0, false)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Negative zero")
            << Number(0, 0, false)
            << Number(0, 0, true)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Integers sub")
            << Number(326, 0, true)
            << Number(63, 0, true)
            << static_cast<quint64>(263) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Negative integers sub")
            << Number(2365, 0, false)
            << Number(58, 0, false)
            << static_cast<quint64>(2307) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Mixed positive and negative integers sub with positive result")
            << Number(326, 0, true)
            << Number(63, 0, false)
            << static_cast<quint64>(389) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Mixed positive and negative integers sub with negative result")
            << Number(25, 0, false)
            << Number(263, 0, true)
            << static_cast<quint64>(288) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Decimals sub")
            << Number(326, 1, true)
            << Number(63, 1, true)
            << static_cast<quint64>(26) << static_cast<quint64>(3) << static_cast<quint64>(10)
                << true << true;

    QTest::newRow("Negative decimals sub")
            << Number(785, 3, false)
            << Number(42, 2, false)
            << static_cast<quint64>(0) << static_cast<quint64>(365) << static_cast<quint64>(1000)
                << false << true;

    QTest::newRow("Mixed positive and negative decimal sub with negative result")
            << Number(569, 2, false)
            << Number(322, 1, true)
            << static_cast<quint64>(37) << static_cast<quint64>(89) << static_cast<quint64>(100)
                << false << true;

    QTest::newRow("Mixed positive and negative decimal sub with positive result")
            << Number(450, 2, true)
            << Number(18, 1, false)
            << static_cast<quint64>(6) << static_cast<quint64>(3) << static_cast<quint64>(10)
                << true << true;

    QTest::newRow("Operate on negative infinite sub")
            << Number::NegativeInfinite
            << Number(1, 6, true)
            << Number::NegativeInfinite.getIntegerPart() << static_cast<quint64>(0)
                << static_cast<quint64>(1) << false << true;

    QTest::newRow("Operate on positive infinite adding")
            << Number::PositiveInfinite
            << Number(1, 6, false)
            << Number::PositiveInfinite.getIntegerPart() << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;

    QTest::newRow("Test positive limit")
            << Number(std::numeric_limits<quint64>::max() - 1, 0, true)
            << Number(25, 0, false)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
    QTest::newRow("Test negative limit")
            << Number(std::numeric_limits<quint64>::max() - 1, 0, false)
            << Number(25, 0, true)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;

    QTest::newRow("Indeterminate infinite operation ")
            << Number::NegativeInfinite
            << Number::NegativeInfinite
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
    QTest::newRow("Indeterminate infinite operation (inverse)")
            << Number::PositiveInfinite
            << Number::PositiveInfinite
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;

    QTest::newRow("Adding with invalid value")
            << Number()
            << Number(1, 6, false)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
}

void NumberTest::test_objectsubstraction()
{
    QFETCH(Number, firstElement);
    QFETCH(Number, secondElement);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number result = firstElement - secondElement;

    QCOMPARE(result.getIntegerPart(),    expectedIntPart);
    QCOMPARE(result.getDecimalPart(),    expectedDecPart);
    QCOMPARE(result.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(result.isPositive(),        expectedPositive);
    QCOMPARE(result.isValid(),           expectedValid);

    firstElement -= secondElement;

    QCOMPARE(firstElement.getIntegerPart(),    expectedIntPart);
    QCOMPARE(firstElement.getDecimalPart(),    expectedDecPart);
    QCOMPARE(firstElement.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(firstElement.isPositive(),        expectedPositive);
    QCOMPARE(firstElement.isValid(),           expectedValid);
}

void NumberTest::test_objectdivide_data()
{
    QTest::addColumn<Number>("firstElement");
    QTest::addColumn<Number>("secondElement");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << Number(0, 0, true)
            << Number(1, 0, true)
            <<  static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Negative zero")
            << Number(0, 0, false)
            << Number(1, 0, true)
            <<  static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;
    QTest::newRow("Divide the infinite")
            << Number::PositiveInfinite
            << Number(25, 0, true)
            << Number::PositiveInfinite.getIntegerPart() << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;

    QTest::newRow("Positive simple divide")
            << Number(25, 0, true)
            << Number(1, 0, true)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Negative simple divide")
            << Number(25, 0, false)
            << Number(1, 0, true)
            << static_cast<quint64>(25) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Positive divide with decimal")
            << Number(4, 0, true)
            << Number(25, 2, true)
            << static_cast<quint64>(16) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Negative divide with decimal")
            << Number(6, 0, false)
            << Number(15, 2, true)
            << static_cast<quint64>(40) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Limit divide with decimal")
            << Number(6, 1, false)
            << Number(15, 6, true)
            << static_cast<quint64>(40000) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << false << true;
    QTest::newRow("Round divide with decimal")
            << Number(82, 0, false)
            << Number(26, 0, true)
            << static_cast<quint64>(3) << static_cast<quint64>(153846)
                << static_cast<quint64>(1000000) << false << true;

    QTest::newRow("Divide by zero")
            << Number(6, 0, false)
            << Number(0, 0, true)
            << static_cast<quint64>(0) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << false;
    QTest::newRow("Divide by infinite")
            << Number(6, 0, false)
            << Number::PositiveInfinite
            << static_cast<quint64>(0) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << false;
    QTest::newRow("Divide by invalid")
            << Number(6, 0, false)
            << Number()
            << static_cast<quint64>(0) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << false;
}

void NumberTest::test_objectdivide()
{
    QFETCH(Number, firstElement);
    QFETCH(Number, secondElement);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number result = firstElement / secondElement;

    QCOMPARE(result.getIntegerPart(),    expectedIntPart);
    QCOMPARE(result.getDecimalPart(),    expectedDecPart);
    QCOMPARE(result.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(result.isPositive(),        expectedPositive);
    QCOMPARE(result.isValid(),           expectedValid);

    firstElement /= secondElement;

    QCOMPARE(firstElement.getIntegerPart(),    expectedIntPart);
    QCOMPARE(firstElement.getDecimalPart(),    expectedDecPart);
    QCOMPARE(firstElement.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(firstElement.isPositive(),        expectedPositive);
    QCOMPARE(firstElement.isValid(),           expectedValid);
}

void NumberTest::test_objectmultiply_data()
{
    QTest::addColumn<Number>("firstElement");
    QTest::addColumn<Number>("secondElement");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Positive zero")
            << Number(0, 0, true)
            << Number(1, 0, true)
            << static_cast<quint64>(0) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Negative zero")
            << Number(0, 0, false)
            << Number(1, 0, true)
            << static_cast<quint64>(0) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Multply the infinite")
            << Number::PositiveInfinite
            << Number(25, 0, true)
            << Number::PositiveInfinite.getIntegerPart() << static_cast<quint64>(0)
                << static_cast<quint64>(1) << true << true;
    QTest::newRow("Multply two infinites")
            << Number::PositiveInfinite
            << Number::NegativeInfinite
            << Number::NegativeInfinite.getIntegerPart() << static_cast<quint64>(0)
                << static_cast<quint64>(1) << false << true;

    QTest::newRow("Positive simple multiply")
            << Number(25, 0, true)
            << Number(1, 0, true)
            << static_cast<quint64>(25) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Negative simple multiply")
            << Number(25, 0, false)
            << Number(1, 0, true)
            << static_cast<quint64>(25) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << false << true;

    QTest::newRow("Positive multiply with decimal")
            << Number(4, 0, true)
            << Number(25, 2, true)
            << static_cast<quint64>(1) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << true;
    QTest::newRow("Negative multiply with decimal")
            << Number(6, 0, false)
            << Number(15, 2, true)
            << static_cast<quint64>(0) << static_cast<quint64>(9) << static_cast<quint64>(10)
                << false << true;

    QTest::newRow("Limit multiply with decimal")
            << Number(6, 1, false)
            << Number(15, 6, true)
            << static_cast<quint64>(0) << static_cast<quint64>(9) << static_cast<quint64>(1000000)
                << false << true;

    QTest::newRow("Multiply infinite with zero")
            << Number(0, 0, false)
            << Number::PositiveInfinite
            <<  static_cast<quint64>(0) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << false;
    QTest::newRow("Multiply by invalid")
            << Number(6, 0, false)
            << Number()
            << static_cast<quint64>(0) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << false;

    QTest::newRow("Test positive limit")
            << Number(std::numeric_limits<quint64>::max() - 1, 0, true)
            << Number(25, 0, true)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
    QTest::newRow("Test negative limit")
            << Number(std::numeric_limits<quint64>::max() - 1, 0, false)
            << Number(25, 0, false)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;
}

void NumberTest::test_objectmultiply()
{
    QFETCH(Number, firstElement);
    QFETCH(Number, secondElement);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number result = firstElement * secondElement;

    QCOMPARE(result.getIntegerPart(),    expectedIntPart);
    QCOMPARE(result.getDecimalPart(),    expectedDecPart);
    QCOMPARE(result.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(result.isPositive(),        expectedPositive);
    QCOMPARE(result.isValid(),           expectedValid);

    firstElement *= secondElement;

    QCOMPARE(firstElement.getIntegerPart(),    expectedIntPart);
    QCOMPARE(firstElement.getDecimalPart(),    expectedDecPart);
    QCOMPARE(firstElement.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(firstElement.isPositive(),        expectedPositive);
    QCOMPARE(firstElement.isValid(),           expectedValid);
}

void NumberTest::test_objectmodulo_data()
{
    QTest::addColumn<Number>("firstElement");
    QTest::addColumn<Number>("secondElement");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("0 mod 1 expects 0")
            << Number(0, 0, true)
            << Number(1, 0, true)
            << static_cast<quint64>(0) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << true;

    QTest::newRow("25 mod 1 expects 0")
            << Number(25, 0, true)
            << Number(1, 0, true)
            << static_cast<quint64>(0) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << true;

    QTest::newRow("25 mod 2 expects 1")
            << Number(25, 0, true)
            << Number(2, 0, true)
            << static_cast<quint64>(1) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << true;

    QTest::newRow("-25 mod 2 expects -1")
            << Number(25, 0, false)
            << Number(2, 0, true)
            << static_cast<quint64>(1) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << false << true;

    QTest::newRow("4 mod 0.3 expects 0.1")
            << Number(4, 0, true)
            << Number(3, 1, true)
            << static_cast<quint64>(0) << static_cast<quint64>(1) <<  static_cast<quint64>(10)
                << true << true;
    QTest::newRow("0.6 mod 14 expects 0.6")
            << Number(6, 1, false)
            << Number(14, 0, true)
            << static_cast<quint64>(0) << static_cast<quint64>(6) << static_cast<quint64>(10)
                << false << true;

    QTest::newRow("0.6 mod 0.000014 expects 2")
            << Number(6, 1, false)
            << Number(14, 6, true)
            << static_cast<quint64>(0) << static_cast<quint64>(2) << static_cast<quint64>(1000000)
                << false << true;

    QTest::newRow("18446744073709551613 mod 18446744073709551614 expects 18446744073709551613")
            << Number(std::numeric_limits<quint64>::max() - 2, 0, true)
            << Number(std::numeric_limits<quint64>::max() - 1, 0, true)
            << static_cast<quint64>(18446744073709551613ULL) << static_cast<quint64>(0)
            << static_cast<quint64>(1)
                << true << true;

    QTest::newRow("+Inf mod 25 expects invalid")
            << Number::PositiveInfinite
            << Number(25, 0, true)
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;

    QTest::newRow("+Inf mod -Inf expects invalid")
            << Number::PositiveInfinite
            << Number::NegativeInfinite
            << static_cast<quint64>(0) << static_cast<quint64>(0) << static_cast<quint64>(1)
                << true << false;

    QTest::newRow("25 mod 0 expects invalid")
            << Number(25, 0, true)
            << Number(0, 0, true)
            <<  static_cast<quint64>(0) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << false;
    QTest::newRow("-6 mod invalid expects invalid")
            << Number(6, 0, false)
            << Number()
            << static_cast<quint64>(0) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << false;
    QTest::newRow("invalid mod -6 expects invalid")
            << Number()
            << Number(6, 0, false)
            << static_cast<quint64>(0) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << false;
}

void NumberTest::test_objectmodulo()
{
    QFETCH(Number, firstElement);
    QFETCH(Number, secondElement);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    Number result = firstElement % secondElement;

    QCOMPARE(result.getIntegerPart(),    expectedIntPart);
    QCOMPARE(result.getDecimalPart(),    expectedDecPart);
    QCOMPARE(result.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(result.isPositive(),        expectedPositive);
    QCOMPARE(result.isValid(),           expectedValid);

    firstElement %= secondElement;

    QCOMPARE(firstElement.getIntegerPart(),    expectedIntPart);
    QCOMPARE(firstElement.getDecimalPart(),    expectedDecPart);
    QCOMPARE(firstElement.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(firstElement.isPositive(),        expectedPositive);
    QCOMPARE(firstElement.isValid(),           expectedValid);
}

void NumberTest::test_objectpow_data()
{
    QTest::addColumn<Number>("number");
    QTest::addColumn<qint8>("power");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Simple positive pow: 2^2 = 4")
            << Number(2)
            << static_cast<qint8>(2)
            << static_cast<quint64>(4) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Simple negative pow: 8^-2 = 0.015625")
            << Number(8)
            << static_cast<qint8>(-2)
            << static_cast<quint64>(0) << static_cast<quint64>(15625)
                <<  static_cast<quint64>(1000000) << true << true;

    QTest::newRow("Zero pow: 53^0 = 1")
            << Number(53)
            << static_cast<qint8>(0)
            << static_cast<quint64>(1) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << true;

    QTest::newRow("One pow: -64^1 = -64")
            << Number(64, 0, false)
            << static_cast<qint8>(1)
            << static_cast<quint64>(64) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << false << true;

    QTest::newRow("One negative pow: 8^-1 = 0.125")
            << Number(8)
            << static_cast<qint8>(-1)
            << static_cast<quint64>(0) << static_cast<quint64>(125) <<  static_cast<quint64>(1000)
                << true << true;

    QTest::newRow("Long positive pow: 2^63 = 9 223 372 036 854 775 808")
            << Number(2)
            << static_cast<qint8>(63)
            << static_cast<quint64>(static_cast<quint64>(std::numeric_limits<qint64>::max()) + 1)
            << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Positive decimal pow: 2.5^2 = 6.25")
            << Number(25, 1)
            << static_cast<qint8>(2)
            << static_cast<quint64>(6)
            << static_cast<quint64>(25) <<  static_cast<quint64>(100)
                << true << true;

    QTest::newRow("Long negative pow: 2^-10 = 0.0009765625")
            << Number(2)
            << static_cast<qint8>(-10)
            << static_cast<quint64>(0)
            << static_cast<quint64>(977) <<  static_cast<quint64>(1000000)
                << true << true;

    QTest::newRow("Invalid number: 2^64 = 18 446 744 073 709 551 616 (overflow)")
            << Number(2)
            << static_cast<qint8>(64)
            << static_cast<quint64>(0)
            << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << false;
}

void NumberTest::test_objectpow()
{
    QFETCH(Number, number);
    QFETCH(qint8, power);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    number.pow(power);

    QCOMPARE(number.getIntegerPart(),    expectedIntPart);
    QCOMPARE(number.getDecimalPart(),    expectedDecPart);
    QCOMPARE(number.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(number.isPositive(),        expectedPositive);
    QCOMPARE(number.isValid(),           expectedValid);
}

void NumberTest::test_objectpoweroften_data()
{
    QTest::addColumn<Number>("number");
    QTest::addColumn<qint8>("powerOfTen");
    QTest::addColumn<quint64>("expectedIntPart");
    QTest::addColumn<quint64>("expectedDecPart");
    QTest::addColumn<quint64>("expectedDecDivisor");
    QTest::addColumn<bool>("expectedPositive");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("Simple positive pow: 2 * 10^2 = 200")
            << Number(2)
            << static_cast<qint8>(2)
            << static_cast<quint64>(200) << static_cast<quint64>(0) <<  static_cast<quint64>(1)
                << true << true;

    QTest::newRow("Simple negative pow: 8 * 10^-2 = 0.08")
            << Number(8)
            << static_cast<qint8>(-2)
            << static_cast<quint64>(0) << static_cast<quint64>(8)
                <<  static_cast<quint64>(100) << true << true;

    QTest::newRow("Zero pow: 16 * 10^0 = 16")
            << Number(16)
            << static_cast<qint8>(0)
            << static_cast<quint64>(16) << static_cast<quint64>(0)
                <<  static_cast<quint64>(1) << true << true;

    QTest::newRow("One positive pow: 9 * 10^1 = 90")
            << Number(9)
            << static_cast<qint8>(1)
            << static_cast<quint64>(90) << static_cast<quint64>(0)
                <<  static_cast<quint64>(1) << true << true;

    QTest::newRow("One negative pow: 9 * 10^-1 = 0.9")
            << Number(9)
            << static_cast<qint8>(-1)
            << static_cast<quint64>(0) << static_cast<quint64>(9)
                <<  static_cast<quint64>(10) << true << true;

    QTest::newRow("Long positive pow: 18 * 10^-1 = 0.9")
            << Number(9)
            << static_cast<qint8>(-1)
            << static_cast<quint64>(0) << static_cast<quint64>(9)
                <<  static_cast<quint64>(10) << true << true;

    QTest::newRow("Long positive pow: 18 * 10^18 = 18 000 000 000 000 000 000")
            << Number(18)
            << static_cast<qint8>(18)
            << static_cast<quint64>(18000000000000000000ULL) << static_cast<quint64>(0)
                <<  static_cast<quint64>(1) << true << true;

    QTest::newRow("Too long positive pow: 1 * 10^20 = 100 000 000 000 000 000 000 (invalid)")
            << Number(1)
            << static_cast<qint8>(20)
            << static_cast<quint64>(0) << static_cast<quint64>(0)
                <<  static_cast<quint64>(1) << true << false;


    QTest::newRow("Too long negative pow: 1 * 10^-8 = 0.00000001 (rounded to 0)")
            << Number(1)
            << static_cast<qint8>(-8)
            << static_cast<quint64>(0) << static_cast<quint64>(0)
                <<  static_cast<quint64>(1) << true << true;
}

void NumberTest::test_objectpoweroften()
{
    QFETCH(Number, number);
    QFETCH(qint8, powerOfTen);
    QFETCH(quint64, expectedIntPart);
    QFETCH(quint64, expectedDecPart);
    QFETCH(quint64, expectedDecDivisor);
    QFETCH(bool, expectedPositive);
    QFETCH(bool, expectedValid);

    number.multiplyWithPowerOfTen(powerOfTen);

    QCOMPARE(number.getIntegerPart(),    expectedIntPart);
    QCOMPARE(number.getDecimalPart(),    expectedDecPart);
    QCOMPARE(number.getDecimalDivisor(), expectedDecDivisor);
    QCOMPARE(number.isPositive(),        expectedPositive);
    QCOMPARE(number.isValid(),           expectedValid);
}

void NumberTest::test_equalityandnotequality_data()
{
    QTest::addColumn<Number>("firstElement");
    QTest::addColumn<Number>("secondElement");
    QTest::addColumn<bool>("expectedEqual");

    QTest::newRow("Test simple equality")
            << Number(2, 0, true)
            << Number::fromInt8(2)
            << true;

    QTest::newRow("Test simple negative equality")
            << Number(2, 0, false)
            << Number::fromInt8(-2)
            << true;

    QTest::newRow("Test decimal equality")
            << Number(1568, 2, true)
            << Number::fromString("15.68")
            << true;

    QTest::newRow("Test simple non equality")
            << Number(0)
            << Number()
            << false;

    QTest::newRow("Test inverse non equality")
            << Number(52, 0, true)
            << Number(52, 0, false)
            << false;

    QTest::newRow("Limit test with decimal")
            << Number(5200, 2, true)
            << Number(52, 0, true)
            << true;
}

void NumberTest::test_equalityandnotequality()
{
    QFETCH(Number, firstElement);
    QFETCH(Number, secondElement);
    QFETCH(bool, expectedEqual);

    QCOMPARE(firstElement == secondElement, expectedEqual);
    QCOMPARE(firstElement != secondElement, !expectedEqual);
}

void NumberTest::test_strictsuperior_data()
{
    QTest::addColumn<Number>("firstElement");
    QTest::addColumn<Number>("secondElement");
    QTest::addColumn<bool>("expectedSuccess");

    QTest::newRow("Test simple superiority")
            << Number(3, 0, true)
            << Number::fromInt8(2)
            << true;

    QTest::newRow("Test simple negative superiority")
            << Number(2, 0, true)
            << Number::fromInt8(-3)
            << true;

    QTest::newRow("Test decimal superiority")
            << Number::fromUInt8(1)
            << Number(25, 6, true)
            << true;

    QTest::newRow("Test equality superiority")
            << Number::fromUInt8(68)
            << Number(68, 0, true)
            << false;

    QTest::newRow("Test simple non superiority")
            << Number(0)
            << Number()
            << false;

    QTest::newRow("Limit same infinity")
            << Number::PositiveInfinite
            << Number::PositiveInfinite
            << false;

    QTest::newRow("Limit infinity")
            << Number::PositiveInfinite
            << Number::NegativeInfinite
            << true;
}

void NumberTest::test_strictsuperior()
{
    QFETCH(Number, firstElement);
    QFETCH(Number, secondElement);
    QFETCH(bool, expectedSuccess);

    QCOMPARE(firstElement > secondElement, expectedSuccess);
}

void NumberTest::test_strictsuperiorwithequality_data()
{
    QTest::addColumn<Number>("firstElement");
    QTest::addColumn<Number>("secondElement");
    QTest::addColumn<bool>("expectedSuccess");

    QTest::newRow("Test simple superiority")
            << Number(3, 0, true)
            << Number::fromInt8(2)
            << true;

    QTest::newRow("Test simple negative superiority")
            << Number(2, 0, true)
            << Number::fromInt8(-3)
            << true;

    QTest::newRow("Test decimal superiority")
            << Number::fromUInt8(1)
            << Number(25, 6, true)
            << true;

    QTest::newRow("Test equality superiority")
            << Number::fromUInt8(68)
            << Number(68, 0, true)
            << true;
    QTest::newRow("Test limitequality superiority")
            << Number::fromUInt8(68)
            << Number(68000, 3, true)
            << true;

    QTest::newRow("Test simple non superiority")
            << Number(0)
            << Number()
            << false;

    QTest::newRow("Limit same infinity")
            << Number::PositiveInfinite
            << Number::PositiveInfinite
            << true;

    QTest::newRow("Limit infinity")
            << Number::PositiveInfinite
            << Number::NegativeInfinite
            << true;

    QTest::newRow("+Inf >= 3, expect success: true")
            << Number::PositiveInfinite
            << Number(3)
            << true;

    QTest::newRow("-Inf >= 3, expect success: false")
            << Number::NegativeInfinite
            << Number(3)
            << false;

    QTest::newRow("-Inf >= -5, expect success: false")
            << Number::NegativeInfinite
            << Number(5, 0, false)
            << false;
}

void NumberTest::test_strictsuperiorwithequality()
{
    QFETCH(Number, firstElement);
    QFETCH(Number, secondElement);
    QFETCH(bool, expectedSuccess);

    QCOMPARE(firstElement >= secondElement, expectedSuccess);
}

void NumberTest::test_strictinferior_data()
{
    QTest::addColumn<Number>("firstElement");
    QTest::addColumn<Number>("secondElement");
    QTest::addColumn<bool>("expectedSuccess");

    QTest::newRow("Test simple superiority")
            << Number::fromInt8(2)
            << Number(3, 0, true)
            << true;

    QTest::newRow("Test simple negative superiority")
            << Number::fromInt8(-3)
            << Number(2, 0, true)
            << true;

    QTest::newRow("Test decimal superiority")
            << Number(25, 6, true)
            << Number::fromUInt8(1)
            << true;

    QTest::newRow("Test equality superiority")
            << Number(68, 0, true)
            << Number::fromUInt8(68)
            << false;

    QTest::newRow("Test simple non superiority")
            << Number(0)
            << Number()
            << false;

    QTest::newRow("Limit same infinity")
            << Number::PositiveInfinite
            << Number::PositiveInfinite
            << false;

    QTest::newRow("Limit infinity")
            << Number::NegativeInfinite
            << Number::PositiveInfinite
            << true;

    QTest::newRow("+Inf < 3, expect success: false")
            << Number::PositiveInfinite
            << Number(3)
            << false;

    QTest::newRow("-Inf < 3, expect success: true")
            << Number::NegativeInfinite
            << Number(3)
            << true;

    QTest::newRow("-Inf < -5, expect success: true")
            << Number::NegativeInfinite
            << Number(5, 0, false)
            << true;
}

void NumberTest::test_strictinferior()
{
    QFETCH(Number, firstElement);
    QFETCH(Number, secondElement);
    QFETCH(bool, expectedSuccess);

    qInfo() << "First elem, int part: " << firstElement.getIntegerPart()
            << ", dec part: " << firstElement.getDecimalPart()
            << ", divisor: " << firstElement.getDecimalDivisor()
            << ", positivity: " << firstElement.isPositive()
            << "; second elem, int part: " << secondElement.getIntegerPart()
            << ", dec part: " << secondElement.getDecimalPart()
            << ", divisor: " << secondElement.getDecimalDivisor()
            << ", positivity: " << secondElement.isPositive();

    QCOMPARE(firstElement < secondElement, expectedSuccess);
}

void NumberTest::test_strictinferiorwithequality_data()
{
    QTest::addColumn<Number>("firstElement");
    QTest::addColumn<Number>("secondElement");
    QTest::addColumn<bool>("expectedSuccess");

    QTest::newRow("Test simple superiority")
            << Number::fromInt8(2)
            << Number(3, 0, true)
            << true;

    QTest::newRow("Test simple negative superiority")
            << Number::fromInt8(-3)
            << Number(2, 0, true)
            << true;

    QTest::newRow("Test decimal superiority")
            << Number(25, 6, true)
            << Number::fromUInt8(1)
            << true;

    QTest::newRow("Test equality superiority")
            << Number(68, 0, true)
            << Number::fromUInt8(68)
            << true;
    QTest::newRow("Test limitequality superiority")
            << Number(68000, 3, true)
            << Number::fromUInt8(68)
            << true;

    QTest::newRow("Test simple non superiority")
            << Number(0)
            << Number()
            << false;

    QTest::newRow("Limit same infinity")
            << Number::PositiveInfinite
            << Number::PositiveInfinite
            << true;

    QTest::newRow("Limit infinity")
            << Number::NegativeInfinite
            << Number::PositiveInfinite
            << true;

    QTest::newRow("+Inf <= 3, expect success: false")
            << Number::PositiveInfinite
            << Number(3)
            << false;

    QTest::newRow("-Inf <= 3, expect success: true")
            << Number::NegativeInfinite
            << Number(3)
            << true;

    QTest::newRow("-Inf <= -5, expect success: true")
            << Number::NegativeInfinite
            << Number(5, 0, false)
            << true;
}

void NumberTest::test_strictinferiorwithequality()
{
    QFETCH(Number, firstElement);
    QFETCH(Number, secondElement);
    QFETCH(bool, expectedSuccess);

    QCOMPARE(firstElement <= secondElement, expectedSuccess);
}

void NumberTest::test_round_data()
{
    QTest::addColumn<Number>("numberToRound");
    QTest::addColumn<quint8>("commaShiftToRound");
    QTest::addColumn<bool>("expectedValid");
    QTest::addColumn<Number>("expectedNumber");

    QTest::newRow("Round: 0, with commaShift: 0, expect: 0")
            << Number(0) << static_cast<quint8>(0) << true << Number(0);

    QTest::newRow("Round: 0.001, with commaShift: 2, expect: 0")
            << Number(1, 3) << static_cast<quint8>(2) << true << Number(0);

    QTest::newRow("Round: 0.005, with commaShift: 2, expect: 0.01")
            << Number(5, 3) << static_cast<quint8>(2) << true << Number(1, 2);

    QTest::newRow("Round: -0.005, with commaShift: 2, expect: -0.01")
            << Number(5, 3, false) << static_cast<quint8>(2) << true << Number(1, 2, false);

    QTest::newRow("Round: -0.005, with commaShift: 2, expect: -0.01")
            << Number(5, 3, false) << static_cast<quint8>(2) << true << Number(1, 2, false);

    QTest::newRow("Round: 1234.56789, with commaShift: 0, expect: 1235")
            << Number(123456789, 5) << static_cast<quint8>(0) << true << Number(1235);

    QTest::newRow("Round: 1234.56789, with commaShift: 1, expect: 1234.6")
            << Number(123456789, 5) << static_cast<quint8>(1) << true << Number(12346, 1);

    QTest::newRow("Round: -1234.56789, with commaShift: 0, expect: -1235")
            << Number(123456789, 5, false) << static_cast<quint8>(0) << true
            << Number(1235, 0, false);

    QTest::newRow("Round: -1234.56789, with commaShift: 1, expect: -1234.6")
            << Number(123456789, 5, false) << static_cast<quint8>(1) << true
            << Number(12346, 1, false);

    QTest::newRow("Round: 1234, with commaShift: 2, expect: 1234")
            << Number(1234) << static_cast<quint8>(0) << true << Number(1234);

    QTest::newRow("Round: -1234, with commaShift: 2, expect: -1234")
            << Number(1234, 0, false) << static_cast<quint8>(0) << true << Number(1234, 0, false);

    QTest::newRow("Try to round invalid number")
            << Number() << static_cast<quint8>(2) << false << Number();

    QTest::newRow("Try to round positive infinite")
            << Number::PositiveInfinite << static_cast<quint8>(2) << true
            << Number::PositiveInfinite;

    QTest::newRow("Try to round negative infinite")
            << Number::NegativeInfinite << static_cast<quint8>(2) << true
            << Number::NegativeInfinite;

    QTest::newRow("Try to round with too big comma shift")
            << Number(123456789, 5, false) << static_cast<quint8>(7) << false << Number();
}

void NumberTest::test_round()
{
    QFETCH(Number, numberToRound);
    QFETCH(quint8, commaShiftToRound);
    QFETCH(bool, expectedValid);
    QFETCH(Number, expectedNumber);

    numberToRound.round(commaShiftToRound);

    QCOMPARE(numberToRound.isValid(), expectedValid);

    if(expectedValid)
    {
        QVERIFY(numberToRound == expectedNumber);
    }
}

QTEST_MAIN(NumberTest)
