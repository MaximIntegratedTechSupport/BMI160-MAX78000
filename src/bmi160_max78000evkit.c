/*******************************************************************************
* Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************
*/
/**
* @file             bmi160_max78000evkit.c
* @brief            This file holds all driver function source code. Driver functions
                    and parameters are explicitly listed in the bmi160_max78000evkit.h
                    header file. Use the header file for sensor configuration.
* @version          1.0.0
* @notes            
*****************************************************************************/

#include <stdio.h>
#include "bmi160_max78000evkit.h"
#include "mxc_delay.h"
#include "i2c.h"
#include "i2c_regs.h"

/*****  Global Variables *****/  

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

/***** Function Prototypes *****/

static void delay_ms(uint32_t ms);
static int I2C_init();
static int8_t i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
static int8_t i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);

/***** Driver implementation *****/

int init_bmi160(struct bmi160_dev* out) {

    int rslt = BMI160_OK;

    if((rslt = I2C_init()) != E_NO_ERROR){
        printf("Error Configuring I2C2 (Error: %d)\n",rslt);
        return E_BAD_STATE;
    }

    bmi160.delay_ms = delay_ms;
    bmi160.read = i2c_read;
    bmi160.write = i2c_write;
    bmi160.id = BMI160_FOC_CONF_ADDR;   //Use address 0x69
    bmi160.intf = BMI160_I2C_INTF; // Tell drivers to use I2C interface

    // Now that we've configured the struct properly for the EVKIT, call the driver init function.
    if ((rslt = bmi160_init(&bmi160)) != BMI160_OK) {
        return BMI160_E_DEV_NOT_FOUND;
    }

    //Wait Maximum reset time
    bmi160.delay_ms(80);

    return BMI160_OK;
}

/*****************************************************************************************************************************/
static void delay_ms(uint32_t ms) {
    MXC_Delay(MXC_DELAY_MSEC(ms));
}

/*****************************************************************************************************************************/
static int I2C_init() {

    int rslt= E_NO_ERROR;

    //Initilize the I2C Port as a master
    if((rslt = MXC_I2C_Init(I2C_MASTER, I2C_CONFIG, 0)) != E_NO_ERROR){
        printf("Error initializing the I2C Port 2 (Error: %d)\n",rslt);
        return E_BAD_PARAM;
    }
    
    //Set the frequency of communication over I2C port
    if((rslt = MXC_I2C_SetFrequency(I2C_MASTER, I2C_FREQ)) < E_NO_ERROR){
        printf("Error setting I2C speed (Error: %d)\n",rslt);
        return E_BAD_PARAM;
    }

    return E_SUCCESS;
}

/*****************************************************************************************************************************/
static int8_t i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    
    int rslt = BMI160_OK;

    //Set paramaters for I2C read
    reqMaster.addr = dev_addr;          //Set device address 
    reqMaster.tx_buf = &reg_addr;       //Pointer to register address
    reqMaster.tx_len = 1;               //Number of bytes to send
    reqMaster.rx_buf = data;            //Pointer to structure to store read values
    reqMaster.rx_len = len;             //Number of bytes to read

    if ((rslt = MXC_I2C_MasterTransaction(&reqMaster)) != E_NO_ERROR) {
        
        //Communication error
        if(rslt != 1){
            printf("Error (%d) reading data: Device = 0x%X; Register = 0x%X\n", rslt, dev_addr, reg_addr);
            return BMI160_E_COM_FAIL;
        }
        //Message not acknowledged
        else{
            printf("Read was not acknowledged: Device = 0x%X; Register = 0x%X\n", dev_addr, reg_addr);
            return BMI160_E_COM_FAIL;
        }
    }

    return BMI160_OK;
}

/*****************************************************************************************************************************/
static int8_t i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    
    int rslt = BMI160_OK;

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
    if ((rslt = MXC_I2C_MasterTransaction(&reqMaster)) != E_NO_ERROR) {
        //Communication error
        if(rslt != 1){
            printf("Error (%d) writing data: Device = 0x%X; Register = 0x%X\n", rslt, dev_addr, reg_addr);
            return BMI160_E_COM_FAIL;
        }
        //Message not acknowledged
        else{
            printf("Write was not acknowledged: Device = 0x%X; Register = 0x%X\n", dev_addr, reg_addr);
            return BMI160_E_COM_FAIL;
        }
    }
    
    free(TXData);

    return BMI160_OK;
}

