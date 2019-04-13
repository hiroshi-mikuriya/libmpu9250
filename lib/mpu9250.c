#include "mpu9250.h"
#include "mpu9250reg.h"
#include "i2c.h"
#include "spi.h"
#include <unistd.h> // usleep

#define AK8963_ADDR 0x0C

typedef enum
{
    NONE = 0,
    I2C_MODE = 1,
    SPI_MODE = 2,
}MODE;

static MODE s_mode;
static int s_cs;
static uint8_t s_dev_addr;

static int i2c_write_with_init(uint8_t dev_addr, uint8_t reg, uint8_t d)
{
    int err = 0;
    err = i2c_init(dev_addr);
    if (err)
        return err;
    uint8_t v[] = { reg, d };
    err = i2c_write(v, sizeof(v));
    if (err)
        return err;
    return 0;
}

static int i2c_read_with_init(uint8_t dev_addr, uint8_t reg, uint8_t * buf, int size)
{
    int err = 0;
    err = i2c_init(dev_addr);
    if (err)
        return err;
    err = i2c_write_read(&reg, sizeof(reg), buf, size);
    if (err)
        return err;
    return 0;
}

int mpu9250_i2c_init(int dev_addr)
{
    int err = 0;
    s_mode = NONE;
    i2c_deinit();
    spi_deinit();
    err = i2c_write_with_init(dev_addr, REG_INT_PIN_CFG, 0x02); // 磁気センサ有効
    if (err)
        return err;
    err = i2c_write_with_init(AK8963_ADDR, REG_MAG_CNTL, 0x16); // 磁気センサの出力周期(100Hz)
    if (err)
        return err;
    s_dev_addr = dev_addr;
    s_mode = I2C_MODE;
    return 0;
}

int mpu9250_spi_init(int cs)
{
    int err = 0;
    s_mode = NONE;
    i2c_deinit();
    spi_deinit();
    err = spi_init();
    if (err)
        return err;
    s_cs = cs;
    s_mode = SPI_MODE;
    return 0;
}

static int read_register(short * dst, uint8_t reg)
{
    int err = 0;
    uint8_t buf[6] = { 0 };
    switch(s_mode){
    default:
        return -1;
    case I2C_MODE:
        err = i2c_read_with_init(s_dev_addr, reg, buf, sizeof(buf));
        break;
    case SPI_MODE:
        reg |= 0x80;
        err = spi_write_read(&reg, sizeof(reg), buf, sizeof(buf), s_cs);
        break;
    }
    if (err)
        return err;
    for (int i = 0; i < 3; ++i) {
        dst[i] = (short)((buf[i * 2] << 8) + (buf[i * 2 + 1] & 0xFF));
    }
    return 0;
}

int mpu9250_read_accel(short * accel)
{
    return read_register(accel, REG_ACCEL_XOUT_H);
}

int mpu9250_read_gyro(short * gyro)
{
    return read_register(gyro, REG_GYRO_XOUT_H);
}

int mpu9250_read_mag(short * mag)
{
    if (s_mode != I2C_MODE)
        return -1;
    int err = 0;
    for (uint8_t b = 0;;) {
        err = i2c_read_with_init(AK8963_ADDR, REG_MAG_ST1, &b, sizeof(b));
        if (err)
            return err;
        if (b & 0x01)
            break;
        usleep(1000);
    }
    uint8_t buf[7] = { 0 };
    err = i2c_read_with_init(AK8963_ADDR, REG_MAG_HXL, buf, sizeof(buf));
    if (err)
        return err;
    for (int i = 0; i < 3; ++i) {
        mag[i] = (short)((buf[i * 2] & 0xFF) + (buf[i * 2 + 1] << 8)); // TODO: use Adj
    }
    return 0;
}
