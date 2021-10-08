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

//Delay Settings
#define SAMPLE_DELAY            1000 //Continous sample rate in ms (Default = 1000)

//Serial Output Settings    
#define ENABLE_ACC_GRAVITY      1   //Accelermoter data output in unit of gravatis (0: DISABLE, 1: *ENABLE)
#define ENABLE_GYR_DEG_PER_SEC  1   //gyroscope data output in units of degrees/second  (0: DISABLE, 1: *ENABLE)
#define ENABLE_BINARY           1   //Data output in unit of binary sensor output   (0: DISABLE, 1: *ENABLE)

//Accelerometer settings
#define ACCELEROMETER_RANGE     3   //0: 2g,    1: 4g,  2: 8g,  3: *16g
#define ACCELEROMETER_DATARATE  3   //0: 12.5 Hz,   1: 25 Hz,   2: 50 Hz,   3: *100 Hz,  4: 200 Hz,  5: 400 Hz,  6: 800 Hz, 7: 1600 Hz 
#define ACCELEROMETER_BANDWITH  2   //0: AVG 1, 1: AVG 2,   2: *AVG 4,   3: AVG 8,   4: AVG 16,  5: AVG 32,  6: AVG 64,  7: AVG 128
#define ACCELEROMETER_POWER     0   //0: *NORMAL,    1: LOW-POWER, 2: SUSPEND

//gyroscope settings
#define GYROSCOPE_RANGE         4   //0: 125 dps,   1: 250 dps, 2: 500 dps, 3: 1000 dps,    4: *2000 dps
#define GYROSCOPE_DATARATE      2   //0: 25 Hz,   1: 50 Hz,   2: *100 Hz,  3: 200 Hz,  4: 400 Hz,  5: 800 Hz,  6: 1600 Hz,   7: 3200 Hz
#define GYROSCOPE_BANDWITH      0   //0: *NORMAL,    1: OVERSAMPLE 2x,   2: OVERSAMPLE 4x
#define GYROSCOPE_POWER         0   //0: *NORMAL,    1: FAST-START,   2: SUSPEND

//Accelerometer Offset Compensation (Default assumes MAX78000 EVKit is sitting on a flat surface display facing upwards)
#define ACCELEROMETER_COMP_X    3   //0: DISABLED,  1: Positive 1g, 2: Negative 1g, 3: *Zero Force
#define ACCELEROMETER_COMP_Y    3   //0: DISABLED,  1: Positive 1g, 2: Negative 1g, 3: *Zero Force
#define ACCELEROMETER_COMP_Z    2   //0: DISABLED,  1: Positive 1g, 2: *Negative 1g, 3: Zero Force

//I2C Paramaters (Default values should not be modified)
#define I2C_MASTER      MXC_I2C2    //MXC_I2C2: *I2C Port 2
#define I2C_FREQ        100000      //Frequency set in Hz (*100000: 100 kHz,    400000: 400 kHz (MAX VALUE))
#define I2C_CONFIG      1           //0: I2C Slave Configuration,   1: *I2C Master Configuration


/***** Global Variables *****/

struct bmi160_dev bmi160;       // Global bmi160 struct


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
* @param[in]    device      Pointer to initialized, bmi160 structure
* @return       BMI160_OK (Success), Error Code (Failure)
*/
int bmi160_config_sensor(struct bmi160_dev *device);

/**
* @brief        Initialize and enable the gyroscope and accelerometer fast offset
*               compensation feature
*
* @param[in]    device      Pointer to initialized, bmi160 structure
* @return       BMI160_OK (Success), Error Code (Failure)
*/
int bmi160_FOC_init(struct bmi160_dev *device);

#endif  /* _MXC_I2C_H_ */