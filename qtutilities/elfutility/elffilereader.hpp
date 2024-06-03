// SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

/* ------------------------------------------------------------------
 * elffilereader.hpp - COPYRIGHT (C) 2019 ALL CIRCUITS
 * ------------------------------------------------------------------
 */

#pragma once

#include <QObject>

namespace ELFIO {
    class elfio;
    class section;
}


/** @brief Helper to parse an ELF file */
class ElfFileReader : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent of the object */
        explicit ElfFileReader(QObject *parent = nullptr);

        /** @brief Load an ELF file
            @param elfFilePath The path of the ELF file to load
            @return True if no problem occurs */
        bool loadElfFile(const QString &elfFilePath);

    private:
        /** @brief Get physical address of a section (if the physical address exists)
            @param section The section to get address from
            @param physAddr The physical address found
            @return True if no problem occurs and if the physical address has been found */
        bool getSectionPhysicalAddress(const ELFIO::section &section, quint64 &physAddr);

    private:
        ELFIO::elfio *_elfReader{Q_NULLPTR};
};
