/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

#define OUR_PARAMETER_DEFAULT 5
#define BUFFER_SIZE 512

static struct fs_command_t foo_bar;
static struct fs_command_t bar;

static struct fs_counter_t my_counter;
static struct fs_counter_t your_counter;

static int our_parameter_value = OUR_PARAMETER_DEFAULT;
static struct fs_parameter_t our_parameter;

static struct fs_filesystem_operations_t generic_ops;
static struct fs_filesystem_t genericfs;

#if defined(ARCH_LINUX)

#define BLOCK_SIZE 512

static uint8_t fat16_buffer[65536] = { 0 };

/* SPIFFS configuration. */

#define PHY_SIZE         2048
#define PHY_ADDR            0
#define PHYS_ERASE_BLOCK  128

#define LOG_BLOCK_SIZE    128
#define LOG_PAGE_SIZE      64

#define FILE_SIZE_MAX     256
#define CHUNK_SIZE_MAX    128

static uint8_t spiffs_buffer[PHY_SIZE];

struct fat16_t fat16_fs;
struct fs_filesystem_t fat16fs;

struct spiffs_t spiffs_fs;
struct fs_filesystem_t spiffsfs;
struct fs_filesystem_spiffs_config_t configfs;
struct spiffs_config_t config;
static uint8_t workspace[2 * LOG_PAGE_SIZE];
static uint8_t fdworkspace[128];
static uint8_t cache[256];

/**
 * FAT16 read block callback.
 */
static ssize_t filesystem_fat16_read_block(void *arg_p,
                                            void *dst_p,
                                            uint32_t src_block)
{
    ASSERT(src_block < sizeof(fat16_buffer) / BLOCK_SIZE);

    memcpy(dst_p, &fat16_buffer[BLOCK_SIZE * src_block], BLOCK_SIZE);

    return (BLOCK_SIZE);
}

/**
 * FAT16 write block callback.
 */
static ssize_t filesystem_fat16_write_block(void *arg_p,
                                             uint32_t dst_block,
                                             const void *src_p)
{
    ASSERT(dst_block < sizeof(fat16_buffer) / 512);

    memcpy(&fat16_buffer[BLOCK_SIZE * dst_block], src_p, BLOCK_SIZE);

    return (BLOCK_SIZE);
}

/**
 * SPIFFS read callback.
 */
static int32_t filesystem_spiffs_read(struct spiffs_t *fs_p,
                                       uint32_t addr,
                                       uint32_t size,
                                       uint8_t *dst_p)
{
    BTASSERT(addr + size <= sizeof(spiffs_buffer));

    memcpy(dst_p, &spiffs_buffer[addr], size);

    return (0);
}

/**
 * SPIFFS write callback.
 */
static int32_t filesystem_spiffs_write(struct spiffs_t *fs_p,
                                        uint32_t addr,
                                        uint32_t size,
                                        uint8_t *src_p)
{
    BTASSERT(addr + size <= sizeof(spiffs_buffer));

    memcpy(&spiffs_buffer[addr], src_p, size);

    return (0);
}

/**
 * SPIFFS erase callback.
 */
static int32_t filesystem_spiffs_erase(struct spiffs_t *fs_p,
                                        uint32_t addr,
                                        uint32_t size)
{
    BTASSERT(addr >= 0);
    BTASSERT(addr + size <= sizeof(spiffs_buffer));

    memset(&spiffs_buffer[addr], -1, size);

    return (0);
}

#endif

static int tmp_foo_bar(int argc,
                       const char *argv[],
                       void *out_p,
                       void *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    UNUSED(out_p);
    UNUSED(in_p);

    BTASSERT(argc == 3);
    std_fprintf(out_p,
                FSTR("argc = %d, argv[0] = %s, argv[1] = %s, argv[2] = %s\n"),
                argc,
                argv[0],
                argv[1],
                argv[2]);

    return (0);
}

static int tmp_bar(int argc,
                   const char *argv[],
                   void *out_p,
                   void *in_p,
                   void *arg_p,
                   void *call_arg_p)
{
    UNUSED(argc);
    UNUSED(argv);
    UNUSED(out_p);
    UNUSED(in_p);

    return (0);
}

