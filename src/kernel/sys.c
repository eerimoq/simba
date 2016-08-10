/**
 * @file sys.c
 * @version 6.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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

struct sys_t sys = {
    .tick = 0,
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

    "        assert=" STRINGIFY(CONFIG_ASSERT) "\r\n"
    "        debug=" STRINGIFY(CONFIG_DEBUG) "\r\n"
    "        fs-cmd-ds18b20-list=" STRINGIFY(CONFIG_FS_CMD_DS18B20_LIST) "\r\n"
    "        fs-cmd-fs-counters-list=" STRINGIFY(CONFIG_FS_CMD_FS_COUNTERS_LIST) "\r\n"
    "        fs-cmd-fs-counters-reset=" STRINGIFY(CONFIG_FS_CMD_FS_COUNTERS_RESET) "\r\n"
    "        fs-cmd-fs-filesystems-append=" STRINGIFY(CONFIG_FS_CMD_FS_FILESYSTEMS_APPEND) "\r\n"
    "        fs-cmd-fs-filesystems-list=" STRINGIFY(CONFIG_FS_CMD_FS_FILESYSTEMS_LIST) "\r\n"
    "        fs-cmd-fs-filesystems-read=" STRINGIFY(CONFIG_FS_CMD_FS_FILESYSTEMS_READ) "\r\n"
    "        fs-cmd-fs-filesystems-write=" STRINGIFY(CONFIG_FS_CMD_FS_FILESYSTEMS_WRITE) "\r\n"
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
    "        fs-cmd-setting-list=" STRINGIFY(CONFIG_FS_CMD_SETTING_LIST) "\r\n"
    "        fs-cmd-setting-read=" STRINGIFY(CONFIG_FS_CMD_SETTING_READ) "\r\n"
    "        fs-cmd-setting-reset=" STRINGIFY(CONFIG_FS_CMD_SETTING_RESET) "\r\n"
    "        fs-cmd-setting-write=" STRINGIFY(CONFIG_FS_CMD_SETTING_WRITE) "\r\n"
    "        fs-cmd-sys-info=" STRINGIFY(CONFIG_FS_CMD_SYS_INFO) "\r\n"
    "        fs-cmd-sys-uptime=" STRINGIFY(CONFIG_FS_CMD_SYS_UPTIME) "\r\n"
    "        fs-cmd-thrd-list=" STRINGIFY(CONFIG_FS_CMD_THRD_LIST) "\r\n"
    "        fs-cmd-thrd-set-log-mask=" STRINGIFY(CONFIG_FS_CMD_THRD_SET_LOG_MASK) "\r\n"
    "        fs-cmd-usb-device-list=" STRINGIFY(CONFIG_FS_CMD_USB_DEVICE_LIST) "\r\n"
    "        fs-cmd-usb-host-list=" STRINGIFY(CONFIG_FS_CMD_USB_HOST_LIST) "\r\n"
    "        monitor-thread=" STRINGIFY(CONFIG_MONITOR_THREAD) "\r\n"
    "        preemptive-scheduler=" STRINGIFY(CONFIG_PREEMPTIVE_SCHEDULER) "\r\n"
    "        profile-stack=" STRINGIFY(CONFIG_PROFILE_STACK) "\r\n"
    "        setting-area-size=" STRINGIFY(CONFIG_SETTING_AREA_SIZE) "\r\n"
    "        shell-command-max=" STRINGIFY(CONFIG_SHELL_COMMAND_MAX) "\r\n"
    "        shell-history-size=" STRINGIFY(CONFIG_SHELL_HISTORY_SIZE) "\r\n"
    "        shell-minimal=" STRINGIFY(CONFIG_SHELL_MINIMAL) "\r\n"
    "        shell-prompt=" STRINGIFY(CONFIG_SHELL_PROMPT) "\r\n"
    "        start-console=" STRINGIFY(CONFIG_START_CONSOLE) "\r\n"
    "        start-console-device-index=" STRINGIFY(CONFIG_START_CONSOLE_DEVICE_INDEX) "\r\n"
    "        start-console-uart-baudrate=" STRINGIFY(CONFIG_START_CONSOLE_UART_BAUDRATE) "\r\n"
    "        start-console-usb-cdc-control-interface=" STRINGIFY(CONFIG_START_CONSOLE_USB_CDC_CONTROL_INTERFACE) "\r\n"
    "        start-console-usb-cdc-endpoint-in=" STRINGIFY(CONFIG_START_CONSOLE_USB_CDC_ENDPOINT_IN) "\r\n"
    "        start-console-usb-cdc-endpoint-out=" STRINGIFY(CONFIG_START_CONSOLE_USB_CDC_ENDPOINT_OUT) "\r\n"
    "        start-console-usb-cdc-wait-for-connetion=" STRINGIFY(CONFIG_START_CONSOLE_USB_CDC_WAIT_FOR_CONNETION) "\r\n"
    "        start-filesystem=" STRINGIFY(CONFIG_START_FILESYSTEM) "\r\n"
    "        start-network-manager=" STRINGIFY(CONFIG_START_NETWORK_MANAGER) "\r\n"
    "        start-shell=" STRINGIFY(CONFIG_START_SHELL) "\r\n"
    "        start-shell-prio=" STRINGIFY(CONFIG_START_SHELL_PRIO) "\r\n"
    "        start-shell-stack-size=" STRINGIFY(CONFIG_START_SHELL_STACK_SIZE) "\r\n"
    "        std-output-buffer-max=" STRINGIFY(CONFIG_STD_OUTPUT_BUFFER_MAX) "\r\n"
    "        system-tick-frequency=" STRINGIFY(CONFIG_SYSTEM_TICK_FREQUENCY) "\r\n"
    "        usb-device-vid=" STRINGIFY(CONFIG_USB_DEVICE_VID) "\r\n"
    "        usb-device-pid=" STRINGIFY(CONFIG_USB_DEVICE_PID) "\r\n"

#endif

    "";

#if CONFIG_START_SHELL == 1

static struct shell_t shell;
static THRD_STACK(shell_stack, CONFIG_START_SHELL_STACK_SIZE);

#endif

extern void time_tick_isr(void);
extern void timer_tick_isr(void);
extern void thrd_tick_isr(void);
extern const FAR char sysinfo[];

#if CONFIG_START_FILESYSTEM == 1

#define PHYS_ERASE_BLOCK       0x100

#define LOG_BLOCK_SIZE           256
#define LOG_PAGE_SIZE            128

#define FILE_SIZE_MAX           8192
#define CHUNK_SIZE_MAX          1024

struct filesystem_t {
    struct flash_driver_t flash;
    struct spiffs_t fs;
    struct spiffs_config_t config;
    uint8_t workspace[2 * LOG_PAGE_SIZE];
    uint8_t fdworkspace[128];
    uint8_t cache[256];
    struct fs_filesystem_t spiffsfs;
};

static struct filesystem_t filesystem;

static int hal_init(void)
{
    if (flash_module_init() != 0) {
        std_printf(FSTR("Failed to initialize the flash module.\r\n"));
        return (-1);
    }
    
    if (flash_init(&filesystem.flash, &flash_0_dev) != 0) {
        std_printf(FSTR("Failed to initialize the flash driver.\r\n"));
        return (-1);
    }

    return (0);
}


static int32_t hal_read(struct spiffs_t *fs_p,
                        uint32_t addr,
                        uint32_t size,
                        uint8_t *dst_p)
{
    if (flash_read(&filesystem.flash, dst_p, addr, size) != size) {
        return (-1);
    }

    return (0);
}

static int32_t hal_write(struct spiffs_t *fs_p,
                         uint32_t addr,
                         uint32_t size,
                         uint8_t *src_p)
{
    if (flash_write(&filesystem.flash, addr, src_p, size) != size) {
        return (-1);
    }

    return (0);
}

static int32_t hal_erase(struct spiffs_t *fs_p,
                         uint32_t addr,
                         uint32_t size)
{
    char buf[PHYS_ERASE_BLOCK];

    memset(buf, -1, sizeof(buf));

    if (flash_write(&filesystem.flash, addr, buf, size) != size) {
        return (-1);
    }

    return (0);
}

/**
 * Start the file system.
 */
