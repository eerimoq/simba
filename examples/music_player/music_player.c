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

/* Player states. */
#define STATE_IDLE          0
#define STATE_PLAYING       1
#define STATE_PAUSED        2
#define STATE_STOPPED       3

/* Music player events. */
#define EVENT_PLAY        0x1
#define EVENT_PAUSE       0x2
#define EVENT_STOP        0x4
#define EVENT_TIMEOUT     0x8

static int handle_event_play(struct music_player_t *self_p)
{
    const char *path_p;

    switch (self_p->state) {

    case STATE_PAUSED:
        self_p->state = STATE_PLAYING;
        break;

    case STATE_STOPPED:
        if ((path_p = self_p->cb.current_song_path_p(self_p->cb.arg_p)) != NULL) {
            fat16_file_open(self_p->fat16_p, &self_p->file, path_p, O_READ);
            strcpy(self_p->path, path_p);
            self_p->state = STATE_PLAYING;
        }
        break;

    case STATE_IDLE:
        if ((path_p = self_p->cb.current_song_path_p(self_p->cb.arg_p)) != NULL) {
            if (fat16_file_open(self_p->fat16_p, &self_p->file, path_p, O_READ) == 0) {
                strcpy(self_p->path, path_p);
                self_p->state = STATE_PLAYING;
            } else {
                std_printf(FSTR("Failed to open %s\r\n"), path_p);
            }
        }
        break;

    default:
        break;
    }

    if (self_p->state == STATE_PLAYING) {
        std_printf(FSTR("Playing | %s\r\n"), self_p->path);
    }

    return (0);
}

static int handle_event_pause(struct music_player_t *self_p)
{
    switch (self_p->state) {

    case STATE_PLAYING:
        std_printf(FSTR("Paused  | %s\r\n"), self_p->path);
        self_p->state = STATE_PAUSED;
        break;

    default:
        break;
    }

    return (0);
}

static int handle_event_stop(struct music_player_t *self_p)
{
    switch (self_p->state) {

    case STATE_PLAYING:
    case STATE_PAUSED:
        fat16_file_close(&self_p->file);
        self_p->state = STATE_STOPPED;
        break;

    default:
        break;
    }

    return (0);
}

/**
 * Read from the file for the current song and add samples to the
 * DAC. In case the song ends, open the next song as preparation for
 * the next call to this function.
 */
static int play_chunk(struct music_player_t *self_p)
{
    const char *path_p;
    uint32_t *buf_p;
    size_t size;
    int i;

    /* Read samples from the file. */
    buf_p = self_p->samples.buf[self_p->samples.index];
    self_p->samples.index++;
    self_p->samples.index %= membersof(self_p->samples.buf);
    size = fat16_file_read(&self_p->file,
                           buf_p,
                           sizeof(self_p->samples.buf[0]));

    /* Optional down sampling of the read samples. */
    if (self_p->down_sampling_mask != 0xfffffffful) {
        for (i = 0; i < size / 4; i++) {
            buf_p[i] &= self_p->down_sampling_mask;
        }
    }

    if (size > 0) {
        /* Add samples for DAC convertion. */
        dac_async_convert(self_p->dac_p, buf_p, size / 4);
    } else {
        /* Start playing the next file in the queue. */
        fat16_file_close(&self_p->file);

        if ((path_p = self_p->cb.next_song_path_p(self_p->cb.arg_p)) != NULL) {
            strcpy(self_p->path, path_p);
            std_printf(FSTR("Playing | %s\r\n"), self_p->path);
            fat16_file_open(self_p->fat16_p, &self_p->file, path_p, O_READ);
        } else {
            self_p->state = STATE_IDLE;
        }
    }

    return (0);
}

/**
 * Periodically called callback used to add samples to the DAC.
 */
static void fill_timer_cb(struct music_player_t *self_p)
{
    uint32_t mask;

    mask = EVENT_TIMEOUT;
    event_write_isr(&self_p->event, &mask, sizeof(mask));
}

/**
 * The main thread of the music player. Handles events and coverts
 * samples to play a song.
 */
static void *music_player_main(struct music_player_t *self_p)
{
    uint32_t mask;
    struct time_t timeout;

    thrd_set_name("music_player");

    /* Start the periodic fill timer. */
    timeout.seconds = 0;
    timeout.nanoseconds = 10000000;
    timer_init(&self_p->timer,
               &timeout,
               (void (*)(void *))fill_timer_cb,
               self_p,
               TIMER_PERIODIC);
    timer_start(&self_p->timer);

    /* Start the main loop of the music player. */
    while (1) {
        mask = (EVENT_PLAY
                | EVENT_PAUSE
                | EVENT_STOP
                | EVENT_TIMEOUT);
        event_read(&self_p->event, &mask, sizeof(mask));

        if (mask & EVENT_STOP) {
            handle_event_stop(self_p);
        }

        if (mask & EVENT_PAUSE) {
            handle_event_pause(self_p);
        }

        if (mask & EVENT_PLAY) {
            handle_event_play(self_p);
        }

        /* Play if the state in playing, eyy! */
        if (self_p->state == STATE_PLAYING) {
            play_chunk(self_p);
        }
    }

    return (NULL);
}

int music_player_init(struct music_player_t *self_p,
                      struct fat16_t *fat16_p,
                      struct dac_driver_t *dac_p,
                      song_path_t current_song_path_p,
                      song_path_t next_song_path_p,
                      void *arg_p)
{
    self_p->state = STATE_IDLE;
    self_p->down_sampling_mask = 0xfffffffful;
    self_p->fat16_p = fat16_p;
    self_p->dac_p = dac_p;
    self_p->samples.index = 0;
    self_p->cb.current_song_path_p = current_song_path_p;
    self_p->cb.next_song_path_p = next_song_path_p;
    self_p->cb.arg_p = arg_p;
    self_p->thrd_p = NULL;
    event_init(&self_p->event);

    return (0);
}

int music_player_start(struct music_player_t *self_p)
{
    self_p->thrd_p = thrd_spawn((void *(*)(void *))music_player_main,
                                self_p,
                                -30,
                                self_p->stack,
                                sizeof(self_p->stack));

    return (self_p->thrd_p == NULL);
}

int music_player_stop(struct music_player_t *self_p)
{
    return (-1);
}

int music_player_song_play(struct music_player_t *self_p)
{
    uint32_t mask;

    mask = EVENT_PLAY;
    event_write(&self_p->event, &mask, sizeof(mask));

    return (0);
}

int music_player_song_pause(struct music_player_t *self_p)
{
    uint32_t mask;

    mask = EVENT_PAUSE;
    event_write(&self_p->event, &mask, sizeof(mask));

    return (0);
}

int music_player_song_stop(struct music_player_t *self_p)
{
    uint32_t mask;

    mask = EVENT_STOP;
    event_write(&self_p->event, &mask, sizeof(mask));

    return (0);
}

int music_player_set_bits_per_sample(struct music_player_t *self_p,
                                     int value)
{
    uint16_t mask;

    value = (12 - value);
    mask = ~((1 << value) - 1);

    self_p->down_sampling_mask = (((uint32_t)mask << 16) | mask);

    log_object_print(NULL,
                     LOG_INFO,
                     OSTR("down_sampling_mask = 0x%lx\r\n"),
                     self_p->down_sampling_mask);

    return (0);
}
