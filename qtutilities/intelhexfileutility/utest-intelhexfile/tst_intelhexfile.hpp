// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

class IntelHexFileTest : public QObject
{
    Q_OBJECT

    public:
        IntelHexFileTest();

    private slots:
        void test_loading_data();
        void test_loading();
        void test_reading_hextest1_data();
        void test_reading_hextest1();
        void test_reading_hextest2_data();
        void test_reading_hextest2();
        void test_reading_hextest3_data();
        void test_reading_hextest3();
        void test_writing_hextest1_data();
        void test_writing_hextest1();
        void test_writing_hextest2_data();
        void test_writing_hextest2();
        void test_writing_hextest3_data();
        void test_writing_hextest3();

    private:
        void readingTestsData(const QString &hexFileName);

        void readingTest(const char *hexData);

        void writingTestsData(const QString &hexFileName);

        void writingTest(const char *hexData);

    private:
        static const constexpr char *hexFileTest1 =
                ":10000000" "52554E21464143547837000068A60000" "FF\n"
                ":10001000" "7837000068A6000000000000B0010000" "72\n"
                ":10002000" "FB000000000000000000000000000000" "D5\n"
                ":10003000" "00000000000000000000000000000000" "C0\n"
                ":10004000" "00000000000000000000000301026100" "49\n"
                ":10005000" "0002000203046101FFFFFFFFFFFFFFFF" "3B\n"
                ":10006000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "A0\n"
                ":10007000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "90\n"
                ":10008000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "80\n"
                ":10009000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "70\n"
                ":1000A000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "60\n"
                ":1000B000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "50\n"
                ":1000C000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF12" "2D\n"
                ":1000D000" "36FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "F9\n"
                ":1000E000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "20\n"
                ":1000F000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "10\n"
                ":00000001"                                    "FF\n";

        static const constexpr char *hexFileTest2 =
                ":10001000" "7837000068A6000000000000B0010000" "72\n"
                ":10000000" "52554E21464143547837000068A60000" "FF\n"
                ":10003000" "00000000000000000000000000000000" "C0\n"
                ":10002000" "FB000000000000000000000000000000" "D5\n"
                ":10006000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "A0\n"
                ":10004000" "00000000000000000000000301026100" "49\n"
                ":10007000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "90\n"
                ":10005000" "0002000203046101FFFFFFFFFFFFFFFF" "3B\n"
                ":10009000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "70\n"
                ":10008000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "80\n"
                ":1000A000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "60\n"
                ":1000C000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF12" "2D\n"
                ":1000B000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "50\n"
                ":1000E000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "20\n"
                ":1000D000" "36FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "F9\n"
                ":1000F000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "10\n"
                ":00000001"                                    "FF\n";

        static const constexpr char *hexFileTest3 =
                ":10000000" "52554E21464143547837000068A60000" "FF\n"
                ":10001000" "7837000068A6000000000000B0010000" "72\n"
                ":10002000" "FB000000000000000000000000000000" "D5\n"
                ":10003000" "00000000000000000000000000000000" "C0\n"
                ":0E004000" "0000000000000000000000030102" "AC\n"
                ":12004E00" "61000002000203046101FFFFFFFFFFFFFFFF" "DA\n"
                ":10006000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "A0\n"
                ":10007000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "90\n"
                ":10008000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "80\n"
                ":10009000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "70\n"
                ":1000A000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "60\n"
                ":1000B000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "50\n"
                ":1000C000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF12" "2D\n"
                ":1000D000" "36FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "F9\n"
                ":1000E000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "20\n"
                ":1000F000" "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" "10\n"
                ":00000001"                                    "FF\n";

        // Wrong hex file: first line checksum isn't correct
        static const constexpr char *hexFileTest4 =
                ":10000000" "52554E21464143547837000068A60000" "00\n"
                ":10001000" "7837000068A6000000000000B0010000" "72\n"
                ":00000001"                                    "FF\n";

        // Wrong hex file: miss end of file
        static const constexpr char *hexFileTest5 =
                ":10000000" "52554E21464143547837000068A60000" "FF\n"
                ":10001000" "7837000068A6000000000000B0010000" "72\n";
};
