# Build Guide

*Reaver Titan Lighting System — Emperor's Children Build*

## Phase 1 — Breadboard Prototype

Goal: verify every electrical decision in the schematic before any permanent build. Each step has an explicit pass/fail check — do not proceed to the next step until the current one passes.

### Step 1 — Set the Buck Converter to 5V

**What you need:** buck converter module, battery, multimeter.

Do this before connecting anything else to the buck converter output.

1. Connect the battery T-connector leads directly to the buck converter IN+ and IN−.
2. Place the multimeter probes on the buck converter OUT+ and OUT− pads.
3. Adjust the buck converter potentiometer until the multimeter reads **5.0V**.
4. Disconnect the battery.

**Pass:** multimeter reads 5.0V ±0.1V on the buck output.

---

### Step 2 — Wire the Breadboard Power Rails

**What you need:** breadboard, 22 AWG solid core wire, buck converter (set to 5V from Step 1), power switch.

1. Wire the power switch inline on the positive lead between the battery T-connector and buck converter IN+. Battery negative connects directly to buck IN−.
2. Connect buck OUT+ to the breadboard red (+) power rail.
3. Connect buck OUT− (GND) to the breadboard blue (−) power rail.
4. Plug in the battery and toggle the switch on. Place the multimeter probes on the breadboard power rails.

**Pass:** 5.0V ±0.1V across the breadboard power rails with the switch on. 0V with the switch off.

---

### Step 3 — ESP32-S3 Power-On *(waiting on ESP32-S3)*

**What you need:** ESP32-S3 Super Mini, USB-C cable, breadboard with 5V rail from Step 2.

1. Seat the ESP32-S3 across the breadboard center channel.
2. Connect ESP32 5V pin to the red power rail.
3. Connect ESP32 GND pin to the blue power rail.
4. Connect USB-C to your computer (for serial monitoring only — power comes from the rail).
5. Toggle the switch on. Open a serial monitor at 115200 baud.

**Pass:** ESP32-S3 boot log visible in serial monitor. No components warm to the touch after 2 minutes.

---

### Step 4 — PCA9685 I2C Connection *(waiting on ESP32-S3)*

**What you need:** SunFounder PCA9685 board, 4× jumper wires.

1. Seat the PCA9685 on the breadboard.
2. Connect PCA9685 VCC to the red power rail (5V).
3. Connect PCA9685 GND to the blue power rail.
4. Connect PCA9685 SDA to ESP32-S3 SDA pin.
5. Connect PCA9685 SCL to ESP32-S3 SCL pin.
6. Upload an I2C scanner sketch to the ESP32-S3.

**Pass:** I2C scanner reports a device at address 0x40 (PCA9685 default). No other unexpected addresses.

---

### Step 5 — Test LED via PCA9685 *(waiting on ESP32-S3)*

**What you need:** 5mm green LED, 150Ω resistor (from resistor assortment), jumper wires.

1. Wire the 150Ω resistor from PCA9685 channel 0 output to the LED anode.
2. Wire the LED cathode to GND rail.
3. Upload a test sketch using the Adafruit PWM Servo Driver library that fades channel 0 from 0 to 4095 and back on repeat.

**Pass:** LED fades smoothly from off to full brightness and back. No flicker at power-on (gate pull-down is not applicable here — this validates the PCA9685 output directly).

---

### Step 6 — WiFi Web UI *(waiting on ESP32-S3)*

**What you need:** phone or laptop with WiFi, ESP32-S3 running updated firmware.

1. Extend the Step 5 sketch to host a WiFi access point and a basic web page with a brightness slider for channel 0.
2. Connect your phone to the ESP32-S3 access point.
3. Open the web UI and adjust the slider.

**Pass:** LED brightness tracks the slider in real time with no flicker or lag. Access point appears within 10 seconds of power-on.

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
