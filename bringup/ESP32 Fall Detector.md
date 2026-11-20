# ESP32 Fall Detector

A wearable-style fall-detection alert prototype built on an ESP32 and an MPU-6500 IMU. The device watches accelerometer data for the signature of a fall (a free-fall dip, an impact spike, then stillness) and sounds an alarm unless the user cancels it with a button.

> **Note:** This is a learning and portfolio prototype, not a medical device. It has not been validated for real-world fall detection.

## Goals

This project is built from scratch, register by register, to practice core embedded skills:

- I2C communication and reading sensor registers directly (no IMU library)
- Converting raw sensor data into physical units
- Threshold tuning for event detection
- A state machine for alarm logic
- Timers and non-blocking code
- Button debouncing
- Driving outputs (OLED display, buzzer, LED)

## Status

- [x] Toolchain set up, blink sketch and serial output working
- [x] I2C scan finds the IMU at address `0x68`
- [x] WHO_AM_I register (`0x75`) reads `0x70`, confirming a genuine MPU-6500
- [ ] Wake the IMU and read accelerometer X/Y/Z
- [ ] Fall detection: free-fall, impact, and stillness thresholds
- [ ] State machine: Idle → Fall Suspected → Alarm → Cancelled
- [ ] OLED status display, buzzer and LED alarm, cancel button
- [ ] Stretch: Wi-Fi or Bluetooth alert to a phone

## Hardware

| Component | Details |
|---|---|
| Microcontroller | ESP32 dev board (ESP32-D0WD-V3, rev v3.1), dual core 240 MHz, Wi-Fi + Bluetooth, CP2102 USB-to-serial |
| IMU | GY-6500 module (InvenSense MPU-6500, 3-axis accelerometer + 3-axis gyroscope), I2C |
| Planned outputs | 0.96" OLED display, active buzzer, LED, push button |

Most parts come from the Elegoo Super Starter Kit.

## Wiring

| GY-6500 pin | ESP32 pin | Notes |
|---|---|---|
| VCC | 3.3V | Not 5V: the ESP32 uses 3.3V logic |
| GND | GND | |
| SDA | GPIO 21 | I2C data |
| SCL | GPIO 22 | I2C clock |

The remaining module pins (EDA, ECL, AD0, INT, NCS, FSYNC) are unconnected. With AD0 left low, the IMU's I2C address is `0x68`.

## Software setup

1. Install [Arduino IDE](https://www.arduino.cc/en/software) (built with 2.3.11).
2. Add the **esp32** board package by Espressif Systems through the Boards Manager.
3. Install the [Silicon Labs CP210x driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers) if the board's COM port doesn't appear.
4. Select **Tools → Board → ESP32 Dev Module** and the board's COM port.
5. Open the Serial Monitor at **115200 baud**.

**Troubleshooting**
- Upload stuck on `Connecting...`: hold the **BOOT** button until the upload starts.
- No serial output after upload: press **EN** to reset the board and rerun `setup()` with the monitor open.
- `serial TX path seems to be down`: check the wiring. In this build it was caused by a wiring mistake.

## Repository structure

```
fall-detector-esp32/
├── bringup/          Small test sketches used to bring up the hardware, one step at a time
│   ├── blink/        Verifies toolchain, upload, and serial output
│   ├── i2c_scan/     Scans the I2C bus for devices
│   └── who_am_i/     Reads the IMU's WHO_AM_I register to confirm the chip
├── docs/             Wiring notes and bring-up log
└── src/              Main fall-detector firmware (in progress)
```

## Bring-up results

**I2C scan:** one device found at `0x68`, the expected MPU-6500 address.

**WHO_AM_I read:**
```
Status: 0
Bytes received: 1
WHO_AM_I value: 0x70
```
`Status: 0` means the IMU acknowledged both its address and the register number. `0x70` is the MPU-6500's chip ID, so the board's label is correct.

## How it works (planned)

A fall shows up in accelerometer data as three phases:

1. **Free fall:** total acceleration drops well below 1 g.
2. **Impact:** a sharp spike well above 1 g.
3. **Stillness:** little motion afterward.

When all three happen in order within a short window, the device enters an alarm state. The user has a few seconds to press the cancel button before the alarm sounds.

## Credits

- I2C scanner based on the standard Arduino I2C scanner example.
- Register addresses from the InvenSense MPU-6500 datasheet and register map.