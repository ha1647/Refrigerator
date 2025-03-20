#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // Reset pin (not used)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define CLK 2  // Rotary Encoder CLK (A) pin
#define DT 3   // Rotary Encoder DT (B) pin
#define TEMP_SENSOR_PIN 5  // DS18B20 Data pin

OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature sensors(&oneWire);

int lastStateCLK;
int setTemperature = 2;  // Default set temp
float currentTemperature = 0.0;

void setup() {
    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    lastStateCLK = digitalRead(CLK); // Read initial state

    sensors.begin();  // Start temperature sensor
    updateDisplay();
}

void loop() {
    // Read rotary encoder
    int currentStateCLK = digitalRead(CLK);
    if (currentStateCLK != lastStateCLK && currentStateCLK == HIGH) {
        if (digitalRead(DT) != currentStateCLK) {
            if (setTemperature < 12) setTemperature++;  // Increase temp
        } else {
            if (setTemperature > 2) setTemperature--;   // Decrease temp
        }
        updateDisplay();
          Serial.print(setTemperature);
    }
    lastStateCLK = currentStateCLK;

    // Read current temperature from sensor
    sensors.requestTemperatures();
    currentTemperature = sensors.getTempCByIndex(0);
    // Update display if temperature changes
    updateDisplay();
    delay(500); // Small delay for stability
}

// Function to update the OLED display
void updateDisplay() {
    display.clearDisplay();
    
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // Display current temperature
    display.setCursor(10, 20);
    display.print("Current: ");
    display.print(currentTemperature, 1); // 1 decimal place
    display.print(" C");

    // Display set temperature
    display.setCursor(10, 40);
    display.print("Set: ");
    display.print(setTemperature);
    display.print(" C");

    display.display();
}
