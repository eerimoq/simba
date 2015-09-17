/**
 * @file fat16.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2009, William Greiman (Arduino Fat16 Library)
 * Copyright (C) 2013-2015, Mikael Patel (Cosa, Refactoring)
 * Copyright (C) 2015, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

/* FAT16 end of chain value used by Microsoft. */
#define EOC16 0xffff

/* Minimum value for FAT16 EOC.  Use to test for EOC. */
#define EOC16_MIN 0xfff8

/* FAT32 end of chain value used by Microsoft. */
#define EOC32 0x0fffffff

/* Minimum value for FAT32 EOC.  Use to test for EOC. */
#define EOC32_MIN 0x0ffffff8

/* Mask a for FAT32 entry. Entries are 28 bits. */
#define ENTRY32_MASK 0x0fffffff

/* Type name for directoryEntry */

/* escape for name[0] = 0xe5 */
#define DIR_NAME_0XE5 0x05

/* name[0] value for entry that is free after being "deleted" */
#define DIR_NAME_DELETED 0xe5

/* name[0] value for entry that is free and no allocated entries follow */
#define DIR_NAME_FREE 0x00

/* file is read-only */
#define DIR_ATT_READ_ONLY 0x01

/* File should hidden in directory listings */
#define DIR_ATT_HIDDEN 0x02

/* Entry is for a system file */
#define DIR_ATT_SYSTEM 0x04

/* Directory entry contains the volume label */
#define DIR_ATT_VOLUME_ID 0x08

/* Entry is for a directory */
#define DIR_ATT_DIRECTORY 0x10

/* Old DOS archive bit for backup support */
#define DIR_ATT_ARCHIVE 0x20

/* Test value for long name entry */
#define DIR_ATT_LONG_NAME 0x0f

/* Test mask for long name entry */
#define DIR_ATT_LONG_NAME_MASK 0x3f

/* defined attribute bits */
#define DIR_ATT_DEFINED_BITS 0x3f

/* Mask for file/subdirectory tests */
#define DIR_ATT_FILE_TYPE_MASK (DIR_ATT_VOLUME_ID | DIR_ATT_DIRECTORY)
#define DIR_ATT_SKIP           (DIR_ATT_VOLUME_ID | DIR_ATT_DIRECTORY)

  /** Default date for file timestamps is 1 Jan 2000 */
#define DEFAULT_DATE (((2000 - 1980) << 9) | (1 << 5) | 1)

/** Default time for file timestamp is 1 am */
#define DEFAULT_TIME (1 << 11)

/* FAT entry */
typedef uint16_t fat_t;

/**
 * MBR partition table entry. A partition table entry for a MBR
 * formatted storage device. The MBR partition table has four entries.
 */
struct part_t {
    /**
     * Boot Indicator. Indicates whether the volume is the active
     * partition.  Legal values include: 0x00. Do not use for booting.
     * 0x80 Active partition.
     */
    uint8_t boot;
    /**
     * Head part of Cylinder-head-sector address of the first block in
     * the partition. Legal values are 0-255. Only used in old PC BIOS.
     */
    uint8_t beginHead;
    /**
     * Sector part of Cylinder-head-sector address of the first block in
     * the partition. Legal values are 1-63. Only used in old PC BIOS.
     */
    unsigned beginSector : 6;
    /**
     * High bits cylinder for first block in partition.
     */
    unsigned beginCylinderHigh : 2;
    /**
     * Combine beginCylinderLow with beginCylinderHigh. Legal values
     * are 0-1023.  Only used in old PC BIOS.
     */
    uint8_t beginCylinderLow;
    /**
     * Partition type. See defines that begin with PART_TYPE_ for
     * some Microsoft partition types.
     */
    uint8_t type;
    /**
     * head part of cylinder-head-sector address of the last sector in the
     * partition.  Legal values are 0-255. Only used in old PC BIOS.
     */
    uint8_t endHead;
    /**
     * Sector part of cylinder-head-sector address of the last sector in
     * the partition.  Legal values are 1-63. Only used in old PC BIOS.
     */
    unsigned endSector : 6;
    /**
     * High bits of end cylinder
     */
    unsigned endCylinderHigh : 2;
    /**
     * Combine endCylinderLow with endCylinderHigh. Legal values
     * are 0-1023.  Only used in old PC BIOS.
     */
    uint8_t endCylinderLow;
    /**
     * Logical block address of the first block in the partition.
     */
    uint32_t firstSector;
    /**
     * Length of the partition, in blocks.
     */
    uint32_t totalSectors;
};

/**
 * Master Boot Record. The first block of a storage device that is
 * formatted with a MBR.
 */
