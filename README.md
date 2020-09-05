# temptaker
Arduino script for a temperature taking system based off an Arduino Nano, a HRSC-04 ultrasonic sensor and a MLX90614-DCI IR Temperature Sensor

# Components
-1x Arduino Nano
-2x SSD1306 OLED Display
-1x HRSC-04 Ultrasonic Sensor
-1x MLX90614-DCI IR Temperature Sensor

Note: The MLX90614 sensor comes in several variants, and can be found in the market as AAA, BCC or DCI types. A quick breakdown of the key differences:
AAA : 40 deg FOV
BCC : 35 deg FOV
DCI : 5 deg FOV

A DCI type is recommended for a non-contact thermometer, where the user is generally positioned 10-20cm away from the sensor. With a 35-40 deg FOV, the effective temperature taking area is 25cm^2 at 15cm distance, hence a significant proportion of the IR signals captured at will come from the surroundings instead of the user's forehead. With a user placed 15cm away from the sensor, the effective temperature taking area is a circular spot of 2.6cm^2 which will be able to fit within the average forehead size.

# Setup
