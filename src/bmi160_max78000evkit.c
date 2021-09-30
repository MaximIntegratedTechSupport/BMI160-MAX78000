#include <stdio.h>
#include "bmi160_max78000evkit.h"
#include "mxc_delay.h"
#include "i2c.h"
#include "i2c_regs.h"

////////////////////////////
// Local defines
////////////////////////////
#define I2C_MASTER      MXC_I2C2        
#define I2C_FREQ        500000          //Frequency set in Hz, Max = 1MHz
#define I2C_CONFIG      1               //Set to 1 for Master, 0 for Slave

////////////////////////////
// Globals
////////////////////////////
mxc_i2c_req_t reqMaster = {
    .addr = 0x00,
    .callback = NULL,
    .i2c = I2C_MASTER,
    .restart = 0,
    .rx_buf = NULL,
    .rx_len = 0,
    .tx_buf = NULL,
    .tx_len = 0
};
uint8_t error;

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
    bmi160.id = BMI160_FOC_CONF_ADDR;
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
    //Initilize the I2C Port as a master
    if(MXC_I2C_Init(I2C_MASTER, I2C_CONFIG, 0) != E_NO_ERROR){
        printf("Error initializing the I2C Port 2\n");
        return E_BAD_PARAM;
    }
    
    //Set the frequency of communication over I2C port
    if(MXC_I2C_SetFrequency(I2C_MASTER, I2C_FREQ) != E_NO_ERROR){
        printf("Error setting I2C speed\n");
        return E_BAD_PARAM;
    }


    return E_SUCCESS;
}

static int8_t i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    
    //Set paramaters for I2C read
    reqMaster.addr = dev_addr;          //Set device address 
    reqMaster.tx_buf = &reg_addr;       //Pointer to register address
    reqMaster.tx_len = 1;               //Number of bytes to send
    reqMaster.rx_buf = data;            //Pointer to structure to store read values
    reqMaster.rx_len = len;             //Number of bytes to read

    if ((error = MXC_I2C_MasterTransaction(&reqMaster)) != E_NO_ERROR) {
        
        //Communication error
        if(error != 1){
            printf("Error (%d) reading data: Device = 0x%X; Register = 0x%X\n", error, dev_addr, reg_addr);
            return E_COMM_ERR;
        }
        //Message not acknowledged
        else{
            printf("Read was not acknowledged: Device = 0x%X; Register = 0x%X\n", dev_addr, reg_addr);
            return E_COMM_ERR;
        }
    }

    return BMI160_OK;
}

static int8_t i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    
    //Allocate memory for register address and data
    uint8_t *TXData = (uint8_t *)malloc(sizeof(data)+sizeof(reg_addr));
    memcpy(TXData, &reg_addr, 1);
    memcpy(TXData+1, data, len);

    //Set paramaters for I2C read
    reqMaster.addr = dev_addr;          //Set device address 
    reqMaster.tx_buf = TXData;          //Pointer to register address
    reqMaster.tx_len = len+1;           //Number of bytes to send
    reqMaster.rx_buf = NULL;            //Pointer to structure to store read values
    reqMaster.rx_len = 0;               //Number of bytes to read

    //Send I2C data
    if ((error = MXC_I2C_MasterTransaction(&reqMaster)) != E_NO_ERROR) {
        //Communication error
        if(error != 1){
            printf("Error (%d) writing data: Device = 0x%X; Register = 0x%X\n", error, dev_addr, reg_addr);
            return E_COMM_ERR;
        }
        //Message not acknowledged
        else{
            printf("Write was not acknowledged: Device = 0x%X; Register = 0x%X\n", dev_addr, reg_addr);
            return E_COMM_ERR;
        }
    }
    return BMI160_OK;
}