#include <stdio.h>
#include "bmi160_max78000evkit.h"


//Function Prototype
void ConfigureSensor_bmi160(void);

//Structurs for holding Data being read
struct bmi160_sensor_data bmi160_accel;
struct bmi160_sensor_data bmi160_gyro;


// This test program shall initialize the BMI160, validate the built-in self-test, and test additional bmi160 driver functions.
int main() {

    volatile int8_t result;

    if ( (result = init_bmi160()) != E_SUCCESS) {
        // TODO:  Handle failure
    
    }
    printf("Result for Initialization: %d\n",result);
    
    if ( (result = bmi160_perform_self_test(BMI160_ACCEL_ONLY, &bmi160)) != BMI160_OK) {
        // TODO:  Handle failure
    }

    printf("Result for Self Test Accelerometer: %d\n",result);

    if ( (result = bmi160_perform_self_test(BMI160_GYRO_ONLY, &bmi160)) != BMI160_OK) {
        // TODO:  Handle failure
    }

    printf("Result for Self Test Gyrescope: %d\n",result);

    // TODO: test additional driver functions

    ConfigureSensor_bmi160();

    while(1){
        //Read data from the sensor
        bmi160_get_sensor_data((BMI160_ACCEL_SEL | BMI160_GYRO_SEL), &bmi160_accel, &bmi160_gyro, &bmi160);
        
        //Dispaly the accelerometer and gyro readings
        printf("Observed Accelerometer and Gyrescope Data\n");
        printf("=========================================\n\n");

        printf("ax:%d\tay:%d\taz:%d\n", bmi160_accel.x, bmi160_accel.y, bmi160_accel.z);
        printf("gx:%d\tgy:%d\tgz:%d\n\n", bmi160_gyro.x, bmi160_gyro.y, bmi160_gyro.z);  

        bmi160.delay_ms(1000);   
    }
    return 1;
}

void ConfigureSensor_bmi160(void){

    int rslt = E_NO_ERROR;

    bmi160.accel_cfg.odr = BMI160_ACCEL_ODR_1600HZ;
    bmi160.accel_cfg.range = BMI160_ACCEL_RANGE_16G;
    bmi160.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;


    bmi160.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

    bmi160.gyro_cfg.odr = BMI160_GYRO_ODR_3200HZ;
    bmi160.gyro_cfg.range = BMI160_GYRO_RANGE_2000_DPS;
    bmi160.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;

    bmi160.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;

    /* Set the sensor configuration */
    if((rslt = bmi160_set_sens_conf(&bmi160)) != E_NO_ERROR){
        printf("Error configuring BMI160 (Error Code: %d)]n",rslt);
    }
}