# Reaver Titan Build

*Emperor's Children | Chaos Reaver | Slaanesh Daemon-Touched*

Documentation and KiCad schematic files for a custom LED lighting system installed in a Warhammer 40K Reaver Titan. Emperor's Children chaos build with a corrupted, fleshy Slaanesh daemon-touched aesthetic — black/pink armour split, silver trim, gold accents, all lighting green. The model stands with its right foot on a destroyed tank.

---

## What's in This Repo

| Directory / File | Contents |
|-----------------|----------|
| `docs/` | Design documentation — one file per zone plus build roadmap and schematic tracker |
| `kicad/` | KiCad 10 project files — schematic sheets, symbols, project config |
| `CHANGELOG.md` | Design history derived from brief revision logs |

### `docs/`

| File | Covers |
|------|--------|
| [README.md](docs/README.md) | Concept, system architecture, full PCA9685 channel map, links to all zone docs |
| [power.md](docs/power.md) | Power system (battery, BMS, buck converter, switch), control system (ESP32-S3, PCA9685, web UI), wire gauge, core BOM, prototyping plan |
| [blaster.md](docs/blaster.md) | Laser blaster arm — barrels, heatsink zones, firing sequence (idle/prime/charge/discharge), MOSFET drive circuit, blaster BOM |
| [head.md](docs/head.md) | Head zones — fiber optic eyes, brain (3-channel pulse), red sensor eye, mouth, startup sequence |
| [power_fist.md](docs/power_fist.md) | Left arm power fist — clear resin knuckles, channel 4 pulse effect |
| [torso.md](docs/torso.md) | Torso cavity dimensions, component fit, connector strategy, mounting tray design, tank fire LED kit and wire routing |
| [build_roadmap.md](docs/build_roadmap.md) | Three-phase build plan (breadboard → perfboard → custom PCB), torso PCB constraints, open items |
| [schematic_progress.md](docs/schematic_progress.md) | KiCad session-by-session tracker — what's been placed, wired, and verified; locked electrical decisions |

### `kicad/`

KiCad 10 project files. The schematic is built in sheets:

| Sheet | Status |
|-------|--------|
| Sheet 1 — Core power system | Complete (rev v1.4) |
| Sheet 2 — Blaster zone (Ch 5–9) | Complete (rev v1.4) |
| Sheet 3 — Head zones (Ch 0–3, 10–11) | Not started |
| Sheet 4 — Power fist (Ch 4) | Not started |

See [docs/schematic_progress.md](docs/schematic_progress.md) for current KiCad status and session notes.

---

## Electrical Overview

```
LiPo (7.4V) → BMS → latching switch → buck converter (5V) → common bus
                                                                   │
                                         ┌─────────────────────────┤
                                         │                         │
                                    ESP32-S3                   PCA9685
                                  (WiFi web UI)           (16-ch PWM driver)
                                         │                         │
                                     I2C bus ───────────────────→  │
                                                                    │
                                                       LED zones (direct or via MOSFET)
```

- **Battery:** Zeee 2S 2200mAh 7.4V LiPo, T-connector (Deans)
- **Protection:** JZK 2S 7.4V 8A BMS
- **Regulation:** MP1584 buck converter → 5V bus
- **Control:** ESP32-S3 Super Mini — dual-core, WiFi web server, I2C master
- **PWM:** SunFounder PCA9685, 16 channels, 12-bit resolution
- **High-current switching:** IRLML6344 N-channel MOSFETs (SOT-23) for LED rope channels
- **All LEDs:** Green. Clear resin parts used throughout for diffusion.

Full details → [docs/power.md](docs/power.md)

---

## Build Status

**Phase 1 — Breadboard prototype** · *Parts ordered, pending arrival*

- [ ] Parts arrived and verified against BOM
- [ ] Core power chain validated on breadboard
- [ ] One blaster MOSFET channel (Q1) verified
- [ ] Web UI controlling LED rope brightness from phone

See [docs/build_roadmap.md](docs/build_roadmap.md) for the full phase plan and open items.

---

## Tooling

- **commitlint + husky** — conventional commit linting on pre-commit
- **semantic-release** — changelog generation from commit history (changelog-only config; no version bumping or GitHub releases)