struct mbr_t {
    /**
     * Code Area for master boot program.
     */
    uint8_t codeArea[440];
    /**
     * Optional WindowsNT disk signature. May contain more boot code.
     */
    uint32_t diskSignature;
    /**
     * Usually zero but may be more boot code.
     */
    uint16_t usuallyZero;
    /**
     * Partition tables.
     */
    part_t part[4];
    /**
     * First MBR signature byte. Must be 0x55
     */
    uint8_t mbrSig0;
    /**
     * Second MBR signature byte. Must be 0xaa
     */
    uint8_t mbrSig1;
};

/**
 * BIOS parameter block; The BIOS parameter block describes the
 * physical layout of a FAT volume.
 */
struct bpb_t {
    /**
     * Count of bytes per sector. This value may take on only the
     * following values: 512, 1024, 2048 or 4096
     */
    uint16_t bytesPerSector;
    /**
     * Number of sectors per allocation unit. This value must be a
     * power of 2 that is greater than 0. The legal values are
     * 1, 2, 4, 8, 16, 32, 64, and 128.
     */
    uint8_t sectorsPerCluster;
    /**
     * Number of sectors before the first FAT.
     * This value must not be zero.
     */
    uint16_t reservedSectorCount;
    /** The count of FAT data structures on the volume. This field should
     *  always contain the value 2 for any FAT volume of any type.
     */
    uint8_t fatCount;
    /**
     * For FAT12 and FAT16 volumes, this field contains the count of
     * 32-byte directory entries in the root directory. For FAT32 volumes,
     * this field must be set to 0. For FAT12 and FAT16 volumes, this
     * value should always specify a count that when multiplied by 32
     * results in a multiple of bytesPerSector.  FAT16 volumes should
     * use the value 512.
     */
    uint16_t rootDirEntryCount;
    /**
     * This field is the old 16-bit total count of sectors on the volume.
     * This count includes the count of all sectors in all four regions
     * of the volume. This field can be 0; if it is 0, then totalSectors32
     * must be non-zero.  For FAT32 volumes, this field must be 0. For
     * FAT12 and FAT16 volumes, this field contains the sector count, and
     * totalSectors32 is 0 if the total sector count fits
     * (is less than 0x10000).
     */
    uint16_t totalSectors16;
    /**
     * This dates back to the old MS-DOS 1.x media determination and is
     * no longer usually used for anything.  0xf8 is the standard value
     * for fixed (non-removable) media. For removable media, 0xf0 is
     * frequently used. Legal values are 0xf0 or 0xf8-0xff.
     */
    uint8_t mediaType;
    /**
     * Count of sectors occupied by one FAT on FAT12/FAT16 volumes.
     * On FAT32 volumes this field must be 0, and sectorsPerFat32
     * contains the FAT size count.
     */
    uint16_t sectorsPerFat16;
    /**
     * Sectors per track for interrupt 0x13. Not used otherwise.
     */
    uint16_t sectorsPerTrtack;
    /**
     * Number of heads for interrupt 0x13.  Not used otherwise.
     */
    uint16_t headCount;
    /**
     * Count of hidden sectors preceding the partition that contains this
     * FAT volume. This field is generally only relevant for media
     *  visible on interrupt 0x13.
     */
    uint32_t hidddenSectors;
    /**
     * This field is the new 32-bit total count of sectors on the volume.
     * This count includes the count of all sectors in all four regions
     * of the volume.  This field can be 0; if it is 0, then
     * totalSectors16 must be non-zero.
     */
    uint32_t totalSectors32;
    /**
     * Count of sectors occupied by one FAT on FAT32 volumes.
     */
    uint32_t sectorsPerFat32;
    /**
     * This field is only defined for FAT32 media and does not exist on
     * FAT12 and FAT16 media.
     * Bits 0-3 -- Zero-based number of active FAT, Only valid if
     * mirroring is disabled.
     * Bits 4-6 -- Reserved.
     * Bit 7 -- 0 means the FAT is mirrored at runtime into all FATs,
     * 1 means only one FAT is active; it is the one referenced in bits 0-3.
     * Bits 8-15 -- Reserved.
     */
    uint16_t fat32Flags;
    /**
     * FAT32 version. High byte is major revision number.
     * Low byte is minor revision number. Only 0.0 define.
     */
    uint16_t fat32Version;
    /**
     * Cluster number of the first cluster of the root directory for FAT32.
     * This usually 2 but not required to be 2.
     */
    uint32_t fat32RootCluster;
    /**
     * Sector number of FSINFO structure in the reserved area of the
     * FAT32 volume. Usually 1.
     */
    uint16_t fat32FSInfo;
    /**
     * If non-zero, indicates the sector number in the reserved area
     * of the volume of a copy of the boot record. Usually 6.
     * No value other than 6 is recommended.
     */
    uint16_t fat32BackBootBlock;
    /**
     * Reserved for future expansion. Code that formats FAT32 volumes
     * should always set all of the bytes of this field to 0.
     */
    uint8_t fat32Reserved[12];
};

