Dobby: ESP32-C3 Desk Companion

Dobby is a small desk companion built on an ESP32-C3 SuperMini. It shows animated faces on a 128x64 OLED display and changes expression when you press a button.

Features
Five faces: blinking, looking up, petted, goodbye, and happy
Button control: each press moves to the next face, wrapping back to the first
Software I2C for the OLED display
Hardware
ESP32-C3 SuperMini microcontroller
0.96" or 1.3" OLED display, SSD1306 or SH1106 driver, 128x64 pixels
Pushbutton, wired to GPIO 7 and GND (internal pull-up)
Planned: TTP223 touch sensor to replace the button
Wiring
Part	ESP32-C3 pin
OLED SDA	GPIO 4
OLED SCL	GPIO 5
OLED VCC	3V3
OLED GND	GND
Button	GPIO 7 and GND
Software
Arduino IDE 2 with the ESP32 board package by Espressif
Libraries: U8g2
Status
 Display working in Wokwi simulator
 Button cycles through all five faces
 Touch sensor replaces the button
 Alarm and exercise timer
What I learned
How I2C connects a display to a microcontroller
Why buttons need debouncing
How to structure an embedded project in Git

Once you’ve saved it, run git add README.md, git commit -m "Add README", then git push. Keep the Status checklist updated as you finish each feature, since it shows your progress to anyone who looks at the repo.
