# Torso & Connector Strategy

*Reaver Titan Lighting System — Emperor's Children Build*

---

## Torso Lighting Zones

| Ch | Zone | LED | Effect | Notes |
|----|------|-----|--------|-------|
| 14 | Power indicators | 2× 5mm domed purple through-hole | Boot ramp, then static. Colour reflects display mode state. | One per side of torso, both driven from same channel |
| 15 | Rear exhaust vents | Orange LED rope (1.5mm silicone-sleeved) | Faint baseline idle glow at all times. Spikes on weapon discharge, fades back to baseline over a few seconds. | Passive heat — reactor always running |

### Power Indicators

Two indicator lights, one on each side of the torso, housed in dome-shaped features on the model. Both driven from Ch 14 — always show the same colour simultaneously. Purple chosen for Slaanesh aesthetic and contrast against the all-green weapon/head lighting.

Display mode state is reflected via the web UI — colour behaviour per mode to be defined during firmware development. Purple is the current colour choice; final selection will be confirmed once the model is assembled and the paint scheme can be evaluated alongside the lighting.

**Electrical:** 2× 5mm domed purple through-hole LEDs wired in parallel on Ch 14. Series resistor TBD based on LED forward voltage. Direct PCA9685 drive, no MOSFET needed.

### Rear Exhaust Vents

Two large vent groupings on the rear of the torso, approximately 6 vent openings total, tightly spaced. A single orange LED rope is bent into rows behind the vent grilles to create even coverage across both sides. The rope insert is designed in slicer software around the rope's bend radius — the same approach used for blaster barrel and heatsink inserts.

The vent piece is a separate printed assembly from the main torso shell. A Dupont connector at the vent-to-torso joint allows the vent piece to be bench tested independently and removed if needed — consistent with the connector strategy used at all other major assembly joints. A small channel or recess at the joint edge accommodates the wire tails and connector.

**Electrical:** Single orange LED rope segment, Ch 15, MOSFET driven (IRLML6344, same circuit as arm pool). Series resistor TBD pending LED rope Vf measurement.

**Effect detail:**
- **Idle:** Faint baseline glow at all times — the reactor is always running
- **Weapon discharge:** Brightness spikes after a firing event, then fades back to baseline over a few seconds. A short delay before the brightness peak (after the blaster discharge completes) suggests heat working its way back through the system from the weapon into the power core.
- **Boot sequence:** Vents ramp up to baseline glow as part of the startup sequence, after the head comes online — suggesting the reactor reaching operating temperature.

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
| **Right shoulder** | 10mm hole, ~30mm above cavity floor | Right arm weapon loom (Ch 6–11, 6 MOSFET-driven channels + ID pin) |
| **Left shoulder** | 10mm hole, ~30mm above cavity floor | Left arm weapon loom (Ch 0–5, 6 MOSFET-driven channels + ID pin) |
| **Pelvis** | Hole, centre of cavity floor | Tank fire kit wiring down through right leg |
| **Neck** | Hole, front-right of cavity floor | Head assembly loom (Ch 12–13) |
| **Rear** | TBD — vent piece joint | Exhaust vent rope (Ch 15) via Dupont connector at vent-to-torso joint |

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
- 12 MOSFET driver circuits for arm pool channels (Ch 0–11)
- MOSFET driver circuit for exhaust vent channel (Ch 15)

Zone headers face outward toward their respective wire exits. The T-connector from the battery connects to the switch → buck converter → junction board.

---

## Connector Strategy

All wiring between major body sections uses Dupont connectors at the structural joints, making the model fully modular — any body section can be disconnected without soldering.

| Connection point | Connector | Zones |
|-----------------|-----------|-------|
| Right shoulder | 9-pin Dupont | Ch 6–11 (6 signal lines) + 5V + GND + ID pin |
| Left shoulder | 9-pin Dupont | Ch 0–5 (6 signal lines) + 5V + GND + ID pin |
| Neck | 4-pin Dupont (minimum) | Ch 12–13 (head loom) |
| Pelvis | 2-pin Dupont | Tank fire kit 5V/GND pair |
| Rear vent joint | 2-pin Dupont | Ch 15 exhaust vent rope |

See [arms.md](arms.md) for full shoulder connector pinout, weapon ID detection, and arm pool channel allocation.

**Design principle:** No permanent solder joints exist between major assemblies. All soldering is internal to each body section. The connection between sections is always a plug-in Dupont connector.

### Future PCB Consolidation

When the design matures to a custom PCB, the torso PCB will include pin headers along its edges — one header per body section. Each body section's Dupont connector plugs directly into its corresponding header. The PCB consolidates the ESP32-S3 socket, PCA9685 IC, MOSFET circuits, series resistors, and power distribution onto a single board. Direct-bus zones like the tank fire kit bypass the PCA9685 and connect straight to 5V/GND pins on the board.

See [build_roadmap.md](build_roadmap.md) for the full Phase 3 PCB design plan.

---

## Tank Fire LED Kit

**Product:** Evan Designs 5–12V version, 14-inch wire. Self-contained fire effect kit with 3 LEDs (1 flashing red, 1 flashing orange, 1 solid orange). Connects directly to the 5V/GND bus — no PCA9685 channel needed. Estimated draw ~60mA.

**Placement:** Fire light emanates from inside the destroyed tank beneath the Titan's right foot, bleeding out through plasma holes in the tank side and the rear boarding dock. LEDs are permanently mounted inside the tank cavity. The tank is glued to the right foot; the model stands on a table without a separate base.

### Wire Routing

Three discrete segments connected by Dupont plugs:

1. **Tank segment** — LEDs soldered inside the tank, wire tails terminating in a Dupont connector inside the tank cavity or just above it inside the foot.
2. **Leg harness** — passive wire run with Dupont connectors on both ends, pre-threaded through the leg channel during assembly before gluing. Stays in the leg permanently, contains no components.
3. **Torso segment** — short wire from the pelvis-side Dupont connector into the torso, connecting to the 5V/GND bus.

> The fire kit can be bench-tested independently by plugging the tank segment directly into the 5V bus before committing to leg assembly.

### Leg Print Notes

- Right leg reprinted with hollow wire channel, minimum 5–6mm internal diameter
- Open access at both ends (ankle/foot and pelvis/hip)
- Leg harness pre-threaded during assembly before gluing, remains permanently installed
- Left leg printed solid for maximum structural strength

---

## Open Items

- Speaker/audio circuit for missile launcher — connects directly to ESP32-S3 via I2S, separate from PCA9685. Missile launcher is a removable piece — wire routing and Dupont connector strategy TBD. Defer to a future phase.
- Power indicator resistor value (Ch 14) — pending purple LED forward voltage spec
- Display mode colour behaviour for power indicators — to be defined during firmware development
- Series resistor for exhaust vent rope (Ch 15) — pending LED rope Vf measurement