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
#include <arpa/inet.h>

/* define fields in flags_ require sync directory entry */
#define F_OFLAG (O_RDWR | O_APPEND | O_SYNC)
#define F_FILE_DIR_DIRTY 0x80

/** Value for byte 510 and 511 of boot block or MBR */
#define BOOTSIG ntohs(0x55aa)

#define CACHE_FOR_READ  0    /* cache a block for read */
#define CACHE_FOR_WRITE 1    /* cache a block and set dirty */

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
#define DIR_ATTR_READ_ONLY 0x01

/* File should hidden in directory listings */
#define DIR_ATTR_HIDDEN 0x02

/* Entry is for a system file */
#define DIR_ATTR_SYSTEM 0x04

/* Directory entry contains the volume label */
#define DIR_ATTR_VOLUME_ID 0x08

/* Entry is for a directory */
#define DIR_ATTR_DIRECTORY 0x10

/* Old DOS archive bit for backup support */
#define DIR_ATTR_ARCHIVE 0x20

/* Test value for long name entry */
#define DIR_ATTR_LONG_NAME 0x0f

/* Test mask for long name entry */
#define DIR_ATTR_LONG_NAME_MASK 0x3f

/* defined attribute bits */
#define DIR_ATTR_DEFINED_BITS 0x3f

/* Mask for file/subdirectory tests */
#define DIR_ATTR_FILE_TYPE_MASK (DIR_ATTR_VOLUME_ID | DIR_ATTR_DIRECTORY)
#define DIR_ATTR_SKIP           (DIR_ATTR_VOLUME_ID | DIR_ATTR_DIRECTORY)

/** Default date for file timestamps is 1 Jan 2000 */
#define DEFAULT_DATE (((2000 - 1980) << 9) | (1 << 5) | 1)

/** Default time for file timestamp is 1 am */
#define DEFAULT_TIME (1 << 11)

static int is_end_of_cluster(fat_t cluster)
{
    return (cluster >= 0xfff8);
}

/**
 * Directory entry is part of a long name.
 */
static inline int dir_is_long_name(const struct dir_t* dir_p)
{
    return ((dir_p->attributes & DIR_ATTR_LONG_NAME_MASK)
            == DIR_ATTR_LONG_NAME);
}

/**
 * Directory entry is for a file.
 */
static inline int dir_is_file(const struct dir_t* dir_p)
{
    return ((dir_p->attributes & DIR_ATTR_FILE_TYPE_MASK) == 0);
}

/**
 * Directory entry is for a subdirectory.
 */
static inline int dir_is_subdir(const struct dir_t* dir_p)
{
    return ((dir_p->attributes & DIR_ATTR_FILE_TYPE_MASK)
            == DIR_ATTR_DIRECTORY);
}

/**
 * Directory entry is for a file or subdirectory
 */
