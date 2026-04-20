# Core Infrastructure

*Reaver Titan Lighting System — Emperor's Children Build*

## Power System

| Component | Specification |
|-----------|---------------|
| **Battery** | Zeee 2S 2200mAh 7.4V 50C Shorty LiPo (soft pack). Dimensions: 73mm × 34mm × 18.5mm, weight 98g. T-connector (Deans) discharge plug, JST-XH balance lead. Removable via torso access panel for external charging. 2-pack purchased — one spare for swap-out during extended display. |
| **Connector** | T-connector (Deans) for battery-to-bus connection. Rated for high current, eliminates the JST undersizing concern flagged in earlier revisions. Internal bus wiring terminates in a matching T-connector for battery plug-in. |
| **Protection** | JZK 2S 7.4V 8A BMS protection board (41mm × 16mm × 3.5mm). Over-charge protection (4.25–4.35V per cell), over-discharge protection (2.5–3.0V), short circuit protection, standby current <10μA. 8A continuous / 10A peak — provides ample headroom over estimated 2A peak system draw. |
| **Charger** | 2S LiPo balance charger required (not included with battery). Must support JST-XH balance lead. Battery is removed from the model and charged externally. |
| **Voltage regulation** | 5V buck converter, minimum 3A rated. A 5A-rated adjustable buck module is recommended for expansion headroom (current peak draw is approximately 2A, but additional zones will increase this). MP1584 (2–3A) is acceptable for prototyping; consider upgrading to a 5A module (e.g., D24V50F5 or similar) for the final build if total draw exceeds 2.5A. |
| **Power switch** | Twidec L-PBS-110-XBK, 7mm latching push button, SPST, 3A 250V AC, pre-soldered wires. Mounted through the torso exterior rectangular housing (existing model feature). Indicator LED to be mounted in the domed feature on top of the same housing, wired to the +SWITCHED net via 270Ω resistor to glow green when system is on. Battery is still always physically unplugged at the T-connector when not in use (LiPo fire safety) — the switch is for convenience during display sessions only. |
| **Bus** | Common 5V and GND bus throughout. |
| **Runtime estimate** | Normal display mode (brain, eyes, tank fire, idle blaster, WiFi active): ~450–600mA draw, ~3–4 hours runtime. With blaster firing continuously: ~2.2A peak draw, ~1 hour runtime. Deep sleep (WiFi off, all LEDs off): <50μA, effectively unlimited standby. |

### Power Chain

![Power chain schematic](../kicad/power-chain.svg)

---

## Control System

### Microcontroller — ESP32-S3 Super Mini

ESP32-S3 Super Mini. Dual-core Xtensa LX7 at 240MHz, built-in WiFi (802.11 b/g/n) and Bluetooth LE 5.0, 512KB SRAM, 4MB flash. Dimensions: 22.52mm × 18mm. USB-C for programming. Runs at 3.3V logic, which the PCA9685 supports natively.

The dual-core architecture is critical: one core runs the WiFi web server and handles incoming commands, while the other runs the lighting animation loops (firing sequences, brain pulsing, breathing effects) without stuttering or timing interruptions.

Power draw is approximately 150–250mA when WiFi is active. On the 2200mAh battery this still provides several hours of runtime.

### PWM Driver — PCA9685

SunFounder PCA9685 16-channel PWM driver board, connected to the ESP32-S3 via I2C. Provides 16 independently controllable PWM channels with 12-bit resolution (0–4095), yielding smoother fading transitions than 8-bit PWM. Driven by the Adafruit PWM Servo Driver library (compatible with ESP32 Arduino framework).

The PCA9685 runs at 5V on the common bus with 3.3V logic from the ESP32-S3 for I2C communication — the PCA9685 is 5V-compliant on its logic inputs. Default PWM frequency is approximately 1kHz (adjustable). A second PCA9685 can be daisy-chained if more than 16 channels are ever needed.

#### Channel Map

| Ch | Zone | Effect | Drive |
|----|------|--------|-------|
| 0 | Eyes (fiber optic) | Warm-up ramp, then static | Direct, 150Ω, ~20mA |
| 1 | Brain A | Breathing pulse | TBD |
| 2 | Brain B | Breathing pulse, offset | TBD |
| 3 | Brain C | Breathing pulse, offset | TBD |
| 4 | Power fist knuckles | Pulse | 100Ω/LED |
| 5 | Blaster barrels | Multi-state firing | MOSFET IRLML6344, 6.8Ω 1W |
| 6 | Blaster heatsink rear (gaps 1–3) | Multi-state firing | MOSFET |
| 7 | Blaster heatsink mid-rear (gaps 4–5) | Multi-state firing | MOSFET |
| 8 | Blaster heatsink mid-front (gaps 6–8) | Multi-state firing | MOSFET |
| 9 | Blaster heatsink front (gaps 9–10) | Multi-state firing | MOSFET |
| 10 | Sensor eye (red) | Static + periodic blink | Direct, 100Ω, ~20mA |
| 11 | Mouth | Organic breathing rhythm | Direct, Evan Designs 5V chip LED |
| 12–15 | Unassigned | — | Reserved |

