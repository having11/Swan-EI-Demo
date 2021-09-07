#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>

LSM9DS1 imu;

#define PRINT_SPEED 10
static unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);

  Wire.begin();

  if(imu.begin() == false) {
        Serial.println("Failed to communicate with LSM9DS1.");
        Serial.println("Double-check wiring.");
        Serial.println("Default settings in this sketch will " \
                    "work for an out of the box LSM9DS1 " \
                    "Breakout, but may need to be modified " \
                    "if the board jumpers are.");
        while (1);
    }
}

void loop() {
    if (imu.accelAvailable())
        imu.readAccel();
    
    if ((lastPrint + PRINT_SPEED) < millis()) {
        Serial.print(imu.ax); Serial.print(',');
        Serial.print(imu.ay); Serial.print(',');
        Serial.print(imu.az);
        Serial.println();

        lastPrint = millis(); // Update lastPrint time
    }
}
