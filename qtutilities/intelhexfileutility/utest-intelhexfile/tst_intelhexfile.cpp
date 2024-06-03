// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tst_intelhexfile.hpp"

#include <QtTest>

#include "intelhexfileutility/intelhexdevice.hpp"


IntelHexFileTest::IntelHexFileTest() : QObject()
{
}

void IntelHexFileTest::test_loading_data()
{
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<bool>("loadResult");

    QTest::newRow("Data to load: hexFileTest1, success: true")
            << QByteArray(hexFileTest1) << true;
    QTest::newRow("Data to load: hexFileTest2, success: true")
            << QByteArray(hexFileTest2) << true;
    QTest::newRow("Data to load: hexFileTest3, success: true")
            << QByteArray(hexFileTest3) << true;
    QTest::newRow("Data to load: hexFileTest4, success: false")
            << QByteArray(hexFileTest4) << false;
    QTest::newRow("Data to load: hexFileTest5, success: false")
            << QByteArray(hexFileTest5) << false;
}

void IntelHexFileTest::test_loading()
{
    QFETCH(QByteArray, data);
    QFETCH(bool, loadResult);

    IntelHexDevice hexFile;
    QBuffer buffer(&data);

    QVERIFY(hexFile.linkToHexDevice("HexFileTest1", &buffer, false));

    QCOMPARE(hexFile.loadDevice(), loadResult);
}

void IntelHexFileTest::test_reading_hextest1_data()
{
    readingTestsData("HexFileTest1");
}

void IntelHexFileTest::test_reading_hextest1()
{
    readingTest(hexFileTest1);
}

void IntelHexFileTest::test_reading_hextest2_data()
{
    readingTestsData("HexFileTest2");
}

void IntelHexFileTest::test_reading_hextest2()
{
    readingTest(hexFileTest2);
}

void IntelHexFileTest::test_reading_hextest3_data()
{
    readingTestsData("HexFileTest3");
}

void IntelHexFileTest::test_reading_hextest3()
{
    readingTest(hexFileTest3);
}

void IntelHexFileTest::test_writing_hextest1_data()
{
    writingTestsData("HexFileTest1");
}

void IntelHexFileTest::test_writing_hextest1()
{
    writingTest(hexFileTest1);
}

void IntelHexFileTest::test_writing_hextest2_data()
{
    writingTestsData("HexFileTest2");
}

void IntelHexFileTest::test_writing_hextest2()
{
    writingTest(hexFileTest2);
}

void IntelHexFileTest::test_writing_hextest3_data()
{
    writingTestsData("HexFileTest3");
}

void IntelHexFileTest::test_writing_hextest3()
{
    writingTest(hexFileTest3);
}

void IntelHexFileTest::readingTestsData(const QString &hexFileName)
{
    QTest::addColumn<quint16>("address");
    QTest::addColumn<int>("size");
    QTest::addColumn<QByteArray>("expectedData");
    QTest::addColumn<bool>("expectGetSuccess");

    QTest::newRow("Read bytes in " + hexFileName.toLatin1() +
                  ", address: 0x0008, size: 1, expectedData: 0x78")
            << static_cast<quint16>(0x0008)
            << static_cast<int>(1)
            << static_cast<QByteArray>(QByteArrayLiteral("\x78"))
            << true;

    QTest::newRow("Read bytes in " + hexFileName.toLatin1() +
                  ", address: 0x001C, size: 1, expectedData: 0xB0")
            << static_cast<quint16>(0x001C)
            << static_cast<int>(1)
            << static_cast<QByteArray>(QByteArrayLiteral("\xB0"))
            << true;

    QTest::newRow("Read bytes in " + hexFileName.toLatin1() +
                  ", address: 0x0020, size: 2, expectedData: 0xFB00")
            << static_cast<quint16>(0x0020)
            << static_cast<int>(2)
            << static_cast<QByteArray>(QByteArrayLiteral("\xFB\x00"))
            << true;

    QTest::newRow("Read bytes in " + hexFileName.toLatin1() +
                  ", address: 0x004E, size: 2, expectedData: 0x6100")
            << static_cast<quint16>(0x004E)
            << static_cast<int>(2)
            << static_cast<QByteArray>(QByteArrayLiteral("\x61\x00"))
            << true;

    QTest::newRow("Read bytes in " + hexFileName.toLatin1() +
                  ", address: 0x0057, size: 9, expectedData: "
                  "0x01FFFFFFFFFFFFFFFF")
            << static_cast<quint16>(0x0057)
            << static_cast<int>(9)
            << static_cast<QByteArray>(QByteArrayLiteral("\x01\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"))
            << true;

    QTest::newRow("Read bytes in " + hexFileName.toLatin1() +
                  ", address: 0x00CF, size: 2, expectedData: "
                  "0x1236")
            << static_cast<quint16>(0x00CF)
            << static_cast<int>(2)
            << static_cast<QByteArray>(QByteArrayLiteral("\x12\x36"))
            << true;

    QTest::newRow("Read bytes in " + hexFileName.toLatin1() +
                  ", address: 0x0100, size: 1, expect error: address "
                  "overflows")
            << static_cast<quint16>(0x0100)
            << static_cast<int>(1)
            << static_cast<QByteArray>(QByteArray())
            << false;

    QTest::newRow("Read bytes in " + hexFileName.toLatin1() +
                  ", address: 0x00FF, size: 2, expect error: only size "
                  "overflows")
            << static_cast<quint16>(0x00FF)
            << static_cast<int>(2)
            << static_cast<QByteArray>(QByteArray())
            << false;
}

