#include <stdio.h>
#include "bmi160_max78000evkit.h"


//Function Prototype
void ConfigureSensor_bmi160(void);

//Structurs for holding Data being read
struct bmi160_sensor_data bmi160_accel;
struct bmi160_sensor_data bmi160_gyro;
struct bmi160_foc_conf bmi160_foc_conf;
struct bmi160_offsets bmi160_offsets;

#define ENABLE_ACC_GRAVITY      1   //enable accelermoter data output in unit of gravatis
#define ENABLE_GYR_DEG_PER_SEC  1   //enable gyrescope data output in units of degrees/second
#define ENABLE_BINARY           1   //enable data output in unit of binary sensor output 


#ifdef ENABLE_ACC_GRAVITY
    float acc_x;
    float acc_y;
    float acc_z;
    float ACC_Conversion_Factor;
#endif

#ifdef ENABLE_GYR_DEG_PER_SEC
    float gyr_x;
    float gyr_y;
    float gyr_z;
    float GYR_Conversion_Factor;
#endif

// This test program shall initialize the BMI160, validate the built-in self-test, and test additional bmi160 driver functions.
int main() {

    volatile int8_t result;

    printf("Beginning Sensor Intitialization and Self Testing\n");
    printf("-------------------------------------------------\n");
    
    
    //Initialize Sensor
    printf("Result for Initialization: ");
    if ( (result = init_bmi160()) != E_SUCCESS) {
        printf("FAILURE\n");
        exit(1);
    }
    else printf("SUCCESS\n");
    
    //Self-Test for accelerometer
    printf("Result for Self Test Accelerometer: ");
    if ( (result = bmi160_perform_self_test(BMI160_ACCEL_ONLY, &bmi160)) != BMI160_OK) {
        printf("FAILURE\n");
        exit(1);
    }
    else printf("SUCCESS\n");
   
    //Self-Test for gryrescope
    printf("Result for Self Test Gyrescope: ");
    if ( (result = bmi160_perform_self_test(BMI160_GYRO_ONLY, &bmi160)) != BMI160_OK) {
        printf("FAILURE\n");
        exit(1);
    }
    else printf("SUCCESS\n\n\n");

    //Set default config and offset for sensor
    ConfigureSensor_bmi160();
    
    while(1){
        
        //Read data from the sensor
        bmi160_get_sensor_data((BMI160_ACCEL_SEL | BMI160_GYRO_SEL), &bmi160_accel, &bmi160_gyro, &bmi160);
        
        //Dispaly the accelerometer and gyro readings
        printf("Observed Accelerometer and Gyrescope Data\n");
        printf("------------------------------------------\n\n");
        
        if(ENABLE_ACC_GRAVITY){

            //Caluculate force in terms of g's
            acc_x = bmi160_accel.x/ACC_Conversion_Factor;
            acc_y = bmi160_accel.y/ACC_Conversion_Factor;
            acc_z = bmi160_accel.z/ACC_Conversion_Factor;
            
            printf("Gravities:\n");
            //print results
            printf("ax:%.2f g\tay:%.2f g\taz:%.2f g\n\n", acc_x, acc_y, acc_z);
        }

        if(ENABLE_GYR_DEG_PER_SEC){

            //Caluculate force in terms of g's
            gyr_x = bmi160_gyro.x/GYR_Conversion_Factor;
            gyr_y = bmi160_gyro.y/GYR_Conversion_Factor;
            gyr_z = bmi160_gyro.z/GYR_Conversion_Factor;
            
            printf("Degrees per second:\n");
            //print results
            printf("ax:%.1f deg/s\tay:%.1f deg/s\taz:%.1f deg/s\n\n", gyr_x, gyr_y, gyr_z);
        }

        if(ENABLE_BINARY){
            
            printf("BINARY:\n");
            //Print Sensor Data
            printf("ax:%d\tay:%d\taz:%d\n", bmi160_accel.x, bmi160_accel.y, bmi160_accel.z);
            printf("gx:%d\tgy:%d\tgz:%d\n\n", bmi160_gyro.x, bmi160_gyro.y, bmi160_gyro.z);
        }
        
        //Delay 1 second between readings
        bmi160.delay_ms(1000);   

    }
    return 1;
}

