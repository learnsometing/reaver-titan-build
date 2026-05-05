# Arms — Modular Weapon System

*Reaver Titan Lighting System — Emperor's Children Build*

## Overview

Both arm sockets draw from a 12-channel arm pool (Ch 0–11), but the hardware is completely separate — each socket has its own dedicated 6 MOSFET circuits on the junction board. The pool concept exists only in firmware.

- Left socket → Ch 0–5 (dedicated circuits on junction board)
- Right socket → Ch 6–11 (dedicated circuits on junction board)

No channel is ever shared between sockets at the hardware level. The firmware thinks of all 12 channels as a single pool and allocates from it based on weapon identity detected at each socket on boot. A laser blaster profile is the same profile regardless of which socket it's plugged into — the firmware applies the correct channel offset (0–5 for left, 6–11 for right) automatically.

This means every weapon arm is physically identical and compatible with either socket. The junction board never changes. The firmware handles everything.

---

## Weapon Loadout

Weapons are interchangeable between arm sockets. Two of the same weapon can be run simultaneously.

| Weapon         | Channels Required | Status                       |
| -------------- | ----------------- | ---------------------------- |
| Laser blaster  | 6                 | Designed — see blaster.md    |
| Volcano cannon | TBD               | Not yet designed             |
| Melta cannon   | TBD               | Not yet designed             |
| Power fist     | 2                 | Designed — see power-fist.md |
| Chain fist     | TBD               | Not yet designed             |

---

## Shoulder Connector Standard

There are two independent shoulder sockets — one on each side of the torso. Both use an identical 9-pin Dupont connector, which means every weapon arm is physically compatible with either socket without modification.

Each socket has its own dedicated ID pin wired to a separate analog-capable GPIO on the ESP32-S3. The ESP32 reads both sockets independently and can detect a different weapon on each side simultaneously.

### Pinout (identical on both sockets)

| Pin | Signal | Notes                 |
| --- | ------ | --------------------- |
| 1   | +5V    | Power to weapon arm   |
| 2   | GND    | Common ground         |
| 3   | CH_A   | PCA9685 signal line 1 |
| 4   | CH_B   | PCA9685 signal line 2 |
| 5   | CH_C   | PCA9685 signal line 3 |
| 6   | CH_D   | PCA9685 signal line 4 |
| 7   | CH_E   | PCA9685 signal line 5 |
| 8   | CH_F   | PCA9685 signal line 6 |
| 9   | ID     | Weapon identity pin   |

### Signal Line to Pool Channel Mapping

| Socket | CH_A | CH_B | CH_C | CH_D | CH_E  | CH_F  |
| ------ | ---- | ---- | ---- | ---- | ----- | ----- |
| Left   | Ch 0 | Ch 1 | Ch 2 | Ch 3 | Ch 4  | Ch 5  |
| Right  | Ch 6 | Ch 7 | Ch 8 | Ch 9 | Ch 10 | Ch 11 |

### Drive Standard

All 12 arm pool channels are MOSFET-driven as standard, regardless of which weapon is mounted or how much current that weapon's LEDs actually draw. One IRLML6344 circuit per channel, 12 total on the torso junction board. No direct-drive exceptions in the arm pool.

### LED Standard

All weapons use the same 1.5mm silicone-sleeved LED rope (same supplier and spec as the blaster). Weapon cavities are modelled in slicer software to accept the rope — the hole is designed around the rope, not the other way around. This keeps the circuit identical across all weapons.

The one exception is colour: the melta cannon uses orange rope to reflect its thermal/heat aesthetic. All other weapons use green rope consistent with the all-green lighting scheme.

The series resistor value for the MOSFET circuit is pending a confirmed Vf measurement of the LED rope under load. Until that measurement is taken, do not finalise resistor values for any weapon. See blaster.md for current measured values.

---

## Weapon Identity Detection

Each shoulder socket has one analog ID pin wired to a dedicated ESP32-S3 GPIO — two pins total, one per socket, read independently. Inside the torso, a 10kΩ pull-up resistor ties each ID pin to 3.3V. Each weapon arm has a fixed resistor to GND built into its connector plug, forming a voltage divider. The ESP32-S3 reads the analog voltage on each pin and maps it to a weapon profile for that socket.

### ID Resistor Table

| Weapon          | ID Resistor        | Voltage at pin (approx) |
| --------------- | ------------------ | ----------------------- |
| Nothing mounted | Open (no resistor) | 3.3V                    |
| Power fist      | 10kΩ               | 1.65V                   |
| Chain fist      | 22kΩ               | 2.27V                   |
| Melta cannon    | 4.7kΩ              | 1.23V                   |
| Volcano cannon  | 2.2kΩ              | 0.67V                   |
| Laser blaster   | 1kΩ                | 0.31V                   |

Resistor values are chosen to space voltage readings far enough apart that ADC tolerance and resistor variance (±1% metal film recommended) cannot cause misidentification. Verify actual voltages on the bench with a multimeter before finalising firmware thresholds.

### Firmware Behaviour

- ESP32-S3 reads both ID pins independently at boot and maps each socket to a weapon profile
- Channel assignments for the arm pool are derived from both loaded profiles combined
- Web UI reflects the detected loadout on each socket automatically
- Unknown voltage (outside all defined bands) defaults to "nothing mounted" and disables that socket's channels
- Weapon identity is read at boot only — power cycle required after changing loadout

### Implementation Cost

- 2 analog-capable GPIO pins total (1 per socket)
- 1 × 10kΩ pull-up resistor per socket inside the torso (2 total)
- 1 ID resistor per weapon arm, built into the connector plug

---

## Channel Pool Allocation (Runtime)

Example — laser blaster (left socket) + power fist (right socket):

| Ch   | Assigned to  | Zone                    |
| ---- | ------------ | ----------------------- |
| 0    | Left socket  | Blaster barrels         |
| 1    | Left socket  | Heatsink rear           |
| 2    | Left socket  | Heatsink mid-rear       |
| 3    | Left socket  | Heatsink mid            |
| 4    | Left socket  | Heatsink mid-front      |
| 5    | Left socket  | Heatsink front          |
| 6    | Right socket | Power fist odd fingers  |
| 7    | Right socket | Power fist even fingers |
| 8–11 | Unassigned   | Idle                    |

Example — laser blaster (left) + laser blaster (right):

| Ch  | Assigned to  | Zone               |
| --- | ------------ | ------------------ |
| 0   | Left socket  | Blaster barrels    |
| 1   | Left socket  | Heatsink rear      |
| 2   | Left socket  | Heatsink mid-rear  |
| 3   | Left socket  | Heatsink mid       |
| 4   | Left socket  | Heatsink mid-front |
| 5   | Left socket  | Heatsink front     |
| 6   | Right socket | Blaster barrels    |
| 7   | Right socket | Heatsink rear      |
| 8   | Right socket | Heatsink mid-rear  |
| 9   | Right socket | Heatsink mid       |
| 10  | Right socket | Heatsink mid-front |
| 11  | Right socket | Heatsink front     |

---

## Open Items

- Design volcano cannon, melta cannon, chain fist lighting zones and document channel budgets
- Confirm ID resistor voltage readings on the bench and set firmware ADC thresholds
- Finalise series resistor value once Vf measurement is taken — applies to power fist, volcano cannon, melta cannon, chain fist. Blaster confirmed at 6.8Ω (see blaster.md)