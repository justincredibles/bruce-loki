# boards/loki-hat — Bruce board definition for the Loki HAT

Board support for the **Loki HAT** ESP32-S3 coprocessor (Project Loki), so
Bruce can be built for Loki's custom PCB:

```sh
pio run -e loki-hat
```

## The board

- **MCU:** ESP32-S3-WROOM-1-N16R8 module (16 MB flash, 8 MB octal PSRAM)
- **Radios on one shared SPI bus (IO11/12/13):** CC1101 sub-GHz module
  (CS IO10, GDO0 IO9, GDO2 IO17), nRF24L01+ module (CE IO4, CSN IO5, IRQ IO6),
  microSD slot (CS IO14, CD IO7)
- **NFC:** PN532 on I2C (SDA IO8 / SCL IO18, IRQ IO40, RST IO41) — deliberately
  not on the RF SPI bus
- **IR:** TSOP38438 receiver on IO1, 940 nm LED via MMBT2222A driver on IO2
- **Buttons:** UP/DOWN/LEFT/RIGHT on IO15/IO16/IO38/IO39, active LOW. The
  button nets are **shared with the Pi** (the Pi reads them through 1 kΩ
  series resistors), which is what lets the ESP32 take on-device
  confirmations (e.g. the BadUSB arm gate) with the Pi powered off
- **Pi link:** hardware UART0 (TX IO43 / RX IO44) at 115200 8N1, carrying the
  Pi↔ESP32 serial protocol; ESP32 → Pi attention IRQ on IO42; the Pi also
  holds the ESP32's EN and IO0 lines, so it can reset/reflash the coprocessor
- **USB (port 2):** native ESP32-S3 USB — BadUSB HID and flashing only, with
  VBUS detect on IO21

## Headless

There is **no display on the ESP32** — the ILI9341-class screen is wired to
the Pi and driven by Loki's Pi-side software. This board therefore sets
`HAS_SCREEN 0`. Power/charge LEDs are likewise driven in hardware on the
power side (charger CHG pin, fuel-gauge ALRT pin), not by the ESP32.

Making the full Bruce tree build and behave with `HAS_SCREEN 0` is planned
fork work; this board definition is the starting point, not a finished port.

## Files

| File | Purpose |
|---|---|
| `loki-hat.ini` | PlatformIO environment (`pio run -e loki-hat`) |
| `pins_arduino.h` | Full GPIO map for the board |
| `interface.cpp` | GPIO setup, button input handler, battery stub |
| `../_boards_json/loki-hat.json` | PlatformIO board definition |

**Pin-map source of truth:** `hardware/PINOUT.md` §3.2 in the Loki repo
(github.com/justincredibles/Loki). If the PCB pinout changes, that document
changes first and this board definition follows.

## TODO before first hardware build

- [ ] **Partition CSV:** confirm Bruce ships `custom_16Mb.csv` and that its
      layout suits Loki (the board JSON falls back to `default_16MB.csv`).
- [ ] **PN532 macro names:** check the names Bruce's RFID module expects
      (here: `PN532_IRQ_PIN` / `PN532_RST_PIN`); if the module hardcodes its
      pins, patch the module instead of this header.
- [ ] **Button macro names:** verify `UP_BTN` / `DW_BTN` / `SEL_BTN` /
      `ESC_BTN` against Bruce's core input handling.
- [ ] **`HAS_SCREEN 0` build:** get a green build with no display compiled
      in, and confirm the serial CLI + protocol work headlessly over UART0.
- [ ] **Serial protocol mode:** implement the firmware side of the Pi↔ESP32
      serial protocol (`loki begin` structured mode, arm gate, job/event
      model) per `docs/protocol/pi-esp32-serial.md` in the Loki repo.
