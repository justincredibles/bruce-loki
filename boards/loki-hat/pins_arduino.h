#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include "soc/soc_caps.h"
#include <stdint.h>

// Loki HAT - ESP32-S3-WROOM-1-N16R8 coprocessor (Project Loki).
// Pin map source of truth: hardware/PINOUT.md (section 3.2) in the Loki repo.
// Forbidden pins per Espressif guidance and the N16R8 module: IO26-32 (flash),
// IO33-37 (octal PSRAM), IO45/46 (strapping) - all left NC on the HAT.

#define USB_VID 0x303a
#define USB_PID 0x1001

// UART0: the Pi <-> ESP32 serial protocol link (Pi GPIO14/15), also esptool.
static const uint8_t TX = 43;
static const uint8_t RX = 44;

// UART2 unused on this board (IO1/IO2 are IR RX/TX instead).
static const uint8_t TXD2 = -1;
static const uint8_t RXD2 = -1;

// I2C: PN532 NFC controller (see PN532_* below).
static const uint8_t SDA = 8;
static const uint8_t SCL = 18;

// Default SPI (shared bus: CC1101 + nRF24 + microSD) - same as SPI_*_PIN below.
static const uint8_t SS = 10;
static const uint8_t MOSI = 11;
static const uint8_t MISO = 13;
static const uint8_t SCK = 12;

static const uint8_t G0 = 0;
static const uint8_t G1 = 1;
static const uint8_t G2 = 2;
static const uint8_t G3 = 3;
static const uint8_t G4 = 4;
static const uint8_t G5 = 5;
static const uint8_t G6 = 6;
static const uint8_t G7 = 7;
static const uint8_t G8 = 8;
static const uint8_t G9 = 9;
static const uint8_t G10 = 10;
static const uint8_t G11 = 11;
static const uint8_t G12 = 12;
static const uint8_t G13 = 13;
static const uint8_t G14 = 14;
static const uint8_t G15 = 15;
static const uint8_t G16 = 16;
static const uint8_t G17 = 17;
static const uint8_t G18 = 18;
static const uint8_t G19 = 19;
static const uint8_t G20 = 20;
static const uint8_t G21 = 21;
static const uint8_t G38 = 38;
static const uint8_t G39 = 39;
static const uint8_t G40 = 40;
static const uint8_t G41 = 41;
static const uint8_t G42 = 42;
static const uint8_t G43 = 43;
static const uint8_t G44 = 44;
static const uint8_t G46 = 46;
static const uint8_t G47 = 47;
static const uint8_t G48 = 48;

// No ADC battery sensing on the ESP32: the MAX17048 fuel gauge sits on the
// Pi's I2C bus, and getBattery() in interface.cpp reports via the Pi instead.
static const uint8_t ADC1 = -1;
static const uint8_t ADC2 = -1;

// No FM radio / I2S audio peripherals on the Loki HAT.
#define FM_RSTPIN -1
#define PIN_CLK -1
#define I2S_SCLK_PIN -1
#define I2S_DATA_PIN -1
#define PIN_DATA -1
#define BCLK -1
#define WCLK -1
#define DOUT -1

// No status LED on the ESP32 (the HAT's power/charging LEDs are driven in
// hardware on the power side). IO21 is USB VBUS detect, not an LED.
#define RGB_LED -1
#define VBUS_DETECT_PIN 21

// Attention line to the Pi (ESP32 -> Pi GPIO22): open-drain style, drive low
// or release. Raised on BadUSB-done / RF events.
#define ESP_ATTN_PIN 42

// BadUSB uses the S3 native USB HID (USB port 2). No CH9329-style serial
// BadUSB module, hence no BAD_TX/BAD_RX pins.
#define BAD_TX -1
#define BAD_RX -1

