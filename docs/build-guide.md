# Build Guide

*Reaver Titan Lighting System — Emperor's Children Build*

## Core Power-On Test (First Prototype)

Goal: verify the entire power chain from battery to visible LED output on a breadboard.

1. **Set up the buck converter.** Connect a bench supply or the LiPo (through BMS) to the MP1584 input. Adjust the potentiometer with a multimeter on the output until it reads 5.0V. Do this before connecting anything else.

2. **Wire the breadboard.** Connect buck converter 5V output to the breadboard's red power rail, GND to the blue power rail.

3. **Plug in the ESP32-S3.** Connect VIN to 5V rail, GND to GND rail. If it boots (onboard LED activity, USB serial output), the power chain works.

4. **Plug in the PCA9685.** Connect VCC to 5V rail, GND to GND rail. Wire SDA and SCL from the ESP32-S3.

5. **Connect a test LED.** Wire a 5mm green LED with a 150Ω resistor from one PCA9685 output channel to GND.

6. **Upload a test sketch.** A simple program that fades the LED up and down using the Adafruit PWM library. Smooth fading validates the entire core system — battery, BMS, buck converter, ESP32-S3, I2C bus, and PCA9685 PWM output.

7. **Test WiFi.** Extend the sketch to host a basic web page with a brightness slider. If you can control the LED brightness from your phone, remote control is validated.

---

## Zone Prototyping Order

1. Blaster arm — most complex: 5 channels, multi-state firing sequence, LED rope characterisation
2. Brain — 3-channel organic pulse, sine wave tuning
3. Head fiber optics — static, simplest zone
4. Power fist — single channel pulse
5. Tank fire kit — direct bus, verify inrush current
6. Full integration and state machine coding
