# Reaver Titan — Build Roadmap

*Emperor's Children | Chaos Reaver | Slaanesh Daemon-Touched*

| | |
|---|---|
| **Version** | 1.0 — Initial roadmap |
| **Status** | Phase 1 pending — parts ordered |
| **Date** | April 2026 |
| **Brief ref** | Reaver Titan Lighting System Design Brief v5.3 |
| **Schematic ref** | KiCad schematic rev v1.4 — core power sheet complete, blaster zone complete |

---

## 1. Overview

This document describes the three-phase journey from breadboard prototype to a finished custom PCB housed inside the Reaver Titan torso cavity. It is a build process reference, not a design specification — design decisions and component specs live in the lighting brief. Schematic decisions live in the KiCad schematic and its progress tracker.

Each phase produces a working deliverable. No phase is skipped — the breadboard validates the design, the perfboard proves the physical fit, and the PCB is the final production-quality result.

| Phase | Name | Output | Schematic ref |
|---|---|---|---|
| **1** | Breadboard prototype | Validated power chain + one working LED zone | Schematic rev v1.4 — core power sheet |
| **2** | Perfboard build | Working drop-in tray installed in torso | Schematic rev v1.4+ — all zone sheets complete |
| **3** | Custom PCB | Manufactured board replacing perfboard tray | KiCad PCB layout — derived from final schematic |

---

## 2. Phase 1 — Breadboard Prototype

### Goal

Verify every electrical decision in the schematic before any permanent build. Every component value, every net connection, and every firmware behaviour must be confirmed working on the breadboard before moving to Phase 2.

### Schematic reference

- Ref: KiCad schematic rev v1.4 — core power sheet (BT1, J1, U1, SW1, U2, U3, U4, D1)
- Ref: Blaster zone: Q1–Q5, R1–R10 (channels 5–9)

### What is being validated

- Full power chain: battery → BMS → switch → buck converter → 5V rail
- Buck converter output set to exactly 5.0V before any load connected
- ESP32-S3 boots, USB serial output visible, WiFi access point appears
- PCA9685 responds to I2C commands from ESP32-S3
- One MOSFET channel (Q1) switches LED rope at correct brightness
- Web UI loads on phone and controls LED rope brightness in real time
- Gate pull-down resistor prevents LED flicker at power-on

### Passing criteria

All items above verified. Buck converter output measured at 5.0V ±0.1V. LED rope brightness controllable from 0–100% via web UI without flicker or instability. No components running hot to the touch after 10 minutes of operation.

### Step-by-step session plan

*To be planned in detail in a dedicated session once parts arrive. Will follow the sequence in Brief v5.3 Section 12.1, expanded with explicit pass/fail criteria for each step.*

---

## 3. Phase 2 — Perfboard Build

### Goal

Translate the validated breadboard circuit into a hand-soldered perfboard assembly that physically fits inside the torso cavity and drives the model during display. This is the working prototype — functional, maintainable, and replaceable if the PCB design requires iteration.

### Schematic reference

- Ref: KiCad schematic — final revision at time of perfboard build (all zone sheets complete)
- Ref: Lighting brief v5.3 Section 9: Torso Cavity and Component Layout
- Ref: Lighting brief v5.3 Section 9.3: Prototype Mounting Tray

### Why perfboard before PCB

- Forces physical layout decisions before committing to copper — reveals fit problems early
- Gives a working display model while PCB design is in progress
- Provides a fallback if the PCB has a problem — the perfboard build continues to work
- Wire routing practice inside the torso informs PCB connector placement

### Key decisions to make during perfboard build

- Final component positions on the tray — which board goes where
- Wire routing paths to each zone exit (shoulders, neck, pelvis)
- Buck converter potentiometer access — pre-set before installation
- ESP32-S3 orientation — USB-C must face the top armor plate opening for firmware updates
- Confirm Dupont connector headers fit and mate cleanly at each zone exit

### Session plan

*To be planned once Phase 1 is complete and all zone schematics are finalised.*

---

## 4. Phase 3 — Custom PCB

### Goal

Replace the perfboard with a single professionally manufactured PCB that fits within the torso cavity, consolidates all electronics onto one board, and provides clean zone connector headers at the appropriate board edges.

### Schematic reference

- Ref: KiCad schematic — final revision (all zones complete, footprints assigned)
- Ref: KiCad PCB layout file — derived from schematic via netlist

### PCB design workflow in KiCad

1. Complete all schematic sheets and run a clean ERC
2. Assign footprints to every schematic symbol (Tools → Assign Footprints)
3. Open PCB editor, import netlist — all components appear as unplaced footprints
4. Set board outline to target dimensions (60×70mm within 63×78mm cavity)
5. Place components — follow the block layout table below
6. Route traces — start with power rails, then signal traces
7. Run DRC (Design Rule Check) — verify fab requirements met
8. Generate Gerber files and order from JLCPCB or OSHPark

### PCB block layout

Component placement follows the torso wire exit positions. Connectors face their respective exits; power input is at the pelvis-side edge; USB-C access faces upward toward the removable top armor plate.

