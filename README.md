# Using the Swan with Edge Impulse Demo

- [Using the Swan with Edge Impulse Demo](#using-the-swan-with-edge-impulse-demo)
  - [Hardware](#hardware)
  - [Setting up the development environment](#setting-up-the-development-environment)
  - [Usage](#usage)

## Hardware

This project requires just three components:

- A Swan Development Board
- An LSM9DS1 IMU
- An SSD1306 128x64 OLED screen

Simply wire them as follows:

| Swan  | LSM9DS1 | OLED  |
| :---: | :-----: | :---: |
|  SCL  |   SCL   |  SCL  |
|  SDA  |   SDA   |  SDA  |
|  3V3  |   VDD   |  VCC  |
|  GND  |   GND   |  GND  |

## Setting up the development environment

In order to program the Swan, simply follow the directions listed [here](https://blueswireless.notion.site/Swan-Debugging-Guide-5decf8f10a2b44399cf5f8e75f9a2d09). Make sure to select the correct board. You will also need to install the Edge Impulse CLI tools which can be found [here](https://docs.edgeimpulse.com/docs/cli-data-forwarder). If you want, you can also look at the [public version](https://studio.edgeimpulse.com/public/47275/latest) of this project and download the model there to skip the data collection and model training process.

## Usage

Once the Swan is plugged in via USB, hold down `BOOT` button, click the `RESET` button, and then release both. Upload the sketch to the board and then run `edge-impulse-data-forwarder` within a terminal window. Log in and select the Edge Impulse project you wish to use. Then just use the 'Data acquisition' tab within the EI Studio to collect your samples.
After generating features and training a model, download it as an Arduino library and import it with the `Sketch > Include Library > Add .ZIP Library` dialog. Finally, upload the [Gesture Classification](GestureClassification/GestureClassification.ino) sketch using the same procedure as before. Notice how the gesture that has the highest confidence is the one displayed on the OLED.
