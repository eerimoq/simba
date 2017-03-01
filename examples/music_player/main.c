/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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
#include "music_player.h"

#define FIRST_SONG_NUMBER       1

/* Music player events. */
#define EVENT_BUTTON_PLAY       0x1
#define EVENT_BUTTON_NEXT       0x2
#define EVENT_BUTTON_PREV       0x4
#define EVENT_BUTTON_STOP       0x8

static struct fs_command_t cmd_list;
static struct fs_command_t cmd_play;
static struct fs_command_t cmd_pause;
static struct fs_command_t cmd_next;
static struct fs_command_t cmd_prev;
static struct fs_command_t cmd_stop;
static struct fs_command_t cmd_repeat;
static struct fs_command_t cmd_set_bits_per_sample;

struct song_t {
    int number;
    char name[16];
    int minutes;
    int seconds;
};

static char qinbuf[32];
static struct uart_driver_t uart;
static struct shell_t shell;

static struct exti_driver_t buttons[4];
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
static int last_song_number = -1;
static struct event_t event;
static THRD_STACK(shell_stack, 1024);

#if defined(MUSIC_PLAYER_STORAGE_SD)
#    define SAMPLES_PER_SOCOND  22050

static struct spi_driver_t spi;
static struct sd_driver_t sd;

static int storage_init(fat16_read_t *read_p,
                        fat16_write_t *write_p,
                        void **arg_pp)
{
    std_printf(FSTR("SD storage.\r\n"));

    std_printf(FSTR("spi bitrate = %lu kbps\r\n"),
               2 * 16 * SAMPLES_PER_SOCOND / 1024);

    /* Initialize SPI for the SD card. */
    spi_init(&spi,
             &spi_device[0],
             &pin_d53_dev,
             SPI_MODE_MASTER,
             SPI_SPEED_2MBPS,
             0,
             1);

    sd_init(&sd, &spi);

    if (sd_start(&sd) != 0) {
        return (-1);
    }

    std_printf(FSTR("sd card started\r\n"));

    *read_p = (fat16_read_t)sd_read_block;
    *write_p = (fat16_write_t)sd_write_block;
    *arg_pp = &sd;

    return (0);
}

#elif defined(MUSIC_PLAYER_STORAGE_USB)
#    define SAMPLES_PER_SOCOND  44100

static struct usb_host_driver_t usb;
static struct usb_host_device_t host_devices[1];

static struct usb_host_class_mass_storage_driver_t mass_storage;
static struct usb_host_class_mass_storage_device_t mass_storage_devices[1];

static ssize_t read_block(void *arg_p,
                          void *dst_p,
                          uint32_t src_block)
{
    return (usb_host_class_mass_storage_device_read(arg_p,
                                                    dst_p,
                                                    512 * src_block,
                                                    512));
}

static ssize_t write_block(void *arg_p,
                           uint32_t dst_block,
                           const void *src_p)
{
    std_printf(FSTR("write block not supported.\r\n"));

    return (-1);
}

static int storage_init(fat16_read_t *read_p,
                        fat16_write_t *write_p,
                        void **arg_pp)
{
    struct usb_host_device_t *device_p;
    union usb_message_t message;

    std_printf(FSTR("USB storage.\r\n"));

    /* Initialize the USB host driver. */
    usb_host_init(&usb,
                  &usb_device[0],
                  host_devices,
                  membersof(host_devices));

    usb_host_class_mass_storage_init(&mass_storage,
                                     &usb,
                                     mass_storage_devices,
                                     membersof(mass_storage_devices));
    usb_host_class_mass_storage_start(&mass_storage);

    /* Start the USB driver. */
    usb_host_start(&usb);

    chan_read(&usb.control, &message, sizeof(message));

    std_printf(FSTR("The USB control thread read a message of type %d\r\n"),
               message.header.type);

    if (message.header.type != USB_MESSAGE_TYPE_ADD) {
        std_printf(FSTR("bad message type %d\r\n"), message.header.type);
        return (-1);
    }

    device_p = usb_host_device_open(&usb, message.add.device);

    *read_p = read_block;
    *write_p = write_block;
    *arg_pp = device_p;

    return (0);

}
#else
#    error "Unsupported storage type."
#endif

/**
 * Button pressed callbacks.
 */
static void on_button_play(void *arg_p)
{
    uint32_t mask = EVENT_BUTTON_PLAY;

    event_write_isr(&event, &mask, sizeof(mask));
}

static void on_button_next(void *arg_p)
{
    uint32_t mask = EVENT_BUTTON_NEXT;

    event_write_isr(&event, &mask, sizeof(mask));
}

static void on_button_prev(void *arg_p)
{
    uint32_t mask = EVENT_BUTTON_PREV;

    event_write_isr(&event, &mask, sizeof(mask));
}

static void on_button_stop(void *arg_p)
{
    uint32_t mask = EVENT_BUTTON_STOP;

    event_write_isr(&event, &mask, sizeof(mask));
}

