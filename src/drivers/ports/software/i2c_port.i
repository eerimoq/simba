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

int i2c_port_module_init()
{
    return (i2c_soft_module_init());
}

int i2c_port_init(struct i2c_driver_t *self_p,
                  struct i2c_device_t *dev_p,
                  int baudrate,
                  int address)
{
    return (i2c_soft_init(&self_p->soft,
                          dev_p->scl_p,
                          dev_p->sda_p,
                          baudrate,
                          dev_p->max_clock_stretching_us,
                          dev_p->clock_stretching_sleep_us));
}

int i2c_port_start(struct i2c_driver_t *self_p)
{
    return (i2c_soft_start(&self_p->soft));
}

int i2c_port_stop(struct i2c_driver_t *self_p)
{
    return (i2c_soft_stop(&self_p->soft));
}

ssize_t i2c_port_read(struct i2c_driver_t *self_p,
                      int address,
                      void *buf_p,
                      size_t size)
{
    return (i2c_soft_read(&self_p->soft, address, buf_p, size));
}

ssize_t i2c_port_write(struct i2c_driver_t *self_p,
                       int address,
                       const void *buf_p,
                       size_t size)
{
    return (i2c_soft_write(&self_p->soft, address, buf_p, size));
}

int i2c_port_scan(struct i2c_driver_t *self_p,
                  int address)
{
    return (i2c_soft_scan(&self_p->soft, address));
}

/**
 * The slave interface is not implemented in this port.
 */

int i2c_port_slave_start(struct i2c_driver_t *self_p)
{
    return (-ENOSYS);
}

int i2c_port_slave_stop(struct i2c_driver_t *self_p)
{
    return (-ENOSYS);
}

ssize_t i2c_port_slave_read(struct i2c_driver_t *self_p,
                            void *buf_p,
                            size_t size)
{
    return (-ENOSYS);
}

ssize_t i2c_port_slave_write(struct i2c_driver_t *self_p,
                             const void *buf_p,
                             size_t size)
{
    return (-ENOSYS);
}
