/**
 * @file slib/fat16.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2009, William Greiman (Arduino Fat16 Library)
 * Copyright (C) 2013-2015, Mikael Patel (Cosa, Refactoring)
 * Copyright (C) 2015, Erik Moqvist (c implementation)
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

#ifndef __SLIB_FAT16_H__
#define __SLIB_FAT16_H__

#include "simba.h"

/*
 * FAT16 file structures on SD card. Note: may only access files on the
 * root directory.
 *
 * @section Acknowledgement
 * Refactoring of Arduino Fat16 Library, Copyright (C) 2009 by William Greiman
 *
 * @section References
 * 1. http://www.microsoft.com/whdc/system/platform/firmware/fatgen.mspx
 * 2. http://code.google.com/p/fat16lib/
 * 3. https://github.com/mikaelpatel/Cosa/tree/master/libraries/FAT16
 */

#define FAT16_LS_DATE 0x1
#define FAT16_LS_SIZE 0x2

#define FAT16_CACHE_FOR_READ  0    /* cache a block for read */
#define FAT16_CACHE_FOR_WRITE 1    /* cache a block and set dirty */

struct fat16_t {
    struct sd_t *sd_p;

    /* Volume info */
    int volume_initialized;        /* true if volume has been initialized */
    uint8_t fat_count;             /* number of FATs */
    uint8_t blocks_per_cluster;    /* must be power of 2 */
    uint16_t root_dir_entry_count; /* should be 512 for FAT16 */
    fat_t blocks_per_fat;          /* number of blocks in one FAT */
    fat_t cluster_count;           /* total clusters in volume */
    uint32_t fat_start_block;      /* start of first FAT */
    uint32_t root_dir_start_block; /* start of root dir */
    uint32_t data_start_block;     /* start of data clusters */

    /* block cache */
    cache16_t cache_buffer;        /* 512 byte cache for raw blocks */
    uint32_t cache_blocknumber;    /* Logical number of block in the cache */
    uint8_t cache_dirty;           /* cacheFlush() will write block if true */
    uint32_t cache_mirror_block;   /* mirror block for second FAT */
};

struct fat16_file_t {
    uint8_t flags;           /* see above for bit definitions */
    int16_t dir_entry_index; /* index of directory entry for open file */
    fat_t first_cluster;     /* first cluster of file */
    size_t file_size;        /* fileSize */
    fat_t cur_cluster;       /* current cluster */
    size_t cur_position;     /* current byte offset */
};

/**
 * Initialize a FAT16 volume.
 * @param[in] dev SD device where the volume is located.
 * @param[in] part partition to be used. Legal values for \a part
 * are 1-4 to use the corresponding partition on a device formatted
 * with a MBR, Master Boot Record, or zero if the device is
 * formatted as a super floppy with the FAT boot sector in block zero.
 * @return The value one, true, is returned for success and the
 * value zero, false, is returned for failure.  reasons for failure
 * include not finding a valid FAT16 file system in the specified
 * partition, a call to begin() after a volume has been successful
 * initialized or an I/O error.
 */
int fat16_init(struct fat16_t *fat16_p, SD* sd, uint8_t partition);

/**
 * Initialize a FAT16 volume.
 * @param[in] dev SD device where the volume is located.
 * @param[in] part partition to be used. Legal values for \a part
 * are 1-4 to use the corresponding partition on a device formatted
 * with a MBR, Master Boot Record, or zero if the device is
 * formatted as a super floppy with the FAT boot sector in block zero.
 * @return The value one, true, is returned for success and the
 * value zero, false, is returned for failure.  reasons for failure
 * include not finding a valid FAT16 file system in the specified
 * partition, a call to begin() after a volume has been successful
 * initialized or an I/O error.
 */
int fat16_start(struct fat16_t *fat16_p);

