// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tst_byte.hpp"

#include <QtTest>

#include "byteutility/bytearrayhelper.hpp"
#include "byteutility/endianesshelper.hpp"


ByteTest::ByteTest()
{
}

ByteTest::~ByteTest()
{
}

void ByteTest::test_quint16endianess_data()
{
    QTest::addColumn<quint16>("value");
    QTest::addColumn<quint8>("byteIndex");
    QTest::addColumn<quint8>("expectedResult");

    QTest::newRow("Byte index equals to 0, value equals to 0x0000, expect 0x00")
            << static_cast<quint16>(0x0000)
            << static_cast<quint8>(0x00)
            << static_cast<quint8>(0x00);
    QTest::newRow("Byte index equals to 1, value equals to 0x0000, expect 0x00")
            << static_cast<quint16>(0x0000)
            << static_cast<quint8>(0x01)
            << static_cast<quint8>(0x00);
    QTest::newRow("Byte index equals to 0, value equals to 0xFFFF, expect 0xFF")
            << static_cast<quint16>(0xFFFF)
            << static_cast<quint8>(0x00)
            << static_cast<quint8>(0xFF);
    QTest::newRow("Byte index equals to 1, value equals to 0xFFFF, expect 0xFF")
            << static_cast<quint16>(0xFFFF)
            << static_cast<quint8>(0x01)
            << static_cast<quint8>(0xFF);
    QTest::newRow("Byte index equals to 0, value equals to 0x00AA, expect 0xAA")
            << static_cast<quint16>(0x00AA)
            << static_cast<quint8>(0x00)
            << static_cast<quint8>(0xAA);
    QTest::newRow("Byte index equals to 1, value equals to 0x00AA, expect 0x00")
            << static_cast<quint16>(0x00AA)
            << static_cast<quint8>(0x01)
            << static_cast<quint8>(0x00);
    QTest::newRow("Byte index equals to 0, value equals to 0xAA00, expect 0x00")
            << static_cast<quint16>(0xAA00)
            << static_cast<quint8>(0x00)
            << static_cast<quint8>(0x00);
    QTest::newRow("Byte index equals to 1, value equals to 0xAA00, expect 0xAA")
            << static_cast<quint16>(0xAA00)
            << static_cast<quint8>(0x01)
            << static_cast<quint8>(0xAA);
    QTest::newRow("Byte index equals to 2, value equals to 0xAAAA, expect 0x00")
            << static_cast<quint16>(0xAAAA)
            << static_cast<quint8>(0x02)
            << static_cast<quint8>(0x00);
}

void ByteTest::test_quint16endianess()
{
    QFETCH(quint16, value);
    QFETCH(quint8, byteIndex);
    QFETCH(quint8, expectedResult);

    quint8 result = EndianessHelper::getByteFromLsbToMsb(value, byteIndex);

    QCOMPARE(expectedResult, result);
}