/**
 * Boot sector for a FAT16 or FAT32 volume.
 */
struct fbs_t {
    /**
     * X86 jmp to boot program
     */
    uint8_t jmpToBootCode[3];
    /**
     * Informational only - don't depend on it
     */
    char oemName[8];
    /**
     * BIOS Parameter Block
     */
    struct bpb_t bpb;
    /**
     * For int0x13 use value 0x80 for hard drive
     */
    uint8_t driveNumber;
    /**
     * Used by Windows NT - should be zero for FAT
     */
    uint8_t reserved1;
    /**
     * 0x29 if next three fields are valid
     */
    uint8_t bootSignature;
    /**
     * Usually generated by combining date and time
     */
    uint32_t volumeSerialNumber;
    /**
     * Should match volume label in root dir
     */
    char volumeLabel[11];
    /**
     * Informational only - don't depend on it
     */
    char fileSystemType[8];
    /**
     * X86 boot code
     */
    uint8_t bootCode[420];
    /**
     * Must be 0x55
     */
    uint8_t bootSectorSig0;
    /**
     * Must be 0xaa
     */
    uint8_t bootSectorSig1;
};

/**
 * FAT short directory entry. Short means short 8.3 name, not the
 * entry size.
 */
struct dir_t {
    /**
     * Short 8.3 name.
     * The first eight bytes contain the file name with blank fill.
     * The last three bytes contain the file extension with blank fill.
     */
    uint8_t name[11];
    /**
     * Entry attributes. The upper two bits of the attribute byte are
     * reserved and should always be set to 0 when a file is created
     * and never modified or looked at after that.  See defines that
     * begin with DIR_ATT_.
     */
    uint8_t attributes;
    /**
     * Reserved for use by Windows NT. Set value to 0 when a file is
     * created and never modify or look at it after that.
     */
    uint8_t reservedNT;
    /**
     * The granularity of the seconds part of creationTime is 2 seconds
     * so this field is a count of tenths of a second and its valid
     * value range is 0-199 inclusive. (WHG note - seems to be hundredths)
     */
    uint8_t creationTimeTenths;
    /**
     * Time file was created.
     */
    uint16_t creationTime;
    /**
     * Date file was created.
     */
    uint16_t creationDate;
    /**
     * Last access date. Note that there is no last access time, only
     * a date.  This is the date of last read or write. In the case of
     * a write, this should be set to the same date as lastWriteDate.
     */
    uint16_t lastAccessDate;
    /**
     * High word of this entry's first cluster number (always 0 for a
     * FAT12 or FAT16 volume).
     */
    uint16_t firstClusterHigh;
    /**
     * Time of last write. File creation is considered a write.
     */
    uint16_t lastWriteTime;
    /**
     * Date of last write. File creation is considered a write.
     */
    uint16_t lastWriteDate;
    /**
     * Low word of this entry's first cluster number.
     */
    uint16_t firstClusterLow;
    /**
     * 32-bit unsigned holding this file's size in bytes.
     */
    uint32_t fileSize;
};

union cache16_t {
    /** Used to access cached file data blocks. */
    uint8_t data[512];
    /** Used to access cached FAT entries. */
    fat_t fat[256];
    /** Used to access cached directory entries. */
    dir_t dir[16];
    /** Used to access a cached Master Boot Record. */
    mbr_t mbr;
    /** Used to access to a cached FAT16 boot sector. */
    fbs_t fbs;
};

/**
 * Directory entry is part of a long name
 */
static inline uint8_t dir_is_long_name(const dir_t* dir)
{
    return (dir->attributes & DIR_ATT_LONG_NAME_MASK) == DIR_ATT_LONG_NAME;
}

/**
 * Directory entry is for a file
 */
static inline uint8_t dir_is_file(const dir_t* dir)
{
    return (dir->attributes & DIR_ATT_FILE_TYPE_MASK) == 0;
}

/**
 * Directory entry is for a subdirectory
 */
static inline uint8_t dir_is_subdir(const dir_t* dir)
{
    return (dir->attributes & DIR_ATT_FILE_TYPE_MASK) == DIR_ATT_DIRECTORY;
}

/**
 * Directory entry is for a file or subdirectory
 */
static inline uint8_t dir_is_file_or_subdir(const dir_t* dir)
{
    return (dir->attributes & DIR_ATT_VOLUME_ID) == 0;
}

