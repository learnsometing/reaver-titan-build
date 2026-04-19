# Reaver Titan Lighting System — Design Brief

*Emperor's Children | Chaos Reaver | Slaanesh Daemon-Touched*

| | |
|---|---|
| **Version** | 5.3 — Power switch specified, LED rope confirmed, blaster zone BOM added |
| **Status** | Prototyping phase — core power and blaster zone |
| **Date** | April 2026 |

---

## 1. Concept

Warhammer 40K Reaver Titan, Emperor's Children chaos build. Corrupted and fleshy Slaanesh daemon-touched aesthetic. Armor is a classic black/pink split scheme with silver trim and gold accents. The model stands with its right foot on a destroyed tank.

All lighting is green throughout. Clear resin parts are used extensively to diffuse light for an organic, warp-infused appearance.

---

## 2. Power System

| Component | Specification |
|---|---|
| **Battery** | Zeee 2S 2200mAh 7.4V 50C Shorty LiPo (soft pack). Dimensions: 73mm × 34mm × 18.5mm, weight 98g. T-connector (Deans) discharge plug, JST-XH balance lead. Removable via torso access panel for external charging. 2-pack purchased — one spare for swap-out during extended display |
| **Connector** | T-connector (Deans) for battery-to-bus connection. Rated for high current, eliminates the JST undersizing concern flagged in earlier revisions. Internal bus wiring terminates in a matching T-connector for battery plug-in |
| **Protection** | JZK 2S 7.4V 8A BMS protection board (41mm × 16mm × 3.5mm). Over-charge protection (4.25–4.35V per cell), over-discharge protection (2.5–3.0V), short circuit protection, standby current less than 10μA. 8A continuous / 10A peak — provides ample headroom over estimated 2A peak system draw |
| **Charger** | 2S LiPo balance charger required (not included with battery). Must support JST-XH balance lead. Battery is removed from model and charged externally |
| **Voltage regulation** | 5V buck converter, minimum 3A rated. A 5A-rated adjustable buck module is recommended for expansion headroom (current peak draw is approximately 2A, but additional zones will increase this). MP1584 (2–3A) is acceptable for prototyping; consider upgrading to a 5A module (e.g., D24V50F5 or similar) for the final build if total draw exceeds 2.5A |
| **Power switch** | Twidec L-PBS-110-XBK, 7mm latching push button, SPST, 3A 250V AC, pre-soldered wires. Mounted through the torso exterior rectangular housing (existing model feature). Indicator LED to be mounted in the domed feature on top of the same housing, wired to the +SWITCHED net via 270Ω resistor to glow green when system is on. Battery is still always physically unplugged at the T-connector when not in use (LiPo fire safety) — the switch is for convenience during display sessions only |
| **Bus** | Common 5V and GND bus throughout |
| **Runtime estimate** | Normal display mode (brain, eyes, tank fire, idle blaster, WiFi active): approximately 450–600mA draw, approximately 3–4 hours runtime. With blaster firing continuously: approximately 2.2A peak draw, approximately 1 hour runtime. Deep sleep (WiFi off, all LEDs off): less than 50μA, effectively unlimited standby |

---

## 3. Control System

### 3.1 Microcontroller

ESP32-S3 Super Mini. Dual-core Xtensa LX7 at 240MHz, built-in WiFi (802.11 b/g/n) and Bluetooth LE 5.0, 512KB SRAM, 4MB flash. Dimensions: 22.52mm × 18mm — significantly smaller than the Arduino Nano it replaces (18mm × 45mm). USB-C for programming. Runs at 3.3V logic, which the PCA9685 supports natively.

The ESP32-S3 replaces the Arduino Nano and eliminates the need for a separate physical switch, IR receiver, or RF module. It hosts a web server directly on the chip, providing a control panel accessible from any phone or device on the same WiFi network. Full remote control over all lighting zones: on/off, brightness, firing sequences, display modes, and startup sequences — all from a browser. No app installation required.

The dual-core architecture is critical for this build: one core runs the WiFi web server and handles incoming commands, while the other core runs the lighting animation loops (firing sequences, brain pulsing, breathing effects) without stuttering or timing interruptions.

Power draw is higher than the Nano (approximately 150–250mA when WiFi is active versus approximately 50mA for the Nano). On the 2200mAh battery this still provides several hours of runtime.

### 3.2 PWM Driver

PCA9685 16-channel PWM driver board (SunFounder), connected to the ESP32-S3 via I2C. This provides 16 independently controllable PWM channels with 12-bit resolution (0–4095), yielding smoother fading transitions than standard 8-bit PWM. Driven by the Adafruit PWM Servo Driver library (compatible with ESP32 Arduino framework).

The PCA9685 runs at 5V on the common bus with 3.3V logic from the ESP32-S3 for I2C communication — the PCA9685 is 5V-compliant on its logic inputs. Default PWM frequency is approximately 1kHz (adjustable), suitable for all LED effects. A second PCA9685 can be daisy-chained if expansion ever requires more than 16 channels.