void ByteTest::test_quint32endianess_data()
{
    QTest::addColumn<quint32>("value");
    QTest::addColumn<quint8>("byteIndex");
    QTest::addColumn<quint8>("expectedResult");

    QTest::newRow("Byte index equals to 0, value equals to 0x00000000, expect 0x00")
            << static_cast<quint32>(0x00000000)
            << static_cast<quint8>(0x00)
            << static_cast<quint8>(0x00);
    QTest::newRow("Byte index equals to 1, value equals to 0x00000000, expect 0x00")
            << static_cast<quint32>(0x00000000)
            << static_cast<quint8>(0x01)
            << static_cast<quint8>(0x00);
    QTest::newRow("Byte index equals to 2, value equals to 0x00000000, expect 0x00")
            << static_cast<quint32>(0x00000000)
            << static_cast<quint8>(0x02)
            << static_cast<quint8>(0x00);
    QTest::newRow("Byte index equals to 3, value equals to 0x00000000, expect 0x00")
            << static_cast<quint32>(0x00000000)
            << static_cast<quint8>(0x03)
            << static_cast<quint8>(0x00);

    QTest::newRow("Byte index equals to 0, value equals to 0xFFFFFFFF, expect 0xFF")
            << static_cast<quint32>(0xFFFFFFFF)
            << static_cast<quint8>(0x00)
            << static_cast<quint8>(0xFF);
    QTest::newRow("Byte index equals to 1, value equals to 0xFFFFFFFF, expect 0xFF")
            << static_cast<quint32>(0xFFFFFFFF)
            << static_cast<quint8>(0x01)
            << static_cast<quint8>(0xFF);
    QTest::newRow("Byte index equals to 2, value equals to 0xFFFFFFFF, expect 0xFF")
            << static_cast<quint32>(0xFFFFFFFF)
            << static_cast<quint8>(0x02)
            << static_cast<quint8>(0xFF);
    QTest::newRow("Byte index equals to 3, value equals to 0xFFFFFFFF, expect 0xFF")
            << static_cast<quint32>(0xFFFFFFFF)
            << static_cast<quint8>(0x03)
            << static_cast<quint8>(0xFF);

    QTest::newRow("Byte index equals to 0, value equals to 0x01234567, expect 0x67")
            << static_cast<quint32>(0x01234567)
            << static_cast<quint8>(0x00)
            << static_cast<quint8>(0x67);
    QTest::newRow("Byte index equals to 1, value equals to 0x01234567, expect 0x45")
            << static_cast<quint32>(0x01234567)
            << static_cast<quint8>(0x01)
            << static_cast<quint8>(0x45);
    QTest::newRow("Byte index equals to 2, value equals to 0x01234567, expect 0x23")
            << static_cast<quint32>(0x01234567)
            << static_cast<quint8>(0x02)
            << static_cast<quint8>(0x23);
    QTest::newRow("Byte index equals to 3, value equals to 0x01234567, expect 0x01")
            << static_cast<quint32>(0x01234567)
            << static_cast<quint8>(0x03)
            << static_cast<quint8>(0x01);

    QTest::newRow("Byte index equals to 4, value equals to 0x01234567, expect 0x00")
            << static_cast<quint32>(0x01234567)
            << static_cast<quint8>(0x04)
            << static_cast<quint8>(0x00);
}

void ByteTest::test_quint32endianess()
{
    QFETCH(quint32, value);
    QFETCH(quint8, byteIndex);
    QFETCH(quint8, expectedResult);

    quint8 result = EndianessHelper::getByteFromLsbToMsb(value, byteIndex);

    QCOMPARE(expectedResult, result);
}

