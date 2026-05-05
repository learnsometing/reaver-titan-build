# Head

*Reaver Titan Lighting System — Emperor's Children Build*

The head contains 2 PCA9685 channels across two lighting zones. A staggered startup sequence brings the head to life on power-on.

---

## Zone 1 — Eyes + Sensor Eye (Ch 12)

### Eyes (Fiber Optic)

4 green eyes fed by fiber optic strands from a single 5mm green through-hole LED (from Arduino starter kit). The LED is mounted in the neck or upper torso area with 4 fiber strands routed up into the head and terminated in the eye sockets. The 5mm domed lens concentrates light forward for efficient fiber coupling.

### Sensor Eye

1 red sensor on the far side of the head, visually distinct from the green eyes. 1× 3V red Evan Designs chip LED (already purchased) with 100Ω inline series resistor for 5V bus compatibility.

### Electrical

| LED                                  | Resistor | Drive          | Current |
| ------------------------------------ | -------- | -------------- | ------- |
| 1× 5mm green through-hole (eyes)     | 150Ω     | Direct PCA9685 | ~20mA   |
| 1× 3V red Evan Designs chip (sensor) | 100Ω     | Direct PCA9685 | ~20mA   |

Both LEDs wired to Ch 12. Direct PCA9685 drive, no MOSFET needed.

### Effect

Boot ramp from off to full brightness over ~2–3 seconds on power-on, then holds at full static brightness for the duration of the display session. The fiber optic diffusion provides a soft, otherworldly glow. Eyes and sensor are always on together — no independent blink effect.

> **Note:** Ch 12 has two different LED types (5mm green through-hole and 3V red chip) on the same channel. They will both respond to the same PWM signal but brightness levels may not be balanced at the same duty cycle. Verify balance on the bench before committing to final wiring.

---

## Zone 2 — Brain + Mouth (Ch 13)

### Brain

The brain protrudes through the head armour, printed in clear resin. Lighting is positioned to illuminate recessed areas in the brain matter — the intent is to make the resin glow and give the brain life, with paint doing the heavy lifting on detail and texture. Single channel, single slow pulse.

### Mouth

1× 5V green Evan Designs chip LED (to be purchased) mounted behind a drilled-out grill sculpted with fleshy daemon detail. Wired to the same channel as the brain — both breathe together as a unified organic pulse across the whole face. The mouth LED is naturally dimmer than the brain due to a higher series resistance, so it reads as a secondary glow rather than competing with the brain.

### Electrical

| LED                                   | Resistor                  | Drive                  | Current |
| ------------------------------------- | ------------------------- | ---------------------- | ------- |
| Brain LEDs (count TBD)                | TBD                       | Direct or MOSFET (TBD) | TBD     |
| 1× 5V green Evan Designs chip (mouth) | Built-in + additional TBD | Direct PCA9685         | ~20mA   |

Both wired to Ch 13. MOSFET requirement for brain depends on final LED count — 1 LED = direct drive, 2+ in parallel = MOSFET (IRLML6344, same circuit as blaster channels).

### Effect

Slow unified pulse across brain and mouth — a single sine wave on Ch 13 makes the whole face breathe together. Pulse period randomises between ~2–5 seconds. The effect suggests the whole head is alive rather than individual zones animating independently.

---

## Startup Sequence

1. **Eyes + sensor** (Ch 12) warm up first — slow ramp from off to full brightness over ~2–3 seconds.
2. **Brain + mouth** (Ch 13) begin pulsing after eyes reach full brightness.

---

## Electrical Summary

| Ch  | Zone              | LEDs                                                   | Drive                | Notes                                           |
| --- | ----------------- | ------------------------------------------------------ | -------------------- | ----------------------------------------------- |
| 12  | Eyes + sensor eye | 1× 5mm green through-hole, 1× 3V red Evan Designs chip | Direct PCA9685       | 150Ω (eyes), 100Ω (sensor)                      |
| 13  | Brain + mouth     | Brain LEDs TBD, 1× 5V green Evan Designs chip          | Direct or MOSFET TBD | MOSFET needed if brain uses 2+ LEDs in parallel |

---

## Open Items

- Brain LED count and type — determines MOSFET requirement for Ch 13
- Brain LED placement — position inside clear resin to illuminate recessed areas evenly
- Mouth LED additional resistor value — to set brightness lower than brain on the same channel