### 3.3 Remote Control

The ESP32-S3 creates a WiFi access point or connects to an existing network. A lightweight web page served from the chip's flash provides the control interface. The web UI includes: zone on/off toggles, brightness sliders for each zone, blaster firing sequence trigger, display mode presets (e.g., idle, show mode, full demo), and startup sequence control. The interface is accessible from any device with a web browser — phone, tablet, or computer.

### 3.4 Architecture Summary

LiPo (T-connector) → BMS → latching switch → buck converter (5V) → common bus. ESP32-S3 and PCA9685 both powered from the 5V bus. ESP32-S3 sends I2C commands to PCA9685. PCA9685 drives all LED zones. The latching push button is hidden in the torso exterior for clean power on/off during display. Battery is always physically unplugged at the T-connector when not in use. All GPIO pins on the ESP32-S3 are free for future expansion (buttons, sensors, etc.).

---

## 4. PCA9685 Channel Map

Channel assignments below. Channels 0–15 are available; unassigned channels are reserved for future expansion.

| Channel | Zone | Effect | Notes |
|---|---|---|---|
| 0 | Eyes (fiber optic) | Warm-up ramp, then static | 1x 5mm green through-hole LED feeding 4 fiber optic strands to eye sockets. 150Ω resistor. Direct drive |
| 1 | Brain channel A | Breathing pulse | Green LEDs (count TBD), slow sine wave |
| 2 | Brain channel B | Breathing pulse | Green LEDs (count TBD), offset from Ch 1 |
| 3 | Brain channel C | Breathing pulse | Green LEDs (count TBD), offset from Ch 1–2 |
| 4 | Power fist knuckles | Pulse | Green LEDs, clear resin knuckles |
| 5 | **Blaster — barrels** | Multi-state | 3 barrels synced, LED rope helical wrap, 7.1mm ID |
| 6 | **Blaster — heatsink rear** | Multi-state | LED rope segment(s), gaps 1–3 (rear zone) |
| 7 | **Blaster — heatsink mid-rear** | Multi-state | LED rope segment(s), gaps 4–5 |
| 8 | **Blaster — heatsink mid-front** | Multi-state | LED rope segment(s), gaps 6–8 |
| 9 | **Blaster — heatsink front** | Multi-state | LED rope segment(s), gaps 9–10 (front zone) |
| 10 | Sensor eye | Static + periodic blink | 1x 3V red Evan Designs chip LED with 100Ω inline resistor. Direct drive |
| 11 | Mouth | Organic breathing rhythm | 1x 5V green Evan Designs chip LED. Direct drive. May be left unprogrammed pending visual evaluation |
| 12–15 | Unassigned | — | Reserved for future zones: wings, base, trophies, etc. |

### 4.1 Non-PWM (Direct Bus)

**Tank fire LED kit (Evan Designs, 5–12V version, 14-inch wire)**

Self-contained fire effect kit with 3 LEDs (1 flashing red, 1 flashing orange, 1 solid orange). The overlapping flicker rates produce a randomised fire effect with no coding required. Connects directly to the 5V/GND bus — no Arduino pin or PCA9685 channel needed.

The fire light emanates from inside the destroyed tank beneath the Titan's right foot, bleeding out through plasma holes in the tank side and the rear boarding dock. LEDs are permanently mounted inside the tank cavity. The tank is glued to the right foot, and the model stands on a table without a separate base.

**Wire routing:**

Dupont connectors at both ends of the leg wire run eliminate the need to ever pull wires through the leg after assembly. The wiring is structured as three segments:

1. **Tank segment:** LEDs soldered inside the tank, short wire tails terminating in a Dupont connector inside the tank cavity or just above it inside the foot.
2. **Leg harness:** a passive wire run with Dupont connectors on both ends, pre-threaded through the leg channel during assembly before gluing. This segment stays in the leg permanently and contains no components — just wire. Replaceable if ever damaged since it is not soldered at either end.
3. **Torso segment:** a short wire from the pelvis-side Dupont connector into the torso, connecting to the 5V/GND bus.

The pelvis junction has limited space and requires a sharp turn from the angled leg channel into the torso cavity. The Dupont connector at this end means the leg harness never needs to be pulled through this turn — it is pre-installed and simply plugs in. The fire kit can be bench-tested independently by plugging the tank segment directly into the 5V bus before committing to the leg assembly.

The leg wire channel should be oversized (minimum 5–6mm internal diameter) to allow future additional wiring through the same leg if needed.

**Leg print notes:**

The right leg must be reprinted with a hollow wire channel running its full length. The channel should follow a gentle path where possible, though a sharp turn at the pelvis junction is unavoidable. Open access at both ends (ankle/foot and pelvis/hip). The leg harness wire is threaded through the channel during assembly before the leg is glued to the pelvis, and remains permanently installed. The left leg does not require a wire channel and can be printed solid for maximum structural strength.

