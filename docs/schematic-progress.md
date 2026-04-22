# Schematic Progress Tracker

*Reaver Titan Lighting System — Emperor's Children Build*

**KiCad version:** 10.0 (Windows)  
**Schematic revision:** v1.5  
**Brief ref:** v5.3

This is a working document. It tracks what has been placed, wired, and verified in KiCad — session by session. It is not a design reference; design decisions live in the zone docs.

---

## KiCad Quick Reference

| Key | Action |
|-----|--------|
| `A` | Open symbol chooser (Add symbol) |
| `W` | Draw wire |
| `L` | Place net label |
| `G` | Grab and move a label/wire without disconnecting it |
| Scroll wheel | Zoom in/out |

- Net labels connect pins automatically when two labels share the same name — no wire needed across long distances
- A small green dot at a junction = connected. No dot = not connected.
- **Inspect → Electrical Rules Checker** — run ERC to verify connections
- Zoom in when placing labels to ensure they snap to pin endpoints

---

## `core-power.kicad_sch` — Core Power System

**Status: Complete. ERC: zero errors, zero warnings.**

### Power Symbols

- [x] `+BATT` power symbol — top left
- [x] `GND` power symbol
- [x] `PWR_FLAG` — on `+BATT` net and `GND` net

### Components

| Ref | Symbol | Value | Notes |
|-----|--------|-------|-------|
| BT1 | Device:Battery | Zeee 2S 2200mAh 7.4V | Positive → +BATT, negative → GND |
| J1 | Connector:Conn_01x02 | T-Connector Deans 14AWG | Pin 1 → +BATT, Pin 2 → GND |
| SW1 | SW_Push_Latching | Latching Push Button | Pin 1 → +BATT, Pin 2 → +SWITCHED |
| U2 | Connector:Conn_01x04 | MP1584 Buck Converter 5V | Pin 1 +SWITCHED, Pin 2 GND, Pin 3 +5V, Pin 4 GND |
| U3 | Connector:Conn_01x04 | ESP32-S3 Super Mini | Pin 1 +5V, Pin 2 GND, Pin 3 I2C_SDA, Pin 4 I2C_SCL |
| U4 | Connector:Conn_01x04 | PCA9685 16-ch PWM Driver | Pin 1 +5V, Pin 2 GND, Pin 3 I2C_SDA, Pin 4 I2C_SCL |
| D1 | Device:LED | Evan Designs Tank Fire LED Kit 5V | Direct 5V bus, anode → +5V, cathode → GND |

### Power Chain (Complete)

```
BT1 → J1 → SW1 → U2 (buck) → +5V bus
                                   │
                  U3 (ESP32-S3) + U4 (PCA9685) + D1 (tank fire)
```

---

## `blaster-zones.kicad_sch` — Blaster Zone (Channels 5–9)

**Status: Complete. ERC: zero errors, zero warnings.**

Five MOSFET channels, one per blaster zone. Circuit pattern repeated × 5:

```
+5V ──── LED rope anode
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

### Components Placed

| Ref | Zone | PCA9685 Ch |
|-----|------|------------|
| Q1 | Blaster barrels | 5 |
| Q2 | Heatsink rear (gaps 1–3) | 6 |
| Q3 | Heatsink mid-rear (gaps 4–5) | 7 |
| Q4 | Heatsink mid-front (gaps 6–8) | 8 |
| Q5 | Heatsink front (gaps 9–10) | 9 |

Series resistors: R2, R4, R6, R8, R10 (6.8Ω 1W each)  
Gate pull-downs: R1, R3, R5, R7, R9 (10kΩ ¼W each)

---

## Up Next — Direct-Drive Head Zones

Simpler circuits, no MOSFET needed for single-LED channels:

| Ch | Zone | Circuit |
|----|------|---------|
| 0 | Eyes (fiber optic) | 150Ω resistor, direct PCA9685 drive, ~20mA |
| 10 | Sensor eye (red) | 100Ω resistor, direct PCA9685 drive, ~20mA |
| 11 | Mouth | Evan Designs 5V chip LED, direct PCA9685 drive |
| 1–3 | Brain A/B/C | TBD — direct drive if 1 LED per channel; MOSFET if 2+ |
| 4 | Power fist knuckles | 100Ω/LED, direct or MOSFET TBD |

---

## Full Channel Map Reference

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

---

## Locked Decisions

| Decision | Value |
|----------|-------|
| Battery | Zeee 2S 2200mAh 7.4V 50C Shorty LiPo, T-connector (Deans), 2-pack |
| Connector | T-connector (Deans) — replaces JST |
| Buck converter | MP1584 (3A) for prototype; upgrade to 5A module for final build |
| MCU | ESP32-S3 Super Mini — WiFi web UI, dual-core, I2C to PCA9685 |
| PWM driver | SunFounder PCA9685, 16-channel, 12-bit, I2C |
| Wire gauge | 14 AWG silicone on 7.4V side; 22 AWG stranded on 5V side |
| MOSFET | IRLML6344 (SOT-23) for all high-current LED rope channels |
| Power switch | Latching push button (SW1), torso exterior housing |
| Tank fire | D1 (Device:LED), direct 5V bus |
| All LEDs | Green throughout. Clear resin parts for diffusion. |