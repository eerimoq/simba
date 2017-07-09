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
#include <limits.h>

#define SECONDS_PER_MSB (INT_MAX / CONFIG_SYSTEM_TICK_FREQUENCY)
#define TICKS_PER_MSB   (SECONDS_PER_MSB * CONFIG_SYSTEM_TICK_FREQUENCY)

#if CONFIG_SYS_LOG_MASK > 0
#    define LOG_OBJECT_PRINT(...) log_object_print(__VA_ARGS__)
#else
#    define LOG_OBJECT_PRINT(...)
#endif

/* 64 bits so it does not wrap around during the system's uptime. */
struct tick_t {
    uint32_t msb;
    uint32_t lsb;
};

struct module_t {
    int initialized;
    struct tick_t tick;
    enum sys_reset_cause_t reset_cause;
#if CONFIG_FS_CMD_SYS_INFO == 1
    struct fs_command_t cmd_info;
#endif
#if CONFIG_FS_CMD_SYS_CONFIG == 1
    struct fs_command_t cmd_config;
#endif
#if CONFIG_FS_CMD_SYS_UPTIME == 1
    struct fs_command_t cmd_uptime;
#endif
#if CONFIG_FS_CMD_SYS_PANIC == 1
    struct fs_command_t cmd_panic;
#endif
#if CONFIG_FS_CMD_SYS_REBOOT == 1
    struct fs_command_t cmd_reboot;
#endif
#if CONFIG_FS_CMD_SYS_BACKTRACE == 1
    struct fs_command_t cmd_backtrace;
#endif
#if CONFIG_FS_CMD_SYS_RESET_CAUSE == 1
    struct fs_command_t cmd_reset_cause;
#endif
};

static struct module_t module;

struct sys_t sys = {
    .on_fatal_callback = sys_stop,
    .stdin_p = NULL,
    .stdout_p = NULL,
    .interrupt = {
        .start = 0,
        .time = 0
    }
};

static const FAR char config[] =
    "config: sys-config-string=" STRINGIFY(CONFIG_SYS_CONFIG_STRING) "\r\n"

#if CONFIG_SYS_CONFIG_STRING == 1

    "        fatal-assert=" STRINGIFY(CONFIG_FATAL_ASSERT) "\r\n"
    "        assert=" STRINGIFY(CONFIG_ASSERT) "\r\n"
    "        debug=" STRINGIFY(CONFIG_DEBUG) "\r\n"
    "        preemptive-scheduler=" STRINGIFY(CONFIG_PREEMPTIVE_SCHEDULER) "\r\n"
    "        profile-stack=" STRINGIFY(CONFIG_PROFILE_STACK) "\r\n"
    "        system-tick-frequency=" STRINGIFY(CONFIG_SYSTEM_TICK_FREQUENCY) "\r\n"

#endif

    "";

const char *sys_reset_cause_string_map[sys_reset_cause_max_t] = {
    "unknown",
    "power_on",
    "watchdog_timeout",
    "software",
    "external",
    "jtag",
#if defined(SYS_PORT_RESET_CAUSE_STRINGS_MAP)
    SYS_PORT_RESET_CAUSE_STRINGS_MAP
#endif
};

extern const FAR char sysinfo[];

extern void time_tick_isr(void);
extern void timer_tick_isr(void);
extern void thrd_tick_isr(void);

static void RAM_CODE sys_tick_isr(void)
{
    module.tick.lsb++;

    if (module.tick.lsb == TICKS_PER_MSB) {
        module.tick.msb++;
        module.tick.lsb = 0;
    }

    timer_tick_isr();
    thrd_tick_isr();
}

#include "sys_port.i"

static void tick_to_time(struct time_t *time_p,
                         struct tick_t *tick_p)
{
    uint32_t lsb_seconds;
    uint32_t lsb_ticks;

    lsb_seconds = (tick_p->lsb / CONFIG_SYSTEM_TICK_FREQUENCY);
    lsb_ticks = (tick_p->lsb - (CONFIG_SYSTEM_TICK_FREQUENCY * lsb_seconds));