---

## 5. Blaster Arm — Detailed Design

The laser blaster is the most complex lighting zone, using 5 PCA9685 channels (5–9) to create a multi-state firing sequence. Each channel is driven through an N-channel MOSFET since the LED rope segments draw up to 300mA, far exceeding the PCA9685's per-channel current limit. The weapon body, barrels, and heatsink are printed in clear resin.

### 5.1 Barrels

Three barrels, each approximately 7.1mm inner diameter. Lit by green LED rope (1.5mm overall diameter including green silicone diffusion sleeve, forward voltage 2.7–3.1V, max current 300mA) wrapped around a helical core printed to fit inside the barrel. The silicone sleeve acts as a diffuser, producing a smooth continuous glow rather than visible point sources. All three barrels are driven as a single synchronized zone from PCA9685 channel 5 via MOSFET driver. A prototype helical core has been tested and confirmed to work at this diameter. LED rope is on hand — cannot be cut; insert designs will be modelled around fixed rope lengths.

### 5.2 Heatsink

11 fins with 10 gaps between them. Internal harness (up to 27mm diameter) houses LED rope segments (same 1.5mm silicone-sleeved rope as barrels, 2.7–3.1V forward voltage, 300mA max) positioned to shine through the gaps and reflect off the fin surfaces. The silicone sleeve provides even light spread across the fin gaps without hot spots. Each zone is driven via its own MOSFET driver from the PCA9685. The 10 gaps are grouped into 4 zones for directional animation effects:

| Zone | Gaps | PCA9685 Ch | Position |
|---|---|---|---|
| Rear | 1–3 | 6 | Reactor end (energy origin) |
| Mid-rear | 4–5 | 7 | |
| Mid-front | 6–8 | 8 | |
| Front | 9–10 | 9 | Muzzle end (closest to barrels) |

### 5.3 Firing Sequence

The heatsink houses the weapon's reactor/battery. All energy originates at the rear of the weapon and propagates forward through the heatsink zones toward the barrels. The rear heatsink zone is always the origin point — it lights first, dims last, and never fully goes dark.

**IDLE**

Faint glow in the rear heatsink zone only (Ch 6, gaps 1–3). All other zones dark. The reactor is ticking over at standby power.

**PRIME**

Rear heatsink zone brightens. Energy propagates forward through the heatsink zones sequentially: rear (Ch 6) reaches full brightness first, then mid-rear (Ch 7), then mid-front (Ch 8), then front (Ch 9). Each zone reaches mid-brightness before the next begins, creating an overlapping forward surge. Barrels remain dark during this phase — the energy has not yet reached them.

**FULL CHARGE**

All four heatsink zones at full brightness. Barrels (Ch 5) now ramp up as the energy reaches the muzzle end. Everything fully lit with a subtle oscillation or flicker suggesting barely contained power.

**DISCHARGE**

Barrels flash bright. The heatsink performs a fast rear-to-front pulse — a final surge of energy shoved forward from the reactor through the weapon and out the barrels. Everything then fades, but front-to-back: barrels dim first, then front heatsink zone, mid-front, mid-rear. The rear heatsink zone stays lit longest as the reactor remains hot, then gradually dims back down to the idle glow level. The cycle can then repeat from idle.

### 5.4 Drive Circuit (Per Blaster Channel)

Each of the 5 blaster channels uses an N-channel MOSFET to switch the LED rope, since the rope segments draw up to 300mA — well beyond the PCA9685's per-channel limit of 25mA. The circuit per channel is as follows:

PCA9685 output → MOSFET gate (with 10kΩ pull-down resistor from gate to GND to prevent LED flicker during power-up before the PCA9685 initialises). MOSFET drain connects to the LED rope cathode through a series current-limiting resistor. MOSFET source connects to common GND. The 5V bus feeds the LED rope anode directly.

MOSFET selection: IRLML6344 (SOT-23 package). Logic-level N-channel, fully switches on at 3.3V gate voltage, handles over 4A continuous drain current. The SOT-23 footprint is compact enough to mount inside the arm or torso without significant space impact.

**Series resistor sizing (at 5V supply, 3.0V LED rope forward voltage, 2.0V drop across resistor):**

- Maximum brightness: 6.8Ω (standard value), yielding approximately 294mA. Resistor must be rated 1W (actual dissipation approximately 0.59W at this current).
- Derated option: 8.2Ω, yielding approximately 244mA with approximately 0.49W dissipation. Provides more thermal margin in enclosed spaces with slightly reduced brightness.

Note: if the LED rope forward voltage is closer to 2.7V (low end of spec), the voltage drop increases to 2.3V and current rises accordingly. Measure actual forward voltage during prototyping and adjust resistor value if needed.

### 5.5 Trigger Method

Triggered via the ESP32-S3 web interface. A dedicated button on the control page initiates the firing sequence. Timed automatic loops and preset display modes are also supported. Physical button input via ESP32-S3 GPIO remains available as an alternative.

