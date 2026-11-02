#include <Arduino.h>
#include <U8g2lib.h>

// Software I2C: (rotation, clock/SCL, data/SDA, reset)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ U8X8_PIN_NONE);

const int BUTTON_PIN = 7;
const int MAX_RABBITS = 5;

int rabbitCount = 0;     // number of rabbits on screen
int lastState = HIGH;    // HIGH = released (pull-up)

// Draws one rabbit. (cx, top) is the center x and the top of the ears.
void drawRabbit(int cx, int top) {
  u8g2.setDrawColor(1);
  u8g2.drawRBox(cx - 5, top, 4, 12, 2);      // left ear
  u8g2.drawRBox(cx + 1, top, 4, 12, 2);      // right ear
  u8g2.drawDisc(cx, top + 17, 7);            // head
  u8g2.drawDisc(cx, top + 28, 8);            // body
  u8g2.setDrawColor(0);
  u8g2.drawPixel(cx + 3, top + 15);          // eye
  u8g2.setDrawColor(1);
}

// Draws n rabbits evenly spaced across the screen.
void drawRabbits(int n) {
  for (int i = 0; i < n; i++) {
    int cx = 64 + (2 * i - (n - 1)) * 13;    // 26 px apart, centered
    drawRabbit(cx, 16);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  u8g2.begin();
}

void loop() {
  int state = digitalRead(BUTTON_PIN);

  // Count a press on the transition from released to pressed
  if (state != lastState) {
    delay(50);                               // simple debounce
    state = digitalRead(BUTTON_PIN);
    if (state == LOW && lastState == HIGH) {
      if (rabbitCount < MAX_RABBITS) {
        rabbitCount++;
      } else {
        rabbitCount = 0;                     // start over after 5
      }
    }
    lastState = state;
  }

  // Draw the status text and the rabbits
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 10, state == LOW ? "PRESSED" : "released");
  drawRabbits(rabbitCount);
  u8g2.sendBuffer();
}