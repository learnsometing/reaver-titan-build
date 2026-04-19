# Head

*Reaver Titan Lighting System — Emperor's Children Build*

The head contains 6 PCA9685 channels across 4 distinct lighting zones: eyes (Ch 0), brain (Ch 1–3), sensor eye (Ch 10), and mouth (Ch 11). A staggered startup sequence brings the head to life on power-on.

---

## Eyes — Channel 0 (Fiber Optic)

4 green eyes fed by fiber optic strands from a single 5mm green through-hole LED (from Arduino starter kit). The LED is mounted in the neck or upper torso area with 4 fiber strands routed up into the head and terminated in the eye sockets. The 5mm domed lens concentrates light forward for efficient fiber coupling.

**Electrical:** 150Ω series resistor on the 5V bus → ~20mA. PCA9685 channel 0, direct drive, no MOSFET needed.

**Effect:** Slow warm-up ramp from off to full brightness over ~2–3 seconds on power-on, then holds at full static brightness. The fiber optic diffusion provides a soft, otherworldly glow rather than harsh point sources.

---

## Brain — Channels 1–3

The brain protrudes through the head armour, printed in clear resin. Lit by 3 independent PCA9685 channels for a randomised organic pulsing effect. Each channel drives green LEDs with randomised sine wave offsets. Periods randomise between ~2–5 seconds with phase drift to prevent channels from synchronising over time.

**Effect:** Three channels breathing independently, occasionally drifting into alignment by chance, then apart again — an organic, living quality.

**Open items (to be resolved during prototyping):**

- LED count per channel — 1 to 4 per channel depending on physical space and diffusion quality. MOSFET requirement depends on this decision (1 LED at ~20mA = direct drive; 2+ LEDs in parallel = MOSFET needed, same IRLML6344 circuit as blaster channels).
- Clear resin print design — internal cavities or channels for LED placement.
- Diffusion approach — how to spread light evenly across brain surface zones from point-source LEDs.
- LED type selection — to be determined based on physical constraints of the brain housing.

---

## Sensor Eye — Channel 10

1 red sensor on the far side of the head, visually distinct from the green eyes.

**Electrical:** 1× 3V red Evan Designs chip LED (already purchased) with 100Ω inline series resistor for 5V bus compatibility → ~20mA. PCA9685 channel 10, direct drive.

**Effect:** Static glow with periodic blink — holds steady brightness, briefly dips to zero at randomised intervals, then returns to steady. Suggests an active scanning or targeting sensor.

---

## Mouth — Channel 11

1× 5V green Evan Designs chip LED (to be purchased) mounted behind a drilled-out grill, allowing light to bleed through the gaps. The grill will be sculpted with fleshy daemon detail for a Slaanesh aesthetic.

**Electrical:** PCA9685 channel 11, direct drive. Evan Designs 5V chip LED with built-in resistor for 5V bus.

**Effect:** Organic breathing rhythm with a slow sine wave pulse, loosely similar in tempo to the brain channels (2–5 second range) but not synchronised. The mouth has its own randomised period and phase drift — it occasionally aligns with the brain pulses by chance, then drifts apart, creating natural moments where the whole head seems to inhale together.

> **Note:** The mouth channel will be wired during the build but may be left unprogrammed until the eyes and brain effects are evaluated visually. If the head reads well with just eyes and brain, the mouth can remain dark as a sculpting-only detail.

---

## Startup Sequence

On power-on, head zones activate in a staggered sequence to create the impression of a daemon coming online:

1. **Eyes** warm up first — slow ramp from off to full brightness over ~2–3 seconds.
2. **Brain** channels begin pulsing after eyes reach full brightness.
3. **Mouth** breathing begins last (if enabled).
4. **Sensor** blink pattern activates alongside or shortly after the brain.

---

## Electrical Summary

| Ch | Zone | LED | Resistor | Drive | Current |
|----|------|-----|----------|-------|---------|
| 0 | Eyes (fiber optic) | 1× 5mm green through-hole | 150Ω | Direct PCA9685 | ~20mA |
| 1 | Brain A | Green (count TBD) | TBD | Direct or MOSFET | TBD |
| 2 | Brain B | Green (count TBD) | TBD | Direct or MOSFET | TBD |
| 3 | Brain C | Green (count TBD) | TBD | Direct or MOSFET | TBD |
| 10 | Sensor eye | 1× 3V red Evan Designs chip | 100Ω | Direct PCA9685 | ~20mA |
| 11 | Mouth | 1× 5V green Evan Designs chip | Built-in | Direct PCA9685 | ~20mA |