---

## 6. Head — Detailed Design

The head contains 6 PCA9685 channels across 4 distinct lighting zones: eyes, sensor, mouth, and brain. A staggered startup sequence brings the head to life on power-on.

### 6.1 Eyes (Fiber Optic)

4 green eyes fed by fiber optic strands from a single 5mm green through-hole LED (from Arduino starter kit). The LED is mounted in the neck or upper torso area with 4 fiber strands routed up into the head and terminated in the eye sockets. The 5mm domed lens concentrates light forward for efficient fiber coupling. 150Ω series resistor on 5V bus yields approximately 20mA. PCA9685 channel 0, direct drive, no MOSFET needed.

The eyes perform a slow warm-up ramp from off to full brightness over approximately 2–3 seconds on power-on, then hold at full static brightness. The fiber optic diffusion provides a soft, otherworldly glow rather than harsh point sources.

### 6.2 Sensor Eye

1 red sensor on the far side of the head, visually distinct from the green eyes. Uses 1x 3V red Evan Designs chip LED (already purchased) with 100Ω inline series resistor for 5V bus compatibility. PCA9685 channel 10, direct drive.

Effect: static glow with periodic blink — holds steady brightness, briefly dips to zero at randomised intervals, then returns to steady. Suggests an active scanning or targeting sensor.

### 6.3 Mouth

1x 5V green Evan Designs chip LED (to be purchased) mounted behind a drilled-out grill, allowing light to bleed through the gaps. The grill will be sculpted with fleshy daemon detail for a creepy Slaanesh aesthetic. PCA9685 channel 11, direct drive.

Effect: organic breathing rhythm with a slow sine wave pulse, loosely similar in tempo to the brain channels (2–5 second range) but not synchronised. The mouth has its own randomised period and phase drift, so it occasionally aligns with the brain pulses by chance, then drifts apart — creating natural moments where the whole head seems to inhale together.

Note: the mouth channel will be wired during the build but may be left unprogrammed until the eyes and brain effects are evaluated visually. If the head reads well with just eyes and brain, the mouth can remain dark as a sculpting-only detail.

### 6.4 Brain

The brain protrudes through the head armor, printed in clear resin. Lit by 3 independent PCA9685 channels (1–3) for a randomised organic pulsing effect. Each channel drives green LEDs with randomised sine wave offsets. Periods randomise between approximately 2–5 seconds with phase drift to prevent channels from synchronising over time.

**Open items (to be resolved during prototyping):**

- LED count per channel — ranges from 1 to 4 per channel depending on physical space and diffusion quality. MOSFET requirement depends on this decision (1 LED at 20mA = direct drive; 4 LEDs at 80mA = MOSFET needed).
- Clear resin print design — internal cavities or channels for LED placement.
- Diffusion approach — how to spread light evenly across brain surface zones from point-source LEDs.
- LED type selection — to be determined based on physical constraints of the brain housing.

### 6.5 Head Startup Sequence

On power-on, the head zones activate in a staggered sequence to create the impression of a daemon coming online:

1. Eyes warm up first — slow ramp from off to full brightness over 2–3 seconds.
2. Brain channels begin pulsing after eyes reach full brightness.
3. Mouth breathing begins last (if enabled).
4. Sensor blink pattern activates alongside or shortly after the brain.

---

## 7. Arms Summary

**Right arm:** Laser blaster (see Section 5 for detailed design)

**Left arm:** Power fist with clear resin knuckles, green LED pulse effect, PCA9685 channel 4

---

## 8. Connector Strategy

All wiring between major body sections uses Dupont connectors at the structural joints. This makes the entire model modular — any body section can be disconnected without soldering. Evan Designs connectors and splitters (already on hand) are used throughout.

### 8.1 Connection Points

- **Right leg (pelvis junction):** tank fire kit 5V/GND pair
- **Right shoulder:** blaster arm wiring loom (5 MOSFET-driven channels from PCA9685)
- **Left shoulder:** power fist wiring (1 PCA9685 channel)
- **Neck:** head assembly wiring loom (eyes, sensor, mouth, brain — up to 6 PCA9685 channels)

All connectors terminate inside the torso at the central electronics hub.

### 8.2 Design Principle

No permanent solder joints exist between major assemblies. All soldering is internal to each body section (e.g., LED to resistor to wire within the arm). The connection between sections is always a plug-in Dupont connector. This enables disassembly for transport, maintenance, and troubleshooting without risk of damaging components.

### 8.3 Future PCB Consolidation

When the design matures to custom PCBs, the torso PCB will include pin headers along its edge — one header per zone/body section. Each body section's Dupont connector plugs directly into its corresponding header. The PCB consolidates the ESP32-S3 socket, PCA9685 IC, MOSFET circuits for high-current channels, series resistors, and power distribution (5V bus from buck converter, common GND) onto a single board. Direct-bus zones like the tank fire kit bypass the PCA9685 and connect straight to 5V/GND pins on the board. The result is a clean, professional central hub that replaces the prototype breadboard wiring.