static struct queue_t qout;
static char qoutbuf[BUFFER_SIZE];

#if defined(ARCH_LINUX)
static struct queue_t qin;
static char qinbuf[32];
#endif

static int test_init(struct harness_t *harness_p)
{
    queue_init(&qout, &qoutbuf[0], sizeof(qoutbuf));

#if defined(ARCH_LINUX)
    queue_init(&qin, &qinbuf[0], sizeof(qinbuf));
#endif

    /* Setup the commands. */
    BTASSERT(fs_command_init(&foo_bar, FSTR("/tmp/foo/bar"), tmp_foo_bar, NULL) == 0);
    BTASSERT(fs_command_register(&foo_bar) == 0);
    BTASSERT(fs_command_init(&bar, FSTR("/tmp/bar"), tmp_bar, NULL) == 0);
    BTASSERT(fs_command_register(&bar) == 0);

    /* Setup the counters. */
    BTASSERT(fs_counter_init(&my_counter, FSTR("/my/counter"), 0) == 0);
    BTASSERT(fs_counter_register(&my_counter) == 0);
    BTASSERT(fs_counter_init(&your_counter, FSTR("/your/counter"), 0) == 0);
    BTASSERT(fs_counter_register(&your_counter) == 0);

    /* Setup the parameter. */
    BTASSERT(fs_parameter_init(&our_parameter,
                               FSTR("/our/parameter"),
                               fs_parameter_int_set,
                               fs_parameter_int_print,
                               &our_parameter_value) == 0);
    BTASSERT(fs_parameter_register(&our_parameter) == 0);

    return (0);
}

static int test_auto_complete(struct harness_t *harness_p)
{
    char buf[CONFIG_FS_PATH_MAX];

    strcpy(buf, "/tmp/q");
    BTASSERT(fs_auto_complete(buf) == -ENOENT);

    strcpy(buf, "/tm");
    BTASSERT(fs_auto_complete(buf) == 2);
    BTASSERT(strcmp(buf, "/tmp/") == 0);

    strcpy(buf, "tm");
    BTASSERT(fs_auto_complete(buf) == 2);
    BTASSERT(strcmp(buf, "tmp/") == 0);

    strcpy(buf, "/tmp/f");
    BTASSERT(fs_auto_complete(buf) == 3);
    BTASSERT(strcmp(buf, "/tmp/foo/") == 0);

    strcpy(buf, "/tmp/foo/");
    BTASSERT(fs_auto_complete(buf) == 4);
    BTASSERT(strcmp(buf, "/tmp/foo/bar ") == 0);

    strcpy(buf, "");
    BTASSERT(fs_auto_complete(buf) == 0);
    BTASSERT(strcmp(buf, "") == 0);

    return (0);
}

static int test_command(struct harness_t *harness_p)
{
    char buf[256];

    strcpy(buf, "  /tmp/foo/bar     foo1 foo2  ");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout, "\n", NULL) > 0);

    strcpy(buf, "/tmp/bar 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -E2BIG);

    strcpy(buf, "/tmp/bar/missing");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -ENOCOMMAND);
    BTASSERT(harness_expect(&qout, "\n", NULL) > 0);

    strcpy(buf, "");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -1);

    return (0);
}

static int test_counter(struct harness_t *harness_p)
{
    char buf[384];

    strcpy(buf, "filesystems/fs/counters/list");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout,
                            "/your/counter                                        0000000000000000\r\n",
                            NULL) > 0);

    fs_counter_increment(&my_counter, 3);

    strcpy(buf, "my/counter");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout, "0000000000000003\r\n", NULL) > 0);

    strcpy(buf, "filesystems/fs/counters/reset");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);

    strcpy(buf, "my/counter");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout, "0000000000000000\r\n", NULL) > 0);

    return (0);
}

static int test_parameter(struct harness_t *harness_p)
{
    char buf[256];

    strcpy(buf, "filesystems/fs/parameters/list");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout,
                            "NAME                                                 VALUE\r\n"
                            "/our/parameter                                       5\r\n",
                            NULL) > 0);

    BTASSERT(our_parameter_value == OUR_PARAMETER_DEFAULT);
    our_parameter_value = 1;
    BTASSERT(our_parameter_value == 1);

    strcpy(buf, "/our/parameter");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout, "1\r\n", NULL) > 0);

    strcpy(buf, "/our/parameter 3");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);

    strcpy(buf, "/our/parameter");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout, "3\r\n", NULL) > 0);

    return (0);
}