static int start_filesystem(void)
{
    int res;

    if (hal_init() != 0) {
        return (-1);
    }
    
    std_printf(FSTR("Trying to mount the file system.\r\n"));

    /* Initiate the config struct. */
    filesystem.config.hal_read_f = hal_read;
    filesystem.config.hal_write_f = hal_write;
    filesystem.config.hal_erase_f = hal_erase;
    filesystem.config.phys_size = CONFIG_START_FILESYSTEM_SIZE;
    filesystem.config.phys_addr = CONFIG_START_FILESYSTEM_ADDRESS;
    filesystem.config.phys_erase_block = PHYS_ERASE_BLOCK;
    filesystem.config.log_block_size = LOG_BLOCK_SIZE;
    filesystem.config.log_page_size = LOG_PAGE_SIZE;
    
    /* Mount the file system to initialize the runtime variables. */
    res = spiffs_mount(&filesystem.fs,
                       &filesystem.config,
                       filesystem.workspace,
                       filesystem.fdworkspace,
                       sizeof(filesystem.fdworkspace),
                       filesystem.cache,
                       sizeof(filesystem.cache),
                       NULL);

    if (res != 0) {
        std_printf(FSTR("Failed to mount the file system. Formatting it.\r\n"));

        res = spiffs_format(&filesystem.fs);

        if (res != 0) {
            std_printf(FSTR("Failed to mount the file system. Formatting it.\r\n"));
            return (-1);
        }
    
        std_printf(FSTR("Trying to mount the file system after formatting it.\r\n"));
        
        res = spiffs_mount(&filesystem.fs,
                           &filesystem.config,
                           filesystem.workspace,
                           filesystem.fdworkspace,
                           sizeof(filesystem.fdworkspace),
                           filesystem.cache,
                           sizeof(filesystem.cache),
                           NULL);
        
        if (res != 0) {
            std_printf(FSTR("Failed to mount the file system.\r\n"));
            return (-1);
        }
    }

    /* Register the SPIFFS file system in the fs module. */
    fs_filesystem_init(&filesystem.spiffsfs,
                       FSTR("/fs"),
                       fs_type_spiffs_t,
                       &filesystem.fs);

    if (fs_filesystem_register(&filesystem.spiffsfs) != 0) {
        std_printf(FSTR("Failed to register the file system into the debug file system.\r\n"));
        return (-1);
    }

    return (0);
}