---

## 9. Torso Cavity and Component Layout

### 9.1 Cavity Dimensions

The torso cavity is an irregular shape, viewed from two perspectives:

**Top-down profile:**

Main rectangular area: 63mm wide × 78mm deep. A centered notch extends from one side, 27mm wide, bringing the total width to 110mm at that point. The overall footprint is a T-shape or cross when viewed from above.

**Side profile:**

78mm deep × 45mm tall. The front-left corner has a stepped cutout: the wall rises 25mm from the floor, steps inward 20mm, then rises another 20mm to the ceiling. This reduces usable height to 25mm in that front-left zone. A curved armor plate covers the step from outside. The full 45mm height is available from the step inward to the back wall. The battery must be positioned in the full-height zone (right of the step) where there is clearance to stack it above the electronics tray. Avoid using the cutout corner for components — the angle is not a clean 90 degrees.

### 9.2 Access and Wire Exits

- **Top access:** the main armor plate on top of the model is removable, held in place by magnets. This provides full access to the cavity from above for battery swaps and maintenance.
- **Shoulder exits:** 10mm diameter holes on both side walls of the cavity for routing wires to the blaster arm (right) and power fist (left). The center of each hole is approximately 30mm above the cavity floor.
- **Pelvis exit:** hole through the center of the cavity floor for routing the tank fire kit wiring down through the right leg.
- **Neck exit:** hole in the front-right of the cavity floor, routing through the neck for head assembly wiring. These floor openings can be repositioned if the component layout requires it.

### 9.3 Prototype Mounting Tray

A 3D-printed mounting tray will be designed to hold all electronics (ESP32-S3, PCA9685, BMS, buck converter) in fixed positions. The tray drops into the cavity as a single unit with clip-in or friction-fit component slots and wire routing channels between them. Zone Dupont connector headers face outward toward their respective wire exits (blaster/power fist headers toward shoulder holes, head header toward neck hole, tank fire header toward pelvis hole).

The battery sits on top of the tray, accessible immediately when the magnetic armor plate is removed. Battery removal: lift armor plate → unplug T-connector → lift battery out. Electronics service: unplug zone connectors → lift entire tray out as one unit.

Note: the mounting tray is for the prototype phase. When the design matures to a custom PCB (see Section 8.3), the PCB replaces the tray and mounts directly into the cavity.

### 9.4 Central Junction

The mounting tray includes a small perfboard or proto-board that serves as the central power and signal junction. This board provides the common 5V rail (output from the buck converter), common GND bus, and Dupont pin headers for each zone connector. In the prototype phase, the MOSFET driver circuits for the blaster channels can also be built on this junction board. Zone headers face outward toward their respective wire exits. The T-connector from the battery connects to the BMS, which feeds the latching switch, which feeds the buck converter, which outputs 5V to the junction board.

### 9.5 Component Fit Summary

| Component | Dimensions (mm) | Placement | Notes |
|---|---|---|---|
| Battery | 73 × 34 × 18.5 | Top of stack | Sits on tray, lifts out from above |
| ESP32-S3 Super Mini | 22.5 × 18 | Tray floor | Very compact, fits anywhere |
| PCA9685 | ~25 × 62 | Tray floor | Main rectangular area |
| BMS | 41 × 16 × 3.5 | Tray floor or wall | Very thin, tucks anywhere |
| Buck converter | ~22 × 17 (MP1584) | Tray floor | Size may increase if upgraded to 5A module |

Total tray footprint fits within the 63mm × 78mm main area with the 27mm notch available for overflow. The battery must sit in the full-height zone (right of the front-left step) where 45mm of height is available. Electronics on the tray floor (approximately 20–25mm including boards and wiring clearance) plus the battery on top (18.5mm) totals approximately 40–43mm, leaving approximately 8mm clearance to the magnetic armor plate.

---

## 10. Electrical Notes