static int test_list(struct harness_t *harness_p)
{
    BTASSERT(fs_list("filesystems", NULL, &qout) == 0);
    BTASSERT(harness_expect(&qout, "fs/\r\n", NULL) > 0);

    BTASSERT(fs_list("/filesystems", "f", &qout) == 0);
    BTASSERT(harness_expect(&qout, "fs/\r\n", NULL) > 0);

    BTASSERT(fs_list("tmp/foo", NULL, &qout) == 0);
    BTASSERT(harness_expect(&qout, "bar\r\n", NULL) > 0);

    BTASSERT(fs_list("", NULL, &qout) == 0);
    BTASSERT(harness_expect(&qout,
                            "filesystems/\r\n"
                            "kernel/\r\n"
                            "my/\r\n"
                            "our/\r\n"
                            "tmp/\r\n"
                            "your/\r\n",
                            NULL) > 0);

    BTASSERT(fs_list("", "o", &qout) == 0);
    BTASSERT(harness_expect(&qout, "our/\r\n", NULL) > 0);

    return (0);
}

static int test_split_merge(struct harness_t *harness_p)
{
    char buf[256];
    char *path_p;
    char * command_p;

    /* Split "/foo/fie/bar" into "/foo/fie" and "bar". */
    strcpy(buf, "/foo/fie/bar");
    fs_split(buf, &path_p, &command_p);

    BTASSERT(path_p == &buf[0]);
    BTASSERT(strcmp(path_p, "/foo/fie") == 0);
    BTASSERT(command_p == &buf[9]);
    BTASSERT(strcmp(command_p, "bar") == 0);

    /* Merge "/foo/fie" and "bar" to "/foo/fie/bar". */
    fs_merge(path_p, command_p);

    BTASSERT(strcmp(buf, "/foo/fie/bar") == 0);

    /* Split "bar" into "" and "bar". */
    strcpy(buf, "bar");
    fs_split(buf, &path_p, &command_p);

    BTASSERT(strcmp(path_p, "") == 0);
    BTASSERT(command_p == &buf[0]);
    BTASSERT(strcmp(command_p, "bar") == 0);

    /* Merge "" and "bar" to "bar". */
    fs_merge(path_p, command_p);

    BTASSERT(strcmp(buf, "bar") == 0);

    /* Split "/bar" into "" and "bar". */
    strcpy(buf, "/bar");
    fs_split(buf, &path_p, &command_p);

    BTASSERT(strcmp(path_p, "") == 0);
    BTASSERT(command_p == &buf[1]);
    BTASSERT(strcmp(command_p, "bar") == 0);

    /* Merge "" and "bar" to "/bar". */
    fs_merge(path_p, command_p);

    BTASSERT(strcmp(buf, "/bar") == 0);

    return (0);
}

static int test_quotes(struct harness_t *harness_p)
{
    char buf[64];

    /* "1 2" is parsed as one argument. */
    strcpy(buf, "tmp/foo/bar \"1 2\" 3");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout, "argv[1] = 1 2, argv[2] = 3\n", NULL) > 0);

    /* "1""" is parsed as one argument. */
    strcpy(buf, "tmp/foo/bar \"1\"\"\" 2");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout, "argv[1] = 1, argv[2] = 2\n", NULL) > 0);

    /* "1 fails because the end " is missing. */
    strcpy(buf, "tmp/foo/bar \"1 2");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -1);

    return (0);
}

static int test_escape(struct harness_t *harness_p)
{
    char buf[64];

    /* \"2\" parsed as "2". */
    strcpy(buf, "tmp/foo/bar 1 \\\"2\\\"");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout,
                            "argv[1] = 1, argv[2] = \"2\"\n",
                            NULL) > 0);

    return (0);
}

