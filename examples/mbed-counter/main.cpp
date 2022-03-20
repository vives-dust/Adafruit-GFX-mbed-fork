#include "mbed.h"
#include "Adafruit_SSD1306_I2c.h"
#include <Fonts/FreeSans9pt7b.h>

DigitalOut led(LED1);

EventQueue queue;

I2C i2c(I2C_SDA,I2C_SCL);
Adafruit_SSD1306_I2c oledDisplay(i2c, NC, SSD_I2C_ADDRESS, 64, 128, SSD_1306);

uint16_t counter = 0;

void display_counter() {
  oledDisplay.clearDisplay();
  oledDisplay.drawRect(0, 0, 128, 64, WHITE);
  oledDisplay.setTextSize(1);
  oledDisplay.setCursor(5, 5);
  oledDisplay.print("Counting ...");
  oledDisplay.setTextSize(3);
  oledDisplay.setCursor(20, 25);

  char buffer[32] = {};
  sprintf(buffer, "%d", counter++);
  oledDisplay.print(buffer);
  oledDisplay.display();

  led = !led;
}

int main() {
  led = 0;

  i2c.frequency(400000);

  oledDisplay.begin();
  oledDisplay.clearDisplay();

  printf("\r\n*** Welcome to OLED Counting Demo ***\r\n");

  oledDisplay.drawRect(0, 0, 128, 64, WHITE);
  oledDisplay.setCursor(4, 20);
  oledDisplay.setTextSize(2);
  oledDisplay.print("Count Demo\n");
  oledDisplay.setTextSize(1);
  oledDisplay.setCursor(4, 40);
  oledDisplay.print("v0.1");
  oledDisplay.display();

  queue.call_every(2s, display_counter);
  queue.dispatch_forever();
}
