# Blaster Arm

*Reaver Titan Lighting System — Emperor's Children Build*

The laser blaster is the most complex lighting zone. It uses 5 PCA9685 channels (5–9) to create a multi-state firing sequence across the barrels and heatsink. Each channel is driven through an N-channel MOSFET since the LED rope segments draw up to 300mA, far exceeding the PCA9685's 25mA per-channel limit. The weapon body, barrels, and heatsink are printed in clear resin.

---

## Barrels — Channel 5

Three barrels, each approximately 7.1mm inner diameter. Lit by green LED rope (1.5mm overall diameter including green silicone diffusion sleeve, Vf 2.7–3.1V, max 300mA) wrapped around a helical core printed to fit inside the barrel. The silicone sleeve acts as a diffuser, producing a smooth continuous glow rather than visible point sources. All three barrels are driven as a single synchronised zone from PCA9685 channel 5 via MOSFET driver.

A prototype helical core has been tested and confirmed to work at this diameter. LED rope is on hand and cannot be cut — insert designs will be modelled around fixed rope lengths.

---

## Heatsink — Channels 6–9

11 fins with 10 gaps between them. An internal harness (up to 27mm diameter) houses LED rope segments (same 1.5mm silicone-sleeved rope as the barrels) positioned to shine through the gaps and reflect off the fin surfaces. The silicone sleeve provides even light spread without hot spots. Each zone is driven via its own MOSFET driver.

The 10 gaps are grouped into 4 zones for directional animation:

| Zone | Gaps | PCA9685 Ch | Position |
|------|------|------------|----------|
| Rear | 1–3 | 6 | Reactor end (energy origin) |
| Mid-rear | 4–5 | 7 | |
| Mid-front | 6–8 | 8 | |
| Front | 9–10 | 9 | Muzzle end (closest to barrels) |

---

## Firing Sequence

The heatsink houses the weapon's reactor. All energy originates at the rear and propagates forward through the heatsink zones toward the barrels. The rear heatsink zone is always the origin point — it lights first, dims last, and never fully goes dark.

### IDLE

Faint glow in the rear heatsink zone only (Ch 6, gaps 1–3). All other zones dark. The reactor is ticking over at standby power.

### PRIME

Rear heatsink zone brightens. Energy propagates forward sequentially: rear (Ch 6) reaches full brightness first, then mid-rear (Ch 7), then mid-front (Ch 8), then front (Ch 9). Each zone reaches mid-brightness before the next begins — an overlapping forward surge. Barrels remain dark during this phase.

### FULL CHARGE

All four heatsink zones at full brightness. Barrels (Ch 5) now ramp up as the energy reaches the muzzle end. Everything fully lit with a subtle oscillation or flicker suggesting barely contained power.

### DISCHARGE

Barrels flash bright. The heatsink performs a fast rear-to-front pulse — a final surge of energy shoved forward from the reactor through the weapon and out the barrels. Everything then fades front-to-back: barrels dim first, then front heatsink zone, mid-front, mid-rear. The rear zone stays lit longest as the reactor remains hot, then gradually dims back down to idle glow. The cycle can then repeat.

---

## MOSFET Drive Circuit

Each of the 5 blaster channels uses an IRLML6344 N-channel MOSFET (SOT-23 package) to switch the LED rope. The circuit per channel:

```
+5V ──────────────────────── LED rope anode
                                    │
                              LED rope cathode
                                    │
                               6.8Ω 1W resistor
                                    │
                              MOSFET drain (IRLML6344)
                              MOSFET source ── GND
                              MOSFET gate ──── PCA9685 output pin
                                    │
                               10kΩ pull-down
                                    │
                                   GND
```

**MOSFET:** IRLML6344 (SOT-23). Logic-level N-channel, fully switches on at 3.3V gate voltage, handles >4A continuous drain current. The SOT-23 footprint is compact enough to mount inside the arm or torso without significant space impact.

**10kΩ gate pull-down:** Prevents LED flicker during power-up before the PCA9685 initialises.

**Series resistor sizing** (at 5V supply, 3.0V LED rope Vf, 2.0V drop across resistor):

| Option | Resistor | Current | Dissipation |
|--------|----------|---------|-------------|
| Max brightness | 6.8Ω 1W | ~294mA | ~0.59W |
| Derated (more thermal margin) | 8.2Ω 1W | ~244mA | ~0.49W |

> **Note:** If LED rope Vf is closer to 2.7V (low end of spec), voltage drop increases to 2.3V and current rises accordingly. Measure actual Vf during prototyping and adjust resistor value if needed.

**Trigger method:** Dedicated button on the ESP32-S3 web UI initiates the firing sequence. Timed automatic loops and display mode presets are also supported. Physical GPIO button input is available as an alternative.

---

## Schematic References

| Ref | Component |
|-----|-----------|
| Q1 | MOSFET — blaster barrels (Ch 5) |
| Q2 | MOSFET — heatsink rear (Ch 6) |
| Q3 | MOSFET — heatsink mid-rear (Ch 7) |
| Q4 | MOSFET — heatsink mid-front (Ch 8) |
| Q5 | MOSFET — heatsink front (Ch 9) |
| R2/R4/R6/R8/R10 | Series resistors (6.8Ω 1W) |
| R1/R3/R5/R7/R9 | Gate pull-down resistors (10kΩ ¼W) |

---

## Bill of Materials — Blaster Zone

Order alongside or immediately after the core power system BOM. LED rope is already on hand.

| Component | Product | Qty | Source | Notes |
|-----------|---------|-----|--------|-------|
| **MOSFET** | IRLML6344TRPBF (Infineon, SOT-23) | 10–20 | Digi-Key | N-channel logic-level MOSFET, SOT-23. Fully switches at 3.3V gate, handles >4A drain. Buy genuine from Digi-Key — counterfeits common on Amazon. Digi-Key part: IRLML6344TRPBF. Buy 10–20 (tiny, easy to lose). |
| **Series resistor** | TE Connectivity ROX1SJ6R8 (6.8Ω 1W metal oxide axial) | 10 | Digi-Key | One per blaster channel, 5 needed. 1W rating required — actual dissipation ~0.59W at 294mA. Buy 10 for spares. |
| **Gate pull-down resistor** | 10kΩ ¼W carbon film | 5 | On hand | From Arduino starter kit. Dissipation only ~2.5mW — ¼W is ample. |
| **LED rope** | Green silicone-sleeved LED rope, 1.5mm | 2 | On hand | Two fixed-length ropes, same supplier, same luminosity. Vf 2.7–3.1V, max 300mA confirmed from packaging. Cannot be cut — barrel and heatsink inserts will be designed around rope lengths. Measure actual Vf under load during prototyping and adjust series resistor if needed. |