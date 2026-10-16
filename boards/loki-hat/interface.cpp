#include "core/powerSave.h"
#include <interface.h>

// Loki HAT board interface (Project Loki).
// HEADLESS board: no display on the ESP32 - the screen, menus and text entry
// live on the Pi side. Inputs are the four navigation buttons, whose nets are
// SHARED with the Pi (see pins_arduino.h and hardware/PINOUT.md section 3.3
// in the Loki repo): UP/DOWN/LEFT/RIGHT on IO15/IO16/IO38/IO39, active LOW.

/***************************************************************************************
** Function name: _setup_gpio()
** Location: main.cpp
** Description:   initial setup for the device
***************************************************************************************/
void _setup_gpio() {
    // Buttons have 10k pull-ups on the HAT; INPUT_PULLUP is belt-and-braces.
    pinMode(BTN_UP_PIN, INPUT_PULLUP);
    pinMode(BTN_DOWN_PIN, INPUT_PULLUP);
    pinMode(BTN_LEFT_PIN, INPUT_PULLUP);
    pinMode(BTN_RIGHT_PIN, INPUT_PULLUP);
    // microSD card-detect.
    pinMode(SDCARD_CD_PIN, INPUT_PULLUP);
}

/***************************************************************************************
** Function name: _post_setup_gpio()
** Location: main.cpp
** Description:   second stage gpio setup to make a few functions work
***************************************************************************************/
void _post_setup_gpio() {}

/***************************************************************************************
** Function name: getBattery()
** location: display.cpp
** Description:   Delivers the battery value from 1-100
***************************************************************************************/
int getBattery() {
    // The MAX17048 fuel gauge is on the Pi's I2C bus, not the ESP32's, so the
    // ESP32 cannot read the battery itself. 0 = "unknown" for Bruce's UI;
    // battery state reaches the ESP32 (if ever needed) via the Pi over the
    // serial protocol, and the low-battery LED is driven by the gauge in
    // hardware on the power side.
    return 0;
}

/*********************************************************************
** Function: setBrightness
** location: settings.cpp
** set brightness value
**********************************************************************/
void _setBrightness(uint8_t brightval) {
    // Headless board: no local display or backlight to dim. The Pi owns the
    // display and its backlight (Pi GPIO18 PWM through a FET).
}

/*********************************************************************
** Function: InputHandler
** Handles the variables PrevPress, NextPress, SelPress, AnyKeyPress and EscPress
**
** Button mapping - identical to the Pi UI convention:
**   UP (IO15)    -> PrevPress   (move up)
**   DOWN (IO16)  -> NextPress   (move down)
**   LEFT (IO38)  -> SelPress    (enter / select)
**   RIGHT (IO39) -> EscPress    (back; the Pi treats long-press as Home)
** Long-press handling lives in the reader (Pi UI / orchestrator); here we
** only report presses, exactly like every other Bruce board.
**********************************************************************/
void InputHandler(void) {
    checkPowerSaveTime();
    PrevPress = false;
    NextPress = false;
    SelPress = false;
    AnyKeyPress = false;
    EscPress = false;

    bool upPressed = digitalRead(BTN_UP_PIN) == LOW;
    bool downPressed = digitalRead(BTN_DOWN_PIN) == LOW;
    bool leftPressed = digitalRead(BTN_LEFT_PIN) == LOW;
    bool rightPressed = digitalRead(BTN_RIGHT_PIN) == LOW;

    if (upPressed || downPressed || leftPressed || rightPressed) {
        if (!wakeUpScreen()) AnyKeyPress = true;
        else goto END;
    }
    if (upPressed) { PrevPress = true; }
    if (downPressed) { NextPress = true; }
    if (rightPressed) { EscPress = true; }
    if (leftPressed) { SelPress = true; }
END:
    if (AnyKeyPress) {
        long tmp = millis();
        while ((millis() - tmp) < 200 &&
               (digitalRead(BTN_UP_PIN) == LOW || digitalRead(BTN_DOWN_PIN) == LOW ||
                digitalRead(BTN_LEFT_PIN) == LOW || digitalRead(BTN_RIGHT_PIN) == LOW));
    }
}

/*********************************************************************
** Function: keyboard
** location: mykeyboard.cpp
** Starts keyboard to type data
**********************************************************************/
String keyboard(String mytext, int maxSize, String msg) {
    // Headless board: no on-device keyboard. Text entry happens on the Pi
    // side (UI / web UI) and reaches the ESP32 as finished strings over the
    // serial protocol.
    return "";
}

/*********************************************************************
** Function: powerOff
** location: mykeyboard.cpp
** Turns off the device (or try to)
**********************************************************************/
void powerOff() {
    // Power is managed on the Pi/power side of the HAT; the ESP32 cannot cut
    // its own supply. A Pi-issued deep sleep over the serial protocol is the
    // low-power path.
}

/*********************************************************************
** Function: checkReboot
** location: mykeyboard.cpp
** Btn logic to turn off the device (name is odd btw)
**********************************************************************/
void checkReboot() {
    // No dedicated power/reboot button on the ESP32 side; the HAT's power
    // button and reboot flow live on the Pi/power side.
}
