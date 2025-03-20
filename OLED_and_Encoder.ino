#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // Reset pin (not used)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define CLK 2  // Rotary Encoder CLK (A) pin
#define DT 3   // Rotary Encoder DT (B) pin

int lastStateCLK;
int temperature = 2;  // Start at 2°C

void setup() {
    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    lastStateCLK = digitalRead(CLK); // Read initial state

    updateDisplay();
}

void loop() {
    int currentStateCLK = digitalRead(CLK);

    // If the state changes, check direction
    if (currentStateCLK != lastStateCLK && currentStateCLK == HIGH) {
        if (digitalRead(DT) != currentStateCLK) {
            if (temperature < 12) temperature++;  // Increase temp
        } else {
            if (temperature > 2) temperature--;   // Decrease temp
        }
        updateDisplay();
    }

    lastStateCLK = currentStateCLK; // Store last CLK state
}

// Function to update the OLED display
void updateDisplay() {
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(25, 25);
    display.print(temperature);
    display.print(" C");
    display.display();
}
