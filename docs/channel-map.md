# PCA9685 Channel Map

*Reaver Titan Lighting System — Emperor's Children Build*

Complete channel allocation across all 16 PCA9685 channels.

---

## Full Channel Map

| Ch  | Zone                      | Body Section   | LED Type                                               | Drive                | Effect                                               | Doc      |
| --- | ------------------------- | -------------- | ------------------------------------------------------ | -------------------- | ---------------------------------------------------- | -------- |
| 0   | Left arm — signal line A  | Left shoulder  | LED rope (green)                                       | MOSFET (IRLML6344)   | Weapon profile dependent                             | arms.md  |
| 1   | Left arm — signal line B  | Left shoulder  | LED rope (green)                                       | MOSFET               | Weapon profile dependent                             | arms.md  |
| 2   | Left arm — signal line C  | Left shoulder  | LED rope (green)                                       | MOSFET               | Weapon profile dependent                             | arms.md  |
| 3   | Left arm — signal line D  | Left shoulder  | LED rope (green)                                       | MOSFET               | Weapon profile dependent                             | arms.md  |
| 4   | Left arm — signal line E  | Left shoulder  | LED rope (green)                                       | MOSFET               | Weapon profile dependent                             | arms.md  |
| 5   | Left arm — signal line F  | Left shoulder  | LED rope (green)                                       | MOSFET               | Weapon profile dependent                             | arms.md  |
| 6   | Right arm — signal line A | Right shoulder | LED rope (green or orange)                             | MOSFET               | Weapon profile dependent                             | arms.md  |
| 7   | Right arm — signal line B | Right shoulder | LED rope (green or orange)                             | MOSFET               | Weapon profile dependent                             | arms.md  |
| 8   | Right arm — signal line C | Right shoulder | LED rope (green or orange)                             | MOSFET               | Weapon profile dependent                             | arms.md  |
| 9   | Right arm — signal line D | Right shoulder | LED rope (green or orange)                             | MOSFET               | Weapon profile dependent                             | arms.md  |
| 10  | Right arm — signal line E | Right shoulder | LED rope (green or orange)                             | MOSFET               | Weapon profile dependent                             | arms.md  |
| 11  | Right arm — signal line F | Right shoulder | LED rope (green or orange)                             | MOSFET               | Weapon profile dependent                             | arms.md  |
| 12  | Eyes + sensor eye         | Head           | 1× 5mm green through-hole, 1× 3V red Evan Designs chip | Direct PCA9685       | Boot ramp, then static                               | head.md  |
| 13  | Brain + mouth             | Head           | Brain LEDs TBD, 1× 5V green Evan Designs chip          | Direct or MOSFET TBD | Slow unified pulse                                   | head.md  |
| 14  | Power indicators          | Torso          | 2× 5mm domed purple through-hole                       | Direct PCA9685       | Boot ramp, then static. Reflects display mode state. | torso.md |
| 15  | Rear exhaust vents        | Torso rear     | Orange LED rope (1.5mm silicone-sleeved)               | MOSFET               | Faint baseline idle glow, spikes on weapon discharge | torso.md |

All 16 channels allocated. No overhead remaining.

---

## Non-PWM (Direct Bus)

| Zone          | Body Section          | LED Type                             | Notes                                                             |
| ------------- | --------------------- | ------------------------------------ | ----------------------------------------------------------------- |
| Tank fire kit | Right leg / tank base | Evan Designs 5–12V fire kit (3 LEDs) | Self-contained flicker circuit, no PCA9685 channel needed. ~60mA. |

---

## Arm Pool Notes

Ch 0–5: left socket. Ch 6–11: right socket. All MOSFET-driven. See [arms.md](arms.md) for full pool architecture, connector pinout, and weapon ID detection.

---

## Blaster Channel Reference

*Quick reference — see [blaster.md](blaster.md) for zone descriptions and firing sequence.*

| Socket | Barrels | Heatsink front | Heatsink mid-front | Heatsink mid | Heatsink mid-rear | Heatsink rear |
| ------ | ------- | -------------- | ------------------ | ------------ | ----------------- | ------------- |
| Left   | 0       | 1              | 2                  | 3            | 4                 | 5             |
| Right  | 6       | 7              | 8                  | 9            | 10                | 11            |

---

## Open Items

- Series resistor values for remaining MOSFET-driven channels — blocked pending bench Vf measurement. Blaster confirmed (see blaster.md); applies to power fist, volcano cannon, melta cannon, chain fist, and exhaust vent
- Brain LED count and type (Ch 13) — determines MOSFET requirement
- Power indicator resistor value (Ch 14) — pending purple LED forward voltage spec
- Display mode colour behaviour for power indicators — to be defined during firmware development
- Speaker/audio circuit for missile launcher — separate from PCA9685, connects directly to ESP32-S3 via I2S. Open item.