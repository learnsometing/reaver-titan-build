# Power Fist

*Reaver Titan Lighting System — Emperor's Children Build*

## Overview

The chaos power fist has 5 fingers, each with a knuckle and a claw, both printed in clear resin. One LED rope segment per finger lights both the knuckle and claw through resin diffusion. Two PCA9685 arm pool channels drive the 5 segments in two groups with offset crackle patterns for a more organic energy effect.

This weapon is part of the modular arm system — see [arms.md](arms.md) for channel pool allocation and shoulder connector standard.

---

## Channel Allocation

| Pool Channel               | Fingers | Zone                            |
| -------------------------- | ------- | ------------------------------- |
| A (Ch 0 left / Ch 6 right) | 1, 3, 5 | Knuckles + claws (odd fingers)  |
| B (Ch 1 left / Ch 7 right) | 2, 4    | Knuckles + claws (even fingers) |

Actual PCA9685 channel numbers depend on which socket the power fist is mounted in — the firmware applies the correct pool offset at boot based on ID pin detection.

---

## Lighting Design

### Physical

- 5 LED rope segments total, one per finger
- Each segment is routed through a 1.5mm channel modelled into the clear resin knuckle and claw housings
- The fingers are articulated — knuckle and claw are separate resin pieces connected at a joint. The rope will be exposed at each joint between the two resin sections.
- Channel geometry is designed in slicer software around the rope diameter — the cavity fits the rope, not the other way around

### Articulated Joint — Prototyping Decision

The exposed rope at each finger joint is a known visual unknown. Two possible outcomes:

1. **It looks intentional.** At Titan scale a thin glowing line at the joint could read as warp energy bleeding through the articulation point — consistent with the Slaanesh daemon-touched aesthetic.

2. **It reads as a hot spot.** If the exposed bend is too bright relative to the knuckle and claw resin, black heat shrink tubing over the rope at each joint will hide the exposed segment cleanly. Heat shrink is flexible, won't interfere with articulation, and can be added or removed during prototyping.

Resolve during prototyping by lighting the assembled finger and evaluating visually before committing to final assembly.

### Effects

**Boot ramp:** All 5 segments ramp from off to idle brightness over ~1–2 seconds on power-on.

**Idle (constant crackling energy):** Both channels run an irregular crackle algorithm — random amplitude variation around a base brightness level. The two channels use different random seeds, so odd and even fingers flicker slightly out of phase, producing a more convincing energy field than a single synchronised glow.

**Activated:** Same crackle algorithm, higher base brightness, faster and more aggressive amplitude variation. Triggered via the web UI.

There is no discharge or cool-down state — the fist returns directly to idle when deactivated.

---

## Electrical

- **LED type:** 1.5mm silicone-sleeved green LED rope (same spec as blaster) — see [arms.md](arms.md) LED standard
- **Drive:** MOSFET-driven (IRLML6344) as per arm pool standard — see [arms.md](arms.md)
- **Series resistor:** TBD — pending confirmed Vf measurement of LED rope under load. Do not finalise until measurement is taken.

---

## Open Items

- Design finger cavities in slicer — 1.5mm channels through clear resin knuckle and claw housings
- Confirm rope segment lengths needed per finger once print dimensions are known
- Evaluate exposed rope at finger joints during prototyping — determine whether heat shrink is needed
- Finalise series resistor value once Vf measurement is available