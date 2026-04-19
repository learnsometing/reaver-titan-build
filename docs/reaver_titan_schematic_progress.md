# Reaver Titan — KiCad Schematic Progress Tracker

**Project:** Reaver Titan Electrical System  
**KiCad version:** 10.0 (Windows)  
**Brief version:** v5.2  
**Schematic revision:** v1.1  

---

## Context

Warhammer 40K Reaver Titan, Emperor's Children chaos build. Slaanesh daemon-touched aesthetic. All lighting is green. Full lighting design is documented in the brief (v5.2). We are building the schematic incrementally in KiCad, learning the tool as we go.

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
- [x] **PWR_FLAG** — placed on +BATT net and GND net

### Components placed and labelled

| Ref | Symbol used | Value | Notes |
|-----|-------------|-------|-------|
| BT1 | Device:Battery | Zeee 2S 2200mAh 7.4V | Positive pin wired to +BATT net, negative to GND |
| J1 | Connector:Conn_01x02 | T-Connector Deans 14AWG | Pin 1 on +BATT net, Pin 2 on GND |
| U1 | Connector:Conn_01x04 | JZK 2S 7.4V 8A BMS | 4-pin generic module |
| SW1 | SW_Push_Latching | Latching Push Button | Pin 1 on +PROT net, Pin 2 on +SWITCHED net |
| U2 | Connector:Conn_01x04 | MP1584 Buck Converter 5V | Pin 1 +SWITCHED, Pin 2 GND, Pin 3 +5V, Pin 4 GND. Text note: set output to 5.0V before connecting load |
| U3 | Connector:Conn_01x04 | ESP32-S3 Super Mini | Pin 1 +5V, Pin 2 GND, Pin 3 I2C_SDA, Pin 4 I2C_SCL |
| U4 | Connector:Conn_01x04 | PCA9685 16-ch PWM Driver | Pin 1 +5V, Pin 2 GND, Pin 3 I2C_SDA, Pin 4 I2C_SCL |
| D1 | Device:LED | Evan Designs Tank Fire LED Kit 5V | Direct 5V bus, anode on +5V net, cathode on GND |

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
- U1 pin 3 → +PROT net label
- U1 pin 4 → GND (via net label)
- SW1 pin 1 → +PROT net (connects to U1 pin 3)
- SW1 pin 2 → +SWITCHED net
- U2 pin 1 → +SWITCHED net, pin 2 → GND, pin 3 → +5V, pin 4 → GND
- U3 pin 1 → +5V, pin 2 → GND, pin 3 → I2C_SDA, pin 4 → I2C_SCL
- U4 pin 1 → +5V, pin 2 → GND, pin 3 → I2C_SDA, pin 4 → I2C_SCL (I2C nets auto-connect U3 and U4)
- D1 anode → +5V, cathode → GND

### Power chain (complete)

```
BT1 (LiPo) → J1 (T-connector) → U1 (BMS) → SW1 (switch) → U2 (buck) → +5V bus
                                                                              ↓
                                                              U3 (ESP32) + U4 (PCA9685) + D1 (tank fire)
```

### ERC status
Run after completing core power system. Results:
- **Zero errors**
- **Zero warnings**
- Ignored tests present — expected, KiCad defaults, no real errors suppressed

---

## Up Next — Zone loads (blaster channels)

The core power sheet is complete. The next session starts the zone load work. The most complex zone is the blaster arm — five PCA9685 channels (5–9), each driving LED rope segments through an IRLML6344 N-channel MOSFET.

### Circuit pattern (repeated for each blaster channel)

```
+5V ──────────────────────── LED rope anode
                                    │
                              LED rope cathode
                                    │
                               6.8Ω 1W resistor
                                    │
                              MOSFET drain (IRLML6344)
                              MOSFET source ── GND
                              MOSFET gate ──── PCA9685 output pin
                                    │
                               10kΩ pull-down
                                    │
                                   GND
```

**Build one channel first, verify it, then copy it four times** for the remaining blaster channels.

### Blaster channels to build

| Channel | Zone | Ref (planned) |
|---------|------|---------------|
| 5 | Blaster barrels | Q1 |
| 6 | Heatsink rear (gaps 1–3) | Q2 |
| 7 | Heatsink mid-rear (gaps 4–5) | Q3 |
| 8 | Heatsink mid-front (gaps 6–8) | Q4 |
| 9 | Heatsink front (gaps 9–10) | Q5 |

Series resistor for all blaster channels: **6.8Ω 1W** (yields ~294mA at 5V with 3.0V LED rope Vf). Consider 8.2Ω for more thermal margin.

### After blaster channels — direct-drive head zones

Simpler circuits, no MOSFET needed for single-LED channels:

| Ch | Zone | Circuit |
|----|------|---------|
| 0 | Eyes (fiber optic) | 150Ω resistor, direct PCA9685 drive, ~20mA |
| 10 | Sensor eye (red) | 100Ω resistor, direct PCA9685 drive, ~20mA |
| 11 | Mouth | Evan Designs 5V chip LED, direct PCA9685 drive |
| 1–3 | Brain A/B/C | TBD — direct drive if 1 LED per channel; MOSFET if 2+ |
| 4 | Power fist knuckles | 100Ω/LED, direct or MOSFET TBD |

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

Tank fire LED kit (D1): direct 5V bus, no PCA9685 channel needed.

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
- **Tank fire LED kit:** Represented as D1 (Device:LED symbol), direct 5V bus
- **All LEDs:** Green throughout. Clear resin parts used for diffusion.