    time_p->seconds = (tick_p->msb * SECONDS_PER_MSB + lsb_seconds);
    time_p->nanoseconds = (1000 * ((1000000UL * lsb_ticks)
                                   / CONFIG_SYSTEM_TICK_FREQUENCY));
}

static void init_drivers(void)
{
#if CONFIG_MODULE_INIT_ADC == 1
    adc_module_init();
#endif

#if CONFIG_MODULE_INIT_ANALOG_INPUT_PIN == 1
    analog_input_pin_module_init();
#endif

#if CONFIG_MODULE_INIT_ANALOG_OUTPUT_PIN == 1
    analog_output_pin_module_init();
#endif

#if CONFIG_MODULE_INIT_CAN == 1
    can_module_init();
#endif

#if CONFIG_MODULE_INIT_CHIPID == 1
#endif

#if CONFIG_MODULE_INIT_DAC == 1
    dac_module_init();
#endif

#if CONFIG_MODULE_INIT_DS18B20 == 1
    ds18b20_module_init();
#endif

#if CONFIG_MODULE_INIT_DS3231 == 1
#endif

#if CONFIG_MODULE_INIT_ESP_WIFI == 1
    esp_wifi_module_init();
#endif

#if CONFIG_MODULE_INIT_EXTI == 1
    exti_module_init();
#endif

#if CONFIG_MODULE_INIT_FLASH == 1
    flash_module_init();
#endif

#if CONFIG_MODULE_INIT_I2C == 1
    i2c_module_init();
#endif

#if CONFIG_MODULE_INIT_I2C_SOFT == 1
    i2c_soft_module_init();
#endif

#if CONFIG_MODULE_INIT_MCP2515 == 1
#endif

#if CONFIG_MODULE_INIT_NRF24L01 == 1
    nrf24l01_module_init();
#endif

#if CONFIG_MODULE_INIT_OWI == 1
#endif

#if CONFIG_MODULE_INIT_PIN == 1
    pin_module_init();
#endif

#if CONFIG_MODULE_INIT_PWM == 1
    pwm_module_init();
#endif

#if CONFIG_MODULE_INIT_PWM_SOFT == 1
    pwm_soft_module_init(500);
#endif

#if CONFIG_MODULE_INIT_SD == 1
#endif

#if CONFIG_MODULE_INIT_SPI == 1
    spi_module_init();
#endif

#if CONFIG_MODULE_INIT_UART == 1
    uart_module_init();
#endif

#if CONFIG_MODULE_INIT_UART_SOFT == 1
#endif

#if CONFIG_MODULE_INIT_USB_DEVICE == 1
    usb_device_module_init();
#endif

#if CONFIG_MODULE_INIT_USB_HOST == 1
    usb_host_module_init();
#endif

#if CONFIG_MODULE_INIT_WATCHDOG == 1
    watchdog_module_init();
#endif

#if CONFIG_MODULE_INIT_RANDOM == 1
    random_module_init();
#endif
}

static void init_inet(void)
{
#if CONFIG_MODULE_INIT_INET == 1
    inet_module_init();
#endif

#if CONFIG_MODULE_INIT_PING == 1
    ping_module_init();
#endif

#if CONFIG_MODULE_INIT_SOCKET == 1
    socket_module_init();
#endif

#if CONFIG_MODULE_INIT_NETWORK_INTERFACE == 1
    network_interface_module_init();
#endif

#if CONFIG_MODULE_INIT_SSL == 1
    ssl_module_init();
#endif
}

#if CONFIG_START_CONSOLE != CONFIG_START_CONSOLE_NONE

static int start_console(void)
{
    console_module_init();
    console_init();
    console_start();

    sys_set_stdin(console_get_input_channel());
    sys_set_stdout(console_get_output_channel());
    log_set_default_handler_output_channel(console_get_output_channel());

    return (0);
}

#endif

#if CONFIG_START_SHELL == 1

static struct shell_t shell;
static THRD_STACK(shell_stack, CONFIG_START_SHELL_STACK_SIZE);

static int start_shell(void)
{
    shell_init(&shell,
               sys_get_stdin(),
               sys_get_stdout(),
               NULL,
               NULL,
               NULL,
               NULL);

    thrd_spawn(shell_main,
               &shell,
               CONFIG_START_SHELL_PRIO,
               shell_stack,
               sizeof(shell_stack));

    return (0);
}

