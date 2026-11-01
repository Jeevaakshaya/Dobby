#include <Arduino.h>
#include <U8g2lib.h>

// Software I2C: (rotation, clock/SCL, data/SDA, reset)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ U8X8_PIN_NONE);

// Button: GPIO 7 to one leg, other leg to GND (internal pull-up, LOW when pressed)
const int BUTTON_PIN = 7;
const unsigned long DEBOUNCE_MS = 50;

int currentFace = 1;             // starts on the blinking eyes
int stableState = HIGH;
int lastReading = HIGH;
unsigned long lastChangeTime = 0;

// Eye positions
const int LEFT_X = 40;
const int RIGHT_X = 88;
const int EYE_Y = 30;

// ---------- Eye helpers ----------

// Open, glowing eye: filled rounded box with a dark pupil and a white highlight
void drawOpenEye(int cx, int cy, int px, int py) {
  u8g2.setDrawColor(1);
  u8g2.drawRBox(cx - 14, cy - 14, 28, 28, 5);
  u8g2.setDrawColor(0);
  u8g2.drawDisc(cx + px, cy + py, 6);
  u8g2.setDrawColor(1);
  u8g2.drawDisc(cx + px + 2, cy + py - 2, 2);
}

// Closed eye: a thin horizontal line
void drawClosedEye(int cx, int cy) {
  u8g2.setDrawColor(1);
  u8g2.drawBox(cx - 14, cy - 1, 28, 3);
}

// Happy closed eye: upper half of a circle (upside-down U)
void drawHappyEye(int cx, int cy) {
  u8g2.setDrawColor(1);
  u8g2.drawCircle(cx, cy + 4, 10, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT);
}

// ---------- The five faces ----------

// Face 1: Blinking. Open eyes that close briefly every 2 seconds.
void faceBlink() {
  bool closed = (millis() % 2000) < 150;
  if (closed) {
    drawClosedEye(LEFT_X, EYE_Y);
    drawClosedEye(RIGHT_X, EYE_Y);
  } else {
    drawOpenEye(LEFT_X, EYE_Y, 0, 0);
    drawOpenEye(RIGHT_X, EYE_Y, 0, 0);
  }
}

// Face 2: Looking up. Open eyes with pupils pushed toward the top.
void faceLookUp() {
  drawOpenEye(LEFT_X, EYE_Y, 0, -7);
  drawOpenEye(RIGHT_X, EYE_Y, 0, -7);
}

// Face 3: Petted. Happy closed eyes, blush dots, and a small smile.
void facePetted() {
  drawHappyEye(LEFT_X, EYE_Y);
  drawHappyEye(RIGHT_X, EYE_Y);
  u8g2.setDrawColor(1);
  u8g2.drawDisc(LEFT_X - 12, EYE_Y + 14, 2);
  u8g2.drawDisc(RIGHT_X + 12, EYE_Y + 14, 2);
  u8g2.drawCircle(64, 46, 7, U8G2_DRAW_LOWER_LEFT | U8G2_DRAW_LOWER_RIGHT);
}

// Face 4: Goodbye. Flat sleepy eyes with a "Bye!" label.
void faceGoodbye() {
  drawClosedEye(LEFT_X, EYE_Y);
  drawClosedEye(RIGHT_X, EYE_Y);
  u8g2.setFont(u8g2_font_ncenB10_tr);
  int textX = (128 - u8g2.getStrWidth("Bye!")) / 2;
  u8g2.drawStr(textX, 62, "Bye!");
}

// Face 5: Happy. Big open eyes with a wide smile.
void faceHappy() {
  drawOpenEye(LEFT_X, EYE_Y, 0, 0);
  drawOpenEye(RIGHT_X, EYE_Y, 0, 0);
  u8g2.setDrawColor(1);
  u8g2.drawCircle(64, 44, 12, U8G2_DRAW_LOWER_LEFT | U8G2_DRAW_LOWER_RIGHT);
}

// ---------- Setup and loop ----------

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  u8g2.begin();
}

void loop() {
  unsigned long now = millis();

  // Each button press moves to the next face. After face 5 it wraps to face 1.
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastReading) {
    lastChangeTime = now;
  }
  if ((now - lastChangeTime) > DEBOUNCE_MS && reading != stableState) {
    stableState = reading;
    if (stableState == LOW) {          // button just pressed
      currentFace = (currentFace % 5) + 1;
    }
  }
  lastReading = reading;

  // Draw the current face
  u8g2.clearBuffer();
  switch (currentFace) {
    case 1: faceBlink();   break;
    case 2: faceLookUp();  break;
    case 3: facePetted();  break;
    case 4: faceGoodbye(); break;
    case 5: faceHappy();   break;
  }
  u8g2.sendBuffer();

  delay(20);   // keeps the blink animation smooth
}