void IntelHexFileTest::readingTest(const char *hexData)
{
    QFETCH(quint16, address);
    QFETCH(int, size);
    QFETCH(QByteArray, expectedData);
    QFETCH(bool, expectGetSuccess);

    IntelHexDevice hexFile;
    QByteArray data(hexData);
    QBuffer buffer(&data);

    QVERIFY(hexFile.linkToHexDevice("HexFileTest", &buffer, false));

    QVERIFY(hexFile.loadDevice());

    QByteArray dataGot;
    QCOMPARE(hexFile.getData(address, size, dataGot), expectGetSuccess);

    QCOMPARE(expectedData, dataGot);
}

void IntelHexFileTest::writingTestsData(const QString &hexFileName)
{
    QTest::addColumn<quint16>("address");
    QTest::addColumn<QByteArray>("dataToModify");
    QTest::addColumn<bool>("expectSuccess");

    QTest::newRow("Write bytes in " + hexFileName.toLatin1() +
                  ", address: 0x0008, dataToModify: 0xCC")
            << static_cast<quint16>(0x0008)
            << static_cast<QByteArray>(QByteArrayLiteral("\xCC"))
            << true;

    QTest::newRow("Write bytes in " + hexFileName.toLatin1() +
                  ", address: 0x0018, dataToModify: 0xCC")
            << static_cast<quint16>(0x0018)
            << static_cast<QByteArray>(QByteArrayLiteral("\xCC"))
            << true;

    QTest::newRow("Write bytes in " + hexFileName.toLatin1() +
                  ", address: 0x0010, dataToModify: 0xAA")
            << static_cast<quint16>(0x0010)
            << static_cast<QByteArray>(QByteArrayLiteral("\xAA"))
            << true;

    QTest::newRow("Write bytes in " + hexFileName.toLatin1() +
                  ", address: 0x001F, dataToModify: 0xAF")
            << static_cast<quint16>(0x001F)
            << static_cast<QByteArray>(QByteArrayLiteral("\xAA"))
            << true;

    QTest::newRow("Write bytes in " + hexFileName.toLatin1() +
                  ", address: 0x001F, dataToModify: 0xAFCB")
            << static_cast<quint16>(0x001F)
            << static_cast<QByteArray>(QByteArrayLiteral("\xAF\xCB"))
            << true;

    QTest::newRow("Write bytes in " + hexFileName.toLatin1() +
                  ", address: 0x0100, dataToModify: 0xAF")
            << static_cast<quint16>(0x0100)
            << static_cast<QByteArray>(QByteArrayLiteral("\xAF"))
            << false;

    QTest::newRow("Write bytes in " + hexFileName.toLatin1() +
                  ", address: 0x00FF, dataToModify: 0xAFCB")
            << static_cast<quint16>(0x00FF)
            << static_cast<QByteArray>(QByteArrayLiteral("\xAF\xCB"))
            << false;
}

void IntelHexFileTest::writingTest(const char *hexData)
{
    QFETCH(quint16, address);
    QFETCH(QByteArray, dataToModify);
    QFETCH(bool, expectSuccess);

    IntelHexDevice hexFile;
    QByteArray data(hexData);
    QBuffer buffer(&data);

    QVERIFY(hexFile.linkToHexDevice("HexFileTest", &buffer, false));

    QVERIFY(hexFile.loadDevice());

    QCOMPARE(hexFile.setData(address, dataToModify, true), expectSuccess);

    if(!expectSuccess)
    {
        // Useless to go forward
        return;
    }

    QByteArray dataGot;
    QVERIFY(hexFile.getData(address, dataToModify.length(), dataGot));

    QCOMPARE(dataToModify, dataGot);

    QVERIFY(hexFile.linkToHexDevice("HexFileTestModified", &buffer, false));

    QVERIFY(hexFile.loadDevice());

    QVERIFY(hexFile.getData(address, dataToModify.length(), dataGot));

    QCOMPARE(dataToModify, dataGot);
}

QTEST_MAIN(IntelHexFileTest)
