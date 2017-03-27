/**
 * @section License
 *
 * Copyright (C) 2009, William Greiman (Arduino Fat16 Library)
 * Copyright (C) 2013-2015, Mikael Patel (Cosa, Refactoring)
 * Copyright (C) 2015-2017, Erik Moqvist
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

#ifndef __FILESYSTEMS_FAT16_H__
#define __FILESYSTEMS_FAT16_H__

#include "simba.h"

/*
 * FAT16 file structures.
 *
 * @section Acknowledgement
 * Refactoring of Arduino Fat16 Library, Copyright (C) 2009 by William Greiman
 *
 * @section References
 * 1. http://www.microsoft.com/whdc/system/platform/firmware/fatgen.mspx
 * 2. http://code.google.com/p/fat16lib/
 * 3. https://github.com/mikaelpatel/Cosa/tree/master/libraries/FAT16
 */

/**
 * The offset is relative to the start of the file.
 */
#define FAT16_SEEK_SET  0

/**
 * The offset is relative to the current position indicator.
 */
#define FAT16_SEEK_CUR  1

/**
 * The offset is relative to the end of the file.
 */
#define FAT16_SEEK_END  2

/**
 * End of file indicator.
 */
#define FAT16_EOF      -1

/**
 * Open for reading.
 */
#define O_READ             0x01

/**
 * Same as O_READ.
 */
#define O_RDONLY           O_READ

/**
 * Open for write.
 */
#define O_WRITE            0x02

/**
 * Same as O_WRITE.
 */
#define O_WRONLY           O_WRITE

/**
 * Open for reading and writing.
 */
#define O_RDWR             (O_READ | O_WRITE)

/**
 * The file position indicator shall be set to the end of the file
 * prior to each write.
 */
#define O_APPEND           0x04

/**
 * Synchronous writes.
 */
#define O_SYNC             0x08

/**
 * Create the file if non-existent.
 */
#define O_CREAT            0x10

/**
 * If O_CREAT and O_EXCL are set, file open shall fail if the file
 * exists.
 */
#define O_EXCL             0x20

/**
 * Truncate the file to zero length.
 */
#define O_TRUNC            0x40

/**
 * File is read-only.
 */
#define DIR_ATTR_READ_ONLY 0x01

/**
 * File should hidden in directory listings.
 */
#define DIR_ATTR_HIDDEN    0x02

/**
 * Entry is for a system file.
 */
#define DIR_ATTR_SYSTEM    0x04

/**
 * Directory entry contains the volume label.
 */
#define DIR_ATTR_VOLUME_ID 0x08

/**
 * Entry is for a directory.
 */
#define DIR_ATTR_DIRECTORY 0x10

/**
 * Old DOS archive bit for backup support.
 */
#define DIR_ATTR_ARCHIVE   0x20

/**
 * Block read function callback.
 */
typedef ssize_t (*fat16_read_t)(void *arg_p,
                                void *dst_p,
                                uint32_t src_block);

/**
 * Block write function callback.
 */
typedef ssize_t (*fat16_write_t)(void *arg_p,
                                 uint32_t dst_block,
                                 const void *src_p);

/**
 * A FAT entry.
 */
typedef uint16_t fat_t;

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
union fat16_time_t {
    uint16_t as_uint16;
    struct {
        uint16_t seconds : 5;
        uint16_t minutes : 6;
        uint16_t hours : 5;
    } bits;
};

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
union fat16_date_t {
    uint16_t as_uint16;
    struct {
        uint16_t day : 5;
        uint16_t month : 4;
        uint16_t year : 7;
    } bits;
};

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
    uint8_t begin_head;
    /**
     * Sector part of Cylinder-head-sector address of the first block in
     * the partition. Legal values are 1-63. Only used in old PC BIOS.
     */
    unsigned begin_sector : 6;
    /**
     * High bits cylinder for first block in partition.
     */
    unsigned begin_cylinder_high : 2;
    /**
     * Combine beginCylinderLow with beginCylinderHigh. Legal values
     * are 0-1023.  Only used in old PC BIOS.
     */
    uint8_t begin_cylinder_low;
    /**
     * Partition type. See defines that begin with PART_TYPE_ for
     * some Microsoft partition types.
     */
    uint8_t type;
    /**
     * head part of cylinder-head-sector address of the last sector in the
     * partition.  Legal values are 0-255. Only used in old PC BIOS.
     */
    uint8_t end_head;
    /**
     * Sector part of cylinder-head-sector address of the last sector in
     * the partition.  Legal values are 1-63. Only used in old PC BIOS.
     */
    unsigned end_sector : 6;
    /**
     * High bits of end cylinder
     */
    unsigned end_cylinder_high : 2;
    /**
     * Combine endCylinderLow with endCylinderHigh. Legal values
     * are 0-1023.  Only used in old PC BIOS.
     */
    uint8_t end_cylinder_low;
    /**
     * Logical block address of the first block in the partition.
     */
    uint32_t first_sector;
    /**
     * Length of the partition, in blocks.
     */
    uint32_t total_sectors;
} PACKED;

