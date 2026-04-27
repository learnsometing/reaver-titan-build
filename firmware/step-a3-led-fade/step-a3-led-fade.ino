#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  pwm.begin();
  pwm.setPWMFreq(1000);
  Serial.println("PCA9685 ready.");
}

void loop() {
  // Fade up
  for (int i = 0; i < 4096; i += 10) {
    pwm.setPWM(0, 0, i);
    delay(5);
  }
  // Fade down
  for (int i = 4095; i >= 0; i -= 10) {
    pwm.setPWM(0, 0, i);
    delay(5);
  }
}