/**
 * Initialize a FAT16 volume.
 * @param[in] dev SD device where the volume is located.
 * @param[in] part partition to be used. Legal values for \a part
 * are 1-4 to use the corresponding partition on a device formatted
 * with a MBR, Master Boot Record, or zero if the device is
 * formatted as a super floppy with the FAT boot sector in block zero.
 * @return The value one, true, is returned for success and the
 * value zero, false, is returned for failure.  reasons for failure
 * include not finding a valid FAT16 file system in the specified
 * partition, a call to begin() after a volume has been successful
 * initialized or an I/O error.
 */
int fat16_stop(struct fat16_t *fat16_p);

/**
 * List directory contents to given iostream with selected
 * information.
 * @param[in] outs output stream.
 * @param[in] flags The inclusive OR of LS_DATE (file modification
 * date) and LS_SIZE (file size).
 */
int fat16_ls(struct fat16_t *fat16_p,
             chan_t *chan_p,
             int flags);

/**
 * Remove a file. The directory entry and all data for the file are
 * deleted.
 * @param[in] fileName name of the file to be removed.
 * @note this function should not be used to delete the 8.3 version
 * of a file that has a long name. For example if a file has the
 * long name "New Text Document.txt" you should not delete the 8.3
 * name "NEWTEX~1.TXT".
 * @return The value one, true, is returned for success and the
 * value zero, false, is returned for failure. Reasons for failure
 * include the file is read only, \a fileName is not found or an I/O
 * error occurred.
 */
int fat16_rm(struct fat16_t *fat16_p,
             const char* path_p);

/**
 * Open a file by file name and mode flags. The file must be in
 * the root directory and must have a DOS 8.3 name. Returns true
 * if successful otherwise false for failure. Reasons for failure
 * include the FAT volume has not been initialized, a file is
 * already open, file name, is invalid, the file does not exist,
 * is a directory, or can't be opened in the access mode specified
 * by oflag.
 * @param[in] fileName a valid 8.3 DOS name for a file in the root.
 * @param[in] oflag mode of file open (create, read, write, etc).
 * @return bool.
 */
inf fat16_file_open(struct fat16_t *fat16_p,
                    struct fat16_file_t *file_p,
                    const char* path_p,
                    int mode);

/**
 * Close a file and force cached data and directory information to
 * be written to the storage device.
 * @return bool, true if successful otherwise false for
 * failure. Reasons for failure include the file is not open for
 * write or an I/O error occurred.
 */
int fat16_file_close(struct fat16_file_t *file_p);

/**
 * Read data to given buffer with given size from the file.
 * @param[in] buf buffer to read into.
 * @param[in] size number of bytes to read.
 * @return number of bytes read or EOF(-1).
 */
ssize_t fat16_file_read(struct fat16_file_t *file_p,
                        void* buf_p,
                        size_t size);

/**
 * Write data from buffer with given size to the file.
 * @param[in] buf buffer to write.
 * @param[in] size number of bytes to write.
 * @return number of bytes written or EOF(-1).
 */
ssize_t fat16_file_write(struct fat16_file_t *file_p,
                         const void *buf_p,
                         size_t size);

/**
 * Sets the file's read/write position relative to mode.
 * @param[in] pos new position in bytes from given mode.
 * @param[in] mode absolute, relative and from end.
 * @return bool, true if successful otherwise false for
 * failure.
 */
int fat16_file_seek(struct fat16_file_t *file_p,
                    size_t pos,
                    int whence);

/**
 * Return current position.
 */
ssize_t fat16_file_tell(struct fat16_file_t *file_p);

/**
 * Return number of bytes in file.
 */
ssize_t fat16_file_size(struct fat16_file_t *file_p);

/**
 * The sync() call causes all modified data and directory fields
 * to be written to the storage device.
 * @return true if successful otherwise false for failure. Reasons
 * for failure include a call to sync() before a file has been
 * opened or an I/O error.
 */
int fat16_file_sync(struct fat16_file_t *file_p);

#endif