/**
 * BIOS parameter block; The BIOS parameter block describes the
 * physical layout of a FAT volume.
 */
struct bpb_t {
    /**
     * Count of bytes per sector. This value may take on only the
     * following values: 512, 1024, 2048 or 4096
     */
    uint16_t bytes_per_sector;
    /**
     * Number of sectors per allocation unit. This value must be a
     * power of 2 that is greater than 0. The legal values are
     * 1, 2, 4, 8, 16, 32, 64, and 128.
     */
    uint8_t sectors_per_cluster;
    /**
     * Number of sectors before the first FAT. This value must not be
     * zero.
     */
    uint16_t reserved_sector_count;
    /**
     * The count of FAT data structures on the volume. This field
     * should always contain the value 2 for any FAT volume of any
     * type.
     */
    uint8_t fat_count;
    /**
     * For FAT12 and FAT16 volumes, this field contains the count of
     * 32-byte directory entries in the root directory. For FAT32 volumes,
     * this field must be set to 0. For FAT12 and FAT16 volumes, this
     * value should always specify a count that when multiplied by 32
     * results in a multiple of bytesPerSector.  FAT16 volumes should
     * use the value 512.
     */
    uint16_t root_dir_entry_count;
    /**
     * This field is the old 16-bit total count of sectors on the volume.
     * This count includes the count of all sectors in all four regions
     * of the volume. This field can be 0; if it is 0, then totalSectors32
     * must be non-zero.  For FAT32 volumes, this field must be 0. For
     * FAT12 and FAT16 volumes, this field contains the sector count, and
     * totalSectors32 is 0 if the total sector count fits
     * (is less than 0x10000).
     */
    uint16_t total_sectors_small;
    /**
     * This dates back to the old MS-DOS 1.x media determination and is
     * no longer usually used for anything.  0xf8 is the standard value
     * for fixed (non-removable) media. For removable media, 0xf0 is
     * frequently used. Legal values are 0xf0 or 0xf8-0xff.
     */
    uint8_t media_type;
    /**
     * Count of sectors occupied by one FAT on FAT12/FAT16 volumes.
     * On FAT32 volumes this field must be 0, and sectorsPerFat32
     * contains the FAT size count.
     */
    uint16_t sectors_per_fat;
    /**
     * Sectors per track for interrupt 0x13. Not used otherwise.
     */
    uint16_t sectors_per_track;
    /**
     * Number of heads for interrupt 0x13.  Not used otherwise.
     */
    uint16_t head_count;
    /**
     * Count of hidden sectors preceding the partition that contains this
     * FAT volume. This field is generally only relevant for media
     *  visible on interrupt 0x13.
     */
    uint32_t hiddden_sectors;
    /**
     * This field is the new 32-bit total count of sectors on the volume.
     * This count includes the count of all sectors in all four regions
     * of the volume.  This field can be 0; if it is 0, then
     * totalSectors16 must be non-zero.
     */
    uint32_t total_sectors_large;
} PACKED;

/**
 * Boot sector for a FAT16 or FAT32 volume.
 */
struct fbs_t {
    /**
     * X86 jmp to boot program
     */
    uint8_t jmp_to_boot_code[3];
    /**
     * Informational only - don't depend on it
     */
    char oem_name[8];
    /**
     * BIOS Parameter Block
     */
    struct bpb_t bpb;
    /**
     * For int0x13 use value 0x80 for hard drive
     */
    uint8_t drive_number;
    /**
     * Used by Windows NT - should be zero for FAT
     */
    uint8_t reserved1;
    /**
     * 0x29 if next three fields are valid
     */
    uint8_t boot_signature;
    /**
     * Usually generated by combining date and time
     */
    uint32_t volume_serial_number;
    /**
     * Should match volume label in root dir
     */
    char volume_label[11];
    /**
     * Informational only - don't depend on it
     */
    char file_system_type[8];
    /**
     * X86 boot code
     */
    uint8_t boot_code[448];
    /**
     * Must be 0x55AA
     */
    uint16_t boot_sector_sig;
} PACKED;

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
    struct part_t part[4];
    /**
     * First MBR signature byte. Must be 0x55
     */
    uint16_t mbr_sig;
} PACKED;

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
    uint8_t reserved1;
    /**
     * The granularity of the seconds part of creationTime is 2 seconds
     * so this field is a count of tenths of a second and its valid
     * value range is 0-199 inclusive. (WHG note - seems to be hundredths)
     */
    uint8_t creation_time_tenths;
    /**
     * Time file was created.
     */
    uint16_t creation_time;
    /**
     * Date file was created.
     */
    uint16_t creation_date;
    /**
     * Last access date. Note that there is no last access time, only
     * a date.  This is the date of last read or write. In the case of
     * a write, this should be set to the same date as lastWriteDate.
     */
    uint16_t last_access_date;
    /**
     * High word of this entry's first cluster number (always 0 for a
     * FAT12 or FAT16 volume).
     */
    uint16_t first_cluster_high;
    /**
     * Time of last write. File creation is considered a write.
     */
    uint16_t last_write_time;
    /**
     * Date of last write. File creation is considered a write.
     */
    uint16_t last_write_date;
    /**
     * Low word of this entry's first cluster number.
     */
    uint16_t first_cluster_low;
    /**
     * 32-bit unsigned holding this file's size in bytes.
     */
    uint32_t file_size;
} PACKED;

