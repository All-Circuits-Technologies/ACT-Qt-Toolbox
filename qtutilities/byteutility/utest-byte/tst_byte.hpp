// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


class ByteTest : public QObject
{
    Q_OBJECT

    public:
        ByteTest();
        ~ByteTest();

    private slots:
        void test_quint16endianess_data();
        void test_quint16endianess();
        void test_quint32endianess_data();
        void test_quint32endianess();
        void test_quint64endianess_data();
        void test_quint64endianess();
        void test_setquintendianess();
        void test_quint16bytearray_data();
        void test_quint16bytearray();
        void test_quint32bytearray_data();
        void test_quint32bytearray();
        void test_quint64bytearray_data();
        void test_quint64bytearray();
        void test_setquintbytearray();
        void test_stringbytearray_data();
        void test_stringbytearray();
        void test_tostringbytearray_data();
        void test_tostringbytearray();
};