static inline int dir_is_file_or_subdir(const struct dir_t* dir_p)
{
    return ((dir_p->attributes & DIR_ATTR_VOLUME_ID) == 0);
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

static int cache_flush(struct fat16_t *fat16_p)
{
    struct fat16_cache_t *cache_p = &fat16_p->cache;

    if (cache_p->dirty)
    {
        if (sd_write(fat16_p->sd_p,
                     cache_p->block_number,
                     cache_p->buffer.data) != SD_BLOCK_SIZE)
        {
            return (1);
        }

        if (cache_p->mirror_block)
        {
            if (sd_write(fat16_p->sd_p,
                         cache_p->mirror_block,
                         cache_p->buffer.data) != SD_BLOCK_SIZE)
            {
                return (1);
            }

            cache_p->mirror_block = 0;
        }

        cache_p->dirty = 0;
    }

    return (0);
}

static inline uint8_t block_of_cluster(uint8_t blocks_per_cluster,
                                       uint32_t position)
{
    return ((position >> 9) & (blocks_per_cluster - 1));
}

static inline uint16_t cache_data_offset(uint32_t position)
{
    return (position & 0x1ff);
}

static inline void cache_set_dirty(struct fat16_cache_t *cache_p)
{
    cache_p->dirty |= CACHE_FOR_WRITE;
}

static inline uint32_t data_block_lba(struct fat16_file_t *file_p,
                                      uint8_t block_of_cluster)
{
    return (file_p->fat16_p->data_start_block +
            (uint32_t)((file_p->cur_cluster - 2)
                       * file_p->fat16_p->blocks_per_cluster) +
            block_of_cluster);
}

static int cache_raw_block(struct fat16_t *fat16_p,
                           uint32_t block_number,
                           uint8_t action)
{
    struct fat16_cache_t *cache_p = &fat16_p->cache;

    if (cache_p->block_number != block_number)
    {
        if (cache_flush(fat16_p) != 0)
        {
            return (1);
        }

        if (sd_read(fat16_p->sd_p,
                    cache_p->buffer.data,
                    block_number) != SD_BLOCK_SIZE)
        {
            return (1);
        }

        cache_p->block_number = block_number;
    }

    cache_p->dirty |= action;

    return (0);
}

static int fat_get(struct fat16_t *fat16_p,
                   fat_t cluster,
                   fat_t* value)
{
    uint32_t lba;

    if (cluster > (fat16_p->cluster_count + 1))
    {
        return (1);
    }

    lba = fat16_p->fat_start_block + (cluster >> 8);

    if (lba != fat16_p->cache.block_number)
    {
        if (cache_raw_block(fat16_p, lba, CACHE_FOR_READ))
        {
            return (1);
        }
    }

    *value = fat16_p->cache.buffer.fat[cluster & 0xff];

    return (0);
}

static int fat_put(struct fat16_t *fat16_p, fat_t cluster, fat_t value)
{
    uint32_t lba;

    if (cluster < 2) {
        return (1);
    }

    if (cluster > (fat16_p->cluster_count + 1)) {
        return (1);
    }

    lba = fat16_p->fat_start_block + (cluster >> 8);

    if (lba != fat16_p->cache.block_number) {
        if (cache_raw_block(fat16_p, lba, CACHE_FOR_READ) != 0) {
            return (1);
        }
    }

    fat16_p->cache.buffer.fat[cluster & 0xff] = value;
    cache_set_dirty(&fat16_p->cache);

    if (fat16_p->fat_count > 1) {
        fat16_p->cache.mirror_block = (lba + fat16_p->blocks_per_fat);
    }

    return (0);
}

static struct dir_t* cache_dir_entry(struct fat16_t *fat16_p,
                                     uint16_t index,
                                     uint8_t action)
{
    if (index >= fat16_p->root_dir_entry_count) {
        return (NULL);
    }

    if (cache_raw_block(fat16_p,
                        fat16_p->root_dir_start_block + (index >> 4),
                        action) != 0) {
        return (NULL);
    }

    return (&fat16_p->cache.buffer.dir[index & 0xf]);
}

static int fat16_read(struct fat16_t *fat16_p,
                      struct dir_t* dir,
                      uint16_t* index,
                      uint8_t skip)
{
    struct dir_t* dir_p;
    uint16_t i;

    for (i = *index; ; i++) {
        if (i >= fat16_p->root_dir_entry_count) {
            return (1);
        }

        if (!(dir_p = cache_dir_entry(fat16_p, i, CACHE_FOR_READ))) {
            return (1);
        }

        /* Done if beyond last used entry */
        if (dir_p->name[0] == DIR_NAME_FREE) {
            return (1);
        }

        /* Skip deleted entry */
        if (dir_p->name[0] == DIR_NAME_DELETED) {
            continue;
        }

        /* Skip long names */
        if ((dir_p->attributes & DIR_ATTR_LONG_NAME_MASK) == DIR_ATTR_LONG_NAME) {
            continue;
        }

        /* Skip if attribute match */
        if (dir_p->attributes & skip) {
            continue;
        }

        /* Return found index */
        *index = i;

        break;
    }

    memcpy(dir, dir_p, sizeof(struct dir_t));

    return (0);
}

static int print_dir_name(chan_t *chan_p,
                          const struct dir_t *dir_p,
                          uint8_t width)
{
    uint8_t w = 0;
    uint8_t i;

    for (i = 0; i < 11; i++) {
        if (dir_p->name[i] == ' ') {
            continue;
        }

        if (i == 8) {
            chan_write(chan_p, ".", 1);
            w++;
        }

        chan_write(chan_p, &dir_p->name[i], 1);
        w++;
    }

    if (dir_is_subdir(dir_p)) {
        chan_write(chan_p, "/", 1);
        w++;
    }

    while (w < width) {
        chan_write(chan_p, " ", 1);
        w++;
    }

    return (0);
}

static int free_chain(struct fat16_t *fat16_p, fat_t cluster)
{
    fat_t next;

    while (1) {
        if (fat_get(fat16_p, cluster, &next) != 0) {
            return (1);
        }

        if (fat_put(fat16_p, cluster, 0) != 0) {
            return (1);
        }

        if (is_end_of_cluster(next)) {
            return (0);
        }

        cluster = next;
    }
}

int fat16_init(struct fat16_t *fat16_p,
               struct sd_driver_t* sd_p,
               unsigned int partition)
{
    /* Error if invalid partition. */
    if (partition > 4) {
        return (-EINVAL);
    }

    /* Initialize datastructure.*/
    fat16_p->sd_p = sd_p;
    fat16_p->partition = partition;
    fat16_p->cache.block_number = 0xffffffff;
    fat16_p->cache.dirty = 0;
    fat16_p->cache.mirror_block = 0;

    return (0);
}

int fat16_start(struct fat16_t *fat16_p)
{
    uint32_t volume_start_block = 0;
    uint32_t total_blocks;
    struct bpb_t* bpb_p;

    /* If part == 0 assume super floppy with FAT16 boot sector in block zero */
    /* If part > 0 assume mbr volume with partition table */
    if (fat16_p->partition > 0) {
        if (cache_raw_block(fat16_p, volume_start_block, CACHE_FOR_READ) != 0) {
            return (1);
        }

        volume_start_block =
            fat16_p->cache.buffer.mbr.part[fat16_p->partition-1].first_sector;
    }

    if (cache_raw_block(fat16_p, volume_start_block, CACHE_FOR_READ) != 0) {
        return (1);
    }

    /* Check boot block signature */
    if (fat16_p->cache.buffer.fbs.boot_sector_sig != BOOTSIG) {
        return (1);
    }

    bpb_p = &fat16_p->cache.buffer.fbs.bpb;
    fat16_p->fat_count = bpb_p->fat_count;
    fat16_p->blocks_per_cluster = bpb_p->sectors_per_cluster;
    fat16_p->blocks_per_fat = bpb_p->sectors_per_fat;
    fat16_p->root_dir_entry_count = bpb_p->root_dir_entry_count;
    fat16_p->fat_start_block = volume_start_block + bpb_p->reserved_sector_count;
    fat16_p->root_dir_start_block = (fat16_p->fat_start_block
                                     + bpb_p->fat_count * bpb_p->sectors_per_fat);
    fat16_p->data_start_block = (fat16_p->root_dir_start_block
                                 + ((32 * bpb_p->root_dir_entry_count + 511) / 512));
    total_blocks = (bpb_p->total_sectors_small
                    ? bpb_p->total_sectors_small
                    : bpb_p->total_sectors_large);
    fat16_p->cluster_count = ((total_blocks
                               - (fat16_p->data_start_block - volume_start_block))
                              / bpb_p->sectors_per_cluster);

    /* Check valid FAT16 volume */
    if ((bpb_p->bytes_per_sector != 512)      /* Only allow 512 byte blocks */
        || (bpb_p->sectors_per_fat == 0)      /* Zero for FAT32 */
        || (fat16_p->cluster_count < 4085)    /* FAT12 if true */
        || (total_blocks > 0x800000)          /* Max size for FAT16 volume */
        || (bpb_p->reserved_sector_count == 0)  /* invalid volume */
        || (bpb_p->fat_count == 0)             /* invalid volume */
        || (bpb_p->sectors_per_fat < (fat16_p->cluster_count >> 8))
        || (bpb_p->sectors_per_cluster == 0)
        || (bpb_p->sectors_per_cluster & (bpb_p->sectors_per_cluster - 1))) {
        return (1);
    }

    return (0);
}

int fat16_stop(struct fat16_t *fat16_p)
{
    return (0);
}

int fat16_ls(struct fat16_t *fat16_p, chan_t *chan_p, int flags)
{
    struct dir_t dir;
    uint16_t index;
    union fat16_date_t date;
    union fat16_time_t time;

    for (index = 0; fat16_read(fat16_p,
                               &dir,
                               &index,
                               DIR_ATTR_VOLUME_ID) == 0; index++)
    {
        /* Print file name with possible blank fill */
        print_dir_name(chan_p,
                       &dir,
                       (flags & (FAT16_LS_DATE | FAT16_LS_SIZE) ? 14 : 0));

        /* Print modify date/time if requested */
        if (flags & FAT16_LS_DATE)
        {
            date = (union fat16_date_t)dir.last_write_date;
            time = (union fat16_time_t)dir.last_write_time;
            std_fprintf(chan_p, FSTR("%u-%u-%u %u:%u:%u"),
                        1980 + date.bits.year,
                        date.bits.month,
                        date.bits.day,
                        time.bits.hours,
                        time.bits.minutes,
                        2 * time.bits.seconds);
        }

        /* Print size if requested */
        if (dir_is_file(&dir) && (flags & FAT16_LS_SIZE))
        {
            std_fprintf(chan_p, FSTR(" %lu"), (unsigned long)dir.file_size);
        }

        std_fprintf(chan_p, FSTR("\r\n"));
    }

    return (0);
}

int fat16_rm(struct fat16_t *fat16_p,
             const char* path_p)
{
    return (0);
}

int fat16_print(struct fat16_t *fat16_p, chan_t *chan_p)
{
    std_fprintf(chan_p,
                FSTR("fat_count = %u\r\n"
                     "blocks_per_cluster = %u\r\n"
                     "root_dir_entry_count = %u\r\n"
                     "blocks_per_fat = %u\r\n"
                     "cluster_count = %u\r\n"
                     "fat_start_block = %lu\r\n"
                     "root_dir_start_block = %lu\r\n"
                     "data_start_block = %lu\r\n"),
                (unsigned int)fat16_p->fat_count,
                (unsigned int)fat16_p->blocks_per_cluster,
                (unsigned int)fat16_p->root_dir_entry_count,
                (unsigned int)fat16_p->blocks_per_fat,
                (unsigned int)fat16_p->cluster_count,
                (unsigned long)fat16_p->fat_start_block,
                (unsigned long)fat16_p->root_dir_start_block,
                (unsigned long)fat16_p->data_start_block);

    return (0);
}

static int file_truncate(struct fat16_file_t *file_p,
                         size_t length)
{
    uint32_t newPos;
    fat_t toFree;

    /* Error if file is not open for write */
    if (!(file_p->flags & O_WRITE)) {
        return (1);
    }

    if (length > file_p->file_size) {
        return (1);
    }

    /* Filesize and length are zero - nothing to do */
    if (file_p->file_size == 0) {
        return (0);
    }

    newPos = file_p->cur_position > length ? length : file_p->cur_position;

    if (length == 0) {
        /* Free all clusters */
        if (!free_chain(file_p->fat16_p, file_p->first_cluster)) {
            return (1);
        }

        file_p->cur_cluster = file_p->first_cluster = 0;
    } else {
        if (!fat16_file_seek(file_p, length, SEEK_SET)) {
            return (1);
        }

        if (fat_get(file_p->fat16_p, file_p->cur_cluster, &toFree) != 0) {
            return (1);
        }

        if (!is_end_of_cluster(toFree)) {
            /* Free extra clusters */
            if (!fat_put(file_p->fat16_p, file_p->cur_cluster, EOC16)) {
                return (1);
            }

            if (!free_chain(file_p->fat16_p, toFree)) {
                return (1);
            }
        }
    }

    file_p->file_size = length;
    file_p->flags |= F_FILE_DIR_DIRTY;

    if (fat16_file_sync(file_p) != 0) {
        return (1);
    }

    return fat16_file_seek(file_p, newPos, SEEK_SET);
}

static int file_open(struct fat16_t *fat16_p,
                     struct fat16_file_t *file_p,
                     uint16_t index,
                     int oflag)
{
    struct dir_t* dir_p;

    if ((oflag & O_TRUNC) && !(oflag & O_WRITE)) {
        return (1);
    }

    dir_p = cache_dir_entry(fat16_p, index, CACHE_FOR_READ);

    /* If bad file index or I/O error */
    if (!dir_p) {
        return (1);
    }

    /* Error if unusedir_p entry */
    if (dir_p->name[0] == DIR_NAME_FREE || dir_p->name[0] == DIR_NAME_DELETED) {
        return (1);
    }

    /* Error if long name, volume label or subdirectory */
    if ((dir_p->attributes & (DIR_ATTR_VOLUME_ID | DIR_ATTR_DIRECTORY)) != 0) {
        return (1);
    }

    /* Don't allow write or truncate if read-only */
    if ((dir_p->attributes & DIR_ATTR_READ_ONLY) && (oflag & (O_WRITE | O_TRUNC))) {
        return (1);
    }

    file_p->fat16_p = fat16_p;
    file_p->cur_cluster = 0;
    file_p->cur_position = 0;
    file_p->dir_entry_index = index;
    file_p->file_size = dir_p->file_size;
    file_p->first_cluster = dir_p->first_cluster_low;
    file_p->flags = oflag & (O_RDWR | O_SYNC | O_APPEND);

    if (oflag & O_TRUNC) {
        return (file_truncate(file_p, 0));
    }

    return (0);
}

static int add_cluster(struct fat16_file_t *file_p)
{
    /* Start search after last cluster of file or at cluster two in FAT */
    fat_t freeCluster = file_p->cur_cluster ? file_p->cur_cluster : 1;
    fat_t value;
    fat_t i;
    fat_t cluster_count = file_p->fat16_p->cluster_count;

    for (i = 0; ; i++)
    {
        /* Return no free clusters */
        if (i >= cluster_count) {
            return (1);
        }

        /* Fat has cluster_count + 2 entries */
        if (freeCluster > cluster_count) {
            freeCluster = 1;
        }

        freeCluster++;

        if (fat_get(file_p->fat16_p, freeCluster, &value) != 0) {
            return (1);
        }

        if (value == 0) {
            break;
        }
    }

    /* Mark cluster allocated */
    if (fat_put(file_p->fat16_p, freeCluster, EOC16) != 0) {
        return (1);
    }

    if (file_p->cur_cluster != 0) {
        /* Link cluster to chain */
        if (fat_put(file_p->fat16_p, file_p->cur_cluster, freeCluster) != 0) {
            return (1);
        }
    } else {
        /* first cluster of file so update directory entry */
        file_p->flags |= F_FILE_DIR_DIRTY;
        file_p->first_cluster = freeCluster;
    }

    file_p->cur_cluster = freeCluster;

    return (0);
}

int fat16_file_open(struct fat16_t *fat16_p,
                    struct fat16_file_t *file_p,
                    const char* path_p,
                    int oflag)
{
    uint8_t dname[11];   /* name formated for dir entry */
    int16_t empty = -1;  /* index of empty slot */
    struct dir_t* dir_p; /* pointer to cached dir entry */
    uint16_t index;

    /* Check valid 8.3 file name */
    if (make_83_name(path_p, dname) != 0) {
        return (1);
    }

    /* Search for the file in the root directory. */
    for (index = 0; index < fat16_p->root_dir_entry_count; index++) {
        if (!(dir_p = cache_dir_entry(fat16_p, index, CACHE_FOR_READ))) {
            return (1);
        }

        if ((dir_p->name[0] == DIR_NAME_FREE)
            || (dir_p->name[0] == DIR_NAME_DELETED)) {
            /* Remember first empty slot */
            if (empty < 0) {
                empty = index;
            }

            /* Done if no entries follow */
            if (dir_p->name[0] == DIR_NAME_FREE) {
                break;
            }
        } else if (!memcmp(dname, dir_p->name, 11)) {
            /* Don't open existing file if O_CREAT and O_EXCL */
            if ((oflag & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL)) {
                return (1);
            }

            /* Open existing file */
            return (file_open(fat16_p, file_p, index, oflag));
        }
    }

    /* Error if directory is full */
    if (empty < 0) {
        return (1);
    }

    /* Only create file if O_CREAT and O_WRITE */
    if ((oflag & (O_CREAT | O_WRITE)) != (O_CREAT | O_WRITE)) {
        return (1);
    }

    if (!(dir_p = cache_dir_entry(fat16_p, empty, CACHE_FOR_WRITE))) {
        return (1);
    }

    /* Initialize as empty file */
    memset(dir_p, 0, sizeof(struct dir_t));
    memcpy(dir_p->name, dname, 11);

    /* Set timestamps with user function or use default date/time */
    /* if (dateTime) { */
    /*     dateTime(&dir_p->creationDate, &dir_p->creationTime); */
    /* } else { */
    dir_p->creation_date = DEFAULT_DATE;
    dir_p->creation_time = DEFAULT_TIME;
    /* } */

    dir_p->last_access_date = dir_p->creation_date;
    dir_p->last_write_date = dir_p->creation_date;
    dir_p->last_write_time = dir_p->creation_time;

    /* Force created directory entry will be written to storage
       device. */
    if (cache_flush(fat16_p) != 0) {
        return (1);
    }

    /* Open entry */
    return (file_open(fat16_p, file_p, empty, oflag));
}

int fat16_file_close(struct fat16_file_t *file_p)
{
    return (0);
}

ssize_t fat16_file_read(struct fat16_file_t *file_p,
                        void* buf_p,
                        size_t size)
{
    uint16_t left;
    uint8_t blk_of_cluster;
    uint16_t block_offset;
    uint8_t* src_p;
    uint16_t n;

    /* Error if not open for read */
    if (!(file_p->flags & O_READ)) {
        return (EOF);
    }

    /* Don't read beyond end of file */
    if ((file_p->cur_position + size) > file_p->file_size) {
        size = file_p->file_size - file_p->cur_position;
    }

    /* Bytes left to read in loop */
    left = size;

    while (left > 0) {
        blk_of_cluster = block_of_cluster(file_p->fat16_p->blocks_per_cluster,
                                            file_p->cur_position);
        block_offset = cache_data_offset(file_p->cur_position);

        if (blk_of_cluster == 0 && block_offset == 0) {
            /* Start next cluster */
            if (file_p->cur_cluster == 0) {
                file_p->cur_cluster = file_p->first_cluster;
            } else {
                if (fat_get(file_p->fat16_p, file_p->cur_cluster, &file_p->cur_cluster) != 0) {
                    return (EOF);
                }
            }

            /* Return error if bad cluster chain */
            if (file_p->cur_cluster < 2 || is_end_of_cluster(file_p->cur_cluster)) {
                return (EOF);
            }
        }

        /* Cache data block */
        if (cache_raw_block(file_p->fat16_p,
                            data_block_lba(file_p, blk_of_cluster),
                            CACHE_FOR_READ) != 0) {
            return (EOF);
        }

        /* Location of data in cache */
        src_p = file_p->fat16_p->cache.buffer.data + block_offset;

        /* Max number of byte available in block */
        n = 512 - block_offset;

        /* Lesser of available and amount to read */
        if (n > left) {
            n = left;
        }

        /* Copy data to caller */
        memcpy(buf_p, src_p, n);

        file_p->cur_position += n;
        src_p += n;
        left -= n;
    }

    return (size);
}

ssize_t fat16_file_write(struct fat16_file_t *file_p,
                         const void *src_p,
                         size_t size)
{
    uint16_t left = size;
    uint8_t blk_of_cluster;
    uint16_t block_offset;
    fat_t next;
    uint32_t lba;
    uint8_t* dst_p;
    uint16_t n;

    /* Error if file is not open for write */
    if (!(file_p->flags & O_WRITE)) {
        return (EOF);
    }

    /* Go to end of file if O_APPEND */
    if ((file_p->flags & O_APPEND)
        && (file_p->cur_position != file_p->file_size)) {
        if (fat16_file_seek(file_p, 0, SEEK_END) != 0) {
            return (EOF);
        }
    }

    while (left > 0) {
        blk_of_cluster = block_of_cluster(file_p->fat16_p->blocks_per_cluster,
                                          file_p->cur_position);

        block_offset = cache_data_offset(file_p->cur_position);

        if (blk_of_cluster == 0 && block_offset == 0) {
            /* Start of new cluster */
            if (file_p->cur_cluster == 0) {
                if (file_p->first_cluster == 0) {
                    /* Allocate first cluster of file */
                    if (add_cluster(file_p) != 0) {
                        return (EOF);
                    }
                } else {
                    file_p->cur_cluster = file_p->first_cluster;
                }
            } else {
                if (fat_get(file_p->fat16_p, file_p->cur_cluster, &next) != 0) {
                    return (EOF);
                }

                if (is_end_of_cluster(next)) {
                    /* Add cluster if at end of chain */
                    if (add_cluster(file_p) != 0) {
                        return (EOF);
                    }
                } else {
                    file_p->cur_cluster = next;
                }
            }
        }

        lba = data_block_lba(file_p, blk_of_cluster);

        if (block_offset == 0 && file_p->cur_position >= file_p->file_size) {
            /* Start of new block don't need to read into cache */
            if (cache_flush(file_p->fat16_p) != 0) {
                return (EOF);
            }

            file_p->fat16_p->cache.block_number = lba;
            cache_set_dirty(&file_p->fat16_p->cache);
        } else {
            /* Rewrite part of block */
            if (cache_raw_block(file_p->fat16_p, lba, CACHE_FOR_WRITE) != 0) {
                return (EOF);
            }
        }

        dst_p = file_p->fat16_p->cache.buffer.data + block_offset;

        /* Max space in block */
        n = 512 - block_offset;

        /* Lesser of space and amount to write */
        if (n > left) {
            n = left;
        }

        /* Copy data to cache */
        memcpy(dst_p, src_p, n);

        file_p->cur_position += n;
        left -= n;
        src_p += n;
    }

    if (file_p->cur_position > file_p->file_size) {
        /* Update file_size and insure sync will update dir entry */
        file_p->file_size = file_p->cur_position;
        file_p->flags |= F_FILE_DIR_DIRTY;
    }/*  else if (dateTime && size) { */
    /* Force sync will update modified date and time */
    /*     file_p->flags |= F_FILE_DIR_DIRTY; */
    /* } */

    if (file_p->flags & O_SYNC) {
        if (fat16_file_sync(file_p) != 0) {
            return (EOF);
        }
    }

    return (size);
}

int fat16_file_seek(struct fat16_file_t *file_p,
                    size_t pos,
                    int whence)
{
    fat_t n;
    uint8_t blocks_per_cluster = file_p->fat16_p->blocks_per_cluster;

    if (whence == SEEK_CUR) {
        pos += file_p->cur_position;
    } else if (whence == SEEK_END) {
        pos = file_p->file_size;
    } else if (whence != SEEK_SET) {
        return (1);
    }

    /* Error if file not open or seek past end of file */
    if (pos > file_p->file_size) {
        return (1);
    }

    if (pos == 0) {
        /* Set position to start of file */
        file_p->cur_cluster = 0;
        file_p->cur_position = 0;

        return (0);
    }

    n = ((pos - 1) >> 9) / blocks_per_cluster;

    if (pos < file_p->cur_position || file_p->cur_position == 0) {
        /* Must follow chain from first cluster */
        file_p->cur_cluster = file_p->first_cluster;
    } else {
        /* Advance from cur_position */
        n -= ((file_p->cur_position - 1) >> 9) / blocks_per_cluster;
    }

    while (n--) {
        if (fat_get(file_p->fat16_p,
                    file_p->cur_cluster,
                    &file_p->cur_cluster) != 0) {
            return (1);
        }
    }

    file_p->cur_position = pos;

    return (0);
}

ssize_t fat16_file_tell(struct fat16_file_t *file_p)
{
    return (file_p->cur_position);
}

ssize_t fat16_file_size(struct fat16_file_t *file_p)
{
    return (0);
}

int fat16_file_sync(struct fat16_file_t *file_p)
{
    struct dir_t* dir_p;

    if (file_p->flags & F_FILE_DIR_DIRTY) {
        /* Cache directory entry */
        dir_p = cache_dir_entry(file_p->fat16_p,
                                file_p->dir_entry_index,
                                CACHE_FOR_WRITE);

        if (!dir_p) {
            return (1);
        }

        /* Update file size and first cluster */
        dir_p->file_size = file_p->file_size;
        dir_p->first_cluster_low = file_p->first_cluster;

        /* Set modify time if user supplied a callback date/time function */
        /* if (dateTime) { */
        /*     dateTime(&dir_p->last_write_date, &dir_p->last_write_time); */
        /*     dir_p->last_access_date = dir_p->last_write_date; */
        /* } */

        file_p->flags &= ~F_FILE_DIR_DIRTY;
    }

    return (cache_flush(file_p->fat16_p));
}

#if 0

int fat16_file_remove(struct fat16_file_t *file_p)
{
    struct struct dir_t* dir_p;

    /* Error if file is not open for write */
    if (!(file_p->file_p->flags & O_WRITE)) {
        return (1);
    }

    if (file_p->first_cluster && !free_chain(file_p->first_cluster)) {
        return (1);
    }

    dir_p = cacheDirEntry(file_p->dir_entry_index, CACHE_FOR_WRITE);

    if (dir_p == NULL) {
        return (1);
    }

    dir_p->name[0] = DIR_NAME_DELETED;
    file_p->file_p->flags = 0;

    return (cacheFlush());
}

int fat16_file_direntry(struct dir_t* dir_p)
{
    if (!sync()) {
        return (1);
    }

    struct dir_t* p = cacheDirEntry(file_p->dir_entry_index, CACHE_FOR_WRITE);

    if (!p) {
        return (1);
    }

    memcpy(dir, p, sizeof(struct dir_t));

    return (0);
}
#endif