void ByteTest::test_quint64endianess_data()
{
    QTest::addColumn<quint64>("value");
    QTest::addColumn<quint8>("byteIndex");
    QTest::addColumn<quint8>("expectedResult");

    QTest::newRow("Byte index equals to 0, value equals to 0x0000000000000000, expect 0x00")
            << static_cast<quint64>(0x0000000000000000)
            << static_cast<quint8>(0x00)
            << static_cast<quint8>(0x00);
    QTest::newRow("Byte index equals to 1, value equals to 0x0000000000000000, expect 0x00")
            << static_cast<quint64>(0x0000000000000000)
            << static_cast<quint8>(0x01)
            << static_cast<quint8>(0x00);
    QTest::newRow("Byte index equals to 2, value equals to 0x0000000000000000, expect 0x00")
            << static_cast<quint64>(0x0000000000000000)
            << static_cast<quint8>(0x02)
            << static_cast<quint8>(0x00);
    QTest::newRow("Byte index equals to 3, value equals to 0x0000000000000000, expect 0x00")
            << static_cast<quint64>(0x0000000000000000)
            << static_cast<quint8>(0x03)
            << static_cast<quint8>(0x00);
    QTest::newRow("Byte index equals to 4, value equals to 0x0000000000000000, expect 0x00")
            << static_cast<quint64>(0x0000000000000000)
            << static_cast<quint8>(0x04)
            << static_cast<quint8>(0x00);
    QTest::newRow("Byte index equals to 5, value equals to 0x0000000000000000, expect 0x00")
            << static_cast<quint64>(0x0000000000000000)
            << static_cast<quint8>(0x05)
            << static_cast<quint8>(0x00);
    QTest::newRow("Byte index equals to 6, value equals to 0x0000000000000000, expect 0x00")
            << static_cast<quint64>(0x0000000000000000)
            << static_cast<quint8>(0x06)
            << static_cast<quint8>(0x00);
    QTest::newRow("Byte index equals to 7, value equals to 0x0000000000000000, expect 0x00")
            << static_cast<quint64>(0x0000000000000000)
            << static_cast<quint8>(0x07)
            << static_cast<quint8>(0x00);

    QTest::newRow("Byte index equals to 0, value equals to 0xFFFFFFFFFFFFFFFF, expect 0xFF")
            << static_cast<quint64>(0xFFFFFFFFFFFFFFFF)
            << static_cast<quint8>(0x00)
            << static_cast<quint8>(0xFF);
    QTest::newRow("Byte index equals to 1, value equals to 0xFFFFFFFFFFFFFFFF, expect 0xFF")
            << static_cast<quint64>(0xFFFFFFFFFFFFFFFF)
            << static_cast<quint8>(0x01)
            << static_cast<quint8>(0xFF);
    QTest::newRow("Byte index equals to 2, value equals to 0xFFFFFFFFFFFFFFFF, expect 0xFF")
            << static_cast<quint64>(0xFFFFFFFFFFFFFFFF)
            << static_cast<quint8>(0x02)
            << static_cast<quint8>(0xFF);
    QTest::newRow("Byte index equals to 3, value equals to 0xFFFFFFFFFFFFFFFF, expect 0xFF")
            << static_cast<quint64>(0xFFFFFFFFFFFFFFFF)
            << static_cast<quint8>(0x03)
            << static_cast<quint8>(0xFF);
    QTest::newRow("Byte index equals to 4, value equals to 0xFFFFFFFFFFFFFFFF, expect 0xFF")
            << static_cast<quint64>(0xFFFFFFFFFFFFFFFF)
            << static_cast<quint8>(0x04)
            << static_cast<quint8>(0xFF);
    QTest::newRow("Byte index equals to 5, value equals to 0xFFFFFFFFFFFFFFFF, expect 0xFF")
            << static_cast<quint64>(0xFFFFFFFFFFFFFFFF)
            << static_cast<quint8>(0x05)
            << static_cast<quint8>(0xFF);
    QTest::newRow("Byte index equals to 6, value equals to 0xFFFFFFFFFFFFFFFF, expect 0xFF")
            << static_cast<quint64>(0xFFFFFFFFFFFFFFFF)
            << static_cast<quint8>(0x06)
            << static_cast<quint8>(0xFF);
    QTest::newRow("Byte index equals to 7, value equals to 0xFFFFFFFFFFFFFFFF, expect 0xFF")
            << static_cast<quint64>(0xFFFFFFFFFFFFFFFF)
            << static_cast<quint8>(0x07)
            << static_cast<quint8>(0xFF);

    QTest::newRow("Byte index equals to 0, value equals to 0x0123456789ABCDEF, expect 0xEF")
            << static_cast<quint64>(0x0123456789ABCDEF)
            << static_cast<quint8>(0x00)
            << static_cast<quint8>(0xEF);
    QTest::newRow("Byte index equals to 1, value equals to 0x0123456789ABCDEF, expect 0xCD")
            << static_cast<quint64>(0x0123456789ABCDEF)
            << static_cast<quint8>(0x01)
            << static_cast<quint8>(0xCD);
    QTest::newRow("Byte index equals to 2, value equals to 0x0123456789ABCDEF, expect 0xAB")
            << static_cast<quint64>(0x0123456789ABCDEF)
            << static_cast<quint8>(0x02)
            << static_cast<quint8>(0xAB);
    QTest::newRow("Byte index equals to 3, value equals to 0x0123456789ABCDEF, expect 0x89")
            << static_cast<quint64>(0x0123456789ABCDEF)
            << static_cast<quint8>(0x03)
            << static_cast<quint8>(0x89);
    QTest::newRow("Byte index equals to 4, value equals to 0x0123456789ABCDEF, expect 0x67")
            << static_cast<quint64>(0x0123456789ABCDEF)
            << static_cast<quint8>(0x04)
            << static_cast<quint8>(0x67);
    QTest::newRow("Byte index equals to 5, value equals to 0x0123456789ABCDEF, expect 0x45")
            << static_cast<quint64>(0x0123456789ABCDEF)
            << static_cast<quint8>(0x05)
            << static_cast<quint8>(0x45);
    QTest::newRow("Byte index equals to 6, value equals to 0x0123456789ABCDEF, expect 0x23")
            << static_cast<quint64>(0x0123456789ABCDEF)
            << static_cast<quint8>(0x06)
            << static_cast<quint8>(0x23);
    QTest::newRow("Byte index equals to 7, value equals to 0x0123456789ABCDEF, expect 0x01")
            << static_cast<quint64>(0x0123456789ABCDEF)
            << static_cast<quint8>(0x07)
            << static_cast<quint8>(0x01);

    QTest::newRow("Byte index equals to 8, value equals to 0x0123456789ABCDEF, expect 0x00")
            << static_cast<quint64>(0x0123456789ABCDEF)
            << static_cast<quint8>(0x08)
            << static_cast<quint8>(0x00);
}

