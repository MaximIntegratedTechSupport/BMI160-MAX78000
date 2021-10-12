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
* @file             main.c
* @brief            This is the main test program for the MAX78000 EVkit BMI160 IMU drivers.
*                   This main function will perform five (5) basic driver functions:
*                       1) Initilize the BMI160 IMU
*                       2) Perform BMI160 self-test for Accelerometer and verify result
*                       3) Perform BMI160 self-test for Gyroscope and verify results
*                       4) Configure the Accelerometer and Gyroscope based on user definitions
*                           in the bmi160_max78000evkit.h header file
*                       5) Continuosly read and display the BMI160 sesor data in format(s)
*                           specefied by user in the bmi160_max78000evkit.h header file
*                   All sensor readings will be sent through the Comm port of the MAX78000
*                   EVKit (default baud = 115200). It can be viewed with any serial monitor
*                   program when device is connected to a PC.
* @version          1.0.0
* @notes            
*****************************************************************************/

#include <stdio.h>
#include "bmi160_max78000evkit.h"

/***** Serial Output Options *****/

//Sample Rate
#define SAMPLE_DELAY            1000 //Continous sample rate in ms (Default = 1000)

//Serial Output Formats    
#define ENABLE_ACC_GRAVITY      1   //Accelermoter data output in unit of gravatis (0: DISABLE, 1: ENABLE)
#define ENABLE_GYR_DEG_PER_SEC  1   //Gyroscope data output in unit of degrees/second  (0: DISABLE, 1: ENABLE)
#define ENABLE_BINARY           1   //Data output in unit of binary sensor output   (0: DISABLE, 1: ENABLE)


/***** Global Variables *****/

//Structurs for holding Data being read
struct bmi160_sensor_data bmi160_accel;
struct bmi160_sensor_data bmi160_gyro;

// This test program shall initialize the BMI160, validate the built-in self-test, and test additional bmi160 driver functions.
int main() {

    volatile int8_t result;

    printf("Beginning Sensor Intitialization and Self Testing\n");
    printf("-------------------------------------------------\n");
    
    //Initialize Sensor
    printf("Result for Initialization: ");
    if ( (result = init_bmi160()) != BMI160_OK) {
        printf("FAILURE\n");
        while(1);
    }
    else printf("SUCCESS\n");

    //Self-Test for Accelerometer
    printf("Result for Self Test Accelerometer: ");
    if ( (result = bmi160_perform_self_test(BMI160_ACCEL_ONLY, &bmi160)) != BMI160_OK) {
        printf("FAILURE\n");
        while(1);
    }
    else printf("SUCCESS\n");
   
    //Self-Test for gryrescope
    printf("Result for Self Test Gyroscope: ");
    if ( (result = bmi160_perform_self_test(BMI160_GYRO_ONLY, &bmi160)) != BMI160_OK) {
        printf("FAILURE\n");
        while(1);
    }
    else printf("SUCCESS\n");

    //Set default config for sensors (Settings are specefied in bmi160_max78000.h file)
    printf("Result for Sensor Configuration: ");
    if ((result = bmi160_config_sensor()) != BMI160_OK) {    
        printf("FAILURE\n");
        while(1);
    }
    else printf("SUCCESS\n\n\n");
    
    //Select Conversion Factor for Accelerometer Gravity Output
#ifdef ENABLE_ACC_GRAVITY
    float acc_x;
    float acc_y;
    float acc_z;
    float ACC_Conversion_Factor;
    switch(ACCELEROMETER_RANGE){
        case BMI160_ACCEL_RANGE_2G:
            ACC_Conversion_Factor = 16384.0;
            break;
        case BMI160_ACCEL_RANGE_4G:
            ACC_Conversion_Factor = 8192.0;
            break;
        case BMI160_ACCEL_RANGE_8G:
            ACC_Conversion_Factor = 4096.0;
            break;
        case BMI160_ACCEL_RANGE_16G:
            ACC_Conversion_Factor = 2048.0;
            break;
        default:
            printf("Undefined Accelerometer Range Setting\n");
            ACC_Conversion_Factor = 1;
            break;
    }
#endif

    //Select Conversion Factor for Gyroscope Degrees/Second Output
#ifdef ENABLE_GYR_DEG_PER_SEC
    float gyr_x;
    float gyr_y;
    float gyr_z;
    float GYR_Conversion_Factor;
    switch(GYROSCOPE_RANGE){
        case BMI160_GYRO_RANGE_125_DPS:
            GYR_Conversion_Factor = 262.4;
            break;
        case BMI160_GYRO_RANGE_250_DPS:
            GYR_Conversion_Factor = 131.2;
            break;
        case BMI160_GYRO_RANGE_500_DPS:
            GYR_Conversion_Factor = 65.6;
            break;
        case BMI160_GYRO_RANGE_1000_DPS:
            GYR_Conversion_Factor = 32.8;
            break;
        case BMI160_GYRO_RANGE_2000_DPS:
            GYR_Conversion_Factor = 16.4;
            break;
        default:
            printf("Undefined Gyroscope Range Setting\n");
            GYR_Conversion_Factor = 1;
            break;
    }
#endif

    //Configure Offset
    if ((result = bmi160_FOC_init()) != BMI160_OK) {
        printf("Offset Calibration Status: FAILURE\n");
        while(1);
    } 
    else{
        bmi160.delay_ms(3000);
        
        //Remove any data that was recorded before offset compensation was configured
        bmi160_get_sensor_data((BMI160_ACCEL_SEL | BMI160_GYRO_SEL), &bmi160_accel, &bmi160_gyro, &bmi160);
        
        printf("Starting data measurments\n\n\n");
        bmi160.delay_ms(2000);
    }

    // Main loop
    while(1){
        
        //Read data from the sensor
        bmi160_get_sensor_data((BMI160_ACCEL_SEL | BMI160_GYRO_SEL), &bmi160_accel, &bmi160_gyro, &bmi160);
        
        //Dispaly the Accelerometer and gyro readings
        printf("Observed Accelerometer and Gyroscope Data\n");
        printf("------------------------------------------\n\n");
        
#ifdef ENABLE_ACC_GRAVITY
        //Caluculate force in terms of g's
        acc_x = bmi160_accel.x/ACC_Conversion_Factor;
        acc_y = bmi160_accel.y/ACC_Conversion_Factor;
        acc_z = bmi160_accel.z/ACC_Conversion_Factor;
        
        //print results
        printf("Gravities:\n");
        printf("ax:%.2f g\tay:%.2f g\taz:%.2f g\n\n", acc_x, acc_y, acc_z);
#endif

#ifdef ENABLE_GYR_DEG_PER_SEC
        //Caluculate force in terms of g's
        gyr_x = bmi160_gyro.x/GYR_Conversion_Factor;
        gyr_y = bmi160_gyro.y/GYR_Conversion_Factor;
        gyr_z = bmi160_gyro.z/GYR_Conversion_Factor;
        
        //print results
        printf("Degrees per second:\n");
        printf("ax:%.1f deg/s\tay:%.1f deg/s\taz:%.1f deg/s\n\n", gyr_x, gyr_y, gyr_z);
#endif

#ifdef ENABLE_BINARY
        //Print Sensor Data
        printf("BINARY:\n");
        printf("ax:%d\tay:%d\taz:%d\n", bmi160_accel.x, bmi160_accel.y, bmi160_accel.z);
        printf("gx:%d\tgy:%d\tgz:%d\n\n", bmi160_gyro.x, bmi160_gyro.y, bmi160_gyro.z);
#endif
        
        //Delay between samples
        bmi160.delay_ms(SAMPLE_DELAY);   
    }
}