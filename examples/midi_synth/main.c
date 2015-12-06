/**
 * @file main.c
 * @version 1.0
 *
 * @section License
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
#include "oscillator.h"
#include "waveforms.h"

COUNTER_DEFINE("/synth/unhandled_command", unhandled_command);

FS_COMMAND_DEFINE("/status", cmd_status);
FS_COMMAND_DEFINE("/set_waveform", cmd_set_waveform);
FS_COMMAND_DEFINE("/set_vibrato", cmd_set_vibrato);
FS_COMMAND_DEFINE("/note_on", cmd_note_on);
FS_COMMAND_DEFINE("/note_off", cmd_note_off);

#define EVENT_TIMEOUT 0x1

static char qinbuf[32];
static struct uart_driver_t uart;
static struct shell_args_t shell_args;
static THRD_STACK(shell_stack, 1024);

static struct uart_driver_t uart_midi;
static uint8_t uart_midi_inbuf[32];

/**
 * Sampling rate: 44100 Hz
 * 10 ms tick period. 100 ticks per second.
 * 44100 / 100 = 441 samples per tick.
 */

#define SAMPLE_RATE 44100
#define SAMPLES_MAX 441

struct note_t {
    int note;
    struct oscillator_t oscillator;
};

/**
 * A MIDI channel. It contains a list of all active notes.
 */
struct channel_t {
    int id;
    struct note_t notes[8];
    int length;
    struct {
        uint32_t *buf_p;
        size_t length;
    } waveform;
};

struct synth_t {
    float vibrato;
    struct event_t events;
    struct channel_t channels[16];
    struct dac_driver_t dac;
    struct timer_t timer;
    struct sem_t sem;
};

static struct synth_t synth;

static int note_off(struct channel_t *channel_p,
                    int note)
{
    int i;

    LOG(INFO, "note off: note = %d", note);

    sem_get(&synth.sem, NULL);
    for (i = 0; i < channel_p->length; i++) {
        if (channel_p->notes[i].note == note) {
            channel_p->notes[i] = channel_p->notes[channel_p->length - 1];
            channel_p->length--;
            break;
        }
    }

    sem_put(&synth.sem, 1);

    return (0);
}

static int note_on(struct channel_t *channel_p,
                   int note,
                   float frequency,
                   int velocity)
{
    LOG(INFO, "note on: note = %d, velocity = %d", note, velocity);

    /* Add the note to the list of notes on the channel. If the list
     * is full, remove the oldest note. */
    sem_get(&synth.sem, NULL);

    if (channel_p->length < membersof(channel_p->notes)) {
        channel_p->notes[channel_p->length].note = note;
        oscillator_init(&channel_p->notes[channel_p->length].oscillator,
                        channel_p->waveform.buf_p,
                        channel_p->waveform.length,
                        frequency,
                        synth.vibrato,
                        SAMPLE_RATE);
        channel_p->length++;
    }

    sem_put(&synth.sem, 1);

    return (0);
}

int cmd_status(int argc,
                   const char *argv[],
                   void *out_p,
                   void *in_p)
{
    int i, j;
    struct channel_t *channel_p;

    for (i = 0; i < membersof(synth.channels); i++) {
        channel_p = &synth.channels[i];

        std_fprintf(out_p,
                    FSTR("channel %d {\r\n"
                         "    length = %d\r\n"),
                    i,
                    channel_p->length);

        for (j = 0; j < channel_p->length; j++) {
            std_fprintf(out_p,
                        FSTR("    notes[%d] {\r\n"
                             "        note = %d\r\n"
                             "        oscillator {\r\n"
                             "            frequency = %f\r\n"
                             "        }\r\n"
                             "    }\r\n"),
                        j,
                        channel_p->notes[j].note,
                        channel_p->notes[j].oscillator.frequency);
        }

        std_fprintf(out_p, FSTR("}\r\n"));
    }

    return (0);
}