void ByteTest::test_quint64endianess()
{
    QFETCH(quint64, value);
    QFETCH(quint8, byteIndex);
    QFETCH(quint8, expectedResult);

    quint8 result = EndianessHelper::getByteFromLsbToMsb(value, byteIndex);

    QCOMPARE(expectedResult, result);
}

void ByteTest::test_setquintendianess()
{
    quint16 setUint16 = EndianessHelper::setBytesFromLsbToMsb(static_cast<quint8>(0x23),
                                                              static_cast<quint8>(0x01));

    QCOMPARE(setUint16, static_cast<quint16>(0x0123));

    quint32 setUint32 = EndianessHelper::setBytesFromLsbToMsb(static_cast<quint8>(0x67),
                                                              static_cast<quint8>(0x45),
                                                              static_cast<quint8>(0x23),
                                                              static_cast<quint8>(0x01));

    QCOMPARE(setUint32, static_cast<quint32>(0x01234567));

    quint64 setUint64 = EndianessHelper::setBytesFromLsbToMsb(static_cast<quint8>(0xEF),
                                                              static_cast<quint8>(0xCD),
                                                              static_cast<quint8>(0xAB),
                                                              static_cast<quint8>(0x89),
                                                              static_cast<quint8>(0x67),
                                                              static_cast<quint8>(0x45),
                                                              static_cast<quint8>(0x23),
                                                              static_cast<quint8>(0x01));

    QCOMPARE(setUint64, static_cast<quint64>(0x0123456789ABCDEF));
}

void ByteTest::test_quint16bytearray_data()
{
    QTest::addColumn<QByteArray>("value");
    QTest::addColumn<bool>("expectedSuccess");
    QTest::addColumn<quint16>("expectedResult");

    QTest::newRow("ByteArray is equal to 0x0000, expect same result: true")
            << static_cast<QByteArray>(QByteArrayLiteral("\x00\x00"))
            << static_cast<bool>(true)
            << static_cast<quint16>(0x0000);
    QTest::newRow("ByteArray is equal to 0xFFFF, expect same result: true")
            << static_cast<QByteArray>(QByteArrayLiteral("\xFF\xFF"))
            << static_cast<bool>(true)
            << static_cast<quint16>(0xFFFF);
    QTest::newRow("ByteArray is equal to 0x0123, expect same result: true")
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\x23"))
            << static_cast<bool>(true)
            << static_cast<quint16>(0x0123);

    QTest::newRow("ByteArray is equal to 0xFF, expect same result: false")
            << static_cast<QByteArray>(QByteArrayLiteral("\xFF"))
            << static_cast<bool>(true)
            << static_cast<quint16>(0x00FF);

    QTest::newRow("ByteArray is equal to 0x01FFFF, expect same result: false")
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\xFF\xFF"))
            << static_cast<bool>(false)
            << static_cast<quint16>(0x0000);
}

void ByteTest::test_quint16bytearray()
{
    QFETCH(QByteArray, value);
    QFETCH(bool, expectedSuccess);
    QFETCH(quint16, expectedResult);

    quint16 result;
    QCOMPARE(ByteArrayHelper::toUintHex(value, result), expectedSuccess);

    QCOMPARE(expectedResult, result);
}

void ByteTest::test_quint32bytearray_data()
{
    QTest::addColumn<QByteArray>("value");
    QTest::addColumn<bool>("expectedSuccess");
    QTest::addColumn<quint32>("expectedResult");

    QTest::newRow("ByteArray is equal to 0x00000000, expect same result: true")
            << static_cast<QByteArray>(QByteArrayLiteral("\x00\x00\x00\x00"))
            << static_cast<bool>(true)
            << static_cast<quint32>(0x00000000);
    QTest::newRow("ByteArray is equal to 0xFFFFFFFF, expect same result: true")
            << static_cast<QByteArray>(QByteArrayLiteral("\xFF\xFF\xFF\xFF"))
            << static_cast<bool>(true)
            << static_cast<quint32>(0xFFFFFFFF);
    QTest::newRow("ByteArray is equal to 0x01234567, expect same result: true")
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\x23\x45\x67"))
            << static_cast<bool>(true)
            << static_cast<quint32>(0x01234567);

    QTest::newRow("ByteArray is equal to 0x01FF, expect same result: false")
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\xFF"))
            << static_cast<bool>(true)
            << static_cast<quint32>(0x000001FF);

    QTest::newRow("ByteArray is equal to 0x01FFFFFFFF, expect same result: false")
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\xFF\xFF\xFF\xFF"))
            << static_cast<bool>(false)
            << static_cast<quint32>(0x00000000);
}