static int test_filesystem_fat16(struct harness_t *harness_p)
{
#if defined(ARCH_LINUX)

    char buf[32];
    struct fs_file_t file;
    struct fs_stat_t stat;
    struct fs_dir_t dir;
    struct fs_dir_entry_t entry;

    /* Initialize a FAT16 file system in RAM, format and mount it. */
    BTASSERT(fat16_init(&fat16_fs,
                        filesystem_fat16_read_block,
                        filesystem_fat16_write_block,
                        NULL,
                        0) == 0);
    BTASSERT(fat16_format(&fat16_fs) == 0);
    BTASSERT(fat16_mount(&fat16_fs) == 0);

    /* Register the FAT16 file system in the fs module. */
    BTASSERT(fs_filesystem_init_fat16(&fat16fs,
                                      "/fat16fs",
                                      &fat16_fs) == 0);
    BTASSERT(fs_filesystem_register(&fat16fs) == 0);

    /* Perform file operations. */
    BTASSERT(fs_open(&file, "/fat16fs/foo.txt", FS_CREAT | FS_RDWR | FS_SYNC) == 0);
    BTASSERT(fs_write(&file, "hello!", 6) == 6);
    BTASSERT(fs_seek(&file, 0, FS_SEEK_SET) == 0);
    BTASSERT(fs_read(&file, buf, 6) == 6);
    BTASSERT(fs_read(&file, buf, 1) == 0);
    BTASSERT(memcmp(buf, "hello!", 6) == 0);
    BTASSERT(fs_tell(&file) == 6);
    BTASSERT(fs_close(&file) == 0);

    /* Try to open a file outside the file system. */
    BTASSERT(fs_open(&file, "/foo.txt", FS_CREAT | FS_RDWR | FS_SYNC) == -1);

    /* Create an empty directory called mkdir. */
    BTASSERT(fs_mkdir("/fat16fs/mkdir") == 0);

    /* Stat the empty directory. */
    BTASSERT(fs_stat("/fat16fs/mkdir", &stat) == 0);
    BTASSERT(stat.size == 64);
    BTASSERT(stat.type == 2);

    /* List all files in the root directory. */
    BTASSERT(fs_dir_open(&dir, "/fat16fs", FS_READ) == 0);

    BTASSERT(fs_dir_read(&dir, &entry) == 1);
    BTASSERT(strcmp(entry.name, "FOO.TXT") == 0);
    BTASSERT(entry.type == FS_TYPE_FILE);

    BTASSERT(fs_dir_read(&dir, &entry) == 1);
    BTASSERT(strcmp(entry.name, "MKDIR") == 0);
    BTASSERT(entry.type == FS_TYPE_DIR);

    BTASSERT(fs_dir_read(&dir, &entry) == 0);

    BTASSERT(fs_dir_close(&dir) == 0);

    return (0);

#else

    return (1);

#endif
}

