# Reaver Titan Build

*Emperor's Children | Chaos Reaver | Slaanesh Daemon-Touched*

Documentation and KiCad schematic files for a custom LED lighting system installed in a Warhammer 40K Reaver Titan. Emperor's Children chaos build with a corrupted, fleshy Slaanesh daemon-touched aesthetic — black/pink armour split, silver trim, gold accents, all lighting green. The model stands with its right foot on a destroyed tank.

---

## What's in This Repo

| Directory / File | Contents |
|-----------------|----------|
| `docs/` | Design documentation — one file per zone plus schematic tracker |
| `kicad/` | KiCad 10 project files — schematic sheets, symbols, project config |
| `CHANGELOG.md` | Design history derived from brief revision logs |

### `docs/`

| File | Covers |
|------|--------|
| [core-infrastructure.md](docs/core-infrastructure.md) | Power system, control system (ESP32-S3, PCA9685), power chain diagram, channel map, wire gauge, core BOM |
| [build-guide.md](docs/build-guide.md) | Core power-on test procedure and zone prototyping order |
| [blaster.md](docs/blaster.md) | Laser blaster arm — barrels, heatsink zones, firing sequence (idle/prime/charge/discharge), MOSFET drive circuit, blaster BOM |
| [head.md](docs/head.md) | Head zones — fiber optic eyes, brain (3-channel pulse), red sensor eye, mouth, startup sequence |
| [power-fist.md](docs/power-fist.md) | Left arm power fist — clear resin knuckles, channel 4 pulse effect |
| [torso.md](docs/torso.md) | Torso cavity dimensions, component fit, connector strategy, mounting tray design, tank fire LED kit and wire routing |
| [schematic-progress.md](docs/schematic-progress.md) | KiCad session-by-session tracker — what's been placed, wired, and verified; locked electrical decisions |

### `kicad/`

KiCad 10 project files. The schematic is built in sheets:

| Sheet | Status |
|-------|--------|
| Sheet 1 — Core power system | Complete (rev v1.4) |
| Sheet 2 — Blaster zone (Ch 5–9) | Complete (rev v1.4) |
| Sheet 3 — Head zones (Ch 0–3, 10–11) | Not started |
| Sheet 4 — Power fist (Ch 4) | Not started |

See [docs/schematic-progress.md](docs/schematic-progress.md) for current KiCad status and session notes.

---

## Build Progress

Open items, phase status, and upcoming work are tracked in the [Reaver Titan Build project](https://github.com/users/learnsometing/projects/2) on GitHub.

---

## Tooling

- **commitlint + husky** — conventional commit linting on pre-commit
- **changelogen** — changelog generation from commit history (changelog-only config; no version bumping or GitHub releases)