// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <functional>
#include <QVector>

class HexLine;
class QIODevice;


/** @brief This class represents the content of an intel hex file
    @note To use it, you will have to link to a @ref QIODevice and then load in memory the file
          content */
class IntelHexDevice : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent class instance*/
        explicit IntelHexDevice(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~IntelHexDevice() override;

    public:
        /** @brief Link the instance with a file
            @note The file needs to exist or an error will be raised
            @note This method creates a QFile and then call the method @see linkToHexDevice
            @param fileName The name of the file to link with
            @return True if no problem occurs */
        bool linkToHexFile(const QString &fileName);

        /** @brief Link the instance with a QIODevice
            @note If the param giveDeviceOwnership is equal to true, and if an errors occurs in the
                  method, it will delete the object
            @param deviceName The name of the device
            @param device The device to read, the device can't be sequential
            @param giveDeviceOwnership If true the class will take care of the device instance life.
                                       If an error occurs in this method and if the param is equal
                                       to true, it will delete the object
                                       If false, the class will never delete the device object,
                                       this can be used if the QIODevice has been locally created.
            @return True if no problem occurs */
        bool linkToHexDevice(const QString &deviceName,
                             QIODevice *device,
                             bool giveDeviceOwnership = true);

        /** @brief Load the device content
            @note This method need to be called before calling read and write method
            @note All the file content is loaded in memory. If the content is large, it will be
                  better to creat a lazy loading method
            @return True if no problem occurs */
        bool loadDevice();

        /** @brief Get a data thanks to the address and length
            @note The method returns false if the address or size wanted overflows the file content
            @param address The address where to get the data
            @param length The length of data to get
            @param data The data got
            @return True if no problem occurs */
        bool getData(quint16 address, int length, QByteArray &data);

        /** @brief Set the data to memory
            @note The method returns false if the address or size overflows the current file content
            @note The method can't append data to the file, it can only update data
            @param address The address where to set the data
            @param data The data to set
            @param flushToDevice True to write the data into memory and then to the device,
                                 False to only write the data into memory
            @return True if no problem occurs */
        bool setData(quint16 address, const QByteArray &data, bool flushToDevice = false);

    private:
        /** @brief Try to find the first "end of line" occurence in buffer
            @note The method will try different "end of line" type: \n, \r\n or \r, to estimate
                  what "end of line" is used
            @param buffer The string to search in
            @param fromIndex Search the end of line from this index in the buffer
            @return The index of the end of line in the buffer, or -1 if nothing has been found */
        int indexOfEndOfLine(const QString &buffer, int fromIndex = 0);

        /** @brief Try to find the end of line, thanks to expected end of line given
            @param buffer The string to search in
            @param expectedEndOfLine The "end of line" string to use for searching
            @param fromIndex Search the end of line from this index in the buffer
            @return The index of the end of line in the buffer, or -1 if nothing has been found */
        int findEndOfLine(const QString &buffer,
                          const QString &expectedEndOfLine,
                          int fromIndex = 0);

        /** @brief Read all data from device and load the Hex lines into RAM
            @return True if no problem occurs */
        bool readAllDataFromDevice();

        /** @brief Helpful method to find the lines involved by the address and length given
            @param address The address to find in lines
            @param length The length of wanted data
            @param actOnData This function  will be called each time a line is involved with the
                             given address and length
                             The function address and size parameters are pieces of the global
                             address and size, and are included in the the given line
            @return True if no problem occurs */
        bool actOnLines(quint16 address,
                        int length,
                        const std::function<bool (quint16 addrToActWith,
                                                  quint16 sizeToActWith,
                                                  int lineIdx,
                                                  HexLine *line)> &actOnData);

        /** @brief Try to find a line which contains the given address
            @note This method does a circular search from the given line idx, to the previous line
                  idx (each line is tested once (if no line has been found))
            @param address The address to find in the line
            @param lineIdx The index of the line found or 0 if nothing has been found
            @param fromLineIdx The index to search the line from
            @return The line found or nullptr if nothing has been found */
        HexLine *findLine(quint16 address, int &lineIdx, int fromLineIdx = 0);

        /** @brief Write specific lines to the device
            @note The file will be written from the first line index given to the last line index
            @param firstLineIdx The index of the first line to write into the device
            @param lastLineIdx The index of the last line to write into the device
            @return True if no problem occurs */
        bool writeLinesToDevice(int firstLineIdx, int lastLineIdx);

        /** @brief Calculate the position in the file of the line index given
            @param lineIdx The index of the line to search its position in the file
            @param pos The position found in the file
            @return True if no problem occurs */
        bool calculateFirstPos(int lineIdx, qint64 &pos);

    private:
        static const constexpr char* carriageReturn = "\r";
        static const constexpr char* lineFeed = "\n";
        static const constexpr char* crLf ="\r\n";

        static const constexpr int bufferSize = 1024;

    private:
        QIODevice *_hexDevice{nullptr};
        bool _hasDeviceOwnership{false};
        QString _deviceName;
        QVector<HexLine*> _fileLines;
        QString _endOfLine;
};
