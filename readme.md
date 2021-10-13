# Introduction
This document disscusses using the MAX78000 EVKit drivers for the [BMI160 Inertial Movement Unit (IMU)](https://community.bosch-sensortec.com/t5/Knowledge-base/BMI160-Series-IMU-Design-Guide/ta-p/7376).

This project includes two main components:
* The BMI160 driver implementation for the MAX78000EVKIT.  This can be found under the "driver" folder of the release package.
* A demo/example program showing how to use the drivers and excercise the BMI160.  This can be found under the "example" folder of the release package.

# Requirements 
- MAX78000 EVKit Connected to PC via micro-USB (power) and the MAX32625 PICO Adapter (programmer and debugger)
- [Maxim Micros SDK](https://www.maximintegrated.com/content/maximintegrated/en/design/software-description.html/swpart=SFW0010820A)
- [VS-Code with Maxim Support](https://github.com/MaximIntegratedTechSupport/VSCode-Maxim)

# Running the Example
## Hardware Setup
Ensure that you MAX78000 EVKit is properly connected to your PC. There should be a micro-USB cable connected directly to the MAX78000 EVKit that is used for powering the device. A blue LED power LED should turn on to indicate that the board is being powered.

A second connection must be made to the MAX78000 EVKit on the SWD port. The SWD port will be connected to the MAX32625 PICO programmer board. This connection will allow for programming the MAX78000 chip and relaying data back to the PC from the communication port. Please visit the [MAX78000 EVKit Product Page](https://www.maximintegrated.com/en/products/microcontrollers/MAX78000EVKIT.html) for additional setup information. 

## Download Firmware
Download the latest [Release](https://github.com/MaximIntegratedTechSupport/BMI160-MAX78000) and extract it to an accessible location.  **Note: There must not be any spaces in file path of the extraction location**

## Connect a Serial Terminal
In order to validate that the BMI160 is functioning as expected, configuration results and data measurements are sent to the PC serial port. These readings can be helpful for viewing real-time data, checking self-test results, and ensuring that the sensor was configured correctly. In order to view the sensor output:

1. Open your serial terminal viewer of choice
2. Configure the serial terminal to connect with the MAX32625 PICO at a baud rate of 1125000 as shown below.

![Serial Monitor Setup](https://github.com/MaximIntegratedTechSupport/BMI160-MAX78000/blob/master/img/SerialSetup.jpg)

3. Verify that the terminal is connected

## Uploading the Sample Code
Now that the hardware is ready, the project has been downloaded, and the serial monitor is connected, it is time to run the program. In order to run the program:

1. Open up the downloaded project in your VS-Code environment with `File -> Open Folder` on the "example" folder.
2. Open the main.c file from the file explorer window
3. Press "F5" to start debugging (NOTE: The VS-Code "Start Debugging" command will automatically build the project and flash the program to the MAX78000 EVKit)
4. Verify that all sensor initialization and configuration tests and procedures are successful when the firmware program begins. This information will be displayed on the serial terminal viewer as shown below:

![Correct Initialization](https://github.com/MaximIntegratedTechSupport/BMI160-MAX78000/blob/master/img/Correct_Initialization.JPG)

5. View data being printed to the serial monitor *(NOTE: Picking up the MAX78000 EVKit and moving it to new orientations should be reflected on the serial output in real-time)*

![Data Out](https://github.com/MaximIntegratedTechSupport/BMI160-MAX78000/blob/master/img/Reading_Data.jpg)

## Configuring the Sensors
The sample program in main.c pulls configuration information from the bmi160_max78000evkit.h header file. The configuration definitions are shown below.

![Definitions](https://github.com/MaximIntegratedTechSupport/BMI160-MAX78000/blob/master/img/Config_Definitions.JPG)

In order to edit these settings, you can change definition values based on the bmi160_defs.h file. In order to view possible configuration options, us **ctrl+click** on the option you would like to configure. For example, if you **ctrl+click** the *BMI160_ACCEL_RANGE_16G*, the declaration of the possible accelerometer ranges should appear, as shown below.

![Possible Definitions](https://github.com/MaximIntegratedTechSupport/BMI160-MAX78000/blob/master/img/Config_List.jpg)

By simply changing the *ACCELEROMETER_RANGE* value from *BMI160_ACCEL_RANGE_16G* to a new setting found in the bmi160_defs.h file, such as *BMI160_ACCEL_RANGE_4G*, the program will make the necessary change the next time you re-build and upload the firmware. This allows for quick testing of different accelerometer and gyroscope configurations.

*NOTE: Any of the predefined configurations are compatible with the sensor configuration function. Please refer to the [BMI160 Design Guide](https://community.bosch-sensortec.com/t5/Knowledge-base/BMI160-Series-IMU-Design-Guide/ta-p/7376) for configuration specific information.*

# Using the drivers
## Design
The driver implementation provides a global `bmi160_dev` struct called `bmi160`.  This is intended to be used with the baseline `bmi160.h` driver functions after the following conditions have been met:
* The initialization function `init_bmi160` has been called.  This sets up I2C communication, links callback functions, and initializes the global `bmi160` struct.
* The configuration function `bmi160_config_sensor` has been called.  This configures the BMI160 with the options set in the `bmi160_max78000evkit.h` header file.

With the above conditions met, the adress of the global `bmi160` struct can be passed as an argument into subsequent driver functions from `bmi160.h`.  See the example project for a usage demonstration.

## Adding the Drivers to a Project
The drivers are distributed in source code format under the "driver" folder of the release package.

To use the drivers in your own project:
* Copy the "driver" folder to a location accessible by your project
* Add the `bmi160.c` and `bmi160_max78000evkit.c` source files to the build
* Add the "driver" folder to the build's search path for both include files and source files.
* Add the "driver" folder to your IDE's include and browse paths for source code lookups.  If you're using [VSCode-Maxim](https://github.com/MaximIntegratedTechSupport/VSCode-Maxim), see the VSCode-Maxim [readme](https://github.com/MaximIntegratedTechSupport/VSCode-Maxim#setting-search-paths-for-intellisense) for more details on this.

See the Makefile of the example project for an example:
![Makefile](https://github.com/MaximIntegratedTechSupport/BMI160-MAX78000/blob/master/img/Makefile.JPG)