/*****************************************************************************************************************************/
int bmi160_config_sensor(struct bmi160_dev *device){
    
    int rslt = BMI160_OK;

    switch(ACCELEROMETER_RANGE){
        case 0:
            bmi160.accel_cfg.range = BMI160_ACCEL_RANGE_2G;
            break;
        case 1:
            bmi160.accel_cfg.range = BMI160_ACCEL_RANGE_4G;
            break;
        case 2:
            bmi160.accel_cfg.range = BMI160_ACCEL_RANGE_8G;
            break;
        case 3:
            bmi160.accel_cfg.range = BMI160_ACCEL_RANGE_16G;
            break;
        default:
            printf("ERROR: Invalid Accelerometer Range\n");
            return BMI160_E_OUT_OF_RANGE; 
    }
    
    switch(ACCELEROMETER_DATARATE){
        case 0:
            bmi160.accel_cfg.odr = BMI160_ACCEL_ODR_12_5HZ;
            break;
        case 1:
            bmi160.accel_cfg.odr = BMI160_ACCEL_ODR_25HZ;
            break;
        case 2:
            bmi160.accel_cfg.odr = BMI160_ACCEL_ODR_50HZ;
            break;
        case 3:
            bmi160.accel_cfg.odr = BMI160_ACCEL_ODR_100HZ;
            break;
        case 4:
            bmi160.accel_cfg.odr = BMI160_ACCEL_ODR_200HZ;
            break;
        case 5:
            bmi160.accel_cfg.odr = BMI160_ACCEL_ODR_400HZ;
            break;
        case 6:
            bmi160.accel_cfg.odr = BMI160_ACCEL_ODR_800HZ;
            break;
        case 7:
            bmi160.accel_cfg.odr = BMI160_ACCEL_ODR_1600HZ;
            break;
        default:
            printf("ERROR: Invalid Accelerometer Data Rate\n");
            return BMI160_E_OUT_OF_RANGE;
            break;
    }
    
    switch(ACCELEROMETER_BANDWITH){
        case 0:
            bmi160.accel_cfg.bw = BMI160_ACCEL_BW_OSR4_AVG1;
            break;
        case 1:
            bmi160.accel_cfg.bw = BMI160_ACCEL_BW_OSR2_AVG2;
            break;
        case 2:
            bmi160.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;
            break;
        case 3:
            bmi160.accel_cfg.bw = BMI160_ACCEL_BW_RES_AVG8;
            break;
        case 4:
            bmi160.accel_cfg.bw = BMI160_ACCEL_BW_RES_AVG16;
            break;
        case 5:
            bmi160.accel_cfg.bw = BMI160_ACCEL_BW_RES_AVG32;
            break;
        case 6:
            bmi160.accel_cfg.bw = BMI160_ACCEL_BW_RES_AVG64;
            break;
        case 7:
            bmi160.accel_cfg.bw = BMI160_ACCEL_BW_RES_AVG128;
            break;
        default:
            printf("ERROR: Invalid Accelerometer Bandwith\n");
            return BMI160_E_OUT_OF_RANGE;
    }

    switch(ACCELEROMETER_POWER){
        case 0:
            bmi160.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;
            break;
        case 1:
            bmi160.accel_cfg.power = BMI160_ACCEL_LOWPOWER_MODE;
            break;
        case 2:
            bmi160.accel_cfg.power = BMI160_ACCEL_SUSPEND_MODE;
            break;
        default:
            printf("ERROR: Invalid Accelerometer Power Mode\n");
            return BMI160_E_OUT_OF_RANGE;
    }

    switch(GYRESCOPE_RANGE){
        case 0:
            bmi160.gyro_cfg.range = BMI160_GYRO_RANGE_125_DPS;
            break;
        case 1:
            bmi160.gyro_cfg.range = BMI160_GYRO_RANGE_250_DPS;
            break;
        case 2:
            bmi160.gyro_cfg.range = BMI160_GYRO_RANGE_500_DPS;
            break;
        case 3:
            bmi160.gyro_cfg.range = BMI160_GYRO_RANGE_1000_DPS;
            break;
        case 4:
            bmi160.gyro_cfg.range = BMI160_GYRO_RANGE_2000_DPS;
            break;
        default:
            printf("ERROR: Invalid Gyrescope Range\n");
            return BMI160_E_OUT_OF_RANGE; 
    }
    
    switch(GYRESCOPE_DATARATE){
        case 0:
            bmi160.gyro_cfg.odr = BMI160_GYRO_ODR_25HZ;
            break;
        case 1:
            bmi160.gyro_cfg.odr = BMI160_GYRO_ODR_50HZ;
            break;
        case 2:
            bmi160.gyro_cfg.odr = BMI160_GYRO_ODR_100HZ;
            break;
        case 3:
            bmi160.gyro_cfg.odr = BMI160_GYRO_ODR_200HZ;
            break;
        case 4:
            bmi160.gyro_cfg.odr = BMI160_GYRO_ODR_400HZ;
            break;
        case 5:
            bmi160.gyro_cfg.odr = BMI160_GYRO_ODR_800HZ;
            break;
        case 6:
            bmi160.gyro_cfg.odr = BMI160_GYRO_ODR_1600HZ;
            break;
        case 7:
            bmi160.gyro_cfg.odr = BMI160_GYRO_ODR_3200HZ;
            break;
        default:
            printf("ERROR: Invalid Gyrescope Data Rate\n");
            return BMI160_E_OUT_OF_RANGE;
            break;
    }
    
    switch(GYRESCOPE_BANDWITH){
        case 0:
            bmi160.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;
            break;
        case 1:
            bmi160.gyro_cfg.bw = BMI160_GYRO_BW_OSR2_MODE;
            break;
        case 2:
            bmi160.gyro_cfg.bw = BMI160_GYRO_BW_OSR4_MODE;
            break;
        default:
            printf("ERROR: Invalid Gyrescope Sample rate\n");
            return BMI160_E_OUT_OF_RANGE;
    }

    switch(GYRESCOPE_POWER){
        case 0:
            bmi160.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;
            break;
        case 1:
            bmi160.gyro_cfg.power = BMI160_GYRO_FASTSTARTUP_MODE;
            break;
        case 2:
            bmi160.gyro_cfg.power = BMI160_GYRO_SUSPEND_MODE;
            break;
        default:
            printf("ERROR: Invalid Gyrescope Power Mode\n");
            return BMI160_E_OUT_OF_RANGE;
    }

    // Set the default sensor configuration
    if((rslt = bmi160_set_sens_conf(&bmi160)) != BMI160_OK){
        printf("Error configuring Accelerometer and Gyroscope (Error Code: %d)\n\n\n",rslt);
        return (rslt);
    }
    
    //Allow registers time to update
    bmi160.delay_ms(100);
    
    return (BMI160_OK);
}

