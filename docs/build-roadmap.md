# Build Roadmap

*Reaver Titan Lighting System — Emperor's Children Build*

**Brief ref:** Lighting System Design Brief v5.3  
**Schematic ref:** KiCad schematic rev v1.4 — core power sheet complete, blaster zone complete

---

## Overview

Three phases from breadboard prototype to a finished custom PCB housed inside the Reaver Titan torso cavity. This is a build process reference, not a design specification — design decisions live in the zone docs; schematic decisions live in the KiCad files and the schematic progress tracker.

Each phase produces a working deliverable. No phase is skipped — the breadboard validates the design, the perfboard proves the physical fit, and the PCB is the final production-quality result.

| Phase | Name | Output | Schematic ref |
|-------|------|--------|---------------|
| **1** | Breadboard prototype | Validated power chain + one working LED zone | v1.4 — core power sheet |
| **2** | Perfboard build | Working drop-in tray installed in torso | v1.4+ — all zone sheets complete |
| **3** | Custom PCB | Manufactured board replacing perfboard tray | KiCad PCB layout derived from final schematic |

---

## Phase 1 — Breadboard Prototype

### Goal

Verify every electrical decision in the schematic before any permanent build. Every component value, every net connection, and every firmware behaviour must be confirmed working on the breadboard before moving to Phase 2.

### What Is Being Validated

- Full power chain: battery → BMS → switch → buck converter → 5V rail
- Buck converter output set to exactly 5.0V before any load connected
- ESP32-S3 boots, USB serial output visible, WiFi access point appears
- PCA9685 responds to I2C commands from ESP32-S3
- One MOSFET channel (Q1) switches LED rope at correct brightness
- Web UI loads on phone and controls LED rope brightness in real time
- Gate pull-down resistor prevents LED flicker at power-on

### Passing Criteria

All items above verified. Buck converter output measured at 5.0V ±0.1V. LED rope brightness controllable from 0–100% via web UI without flicker or instability. No components running hot to the touch after 10 minutes of operation.

### Step-by-Step Session Plan

*To be planned in detail once parts arrive. Will follow the sequence in [power.md](power.md) → Core Power-On Test, expanded with explicit pass/fail criteria for each step.*

---

## Phase 2 — Perfboard Build

### Goal

Translate the validated breadboard circuit into a hand-soldered perfboard assembly that physically fits inside the torso cavity and drives the model during display. This is the working prototype — functional, maintainable, and replaceable if the PCB design requires iteration.

### Why Perfboard Before PCB

- Forces physical layout decisions before committing to copper — reveals fit problems early
- Gives a working display model while PCB design is in progress
- Provides a fallback if the PCB has a problem — the perfboard build continues to work
- Wire routing practice inside the torso informs PCB connector placement

### Key Decisions to Make During Perfboard Build

- Final component positions on the tray — which board goes where
- Wire routing paths to each zone exit (shoulders, neck, pelvis)
- Buck converter potentiometer access — pre-set before installation
- ESP32-S3 orientation — USB-C must face the top armour plate opening for firmware updates
- Confirm Dupont connector headers fit and mate cleanly at each zone exit

### Session Plan

*To be planned once Phase 1 is complete and all zone schematics are finalised.*

---

## Phase 3 — Custom PCB

### Goal

Replace the perfboard with a single professionally manufactured PCB that fits within the torso cavity, consolidates all electronics onto one board, and provides clean zone connector headers at the appropriate board edges.

### PCB Design Workflow in KiCad

1. Complete all schematic sheets and run a clean ERC
2. Assign footprints to every schematic symbol (Tools → Assign Footprints)
3. Open PCB editor, import netlist — all components appear as unplaced footprints
4. Set board outline to target dimensions (60×70mm within 63×78mm cavity)
5. Place components following the block layout below
6. Route traces — start with power rails, then signal traces
7. Run DRC (Design Rule Check) — verify fab requirements met
8. Generate Gerber files and order from JLCPCB or OSHPark

### PCB Block Layout

