#include <stdio.h>
#include "bmi160_max78000evkit.h"

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
    while(1){
    }
    return 1;
}