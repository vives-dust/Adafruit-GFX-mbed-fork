
#include <cstdio>
#include <TaskManager.h>
#include <SwitchInput.h>
#include "mbed.h"
//#include "Adafruit_SSD1306_I2c.h"
#include "Adafruit_SSD1306_Spi.h"
#include <IoLogging.h>
#include <Fonts/FreeSans9pt7b.h>

// Host PC Communication channels
BufferedSerial pc(USBTX, USBRX); // tx, rx
FILE* serPort = fdopen(&pc, "w");
MBedLogger LoggingPort(pc);

//I2C i2c(PF_0,PF_1);
//Adafruit_SSD1306_I2c gfx(i2c, NC, SSD_I2C_ADDRESS, 64, 132, SH_1106);

SPI spi(PA_7, PA_6, PA_5);
Adafruit_SSD1306_Spi gfx(spi, PD_15, PF_12, PF_13, 64, 128, SSD_1306);

bool exitApp = false;

const uint8_t iconWifiThreeBar[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x03,
        0x30, 0x0c, 0x08, 0x10, 0xc0, 0x03, 0x20, 0x04, 0x80, 0x01, 0x80, 0x01
};
int main()
{
    //i2c.frequency(400000);
    gfx.begin();
    pc.set_baud(115200);
    fprintf(serPort, "Hello from mbed graphics demo\n");

    switches.initialise(internalDigitalIo(), true);
    switches.addSwitch(PE_4, [] (pinid_t id, bool held) {
        fprintf(serPort, "Switch Pressed %d, %d\n", (int)id, (int) held);
    });
    switches.addSwitch(USER_BUTTON, [] (pinid_t id, bool held) {
        fprintf(serPort, "User Pressed %d, %d\n", (int)id, (int) held);
    }, NO_REPEAT, true);

    setupRotaryEncoderWithInterrupt(PE_2, PE_5, []( int val) {
        fprintf(serPort, "Encoder %d\n", val);
    });

    fprintf(serPort, "Created\n");

    // Display with the Adafruit Library
    gfx.clearDisplay();

    taskManager.scheduleFixedRate(75, [] {
        gfx.setFont(nullptr);
        gfx.setCursor(0, 0);
        gfx.print("hello world");
        gfx.fillRect(10, 25, 50, 10, BLACK);
        gfx.setCursor(10, 25);
        gfx.print((double) millis() / 1000.0);
        gfx.drawXBitmap(40, 40, iconWifiThreeBar, 16, 12, WHITE);
        gfx.drawCircle(100, 40, 10, WHITE);
        gfx.display();
    });

    while(!exitApp) {
        taskManager.runLoop();
    }
}