/**
 * @file channel.h
 * @version 0.6.0
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
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

#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include "simba.h"
#include "note.h"

/* Channel states. */
#define CHANNEL_STATE_ON  0
#define CHANNEL_STATE_OFF 1

/**
 * A MIDI channel. It contains a list of all active notes.
 */
struct channel_t {
    int id;
    int state;
    struct note_t notes[8];
    int length;
    struct {
        int pos;
    } iter;
    struct {
        int32_t *buf_p;
        size_t length;
    } waveform;
};

/**
 * Initialize a channel with given configuration.
 *
 * @param[in] self_p The channel to initialize.
 * @param[in] id Channel id.
 * @param[in] buf_p Waveform to use.
 * @param[in] length Length of the waveform.
 *
 * @return zero(0) or negative error code.
 */
int channel_init(struct channel_t *self_p,
                 int id,
                 int32_t *buf_p,
                 size_t length);

/**
 * Do the signal processing.
 *
 * @param[in] self_p Initialize channel.
 * @param[in] samples_p Output sample buffer.
 * @param[in] buf_p Workspace buffer for the signal processing.
 * @param[in] length Number of members in sample buffers.
 *
 * @return zero(0) or negative error code.
 */
int channel_process(struct channel_t *self_p,
                    int32_t *samples_p,
                    int32_t *buf_p,
                    size_t length);

/**
 * Set the channel waveform.
 *
 * @param[in] self_p Initialize channel.
 * @param[in] buf_p Pointer to the waveform.
 * @param[in] length Length of the waveform.
 *
 * @return zero(0) or negative error code.
 */
int channel_set_waveform(struct channel_t *self_p,
                         int32_t *buf_p,
                         size_t length);

/**
 * Set the channel state.
 *
 * @param[in] self_p Initialize channel.
 * @param[in] state New state.
 *
 * @return zero(0) or negative error code.
 */
int channel_set_state(struct channel_t *self_p,
                      int state);

/**
 * Get the channel id.
 *
 * @param[in] self_p Initialize channel.
 *
 * @return The channel id or negative error code.
 */
int channel_get_id(struct channel_t *self_p);

/**
 * Allocate a note.
 *
 * @param[in] self_p Initialize channel.
 *
 * @return The allocated note ot NULL on failure.
 */
struct note_t *channel_note_alloc(struct channel_t *self_p);

/**
 * Get the channel id.
 *
 * @param[in] self_p Initialize channel.
 *
 * @return The channel id or negative error code.
 */
struct note_t *channel_note_get(struct channel_t *self_p,
                                int note);

#endif