static int test_filesystem_spiffs(struct harness_t *harness_p)
{
#if defined(ARCH_LINUX)

    char buf[32];
    struct fs_file_t file;
    struct fs_stat_t stat;
    struct fs_dir_t dir;
    struct fs_dir_entry_t entry;

    /* Initiate the config struct. */
    config.hal_read_f = filesystem_spiffs_read;
    config.hal_write_f = filesystem_spiffs_write;
    config.hal_erase_f = filesystem_spiffs_erase;
    config.phys_size = PHY_SIZE;
    config.phys_addr = PHY_ADDR;
    config.phys_erase_block = PHYS_ERASE_BLOCK;
    config.log_block_size = LOG_BLOCK_SIZE;
    config.log_page_size = LOG_PAGE_SIZE;

    /* Mount the file system to initialize the runtime variables. */
    BTASSERT(spiffs_mount(&spiffs_fs,
                          &config,
                          workspace,
                          fdworkspace,
                          sizeof(fdworkspace),
                          cache,
                          sizeof(cache),
                          NULL) != 0);

    /* Format and mount the file system again. */
    BTASSERT(spiffs_format(&spiffs_fs) == 0);
    BTASSERT(spiffs_mount(&spiffs_fs,
                          &config,
                          workspace,
                          fdworkspace,
                          sizeof(fdworkspace),
                          cache,
                          sizeof(cache),
                          NULL) == 0);

    /* Register the SPIFFS file system in the fs module. */
    configfs.config_p = &config;
    configfs.workspace_p = workspace;
    configfs.fdworkspace.buf_p = fdworkspace;
    configfs.fdworkspace.size = sizeof(fdworkspace);
    configfs.cache.buf_p = cache;
    configfs.cache.size = sizeof(cache);
    BTASSERT(fs_filesystem_init_spiffs(&spiffsfs,
                                       "/spiffsfs",
                                       &spiffs_fs,
                                       &configfs) == 0);
    BTASSERT(fs_filesystem_register(&spiffsfs) == 0);

    /* Perform file operations. */
    BTASSERT(fs_open(&file, "/spiffsfs/foo.txt", FS_CREAT | FS_RDWR | FS_SYNC) == 0);
    BTASSERT(fs_write(&file, "hello!", 6) == 6);
    BTASSERT(fs_seek(&file, 0, FS_SEEK_SET) == 0);
    BTASSERT(fs_read(&file, buf, 6) == 6);
    BTASSERT(fs_read(&file, buf, 1) == 0);
    BTASSERT(memcmp(buf, "hello!", 6) == 0);
    BTASSERT(fs_tell(&file) == 6);
    BTASSERT(fs_close(&file) == 0);

    /* Stat the file foo.txt. */
    BTASSERT(fs_stat("/spiffsfs/foo.txt", &stat) == 0);
    BTASSERT(stat.size == 6);
    BTASSERT(stat.type == 1);

    /* Fail to create an empty directory called mkdir. */
    BTASSERT(fs_mkdir("/spiffsfs/mkdir") == -1);

    /* List all files in the root directory. */
    BTASSERT(fs_dir_open(&dir, "/spiffsfs", FS_READ) == 0);

    BTASSERT(fs_dir_read(&dir, &entry) == 1);
    BTASSERT(strcmp(entry.name, "foo.txt") == 0);
    BTASSERT(entry.type == FS_TYPE_FILE);

    BTASSERT(fs_dir_read(&dir, &entry) == 0);

    BTASSERT(fs_dir_close(&dir) == 0);

    /* Remove the file 'foo.txt'. */
    BTASSERT(fs_remove("/spiffsfs/foo.txt") == 0);

    return (0);

#else

    return (1);

#endif
}

static int test_filesystem_generic(struct harness_t *harness_p)
{
    char buf[32];
    struct fs_file_t file;

    /* Create and register the tftp file system used to write received
       software to the application area. */
    BTASSERT(fs_filesystem_init_generic(&genericfs,
                                        "/generic",
                                        &generic_ops) == 0);
    BTASSERT(fs_filesystem_register(&genericfs) == 0);

    /* Perform file operations. */
    BTASSERT(fs_open(&file,
                     "/generic/foo.txt",
                     FS_CREAT | FS_RDWR | FS_SYNC) == 0);
    BTASSERT(fs_write(&file, "hello!", 6) == 6);
    BTASSERT(fs_seek(&file, 0, FS_SEEK_SET) == 0);
    BTASSERT(fs_read(&file, buf, 6) == 6);
    BTASSERT(fs_close(&file) == 0);

    return (0);
}

static int test_filesystem(struct harness_t *harness_p)
{
#if defined(ARCH_LINUX)

    struct fs_stat_t stat;

    /* Stat a file that does not exist. */
    BTASSERT(fs_stat("/apa", &stat) == -1);
    BTASSERT(fs_stat("/fat16fs/apa", &stat) == -1);
    BTASSERT(fs_stat("/spiffsfs/apa", &stat) == -1);

    return (0);

#else

    return (1);

#endif
}

