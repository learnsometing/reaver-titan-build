# Build Guide

*Reaver Titan Lighting System — Emperor's Children Build*

## Phase 1 — Breadboard Prototype

Goal: verify every electrical decision in the schematic before any permanent build. Each step has an explicit pass/fail check — do not proceed to the next step until the current one passes.

Phase 1 splits into four tracks. Tracks A and B can be completed in either order. Track C depends on Track A (Step A4 must pass first). Track D depends on Track A (Step A3) and Track B (Step B2).

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

#### Step A5 — Svelte UI Build and Flash

**What you need:** Node.js installed, ESP32-S3 with LED wired from Step A3, phone or laptop with WiFi.

**Depends on:** Step A4 passing, Track C Steps C1–C2 complete.

1. From the project root, run the build:
   ```bash
   npm run ui:build
   ```
   This generates `firmware/ui/ui.h`, which the sketch includes via `../ui/ui.h`.
2. Open and upload [`firmware/step-a5-web-ui-svelte/step-a5-web-ui-svelte.ino`](../firmware/step-a5-web-ui-svelte/step-a5-web-ui-svelte.ino).
3. Connect to WiFi network **Reaver-Titan**, password **warmaster40k**.
4. Open a browser and navigate to `192.168.4.1`.
5. Drag the brightness slider. Tap zone toggles and blaster state buttons and confirm they log to the serial monitor.

**Pass:** Brightness slider controls the LED in real time. Zone and blaster endpoints log to serial. The UI served is identical to `npm run ui:dev`.

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

#### Step C1 — Framework Selection and Setup ✓

Svelte + Vite scaffolded under `firmware/ui/`. Source lives there; root `package.json` gains `ui:dev` and `ui:build` scripts. All Svelte/Vite deps install at the project root — no nested `package.json`.

- `npm run ui:dev` — starts Vite dev server at `localhost:5173` with hot reload and a mock ESP32 server that stubs all HTTP endpoints
- `npm run ui:build` — bundles to a single inlined HTML file and runs `gen-ui-header.mjs` to write `firmware/step-a5-web-ui-svelte/ui.h`

See [`firmware/ui/README.md`](../firmware/ui/README.md) for full aesthetic spec, zone map, and endpoint reference.

---

#### Step C2 — Extract Web UI into Svelte Project ✓

Dark Mechanicus terminal UI built in `firmware/ui/src/App.svelte`. Replaces the inline HTML string from Step A4.

- Brightness slider (0–4095) — drives `GET /brightness?v=X`, controls LED channel 0
- Head / Torso zone toggles — `POST /zone/head` and `/zone/torso`
- Laser Blaster weapon card — taps into blaster detail view with IDLE / PRIME / FULL CHARGE / DISCHARGE state buttons (`POST /blaster/state`)

---

#### Step C3 — Build, Bundle, and Flash

See **Step A5** above. That step completes this track.

**Pass:** LED brightness slider works identically to Step A4, but the HTML is generated from the Svelte build rather than hand-written in the sketch.

---

### Track D — MOSFET Zone Prototype

**Depends on:** Step A3 (PCA9685 controlling LED via PWM) and Step B2 (5V power rails on breadboard).

Validate the MOSFET driver circuit for a single heatsink zone before wiring all five blaster channels. Uses the SOT-23 DIP breakout to make the IRLML6344 breadboard-compatible.

---

#### Step D1 — Single MOSFET Heatsink Zone (Ch 6) ✓

**What you need:** IRLML6344 soldered to SOT-23 DIP breakout board (with male headers), 10kΩ gate pull-down resistor, 6.8Ω 1W series resistor, one heatsink LED rope segment (5.11"), breadboard with 5V rails from Step B2, PCA9685 from Step A2.

**Prep — assemble the breakout board:**
1. Place the IRLML6344 on the SOT-23 breakout pads, orienting pin 1 (Gate) to pad G, pin 2 (Source) to pad S, pin 3 (Drain) to pad D. Solder all three pads.
2. Solder male headers to the DIP through-hole pads. Let cool before handling.

**Wire the circuit (5V rail → series resistor → LED rope → MOSFET → GND):**
3. Seat the breakout board on the breadboard.
4. Wire the 6.8Ω series resistor from the 5V rail to the LED rope anode (+).
5. Connect the LED rope cathode (−) to the breakout Drain pad.
6. Connect the breakout Source pad to the GND rail.
7. Connect PCA9685 channel 6 output to the breakout Gate pad.
8. Wire the 10kΩ pull-down resistor from the Gate pad to the GND rail.

**Upload firmware:**
9. Open and upload [`firmware/step-d1-mosfet-zone/step-d1-mosfet-zone.ino`](../firmware/step-d1-mosfet-zone/step-d1-mosfet-zone.ino).

**Pass:** LED rope fades smoothly on channel 6 with no flicker at startup. Measure current with a multimeter in series on the LED rope — expect ~72mA at full brightness.

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