#endif

#if CONFIG_START_SOAM == 1

static struct slip_t slip;
static uint8_t slip_buf[128];

static struct soam_t soam;
static uint8_t soam_buf[128];

static THRD_STACK(soam_stack, CONFIG_START_SOAM_STACK_SIZE);

/**
 * This thread executes file system commands.
 */
static void *soam_main(void *arg_p)
{
    uint8_t byte;
    ssize_t size;

    thrd_set_name("soam");

    while (1) {
        chan_read(sys_get_stdin(), &byte, sizeof(byte));

        size = slip_input(&slip, byte);

        if (size > 0) {
            soam_input(&soam, &slip_buf[0], size);
        }
    }

    return (NULL);
}

static int start_soam(void)
{
    if (slip_init(&slip,
                  &slip_buf[0],
                  sizeof(slip_buf),
                  sys_get_stdout()) != 0) {
        return (-1);
    }

    if (soam_init(&soam,
                  &soam_buf[0],
                  sizeof(soam_buf),
                  slip_get_output_channel(&slip)) != 0) {
        return (-1);
    }

    log_set_default_handler_output_channel(soam_get_log_input_channel(&soam));
    sys_set_stdout(soam_get_stdout_input_channel(&soam));

    thrd_spawn(soam_main,
               &soam,
               CONFIG_START_SOAM_PRIO,
               soam_stack,
               sizeof(soam_stack));

    return (0);
}

#endif

#if CONFIG_START_FILESYSTEM == 1
#    include "sys/filesystem.i"
#endif

#if CONFIG_START_NETWORK == 1
#    include "sys/network.i"
#endif

#if CONFIG_START_NVM == 1
#    include "sys/nvm.i"
#endif

#if CONFIG_FS_CMD_SYS_INFO == 1

static int cmd_info_cb(int argc,
                       const char *argv[],
                       void *out_p,
                       void *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    std_fprintf(out_p, sysinfo);

    return (0);
}

#endif

#if CONFIG_FS_CMD_SYS_CONFIG == 1

static int cmd_config_cb(int argc,
                         const char *argv[],
                         void *out_p,
                         void *in_p,
                         void *arg_p,
                         void *call_arg_p)
{
    std_fprintf(out_p, config);

    return (0);
}

#endif

#if CONFIG_FS_CMD_SYS_UPTIME == 1

static int cmd_uptime_cb(int argc,
                         const char *argv[],
                         void *out_p,
                         void *in_p,
                         void *arg_p,
                         void *call_arg_p)
{
    struct time_t uptime;

    sys_uptime(&uptime);

    std_fprintf(out_p,
                OSTR("%lu.%03lu seconds\r\n"),
                uptime.seconds,
                uptime.nanoseconds / 1000000ul);

    return (0);
}

#endif

#if CONFIG_FS_CMD_SYS_PANIC == 1

static int cmd_panic_cb(int argc,
                         const char *argv[],
                         void *out_p,
                         void *in_p,
                         void *arg_p,
                         void *call_arg_p)
{
    sys_panic("Panic!\r\n");

    return (0);
}

#endif

#if CONFIG_FS_CMD_SYS_REBOOT == 1

static int cmd_reboot_cb(int argc,
                         const char *argv[],
                         void *out_p,
                         void *in_p,
                         void *arg_p,
                         void *call_arg_p)
{
    sys_reboot();

    return (0);
}

#endif

#if CONFIG_FS_CMD_SYS_BACKTRACE == 1

static int cmd_backtrace_cb(int argc,
                            const char *argv[],
                            void *out_p,
                            void *in_p,
                            void *arg_p,
                            void *call_arg_p)
{
    int i;
    int depth;
    void *buf[32];

    depth = sys_backtrace(buf, sizeof(buf));

    std_fprintf(out_p, OSTR("Backtrace: "));

    for (i = 0; i < depth; i++) {
        std_fprintf(out_p,
                    OSTR("0x%08x:0x%08x "),
                    buf[2 * i],
                    buf[2 * i + 1]);
    }

    std_fprintf(out_p, OSTR("\r\n"));

    return (0);
}

