#include "bmi160.h"
#include "mxc_errors.h"

// Global bmi160 struct.  Can be used for other driver calls after init_bmi160() is called.
struct bmi160_dev bmi160;

// BMI160 initialization function for the MAX78000EVKIT
int init_bmi160();