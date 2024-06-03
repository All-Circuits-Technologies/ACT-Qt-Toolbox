// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "recordtype.hpp"


/** @brief An object representation of a intel hex line */
class HexLine : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @note This will create an invalid HexLine
            @param parent The parent instance class */
        explicit HexLine(QObject *parent = nullptr);

        /** @brief Class constructor
            @param addressMsbFirst The address of the line, written in MSB first
            @param recordType The record type linked to the HexLine
            @param data The data written in the line
            @param parent The parent instance class */
        explicit HexLine(quint16 addressMsbFirst,
                         RecordType::Enum recordType,
                         const QByteArray &data,
                         QObject *parent = nullptr);

    public:
        /** @brief Say if the HexLine is valid
            @return True if valid */
        bool isValid() const { return _valid; }

        /** @brief Get the type of record linked to the line */
        RecordType::Enum getRecordType() const { return _recordType; }

        /** @brief Test if the address given is in the line
            @param addressMsbFirst The address to find in the line (MSB first)
            @return True if the address is contained in the line */
        bool isContained(quint16 addressMsbFirst) const;

        /** @brief Get the line address */
        quint16 getAddressMsbFirst() const { return _addressMsbFirst; }

        /** @brief Get the last address contained in the line */
        quint16 getLastAddressMsbFirst() const { return _lastAddressMsbFirst; }

        /** @brief Get the wanted data
            @note The method will exit on error, if the address or the size is over the line
            @param addressMsbFirst The data address to get
            @param size The data size to get
            @param data The data got from the line
            @return True if no problem occurs */
        bool getData(quint16 addressMsbFirst, quint8 size, QByteArray &data) const;

        /** @brief Set the wanted data in the line
            @param addressMsbFirst The data address to set
            @param data The data to set
            @return True if no problem occurs */
        bool setData(quint16 addressMsbFirst, const QByteArray &data);

        /** @brief Return a string representation of the line (which can be added in a intel hex
                   file)
            @param line The string representation of the line
            @return True if no problem occurs */
        bool toHexLine(QString &line);

        /** @brief Returns a byte array representation of the line
            @note All the bytes are stored in hexa and not in ASCII, for instance:
                    We manage the byte 0xCF at address 0, it will be store like it is in the byte
                    are: 0xCF and not in ASCII values: 0x4346
            @note The returned byte array doesn't contain the start code character
            @param line The byte array representation of the line
            @return True if no problem occurs */
        bool toHexLine(QByteArray &line);

        /** @brief Returns the size the line would be if we export it in string
            @param size The size of the line (in string)
            @return True if no problem occurs */
        bool getStringSize(int &size);

    public:
        /** @brief Create a specific "end of line" line
            @param parent The parent object instance
            @return The hex line parsed or null if a problem occured */
        static HexLine *createEndOfLineFile(QObject *parent = Q_NULLPTR);

        /** @brief Parse the hex line from string
            @param line The string line to parse
            @param parent The parent object instance
            @return The hex line parsed or null if a problem occured  */
        static HexLine *parseHexLine(const QString &line, QObject *parent = Q_NULLPTR);

        /** @brief Parse the hex line from byte array
            @note The bytes in the array have to be pure bytes and not ASCII values representation
            @note The bytes line can't contain the start code (so the bytesLine param is the bytes
                  representation of the line without the start code char)
            @param bytesLine The bytes representation of the line
            @param parent The parent instance class
            @return The hex line parsed or null if a problem occured */
        static HexLine *parseHexLine(const QByteArray &bytesLine, QObject *parent = Q_NULLPTR);

    private:
        /** @brief This method recalculates the checksum of the line and set the instance checksum
            @return True if no problem occurs */
        bool recalculateChecksum();

        /** @brief Calculate the last address in the line of the last byte
            @note The calculation will be done thanks to the address and the bytes length */
        void calculateLastAddress();

    private:
        static const constexpr char StartCodeChar = ':'; // 0x3A

        static const constexpr quint8 ByteNbIndex = 0;

        static const constexpr quint8 AddressIndex = 1;

        static const constexpr quint8 RecordTypeIndex = 3;

        static const constexpr quint8 DataIndex = 4;

        /** @brief Number of all the fixed bytes in the line
            @note We have:
                  - 1 byte for the data byte number
                  - 2 bytes for the address
                  - 1 byte for the record type
                  - 1 byte for the checksum */
        static const constexpr quint8 FixedBytesNbInLine = 5;

        static const constexpr int HexBase = 16;

    private:
        quint16 _addressMsbFirst{0};
        quint16 _lastAddressMsbFirst{0};
        RecordType::Enum _recordType{RecordType::Unknown};
        QByteArray _data;
        quint8 _checksum{0};

        bool _valid{false};
};
