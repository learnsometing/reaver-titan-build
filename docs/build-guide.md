# Build Guide

*Reaver Titan Lighting System — Emperor's Children Build*

## Phase 1 — Breadboard Prototype

Goal: verify every electrical decision in the schematic before any permanent build. Each step has an explicit pass/fail check — do not proceed to the next step until the current one passes.

Phase 1 splits into three tracks. Tracks A and B can be completed in either order. Track C depends on Track A (Step A4 must pass first).

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

#### Step A4 — WiFi Web UI ✓

**What you need:** phone or laptop with WiFi, ESP32-S3 with LED wired from Step A3.

**Required library:** `WebServer` — included with the ESP32 Arduino core, no extra install needed.

1. Open and upload [`firmware/step-a4-wifi-web-ui/step-a4-wifi-web-ui.ino`](../firmware/step-a4-wifi-web-ui/step-a4-wifi-web-ui.ino).
2. Open the serial monitor at 115200 baud. Wait for `Web server ready.` and note the AP IP (default: `192.168.4.1`).
3. On your phone, connect to WiFi network **Reaver-Titan**, password **warmaster40k**.
4. Open a browser and navigate to `192.168.4.1`.
5. Drag the brightness slider.

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

### Track C — Node.js UI Workflow

**Depends on:** Step A4 passing.

Prove out the development workflow for building the web UI as a proper Node.js project and bundling it into the Arduino sketch, so all future UI work can be done with a real browser dev environment instead of editing HTML strings in firmware.

#### Step C1 — Framework Selection and Setup

*To be planned. See [issue #33](https://github.com/learnsometing/reaver-titan-build/issues/33).*

---

#### Step C2 — Extract Web UI into Node.js Project

*To be planned.*

---

#### Step C3 — Build, Bundle, and Flash

*To be planned.*

**Pass:** LED brightness slider works identically to Step A4, but the HTML is now generated from the Node.js build rather than hand-written in the sketch.

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