| Block | Components | Location | Notes |
|-------|------------|----------|-------|
| **Power input** | T-connector pads, BMS header | Board edge — pelvis side | 7.4V side. Short runs, wide traces. |
| **Switch header** | 2-pin header for SW1 leads | Board edge | SW1 is off-board, wired back to header. |
| **Buck converter** | MP1584 module on pin header | Board interior | Module plugs in, replaceable. Upgrade to 5A for final build. |
| **ESP32-S3** | Castellated socket header | Board interior | Plugs in, replaceable. USB-C edge must be accessible. |
| **PCA9685** | TSSOP28 IC direct on board | Board interior | Schematic ref: U4 |
| **MOSFET drivers** | 5× IRLML6344 + 5× 6.8Ω 1W + 5× 10kΩ | Board interior | Schematic ref: Q1–Q5, R2/R4/R6/R8/R10, R1/R3/R5/R7/R9 |
| **Zone connectors** | 2-pin Dupont headers, one per zone exit | Board edges | Blaster (right shoulder), power fist (left shoulder), head (neck), tank fire (pelvis) |
| **Power rails** | Copper pours for +5V and GND | Full board | Pour width sized for 2A+ continuous. GND pour on both layers. |

### Fabrication

- **Target fab:** JLCPCB (low cost, fast turnaround, PCBA option for SOT-23 MOSFETs)
- **Board spec:** 2-layer, 1.6mm FR4, HASL finish, green soldermask
- **Minimum order:** 5 boards — keep spares
- **PCBA option:** Consider having JLCPCB place and solder the IRLML6344 MOSFETs (SOT-23 is fiddly to hand-solder)
- **Estimated cost:** ~$15–25 for 5 boards including shipping

### Session Plan

*To be planned once Phase 2 is complete and physical layout is confirmed from the perfboard build.*

---

## Torso Cavity Constraints

All constraints below must be satisfied simultaneously. The PCB layout must be planned with all of them in view before any component is placed. Full cavity dimensions in [torso.md](torso.md).

| Constraint | Dimension | Impact | Design response |
|------------|-----------|--------|----------------|
| Main cavity width | 63mm | Hard board width limit | Target PCB width ≤60mm to allow standoff clearance |
| Main cavity depth | 78mm | Hard board depth limit | Target PCB depth ≤70mm |
| Notch width | 27mm centred | Additional overflow area | Use for buck converter or zone connector headers that overflow main area |
| Usable height | 25mm (front-left) / 45mm (main) | PCB + components must fit under battery | Keep all tall components (caps, module headers) in the 45mm zone |
| Battery above PCB | 18.5mm + clearance | ~20–25mm PCB stack budget | Low-profile components required |
| Shoulder exits | 10mm dia, 30mm above floor | Zone connectors must face these holes | Blaster header on right edge, power fist header on left edge |
| Neck exit | Front-right floor | Head loom connector placement | Head zone header at front-right corner of PCB |
| Pelvis exit | Centre floor | Tank fire connector placement | Tank fire header at front-centre or routed via short lead |
| ESP32-S3 USB-C | Must be reachable | Firmware update access without board removal | Orient ESP32-S3 so USB-C faces the removable top armour plate opening |
| Buck converter pot | Must be pre-set to 5.0V | No access needed after installation | Set and verify on bench before installing PCB in torso |

**Target PCB size:** 60×70mm. All tall components in the 45mm-height zone (right of the front-left step). Zone connectors face their respective wire exits. ESP32-S3 USB-C faces upward. Buck converter potentiometer pre-set on bench.

---

## Open Items

### Before Phase 1

- Parts arrive — verify all components against BOM (see [power.md](power.md) and [blaster.md](blaster.md))
- Measure actual LED rope Vf under load — confirm 6.8Ω or adjust to 8.2Ω

### Before Phase 2

- Complete all zone schematic sheets (head, power fist, remaining channels)
- Measure LED rope lengths against printed barrel and heatsink parts — design inserts around fixed lengths
- Design torso mounting tray — incorporate 7mm switch hole and indicator LED dome
- Decide brain channel LED count per channel — determines MOSFET requirement (see [head.md](head.md))

### Before Phase 3

- Perfboard build complete and working in model
- All component footprints identified and available in KiCad libraries
- Decide whether to place MOSFET soldering in-house or use JLCPCB PCBA
- Confirm buck converter upgrade to 5A module if total draw exceeds 2.5A