- **Eye fiber optic feed:** 1x 5mm green through-hole LED (Arduino starter kit), 150Ω series resistor at 5V, approximately 20mA. Direct PCA9685 drive
- **Sensor eye:** 1x 3V red Evan Designs chip LED with 100Ω inline resistor for 5V compatibility, approximately 20mA. Direct PCA9685 drive
- **Mouth:** 1x 5V green Evan Designs chip LED (to be purchased), pre-wired with resistor for 5V. Direct PCA9685 drive
- **Brain:** LED count and type TBD. MOSFET requirement depends on final count per channel (1 LED = direct drive; 2+ LEDs in parallel = likely needs MOSFET). Uses same IRLML6344 circuit as blaster if MOSFETs are needed
- **Power fist:** LED type and count TBD. 100Ω series resistor per LED at 5V as baseline
- **Tank fire kit:** Evan Designs 5–12V version, self-contained with built-in resistors and flicker circuits. Connects directly to 5V/GND bus. Estimated draw approximately 60mA (3 LEDs at approximately 20mA each)
- **LED rope segments:** 1.5mm overall diameter including green silicone diffusion sleeve. Forward voltage 2.7–3.1V (confirmed from packaging), maximum current 300mA (confirmed from packaging — Amazon listing shows 30mA which is a per-segment test condition, not maximum). On hand — two lengths from same supplier, same luminosity. Cannot be cut; insert designs will be modelled around fixed rope lengths. Requires MOSFET driver per channel (see Section 5.4)
- **Blaster MOSFET driver:** IRLML6344 (SOT-23, logic-level N-channel) per channel, with 10kΩ gate pull-down resistor. Series current-limiting resistor: 6.8Ω 1W (max brightness, approximately 294mA) or 8.2Ω 1W (derated, approximately 244mA). 5 MOSFET circuits total for blaster channels
- **Battery connector:** T-connector (Deans) rated for high current. Eliminates the JST undersizing concern from earlier revisions. Peak system draw of approximately 2A is well within T-connector ratings
- **BMS:** JZK 2S 7.4V 8A board confirmed. Over-charge 4.25–4.35V, over-discharge 2.5–3.0V, short circuit protection. 8A continuous rating provides 4x headroom over estimated 2A peak draw
- **Buck converter:** MP1584 (2–3A) acceptable for prototyping. Recommend upgrading to a 5A-rated module for the final build to provide expansion headroom. Verify specific board rating before purchase — some MP1584 boards are only 2A
- **Wire gauge — 7.4V side** (battery to BMS to switch to buck converter input): 14 AWG stranded silicone wire. Carries full system current at battery voltage. Pre-wired T-connector leads are typically 14 AWG
- **Wire gauge — 5V side** (buck converter output through bus, junction board, ESP32-S3, PCA9685, and all zone runs): 22 AWG stranded hookup wire. Rated for approximately 3A, provides headroom over any single run's current demand

---

## 11. Core Power System — Bill of Materials

Components required to prototype the core power delivery and control system. This covers everything needed to build the drop-in tray with a functioning 5V bus, PWM control, and battery power. Zone-specific components (LEDs, resistors, MOSFETs, LED rope) are listed separately when each zone is prototyped.

| Component | Product | Qty | Source | Notes |
|---|---|---|---|---|
| **Battery** | Zeee 2S 2200mAh 7.4V 50C Shorty LiPo, T-connector (2-pack) | 1 pack | Amazon | 73×34×18.5mm. Spare included for swap-out during extended display |
| **Balance charger** | SUPULSE B3 Pro 25W 2S/3S LiPo balance charger | 1 | Amazon | AC wall plug, JST-XH balance port, 2000mA charge rate. Red/green LED status. Charges battery externally |
| **LiPo safety bag** | Fireproof LiPo charging/storage bag | 1 | Amazon | Always charge and store LiPo batteries inside a fireproof bag |
| **BMS** | JZK 2S 7.4V 8A BMS protection board (5-pack) | 1 pack | Amazon | 41×16×3.5mm. Spares included |
| **Buck converter** | EBOOT MP1584EN DC-DC 3A adjustable buck converter (6-pack) | 1 pack | Amazon | 22×17×4mm. Set to 5V with potentiometer + multimeter before connecting anything. Spares included |
| **Microcontroller** | Waveshare ESP32-S3 Mini (pre-soldered headers) | 2 | Amazon | Dual-core 240MHz, WiFi, BLE 5, USB-C. One for breadboard prototyping, one for final build |
| **PWM driver** | SunFounder PCA9685 16-channel 12-bit PWM driver board | 1 | Amazon | I2C, 5V logic compatible with 3.3V ESP32. Check for built-in series resistors on outputs |
| **T-connectors** | T-connector (Deans) male/female pairs, pre-wired 14 AWG (multi-pack) | 1 pack | Amazon | One pair for battery connection. Harvest 14 AWG wire from spare pairs for BMS-to-buck-converter runs |
| **Power switch** | Twidec L-PBS-110-XBK latching push button (4-pack) | 1 | Amazon | 7mm latching SPST, 3A 250V AC, pre-soldered wires. Mounted through torso exterior rectangular housing. Spares included in 4-pack |
| **Breadboard wire** | TUOFENG 22 AWG solid core hookup wire (6-color kit) | 1 kit | Amazon | Solid core for breadboard prototyping only. 30ft per color in dispensing box |
| **Breadboard** | Full-size solderless breadboard (830 tie points) | 1 | Amazon | Existing 300-point board is too small for ESP32 + PCA9685 + buck converter together |
| **Test LED + resistor** | 5mm green LED + 150Ω resistor | 1 each | On hand | From Arduino starter kit. Verifies PCA9685 output during core power-on test |

Tools assumed on hand: soldering iron, solder, wire strippers, multimeter, small screwdriver (for buck converter potentiometer adjustment).