void ByteTest::test_quint32bytearray()
{
    QFETCH(QByteArray, value);
    QFETCH(bool, expectedSuccess);
    QFETCH(quint32, expectedResult);

    quint32 result;
    QCOMPARE(ByteArrayHelper::toUintHex(value, result), expectedSuccess);

    QCOMPARE(expectedResult, result);
}

void ByteTest::test_quint64bytearray_data()
{
    QTest::addColumn<QByteArray>("value");
    QTest::addColumn<bool>("expectedSuccess");
    QTest::addColumn<quint64>("expectedResult");

    QTest::newRow("ByteArray is equal to 0x0000000000000000, expect same result: true")
            << static_cast<QByteArray>(QByteArrayLiteral("\x00\x00\x00\x00\x00\x00\x00\x00"))
            << static_cast<bool>(true)
            << static_cast<quint64>(0x0000000000000000);
    QTest::newRow("ByteArray is equal to 0xFFFFFFFF, expect same result: true")
            << static_cast<QByteArray>(QByteArrayLiteral("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"))
            << static_cast<bool>(true)
            << static_cast<quint64>(0xFFFFFFFFFFFFFFFF);
    QTest::newRow("ByteArray is equal to 0x01234567, expect same result: true")
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\x23\x45\x67\x89\xAB\xCD\xEF"))
            << static_cast<bool>(true)
            << static_cast<quint64>(0x0123456789ABCDEF);

    QTest::newRow("ByteArray is equal to 0x01FF, expect same result: true")
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\xFF"))
            << static_cast<bool>(true)
            << static_cast<quint64>(0x00000000000001FF);

    QTest::newRow("ByteArray is equal to 0x01FFFFFFFF, expect same result: false")
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"))
            << static_cast<bool>(false)
            << static_cast<quint64>(0x0000000000000000);
}

void ByteTest::test_quint64bytearray()
{
    QFETCH(QByteArray, value);
    QFETCH(bool, expectedSuccess);
    QFETCH(quint64, expectedResult);

    quint64 result;
    QCOMPARE(ByteArrayHelper::toUintHex(value, result), expectedSuccess);

    QCOMPARE(expectedResult, result);
}

void ByteTest::test_setquintbytearray()
{
    QByteArray tmp;
    ByteArrayHelper::parseUintToArray(static_cast<quint16>(0x0123), tmp);

    QCOMPARE(tmp, QByteArrayLiteral("\x01\x23"));

    ByteArrayHelper::parseUintToArray(static_cast<quint32>(0x01234567), tmp);

    QCOMPARE(tmp, QByteArrayLiteral("\x01\x23\x45\x67"));

    ByteArrayHelper::parseUintToArray(static_cast<quint64>(0x0123456789ABCDEF), tmp);

    QCOMPARE(tmp, QByteArrayLiteral("\x01\x23\x45\x67\x89\xAB\xCD\xEF"));
}

