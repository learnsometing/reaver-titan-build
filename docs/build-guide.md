# Build Guide

*Reaver Titan Lighting System — Emperor's Children Build*

## Phase 1 — Breadboard Prototype

Goal: verify every electrical decision in the schematic before any permanent build. Each step has an explicit pass/fail check — do not proceed to the next step until the current one passes.

Phase 1 splits into two independent tracks that can be completed in either order.

---

### Track A — Firmware Prototype (USB-powered)

Validate firmware and component communication with the ESP32-S3 powered via USB only. No battery or buck converter required.

#### Step A1 — ESP32-S3 Hello World ✓

**What you need:** ESP32-S3 Super Mini, USB-C cable.

1. Connect USB-C to your computer. Open a serial monitor at 115200 baud.
2. Open and upload [`firmware/step-a1-hello-world/step-a1-hello-world.ino`](../firmware/step-a1-hello-world/step-a1-hello-world.ino).

**Pass:** "Reaver online." visible in serial monitor.

---

#### Step A2 — PCA9685 I2C Connection ✓

**What you need:** SunFounder PCA9685 board, breadboard, 4× jumper wires.

1. Seat the ESP32-S3 on the breadboard straddling the gap
2. Place the PCA9685 next to the breadboard.
3. Connect ESP32-S3 5V Pin to Red Rail, GND pin to Blue Rail
4. Connect PCA9685 VCC to 5V Rail; PCA9685 GND to GND Rail.
5. Connect PCA9685 SDA to ESP32-S3 GPIO 8; SCL to GPIO 9.
6. Open and upload [`firmware/step-a2-i2c-scan/step-a2-i2c-scan.ino`](../firmware/step-a2-i2c-scan/step-a2-i2c-scan.ino).

**Pass:** I2C scanner reports a device at address 0x40 (PCA9685 default). No other unexpected addresses.

```
ESP-ROM:esp32s3-20210327
Scanning I2C bus...
Device found at address 0x40
Scan complete.
```

---

#### Step A3 — LED Fade via PCA9685 ✓

**What you need:** 5mm green LED, 150Ω resistor, jumper wires.

1. Wire the 150Ω resistor from PCA9685 channel 0 output to the LED anode.
2. Wire the LED cathode to GND.
3. Open and upload [`firmware/step-a3-led-fade/step-a3-led-fade.ino`](../firmware/step-a3-led-fade/step-a3-led-fade.ino).

**Pass:** LED fades smoothly from off to full brightness and back.

---

#### Step A4 — WiFi Web UI

**What you need:** phone or laptop with WiFi, ESP32-S3 running updated firmware.

1. Extend the Step A3 sketch to host a WiFi access point and a basic web page with a brightness slider for channel 0.
2. Connect your phone to the ESP32-S3 access point.
3. Open the web UI and adjust the slider.

**Pass:** LED brightness tracks the slider in real time with no flicker or lag. Access point appears within 10 seconds of power-on.

---

### Track B — Power Chain Verification

Validate the battery → switch → buck → 5V rail chain independently of firmware work.

#### Step B1 — Set the Buck Converter to 5V ✓

**What you need:** buck converter module, battery, multimeter.

Do this before connecting anything else to the buck converter output.

1. Connect the battery T-connector leads directly to the buck converter IN+ and IN−.
2. Place the multimeter probes on the buck converter OUT+ and OUT− pads.
3. Adjust the buck converter potentiometer until the multimeter reads **5.0V**.
4. Disconnect the battery.

**Pass:** multimeter reads 5.0V ±0.1V on the buck output.

---

#### Step B2 — Wire the Breadboard Power Rails ✓

**What you need:** breadboard, 22 AWG solid core wire, buck converter (set to 5V from Step B1), power switch.

1. Wire the power switch inline on the positive lead between the battery T-connector and buck converter IN+. Battery negative connects directly to buck IN−.
2. Connect buck OUT+ to the breadboard red (+) power rail.
3. Connect buck OUT− (GND) to the breadboard blue (−) power rail.
4. Plug in the battery and toggle the switch on. Place the multimeter probes on the breadboard power rails.

**Pass:** 5.0V ±0.1V across the breadboard power rails with the switch on. 0V with the switch off.

---

## Phase 2 — Perfboard Build

To be planned once Phase 1 passes all steps.

---

## Phase 3 — Custom PCB

To be planned once Phase 2 is complete and verified in the model.

---

## Zone Prototyping Order

Once the core infrastructure breadboard passes all steps, prototype zones in this order:

1. Blaster arm — most complex: 5 channels, multi-state firing sequence, LED rope characterisation
2. Brain — 3-channel organic pulse, sine wave tuning
3. Head fiber optics — static, simplest zone
4. Power fist — single channel pulse
5. Tank fire kit — direct bus, verify inrush current
6. Full integration and state machine coding
