#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Channel under test — change to match whichever channel is wired on the bench
#define CHANNEL 0

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  pwm.begin();
  pwm.setPWMFreq(1000);
  pwm.setPWM(CHANNEL, 0, 4095);
  Serial.println("Channel " + String(CHANNEL) + " at full duty. Ready to measure.");
}

void loop() {}
