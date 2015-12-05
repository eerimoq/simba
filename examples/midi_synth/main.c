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
};

struct synth_t {
    uint32_t *waveform_p;
    float vibrato;
    struct event_t events;
    struct channel_t channels[16];
    struct dac_driver_t dac;
    struct timer_t timer;
    struct sem_t sem;
};

static struct synth_t synth;

/**
 * A sine wave.
 *
 *  65535      ,--.          ,--.          ,-
 *            /    \        /    \        /
 *           /      \      /      \      /
 *          /        \    /        \    /
 *      0 -´          `--´          `--´
 */
static uint32_t waveform_sine_256[] = {
    32767, 33571, 34375, 35178, 35979, 36778, 37575, 38369,
    39160, 39946, 40729, 41507, 42279, 43046, 43806, 44560,
    45307, 46046, 46777, 47500, 48213, 48918, 49613, 50298,
    50972, 51635, 52287, 52927, 53554, 54170, 54772, 55362,
    55937, 56499, 57046, 57579, 58097, 58599, 59086, 59557,
    60012, 60451, 60873, 61278, 61665, 62036, 62388, 62723,
    63040, 63339, 63619, 63881, 64124, 64348, 64552, 64738,
    64905, 65052, 65180, 65288, 65377, 65446, 65495, 65525,
    65535, 65525, 65495, 65446, 65377, 65288, 65180, 65052,
    64905, 64738, 64552, 64348, 64124, 63881, 63619, 63339,
    63040, 62723, 62388, 62036, 61665, 61278, 60873, 60451,
    60012, 59557, 59086, 58599, 58097, 57579, 57046, 56499,
    55937, 55362, 54772, 54170, 53554, 52927, 52287, 51635,
    50972, 50298, 49613, 48918, 48213, 47500, 46777, 46046,
    45307, 44560, 43806, 43046, 42279, 41507, 40729, 39946,
    39160, 38369, 37575, 36778, 35979, 35178, 34375, 33571,
    32767, 31963, 31159, 30356, 29555, 28756, 27959, 27165,
    26374, 25588, 24805, 24027, 23255, 22488, 21728, 20974,
    20227, 19488, 18757, 18034, 17321, 16616, 15921, 15236,
    14562, 13899, 13247, 12607, 11980, 11364, 10762, 10172,
    9597, 9035, 8488, 7955, 7437, 6935, 6448, 5977,
    5522, 5083, 4661, 4256, 3869, 3498, 3146, 2811,
    2494, 2195, 1915, 1653, 1410, 1186, 982, 796,
    629, 482, 354, 246, 157, 88, 39, 9,
    0, 9, 39, 88, 157, 246, 354, 482,
    629, 796, 982, 1186, 1410, 1653, 1915, 2195,
    2494, 2811, 3146, 3498, 3869, 4256, 4661, 5083,
    5522, 5977, 6448, 6935, 7437, 7955, 8488, 9035,
    9597, 10172, 10762, 11364, 11980, 12607, 13247, 13899,
    14562, 15236, 15921, 16616, 17321, 18034, 18757, 19488,
    20227, 20974, 21728, 22488, 23255, 24027, 24805, 25588,
    26374, 27165, 27959, 28756, 29555, 30356, 31159, 31963
};

/**
 * A square wave.
 *
 *  65535         +--------+        +--------+
 *                |        |        |        |
 *                |        |        |        |
 *                |        |        |        |
 *      0 --------+        +--------+        +--------
 */
static uint32_t waveform_square_256[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535
};

/**
 * A saw wave.
 *
 *  65535     ,   ,   ,   ,   ,   ,   ,
 *           /|  /|  /|  /|  /|  /|  /|
 *          / | / | / | / | / | / | / |
 *         /  |/  |/  |/  |/  |/  |/  |
 *      0 ´   ´   ´   ´   ´   ´   ´   ´
 */
static uint32_t waveform_saw_256[] = {
    0, 257, 514, 771, 1028, 1285, 1542, 1799,
    2056, 2313, 2570, 2827, 3084, 3341, 3598, 3855,
    4112, 4369, 4626, 4883, 5140, 5397, 5654, 5911,
    6168, 6425, 6682, 6939, 7196, 7453, 7710, 7967,
    8224, 8481, 8738, 8995, 9252, 9509, 9766, 10023,
    10280, 10537, 10794, 11051, 11308, 11565, 11822, 12079,
    12336, 12593, 12850, 13107, 13364, 13621, 13878, 14135,
    14392, 14649, 14906, 15163, 15420, 15677, 15934, 16191,
    16448, 16705, 16962, 17219, 17476, 17733, 17990, 18247,
    18504, 18761, 19018, 19275, 19532, 19789, 20046, 20303,
    20560, 20817, 21074, 21331, 21588, 21845, 22102, 22359,
    22616, 22873, 23130, 23387, 23644, 23901, 24158, 24415,
    24672, 24929, 25186, 25443, 25700, 25957, 26214, 26471,
    26728, 26985, 27242, 27499, 27756, 28013, 28270, 28527,
    28784, 29041, 29298, 29555, 29812, 30069, 30326, 30583,
    30840, 31097, 31354, 31611, 31868, 32125, 32382, 32639,
    32896, 33153, 33410, 33667, 33924, 34181, 34438, 34695,
    34952, 35209, 35466, 35723, 35980, 36237, 36494, 36751,
    37008, 37265, 37522, 37779, 38036, 38293, 38550, 38807,
    39064, 39321, 39578, 39835, 40092, 40349, 40606, 40863,
    41120, 41377, 41634, 41891, 42148, 42405, 42662, 42919,
    43176, 43433, 43690, 43947, 44204, 44461, 44718, 44975,
    45232, 45489, 45746, 46003, 46260, 46517, 46774, 47031,
    47288, 47545, 47802, 48059, 48316, 48573, 48830, 49087,
    49344, 49601, 49858, 50115, 50372, 50629, 50886, 51143,
    51400, 51657, 51914, 52171, 52428, 52685, 52942, 53199,
    53456, 53713, 53970, 54227, 54484, 54741, 54998, 55255,
    55512, 55769, 56026, 56283, 56540, 56797, 57054, 57311,
    57568, 57825, 58082, 58339, 58596, 58853, 59110, 59367,
    59624, 59881, 60138, 60395, 60652, 60909, 61166, 61423,
    61680, 61937, 62194, 62451, 62708, 62965, 63222, 63479,
    63736, 63993, 64250, 64507, 64764, 65021, 65278, 65535
};

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
                        synth.waveform_p,
                        membersof(waveform_sine_256),
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
    if (argc != 2) {
        std_fprintf(out_p,
                    FSTR("Usage: %s <square or saw>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    if (strcmp(argv[1], "sine") == 0) {
        synth.waveform_p = waveform_sine_256;
    } else if (strcmp(argv[1], "saw") == 0) {
        synth.waveform_p = waveform_saw_256;
    } else if (strcmp(argv[1], "square") == 0) {
        synth.waveform_p = waveform_square_256;
    } else {
        std_fprintf(out_p,
                    FSTR("bad waveform %s\r\n"),
                    argv[1]);

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
    }

    synth.waveform_p = waveform_square_256;
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
