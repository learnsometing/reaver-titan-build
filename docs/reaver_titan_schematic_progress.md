# Reaver Titan — KiCad Schematic Progress Tracker

**Project:** Reaver Titan Electrical System  
**KiCad version:** 10.0 (Windows)  
**Brief version:** v5.2  
**Schematic revision:** v1.0  

---

## Context

Warhammer 40K Reaver Titan, Emperor's Children chaos build. Slaanesh daemon-touched aesthetic. All lighting is green. Full lighting design is documented in the brief (v5.1). We are building the schematic incrementally in KiCad, learning the tool as we go.

The schematic will be built in sections. This tracker covers the **core power system** first, then expands to zone loads.

---

## KiCad Orientation Notes

- **A** — open symbol chooser (Add symbol)
- **W** — draw wire
- **L** — place net label
- **P** — place power symbol (in some versions; use A if P doesn't work)
- **G** — grab and move a label/wire without disconnecting it
- Net labels connect pins automatically when two labels share the same name — no wire needed across long distances
- A small green dot at a junction = connected. No dot = not connected.
- **Inspect → Electrical Rules Checker** — run ERC to verify connections
- Zoom in with scroll wheel when placing labels to ensure they snap to pin endpoints

---

## Completed So Far

### Power symbols placed
- [x] **+BATT** power symbol — top left area
- [x] **GND** power symbol — placed nearby

### Components placed and labelled

| Ref | Symbol used | Value | Notes |
|-----|-------------|-------|-------|
| BT1 | Device:Battery | Zeee 2S 2200mAh 7.4V | Positive pin wired to +BATT net, negative to GND |
| J1 | Connector:Conn_01x02 | T-Connector Deans 14AWG | Pin 1 on +BATT net, Pin 2 on GND |
| U1 | Connector:Conn_01x04 | JZK 2S 7.4V 8A BMS | 4-pin generic module |

### Net labels placed on U1 (BMS)

| Pin | Net label |
|-----|-----------|
| 1 | +BATT (battery positive in) |
| 2 | GND (battery negative in) |
| 3 | +PROT (protected positive out) |
| 4 | GND (protected negative out) |

### Wiring completed
- BT1 positive → +BATT symbol → J1 pin 1 (all on +BATT net)
- BT1 negative → GND
- J1 pin 2 → GND
- U1 pin 1 → +BATT net (via net label)
- U1 pin 2 → GND (via net label)
- U1 pin 3 → +PROT net label (not yet connected forward — next session)
- U1 pin 4 → GND (via net label)

### ERC status
Run after placing BT1, J1, U1. Results:
- 2x `power_pin_not_driven` errors — expected, will fix with PWR_FLAG symbols at end
- 1x `isolated_pin_label` warning on +PROT — expected, nothing connected to it yet
- **No real wiring errors**

---

## Up Next (start here next session)

### 1. Place SW1 — Power switch ✅ Decision: INCLUDE

Press **A**, search `SW_Push_Latching`, place to the right of U1.
- Reference: SW1
- Value: Latching Push Button

Wire: +PROT net label on SW1 pin 1. New net label on SW1 pin 2: **+SWITCHED**

> **Decision rationale:** The switch is hidden in the torso exterior and provides a clean hard disconnect during display sessions without opening the model or unplugging the battery. The battery is still always physically unplugged at the T-connector when not in use (LiPo fire safety). The switch is purely for display convenience.

---

### 2. Place U2 — Buck converter

**A** → search `Conn_01x04` (same generic 4-pin module as U1)
- Reference: U2
- Value: MP1584 Buck Converter 5V

Pins:
| Pin | Net label |
|-----|-----------|
| 1 | +SWITCHED (from SW1 pin 2) |
| 2 | GND |
| 3 | +5V |
| 4 | GND |

Add a text note near U2: **"Set output to 5.0V with multimeter BEFORE connecting load"**

---

### 3. Place U3 — ESP32-S3 Super Mini

**A** → search `Conn_01x04` (or a larger connector if you want more pins represented)
- Reference: U3
- Value: ESP32-S3 Super Mini

Minimum pins to show:
| Pin | Net label |
|-----|-----------|
| 1 | +5V |
| 2 | GND |
| 3 | I2C_SDA |
| 4 | I2C_SCL |

---

### 4. Place U4 — PCA9685 PWM driver

**A** → search `Conn_01x04`
- Reference: U4
- Value: PCA9685 16-ch PWM Driver

Minimum pins to show:
| Pin | Net label |
|-----|-----------|
| 1 | +5V |
| 2 | GND |
| 3 | I2C_SDA |
| 4 | I2C_SCL |

I2C_SDA and I2C_SCL net labels will connect U3 and U4 automatically.

---

### 5. Place J2 — Tank fire LED kit (direct 5V bus)

**A** → search `Conn_01x02`
- Reference: J2
- Value: Evan Designs Tank Fire LED Kit 5-12V

| Pin | Net label |
|-----|-----------|
| 1 | +5V |
| 2 | GND |

---

### 6. Add PWR_FLAG symbols (fixes ERC errors)

**A** → search `PWR_FLAG`. Place one on the **+BATT** net and one on the **GND** net. These tell KiCad's ERC that these nets are intentionally driven by a power source.

---

### 7. Run final ERC

Should be clean (zero errors, zero warnings) after PWR_FLAGs are added and all pins are connected.

---

## Full PCA9685 Channel Map (for future zone sheets)

| Ch | Zone | Effect | Drive |
|----|------|--------|-------|
| 0 | Eyes (fiber optic) | Warm-up ramp, then static | Direct, 150Ω, ~20mA |
| 1 | Brain A | Breathing pulse | TBD (MOSFET if >1 LED) |
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
| 12–15 | Unassigned | — | Reserved (wings, base, trophies) |

Tank fire LED kit: direct 5V bus, no PCA9685 channel needed.

---

## Key Electrical Decisions Locked In

- **Battery:** Zeee 2S 2200mAh 7.4V 50C Shorty LiPo, T-connector (Deans), 2-pack
- **BMS:** JZK 2S 7.4V 8A — over-charge, over-discharge, short circuit protection
- **Connector:** T-connector (Deans) — replaces JST due to current rating concerns
- **Buck converter:** MP1584 (3A) for prototype; upgrade to 5A module for final build
- **MCU:** ESP32-S3 Super Mini — WiFi web UI, dual-core, I2C to PCA9685
- **PWM driver:** SunFounder PCA9685, 16-channel, 12-bit, I2C
- **Wire gauge:** 14 AWG silicone on 7.4V side; 22 AWG stranded on 5V side
- **MOSFET:** IRLML6344 (SOT-23) for all high-current LED rope channels
- **Power switch:** Latching push button (SW1), hidden in torso exterior — for display session convenience. Battery always unplugged at T-connector when not in use.
- **All LEDs:** Green throughout. Clear resin parts used for diffusion.
