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

#define SAMPLES_MAX      1024

#define SONG_PATH_MAX      64
#define QUEUE_MAX           8

typedef const char *(*song_path_t)(void *arg_p);

struct music_player_t {
    int state;
    uint32_t down_sampling_mask;
    struct event_t event;
    struct timer_t timer;
    struct fat16_t *fat16_p;
    struct dac_driver_t *dac_p;
    char path[32];
    struct fat16_file_t file;
    struct {
        int index;
        uint32_t buf[4][SAMPLES_MAX] __attribute__((aligned (8)));
    } samples __attribute__((aligned (8)));
    struct {
        song_path_t current_song_path_p;
        song_path_t next_song_path_p;
        void *arg_p;
    } cb;
    struct thrd_t *thrd_p;
    THRD_STACK(stack, 1024);
};

/**
 * Initialize the music player datastructure.
 *
 * @param[out] self_p Music player to initialize.
 * @param[in] fat16_p Fat16 file system to use.
 * @param[in] dac_p DAC used to convert song samples to an analog
 *                  singal.
 * @param[in] current_song_path_p Callback to get the current song
 *                                path.
 * @param[in] next_song_path_p Callback to get the next song path.
 * @param[in] arg_p Argument passed to song callbacks.
 *
 * @return zero(0) or negative error code.
 */
int music_player_init(struct music_player_t *self_p,
                      struct fat16_t *fat16_p,
                      struct dac_driver_t *dac_p,
                      song_path_t current_song_path_p,
                      song_path_t next_song_path_p,
                      void *arg_p);

/**
 * Start the music player.
 *
 * @return zero(0) or negative error code.
 */
int music_player_start(struct music_player_t *self_p);

/**
 * Stop the music player.
 *
 * @return zero(0) or negative error code.
 */
int music_player_stop(struct music_player_t *self_p);

/**
 * Resume any paused song, or restart a stopped song, or start playing
 * the first song.
 *
 * @return zero(0) or negative error code.
 */
int music_player_song_play(struct music_player_t *self_p);

/**
 * Pause the current song. Can be resumed later by calling
 * music_player_song_play().
 *
 * @return zero(0) or negative error code.
 */
int music_player_song_pause(struct music_player_t *self_p);

/**
 * Stop the current song and reset the it to start playing from the
 * beginning of the song when music_player_song_play() is called.
 *
 * @return zero(0) or negative error code.
 */
int music_player_song_stop(struct music_player_t *self_p);

/**
 * Set the number of bits per sample.

 * @return zero(0) or negative error code.
 */
int music_player_set_bits_per_sample(struct music_player_t *self_p,
                                     int value);
