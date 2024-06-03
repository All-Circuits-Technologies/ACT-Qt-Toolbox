// SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

/* ------------------------------------------------------------------
 * elffilereader.cpp - COPYRIGHT (C) 2019 ALL CIRCUITS
 * ------------------------------------------------------------------
 */

#include "elffilereader.hpp"

#include <QDebug>
#include <QFileInfo>

#include "elfconstants.hpp"
#include "elfio/elfio.hpp"

using namespace ELFIO;


ElfFileReader::ElfFileReader(QObject *parent) :
    QObject(parent)
{
}

bool ElfFileReader::loadElfFile(const QString &elfFilePath)
{
    if(_elfReader != Q_NULLPTR)
    {
        qWarning() << "A file is already loaded";
        return false;
    }

    QFileInfo elfFile(elfFilePath);

    if(!elfFile.exists())
    {
        qWarning() << "The file given doesn't exist: " << elfFilePath;
        return false;
    }

    if(!ElfConstants::elfFileSuffixes.contains(elfFile.suffix()))
    {
        qWarning() << "The file suffix: " << elfFile.suffix() << " is not known";
        return false;
    }

    _elfReader = new ELFIO::elfio();

    if(!_elfReader->load(elfFilePath.toStdString()))
    {
        qWarning() << "A problem occured when loading ELF file: " << elfFilePath;
        delete _elfReader;
        _elfReader = Q_NULLPTR;
        return false;
    }

    return true;
}

bool ElfFileReader::getSectionPhysicalAddress(const section &section, quint64 &physAddr)
{
    std::vector<segment*>::const_iterator iter = _elfReader->segments.begin();
    ELFIO::segment *segmentFound = Q_NULLPTR;

    while(iter != _elfReader->segments.end() && segmentFound == Q_NULLPTR)
    {
        ELFIO::segment *segment = (*iter);

        for(Elf_Half idx = 0; idx < segment->get_sections_num(); idx++)
        {
            if(section.get_index() == segment->get_section_index_at(idx))
            {
                segmentFound = segment;
                break;
            }
        }

        iter++;
    }

    if(segmentFound == Q_NULLPTR)
    {
        // No segment has been found for the section
        return false;
    }

    Elf64_Off physOffset = section.get_offset() - segmentFound->get_offset();

    physAddr = segmentFound->get_physical_address() + physOffset;

    return true;
}
