# Changelog

All notable changes to this project will be documented in this file. See [Conventional Commits](https://conventionalcommits.org) for commit guidelines.

---

*Versions 1.0.0 through 2.1.0 are derived from the design document revision log, predating git history. Git history begins at initial commit, April 2026.*

---

## [2.1.0] - April 2026

### Added
- Power switch specified: Twidec L-PBS-110-XBK 7mm latching push button, mounted through existing torso exterior rectangular housing feature
- Indicator LED planned for domed feature above switch, wired to +SWITCHED net
- Blaster zone BOM added (Section 11.2) — MOSFETs, series resistors, gate pull-down resistors
- Resistor assortment (BOJACK B08FD1XVL6, 25 values 1/4W) added to core BOM

### Changed
- LED rope confirmed on hand: 1.5mm overall diameter with green silicone diffusion sleeve, Vf 2.7–3.1V, max 300mA, two fixed-length ropes from same supplier. Cannot be cut — insert designs will be modelled around rope lengths
- LED rope specs updated throughout (blaster barrels, heatsink, electrical notes)
- Status updated to prototyping phase

---

## [2.0.0] - April 2026

### Changed
- **Breaking:** Replaced Arduino Nano with ESP32-S3 Super Mini (22.52×18mm, dual-core 240MHz, WiFi, BLE). Fundamental architecture change — eliminates need for separate RF module or IR receiver
- Remote control now via web browser — no app needed. Web UI provides zone toggles, brightness sliders, firing triggers, and display mode presets
- Physical switch removed from architecture; battery always unplugged at T-connector when not in use
- Blaster trigger method updated to web interface
- Runtime estimates updated to reflect higher ESP32 power draw

### Added
- Remote control section: WiFi access point, web UI, dual-core architecture rationale
- OTA firmware update and multi-model sync noted as future expansion possibilities

---

## [1.3.1] - April 2026

### Fixed
- Corrected torso cavity side profile: front-left corner is a stepped cutout (wall rises 25mm, steps inward 20mm, then rises 20mm to ceiling), not a downward shelf
- Usable height clarified: 25mm in cutout zone, full 45mm elsewhere. Battery must sit in full-height zone

### Added
- Central junction section: perfboard with 5V/GND bus and zone pin headers
- Core power system BOM with all components needed to prototype the drop-in tray

---

## [1.3.0] - April 2026

### Added
- Torso cavity dimensions documented: 63×78mm main area, 27mm centered notch, 45mm height with stepped shelf
- Magnetic removable top armor plate for battery and maintenance access
- Wire exits: 10mm shoulder holes at 30mm height on both sides, pelvis hole center floor, neck hole front-right floor
- 3D-printed drop-in mounting tray concept for prototype phase
- Component fit analysis confirms all boards plus battery within 45mm height ceiling

### Changed
- Buck converter spec updated: recommend 5A-rated module for expansion headroom (MP1584 acceptable for prototyping only)

---

## [1.2.2] - April 2026

### Added
- Power system fully specified: Zeee 2S 2200mAh 50C Shorty LiPo (73×34×18.5mm, T-connector, 2-pack)
- BMS specified: JZK 2S 7.4V 8A (41×16×3.5mm, over-charge/discharge/short circuit protection)
- Runtime estimates added
- Charger requirement documented

### Changed
- Battery connector changed from JST to T-connector (Deans) — resolves previous current rating concern
- Firing sequence revised: energy now originates at rear reactor/heatsink and propagates forward, rear zone never fully dark

---

## [1.2.1] - April 2026

### Added
- Tank fire wire routing plan: tank → foot → leg channel → pelvis → torso
- Fire kit repurchased as 5–12V version with 14-inch wire
- Right leg to be reprinted with hollow wire channel (5–6mm ID, oversized for future use). Left leg solid
- Connector strategy: Dupont connectors at all major body joints (pelvis, shoulders, neck) for modular assembly
- PCB consolidation roadmap

### Changed
- Tank stands as base for right foot — no separate model base

---

## [1.2.0] - April 2026

### Added
- Eyes redesigned to fiber optic: 4 strands from 1x 5mm green LED, 150Ω, PCA9685 channel 0, warm-up ramp on power-on
- Red sensor eye: 3V Evan Designs chip LED with 100Ω inline resistor, PCA9685 channel 10, periodic blink effect
- Mouth zone: 5V green Evan Designs chip LED, PCA9685 channel 11, organic breathing rhythm loosely related to brain tempo
- Head startup sequence defined: eyes → brain → mouth
- Electrical notes expanded with per-zone LED specs
- 12 of 16 PCA9685 channels now allocated

### Changed
- Complete head redesign
- Brain LED count left as open item pending print design and diffusion prototyping

---

## [1.1.1] - April 2026

### Added
- MOSFET driver circuit design: IRLML6344 for all 5 blaster channels
- Series resistor values defined: 6.8Ω (max brightness) and 8.2Ω (derated) options

### Changed
- LED rope electrical specs confirmed: Vf 2.7–3.0V, max 300mA

### Fixed
- JST connector current rating concern flagged — potential XT30 upgrade noted

---

## [1.1.0] - April 2026

### Added
- PCA9685 16-channel 12-bit PWM driver via I2C
- Blaster expanded to 5 channels: barrels + 4 heatsink zones
- Multi-state firing sequence defined: idle / prime / full charge / discharge
- LED rope specs and heatsink zone grouping documented
- Prototyping plan added

### Changed
- Replaced Arduino Uno with Arduino Nano

---

## [1.0.0] - April 2026

### Added
- Initial design brief
- Arduino Uno controller
- 6 native PWM channels
- Blaster as single static glow zone