#endif

#if CONFIG_FS_CMD_SYS_RESET_CAUSE == 1

static int cmd_reset_cause_cb(int argc,
                              const char *argv[],
                              void *out_p,
                              void *in_p,
                              void *arg_p,
                              void *call_arg_p)
{
    std_fprintf(out_p,
                OSTR("%s\r\n"),
                sys_reset_cause_string_map[sys_reset_cause()]);

    return (0);
}

#endif

int sys_module_init(void)
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

#if CONFIG_FS_CMD_SYS_INFO == 1
    fs_command_init(&module.cmd_info,
                    CSTR("/kernel/sys/info"),
                    cmd_info_cb,
                    NULL);
    fs_command_register(&module.cmd_info);
#endif

#if CONFIG_FS_CMD_SYS_CONFIG == 1
    fs_command_init(&module.cmd_config,
                    CSTR("/kernel/sys/config"),
                    cmd_config_cb,
                    NULL);
    fs_command_register(&module.cmd_config);
#endif

#if CONFIG_FS_CMD_SYS_UPTIME == 1
    fs_command_init(&module.cmd_uptime,
                    CSTR("/kernel/sys/uptime"),
                    cmd_uptime_cb,
                    NULL);
    fs_command_register(&module.cmd_uptime);
#endif

#if CONFIG_FS_CMD_SYS_PANIC == 1
    fs_command_init(&module.cmd_panic,
                    CSTR("/kernel/sys/panic"),
                    cmd_panic_cb,
                    NULL);
    fs_command_register(&module.cmd_panic);
#endif

#if CONFIG_FS_CMD_SYS_REBOOT == 1
    fs_command_init(&module.cmd_reboot,
                    CSTR("/kernel/sys/reboot"),
                    cmd_reboot_cb,
                    NULL);
    fs_command_register(&module.cmd_reboot);
#endif

#if CONFIG_FS_CMD_SYS_BACKTRACE == 1
    fs_command_init(&module.cmd_backtrace,
                    CSTR("/kernel/sys/backtrace"),
                    cmd_backtrace_cb,
                    NULL);
    fs_command_register(&module.cmd_backtrace);
#endif

#if CONFIG_FS_CMD_SYS_RESET_CAUSE == 1
    fs_command_init(&module.cmd_reset_cause,
                    CSTR("/kernel/sys/reset_cause"),
                    cmd_reset_cause_cb,
                    NULL);
    fs_command_register(&module.cmd_reset_cause);
#endif

    return (sys_port_module_init());
}

int sys_start(void)
{
    sys.stdin_p = chan_null();
    sys.stdout_p = chan_null();

#if CONFIG_MODULE_INIT_RWLOCK == 1
    rwlock_module_init();
#endif
#if CONFIG_MODULE_INIT_FS == 1
    fs_module_init();
#endif
#if CONFIG_MODULE_INIT_STD == 1
    std_module_init();
#endif
#if CONFIG_MODULE_INIT_SEM == 1
    sem_module_init();
#endif
#if CONFIG_MODULE_INIT_TIMER == 1
    timer_module_init();
#endif
#if CONFIG_MODULE_INIT_LOG == 1
    log_module_init();
#endif
#if CONFIG_MODULE_INIT_CHAN == 1
    chan_module_init();
#endif
#if CONFIG_MODULE_INIT_THRD == 1
    thrd_module_init();
#endif
#if CONFIG_MODULE_INIT_SHELL == 1
    shell_module_init();
#endif
    sys_module_init();
#if CONFIG_MODULE_INIT_BUS == 1
    bus_module_init();
#endif

    init_drivers();
    init_inet();

#if CONFIG_START_CONSOLE != CONFIG_START_CONSOLE_NONE
    start_console();
#endif

#if CONFIG_START_SHELL == 1
    start_shell();
#endif

#if CONFIG_START_SOAM == 1
    start_soam();
#endif

#if CONFIG_START_NVM == 1
    start_nvm();
#endif

#if CONFIG_MODULE_INIT_SETTINGS == 1
    settings_module_init();
#endif

#if CONFIG_START_FILESYSTEM == 1
    start_filesystem();
#endif

#if CONFIG_START_NETWORK == 1
    start_network();
#endif

#if CONFIG_MODULE_INIT_UPGRADE == 1
    upgrade_module_init();
#endif

    return (0);
}