static int next()
{
    music_player_song_stop(&music_player);

    /* Increment current song. */
    sem_take(&sem, NULL);
    current_song++;

    if (hash_map_get(&song_map, current_song) == NULL) {
        current_song = FIRST_SONG_NUMBER;
    }

    sem_give(&sem, 1);

    return (music_player_song_play(&music_player));
}

static int prev()
{
    music_player_song_stop(&music_player);

    /* Increment current song. */
    sem_take(&sem, NULL);

    if (current_song == FIRST_SONG_NUMBER) {
        current_song = last_song_number;
    } else {
        current_song--;
    }

    sem_give(&sem, 1);

    return (music_player_song_play(&music_player));
}

static int handle_event_play()
{
    return (music_player_song_play(&music_player));
}

static int handle_event_next()
{
    return (next());
}

static int handle_event_prev()
{
    return (prev());
}

static int handle_event_stop()
{
    return (music_player_song_stop(&music_player));
}

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

    sem_take(&sem, NULL);
    song_p = hash_map_get(&song_map, current_song);
    sem_give(&sem, 1);

    return (song_p->name);
}

/**
 * Music player callback to get the next song path.
 */
static const char *get_next_song_path(void *arg_p)
{
    struct song_t *song_p;

    sem_take(&sem, NULL);

    /* Increment current song. */
    if (repeat == 0) {
        current_song++;
    }

    song_p = hash_map_get(&song_map, current_song);
    sem_give(&sem, 1);

    if (song_p != NULL) {
        return (song_p->name);
    } else {
        current_song = FIRST_SONG_NUMBER;
        return (NULL);
    }
}

static int cmd_list_cb(int argc,
                       const char *argv[],
                       void *out_p,
                       void *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    struct song_t *song_p;
    int number;

    if (argc != 1) {
        std_fprintf(out_p, OSTR("Usage: %s\r\n"), argv[0]);

        return (1);
    }

    /* Write the header. */
    std_fprintf(out_p,
                OSTR("NUMBER            NAME  LENGTH\r\n"));

    /* Iterate over all songs in the ordered hash map. */
    for (number = FIRST_SONG_NUMBER;
         ((song_p = hash_map_get(&song_map, number)) != NULL);
         number++) {
        std_fprintf(out_p,
                    OSTR("%6d %15s %4d:%02d\r\n"),
                    song_p->number,
                    song_p->name,
                    song_p->minutes,
                    song_p->seconds);
    }

    return (0);
}

static int cmd_play_cb(int argc,
                       const char *argv[],
                       void *out_p,
                       void *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    long song_number;
    struct song_t *song_p;

    if (argc > 2) {
        std_fprintf(out_p, OSTR("Usage: %s [<song number>]\r\n"), argv[0]);

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
        sem_take(&sem, NULL);
        current_song = song_number;
        sem_give(&sem, 1);
    }

    /* Play the song or resume it if it's paused. */
    return (music_player_song_play(&music_player));
}

static int cmd_pause_cb(int argc,
                        const char *argv[],
                        void *out_p,
                        void *in_p,
                        void *arg_p,
                        void *call_arg_p)
{
    if (argc != 1) {
        std_fprintf(out_p, OSTR("Usage: %s\r\n"), argv[0]);

        return (-EINVAL);
    }

    return (music_player_song_pause(&music_player));
}

static int cmd_next_cb(int argc,
                       const char *argv[],
                       void *out_p,
                       void *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    if (argc != 1) {
        std_fprintf(out_p, OSTR("Usage: %s\r\n"), argv[0]);

        return (-EINVAL);
    }

    return (next());
}

static int cmd_prev_cb(int argc,
                       const char *argv[],
                       void *out_p,
                       void *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    if (argc != 1) {
        std_fprintf(out_p, OSTR("Usage: %s\r\n"), argv[0]);

        return (-EINVAL);
    }

    return (prev());
}

static int cmd_stop_cb(int argc,
                       const char *argv[],
                       void *out_p,
                       void *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    if (argc != 1) {
        std_fprintf(out_p, OSTR("Usage: %s\r\n"), argv[0]);

        return (-EINVAL);
    }

    return (music_player_song_stop(&music_player));
}

static int cmd_repeat_cb(int argc,
                         const char *argv[],
                         void *out_p,
                         void *in_p,
                         void *arg_p,
                         void *call_arg_p)
{
    if (argc != 1) {
        std_fprintf(out_p, OSTR("Usage: %s\r\n"), argv[0]);

        return (-EINVAL);
    }

    repeat ^= 1;

    return (0);
}

