#include "simba.h"
int eeprom_i2c_write_buf(struct i2c_soft_driver_t *self_p,int device, int addr, char *buf, int size);
int eeprom_i2c_read_buf (struct i2c_soft_driver_t *self_p,int device, int addr, char *buf, int size);
int eeprom_i2c_init(struct i2c_soft_driver_t *self_p,
                  struct pin_device_t *scl_dev_p,
                  struct pin_device_t *sda_dev_p,
                  long baudrate,
                  long max_clock_stretching_us,
                  long clock_stretching_sleep_us);
int eeprom_i2c_module_init();