Tank fire LED kit (D1): direct 5V bus, no PCA9685 channel.

### Remote Control (Web UI)

The ESP32-S3 creates a WiFi access point or connects to an existing network. A lightweight web page served from the chip's flash provides the control interface:

- Zone on/off toggles
- Brightness sliders per zone
- Blaster firing sequence trigger
- Display mode presets (idle, show mode, full demo)
- Startup sequence control

Accessible from any device with a web browser — phone, tablet, or computer. No app installation required.

---

## Wire Gauge

| Side | Gauge | Rationale |
|------|-------|-----------|
| 7.4V (battery → BMS → switch → buck input) | 14 AWG stranded silicone | Carries full system current at battery voltage. Pre-wired T-connector leads are typically 14 AWG. |
| 5V (buck output → bus → all zone runs) | 22 AWG stranded | Rated for ~3A, provides headroom over any single run's current demand. |

---

## Bill of Materials — Core Power System

Components required to prototype the core power delivery and control system. Zone-specific components (LEDs, resistors, MOSFETs, LED rope) are listed in each zone doc.

| Component | Product | Qty | Source | Status | Notes |
|-----------|---------|-----|--------|--------|-------|
| **Battery** | Zeee 2S 2200mAh 7.4V 50C Shorty LiPo, T-connector (2-pack) | 1 pack | Amazon | Arrived | 73×34×18.5mm. Spare included for swap-out during extended display. |
| **Balance charger** | SUPULSE B3 Pro 25W 2S/3S LiPo balance charger | 1 | Amazon | ordered | AC wall plug, JST-XH balance port, 2000mA charge rate. Red/green LED status. Charges battery externally. |
| **LiPo safety bag** | Fireproof LiPo charging/storage bag | 1 | Amazon | Arrived | Always charge and store LiPo batteries inside a fireproof bag. |
| **BMS** | JZK 2S 7.4V 8A BMS protection board (5-pack) | 1 pack | Amazon | ordered | 41×16×3.5mm. Spares included. |
| **Buck converter** | EBOOT MP1584EN DC-DC 3A adjustable buck converter (6-pack) | 1 pack | Amazon | Arrived | 22×17×4mm. Set to 5V with potentiometer + multimeter before connecting anything. Spares included. |
| **Microcontroller** | Waveshare ESP32-S3 Mini (pre-soldered headers) | 2 | Amazon | ordered | Dual-core 240MHz, WiFi, BLE 5, USB-C. One for breadboard prototyping, one for final build. |
| **PWM driver** | SunFounder PCA9685 16-channel 12-bit PWM driver board | 1 | Amazon | Arrived | I2C, 5V logic compatible with 3.3V ESP32. Check for built-in series resistors on outputs. |
| **T-connectors** | T-connector (Deans) male/female pairs, pre-wired 14 AWG (multi-pack) | 1 pack | Amazon | Arrived | One pair for battery connection. Harvest 14 AWG wire from spare pairs for BMS-to-buck-converter runs. |
| **Power switch** | Twidec L-PBS-110-XBK latching push button (4-pack) | 1 | Amazon | Arrived | 7mm latching SPST, 3A 250V AC, pre-soldered wires. Mounted through torso exterior rectangular housing. Spares included in 4-pack. |
| **Breadboard wire** | TUOFENG 22 AWG solid core hookup wire (6-color kit) | 1 kit | Amazon | Arrived | Solid core for breadboard prototyping only. 30ft per color in dispensing box. |
| **Breadboard** | Full-size solderless breadboard (830 tie points) | 1 | Amazon | Arrived | Existing 300-point board is too small for ESP32 + PCA9685 + buck converter together. |
| **Resistor assortment** | BOJACK 1000pcs 25-value ¼W assortment (B08FD1XVL6) | 1 | Amazon | Arrived | Includes 100Ω and 150Ω needed for head zones. Covers all remaining ¼W resistor values for the full build. |
| **Test LED + resistor** | 5mm green LED + 150Ω resistor | 1 each | On hand | Arrived | From Arduino starter kit. Verifies PCA9685 output during core power-on test. |

*Tools assumed on hand: soldering iron, solder, wire strippers, multimeter, small screwdriver (for buck converter potentiometer adjustment).*

*Deferred purchases (buy when moving from breadboard to permanent build):* perfboard for junction board, male pin header strips, 22 AWG stranded silicone wire.