static int make_83_name(const char* str_p, uint8_t* name_p)
{
    uint8_t c;
    uint8_t n = 7;
    uint8_t i;
    uint8_t b;            
    FAR const char *invalid_p;

    memset(name_p, ' ', 11);

    i = 0;

    while ((c = *str_p++) != '\0') {
        if (c == '.') {
            if (n == 10) {
                return (0);
            }

            n = 10;
            i = 8;
        } else {
            /* Invalid characters in file name. */
            invalid_p = "|<>^+=?/[];,*\"\\";

            while ((b = *invalid_p++) != '\0') {
                if (b == c) {
                    return (1);
                }
            }

            /* Invalid characters in file name. */
            if (i > n || c < 0x21 || c > 0x7e) {
                return (1);
            }
            
            /* Add valid character to name. */
            name_p[i++] = ((c < 'a' || c > 'z')
                           ?  c
                           : c + ('A' - 'a'));
        }
    }

    return (name_p[0] == ' ');
}

/**
 * FAT date representation support
 * Date Format. A FAT directory entry date stamp is a 16-bit field
 * that is basically a date relative to the MS-DOS epoch of
 * 01/01/1980. Here is the format (bit 0 is the LSB of the 16-bit
 * word, bit 15 is the MSB of the 16-bit word):
 *
 * Bits 0-4: Day of month, valid value range 1-31 inclusive.
 * Bits 5-8: Month of year, 1 = January, valid value range 1-12
 * inclusive.
 * Bits 9-15: Count of years from 1980, valid value range 0-127
 * inclusive (1980-2107).
 */
union date_t {
    uint16_t as_uint16;
    struct {
        uint8_t day:5;
        uint8_t month:4;
        uint8_t year:7;
    } bits;
};

/**
 * FAT Time Format. A FAT directory entry time stamp is a 16-bit
 * field that has a granularity of 2 seconds. Here is the format
 * (bit 0 is the LSB of the 16-bit word, bit 15 is the MSB of the
 * 16-bit word).
 *
 * Bits 0-4: 2-second count, valid value range 0-29 inclusive
 * (0-58 seconds).
 * Bits 5-10: Minutes, valid value range 0-59 inclusive.
 * Bits 11-15: Hours, valid value range 0-23 inclusive.
 *
 * The valid time range is from Midnight 00:00:00 to 23:59:58.
 */
union time_t {
    uint16_t as_uint16;
    struct {
        uint8_t seconds:5;
        uint8_t minutes:6;
        uint8_t hours:5;
    } bits;
};

static inline uint8_t block_of_cluster(uint32_t position)
{
    return ((position >> 9) & (blocksPerCluster - 1));
}

static inline uint16_t cache_data_offset(uint32_t position)
{
    return (position & 0x1ff);
}

static inline void cache_set_dirty(void)
{
    cacheDirty |= CACHE_FOR_WRITE;
}

static inline uint32_t data_block_lba(fat_t cluster, uint8_t blockOfCluster)
{
    return (dataStartBlock +
	    (uint32_t)(cluster - 2) * blocksPerCluster +
	    blockOfCluster);
}

int fat16_init(struct fat16_t *fat16_p,
               struct sd_t* sd_p,
               unsigned int partition)
{
    uint32_t volumeStartBlock = 0;
    struct bpb_t* bpb_p;
    uint32_t totalBlocks;

    /* Error if invalid partition. */
    if (part > 4) {
        return (1);
    }

    fat16_p->device = sd;

    // If part == 0 assume super floppy with FAT16 boot sector in block zero
    // If part > 0 assume mbr volume with partition table
    if (partition > 0) {
        if (!cacheRawBlock(fat16_p->volumeStartBlock)) {
            return (1);
        }

        fat16_p->volumeStartBlock = cacheBuffer.mbr.part[part-1].firstSector;
    }

    if (!cacheRawBlock(volumeStartBlock)) {
        return (1);
    }

    // Check boot block signature
    if ((cacheBuffer.data[510] != BOOTSIG0)
        || (cacheBuffer.data[511] != BOOTSIG1)) {
        return (1);
    }

    bpb_p = &cacheBuffer.fbs.bpb;
    fat16_p->fatCount = bpb_p->fatCount;
    fat16_p->blocksPerCluster = bpb_p->sectorsPerCluster;
    fat16_p->blocksPerFat = bpb_p->sectorsPerFat16;
    fat16_p->rootDirEntryCount = bpb_p->rootDirEntryCount;
    fat16_p->fatStartBlock = fat16_p->volumeStartBlock + bpb_p->reservedSectorCount;
    fat16_p->rootDirStartBlock = (fat16_p->fatStartBlock
                                  + bpb_p->fatCount*bpb_p->sectorsPerFat16);
    fat16_p->dataStartBlock = (fat16_p->rootDirStartBlock
                               + ((32 * bpb_p->rootDirEntryCount + 511) / 512));
    fat16_p->totalBlocks = (bpb_p->totalSectors16
                            ? bpb_p->totalSectors16
                            : bpb_p->totalSectors32);
    fat16_p->clusterCount = ((totalBlocks - (dataStartBlock - volumeStartBlock))
                             / bpb_p->sectorsPerCluster);

    // Check valid FAT16 volume
    if ((bpb_p->bytesPerSector != 512)	 // Only allow 512 byte blocks
        || (bpb_p->sectorsPerFat16 == 0)     // Zero for FAT32
        || (clusterCount < 4085)           // FAT12 if true
        || (totalBlocks > 0x800000)	 // Max size for FAT16 volume
        || (bpb_p->reservedSectorCount == 0) // invalid volume
        || (bpb_p->fatCount == 0)		 // invalid volume
        || (bpb_p->sectorsPerFat16 < (clusterCount >> 8))
        || (bpb_p->sectorsPerCluster == 0)
        || (bpb_p->sectorsPerCluster & (bpb_p->sectorsPerCluster - 1))) {
        return (1);
    }

    return (0);
}

