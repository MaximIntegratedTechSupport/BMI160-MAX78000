# Introduction
This document disscusses using the MAX78000 EVKit drivers for the [BMI160 Inertial Movement Unit (IMU)](https://community.bosch-sensortec.com/t5/Knowledge-base/BMI160-Series-IMU-Design-Guide/ta-p/7376).

# Requirments 
- MAX78000 EVKit Connected to PC via USB and MAX
- [Maxim Micros SDK](https://www.maximintegrated.com/content/maximintegrated/en/design/software-description.html/swpart=SFW0010820A)
- [VS-Code with Maxim Support](https://github.com/MaximIntegratedTechSupport/VSCode-Maxim)

# Initial Setup
## Hardware Setup
Ensure that you MAX78000 EVKit is properly connected to your PC. There should be a micro-USB cable connected to the MAX78000 EVKit that is used for powering the device. A second connection must be made to the MAX78000 EVKit on the SWD port. The SWD port will be connected to the MAX32625 PICO programmer board. This connection will allow for programming the MAX78000 chip and relaying data information as a communication port back to the PC. Please visit the [MAX78000 EVKit Product Page](https://www.maximintegrated.com/en/products/microcontrollers/MAX78000EVKIT.html) for addittional setup information. 

## Import Firmware
Import the most recent version of the MAX78000 EVKit drviers for the BMI160 in the desired file location. Make sure to use a recursive pull request so that the most recent version of the drivers are pulled. An example pull command:

	git clone --recurse-submodules https://github.com/MaximIntegratedTechSupport/BMI160-MAX78000.git

# Evaluating the Sensor's Output
In order to validate the BMI160 functionality, configuration results and data measurments are sent to the PC serial port. These readings can be helpful for viewing real-time data, checking self-test results, and ensuring that the sensor was configured correctly. In order to view the sensor output:

1. Open your serial terminal of choice
2. Configure the serial terminal to connect with the MAX32625 PICO at a baud rate of 1125000 as shown below.
![Serial Monitor Setup](https://github.com/MaximIntegratedTechSupport/BMI160-MAX78000/blob/master/img/SerialSetup.jpg)
3. Verify that the terminal is connected

# Uploading the Sample Code
Now that the hardware is connected and the firmware has been imported, and the serial monitor is connected, it is time to run the program. In order to run the program:

1. Open up the downloaded project in you VS-Code envioroment
2. Open the main.c file from the file explorer window
3. Press "F5" to start debugging (NOTE: The VS-Code "Start Debugging" command will automatically build the project and flash the program to the MAX78000)
4. Verify that all initilization and configuration tests and procedures are successful when firmware program begins on your serial terminal
![Correct Initialization](https://github.com/MaximIntegratedTechSupport/BMI160-MAX78000/blob/master/img/Correct_Initialization.jpg)
5. View data being printed to the serial monitor (NOTE: Picking up the MAX78000 EVKit and moving it to new orientations should be reflected on the serial output)
![Serial Monitor Setup](https://github.com/MaximIntegratedTechSupport/BMI160-MAX78000/blob/master/img/Reading_Data.jpg)

# Configuring the Sensors
The sample program in main.c pulls configuration information from the bmi160_max78000evkit.h header file. The configuration definitions are shown below.
![Serial Monitor Setup](https://github.com/MaximIntegratedTechSupport/BMI160-MAX78000/blob/master/img/Config_Definitions.jpg)

In order to adapt these settings, you can change definition values based on the bmi160_defs.h file. In order to view possible configuration options, us **ctrl+click** on the option you would like to configure. For example, if you **ctrl+click** the *BMI160_ACCEL_RANGE_16G*, the decleration of the sensor configuration defintions should appear, as shown below.
![Serial Monitor Setup](https://github.com/MaximIntegratedTechSupport/BMI160-MAX78000/blob/master/img/Config_List.jpg)

*NOTE: Any of the predefined configurations are compatible with the sensor configuration function. Please refer to the [BMI160 Design Guide](https://community.bosch-sensortec.com/t5/Knowledge-base/BMI160-Series-IMU-Design-Guide/ta-p/7376) for configuration specefic information.*