static int test_filesystem_commands(struct harness_t *harness_p)
{
#if defined(ARCH_LINUX)

    char buf[64];

    strcpy(buf, "/filesystems/fs/filesystems/list");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout,
                            "MOUNT-POINT                    MEDIUM   TYPE     AVAILABLE  SIZE  USAGE\r\n"
                            "/generic                       -        generic          -     -     -%\r\n"
                            "/spiffsfs                      -        spiffs           -     -     -%\r\n"
                            "/fat16fs                       -        fat16            -     -     -%\r\n",
                            NULL) > 0);

    /* Bad arguments. */
    strcpy(buf, "/filesystems/fs/read");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -1);
    BTASSERT(harness_expect(&qout, "Usage: read <file>\r\n", NULL) > 0);

    strcpy(buf, "/filesystems/fs/write");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -1);
    BTASSERT(harness_expect(&qout,
                            "Usage: write <file> [<data>]\r\n",
                            NULL) > 0);

    strcpy(buf, "/filesystems/fs/append");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -1);
    BTASSERT(harness_expect(&qout,
                            "Usage: append <file> <data>\r\n",
                            NULL) > 0);

    strcpy(buf, "/filesystems/fs/list");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -1);
    BTASSERT(harness_expect(&qout, "Usage: list <path>\r\n", NULL) > 0);

    /* Non-existing file. */
    strcpy(buf, "/filesystems/fs/read spiffsfs/cmd.txt");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -1);
    BTASSERT(harness_expect(&qout,
                            "Failed to open spiffsfs/cmd.txt.\r\n",
                            NULL) > 0);

    strcpy(buf, "/filesystems/fs/append spiffsfs/cmd.txt a");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -1);
    BTASSERT(harness_expect(&qout,
                            "Failed to open spiffsfs/cmd.txt.\r\n",
                            NULL) > 0);

    /* Write, append and read. */
    strcpy(buf, "/filesystems/fs/write spiffsfs/cmd.txt 1");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);

    strcpy(buf, "/filesystems/fs/append spiffsfs/cmd.txt 2");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);

    strcpy(buf, "/filesystems/fs/read spiffsfs/cmd.txt");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout, "12", NULL) > 0);

    /* Write in paste mode, and read. */
    chan_write(&qin, "First\r\nSecond\r\n\x04", 16);
    strcpy(buf, "/filesystems/fs/write spiffsfs/cmd.txt");
    BTASSERT(fs_call(buf, &qin, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout,
                            "Reading, press Ctrl-D when done.\r\n",
                            NULL) > 0);
    BTASSERT(harness_expect(&qout,
                            "Wrote 15 bytes to spiffsfs/cmd.txt.\r\n",
                            NULL) > 0);

    strcpy(buf, "/filesystems/fs/read spiffsfs/cmd.txt");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout, "First\r\nSecond\r\n", NULL) > 0);

    /* Truncate existing file. */
    strcpy(buf, "/filesystems/fs/write spiffsfs/cmd.txt 1");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);

    strcpy(buf, "/filesystems/fs/read spiffsfs/cmd.txt");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout, "1", NULL) > 0);

    /* List all files in the FAT16 file system. */
    strcpy(buf, "/filesystems/fs/list fat16fs");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout,
                            "2000-01-01 01:00        6 FOO.TXT\r\n",
                            NULL) > 0);

    /* List all files in the SPIFFS file system. */
    strcpy(buf, "/filesystems/fs/list spiffsfs");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);
    BTASSERT(harness_expect(&qout,
                            "xxxx-xx-xx xx-xx        1 cmd.txt\r\n",
                            NULL) > 0);

    /* Remove a file. */
    strcpy(buf, "/filesystems/fs/remove spiffsfs/cmd.txt");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == 0);

    /* Remove a non-existing file. */
    strcpy(buf, "/filesystems/fs/remove spiffsfs/cmd.txt");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -1);

    /* Bad arguments. */
    strcpy(buf, "/filesystems/fs/remove");
    BTASSERT(fs_call(buf, NULL, &qout, NULL) == -1);
    BTASSERT(harness_expect(&qout, "Usage: remove <file>\r\n", NULL) > 0);

    return (0);

#else

    return (1);

#endif
}