int fat16_start(struct fat16_t *fat16_p)
{
    return (0);
}

int fat16_stop(struct fat16_t *fat16_p)
{
    return (0);
}

void fat16_ls(struct fat16_t *fat16_p,
              chan_t *chan_p,
              uint8_t flags)
{
    return (0);    
}

int fat16_rm(struct fat16_t *fat16_p,
             const char* path_p)
{
    return (0);
}

int fat16_file_open(uint16_t index, uint8_t oflag)
{
    if (!volumeInitialized || is_open()) return (false);

    if ((oflag & O_TRUNC) && !(oflag & O_WRITE)) return (false);

    dir_t* d = cacheDirEntry(index);

    // If bad file index or I/O error
    if (!d) return (false);

    // Error if unused entry
    if (d->name[0] == DIR_NAME_FREE || d->name[0] == DIR_NAME_DELETED)
        return (false);

    // Error if long name, volume label or subdirectory
    if ((d->attributes & (DIR_ATT_VOLUME_ID | DIR_ATT_DIRECTORY)) != 0)
        return (false);

    // Don't allow write or truncate if read-only
    if ((d->attributes & DIR_ATT_READ_ONLY) && (oflag & (O_WRITE | O_TRUNC)))
        return (false);

    m_curCluster = 0;
    m_curPosition = 0;
    m_dirEntryIndex = index;
    m_fileSize = d->fileSize;
    m_firstCluster = d->firstClusterLow;
    m_flags = oflag & (O_RDWR | O_SYNC | O_APPEND);

    if (oflag & O_TRUNC) return (truncate(0));

    return (true);
}

inf fat16_file_open(struct fat16_file_t *file_p,
                    const char* path_p,
                    int mode)
{
    uint8_t dname[11];   // name formated for dir entry
    int16_t empty = -1;  // index of empty slot
    dir_t* p;            // pointer to cached dir entry
    uint16_t index;

    // Check valid 8.3 file name
    if (!make_83_name(fileName, dname)) {
        return (1);
    }

    for (index = 0; index < rootDirEntryCount; index++) {
        if (!(p = cacheDirEntry(index))) {
            return (1);
        }

        if (p->name[0] == DIR_NAME_FREE || p->name[0] == DIR_NAME_DELETED) {
            // Remember first empty slot
            if (empty < 0) {
                empty = index;
            }

            // Done if no entries follow
            if (p->name[0] == DIR_NAME_FREE) {
                break;
            }
        } else if (!memcmp(dname, p->name, 11)) {
            // Don't open existing file if O_CREAT and O_EXCL
            if ((oflag & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL)) {
                return (1);
            }

            // Open existing file
            return (open(index, oflag));
        }
    }

    // Error if directory is full
    if (empty < 0) {
        return (1);
    }

    // Only create file if O_CREAT and O_WRITE
    if ((oflag & (O_CREAT | O_WRITE)) != (O_CREAT | O_WRITE)) {
        return (false);
    }

    if (!(p = cacheDirEntry(empty, CACHE_FOR_WRITE))) {
        return (false);
    }

    // Initialize as empty file
    memset(p, 0, sizeof(dir_t));
    memcpy(p->name, dname, 11);

    // Set timestamps with user function or use default date/time
    if (dateTime) {
        dateTime(&p->creationDate, &p->creationTime);
    } else {
        p->creationDate = DEFAULT_DATE;
        p->creationTime = DEFAULT_TIME;
    }

    p->lastAccessDate = p->creationDate;
    p->lastWriteDate = p->creationDate;
    p->lastWriteTime = p->creationTime;

    // Force created directory entry will be written to storage device
    if (!cacheFlush()) {
        return (false);
    }

    // Open entry
    return (open(empty, oflag));
}

