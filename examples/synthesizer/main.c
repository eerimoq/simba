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
#include "channel.h"
#include "waveforms.h"

static struct fs_counter_t midi_unhandled_command;

static struct fs_command_t cmd_status;
static struct fs_command_t cmd_set_waveform;
static struct fs_command_t cmd_set_vibrato;
static struct fs_command_t cmd_set_envelope;
static struct fs_command_t cmd_note_on;
static struct fs_command_t cmd_note_off;
static struct fs_command_t cmd_channel_on;
static struct fs_command_t cmd_channel_off;

#define EVENT_TIMEOUT 0x1

/**
 * Sampling rate: 44100 Hz
 * 10 ms tick period. 100 ticks per second.
 * 44100 / 100 = 441 samples per tick.
 */

#define SAMPLE_RATE 44100
#define SAMPLES_MAX 441

struct synthesizer_t {
    float vibrato;
    long attack;
    long decay;
    long release;
    struct event_t events;
    struct channel_t channels[16];
    struct dac_driver_t dac;
    struct timer_t timer;
    struct sem_t sem;
    struct {
        struct adc_driver_t frequency;
        struct adc_driver_t vibrato;
        struct event_t events;
    } sensors;
};

static char qinbuf[32];
static struct uart_driver_t uart;
static struct shell_t shell;
static THRD_STACK(shell_stack, 1024);

static struct uart_driver_t uart_midi;
static uint8_t uart_midi_inbuf[32];

static int32_t samples[4][SAMPLES_MAX];
static int32_t buf[SAMPLES_MAX];

static struct synthesizer_t synthesizer;

static THRD_STACK(sensors_main_stack, 1024);
static THRD_STACK(midi_main_stack, 1024);

static int note_off(struct channel_t *channel_p,
                    int note)
{
    struct note_t *note_p;

    log_object_print(NULL,
                     LOG_INFO,
                     OSTR("note off: note = %d"),
                     note);

    sem_take(&synthesizer.sem, NULL);

    note_p = channel_note_get(channel_p, note);

    if (note_p != NULL) {
        note_stop(note_p);
    }

    sem_give(&synthesizer.sem, 1);

    return (0);
}

static int note_on(struct channel_t *channel_p,
                   int note,
                   float frequency,
                   int velocity)
{
    struct note_t *note_p;

    log_object_print(NULL,
                     LOG_INFO,
                     OSTR("note on: note = %d, velocity = %d"),
                     note,
                     velocity);

    /* Add the note to the list of notes on the channel. If the list
     * is full, remove the oldest note. */
    sem_take(&synthesizer.sem, NULL);

    if ((note_p = channel_note_alloc(channel_p)) != NULL) {
        note_init(note_p,
                  note,
                  channel_p->waveform.buf_p,
                  channel_p->waveform.length,
                  frequency,
                  synthesizer.vibrato,
                  synthesizer.attack,
                  synthesizer.decay,
                  synthesizer.release,
                  SAMPLE_RATE);
        note_start(note_p);
    }

    sem_give(&synthesizer.sem, 1);

    return (0);
}

static int cmd_status_cb(int argc,
                         const char *argv[],
                         void *out_p,
                         void *in_p,
                         void *arg_p,
                         void *call_arg_p)
{
    int i, j;
    struct channel_t *channel_p;

    for (i = 0; i < membersof(synthesizer.channels); i++) {
        channel_p = &synthesizer.channels[i];

        std_fprintf(out_p,
                    OSTR("channel %d {\r\n"
                         "    status = %s\r\n"
                         "    length = %d\r\n"),
                    i,
                    (channel_p->state == CHANNEL_STATE_ON ? "on" : "off"),
                    channel_p->length);

        for (j = 0; j < channel_p->length; j++) {
            std_fprintf(out_p,
                        OSTR("    notes[%d] {\r\n"
                             "        note = %d\r\n"
                             "        oscillator {\r\n"
                             "            frequency = %f\r\n"
                             "        }\r\n"
                             "    }\r\n"),
                        j,
                        channel_p->notes[j].note,
                        channel_p->notes[j].oscillator.frequency);
        }

        std_fprintf(out_p, OSTR("}\r\n"));
    }

    return (0);
}