void ConfigureSensor_bmi160(void){

    int rslt = E_NO_ERROR;
    uint8_t data[1];
    uint8_t error_flag = 0;

    //Set default sensor configuration
    bmi160.accel_cfg.odr = BMI160_ACCEL_ODR_100HZ;
    bmi160.accel_cfg.range = BMI160_ACCEL_RANGE_16G;
    bmi160.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;

    bmi160.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

    bmi160.gyro_cfg.odr = BMI160_GYRO_ODR_100HZ;
    bmi160.gyro_cfg.range = BMI160_GYRO_RANGE_2000_DPS;
    bmi160.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;

    bmi160.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;


    //Set Conversion Factor for accelerometer gravity calculations
    if(ENABLE_ACC_GRAVITY){
        switch(bmi160.accel_cfg.range){
        
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
                ACC_Conversion_Factor = 2048.0;
                break;
        }
    }

    //Set Conversion Factor for gyrescope deg/s calculations
    if(ENABLE_GYR_DEG_PER_SEC){
        switch(bmi160.accel_cfg.range){
        
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
                GYR_Conversion_Factor = 16.4;
                break;
        }
    }


    // Set the default sensor configuration
    if((rslt = bmi160_set_sens_conf(&bmi160)) != E_NO_ERROR){
        printf("Error configuring Accelerometer and Gyroscope (Error Code: %d)n\n\n",rslt);
        error_flag = 1;
    }

    //Allow registers to update
    bmi160.delay_ms(100);

    //Calculate the Fast Offfset Compensation (FOC) settings
    data[0] = 0x40;     //Enable Gyrescope

    data[0] |= (BMI160_FOC_ACCEL_0G << 4);          //Set accelerometer X startup state
    data[0] |= (BMI160_FOC_ACCEL_0G << 2);          //Set accelerometer Y startup state
    data[0] |= (BMI160_FOC_ACCEL_NEGATIVE_G << 0);  //Set accelerometer Z startup state

    printf("Intializing the Offset Error Compensation\n");
    printf("----------------------------------------\n\n");
    printf("Please ensure that the EVKIT is laying\n");
    printf("on a flat surface for proper calibration.\n\n");
    printf("Ambient noise or vibration can cause\n");
    printf("innaccurate calibration of this device.\n\n\n\n");

    bmi160.delay_ms(5000);

    printf("Calibration begins in:\n\n");

    printf("3...\n");
    bmi160.delay_ms(1000);
    printf("2...\n");
    bmi160.delay_ms(1000);
    printf("1...\n");
    bmi160.delay_ms(1000);

    //Write setup config for FOC
    if((rslt = bmi160_set_regs(0x69, data, 1, &bmi160)) != E_NO_ERROR) {
        printf("Error setting config register for Fast Offset Correction (Error: %d)\n\n",rslt);
        error_flag = 1;
    }

    //Start FOC calibration
    data[0] = BMI160_START_FOC_CMD;
    if((rslt = bmi160_set_regs(0x7E, data, 1, &bmi160)) != E_NO_ERROR){
        printf("Error initialising FOC (Error: %d)\n\n",rslt);
        error_flag = 1;
    }

    //Wait the maximum setup time
    bmi160.delay_ms(250);

    //Read offset register
    if((rslt = bmi160_get_regs(0x77, data, 1, &bmi160)) != E_NO_ERROR){
        printf("Error reading offset register (Error: %d)\n\n",rslt);
        error_flag = 1;
    }

    //Enable offset
    data[0] |= 0xC0;
    if((rslt = bmi160_set_regs(0x77, data, 1, &bmi160)) != E_NO_ERROR){
        printf("Error enabling offset (Error: %d)\n\n",rslt);
        error_flag = 1;
    }

    //print initialization results
    printf("Result for Sensor Configuration and Offset Compensation: ");
    
    if(error_flag){ 
        printf("FAILURE\n");
        printf("Please fix error(s) listed above.\n\n\n");
    }
    
    else{
        printf("SUCCESS\n\n\n");
        bmi160.delay_ms(3000);
        printf("Starting data measurments\n\n\n");
        bmi160.delay_ms(2000);
    }
   
}