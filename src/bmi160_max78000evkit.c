#include "bmi160_max78000evkit.h"
#include "mxc_delay.h"

////////////////////////////
// Local functions
////////////////////////////
static void delay_ms(uint32_t ms);
static int I2C_init();
static int8_t i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
static int8_t i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);

////////////////////////////
// Driver implementation
////////////////////////////

// BMI160 initialization function.  Users should call this function instead of the built-in "bmi160_init" function from bmi160.h
int init_bmi160(struct bmi160_dev* out) {
    I2C_init();

    // TODO: Configure bmi160 struct properly.  See https://github.com/BoschSensortec/BMI160_driver/blob/master/examples/read_chip_id/read_chip_id.c and https://github.com/MaximIntegratedAI/refdes/blob/main/maxrefdes178-UNet/maxrefdes178_max32666/src/max32666_accel.c for examples
    bmi160.delay_ms = delay_ms;
    bmi160.read = i2c_read;
    bmi160.write = i2c_write;
    bmi160.id = BMI160_I2C_ADDR;
    bmi160.intf = BMI160_I2C_INTF; // Tell drivers to use I2C interface

    // Now that we've configured the struct properly for the EVKIT, call the driver init function.
    int8_t result;
    if ( (result = bmi160_init(&bmi160)) != BMI160_OK) {
        return E_NO_DEVICE;
    }

    // TODO: (OPTIONAL?) Set up interrupt configuration

    return E_SUCCESS;
}

static void delay_ms(uint32_t ms) {
    MXC_Delay(MXC_DELAY_MSEC(ms));
}

static int I2C_init() {
    // TODO: Set up I2C
    return E_SUCCESS;
}

static int8_t i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    // TODO
    return BMI160_OK;
}

static int8_t i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    // TODO
    return BMI160_OK;
}