int fat16_file_close(struct fat16_file_t *file_p)
{
    return (0);
}

ssize_t fat16_file_read(struct fat16_file_t *file_p,
                        void* buf_p,
                        size_t size)
{
    uint16_t nToRead;
    uint8_t blkOfCluster;
    uint16_t blockOffset;
    uint8_t* src_p;
    uint16_t n;
    
    // Error if not open for read
    if (!(m_flags & O_READ)) {
        return (EOF);
    }

    // Don't read beyond end of file
    if ((m_curPosition + nbyte) > m_fileSize) {
        nbyte = m_fileSize - m_curPosition;
    }

    // Bytes left to read in loop
    nToRead = nbyte;

    while (nToRead > 0) {
        blkOfCluster = blockOfCluster(m_curPosition);
        blockOffset = cacheDataOffset(m_curPosition);

        if (blkOfCluster == 0 && blockOffset == 0) {
            // Start next cluster
            if (m_curCluster == 0) {
                m_curCluster = m_firstCluster;
            } else {
                if (!fatGet(m_curCluster, &m_curCluster)) {
                    return (EOF);
                }
            }

            // Return error if bad cluster chain
            if (m_curCluster < 2 || isEOC(m_curCluster)) {
                return (EOF);
            }
        }

        // Cache data block
        if (!cacheRawBlock(dataBlockLba(m_curCluster, blkOfCluster))) {
            return (EOF);
        }

        // Location of data in cache
        src_p = cacheBuffer.data + blockOffset;

        // Max number of byte available in block
        n = 512 - blockOffset;

        // Lesser of available and amount to read
        if (n > nToRead) {
            n = nToRead;
        }

        // Copy data to caller
        memcpy(dst, src, n);

        m_curPosition += n;
        dst += n;
        nToRead -= n;
    }

    return (nbyte);
}

ssize_t fat16_file_write(struct fat16_file_t *file_p,
                         const void *buf_p,
                         size_t size)
{
    uint16_t nToWrite = nbyte;
    uint8_t blkOfCluster;
    uint16_t blockOffset;
    fat_t next;
    uint32_t lba;
    uint8_t* dst_p;
    uint16_t n;

    // Error if file is not open for write
    if (!(m_flags & O_WRITE)) {
        return (EOF);
    }

    // Go to end of file if O_APPEND
    if ((m_flags & O_APPEND) && m_curPosition != m_fileSize) {
        if (!seek(0, SEEK_END)) {
            return (EOF);
        }
    }

    while (nToWrite > 0) {
        blkOfCluster = blockOfCluster(m_curPosition);
        blockOffset = cacheDataOffset(m_curPosition);

        if (blkOfCluster == 0 && blockOffset == 0) {
            // Start of new cluster
            if (m_curCluster == 0) {
                if (m_firstCluster == 0) {
                    // Allocate first cluster of file
                    if (!addCluster()) {
                        return (EOF);
                    }
                } else {
                    m_curCluster = m_firstCluster;
                }
            } else {
                if (!fatGet(m_curCluster, &next)) {
                    return (EOF);
                }
                
                if (isEOC(next)) {
                    // Add cluster if at end of chain
                    if (!addCluster()) {
                        return (EOF);
                    }
                } else {
                    m_curCluster = next;
                }
            }
        }

        lba = dataBlockLba(m_curCluster, blkOfCluster);

        if (blockOffset == 0 && m_curPosition >= m_fileSize) {
            // Start of new block don't need to read into cache
            if (!cacheFlush()) {
                return (EOF);
            }

            cacheBlockNumber = lba;
            cacheSetDirty();
        } else {
            // Rewrite part of block
            if (!cacheRawBlock(lba, CACHE_FOR_WRITE)) {
                return (EOF);
            }
        }

        dst_p = cacheBuffer.data + blockOffset;

        // Max space in block
        n = 512 - blockOffset;

        // Lesser of space and amount to write
        if (n > nToWrite) {
            n = nToWrite;
        }

        // Copy data to cache
        memcpy(dst, src, n);

        m_curPosition += n;
        nToWrite -= n;
        src += n;
    }

    if (m_curPosition > m_fileSize) {
        // Update fileSize and insure sync will update dir entry
        m_fileSize = m_curPosition;
        m_flags |= F_FILE_DIR_DIRTY;
    } else if (dateTime && nbyte) {
        // Force sync will update modified date and time
        m_flags |= F_FILE_DIR_DIRTY;
    }

    if (m_flags & O_SYNC) {
        if (!sync()) {
            return (EOF);
        }
    }
 
    return (nbyte);
}