static int cmd_set_waveform_cb(int argc,
                               const char *argv[],
                               void *out_p,
                               void *in_p,
                               void *arg_p,
                               void *call_arg_p)
{
    long channel;
    struct channel_t *channel_p;

    if (argc != 3) {
        std_fprintf(out_p,
                    OSTR("Usage: %s <channel> <square or saw>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &channel) == NULL) {
        return (-EINVAL);
    }

    channel_p = &synthesizer.channels[channel];

    if (strcmp(argv[2], "sine") == 0) {
        channel_set_waveform(channel_p,
                             waveform_sine_256,
                             membersof(waveform_sine_256));
    } else if (strcmp(argv[2], "saw") == 0) {
        channel_set_waveform(channel_p,
                             waveform_saw_256,
                             membersof(waveform_saw_256));
    } else if (strcmp(argv[2], "square") == 0) {
        channel_set_waveform(channel_p,
                             waveform_square_256,
                             membersof(waveform_square_256));
    } else {
        std_fprintf(out_p, OSTR("bad waveform %s\r\n"), argv[2]);

        return (-EINVAL);
    }

    return (0);
}

static int cmd_set_vibrato_cb(int argc,
                              const char *argv[],
                              void *out_p,
                              void *in_p,
                              void *arg_p,
                              void *call_arg_p)
{
    long value;

    if (argc != 2) {
        std_fprintf(out_p,
                    OSTR("Usage: %s <0 to 100>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &value) == NULL) {
        return (-EINVAL);
    }

    synthesizer.vibrato = (0.002 * value);

    return (0);
}

static int cmd_set_envelope_cb(int argc,
                               const char *argv[],
                               void *out_p,
                               void *in_p,
                               void *arg_p,
                               void *call_arg_p)
{
    long value;

    if (argc != 4) {
        std_fprintf(out_p,
                    OSTR("Usage: %s <attack> <decay> <release>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &value) == NULL) {
        return (-EINVAL);
    }

    synthesizer.attack = value;

    if (std_strtol(argv[2], &value) == NULL) {
        return (-EINVAL);
    }

    synthesizer.decay = value;

    if (std_strtol(argv[3], &value) == NULL) {
        return (-EINVAL);
    }

    synthesizer.release = value;

    return (0);
}

static int cmd_note_on_cb(int argc,
                          const char *argv[],
                          void *out_p,
                          void *in_p,
                          void *arg_p,
                          void *call_arg_p)
{
    long channel, note, frequency;

    if (argc != 4) {
        std_fprintf(out_p,
                    OSTR("Usage: %s <channel> <note> <frequency>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &channel) == NULL) {
        return (-EINVAL);
    }

    if (std_strtol(argv[2], &note) == NULL) {
        return (-EINVAL);
    }

    if (std_strtol(argv[3], &frequency) == NULL) {
        return (-EINVAL);
    }

    return (note_on(&synthesizer.channels[channel],
                    note,
                    frequency,
                    65));
}

static int cmd_note_off_cb(int argc,
                           const char *argv[],
                           void *out_p,
                           void *in_p,
                           void *arg_p,
                           void *call_arg_p)
{
    long channel, note;

    if (argc != 3) {
        std_fprintf(out_p,
                    OSTR("Usage: %s <channel> <note>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &channel) == NULL) {
        return (-EINVAL);
    }

    if (std_strtol(argv[2], &note) == NULL) {
        return (-EINVAL);
    }

    return (note_off(&synthesizer.channels[channel], note));
}

static int cmd_channel_on_cb(int argc,
                             const char *argv[],
                             void *out_p,
                             void *in_p,
                             void *arg_p,
                             void *call_arg_p)
{
    long channel;

    if (argc != 2) {
        std_fprintf(out_p,
                    OSTR("Usage: %s <channel>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &channel) == NULL) {
        return (-EINVAL);
    }

    synthesizer.channels[channel].state = CHANNEL_STATE_ON;

    return (0);
}

static int cmd_channel_off_cb(int argc,
                              const char *argv[],
                              void *out_p,
                              void *in_p,
                              void *arg_p,
                              void *call_arg_p)
{
    long channel;

    if (argc != 2) {
        std_fprintf(out_p,
                    OSTR("Usage: %s <channel>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &channel) == NULL) {
        return (-EINVAL);
    }

    synthesizer.channels[channel].state = CHANNEL_STATE_OFF;

    return (0);
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

    if (channel_get_id(channel_p) == 9) {
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
    event_write_isr(&synthesizer.events, &mask, sizeof(mask));
    mask = EVENT_TIMEOUT;
    event_write_isr(&synthesizer.sensors.events, &mask, sizeof(mask));
}

/**
 * A thread that reads the analog input pins and configures the
 * synthesizer accordingly.
 */
static void *sensors_main(void *arg_p)
{
    uint32_t mask;
    uint16_t sample;
    float frequency, vibrato;

    thrd_set_name("sensors");

    adc_init(&synthesizer.sensors.frequency,
             &adc_device[0],
             &pin_a0_dev,
             0,
             -1);

    adc_init(&synthesizer.sensors.vibrato,
             &adc_device[0],
             &pin_a1_dev,
             0,
             -1);

    while (1) {
        /* Wait for the periodic timeout event that drives the synth
         * signal processing. */
        mask = EVENT_TIMEOUT;
        event_read(&synthesizer.sensors.events, &mask, sizeof(mask));

        adc_convert(&synthesizer.sensors.frequency, &sample, 1);
        frequency = sample;

        adc_convert(&synthesizer.sensors.vibrato, &sample, 1);
        vibrato = (0.002 * ((float)sample / 40.96));

        sem_take(&synthesizer.sem, NULL);

        synthesizer.vibrato = vibrato;
        oscillator_set_frequency(&synthesizer.channels[15].notes[0].oscillator,
                                 frequency);
        oscillator_set_vibrato(&synthesizer.channels[15].notes[0].oscillator,
                               vibrato);

        sem_give(&synthesizer.sem, 1);
    }

    return (NULL);
}

/**
 * A thread that reads the MIDI input and configures the synthesizer
 * accordingly.
 */
static void *midi_main(void *arg_p)
{
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
            handle_note_off(&synthesizer.channels[channel],
                            buf_p,
                            size);
            break;

        case MIDI_NOTE_ON:
            handle_note_on(&synthesizer.channels[channel],
                           buf_p,
                           size);
            previous_status = status;
            break;

        case MIDI_SET_INTRUMENT:
            uart_read(&uart_midi, buf_p, 1);
            break;

        default:
            fs_counter_increment(&midi_unhandled_command, 1);
            break;
        }
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

    fs_counter_init(&midi_unhandled_command,
                    FSTR("/synth/midi_unhandled_command"),
                    0);
    fs_counter_register(&midi_unhandled_command);

    fs_command_init(&cmd_status,
                    CSTR("/status"),
                    cmd_status_cb,
                    NULL);
    fs_command_register(&cmd_status);

    fs_command_init(&cmd_set_waveform,
                    CSTR("/set_waveform"),
                    cmd_set_waveform_cb,
                    NULL);
    fs_command_register(&cmd_set_waveform);

    fs_command_init(&cmd_set_vibrato,
                    CSTR("/set_vibrato"),
                    cmd_set_vibrato_cb,
                    NULL);
    fs_command_register(&cmd_set_vibrato);

    fs_command_init(&cmd_set_envelope,
                    CSTR("/set_envelope"),
                    cmd_set_envelope_cb,
                    NULL);
    fs_command_register(&cmd_set_envelope);

    fs_command_init(&cmd_note_on,
                    CSTR("/note_on"),
                    cmd_note_on_cb,
                    NULL);
    fs_command_register(&cmd_note_on);

    fs_command_init(&cmd_note_off,
                    CSTR("/note_off"),
                    cmd_note_off_cb,
                    NULL);
    fs_command_register(&cmd_note_off);

    fs_command_init(&cmd_channel_on,
                    CSTR("/channel_on"),
                    cmd_channel_on_cb,
                    NULL);
    fs_command_register(&cmd_channel_on);

    fs_command_init(&cmd_channel_off,
                    CSTR("/channel_off"),
                    cmd_channel_off_cb,
                    NULL);
    fs_command_register(&cmd_channel_off);

    uart_init(&uart_midi,
              &uart_device[1],
              MIDI_BAUDRATE,
              uart_midi_inbuf,
              sizeof(uart_midi_inbuf));
    uart_start(&uart_midi);

    sys_set_stdout(&uart.chout);

    std_printf(sys_get_info());

    event_init(&synthesizer.events);
    event_init(&synthesizer.sensors.events);
    sem_init(&synthesizer.sem, 0, 1);

    dac_init(&synthesizer.dac,
             &dac_0_dev,
             &pin_dac0_dev,
             &pin_dac1_dev,
             2 * SAMPLE_RATE);

    adc_module_init();

    for (i = 0; i < membersof(synthesizer.channels); i++) {
        channel_init(&synthesizer.channels[i],
                     i,
                     waveform_square_256,
                     membersof(waveform_square_256));
    }

    synthesizer.vibrato = 0.0;
    synthesizer.attack = 1000;
    synthesizer.decay = 0;
    synthesizer.release = 1000;

    /* Spawn the shell. */
    shell_init(&shell, &uart.chin, &uart.chout, NULL, NULL, NULL, NULL);
    thrd_spawn(shell_main,
               &shell,
               15,
               shell_stack,
               sizeof(shell_stack));

    thrd_spawn(midi_main,
               NULL,
               10,
               midi_main_stack,
               sizeof(midi_main_stack));

    thrd_spawn(sensors_main,
               NULL,
               5,
               sensors_main_stack,
               sizeof(sensors_main_stack));

    return (0);
}

int main()
{
    int i;
    int32_t *samples_p;
    int samples_index = 0;
    uint32_t mask;
    struct time_t timeout;

    init();

    thrd_set_name("synthesizer");

    /* Start the periodic fill timer. */
    timeout.seconds = 0;
    timeout.nanoseconds = 10000000;
    timer_init(&synthesizer.timer,
               &timeout,
               (void (*)(void *))fill_timer_cb,
               NULL,
               TIMER_PERIODIC);
    timer_start(&synthesizer.timer);

    while (1) {
        /* Wait for the periodic timeout event that drives the synth
         * signal processing. */
        mask = EVENT_TIMEOUT;
        event_read(&synthesizer.events, &mask, sizeof(mask));

        /* Prepare the sample buffer. */
        samples_p = &samples[samples_index][0];
        samples_index++;
        samples_index %= membersof(samples);
        memset(samples_p, 0, sizeof(samples[0]));

        /* Calculate the next few samples and start the convertion in
         * the DAC. The smaller the buffer, the shorter the delay to
         * the speaker. */
        sem_take(&synthesizer.sem, NULL);

        for (i = 0; i < membersof(synthesizer.channels); i++) {
            channel_process(&synthesizer.channels[i],
                            samples_p,
                            buf,
                            SAMPLES_MAX);
        }

        sem_give(&synthesizer.sem, 1);

        for (i = 0; i < SAMPLES_MAX; i++) {
            samples_p[i] = ((samples_p[i] >> 7) + 2048);
        }

        dac_async_convert(&synthesizer.dac, (uint32_t *)samples_p, SAMPLES_MAX);
    }

    return (0);
}