#endif

static void sys_tick(void) {
    sys.tick++;
    time_tick_isr();
    timer_tick_isr();
    thrd_tick_isr();
}

#include "sys_port.i"

#if CONFIG_FS_CMD_SYS_INFO == 1

static struct fs_command_t cmd_info;

static int cmd_info_cb(int argc,
                       const char *argv[],
                       chan_t *out_p,
                       chan_t *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    std_fprintf(out_p, sysinfo);
    std_fprintf(out_p, config);

    return (0);
}

#endif

#if CONFIG_FS_CMD_SYS_UPTIME == 1

static struct fs_command_t cmd_uptime;

static int cmd_uptime_cb(int argc,
                         const char *argv[],
                         chan_t *out_p,
                         chan_t *in_p,
                         void *arg_p,
                         void *call_arg_p)
{
    struct time_t now;

    time_get(&now);

    std_fprintf(out_p,
                FSTR("%lu.%lu seconds\r\n"),
                now.seconds,
                now.nanoseconds / 1000000ul);

    return (0);
}

#endif

int sys_module_init(void)
{
#if CONFIG_FS_CMD_SYS_INFO == 1

    fs_command_init(&cmd_info,
                    FSTR("/kernel/sys/info"),
                    cmd_info_cb,
                    NULL);
    fs_command_register(&cmd_info);

#endif

#if CONFIG_FS_CMD_SYS_UPTIME == 1

    fs_command_init(&cmd_uptime,
                    FSTR("/kernel/sys/uptime"),
                    cmd_uptime_cb,
                    NULL);
    fs_command_register(&cmd_uptime);

#endif

    return (sys_port_module_init());
}

int sys_start(void)
{
    rwlock_module_init();
    fs_module_init();
    setting_module_init();
    std_module_init();
    sem_module_init();
    timer_module_init();
    log_module_init();
    chan_module_init();
    thrd_module_init();
    shell_module_init();
    sys_module_init();

#if CONFIG_START_CONSOLE != CONFIG_START_CONSOLE_NONE

    console_module_init();
    console_init();
    console_start();

    sys_set_stdin(console_get_input_channel());
    sys_set_stdout(console_get_output_channel());
    log_set_default_handler_output_channel(console_get_output_channel());

#endif

#if CONFIG_START_SHELL == 1

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
#endif

# if CONFIG_START_FILESYSTEM == 1

    start_filesystem();

#endif

    return (0);
}

void sys_set_on_fatal_callback(void (*callback)(int error))
{
    sys.on_fatal_callback = callback;
}

void sys_set_stdin(chan_t *chan_p)
{
    sys.stdin_p = chan_p;
}

chan_t *sys_get_stdin()
{
    return (sys.stdin_p);
}

void sys_set_stdout(chan_t *chan_p)
{
    sys.stdout_p = chan_p;
}

chan_t *sys_get_stdout()
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

void sys_lock_isr()
{
    sys_port_lock_isr();
}

void sys_unlock_isr()
{
    sys_port_unlock_isr();
}

const FAR char *sys_get_info()
{
    return (sysinfo);
}

const FAR char *sys_get_config()
{
    return (config);
}

float sys_interrupt_cpu_usage_get()
{
    return (sys_port_interrupt_cpu_usage_get());
}

void sys_interrupt_cpu_usage_reset()
{
    sys_port_interrupt_cpu_usage_reset();
}
