# Torso & Connector Strategy

*Reaver Titan Lighting System — Emperor's Children Build*

---

## Torso Cavity Dimensions

### Top-Down Profile

Main rectangular area: **63mm wide × 78mm deep**. A centred notch extends from one side, 27mm wide, bringing the total width to 110mm at that point. The overall footprint is a T-shape when viewed from above.

### Side Profile

78mm deep × 45mm tall. The front-left corner has a stepped cutout: the wall rises 25mm from the floor, steps inward 20mm, then rises another 20mm to the ceiling. This reduces usable height to **25mm** in the front-left zone. The full **45mm** height is available from the step inward to the back wall.

> **Key constraint:** The battery must be positioned in the full-height zone (right of the step). Avoid placing components in the cutout corner — the angle is not a clean 90 degrees.

---

## Access and Wire Exits

| Exit | Location | Purpose |
|------|----------|---------|
| **Top access** | Removable magnetic armour plate | Battery swaps and maintenance |
| **Right shoulder** | 10mm hole, ~30mm above cavity floor | Blaster arm wiring loom (5 MOSFET-driven channels) |
| **Left shoulder** | 10mm hole, ~30mm above cavity floor | Power fist wiring (1 PCA9685 channel) |
| **Pelvis** | Hole, centre of cavity floor | Tank fire kit wiring down through right leg |
| **Neck** | Hole, front-right of cavity floor | Head assembly wiring loom (eyes, sensor, mouth, brain) |

Floor openings can be repositioned if the component layout requires it.

---

## Component Fit Summary

| Component | Dimensions (mm) | Placement | Notes |
|-----------|----------------|-----------|-------|
| Battery | 73 × 34 × 18.5 | Top of stack | Sits on tray, lifts out from above |
| ESP32-S3 Super Mini | 22.5 × 18 | Tray floor | Very compact, fits anywhere |
| PCA9685 | ~25 × 62 | Tray floor | Main rectangular area |
| Buck converter | ~22 × 17 (MP1584) | Tray floor | Size may increase if upgraded to 5A module |

Total tray footprint fits within the 63mm × 78mm main area with the 27mm notch available for overflow. Electronics on the tray floor (~20–25mm including boards and wiring clearance) plus the battery on top (18.5mm) totals ~40–43mm, leaving ~8mm clearance to the magnetic armour plate.

---

## Prototype Mounting Tray

A 3D-printed mounting tray holds all electronics (ESP32-S3, PCA9685, buck converter) in fixed positions. The tray drops into the cavity as a single unit with clip-in or friction-fit component slots and wire routing channels. Zone Dupont connector headers face outward toward their respective wire exits.

The battery sits on top of the tray, accessible immediately when the magnetic armour plate is removed.

**Service sequence:**
- Battery swap: lift armour plate → unplug T-connector → lift battery out
- Electronics service: unplug zone connectors → lift entire tray out as one unit

> The mounting tray is for the prototype phase. When the design matures to a custom PCB (see [build_roadmap.md](build_roadmap.md)), the PCB replaces the tray and mounts directly into the cavity.

### Central Junction

The tray includes a small perfboard that serves as the central power and signal junction:

- Common 5V rail (output from buck converter)
- Common GND bus
- Dupont pin headers for each zone connector
- MOSFET driver circuits for blaster channels (prototype phase)

Zone headers face outward toward their respective wire exits. The T-connector from the battery connects to the switch → buck converter → junction board.

---

## Connector Strategy

All wiring between major body sections uses Dupont connectors at the structural joints, making the model fully modular — any body section can be disconnected without soldering.

| Connection point | Zones |
|-----------------|-------|
| Right shoulder | Blaster arm wiring loom (5 MOSFET-driven PCA9685 channels) |
| Left shoulder | Power fist wiring (1 PCA9685 channel) |
| Neck | Head assembly loom (eyes, sensor, mouth, brain — up to 6 PCA9685 channels) |
| Pelvis | Tank fire kit 5V/GND pair |

**Design principle:** No permanent solder joints exist between major assemblies. All soldering is internal to each body section. The connection between sections is always a plug-in Dupont connector. This enables disassembly for transport, maintenance, and troubleshooting without risk of damaging components.

### Future PCB Consolidation

When the design matures to a custom PCB, the torso PCB will include pin headers along its edges — one header per body section. Each body section's Dupont connector plugs directly into its corresponding header. The PCB consolidates the ESP32-S3 socket, PCA9685 IC, MOSFET circuits, series resistors, and power distribution (5V bus, common GND) onto a single board. Direct-bus zones like the tank fire kit bypass the PCA9685 and connect straight to 5V/GND pins on the board.

See [build_roadmap.md](build_roadmap.md) for the full Phase 3 PCB design plan.

---

## Tank Fire LED Kit

**Product:** Evan Designs 5–12V version, 14-inch wire. Self-contained fire effect kit with 3 LEDs (1 flashing red, 1 flashing orange, 1 solid orange). Overlapping flicker rates produce a randomised fire effect with no coding required. Connects directly to the 5V/GND bus — no PCA9685 channel needed. Estimated draw ~60mA.

**Placement:** Fire light emanates from inside the destroyed tank beneath the Titan's right foot, bleeding out through plasma holes in the tank side and the rear boarding dock. LEDs are permanently mounted inside the tank cavity. The tank is glued to the right foot; the model stands on a table without a separate base.

### Wire Routing

The wiring runs as three discrete segments connected by Dupont plugs, eliminating the need to pull wires through the leg after assembly:

1. **Tank segment** — LEDs soldered inside the tank, short wire tails terminating in a Dupont connector inside the tank cavity or just above it inside the foot.

2. **Leg harness** — passive wire run with Dupont connectors on both ends, pre-threaded through the leg channel during assembly before gluing. Stays in the leg permanently, contains no components. Replaceable if ever damaged since it is not soldered at either end.

3. **Torso segment** — short wire from the pelvis-side Dupont connector into the torso, connecting to the 5V/GND bus.

> The pelvis junction has limited space and requires a sharp turn from the angled leg channel into the torso cavity. The Dupont connector at this end means the leg harness never needs to be pulled through this turn — it is pre-installed and simply plugs in. The fire kit can be bench-tested independently by plugging the tank segment directly into the 5V bus before committing to leg assembly.

### Leg Print Notes

The right leg must be reprinted with a hollow wire channel running its full length:

- Minimum 5–6mm internal diameter, oversized to allow future additional wiring through the same leg
- Follow a gentle path where possible; a sharp turn at the pelvis junction is unavoidable
- Open access at both ends (ankle/foot and pelvis/hip)
- The leg harness wire is threaded through the channel during assembly before the leg is glued to the pelvis, and remains permanently installed

The left leg does not require a wire channel and can be printed solid for maximum structural strength.