// Four navigation buttons, SHARED nets with the Pi (Pi reads them through
// 1k series resistors; neither chip ever drives these nets). Active LOW,
// pulled up on the HAT. Mapping (matches the Pi UI and interface.cpp):
//   UP -> Prev, DOWN -> Next, LEFT -> Select/enter, RIGHT -> Esc/back.
// TODO: verify the UP_BTN/DW_BTN/SEL_BTN/ESC_BTN macro names against what
// Bruce's core input handling actually reads at first build.
#define HAS_BTN 1
#define BTN_ALIAS "\"Up\""
#define BTN_PIN 15
#define BTN_ACT LOW
#define BTN_UP_PIN 15
#define BTN_DOWN_PIN 16
#define BTN_LEFT_PIN 38
#define BTN_RIGHT_PIN 39
#define UP_BTN BTN_UP_PIN
#define DW_BTN BTN_DOWN_PIN
#define SEL_BTN BTN_LEFT_PIN
#define ESC_BTN BTN_RIGHT_PIN

// IR: receiver is a TSOP38438 (OUT on IO1); TX LED is driven through an
// MMBT2222A transistor from IO2 - never drive the LED from the GPIO directly.
#define IR_TX_PINS '{{"IR LED", 2}}'
#define IR_RX_PINS '{{"TSOP38438", 1}}'
#define TXLED 2
#define LED_ON HIGH
#define LED_OFF LOW

// One-pin RF modules are NOT populated: sub-GHz is the CC1101 over SPI and
// 2.4 GHz is the nRF24 over SPI (see below).
#define RF_TX_PINS '{{"CC1101 (SPI)", -1}}'
#define RF_RX_PINS '{{"CC1101 (SPI)", -1}}'

// CC1101 sub-GHz module on the shared SPI bus.
#define CC1101_GDO0_PIN 9
#define CC1101_GDO2_PIN 17
#define CC1101_SS_PIN 10
#define CC1101_MOSI_PIN SPI_MOSI_PIN
#define CC1101_SCK_PIN SPI_SCK_PIN
#define CC1101_MISO_PIN SPI_MISO_PIN

// nRF24L01+ module on the shared SPI bus.
#define NRF24_CE_PIN 4
#define NRF24_SS_PIN 5
#define NRF24_IRQ_PIN 6
#define NRF24_MOSI_PIN SPI_MOSI_PIN
#define NRF24_SCK_PIN SPI_SCK_PIN
#define NRF24_MISO_PIN SPI_MISO_PIN

// PN532 NFC on I2C (0x24), deliberately NOT on the RF SPI bus.
// TODO: verify these macro names against Bruce's RFID/PN532 module at first
// build - if the module hardcodes its pins, patch the module instead.
#define PN532_IRQ_PIN 40
#define PN532_RST_PIN 41

#define FP 1
#define FM 2
#define FG 3

// HEADLESS: the display lives on the Pi side of the HAT, driven by the Pi
// over its own SPI bus. The ESP32 has no screen; Bruce menu rendering must
// not be compiled for this board. Making the full Bruce tree build and run
// with HAS_SCREEN 0 is planned fork work (see boards/loki-hat/README.md).
#define HAS_SCREEN 0

// microSD slot on the shared SPI bus (Bruce stores payloads, .sub files and
// PCAPs on SD). CD = card-detect input.
#define SDCARD_CS 14
#define SDCARD_SCK SPI_SCK_PIN
#define SDCARD_MISO SPI_MISO_PIN
#define SDCARD_MOSI SPI_MOSI_PIN
#define SDCARD_CD_PIN 7

// No Grove port on the Loki HAT.
#define GROVE_SDA -1
#define GROVE_SCL -1

// Shared SPI bus (SPI2): CC1101 + nRF24 + microSD, per-device CS.
#define SPI_SCK_PIN 12
#define SPI_MOSI_PIN 11
#define SPI_MISO_PIN 13
#define SPI_SS_PIN 10

#endif /* Pins_Arduino_h */
