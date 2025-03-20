#include <Wire.h>
#include <Adafruit_GFX.h> // Library for the LCD display
#include <Adafruit_SSD1306.h> // Library for the LCD display
#include <OneWire.h> // Library for the DS18B20 sensor
#include <DallasTemperature.h> // Library for the DS18B20 sensor

// OLED Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DS18B20 Sensor Settings
#define ONE_WIRE_BUS 2  // DS18B20 data pin connected to D2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
    Serial.begin(9600);

    // Initialize OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    sensors.begin(); // Initialize DS18B20 Sensor 
    display.clearDisplay(); // Clear OLED display
}

void loop() {
    sensors.requestTemperatures();  // Request temperature from sensor
    float temperatureC = sensors.getTempCByIndex(0);  // Get temperature in Celsius

    //Print to Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println("°C");

    // Display temperature on OLED
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20);
    display.print("Temp: ");
    display.print("   ");
    display.print(temperatureC);
    display.print(" C");
    display.display();

    delay(1000);  // Update every 1 second
}
