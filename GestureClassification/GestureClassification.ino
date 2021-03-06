#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Make sure to add your own model first via Sketch > Include Library > Add .ZIP Library!
#include <Swan-EI-demo_inferencing.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

LSM9DS1 imu;

static uint64_t nextTick = 0;

void setup() {
    Serial.begin(115200);

    Wire.begin();

    delay(3000);

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        while(1);
    }

    display.display();
    delay(2000); // Pause for 2 seconds
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(SSD1306_WHITE);
    display.print("Test");
    display.display();
    delay(2000);

    if (imu.begin() == false) {
        Serial.println("Failed to communicate with LSM9DS1.");
        Serial.println("Double-check wiring.");
        Serial.println("Default settings in this sketch will " \
                    "work for an out of the box LSM9DS1 " \
                    "Breakout, but may need to be modified " \
                    "if the board jumpers are.");
        while (1);
    }
}

void loop()
{
    float readingBuf[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = { 0 };
    
    for(size_t i = 0; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i += 3) {
        uint64_t nextTick = micros() + (EI_CLASSIFIER_INTERVAL_MS * 1000);

        while(!imu.accelAvailable())
            ;
        imu.readAccel();
        
        readingBuf[i] = imu.ax;
        readingBuf[i + 1] = imu.ay;
        readingBuf[i + 2] = imu.az;

        delayMicroseconds(nextTick - micros());
    }
       
    // Time to classify
    signal_t signal;

    int err = numpy::signal_from_buffer(readingBuf, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if(err != 0) {
        ei_printf("Failed to create signal from buffer (%d)\n", err);
    }

    ei_impulse_result_t result = { 0 };

    err = run_classifier(&signal, &result, false);
    if (err != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", err);
    }

    uint8_t maxLabel = 0;
    
    for(size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {

        Serial.printf("%s: %3.3f\n", result.classification[ix].label, result.classification[ix].value);

        if(result.classification[ix].value > result.classification[maxLabel].value)
            maxLabel = ix;
    }

    // Show result
    display.clearDisplay();
    display.setCursor(10, 10);
    display.print(result.classification[maxLabel].label);
    display.display();
}
