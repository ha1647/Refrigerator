#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Display Setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pins
#define BUTTON_PIN 8  // Player's Button
#define GREEN_LED 6   // Success Indicator
#define RED_LED 5     // Failure Indicator

// Animation Variables
#define NUMFLAKES     10 // Number of snowflakes in the animation example
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

// Game Variables
#define F1_THRESHOLD 500  // Reaction Time Threshold
bool gameActive = false;
unsigned long startTime, reactionTime;

void setup() {
    Serial.begin(9600);

    // Pin Modes
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);

    // Initialize OLED Display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    delay(2000);
    testfillrect();    
    
    // Welcome message
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 10);
    display.setTextColor(SSD1306_WHITE);
    welcometext();    
    display.display();
    delay(2000);

    testfillroundrect();
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 25);
    display.println("Chilly   McFrosty!!");
    display.display();
    delay(2000);
}

// The main reaction game loop
void loop() {
    gameActive = false;
    
    // Display challenge message
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Can you   beat an F1 Driver?   (200ms)");
    display.display();
    delay(2000);

    // Countdown before start
    for (int i = 3; i > 0; i--) {
        display.clearDisplay();
        display.setTextSize(5);
        display.setCursor(50, 20);
        display.print(i);
        display.display();
        delay(1000);
    }

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 20);
    display.println("Wait for  it...");
    display.display();
    delay(random(0, 5000));
    
    // Game starts
    display.clearDisplay();
    display.setTextSize(4);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(30, 20);
    display.println("GO!");
    display.display();
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    
    gameActive = true;
    startTime = millis();
    
    // Wait for player input
    while (gameActive) {
        if (digitalRead(BUTTON_PIN) == LOW) {
            reactionTime = millis() - startTime;
            announceResult(reactionTime);
            testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
            return;
        }
    }
}

// Determines the result and displays it
void announceResult(unsigned long reactionTime) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 10);
    
    display.print("Time:    ");
    display.print(reactionTime);
    display.println("ms");
    display.println("         ");

    if (reactionTime < F1_THRESHOLD) {
        display.println("F1 Level  Reflexes!!");
        digitalWrite(GREEN_LED, HIGH);
    } else {
        display.println("Tough    Luck!");
        digitalWrite(RED_LED, HIGH);
    }
    
    display.display();
    delay(3000);
}

void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][3];

  // Initialize 'snowflake' positions
  for(f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
    icons[f][YPOS]   = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
  }

  for(;;) { // Loop forever...
    display.clearDisplay(); // Clear the display buffer

    // Draw each snowflake:
    for(f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
    }

    display.display(); // Show the display buffer on the screen
    delay(200);        // Pause for 1/10 second

    // Then update coordinates of each flake...
    for(f=0; f< NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= display.height()) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
        icons[f][YPOS]   = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 2);
      }
    }
  }
}

void welcometext(void) {
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("Welcome  To ..."));
  display.display();     
  delay(100);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}

void testfillrect(void) {
  display.clearDisplay();
  for(int16_t i=0; i<display.height()/2; i+=3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, SSD1306_INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }
  delay(2000);
}

void testfillroundrect(void) {
  display.clearDisplay();
  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    // The INVERSE color is used so round-rects alternate white/black
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_INVERSE);
    display.display();
    delay(1);
  }
  delay(2000);
}