int fat16_file_seek(struct fat16_file_t *file_p,
                    size_t pos,
                    int whence)
{
    fat_t n;

    if (whence == SEEK_CUR) {
        pos += m_curPosition;
    } else if (whence == SEEK_END) {
        pos = m_fileSize;
    } else if (whence != SEEK_SET) {
        return (1);
    }

    // Error if file not open or seek past end of file
    if (pos > m_fileSize) {
        return (1);
    }
    
    if (pos == 0) {
        // Set position to start of file
        m_curCluster = 0;
        m_curPosition = 0;

        return (0);
    }

    fat_t n = ((pos - 1) >> 9) / blocksPerCluster;
    
    if (pos < m_curPosition || m_curPosition == 0) {
        // Must follow chain from first cluster
        m_curCluster = m_firstCluster;
    } else {
        // Advance from curPosition
        n -= ((m_curPosition - 1) >> 9)/blocksPerCluster;
    }

    while (n--) {
        if (!fatGet(m_curCluster, &m_curCluster)) {
            return (1);
        }
    }
    
    m_curPosition = pos;
    
    return (0);
}

ssize_t fat16_file_tell(struct fat16_file_t *file_p)
{
    return (0);
}

ssize_t fat16_file_size(struct fat16_file_t *file_p)
{
    return (0);
}

int fat16_file_truncate(struct fat16_file_t *file_p,
                        size_t length)
{
    uint32_t newPos;
    fat_t toFree;

    // Error if file is not open for write
    if (!(m_flags & O_WRITE)) {
        return (1);
    }

    if (length > m_fileSize) {
        return (1);
    }

    // Filesize and length are zero - nothing to do
    if (m_fileSize == 0) {
        return (0);
    }

    newPos = m_curPosition > length ? length : m_curPosition;

    if (length == 0) {
        // Free all clusters
        if (!freeChain(m_firstCluster)) {
            return (1);
        }

        m_curCluster = m_firstCluster = 0;
    } else {
        if (!seek(length)) {
            return (1);
        }

        if (!fatGet(m_curCluster, &toFree)) {
            return (1);
        }

        if (!isEOC(toFree)) {
            // Free extra clusters
            if (!fatPut(m_curCluster, EOC16)) {
                return (1);
            }

            if (!freeChain(toFree)) {
                return (1);
            }
        }
    }

    m_fileSize = length;
    m_flags |= F_FILE_DIR_DIRTY;

    if (!sync()) {
        return (1);
    }

    return seek(newPos);
}

int fat16_file_sync(struct fat16_file_t *file_p)
{
    if (m_flags & F_FILE_DIR_DIRTY) {
        // Cache directory entry
        dir_t* d = cacheDirEntry(m_dirEntryIndex, CACHE_FOR_WRITE);
        if (!d) return (false);
        
        // Update file size and first cluster
        d->fileSize = m_fileSize;
        d->firstClusterLow = m_firstCluster;
        
        // Set modify time if user supplied a callback date/time function
        if (dateTime) {
            dateTime(&d->lastWriteDate, &d->lastWriteTime);
            d->lastAccessDate = d->lastWriteDate;
        }
        
        m_flags &= ~F_FILE_DIR_DIRTY;
    }

    return (cacheFlush());
}

int fat16_file_remove(struct fat16_file_t *file_p)
{
    struct dir_t* dir_p;

    // Error if file is not open for write
    if (!(file_p->m_flags & O_WRITE)) {
        return (1);
    }

    if (m_firstCluster && !freeChain(m_firstCluster)) {
        return (1);
    }
    
    dir_p = cacheDirEntry(m_dirEntryIndex, CACHE_FOR_WRITE);

    if (dir_p == NULL) {
        return (1);
    }
    
    dir_p->name[0] = DIR_NAME_DELETED;
    file_p->m_flags = 0;
    
    return (cacheFlush());
}

int fat16_file_direntry(struct dir_t* dir_p)
{
    if (!sync()) {
        return (1);
    }
    
    dir_t* p = cacheDirEntry(m_dirEntryIndex, CACHE_FOR_WRITE);

    if (!p) {
        return (1);
    }

    memcpy(dir, p, sizeof(dir_t));

    return (0);
}

