/**
 * @file main.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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
#include "music_player.h"

#define FIRST_SONG_NUMBER     1

FS_COMMAND_DEFINE("/list", cmd_list);
FS_COMMAND_DEFINE("/play", cmd_play);
FS_COMMAND_DEFINE("/stop", cmd_stop);
FS_COMMAND_DEFINE("/repeat", cmd_repeat);

struct song_t {
    int number;
    char name[16];
    int minutes;
    int seconds;
};

static char qinbuf[32];
static struct uart_driver_t uart;
static struct shell_args_t shell_args;

static struct spi_driver_t spi;
static struct sd_driver_t sd;
static struct fat16_t fs;
static struct dac_driver_t dac;
static struct music_player_t music_player;
static struct hash_map_t song_map;
static struct hash_map_bucket_t buckets[64];
static struct hash_map_entry_t entries[128];
static struct song_t songs[128];
static struct sem_t sem;
static long current_song;
static int repeat = 0;

/**
 * Hash song number.
 */
static int hash_number(long key)
{
    return ((key << 7) ^ (key << 3) ^ key);
}

/**
 * Music player callback to get the current song path.
 */
static const char *get_current_song_path(void *arg_p)
{
    struct song_t *song_p;

    sem_get(&sem, NULL);
    song_p = hash_map_get(&song_map, current_song);
    sem_put(&sem, 1);

    return (song_p->name);
}

/**
 * Music player callback to get the next song path.
 */
static const char *get_next_song_path(void *arg_p)
{
    struct song_t *song_p;

    sem_get(&sem, NULL);

    /* Increment current song. */
    if (repeat == 0) {
        current_song++;
    }

    song_p = hash_map_get(&song_map, current_song);
    sem_put(&sem, 1);

    if (song_p != NULL) {
        return (song_p->name);
    } else {
        current_song = FIRST_SONG_NUMBER;
        return (NULL);
    }
}

int cmd_list(int argc,
             const char *argv[],
             void *out_p,
             void *in_p)
{
    struct song_t *song_p;
    int number;

    if (argc != 1) {
        std_fprintf(out_p, FSTR("Usage: %s\r\n"), argv[0]);

        return (1);
    }

    /* Write the header. */
    std_fprintf(out_p,
                FSTR("NUMBER            NAME  LENGTH\r\n"));

    /* Iterate over all songs in the ordered hash map. */
    for (number = FIRST_SONG_NUMBER;
         ((song_p = hash_map_get(&song_map, number)) != NULL);
         number++) {
        std_fprintf(out_p,
                    FSTR("%6d %15s %4d:%02d\r\n"),
                    song_p->number,
                    song_p->name,
                    song_p->minutes,
                    song_p->seconds);
    }

    return (0);
}

int cmd_play(int argc,
             const char *argv[],
             void *out_p,
             void *in_p)
{
    long song_number;
    struct song_t *song_p;

    if (argc > 2) {
        std_fprintf(out_p, FSTR("Usage: %s [<song number>]\r\n"), argv[0]);

        return (-EINVAL);
    }

    if (argc == 2) {
        if (std_strtol(argv[1], &song_number) != 0) {
            return (-EINVAL);
        }

        /* Find the song in the hash map. */
        song_p = hash_map_get(&song_map, song_number);

        if (song_p == NULL) {
            return (-EINVAL);
        }

        /* Stop the current song and set the new current song. */
        music_player_song_stop(&music_player);
        sem_get(&sem, NULL);
        current_song = song_number;
        sem_put(&sem, 1);
    }

    /* Play the song or resume it if it's paused. */
    return (music_player_song_play(&music_player));
}

int cmd_pause(int argc,
              const char *argv[],
              void *out_p,
              void *in_p)
{
    if (argc != 1) {
        std_fprintf(out_p, FSTR("Usage: %s\r\n"), argv[0]);

        return (-EINVAL);
    }

    return (music_player_song_pause(&music_player));
}

int cmd_stop(int argc,
             const char *argv[],
             void *out_p,
             void *in_p)
{
    if (argc != 1) {
        std_fprintf(out_p, FSTR("Usage: %s\r\n"), argv[0]);

        return (-EINVAL);
    }

    return (music_player_song_stop(&music_player));
}

int cmd_repeat(int argc,
               const char *argv[],
               void *out_p,
               void *in_p)
{
    if (argc != 1) {
        std_fprintf(out_p, FSTR("Usage: %s\r\n"), argv[0]);

        return (-EINVAL);
    }

    repeat ^= 1;

    return (0);
}

static void init(void)
{
    long number;
    struct fat16_dir_t dir;
    struct fat16_dir_entry_t entry;
    struct song_t *song_p;
    uint32_t seconds;

    sys_start();
    uart_module_init();

    uart_init(&uart, &uart_device[0], 38400, qinbuf, sizeof(qinbuf));
    uart_start(&uart);

    sys_set_stdout(&uart.chout);

    std_printf(sys_get_info());

    spi_init(&spi,
             &spi_device[0],
             &pin_d53_dev,
             SPI_MODE_MASTER,
             SPI_SPEED_1MBPS,
             0,
             1);

    sd_init(&sd, &spi);
    sd_start(&sd);
    std_printf(FSTR("sd card started\r\n"));
    fat16_init(&fs, &sd, 0);
    fat16_start(&fs);
    std_printf(FSTR("fat16 started\r\n"));

    dac_init(&dac,
             &dac_0_dev,
             &pin_dac0_dev,
             &pin_dac1_dev,
             2 * 11025);

    hash_map_init(&song_map,
                  buckets,
                  membersof(buckets),
                  entries,
                  membersof(entries),
                  hash_number);

    sem_init(&sem, 1);

    music_player_init(&music_player,
                      &fs,
                      &dac,
                      get_current_song_path,
                      get_next_song_path,
                      NULL);

    /* Initialize the song number. */
    current_song = FIRST_SONG_NUMBER;
    number = FIRST_SONG_NUMBER;

    /* Add songs to the hash map. */
    fat16_dir_open(&fs, &dir, ".", O_READ);

    while (fat16_dir_read(&dir, &entry) == 1) {
        if (number - FIRST_SONG_NUMBER == membersof(songs)) {
            std_printf("Maximum number of songs already added. "
                       "Skipping the rest of the songs.\r\n");
            break;
        }

        /* Skip folders. */
        if (entry.is_dir == 1) {
            continue;
        }

        std_printf("Adding song %s to playlist.\r\n", entry.name);
        song_p = &songs[number - FIRST_SONG_NUMBER];

        /* Initialize the song entry. */
        song_p->number = number;
        strcpy(song_p->name, entry.name);
        seconds = (entry.size / 2 / 11025);
        song_p->minutes = (seconds / 60);
        song_p->seconds = (seconds % 60);

        hash_map_add(&song_map, number, song_p);
        number++;
    }

    fat16_dir_close(&dir);

    music_player_start(&music_player);
}

int main()
{
    init();

    shell_args.chin_p = &uart.chin;
    shell_args.chout_p = &uart.chout;
    shell_args.username_p = NULL;
    shell_args.password_p = NULL;
    shell_entry(&shell_args);

    return (0);
}