| Block | Components | Location | Notes |
|---|---|---|---|
| **Power input** | T-connector pads, BMS header | Board edge — pelvis side | 7.4V side. Short runs, wide traces |
| **Switch header** | 2-pin header for SW1 leads | Board edge | SW1 is off-board, wired back to header |
| **Buck converter** | MP1584 module on pin header | Board interior | Module plugs in, replaceable. Upgrade to 5A for final build |
| **ESP32-S3** | Castellated socket header | Board interior | Plugs in, replaceable. USB-C edge must be accessible |
| **PCA9685** | TSSOP28 IC direct on board | Board interior | Schematic ref: U4 (Driver_LED:PCA9685PW) |
| **MOSFET drivers** | 5x IRLML6344 + 5x 6.8Ω 1W + 5x 10kΩ | Board interior | Schematic ref: Q1–Q5, R2/R4/R6/R8/R10, R1/R3/R5/R7/R9 |
| **Zone connectors** | 2-pin Dupont headers, one per zone exit | Board edges — toward wire exits | Blaster (right shoulder), power fist (left shoulder), head (neck), tank fire (pelvis) |
| **Power rails** | Copper pours for +5V and GND | Full board | Pour width sized for 2A+ continuous. GND pour on both layers |

### Fabrication

- Target fab: JLCPCB (low cost, fast turnaround, PCBA option for SOT-23 MOSFETs)
- Board spec: 2-layer, 1.6mm FR4, HASL finish, green soldermask
- Minimum order: 5 boards — keep spares
- PCBA option: consider having JLCPCB place and solder the IRLML6344 MOSFETs (SOT-23 is fiddly to hand-solder)
- Estimated cost: approximately $15–25 for 5 boards including shipping

### PCB design session plan

*To be planned once Phase 2 is complete and physical layout is confirmed from the perfboard build. Will cover footprint assignment, board outline, component placement, trace routing, and DRC in KiCad.*

---

## 5. Torso Cavity Constraints

**Ref: Lighting brief v5.3 Section 9: Torso Cavity and Component Layout**

The torso cavity sets hard physical limits on the PCB. All constraints below must be satisfied simultaneously. The PCB layout must be planned with all of them in view before any component is placed.

| Constraint | Dimension | Impact | Design response |
|---|---|---|---|
| **Main cavity width** | 63mm | Hard board width limit | Target PCB width ≤60mm to allow standoff clearance |
| **Main cavity depth** | 78mm | Hard board depth limit | Target PCB depth ≤70mm |
| **Notch width** | 27mm centered | Additional overflow area | Use for buck converter or zone connector headers that overflow main area |
| **Usable height** | 25mm (front-left) / 45mm (main) | PCB + components must fit under battery | Keep all tall components (caps, module headers) in the 45mm zone |
| **Battery above PCB** | 18.5mm + clearance | ~20–25mm PCB stack budget | Low-profile components required. Module headers set max height |
| **Shoulder exits** | 10mm dia, 30mm above floor | Zone connectors must face these holes | Blaster header on right edge, power fist header on left edge |
| **Neck exit** | Front-right floor | Head loom connector placement | Head zone header at front-right corner of PCB |
| **Pelvis exit** | Center floor | Tank fire connector placement | Tank fire header at front-center or routed via short lead |
| **ESP32-S3 USB-C** | Must be reachable | Firmware update access without board removal | Orient ESP32-S3 so USB-C faces the removable top armor plate opening |
| **Buck converter pot** | Must be pre-set to 5.0V | No access needed after installation | Set and verify on bench before installing PCB in torso |

### Summary

Target PCB size is 60×70mm. All tall components stay in the 45mm-height zone (right of the front-left step). Zone connectors face their respective wire exits. ESP32-S3 USB-C faces upward. Buck converter potentiometer is pre-set on the bench — no post-installation access assumed. The 27mm notch is available for overflow components if the main area is tight.

---

## 6. Open Items

The following items must be resolved before or during the relevant phase.

### Before Phase 1

- Parts arrive — verify all components against BOM *(Brief v5.3 Section 11, 11.2)*
- Measure actual LED rope forward voltage under load — confirm 6.8Ω or adjust to 8.2Ω *(Brief v5.3 Section 5.4)*

### Before Phase 2

- Complete all zone schematic sheets (head, power fist, remaining channels) *(KiCad schematic — deferred zones)*
- Measure LED rope lengths against printed barrel and heatsink parts — design inserts around fixed lengths *(Brief v5.3 Section 5.1)*
- Design torso mounting tray in slicer — incorporate 7mm switch hole and indicator LED dome *(Brief v5.3 Section 9.3)*
- Decide brain channel LED count per channel — determines MOSFET requirement *(Brief v5.3 Section 6.4)*

### Before Phase 3

- Perfboard build complete and working in model
- All component footprints identified and available in KiCad libraries
- Decide whether to place MOSFET soldering in-house or use JLCPCB PCBA
- Confirm buck converter upgrade to 5A module if total draw exceeds 2.5A *(Brief v5.3 Section 2)*

---

## Revision Log

| Version | Date | Changes |
|---|---|---|
| 1.0 | Apr 2026 | Initial roadmap. Three-phase structure defined (breadboard → perfboard → PCB). Torso constraint table captured. PCB block layout table captured. Phase 1 and Phase 2 session plans flagged as placeholders pending parts arrival and zone schematic completion |

---

*Emperor's Children — Slaanesh Daemon-Touched Build*