Deferred purchases (buy when moving from breadboard to permanent build): perfboard for junction board, male pin header strips, 22 AWG stranded silicone wire (for final model wiring — the solid core TUOFENG wire is for breadboard only).

### 11.2 Blaster Zone — Bill of Materials

Components required to prototype the blaster arm zone (PCA9685 channels 5–9). Order alongside or immediately after the core power system BOM. LED rope is already on hand.

| Component | Product | Qty | Source | Notes |
|---|---|---|---|---|
| **MOSFET** | IRLML6344TRPBF (Infineon, SOT-23) | 10–20 | Digi-Key | N-channel logic-level MOSFET, SOT-23. Fully switches at 3.3V gate, handles >4A drain. Buy genuine from Digi-Key — counterfeits common on Amazon. Digi-Key part: IRLML6344TRPBF. Buy 10–20 (tiny, easy to lose) |
| **Series resistor (blaster)** | TE Connectivity ROX1SJ6R8 (6.8Ω 1W metal oxide axial) | 10 | Digi-Key | One per blaster channel, 5 needed. 1W rating required — actual dissipation ~0.59W at 294mA. Digi-Key part: ROX1SJ6R8. Buy 10 for spares |
| **Gate pull-down resistor** | 10kΩ ¼W carbon film | 5 | On hand | From Arduino starter kit (brown/black/orange/gold = 10kΩ ¼W). Dissipation only ~2.5mW — ¼W rating is ample |
| **Resistor assortment** | BOJACK 1000pcs 25-value ¼W assortment (B08FD1XVL6) | 1 | Amazon | Includes 100Ω and 150Ω needed for head zones (sensor eye and fiber optic LED). Covers all remaining ¼W resistor values for the full build |
| **LED rope** | Green silicone-sleeved LED rope, 1.5mm | 2 | On hand | Two fixed-length ropes, same supplier, same luminosity. Vf 2.7–3.1V, max 300mA confirmed from packaging. Cannot be cut — barrel and heatsink inserts will be designed around rope lengths. Measure actual Vf under load during prototyping and adjust series resistor if needed |

---

## 12. Prototyping Plan

Each lighting zone will be prototyped individually on breadboard with the PCA9685 and ESP32-S3 before integration. Effects will be evaluated visually and refined before committing to final wiring. The ESP32-S3 web interface allows real-time adjustment of effects during prototyping. Once all zones are validated, the design may be matured into custom PCBs.

### 12.1 Core Power-On Test (First Prototype)

Goal: verify the entire power chain from battery to visible LED output on a breadboard.

1. Set up the buck converter: connect a bench supply or the LiPo (through BMS) to the MP1584 input. Adjust the potentiometer with a multimeter on the output until it reads 5.0V. Do this before connecting anything else.
2. Wire the breadboard: connect buck converter 5V output to the breadboard's red power rail, GND to the blue power rail. These rails are the 5V/GND bus for prototyping.
3. Plug in the ESP32-S3: connect its VIN pin to the 5V rail, GND to the GND rail. If it boots (onboard LED activity, USB serial output), the power chain works.
4. Plug in the PCA9685: connect VCC to 5V rail, GND to GND rail. Wire SDA and SCL from the ESP32-S3 to the PCA9685.
5. Connect a test LED: wire a 5mm green LED with a 150Ω resistor from one PCA9685 output channel to GND.
6. Upload a test sketch: a simple program that fades the LED up and down on that channel using the Adafruit PWM library. If the LED fades smoothly, the entire core system is validated — battery, BMS, buck converter, ESP32-S3, I2C bus, and PCA9685 PWM output all working together.
7. Test WiFi: extend the sketch to host a basic web page with a brightness slider. If you can control the LED brightness from your phone, remote control is validated.

### 12.2 Zone Prototyping Order

1. Blaster arm (most complex — 5 channels, multi-state firing sequence, LED rope characterisation)
2. Brain (3-channel organic pulse, sine wave tuning)
3. Head fiber optics (static, simplest zone)
4. Power fist (single channel pulse)
5. Tank fire kit (direct bus, verify inrush current)
6. Full integration and state machine coding

---

## 13. Expansion Headroom

PCA9685 channels 12–15 are unassigned and available for future lighting zones such as wing lights, base effects, trophy glow, or additional weapon effects. A second PCA9685 can be daisy-chained on the same I2C bus if more than 16 total channels are ever needed.

All GPIO pins on the ESP32-S3 are free for input use (buttons, sensors, mode switches) since no PWM output is handled by the microcontroller directly. The ESP32-S3's WiFi capability also enables future features such as OTA (over-the-air) firmware updates, integration with home automation systems, or synchronised effects across multiple models.

---

## Revision Log

