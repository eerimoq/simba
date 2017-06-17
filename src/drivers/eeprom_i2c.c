#include "simba.h"
#if CONFIG_PUMBAA_CLASS_I2C_SOFT == 1
struct i2c_soft_driver_t i2c;

int eeprom_i2c_init(struct i2c_soft_driver_t *self_p,
                  struct pin_device_t *scl_dev_p,
                  struct pin_device_t *sda_dev_p,
                  long baudrate,
                  long max_clock_stretching_us,
                  long clock_stretching_sleep_us)
{
    return i2c_soft_init(self_p,scl_dev_p,sda_dev_p,baudrate,max_clock_stretching_us,clock_stretching_sleep_us);
}



static int start_cond(struct i2c_soft_driver_t *self_p)
{
    return i2c_soft_start_cond(self_p);

}


static int stop_cond(struct i2c_soft_driver_t *self_p)
{
    return i2c_soft_stop_cond(self_p);

}

static int write_byte(struct i2c_soft_driver_t *self_p,
                      uint8_t byte, int ACK)
{
    return i2c_soft_write_byte(self_p,byte,ACK);
}

/**
 * Read a byte from I2C bus.
 */
static int read_byte(struct i2c_soft_driver_t *self_p,
                     uint8_t *byte_p, uint8_t ACK)
{
    return i2c_soft_read_byte(self_p,byte_p,ACK);
}


#define EEPROM_PAGE_DIM 32
#define EEPROM_BYTE_READ   0b10100001
#define EEPROM_BYTE_WRITE  0b10100000


#define CHK(x) do { \
  int retval = (x); \
  if (retval != 0) { \
    fprintf(stderr, "Eeprom error: %s returned %d at %s:%d", #x, retval, __FILE__, __LINE__); \
    stop_cond(&i2c); \
    return -1 ; \
  } \
} while (0)
int eeprom_i2c_write_buf(struct i2c_soft_driver_t *self_p,int device, int addr, char *buf, int size)
{
    int i; 

    for(i = addr; i < addr + size ; i ++ ) 
    {
        if( i == addr || i % EEPROM_PAGE_DIM == 0 )
        {
            //new page 
            if( i > addr ) 
            {
                //stop for previous stream 
                CHK(stop_cond(self_p));
            }
            //poll the write eeprom write cycle 
            while(1)
            {
                if(start_cond(self_p) == 0 
                   && write_byte(self_p, EEPROM_BYTE_WRITE | (device << 1),1 ) == 0)
                    break ; 
            }
            CHK(write_byte(self_p, (uint8_t)(i >> 8) & 255,1));
            CHK(write_byte(self_p, (uint8_t)i & 255,1));
        }
        //write in the next byte of the current page 
        CHK(write_byte(self_p, buf[i - addr], 1));
        
    }
    CHK(stop_cond(self_p));
    return 0; 
    
}
int eeprom_i2c_read_buf(struct i2c_soft_driver_t *self_p, int device, int addr, char *buf, int size)
{
    int i;
    for(i = addr ; i < addr + size ; i ++ ) 
    {
        if( i == addr || i % EEPROM_PAGE_DIM == 0 )
        {
            if(i > addr ) 
            {
                
            }
            if( i == addr )
            {
                //poll the eeprom write cycle 
                while(1) 
                {
                    if(start_cond(self_p) == 0 
                        && write_byte(self_p, EEPROM_BYTE_WRITE | (device << 1), 1) == 0)
                        break; 
                }
                CHK(write_byte(self_p, (uint8_t)(i >> 8) & 255, 1) );
                CHK(write_byte(self_p, (uint8_t)i & 255,1));
                CHK(start_cond(self_p));
                CHK(write_byte(self_p, EEPROM_BYTE_READ | (device << 1),1));
            }
        }
        if( i == addr + size - 1 ) 
        {
            //read, no ack 
            CHK(read_byte(self_p, (uint8_t *) &buf[i - addr], 0));
        }
        else 
        {
            //sequential read, ack 
            CHK(read_byte(self_p, (uint8_t *) &buf[i - addr], 1));
        }
    }
    CHK(stop_cond(self_p));
    return 0;
}
int eeprom_i2c_module_init()
{
    return 0;
}
#endif