/*****************************************************************************************************************************/
int bmi160_FOC_init(struct bmi160_dev *device){
    
    int rslt = BMI160_OK;
    uint8_t data[1];

    //Calculate the Fast Offfset Compensation (FOC) settings
    data[0] = 0x40;     //Enable Gyrescope

    data[0] |= (ACCELEROMETER_COMP_X << 4);     //Set accelerometer X startup state
    data[0] |= (ACCELEROMETER_COMP_Y << 2);     //Set accelerometer Y startup state
    data[0] |= (ACCELEROMETER_COMP_Z << 0);     //Set accelerometer Z startup state

    printf("Intializing Offset Error Compensation\n");
    printf("----------------------------------------\n");
    printf("Please ensure that the EVKIT is laying\n");
    printf("on a flat surface for proper calibration.\n\n");
    printf("Ambient noise or vibration can cause\n");
    printf("innaccurate calibration of this device.\n\n\n");

    bmi160.delay_ms(5000);

    printf("Calibration begins in:\n\n");

    printf("3...\n");
    bmi160.delay_ms(1000);
    printf("2...\n");
    bmi160.delay_ms(1000);
    printf("1...\n");
    bmi160.delay_ms(1000);
    printf("Calculating Offset...\n\n");

    //Write setup config for FOC
    if((rslt = bmi160_set_regs(0x69, data, 1, device)) != BMI160_OK) {
        printf("Error setting config register for Fast Offset Correction (Error: %d)\n\n",rslt);
        return (rslt);
    }

    //Start FOC calibration
    data[0] = BMI160_START_FOC_CMD;
    if((rslt = bmi160_set_regs(0x7E, data, 1, device) != BMI160_OK)){
        printf("Error initialising FOC (Error: %d)\n\n",rslt);
        return (rslt);
    }

    //Wait the maximum setup time
    bmi160.delay_ms(250);

    //Enable offset
    data[0] |= 0xC0;
    if((rslt = bmi160_set_regs(0x77, data, 1, device)) != BMI160_OK){
        printf("Error enabling offset (Error: %d)\n\n",rslt);
        return (rslt);
    }

    printf("Offset Calibration Status: SUCCESS\n\n");
    
    return BMI160_OK;
}