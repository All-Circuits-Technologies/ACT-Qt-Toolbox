// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include <QtTest>

#include "number.hpp"


class NumberTest : public QObject
{
    Q_OBJECT

    public:
        NumberTest();
        ~NumberTest();

    private slots:
        void test_basicnumberstates();
        void test_create_basicnumber_data();
        void test_create_basicnumber();
        void test_createobjectcompletevalue_data();
        void test_createobjectcompletevalue();
        void test_createobjectstring_data();
        void test_createobjectstring();
        void test_createobjectfromfloat_data();
        void test_createobjectfromfloat();
        void test_createobjectfromdouble_data();
        void test_createobjectfromdouble();
        void test_createobjectfromint8_data();
        void test_createobjectfromint8();
        void test_createobjectfromuint8_data();
        void test_createobjectfromuint8();
        void test_createobjectfromint16_data();
        void test_createobjectfromint16();
        void test_createobjectfromuint16_data();
        void test_createobjectfromuint16();
        void test_createobjectfromint32_data();
        void test_createobjectfromint32();
        void test_createobjectfromuint32_data();
        void test_createobjectfromuint32();
        void test_createobjectfromint64_data();
        void test_createobjectfromint64();
        void test_createobjectfromuint64_data();
        void test_createobjectfromuint64();
        void test_numbertoint8_data();
        void test_numbertoint8();
        void test_numbertouint8_data();
        void test_numbertouint8();
        void test_numbertoint16_data();
        void test_numbertoint16();
        void test_numbertouint16_data();
        void test_numbertouint16();
        void test_numbertoint32_data();
        void test_numbertoint32();
        void test_numbertouint32_data();
        void test_numbertouint32();
        void test_numbertoint64_data();
        void test_numbertoint64();
        void test_numbertouint64_data();
        void test_numbertouint64();
        void test_objectassignment();
        void test_inverseobject_data();
        void test_inverseobject();
        void test_objectaddition_data();
        void test_objectaddition();
        void test_objectsubstraction_data();
        void test_objectsubstraction();
        void test_objectdivide_data();
        void test_objectdivide();
        void test_objectmultiply_data();
        void test_objectmultiply();
        void test_objectmodulo_data();
        void test_objectmodulo();
        void test_objectpow_data();
        void test_objectpow();
        void test_objectpoweroften_data();
        void test_objectpoweroften();
        void test_equalityandnotequality_data();
        void test_equalityandnotequality();
        void test_strictsuperior_data();
        void test_strictsuperior();
        void test_strictsuperiorwithequality_data();
        void test_strictsuperiorwithequality();
        void test_strictinferior_data();
        void test_strictinferior();
        void test_strictinferiorwithequality_data();
        void test_strictinferiorwithequality();
        void test_round_data();
        void test_round();

    private:
        template<class T>
        void numberToIntegerData();

        template<class T>
        void numberToIntegerTest();

        template<class T>
        T numberToIntegerDataCall(const Number &number, quint8 commaShiftToRound, bool &ok);
};

template<class T>
void NumberTest::numberToIntegerData()
{
    static_assert(std::is_same<quint8, T>::value   || std::is_same<qint8, T>::value   ||
                  std::is_same<quint16, T>::value  || std::is_same<qint16, T>::value  ||
                  std::is_same<quint32, T>::value  || std::is_same<qint32, T>::value  ||
                  std::is_same<qint64, T>::value,
                  "T must be a signed or unsigned integer");

    bool signedInt = (std::is_same<qint8, T>::value ||
                      std::is_same<qint16, T>::value  ||
                      std::is_same<qint32, T>::value  ||
                      std::is_same<qint64, T>::value);

    QTest::addColumn<Number>("numberValue");
    QTest::addColumn<quint8>("commaShiftToRound");
    QTest::addColumn<bool>("expectedSuccess");
    QTest::addColumn<T>("expectedResult");

    QTest::newRow("Zero number")
            << Number(0) << static_cast<quint8>(0) << true << static_cast<T>(0);

    QTest::newRow("Invalid number")
            << Number() << static_cast<quint8>(0) << false << static_cast<T>(0);

    QTest::newRow("Wrong comma shift")
            << Number(56, 2) << static_cast<quint8>(7) << false << static_cast<T>(0);

    QTest::newRow("Positive number")
            << Number(56) << static_cast<quint8>(0) << true << static_cast<T>(56);

    QTest::newRow("Negative number")
            << Number(56, 0, false)
            << static_cast<quint8>(0)
            << signedInt
            << (signedInt ? static_cast<T>(-56) : static_cast<T>(0));

    QTest::newRow("Positive number to round")
            << Number(15, 1) << static_cast<quint8>(0) << true << static_cast<T>(2);

    QTest::newRow("Negative number to round")
            << Number(15, 1, false)
            << static_cast<quint8>(0)
            << signedInt
            << (signedInt ? static_cast<T>(-2) : static_cast<T>(0));

    QTest::newRow("Positive number to round with multiply")
            << Number(11, 1) << static_cast<quint8>(2) << true << static_cast<T>(110);

    QTest::newRow("Negative number to round with multiply")
            << Number(11, 1, false)
            << static_cast<quint8>(2)
            << signedInt
            << (signedInt ? static_cast<T>(-110) : static_cast<T>(0));

    QTest::newRow("Test max limit")
            << Number(std::numeric_limits<T>::max())
            << static_cast<quint8>(0)
            << true
            << static_cast<T>(std::numeric_limits<T>::max());

    QTest::newRow("Test overflow max limit")
            << Number(std::numeric_limits<T>::max() + 1ULL)
            << static_cast<quint8>(0)
            << false
            << static_cast<T>(0);

    if(signedInt)
    {
        QTest::newRow("Test min limit")
                << Number(std::numeric_limits<T>::max() + 1ULL, 0, false)
                << static_cast<quint8>(0)
                << true
                << static_cast<T>(std::numeric_limits<T>::min());

        QTest::newRow("Test overflow min limit")
                << Number(std::numeric_limits<T>::max() + 2ULL, 0, false)
                << static_cast<quint8>(0)
                << false
                << static_cast<T>(0);
    }
}

