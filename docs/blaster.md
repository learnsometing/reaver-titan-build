# Blaster Arm

*Reaver Titan Lighting System — Emperor's Children Build*

The laser blaster is the most complex lighting zone. It uses 6 PCA9685 channels to create a multi-state firing sequence across the barrels and heatsink. Each channel is driven through an N-channel MOSFET. The weapon body, barrels, and heatsink are printed in clear resin.

When mounted in the left socket, the blaster uses Ch 0–5. When mounted in the right socket, it uses Ch 6–11. The firmware applies the correct offset based on weapon ID detection at boot — the weapon arm itself does not change. See [arms.md](arms.md) for the full arm pool and socket architecture.

Channel references below use left socket numbering (Ch 0–5). Substitute Ch 6–11 for right socket.

---

## Barrels — Channel 0

Three barrels, each approximately 7.1mm inner diameter. Lit by green LED rope (1.5mm overall diameter including green silicone diffusion sleeve, Vf 2.7–3.1V, max 300mA) wrapped around a helical core printed to fit inside the barrel. The silicone sleeve acts as a diffuser, producing a smooth continuous glow rather than visible point sources. All three barrels are driven as a single synchronised zone from Ch 0 via MOSFET driver.

A prototype helical core has been tested and confirmed to work at this diameter. LED rope is on hand and cannot be cut — insert designs will be modelled around fixed rope lengths.

---

## Heatsink — Channels 1–5

11 fins with 10 gaps between them. An internal harness (up to 27mm diameter) houses LED rope segments (same 1.5mm silicone-sleeved rope as the barrels) positioned to shine through the gaps and reflect off the fin surfaces. The silicone sleeve provides even light spread without hot spots. Each zone is driven via its own MOSFET driver.

The 10 gaps are grouped into 5 equal zones of 2 gaps each for directional animation:

| Zone | Gaps | Ch (left socket) | Ch (right socket) | Position |
|------|------|-----------------|------------------|----------|
| Rear | 1–2 | 1 | 7 | Reactor end (energy origin) |
| Mid-rear | 3–4 | 2 | 8 | |
| Mid | 5–6 | 3 | 9 | |
| Mid-front | 7–8 | 4 | 10 | |
| Front | 9–10 | 5 | 11 | Muzzle end (closest to barrels) |

---

## Firing Sequence

The heatsink houses the weapon's reactor. All energy originates at the rear and propagates forward through the heatsink zones toward the barrels. The rear heatsink zone is always the origin point — it lights first, dims last, and never fully goes dark.

### IDLE

Faint glow in the rear heatsink zone only (Ch 1, gaps 1–2). All other zones dark. The reactor is ticking over at standby power.

### PRIME

Rear heatsink zone brightens. Energy propagates forward sequentially: rear (Ch 1) reaches full brightness first, then mid-rear (Ch 2), mid (Ch 3), mid-front (Ch 4), then front (Ch 5). Each zone reaches mid-brightness before the next begins — an overlapping forward surge. Barrels remain dark during this phase.

### FULL CHARGE

All five heatsink zones at full brightness. Barrels (Ch 0) now ramp up as the energy reaches the muzzle end. Everything fully lit with a subtle oscillation or flicker suggesting barely contained power.

### DISCHARGE

Barrels flash bright. The heatsink performs a fast rear-to-front pulse — a final surge of energy shoved forward from the reactor through the weapon and out the barrels. Everything then fades front-to-back: barrels dim first, then front heatsink zone, mid-front, mid, mid-rear. The rear zone stays lit longest as the reactor remains hot, then gradually dims back down to idle glow. The cycle can then repeat.

---

## Schematic

Each of the 6 blaster channels uses an IRLML6344 N-channel MOSFET (SOT-23 package) to switch the LED rope. The circuit per channel:

**Why a MOSFET?** The PCA9685 is limited to 25mA per output channel. Each LED rope segment draws 30–100mA (72mA measured at 5V through 6.8Ω) — exceeding the 25mA limit. The MOSFET solves this by acting as a high-current switch: the PCA9685 controls the gate using only a tiny signal current, while the MOSFET handles the full LED load between drain and source. The controller stays safe; the MOSFET does the heavy lifting.

![Blaster zone schematic](../kicad/blaster.svg)

**MOSFET:** IRLML6344 (SOT-23). Logic-level N-channel, fully switches on at 3.3V gate voltage, handles >4A continuous drain current.

**10kΩ gate pull-down:** Prevents LED flicker during power-up before the PCA9685 initialises.

**Series resistor sizing** (at 5V supply, 3.0V LED rope Vf, 2.0V drop across resistor):

| Option | Resistor | Current | Dissipation |
|--------|----------|---------|-------------|
| Max brightness | 6.8Ω 1W | ~294mA | ~0.59W |
| Derated (more thermal margin) | 8.2Ω 1W | ~244mA | ~0.49W |

> **Measured:** Actual current draw tested at 5V through 6.8Ω measured **72mA** — consistent with the Amazon listing's 30–100mA typical range. The 300mA figure on the packaging is the absolute maximum rating. At 72mA the resistor dissipates only ~0.035W, well under its 1W rating. The 6.8Ω resistor remains the right choice; if more brightness is needed, the resistor value can be reduced since there is significant headroom before hitting the 300mA maximum. Revisit resistor sizing later in the prototype phase if more brightness is needed.

**Trigger method:** Dedicated button on the ESP32-S3 web UI initiates the firing sequence. Timed automatic loops and display mode presets are also supported. Physical GPIO button input is available as an alternative.

---

## Bill of Materials — Blaster Zone

| Component | Product | Qty | Source | Status | Notes |
|-----------|---------|-----|--------|--------|-------|
| **MOSFET** | IRLML6344TRPBF (Infineon, SOT-23) | 10–20 | Digi-Key | Arrived | N-channel logic-level MOSFET, SOT-23. Fully switches at 3.3V gate, handles >4A drain. Buy genuine from Digi-Key — counterfeits common on Amazon. |
| **Series resistor** | TE Connectivity ROX1SJ6R8 (6.8Ω 1W metal oxide axial) | 10 | Digi-Key | Arrived | One per blaster channel, 6 needed. 1W rating provides large headroom — actual dissipation ~0.035W at 72mA measured draw. |
| **Gate pull-down resistor** | 10kΩ ¼W carbon film | 6 | On hand | Arrived | From Arduino starter kit. |
| **Barrel LED rope** | Green silicone-sleeved LED rope, 1.5mm — 11.81" lengths | 3 | On hand | Arrived | One per barrel. Cannot be cut — barrel inserts modelled around this length. |
| **Heatsink LED rope** | Green silicone-sleeved LED rope, 1.5mm — 5.11" lengths | 5 | On hand | Arrived | One per heatsink zone (Ch 1–5). Cannot be cut — heatsink harness modelled around this length. |
| **SOT-23 breakout boards** | SOT-23 to DIP adapter breakout boards | 5–10 | Amazon | Arrived | For breadboard prototyping. One per blaster channel. |