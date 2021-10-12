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
* @file             bmi160_max78000evkit.h
* @brief            This is the heade file for the BMI160 drivers. Configuration
*                   options for the BMI160 sensors and serial output can be
*                   modified here. Please refer to definition comments for proper
*                   use of the sensor.
* @version          1.0.0
* @notes            
*****************************************************************************/

/* Define to prevent redundant inclusion */
#ifndef _BMI160_MAX78000EVKIT_H_    
#define _BMI160_MAX78000EVKIT_H_

#include "bmi160.h"
#include "mxc_errors.h"
/***** Definitions *****/

//Setting Comments with an asterisks(*) refer to default conditions

//Accelerometer settings
#define ACCELEROMETER_RANGE     BMI160_ACCEL_RANGE_16G          //Default: 16g
#define ACCELEROMETER_DATARATE  BMI160_ACCEL_ODR_100HZ          //Default: 100 Hz
#define ACCELEROMETER_BANDWITH  BMI160_ACCEL_BW_NORMAL_AVG4     //Default: AVG 4
#define ACCELEROMETER_POWER     BMI160_ACCEL_NORMAL_MODE        //Default: NORMAL

//gyroscope settings
#define GYROSCOPE_RANGE         BMI160_GYRO_RANGE_2000_DPS      //Default: 2000 dps
#define GYROSCOPE_DATARATE      BMI160_GYRO_ODR_100HZ           //Default: 100 Hz
#define GYROSCOPE_BANDWITH      BMI160_GYRO_BW_NORMAL_MODE      //Default: NORMAL
#define GYROSCOPE_POWER         BMI160_GYRO_NORMAL_MODE         //Default: NORMAL

//Accelerometer Offset Compensation (Default assumes MAX78000 EVKit is sitting on a flat surface display facing upwards)
#define ACCELEROMETER_COMP_X    BMI160_FOC_ACCEL_0G             //Default: 0g
#define ACCELEROMETER_COMP_Y    BMI160_FOC_ACCEL_0G             //Default: 0g
#define ACCELEROMETER_COMP_Z    BMI160_FOC_ACCEL_NEGATIVE_G     //Default: Negative 1g

//I2C Paramaters (Default values should not be modified)
#define I2C_MASTER      MXC_I2C2    //MXC_I2C2: *I2C Port 2
#define I2C_FREQ        100000      //Frequency set in Hz (*100000: 100 kHz,    400000: 400 kHz (MAX VALUE))
#define I2C_CONFIG      1           //0: I2C Slave Configuration,   1: *I2C Master Configuration


/***** Global Variables *****/

struct bmi160_dev bmi160;       // Global bmi160 struct.  Call init_bmi160() and bmi160_config_sensor() to initialize and configure the sensor.  Then, this struct can be used as an argument in the driver functions from bmi160.h


/***** Function Prototypes *****/

/**
* @brief        Initializes the BMI160 IMU to factory default settings. Users should call
*               this function instead of the built-in "bmi160_init" function from bmi160.h
*
* @return       BMI160_OK (Success), Error Code (Failure)
*/
int init_bmi160();

/**
* @brief        Configure BMI160 accelerometer and gyroscope based on user definitions in 
*               the bmi160_max78000.h header file
*
* @return       BMI160_OK (Success), Error Code (Failure)
*/
int bmi160_config_sensor();

/**
* @brief        Initialize and enable the gyroscope and accelerometer fast offset
*               compensation feature
*
* @return       BMI160_OK (Success), Error Code (Failure)
*/
int bmi160_FOC_init();

#endif  /* _MXC_I2C_H_ */