int cmd_set_waveform(int argc,
                     const char *argv[],
                     void *out_p,
                     void *in_p)
{
    long channel;
    struct channel_t *channel_p;

    if (argc != 3) {
        std_fprintf(out_p,
                    FSTR("Usage: %s <channel> <square or saw>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &channel) != 0) {
        return (-EINVAL);
    }

    channel_p = &synth.channels[channel];

    if (strcmp(argv[2], "sine") == 0) {
        channel_p->waveform.buf_p = waveform_sine_256;
        channel_p->waveform.length = membersof(waveform_sine_256);
    } else if (strcmp(argv[2], "saw") == 0) {
        channel_p->waveform.buf_p = waveform_saw_256;
        channel_p->waveform.length = membersof(waveform_saw_256);
    } else if (strcmp(argv[2], "square") == 0) {
        channel_p->waveform.buf_p = waveform_square_256;
        channel_p->waveform.length = membersof(waveform_square_256);
    } else if (strcmp(argv[2], "acoustic_guitar") == 0) {
        channel_p->waveform.buf_p = waveform_acoustic_guitar_600;
        channel_p->waveform.length =
            membersof(waveform_acoustic_guitar_600);
    } else if (strcmp(argv[2], "flute") == 0) {
        channel_p->waveform.buf_p = waveform_flute_600;
        channel_p->waveform.length = membersof(waveform_flute_600);
    } else if (strcmp(argv[2], "theremin") == 0) {
        channel_p->waveform.buf_p = waveform_theremin_600;
        channel_p->waveform.length = membersof(waveform_theremin_600);
    } else {
        std_fprintf(out_p,
                    FSTR("bad waveform %s\r\n"),
                    argv[2]);

        return (-EINVAL);
    }

    return (0);
}

int cmd_set_vibrato(int argc,
                    const char *argv[],
                    void *out_p,
                    void *in_p)
{
    long value;

    if (argc != 2) {
        std_fprintf(out_p,
                    FSTR("Usage: %s <0 to 100>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &value) != 0) {
        return (-EINVAL);
    }

    synth.vibrato = (0.002 * value);

    return (0);
}

int cmd_note_on(int argc,
                const char *argv[],
                void *out_p,
                void *in_p)
{
    long channel, note, frequency;

    if (argc != 4) {
        std_fprintf(out_p,
                    FSTR("Usage: %s <channel> <note> <frequency>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &channel) != 0) {
        return (-EINVAL);
    }

    if (std_strtol(argv[2], &note) != 0) {
        return (-EINVAL);
    }

    if (std_strtol(argv[3], &frequency) != 0) {
        return (-EINVAL);
    }

    return (note_on(&synth.channels[channel],
                    note,
                    frequency,
                    65));
}

int cmd_note_off(int argc,
                 const char *argv[],
                 void *out_p,
                 void *in_p)
{
    long channel, note;

    if (argc != 3) {
        std_fprintf(out_p,
                    FSTR("Usage: %s <channel> <note>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &channel) != 0) {
        return (-EINVAL);
    }

    if (std_strtol(argv[2], &note) != 0) {
        return (-EINVAL);
    }

    return (note_off(&synth.channels[channel], note));
}

static int handle_note_off(struct channel_t *channel_p,
                           uint8_t *buf_p,
                           size_t size)
{
    int note;

    uart_read(&uart_midi, &buf_p[size], 2 - size);
    note = *buf_p;

    return (note_off(channel_p, note));
}

static int handle_note_on(struct channel_t *channel_p,
                          uint8_t *buf_p,
                          size_t size)
{
    int note;
    int velocity;

    uart_read(&uart_midi, buf_p + size, 2 - size);

    note = buf_p[0];
    velocity = buf_p[1];

    if (channel_p->id == 9) {
        return (0);
    }

    if (velocity > 0) {
        note_on(channel_p,
                note,
                midi_note_to_frequency(note),
                velocity);
    } else {
        note_off(channel_p, note);
    }

    return (0);
}

/**
 * Periodically called callback to add samples to the DAC.
 */
static void fill_timer_cb(void *arg_p)
{
    uint32_t mask;

    mask = EVENT_TIMEOUT;
    event_write_irq(&synth.events, &mask, sizeof(mask));
}

static uint32_t samples[4][SAMPLES_MAX];
static uint32_t buf[SAMPLES_MAX];

static THRD_STACK(synth_main_stack, 1024);

static void *synth_main(void *arg_p)
{
    int i, j, k;
    struct channel_t *channel_p;
    struct note_t *note_p;
    uint32_t *samples_p;
    int samples_index = 0;
    uint32_t mask;
    struct time_t timeout;

    thrd_set_name("synth");

    /* Start the periodic fill timer. */
    timeout.seconds = 0;
    timeout.nanoseconds = 10000000;
    timer_set(&synth.timer,
              &timeout,
              (void (*)(void *))fill_timer_cb,
              NULL,
              TIMER_PERIODIC);

    while (1) {
        /* Wait for the periodic timeout event that drives the synth
         * signal processing. */
        mask = EVENT_TIMEOUT;
        event_read(&synth.events, &mask, sizeof(mask));

        for (i = 0; i < SAMPLES_MAX; i++) {
            buf[i] = 0;
        }

        samples_p = &samples[samples_index][0];
        samples_index++;
        samples_index %= membersof(samples);

        memset(samples_p, 0, sizeof(samples[0]));

        /* Calculate the next few samples and start the convertion in
         * the DAC. The smaller the buffer, the shorter the delay will
         * be. */
        sem_get(&synth.sem, NULL);

        for (i = 0; i < membersof(synth.channels); i++) {
            channel_p = &synth.channels[i];

            /* Process all notes for the channel. */
            for (j = 0; j < channel_p->length; j++) {
                note_p = &channel_p->notes[j];
                oscillator_read(&note_p->oscillator,
                                buf,
                                membersof(buf));

                /* Apply note effects. */

                for (k = 0; k < SAMPLES_MAX; k++) {
                    samples_p[k] += buf[k];
                }                
            }

            /* Apply channel effects. */
        }

        sem_put(&synth.sem, 1);

        /* Apply stream effects. */
        for (i = 0; i < SAMPLES_MAX; i++) {
            samples_p[i] >>= 7;
        }

        dac_async_convert(&synth.dac, samples_p, SAMPLES_MAX);
    }

    return (NULL);
}

static int init(void)
{
    int i;

    sys_start();
    uart_module_init();

    uart_init(&uart, &uart_device[0], 38400, qinbuf, sizeof(qinbuf));
    uart_start(&uart);

    uart_init(&uart_midi,
              &uart_device[1],
              MIDI_BAUDRATE,
              uart_midi_inbuf,
              sizeof(uart_midi_inbuf));
    uart_start(&uart_midi);

    sys_set_stdout(&uart.chout);

    std_printf(sys_get_info());

    event_init(&synth.events);
    sem_init(&synth.sem, 1);

    dac_init(&synth.dac,
             &dac_0_dev,
             &pin_dac0_dev,
             &pin_dac1_dev,
             2 * SAMPLE_RATE);

    for (i = 0; i < membersof(synth.channels); i++) {
        synth.channels[i].id = i;
        synth.channels[i].length = 0;
        synth.channels[i].waveform.buf_p = waveform_square_256;
        synth.channels[i].waveform.length = membersof(waveform_square_256);
    }

    synth.vibrato = 0.0;

    /* Spawn the shell. */
    shell_args.chin_p = &uart.chin;
    shell_args.chout_p = &uart.chout;
    shell_args.username_p = NULL;
    shell_args.password_p = NULL;
    thrd_spawn(shell_entry,
               &shell_args,
               0,
               shell_stack,
               sizeof(shell_stack));

    thrd_spawn(synth_main,
               NULL,
               -1,
               synth_main_stack,
               sizeof(synth_main_stack));

    return (0);
}

int main()
{
    init();

    uint8_t buf[3], previous_status = 0;
    uint8_t *buf_p;
    uint8_t status;
    int command;
    int channel;
    size_t size;

    while (1) {
        /* Wait for a MIDI command on the serial port. */
        buf_p = buf;
        uart_read(&uart_midi, buf_p, 1);

        /* Use the previous status if a data byte is read. */
        if ((*buf_p & 0x80) != 0) {
            status = *buf_p++;
            size = 0;
        } else {
            status = previous_status;
            size = 1;
        }

        /* Extract the command and the channel from the status. */
        command = (status & 0xf0);
        channel = (status & 0x0f);

        switch (command) {

        case MIDI_NOTE_OFF:
            handle_note_off(&synth.channels[channel],
                            buf_p,
                            size);
            break;

        case MIDI_NOTE_ON:
            handle_note_on(&synth.channels[channel],
                           buf_p,
                           size);
            previous_status = status;
            break;

        case MIDI_SET_INTRUMENT:
            uart_read(&uart_midi, buf_p, 1);
            break;

        default:
            COUNTER_INC(unhandled_command, 1);
            break;
        }
    }

    return (0);
}