void sys_stop(int error)
{
    sys_port_stop(error);
}

void sys_panic(const char *message_p)
{
    int i;
    int count;
    void *backtrace[24];
    char buf[23];
    char *buf_p;
    FAR const char *info_p;

#if !defined(ARCH_LINUX)
    sys_lock();
#endif

#if CONFIG_SYS_PANIC_KICK_WATCHDOG == 1
    watchdog_kick();
#endif

    /* Output the message. */
    while (*message_p != '\0') {
        sys_port_panic_putc(*message_p++);
    }

    sys_port_panic_putc(':');
    sys_port_panic_putc(' ');

    /* Output the backtrace. */
    count = sys_backtrace(&backtrace[0], sizeof(backtrace));

    for (i = 0; i < count; i++) {
        std_sprintf(&buf[0],
                    FSTR("0x%08lx:0x%08lx "),
                    (long)(uintptr_t)backtrace[2 * i],
                    (long)(uintptr_t)backtrace[2 * i + 1]);
        buf_p = &buf[0];

        while (*buf_p != '\0') {
            sys_port_panic_putc(*buf_p++);
        }
    }

    /* Print system information. */
    info_p = &sysinfo[0];

    while (*info_p != '\0') {
        sys_port_panic_putc(*info_p++);
    }

    /* Reboot the system. */
    message_p = "Rebooting...";

    while (*message_p != '\0') {
        sys_port_panic_putc(*message_p++);
    }

    sys_reboot();
}

void sys_reboot(void)
{
    sys_port_reboot();
}

int sys_backtrace(void **buf_pp, size_t size)
{
    return (sys_port_backtrace(buf_pp, size));
}

enum sys_reset_cause_t sys_reset_cause()
{
    return (module.reset_cause);
}

int sys_uptime(struct time_t *uptime_p)
{
    ASSERTN(uptime_p != NULL, EINVAL);

    struct tick_t tick;
    struct time_t offset;

    offset.seconds = 0;

    sys_lock();
    tick = module.tick;
    offset.nanoseconds = sys_port_get_time_into_tick();
    sys_unlock();

    tick_to_time(uptime_p, &tick);

    return (time_add(uptime_p, uptime_p, &offset));
}

int sys_uptime_isr(struct time_t *uptime_p)
{
    ASSERTN(uptime_p != NULL, EINVAL);

    struct tick_t tick;
    struct time_t offset;

    offset.seconds = 0;

    tick = module.tick;
    offset.nanoseconds = sys_port_get_time_into_tick();

    tick_to_time(uptime_p, &tick);

    return (time_add(uptime_p, uptime_p, &offset));
}

void sys_set_on_fatal_callback(sys_on_fatal_fn_t callback)
{
    sys.on_fatal_callback = callback;
}

void sys_set_stdin(void *chan_p)
{
    if (chan_p != NULL) {
        sys.stdin_p = chan_p;
    } else {
        sys.stdin_p = chan_null();
    }
}

void *sys_get_stdin()
{
    return (sys.stdin_p);
}

void sys_set_stdout(void *chan_p)
{
    if (chan_p != NULL) {
        sys.stdout_p = chan_p;
    } else {
        sys.stdout_p = chan_null();
    }
}

void *sys_get_stdout()
{
    return (sys.stdout_p);
}

void sys_lock()
{
    sys_port_lock();
}

void sys_unlock()
{
    sys_port_unlock();
}

void RAM_CODE sys_lock_isr()
{
    sys_port_lock_isr();
}

void RAM_CODE sys_unlock_isr()
{
    sys_port_unlock_isr();
}

far_string_t sys_get_info()
{
    return (sysinfo);
}

far_string_t sys_get_config()
{
    return (config);
}

cpu_usage_t sys_interrupt_cpu_usage_get()
{
    return (sys_port_interrupt_cpu_usage_get());
}

void sys_interrupt_cpu_usage_reset()
{
    sys_port_interrupt_cpu_usage_reset();
}