template<class T>
void NumberTest::numberToIntegerTest()
{
    static_assert(std::is_same<quint8, T>::value   || std::is_same<qint8, T>::value   ||
                  std::is_same<quint16, T>::value  || std::is_same<qint16, T>::value  ||
                  std::is_same<quint32, T>::value  || std::is_same<qint32, T>::value  ||
                  std::is_same<quint64, T>::value  || std::is_same<qint64, T>::value,
                  "T must be a signed or unsigned integer");

    QFETCH(Number, numberValue);
    QFETCH(quint8, commaShiftToRound);
    QFETCH(bool, expectedSuccess);
    QFETCH(T, expectedResult);

    bool ok = false;

    T result = numberToIntegerDataCall<T>(numberValue, commaShiftToRound, ok);

    QCOMPARE(ok, expectedSuccess);
    QCOMPARE(result, expectedResult);
}

template<class T>
T NumberTest::numberToIntegerDataCall(const Number &number, quint8 commaShiftToRound, bool &ok)
{
    static_assert(std::is_same<quint8, T>::value   || std::is_same<qint8, T>::value   ||
                  std::is_same<quint16, T>::value  || std::is_same<qint16, T>::value  ||
                  std::is_same<quint32, T>::value  || std::is_same<qint32, T>::value  ||
                  std::is_same<quint64, T>::value  || std::is_same<qint64, T>::value,
                  "T must be a signed or unsigned integer");

    Q_UNUSED(number)
    Q_UNUSED(commaShiftToRound)

    ok = false;

    QVERIFY(false);

    return 0;
}

template<>
inline qint8 NumberTest::numberToIntegerDataCall<qint8>(const Number &number,
                                                        quint8 commaShiftToRound,
                                                        bool &ok)
{
    return number.toInt8(&ok, commaShiftToRound);
}

template<>
inline quint8 NumberTest::numberToIntegerDataCall<quint8>(const Number &number,
                                                          quint8 commaShiftToRound,
                                                          bool &ok)
{
    return number.toUint8(&ok, commaShiftToRound);
}

template<>
inline qint16 NumberTest::numberToIntegerDataCall<qint16>(const Number &number,
                                                          quint8 commaShiftToRound,
                                                          bool &ok)
{
    return number.toInt16(&ok, commaShiftToRound);
}

template<>
inline quint16 NumberTest::numberToIntegerDataCall<quint16>(const Number &number,
                                                            quint8 commaShiftToRound,
                                                            bool &ok)
{
    return number.toUint16(&ok, commaShiftToRound);
}

template<>
inline qint32 NumberTest::numberToIntegerDataCall<qint32>(const Number &number,
                                                          quint8 commaShiftToRound,
                                                          bool &ok)
{
    return number.toInt32(&ok, commaShiftToRound);
}

template<>
inline quint32 NumberTest::numberToIntegerDataCall<quint32>(const Number &number,
                                                            quint8 commaShiftToRound,
                                                            bool &ok)
{
    return number.toUint32(&ok, commaShiftToRound);
}

template<>
inline qint64 NumberTest::numberToIntegerDataCall<qint64>(const Number &number,
                                                          quint8 commaShiftToRound,
                                                          bool &ok)
{
    return number.toInt64(&ok, commaShiftToRound);
}

template<>
inline quint64 NumberTest::numberToIntegerDataCall<quint64>(const Number &number,
                                                            quint8 commaShiftToRound,
                                                            bool &ok)
{
    return number.toUint64(&ok, commaShiftToRound);
}
