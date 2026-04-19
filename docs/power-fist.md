# Power Fist

*Reaver Titan Lighting System — Emperor's Children Build*

## Overview

The left arm power fist uses clear resin knuckles to diffuse green LED light. A single PCA9685 channel drives a pulsing glow effect.

| Channel | Zone | Effect |
|---------|------|--------|
| 4 | Power fist knuckles | Pulse |

---

## Electrical

- **LED type and count:** TBD — depends on physical space inside the knuckle housing
- **Series resistor:** 100Ω per LED at 5V as baseline
- **Drive:** Direct PCA9685 channel 4 if LED count is low (~1 LED at ~20mA). MOSFET driver (IRLML6344, same circuit as blaster channels) if multiple LEDs are run in parallel

**Effect:** Slow pulse — a rhythmic energy build-up in the knuckles, distinct from the brain's breathing rhythm.

---

## Open Items

- Final LED count per channel — determines whether a MOSFET driver is needed
- LED type selection — to be determined based on physical fit inside the clear resin knuckle cavities