void ByteTest::test_stringbytearray_data()
{
    QTest::addColumn<QString>("value");
    QTest::addColumn<bool>("expectedSuccess");
    QTest::addColumn<QByteArray>("expectedResult");

    QTest::newRow("Value is equal to \"0x0000000000000000\", expect same result: true")
            << static_cast<QString>("0x0000000000000000")
            << static_cast<bool>(true)
            << static_cast<QByteArray>(QByteArrayLiteral("\x00\x00\x00\x00\x00\x00\x00\x00"));

    QTest::newRow("Value is equal to \"0xFFFFFFFFFFFFFFFF\", expect same result: true")
            << static_cast<QString>("0xFFFFFFFFFFFFFFFF")
            << static_cast<bool>(true)
            << static_cast<QByteArray>(QByteArrayLiteral("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"));

    QTest::newRow("Value is equal to \"0x0123456789ABCDEF\", expect same result: true")
            << static_cast<QString>("0x0123456789ABCDEF")
            << static_cast<bool>(true)
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\x23\x45\x67\x89\xAB\xCD\xEF"));

    QTest::newRow("Value is equal to \"0x123AB\", expect same result: true")
            << static_cast<QString>("0x123AB")
            << static_cast<bool>(true)
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\x23\xAB"));

    QTest::newRow("Value is equal to \"0X123AB\", expect same result: true")
            << static_cast<QString>("0X123AB")
            << static_cast<bool>(true)
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\x23\xAB"));

    QTest::newRow("Value is equal to \"123AB\", expect same result: true")
            << static_cast<QString>("123AB")
            << static_cast<bool>(true)
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\x23\xAB"));

    QTest::newRow("Value is equal to \"0h123AB\", expect same result: true")
            << static_cast<QString>("0h123AB")
            << static_cast<bool>(true)
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\x23\xAB"));

    QTest::newRow("Value is equal to \"0H123AB\", expect same result: true")
            << static_cast<QString>("0H123AB")
            << static_cast<bool>(true)
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\x23\xAB"));

    QTest::newRow("Value is equal to \"123ABh\", expect same result: true")
            << static_cast<QString>("123ABh")
            << static_cast<bool>(true)
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\x23\xAB"));

    QTest::newRow("Value is equal to \"123ABH\", expect same result: true")
            << static_cast<QString>("123ABH")
            << static_cast<bool>(true)
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\x23\xAB"));

    QTest::newRow("Value is equal to \"t123AB\", expect same result: true, linked to test limitation")
            << static_cast<QString>("t123AB")
            << static_cast<bool>(true)
            << static_cast<QByteArray>(QByteArray("\x01\x23\xAB"));

    QTest::newRow("Value is equal to \"123ABj\", expect same result: true, linked to test limitation")
            << static_cast<QString>("123ABj")
            << static_cast<bool>(true)
            << static_cast<QByteArray>(QByteArray("\x01\x23\xAB"));

    QTest::newRow("Value is equal to \"12Y3AB\", expect same result: true, linked to test limitation")
            << static_cast<QString>("12Y3AB")
            << static_cast<bool>(true)
            << static_cast<QByteArray>(QByteArray("\x01\x23\xAB"));

    QTest::newRow("Value is equal to \"t0123AB\", expect same result: true, linked to test limitation")
            << static_cast<QString>("t0123AB")
            << static_cast<bool>(false)
            << static_cast<QByteArray>(QByteArray());

    QTest::newRow("Value is equal to \"accrobate\", expect same result: false")
            << static_cast<QString>("accrobate")
            << static_cast<bool>(false)
            << static_cast<QByteArray>(QByteArray());

    QTest::newRow("Value is equal to \"0123ABj\", expect same result: false")
            << static_cast<QString>("0123ABj")
            << static_cast<bool>(false)
            << static_cast<QByteArray>(QByteArray());
}

void ByteTest::test_stringbytearray()
{
    QFETCH(QString, value);
    QFETCH(bool, expectedSuccess);
    QFETCH(QByteArray, expectedResult);

    QByteArray tmp;
    QCOMPARE(ByteArrayHelper::parseFromHex(value, tmp), expectedSuccess);

    QCOMPARE(tmp, expectedResult);
}

void ByteTest::test_tostringbytearray_data()
{
    QTest::addColumn<QByteArray>("value");
    QTest::addColumn<int>("byteNb");
    QTest::addColumn<QString>("expectedResult");

    QTest::newRow("Value is equal to 0xFFFFFF, byteNb is equal to: -1, expect same result: true")
            << static_cast<QByteArray>(QByteArrayLiteral("\xFF\xFF\xFF"))
            << static_cast<int>(-1)
            << static_cast<QString>("0xffffff");

    QTest::newRow("Value is equal to 0x000000, byteNb is equal to: -1, expect same result: true")
            << static_cast<QByteArray>(QByteArrayLiteral("\x00\x00\x00"))
            << static_cast<int>(-1)
            << static_cast<QString>("0x000000");

    QTest::newRow("Value is equal to 0x012345, byteNb is equal to: -1, expect same result: true")
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\x23\x45"))
            << static_cast<int>(-1)
            << static_cast<QString>("0x012345");

    QTest::newRow("Value is equal to 0x012345, byteNb is equal to: 2, expect same result: true")
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\x23\x45"))
            << static_cast<int>(2)
            << static_cast<QString>("0x012345");

    QTest::newRow("Value is equal to 0x01234567, byteNb is equal to: 5, result: \"0x0001234567\"")
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\x23\x45\x67"))
            << static_cast<int>(5)
            << static_cast<QString>("0x0001234567");
}

void ByteTest::test_tostringbytearray()
{
    QFETCH(QByteArray, value);
    QFETCH(int, byteNb);
    QFETCH(QString, expectedResult);

    QString tmp = ByteArrayHelper::toStringHex(value, true, byteNb);

    QCOMPARE(tmp, expectedResult);
}

QTEST_MAIN(ByteTest)
