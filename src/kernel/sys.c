/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

struct sys_t sys = {
    .on_fatal_callback = sys_stop,
    .stdin_p = NULL,
    .stdout_p = NULL,
    .interrupt = {
        .start = 0,
        .time = 0
    }
};

struct module_t {
    int initialized;
#if CONFIG_FS_CMD_SYS_INFO == 1
    struct fs_command_t cmd_info;
#endif
#if CONFIG_FS_CMD_SYS_CONFIG == 1
    struct fs_command_t cmd_config;
#endif
#if CONFIG_FS_CMD_SYS_UPTIME == 1
    struct fs_command_t cmd_uptime;
#endif
#if CONFIG_FS_CMD_SYS_REBOOT == 1
    struct fs_command_t cmd_reboot;
#endif
};

static struct module_t module;

static const FAR char config[] =
    "config: sys-config-string=" STRINGIFY(CONFIG_SYS_CONFIG_STRING) "\r\n"

#if CONFIG_SYS_CONFIG_STRING == 1

    "        assert=" STRINGIFY(CONFIG_ASSERT) "\r\n"
    "        debug=" STRINGIFY(CONFIG_DEBUG) "\r\n"
    "        fs-cmd-ds18b20-list=" STRINGIFY(CONFIG_FS_CMD_DS18B20_LIST) "\r\n"
    "        fs-cmd-fs-counters-list=" STRINGIFY(CONFIG_FS_CMD_FS_COUNTERS_LIST) "\r\n"
    "        fs-cmd-fs-counters-reset=" STRINGIFY(CONFIG_FS_CMD_FS_COUNTERS_RESET) "\r\n"
    "        fs-cmd-fs-filesystems-append=" STRINGIFY(CONFIG_FS_CMD_FS_APPEND) "\r\n"
    "        fs-cmd-fs-filesystems-list=" STRINGIFY(CONFIG_FS_CMD_FS_LIST) "\r\n"
    "        fs-cmd-fs-filesystems-read=" STRINGIFY(CONFIG_FS_CMD_FS_READ) "\r\n"
    "        fs-cmd-fs-filesystems-write=" STRINGIFY(CONFIG_FS_CMD_FS_WRITE) "\r\n"
    "        fs-cmd-fs-parameters-list=" STRINGIFY(CONFIG_FS_CMD_FS_PARAMETERS_LIST) "\r\n"
    "        fs-cmd-i2c-read=" STRINGIFY(CONFIG_FS_CMD_I2C_READ) "\r\n"
    "        fs-cmd-i2c-write=" STRINGIFY(CONFIG_FS_CMD_I2C_WRITE) "\r\n"
    "        fs-cmd-log-list=" STRINGIFY(CONFIG_FS_CMD_LOG_LIST) "\r\n"
    "        fs-cmd-log-print=" STRINGIFY(CONFIG_FS_CMD_LOG_PRINT) "\r\n"
    "        fs-cmd-log-set-log-mask=" STRINGIFY(CONFIG_FS_CMD_LOG_SET_LOG_MASK) "\r\n"
    "        fs-cmd-network-interface-list=" STRINGIFY(CONFIG_FS_CMD_NETWORK_INTERFACE_LIST) "\r\n"
    "        fs-cmd-pin-read=" STRINGIFY(CONFIG_FS_CMD_PIN_READ) "\r\n"
    "        fs-cmd-pin-set-mode=" STRINGIFY(CONFIG_FS_CMD_PIN_SET_MODE) "\r\n"
    "        fs-cmd-pin-write=" STRINGIFY(CONFIG_FS_CMD_PIN_WRITE) "\r\n"
    "        fs-cmd-settings-list=" STRINGIFY(CONFIG_FS_CMD_SETTINGS_LIST) "\r\n"
    "        fs-cmd-settings-read=" STRINGIFY(CONFIG_FS_CMD_SETTINGS_READ) "\r\n"
    "        fs-cmd-settings-reset=" STRINGIFY(CONFIG_FS_CMD_SETTINGS_RESET) "\r\n"
    "        fs-cmd-settings-write=" STRINGIFY(CONFIG_FS_CMD_SETTINGS_WRITE) "\r\n"
    "        fs-cmd-sys-info=" STRINGIFY(CONFIG_FS_CMD_SYS_INFO) "\r\n"
    "        fs-cmd-sys-uptime=" STRINGIFY(CONFIG_FS_CMD_SYS_UPTIME) "\r\n"
    "        fs-cmd-thrd-list=" STRINGIFY(CONFIG_FS_CMD_THRD_LIST) "\r\n"
    "        fs-cmd-thrd-set-log-mask=" STRINGIFY(CONFIG_FS_CMD_THRD_SET_LOG_MASK) "\r\n"
    "        fs-cmd-usb-device-list=" STRINGIFY(CONFIG_FS_CMD_USB_DEVICE_LIST) "\r\n"
    "        fs-cmd-usb-host-list=" STRINGIFY(CONFIG_FS_CMD_USB_HOST_LIST) "\r\n"
    "        monitor-thread=" STRINGIFY(CONFIG_MONITOR_THREAD) "\r\n"
    "        preemptive-scheduler=" STRINGIFY(CONFIG_PREEMPTIVE_SCHEDULER) "\r\n"
    "        profile-stack=" STRINGIFY(CONFIG_PROFILE_STACK) "\r\n"
    "        settings-area-size=" STRINGIFY(CONFIG_SETTINGS_AREA_SIZE) "\r\n"
    "        shell-command-max=" STRINGIFY(CONFIG_SHELL_COMMAND_MAX) "\r\n"
    "        shell-history-size=" STRINGIFY(CONFIG_SHELL_HISTORY_SIZE) "\r\n"
    "        shell-minimal=" STRINGIFY(CONFIG_SHELL_MINIMAL) "\r\n"
    "        shell-prompt=" STRINGIFY(CONFIG_SHELL_PROMPT) "\r\n"
    "        spiffs=" STRINGIFY(CONFIG_SPIFFS) "\r\n"
    "        start-console=" STRINGIFY(CONFIG_START_CONSOLE) "\r\n"
    "        start-console-device-index=" STRINGIFY(CONFIG_START_CONSOLE_DEVICE_INDEX) "\r\n"
    "        start-console-uart-baudrate=" STRINGIFY(CONFIG_START_CONSOLE_UART_BAUDRATE) "\r\n"
    "        start-console-usb-cdc-control-interface=" STRINGIFY(CONFIG_START_CONSOLE_USB_CDC_CONTROL_INTERFACE) "\r\n"
    "        start-console-usb-cdc-endpoint-in=" STRINGIFY(CONFIG_START_CONSOLE_USB_CDC_ENDPOINT_IN) "\r\n"
    "        start-console-usb-cdc-endpoint-out=" STRINGIFY(CONFIG_START_CONSOLE_USB_CDC_ENDPOINT_OUT) "\r\n"
    "        start-console-usb-cdc-wait-for-connetion=" STRINGIFY(CONFIG_START_CONSOLE_USB_CDC_WAIT_FOR_CONNETION) "\r\n"
    "        start-filesystem=" STRINGIFY(CONFIG_START_FILESYSTEM) "\r\n"
    "        start-filesystem-address=" STRINGIFY(CONFIG_START_FILESYSTEM_ADDRESS) "\r\n"
    "        start-filesystem-size=" STRINGIFY(CONFIG_START_FILESYSTEM_SIZE) "\r\n"
    "        start-network=" STRINGIFY(CONFIG_START_NETWORK) "\r\n"
    "        start-network-interface-wifi-ssid=" STRINGIFY(CONFIG_START_NETWORK_INTERFACE_WIFI_SSID) "\r\n"
    "        start-network-interface-wifi-password=********\r\n"
    "        start-shell=" STRINGIFY(CONFIG_START_SHELL) "\r\n"
    "        start-shell-prio=" STRINGIFY(CONFIG_START_SHELL_PRIO) "\r\n"
    "        start-shell-stack-size=" STRINGIFY(CONFIG_START_SHELL_STACK_SIZE) "\r\n"
    "        std-output-buffer-max=" STRINGIFY(CONFIG_STD_OUTPUT_BUFFER_MAX) "\r\n"
    "        system-tick-frequency=" STRINGIFY(CONFIG_SYSTEM_TICK_FREQUENCY) "\r\n"
    "        usb-device-vid=" STRINGIFY(CONFIG_USB_DEVICE_VID) "\r\n"
    "        usb-device-pid=" STRINGIFY(CONFIG_USB_DEVICE_PID) "\r\n"