union fat16_cache16_t {
    /* Used to access cached file data blocks. */
    uint8_t data[512];
    /* Used to access cached FAT entries. */
    fat_t fat[256];
    /* Used to access cached directory entries. */
    struct dir_t dir[16];
    /* Used to access a cached Master Boot Record. */
    struct mbr_t mbr;
    /* Used to access to a cached FAT16 boot sector. */
    struct fbs_t fbs;
};

struct fat16_cache_t {
    uint32_t block_number;         /* Logical number of block in the cache */
    uint8_t dirty;                 /* cacheFlush() will write block if true */
    uint32_t mirror_block;         /* mirror block for second FAT */
    union fat16_cache16_t buffer;  /* 512 byte cache for raw blocks */
};

struct fat16_t {
    /* Data block read and wrte functions. */
    fat16_read_t read;
    fat16_write_t write;
    void *arg_p;
    unsigned int partition;

    /* Volume info */
    uint8_t fat_count;             /* number of FATs */
    uint8_t blocks_per_cluster;    /* must be power of 2 */
    uint16_t root_dir_entry_count; /* should be 512 for FAT16 */
    fat_t blocks_per_fat;          /* number of blocks in one FAT */
    fat_t cluster_count;           /* total clusters in volume */
    uint32_t volume_start_block;   /* start of volume */
    uint32_t fat_start_block;      /* start of first FAT */
    uint32_t root_dir_start_block; /* start of root dir */
    uint32_t data_start_block;     /* start of data clusters */

    /* block cache */
    struct fat16_cache_t cache;
};

struct fat16_file_t {
    struct fat16_t *fat16_p; /* file system that contains this file */
    uint8_t flags;           /* see above for bit definitions */
    int16_t dir_entry_block; /* block of directory entry for open file */
    int16_t dir_entry_index; /* index of directory entry for open file */
    fat_t first_cluster;     /* first cluster of file */
    size_t file_size;        /* fileSize */
    fat_t cur_cluster;       /* current cluster */
    size_t cur_position;     /* current byte offset */
};

struct fat16_dir_t {
    int16_t root_index;
    struct fat16_file_t file; /* use the file datastructure to
                                 reresent a directory */
};

struct fat16_dir_entry_t {
    char name[256];
    int is_dir;
    size_t size;
    struct date_t latest_mod_date;
};

struct fat16_stat_t {
    size_t size;
    int is_dir;
};

/**
 * Initialize a FAT16 volume.
 *
 * @param[in,out] self_p FAT16 object to initialize.
 * @param[in] read Callback function used to read blocks of data.
 * @param[in] write Callback function used to write blocks of data.
 * @param[in] arg_p Argument passed as the first arguemtn to read()
 *                  and write().
 * @param[in] partition Partition to be used. Legal values for a
 *                      partition are 1-4 to use the corresponding
 *                      partition on a device formatted with a MBR,
 *                      Master Boot Record, or zero if the device is
 *                      formatted as a super floppy with the FAT boot
 *                      sector in block zero.
 *
 * @return zero(0) or negative error code.
 */
int fat16_init(struct fat16_t *self_p,
               fat16_read_t read,
               fat16_write_t write,
               void *arg_p,
               unsigned int partition);

/**
 * Mount given FAT16 volume.
 *
 * @param[in] self_p FAT16 object.
 *
 * @return zero(0) or negative error code.
 */
int fat16_mount(struct fat16_t *self_p);

/**
 * Unmount given FAT16 volume.
 *
 * @param[in] self_p FAT16 object.
 *
 * @return zero(0) or negative error code.
 */
int fat16_unmount(struct fat16_t *self_p);

/**
 * Create an empty FAT16 file system on the device.
 *
 * @param[in] self_p FAT16 object.
 */