static int test_read_line(struct harness_t *harness_p)
{
#if defined(ARCH_LINUX)

    struct fs_file_t file;
    char line[16];

    /* Write a few lines to a file. */
    BTASSERT(fs_open(&file, "/spiffsfs/lines.txt", FS_CREAT | FS_RDWR | FS_SYNC) == 0);

    BTASSERT(fs_write(&file, "line\n", 5) == 5);
    BTASSERT(fs_write(&file, "\x22\0\n", 3) == 3);
    BTASSERT(fs_write(&file, "12345678901234567890\n", 21) == 21);
    BTASSERT(fs_write(&file, "\n", 1) == 1);
    BTASSERT(fs_write(&file, "no newline", 10) == 10);

    BTASSERT(fs_close(&file) == 0);

    /* Re-open the file and read one line at a time. */
    BTASSERT(fs_open(&file, "/spiffsfs/lines.txt", FS_READ) == 0);

    /* A human readable string. */
    BTASSERT(fs_read_line(&file, line, sizeof(line)) == 4);
    BTASSERT(strcmp(line, "line") == 0);

    /* Non-printable data. */
    BTASSERT(fs_read_line(&file, line, sizeof(line)) == 2);
    BTASSERT(memcmp(line, "\x22\0\0", 3) == 0);

    /* Destination buffer too small. */
    BTASSERT(fs_read_line(&file, line, sizeof(line)) == sizeof(line));

    /* Read remaining part of the line. */
    BTASSERT(fs_read_line(&file, line, sizeof(line)) == 4);
    BTASSERT(strcmp(line, "7890") == 0);

    /* Empty line. */
    BTASSERT(fs_read_line(&file, line, sizeof(line)) == 0);
    BTASSERT(strcmp(line, "") == 0);

    /* No newline at end of file. */
    BTASSERT(fs_read_line(&file, line, sizeof(line)) == 10);
    BTASSERT(strcmp(line, "no newline") == 0);

    /* End of file reached. */
    BTASSERT(fs_read_line(&file, line, sizeof(line)) == -1);

    BTASSERT(fs_close(&file) == 0);

    return (0);

#else

    return (1);

#endif
}

static int test_cwd(struct harness_t *harness_p)
{
#if defined(ARCH_LINUX)

    struct fs_file_t file;
    struct thrd_environment_variable_t variables[2];

    BTASSERT(thrd_init_env(variables, membersof(variables)) == 0);

    /* CWD without terminating slash. */
    BTASSERT(thrd_set_env("CWD", "/spiffsfs") == 0);
    BTASSERT(fs_open(&file, "lines.txt", FS_CREAT | FS_RDWR | FS_SYNC) == 0);
    BTASSERT(fs_close(&file) == 0);

    /* CWD with terminating slash. */
    BTASSERT(thrd_set_env("CWD", "/spiffsfs/") == 0);
    BTASSERT(fs_open(&file, "lines.txt", FS_CREAT | FS_RDWR | FS_SYNC) == 0);
    BTASSERT(fs_close(&file) == 0);

    /* CWD set but absolute path given. */
    BTASSERT(thrd_set_env("CWD", "/spiffsfs") == 0);
    BTASSERT(fs_open(&file, "/spiffsfs/lines.txt", FS_CREAT | FS_RDWR | FS_SYNC) == 0);
    BTASSERT(fs_close(&file) == 0);

    /* CWD as empty string. */
    BTASSERT(thrd_set_env("CWD", "") == 0);
    BTASSERT(fs_open(&file, "spiffsfs/lines.txt", FS_CREAT | FS_RDWR | FS_SYNC) == 0);

    /* Bad CWD. */
    BTASSERT(thrd_set_env("CWD", "/foo") == 0);
    BTASSERT(fs_open(&file, "lines.txt", FS_CREAT | FS_RDWR | FS_SYNC) == -1);

    /* Too long cwd. */
    BTASSERT(thrd_set_env("CWD", "/12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890") == 0);
    BTASSERT(fs_open(&file, "lines.txt", FS_CREAT | FS_RDWR | FS_SYNC) == -1);

    return (0);

#else

    return (1);

#endif
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_auto_complete, "test_auto_complete" },
        { test_command, "test_command" },
        { test_counter, "test_counter" },
        { test_parameter, "test_parameter" },
        { test_list, "test_list" },
        { test_split_merge, "test_split_merge" },
        { test_quotes, "test_quotes" },
        { test_escape, "test_escape" },
        { test_filesystem_fat16, "test_filesystem_fat16" },
        { test_filesystem_spiffs, "test_filesystem_spiffs" },
        { test_filesystem_generic, "test_filesystem_generic" },
        { test_filesystem, "test_filesystem" },
        { test_filesystem_commands, "test_filesystem_commands" },
        { test_read_line, "test_read_line" },
        { test_cwd, "test_cwd" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