static int cmd_set_bits_per_sample_cb(int argc,
                                      const char *argv[],
                                      void *out_p,
                                      void *in_p,
                                      void *arg_p,
                                      void *call_arg_p)
{
    long bits_per_sample;

    if (argc != 2) {
        std_fprintf(out_p, OSTR("Usage: %s <number of bits>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &bits_per_sample) != 0) {
        std_fprintf(out_p, OSTR("Usage: %s <number of bits>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    if ((bits_per_sample < 0) || (bits_per_sample > 12)) {
        std_printf(OSTR("The number of bits per sample bust be "
                        "an interger from 0 to 12.\r\n"));

        return (-EINVAL);
    }

    return (music_player_set_bits_per_sample(&music_player,
                                             bits_per_sample));
}

static void init(void)
{
    long number;
    struct fat16_dir_t dir;
    struct fat16_dir_entry_t entry;
    struct song_t *song_p;
    uint32_t seconds;
    fat16_read_t read;
    fat16_write_t write;
    void * arg_p;

    sys_start();
    uart_module_init();

    uart_init(&uart, &uart_device[0], 38400, qinbuf, sizeof(qinbuf));
    uart_start(&uart);

    sys_set_stdout(&uart.chout);

    std_printf(sys_get_info());

    fs_command_init(&cmd_list, CSTR("/list"), cmd_list_cb, NULL);
    fs_command_register(&cmd_list);
    fs_command_init(&cmd_play, CSTR("/play"), cmd_play_cb, NULL);
    fs_command_register(&cmd_play);
    fs_command_init(&cmd_pause, CSTR("/pause"), cmd_pause_cb, NULL);
    fs_command_register(&cmd_pause);
    fs_command_init(&cmd_next, CSTR("/next"), cmd_next_cb, NULL);
    fs_command_register(&cmd_next);
    fs_command_init(&cmd_prev, CSTR("/prev"), cmd_prev_cb, NULL);
    fs_command_register(&cmd_prev);
    fs_command_init(&cmd_stop, CSTR("/stop"), cmd_stop_cb, NULL);
    fs_command_register(&cmd_stop);
    fs_command_init(&cmd_repeat, CSTR("/repeat"), cmd_repeat_cb, NULL);
    fs_command_register(&cmd_repeat);
    fs_command_init(&cmd_set_bits_per_sample,
                    CSTR("/set_bits_per_sample"),
                    cmd_set_bits_per_sample_cb,
                    NULL);
    fs_command_register(&cmd_set_bits_per_sample);

    if (storage_init(&read, &write, &arg_p) != 0) {
        std_printf(FSTR("storage init failed\r\n"));
        return;
    }

    std_printf(FSTR("initializing fat16\r\n"));
    fat16_init(&fs, read, write, arg_p, 0);

    std_printf(FSTR("fat16 initialized\r\n"));

    if (fat16_mount(&fs) != 0) {
        std_printf(FSTR("failed to mount fat16\r\n"));
        return;
    }

    std_printf(FSTR("fat16 mounted\r\n"));

    event_init(&event);

    exti_module_init();

    /* Initialize the buttons. */
    exti_init(&buttons[0],
              &exti_d18_dev,
              EXTI_TRIGGER_FALLING_EDGE,
              on_button_play,
              NULL);
    exti_start(&buttons[0]);
    exti_init(&buttons[1],
              &exti_d19_dev,
              EXTI_TRIGGER_FALLING_EDGE,
              on_button_prev,
              NULL);
    exti_start(&buttons[1]);
    exti_init(&buttons[2],
              &exti_d20_dev,
              EXTI_TRIGGER_FALLING_EDGE,
              on_button_next,
              NULL);
    exti_start(&buttons[2]);
    exti_init(&buttons[3],
              &exti_d21_dev,
              EXTI_TRIGGER_FALLING_EDGE,
              on_button_stop,
              NULL);
    exti_start(&buttons[3]);

    dac_init(&dac,
             &dac_0_dev,
             &pin_dac0_dev,
             &pin_dac1_dev,
             2 * SAMPLES_PER_SOCOND);

    hash_map_init(&song_map,
                  buckets,
                  membersof(buckets),
                  entries,
                  membersof(entries),
                  hash_number);

    sem_init(&sem, 0, 1);

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

        song_p = &songs[number - FIRST_SONG_NUMBER];

        /* Initialize the song entry. */
        song_p->number = number;
        strcpy(song_p->name, entry.name);
        seconds = (entry.size / 4 / SAMPLES_PER_SOCOND);
        song_p->minutes = (seconds / 60);
        song_p->seconds = (seconds % 60);

        std_printf("Adding song %s to playlist.\r\n",
                   entry.name);

        hash_map_add(&song_map, number, song_p);
        number++;
    }

    fat16_dir_close(&dir);

    last_song_number = (number - 1);

    music_player_start(&music_player);
}

int main()
{
    uint32_t mask;

    init();

    /* Spawn the shell. */
    shell_init(&shell, &uart.chin, &uart.chout, NULL, NULL, NULL, NULL);
    thrd_spawn(shell_main,
               &shell,
               0,
               shell_stack,
               sizeof(shell_stack));

    while (1) {
        mask = (EVENT_BUTTON_PLAY
                | EVENT_BUTTON_NEXT
                | EVENT_BUTTON_PREV
                | EVENT_BUTTON_STOP);
        event_read(&event, &mask, sizeof(mask));

        if (mask & EVENT_BUTTON_PLAY) {
            handle_event_play();
        }

        if (mask & EVENT_BUTTON_NEXT) {
            handle_event_next();
        }

        if (mask & EVENT_BUTTON_PREV) {
            handle_event_prev();
        }

        if (mask & EVENT_BUTTON_STOP) {
            handle_event_stop();
        }
    }

    return (0);
}
