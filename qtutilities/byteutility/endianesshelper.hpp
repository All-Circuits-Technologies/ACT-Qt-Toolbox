// SPDX-FileCopyrightText: 2018 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QDebug>


/*! @brief Help to manage the endianess of bytes */
class EndianessHelper
{
    public:
        /*! @brief Get byte from bytes, from LSB to MSB
            @note If byteIndex == 0, return the LSB
                  If byteIndex == 7, return the MSB
            @param bytes The bytes to get the byte from
            @param byteIndex The index from the lowest significant byte to the most significant
                             byte
            @return The byte wanted */
        static quint8 getByteFromLsbToMsb(quint64 bytes, quint8 byteIndex)
        { return getByte<quint64>(bytes, byteIndex, ByteNbInUint64); }

        /*! @brief Get byte from bytes, from LSB to MSB
            @note If byteIndex == 0, return the LSB
                  If byteIndex == 3, return the MSB
            @param bytes The bytes to get the byte from
            @param byteIndex The index from the lowest significant byte to the most significant
                             byte
            @return The byte wanted */
        static quint8 getByteFromLsbToMsb(quint32 bytes, quint8 byteIndex)
        { return getByte<quint32>(bytes, byteIndex, ByteNbInUint32); }

        /*! @brief Get byte from bytes, from LSB to MSB
            @note If byteIndex == 0, return the LSB
                  If byteIndex == 1, return the MSB
            @param bytes The bytes to get the byte from
            @param byteIndex The index from the lowest significant byte to the most significant
                             byte
            @return The byte wanted */
        static quint8 getByteFromLsbToMsb(quint16 bytes, quint8 byteIndex)
        { return getByte<quint16>(bytes, byteIndex, ByteNbInUint16); }

        /*! @brief Set a bytes list to a quint16, from LSB to MSB
            @param lsbByte The LSB byte of the quint16
            @param msbByte The MSB byte of the quint16
            @return The quint16 wanted */
        static quint16 setBytesFromLsbToMsb(quint8 lsbByte, quint8 msbByte)
        { return setBytes<quint16>({ lsbByte, msbByte }, ByteNbInUint16); }

        /*! @brief Set a bytes list to a quint32, from LSB to MSB
            @param lsbByte The LSB byte of the quint32
            @param lsbMidByte The first byte after the LSB byte of the quint32
            @param msbMidByte The second byte after the LSB byte of the quint32
            @param msbByte The third byte after the LSB byte (the MSB byte) of the quint32
            @return The quint32 wanted */
        static quint32 setBytesFromLsbToMsb(quint8 lsbByte,
                                            quint8 lsbMidByte,
                                            quint8 msbMidByte,
                                            quint8 msbByte)
        { return setBytes<quint32>({ lsbByte, lsbMidByte, msbMidByte, msbByte }, ByteNbInUint32); }

        /*! @brief Set a bytes list to a quint64, from LSB to MSB
            @param lsbByte The LSB byte of the quint64
            @param lsbLowByte The first byte after the LSB byte of the quint64
            @param lsbMidByte The second byte after the LSB byte of the quint64
            @param lsbHighByte The third byte after the LSB byte of the quint64
            @param msbLowByte The fourth byte after the LSB byte of the quint64
            @param msbMidByte The fifth byte after the LSB byte of the quint64
            @param msbHighByte The sixth byte after the LSB byte of the quint64
            @param msbByte The seventh byte after the LSB byte of the quint64
            @return The quint64 wanted */
        static quint64 setBytesFromLsbToMsb(quint8 lsbByte,
                                            quint8 lsbLowByte,
                                            quint8 lsbMidByte,
                                            quint8 lsbHighByte,
                                            quint8 msbLowByte,
                                            quint8 msbMidByte,
                                            quint8 msbHighByte,
                                            quint8 msbByte)
        { return setBytes<quint64>({ lsbByte,
                                     lsbLowByte,
                                     lsbMidByte,
                                     lsbHighByte,
                                     msbLowByte,
                                     msbMidByte,
                                     msbHighByte,
                                     msbByte },
                                     ByteNbInUint64); }

    private:
        /*! @brief Get byte from bytes, from LSB to MSB
            @note The bytes given has to be a quint16, a quint32 or a quint64
            @note If byteIndex == 0, return the LSB
                  If byteIndex == bytesNb, return the MSB
            @param bytes The bytes to get the byte from
            @param byteIndex The index from the lowest significant byte to the most significant
                             byte
            @param bytesNb The integer bytes number*/
        template<class T>
        static quint8 getByte(T bytes, quint8 byteIndex, quint8 bytesNb);

        /*! @brief Set a byte list into a group of bytes, from LSB to MSB
            @note The group of bytes given has to be a quint16, a quint32 or a quint64
            @note The first element of the vector list is the LSB element, and so last one is the
                  MSB
            @param bytes The bytes to insert in the group of bytes (LSB first)
            @param bytesNb The integer bytes number
            @return The bytes group to fill */
        template<class T>
        static T setBytes(const QVector<quint8> &bytes, quint8 bytesNb);


        /*! @brief Verify if the index doesn't overflow the bytes nb
            @param index An index
            @param bytesNb The number to not overflow
            @return True if the index doesn't overflow the number*/
        inline static bool verifyIndex(quint8 index, quint8 bytesNb) Q_DECL_CONST_FUNCTION
        {
            return (bytesNb > index);
        }

    public:
        static const constexpr quint8 ByteNbInUint64 = 8;
        static const constexpr quint8 ByteNbInUint32 = 4;
        static const constexpr quint8 ByteNbInUint16 = 2;
        static const constexpr quint8 ByteNbInUint8  = 1;

    private:
        static const constexpr int NbOfBitsInByte = 8;
        static const constexpr int ByteMask = 0xff;
};

template<class T>
quint8 EndianessHelper::getByte(T bytes, quint8 byteIndex, quint8 bytesNb)
{
    if(!verifyIndex(byteIndex, bytesNb))
    {
        qWarning() << "The byteIndex: "  << byteIndex << " overflow the size of the primary type";
        return 0;
    }

    quint8 bitIndex = byteIndex * EndianessHelper::NbOfBitsInByte;
    return ((bytes & (static_cast<T>(EndianessHelper::ByteMask) << bitIndex)) >> bitIndex);
}

template<class T>
T EndianessHelper::setBytes(const QVector<quint8> &bytes, quint8 bytesNb)
{
    if(bytes.count() > bytesNb)
    {
        qWarning() << "The number of bytes: "  << bytes.count() << " overflows the size of "
                   << "the primary type";
        return 0;
    }

    T bytesElement = 0;

    // In the calling method the bytes list can't contain more than the max of a quint8 nb
    // NOLINTNEXTLINE(bugprone-too-small-loop-variable)
    for(quint8 idx = 0; idx < bytes.count(); idx++)
    {
        bytesElement |= (static_cast<T>(bytes.at(idx)) << (idx * EndianessHelper::NbOfBitsInByte));
    }

    return bytesElement;
}