int fat16_read(dir_t* dir, uint16_t* index, uint8_t skip)
{
  dir_t* p;
  for (uint16_t i = *index; ; i++) {
    if (i >= rootDirEntryCount) return (false);
    if (!(p = cacheDirEntry(i))) return (false);

    // Done if beyond last used entry
    if (p->name[0] == DIR_NAME_FREE) return (false);

    // Skip deleted entry
    if (p->name[0] == DIR_NAME_DELETED) continue;

    // Skip long names
    if ((p->attributes & DIR_ATT_LONG_NAME_MASK) == DIR_ATT_LONG_NAME) continue;

    // Skip if attribute match
    if (p->attributes & skip) continue;

    // Return found index
    *index = i;
    break;
  }
  memcpy(dir, p, sizeof(dir_t));
  return (true);
}

int fat16_ls(chan_t *chan_p, uint8_t flags)
{
  dir_t d;

  for (uint16_t index = 0; read(&d, &index, DIR_ATT_VOLUME_ID); index++) {
    // Print file name with possible blank fill
    printDirName(outs, d, flags & (LS_DATE | LS_SIZE) ? 14 : 0);

    // Print modify date/time if requested
    if (flags & LS_DATE) {
      date_t date(d.lastWriteDate);
      time_t time(d.lastWriteTime);
      outs << date << ' ' << time;
    }

    // Print size if requested
    if (DIR_IS_FILE(&d) && (flags & LS_SIZE)) {
      outs << ' ' << d.fileSize;
    }
    outs << endl;
  }
}

int fat16_file_add_cluster()
{
  // Start search after last cluster of file or at cluster two in FAT
  fat_t freeCluster = m_curCluster ? m_curCluster : 1;

  for (fat_t i = 0; ; i++) {
    // Return no free clusters
    if (i >= clusterCount) return (false);
    // Fat has clusterCount + 2 entries
    if (freeCluster > clusterCount) freeCluster = 1;
    freeCluster++;
    fat_t value;
    if (!fatGet(freeCluster, &value)) return (false);
    if (value == 0) break;
  }

  // Mark cluster allocated
  if (!fatPut(freeCluster, EOC16)) return (false);

  if (m_curCluster != 0) {
    // Link cluster to chain
    if (!fatPut(m_curCluster, freeCluster)) return (false);
  } else {
    // first cluster of file so update directory entry
    m_flags |= F_FILE_DIR_DIRTY;
    m_firstCluster = freeCluster;
  }
  m_curCluster = freeCluster;
  return (true);
}

struct dir_t* fat16_cache_dir_entry(uint16_t index, uint8_t action)
{
    if (index >= rootDirEntryCount) {
        return NULL;
    }

    if (!cacheRawBlock(rootDirStartBlock + (index >> 4), action)) {
        return NULL;
    }

    return (&cacheBuffer.dir[index & 0xf]);
}

uint8_t fat16_cache_flush(void)
{
  if (cacheDirty) {
    if (!device->write(cacheBlockNumber, cacheBuffer.data)) {
      return (false);
    }
    if (cacheMirrorBlock) {
      if (!device->write(cacheMirrorBlock, cacheBuffer.data)) {
        return (false);
      }
      cacheMirrorBlock = 0;
    }
    cacheDirty = 0;
  }
  return (true);
}

uint8_t
FAT16::cacheRawBlock(uint32_t blockNumber, uint8_t action)
{
  if (cacheBlockNumber != blockNumber) {
    if (!cacheFlush()) return (false);
    if (!device->read(blockNumber, cacheBuffer.data)) return (false);
    cacheBlockNumber = blockNumber;
  }
  cacheDirty |= action;
  return (true);
}

bool
FAT16::fatGet(fat_t cluster, fat_t* value)
{
  if (cluster > (clusterCount + 1)) return (false);
  uint32_t lba = fatStartBlock + (cluster >> 8);
  if (lba != cacheBlockNumber) {
    if (!cacheRawBlock(lba)) return (false);
  }
  *value = cacheBuffer.fat[cluster & 0xff];
  return (true);
}

bool
FAT16::fatPut(fat_t cluster, fat_t value)
{
  if (cluster < 2) return (false);
  if (cluster > (clusterCount + 1)) return (false);
  uint32_t lba = fatStartBlock + (cluster >> 8);
  if (lba != cacheBlockNumber) {
    if (!cacheRawBlock(lba)) return (false);
  }
  cacheBuffer.fat[cluster & 0xff] = value;
  cacheSetDirty();
  if (fatCount > 1) cacheMirrorBlock = lba + blocksPerFat;
  return (true);
}

bool
FAT16::File::freeChain(fat_t cluster)
{
  while (1) {
    fat_t next;
    if (!fatGet(cluster, &next)) return (false);
    if (!fatPut(cluster, 0)) return (false);
    if (isEOC(next)) return (true);
    cluster = next;
  }
}