#endif

    "";

extern const FAR char sysinfo[];

extern void time_tick_isr(void);
extern void timer_tick_isr(void);
extern void thrd_tick_isr(void);

static void RAM_CODE sys_tick_isr(void)
{
    time_tick_isr();
    timer_tick_isr();
    thrd_tick_isr();
}

#include "sys_port.i"

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

#if CONFIG_MODULE_INIT_SDIO == 1
    sdio_module_init();
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
    struct time_t now;

    time_get(&now);

    std_fprintf(out_p,
                OSTR("%lu.%lu seconds\r\n"),
                now.seconds,
                now.nanoseconds / 1000000ul);

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

#if CONFIG_FS_CMD_SYS_REBOOT == 1
    fs_command_init(&module.cmd_reboot,
                    CSTR("/kernel/sys/reboot"),
                    cmd_reboot_cb,
                    NULL);
    fs_command_register(&module.cmd_reboot);
#endif

    return (sys_port_module_init());
}

int sys_start(void)
{
#if CONFIG_MODULE_INIT_RWLOCK == 1
    rwlock_module_init();
#endif
#if CONFIG_MODULE_INIT_FS == 1
    fs_module_init();
#endif
#if CONFIG_MODULE_INIT_SETTINGS == 1
    settings_module_init();
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

# if CONFIG_START_FILESYSTEM == 1
    start_filesystem();
#endif

# if CONFIG_START_NETWORK == 1
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

void sys_reboot(void)
{
    sys_port_reboot();
}

void sys_set_on_fatal_callback(void (*callback)(int error))
{
    sys.on_fatal_callback = callback;
}

void sys_set_stdin(void *chan_p)
{
    sys.stdin_p = chan_p;
}

void *sys_get_stdin()
{
    return (sys.stdin_p);
}

void sys_set_stdout(void *chan_p)
{
    sys.stdout_p = chan_p;
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