int fat16_format(struct fat16_t *self_p);

/**
 * Print volume information to given channel.
 *
 * @param[in] self_p FAT16 object.
 * @param[in] chan_p Output channel.
 *
 * @return zero(0) or negative error code.
 */
int fat16_print(struct fat16_t *self_p, void *chan_p);

/**
 * Open a file by file path and mode flags.
 *
 * @param[in] self_p FAT16 object.
 * @param[in,out] file_p File object to be initialized.
 * @param[in] path_p A valid 8.3 DOS name for a file path.
 * @param[in] oflag mode of file open (create, read, write, etc).
 *
 * @return zero(0) or negative error code.
 */
int fat16_file_open(struct fat16_t *self_p,
                    struct fat16_file_t *file_p,
                    const char *path_p,
                    int oflag);

/**
 * Close a file and force cached data and directory information to be
 * written to the media.
 *
 * @param[in] file_p File object.
 *
 * @return zero(0) or negative error code.
 */
int fat16_file_close(struct fat16_file_t *file_p);

/**
 * Read data to given buffer with given size from the file.
 *
 * @param[in] file_p File object.
 * @param[in] buf_p Buffer to read into.
 * @param[in] size number of bytes to read.
 *
 * @return Number of bytes read or EOF(-1).
 */
ssize_t fat16_file_read(struct fat16_file_t *file_p,
                        void *buf_p,
                        size_t size);

/**
 * Write data from buffer with given size to the file.
 *
 * @param[in] file_p File object.
 * @param[in] buf_p Buffer to write.
 * @param[in] size number of bytes to write.
 *
 * @return Number of bytes written or EOF(-1).
 */
ssize_t fat16_file_write(struct fat16_file_t *file_p,
                         const void *buf_p,
                         size_t size);

/**
 * Sets the file's read/write position relative to mode.
 *
 * @param[in] file_p File object.
 * @param[in] pos New position in bytes from given mode.
 * @param[in] whence Absolute, relative or from end.
 *
 * @return zero(0) or negative error code.
 */
int fat16_file_seek(struct fat16_file_t *file_p,
                    int pos,
                    int whence);

/**
 * Return current position in the file.
 *
 * @param[in] file_p File object.
 *
 * @return Current position or negative error code.
 */
ssize_t fat16_file_tell(struct fat16_file_t *file_p);

/**
 * Truncate given file to a size of precisly `size` bytes.
 *
 * If the file previously was larger than this size, the extra data is
 * lost. If the file previously was shorter, it is extended, and the
 * extended part reads as null bytes ('\0').
 *
 * @param[in] file_p File object.
 * @param[in] size New size of the file in bytes.
 *
 * @return zero(0) or negative error code.
 */
int fat16_file_truncate(struct fat16_file_t *file_p,
                        size_t size);

/**
 * Return number of bytes in the file.
 *
 * @param[in] file_p File object.
 *
 * @return File size in bytes or negative error code.
 */
ssize_t fat16_file_size(struct fat16_file_t *file_p);

/**
 * Causes all modified data and directory fields to be written to the
 * storage device.
 *
 * @param[in] file_p File object.
 *
 * @return zero(0) or negative error code.
 */
int fat16_file_sync(struct fat16_file_t *file_p);

/**
 * Open a directory by directory path and mode flags.
 *
 * @param[in] self_p FAT16 object.
 * @param[in,out] dir_p Directory object to be initialized.
 * @param[in] path_p A valid 8.3 DOS name for a directory path.
 * @param[in] oflag mode of the directory to open (create, read, etc).
 *
 * @return zero(0) or negative error code.
 */
int fat16_dir_open(struct fat16_t *self_p,
                   struct fat16_dir_t *dir_p,
                   const char *path_p,
                   int oflag);

/**
 * Close given directory.
 *
 * @param[in] dir_p Directory object.
 *
 * @return zero(0) or negative error code.
 */
int fat16_dir_close(struct fat16_dir_t *dir_p);

/**
 * Read the next file or directory within the opened directory.
 *
 * @param[in] dir_p Directory object.
 * @param[out] entry_p Read entry.
 *
 * @return true(1) if an entry was read or false(0) if no entry could
 *         be read, otherwise negative error code.
 */
int fat16_dir_read(struct fat16_dir_t *dir_p,
                   struct fat16_dir_entry_t *entry_p);

/**
 * Gets file status by path.
 *
 * @param[in] self_p The file system struct.
 * @param[in] path_p The path of the file to stat.
 * @param[in] stat_p The stat struct to populate.
 *
 * @return zero(0) or negative error code.
 */
int fat16_stat(struct fat16_t *self_p,
               const char *path_p,
               struct fat16_stat_t *stat_p);

#endif