| Version | Date | Changes |
|---|---|---|
| 1.0 | Apr 2026 | Initial design brief — Arduino Uno, 6 native PWM channels, blaster as single static glow zone |
| 2.0 | Apr 2026 | Replaced Uno with Nano. Added PCA9685 PWM driver (16 channels, 12-bit, I2C). Expanded blaster to 5 channels (barrels + 4 heatsink zones). Defined multi-state firing sequence (idle/prime/charge/discharge). Documented LED rope specs and heatsink zone grouping. Added prototyping plan |
| 2.1 | Apr 2026 | Confirmed LED rope electrical specs (2.7–3.0V Vf, 300mA max). Added MOSFET driver circuit design (IRLML6344) for all 5 blaster channels. Defined series resistor values for max brightness (6.8Ω) and derated (8.2Ω) options. Updated JST connector current concerns — may need XT30 upgrade. PCA9685 supplier confirmed as SunFounder |
| 3.0 | Apr 2026 | Complete head redesign. Eyes changed to fiber optic (4 strands from 1x 5mm green LED, 150Ω, Ch 0) with warm-up ramp on power-on. Added red sensor eye (3V Evan Designs chip + 100Ω, Ch 10) with periodic blink. Added mouth zone (5V green Evan Designs chip, Ch 11) with organic breathing rhythm, loosely related to brain tempo. Brain LED count left as open item pending print design and diffusion prototyping. Defined head startup sequence (eyes → brain → mouth). Expanded electrical notes with per-zone LED specs. 12 of 16 PCA9685 channels now allocated |
| 3.1 | Apr 2026 | Expanded tank fire section with full wire routing plan (tank → foot → leg channel → pelvis → torso). Fire kit to be repurchased as 5–12V version with 14-inch wire. Right leg to be reprinted with hollow wire channel (5–6mm ID, oversized for future use). Left leg solid. Added Section 8: Connector Strategy — Dupont connectors at all major body joints (pelvis, shoulders, neck) for modular assembly. Added PCB consolidation roadmap (Section 8.3). Tank stands as base for right foot, no separate model base |
| 3.2 | Apr 2026 | Power system fully specified. Battery: Zeee 2S 2200mAh 50C Shorty LiPo (73×34×18.5mm, T-connector, 2-pack). BMS: JZK 2S 7.4V 8A (41×16×3.5mm, over-charge/discharge/short circuit protection). Battery connector changed from JST to T-connector (Deans) — resolves previous current rating concern. Added runtime estimates, charger requirement note, and updated architecture summary. Firing sequence revised: energy now originates at rear reactor/heatsink and propagates forward, rear zone never fully dark |
| 4.0 | Apr 2026 | Added Section 9: Torso Cavity and Component Layout. Documented cavity dimensions (63×78mm main area, 27mm centered notch to 110mm total width, 45mm height with stepped shelf). Magnetic removable top armor plate for access. Wire exits: 10mm shoulder holes at 30mm height on both sides, pelvis hole center floor, neck hole front-right floor. 3D-printed drop-in mounting tray concept for prototype phase. Component fit analysis confirms all boards plus battery within 45mm height ceiling. Buck converter spec updated: recommend 5A-rated module for expansion headroom (MP1584 acceptable for prototyping only) |
| 4.1 | Apr 2026 | Corrected torso cavity side profile: front-left corner is a stepped cutout (wall rises 25mm, steps inward 20mm, then rises 20mm to ceiling), not a downward shelf. Usable height is 25mm in cutout zone, full 45mm elsewhere. Battery must sit in full-height zone. Added Section 9.4: Central Junction (perfboard with 5V/GND bus and zone pin headers). Added Section 11: Core Power System BOM with all components needed to prototype the drop-in tray. Tools assumed on hand |
| 5.0 | Apr 2026 | Major architecture change: replaced Arduino Nano with ESP32-S3 Super Mini (22.52×18mm, dual-core 240MHz, WiFi, BLE). Enables full remote control via web browser — no app needed. Added Section 3.3: Remote Control (web UI with zone toggles, brightness sliders, firing triggers, display mode presets). Physical power switch removed; battery is always unplugged at T-connector when not in use. Blaster trigger method updated to web interface. Updated BOM, component fit table, runtime estimates (higher ESP32 power draw), and all Nano references throughout. Expansion section updated with OTA and multi-model sync possibilities |
| 5.3 | Apr 2026 | Power switch specified: Twidec L-PBS-110-XBK 7mm latching push button, mounted through existing torso exterior rectangular housing feature. Indicator LED planned for domed feature above switch, wired to +SWITCHED net. LED rope confirmed on hand: 1.5mm overall diameter with green silicone diffusion sleeve, Vf 2.7–3.1V, max 300mA, two fixed-length ropes from same supplier. Cannot be cut — insert designs will be modelled around rope lengths. LED rope specs updated throughout (sections 5.1, 5.2, 10). Blaster zone BOM added as Section 11.2. Resistor assortment (BOJACK B08FD1XVL6, 25 values 1/4W) added to core BOM. Status updated to prototyping phase |

---

*Emperor's Children — Slaanesh Daemon-Touched Build*