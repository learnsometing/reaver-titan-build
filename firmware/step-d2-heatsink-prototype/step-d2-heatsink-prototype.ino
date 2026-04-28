#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <WebServer.h>
#include "../ui/ui.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
WebServer server(80);

const char* AP_SSID = "Reaver-Titan";
const char* AP_PASS = "warmaster40k";

#define CH_REAR     6
#define CH_MID_REAR 7
#define IDLE_LEVEL  400
#define FULL_LEVEL  4095

enum State { IDLE, PRIME, FULL_CHARGE, DISCHARGE };
State currentState = IDLE;
uint32_t stateStart = 0;

void setChannel(uint8_t ch, uint16_t val) {
  pwm.setPWM(ch, 0, val);
}

void enterState(State s) {
  currentState = s;
  stateStart = millis();
  const char* names[] = { "IDLE", "PRIME", "FULL CHARGE", "DISCHARGE" };
  Serial.println(names[s]);
  if (s == IDLE) {
    setChannel(CH_REAR, IDLE_LEVEL);
    setChannel(CH_MID_REAR, 0);
  }
}

void handleRoot() {
  server.send(200, "text/html", INDEX_HTML);
}

void handleBlasterState() {
  String s = server.arg("state");
  if      (s == "idle")      enterState(IDLE);
  else if (s == "prime")     enterState(PRIME);
  else if (s == "charge")    enterState(FULL_CHARGE);
  else if (s == "discharge") enterState(DISCHARGE);
  server.send(200, "text/plain", "OK");
}

void handleBrightness() {
  server.send(200, "text/plain", "OK");
}

void handleZone() {
  server.send(200, "text/plain", "OK");
}

void updateStateMachine() {
  uint32_t elapsed = millis() - stateStart;

  switch (currentState) {
    case IDLE:
      break;

    case PRIME: {
      float t = min(1.0f, elapsed / 1200.0f);
      setChannel(CH_REAR,     IDLE_LEVEL + (uint16_t)((FULL_LEVEL - IDLE_LEVEL) * t));
      setChannel(CH_MID_REAR, t > 0.4f ? (uint16_t)(FULL_LEVEL * (t - 0.4f) / 0.6f) : 0);
      break;
    }

    case FULL_CHARGE: {
      float t = elapsed / 1000.0f;
      uint16_t level = 3600 + (uint16_t)(495.0f * (0.5f + 0.5f * sin(t * 2.0f * PI * 1.3f)));
      setChannel(CH_REAR, level);
      setChannel(CH_MID_REAR, level);
      break;
    }

    case DISCHARGE: {
      // 0–80ms:    both at full (rear pulse)
      // 80–280ms:  both at full (mid-rear joins)
      // 280–980ms:  CH7 fades FULL→0 (700ms)
      // 980–2380ms: CH6 fades FULL→IDLE (1400ms)
      // >2380ms:   return to IDLE
      if (elapsed < 80) {
        setChannel(CH_REAR, FULL_LEVEL);
        setChannel(CH_MID_REAR, FULL_LEVEL);
      } else if (elapsed < 280) {
        // hold — both already at FULL from previous frames
      } else if (elapsed < 980) {
        float t = (elapsed - 280) / 700.0f;
        setChannel(CH_MID_REAR, (uint16_t)(FULL_LEVEL * (1.0f - t)));
      } else if (elapsed < 2380) {
        float t = (elapsed - 980) / 1400.0f;
        setChannel(CH_REAR, FULL_LEVEL - (uint16_t)((FULL_LEVEL - IDLE_LEVEL) * t));
        setChannel(CH_MID_REAR, 0);
      } else {
        enterState(IDLE);
      }
      break;
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  pwm.begin();
  pwm.setPWMFreq(1000);

  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/brightness", handleBrightness);
  server.on("/zone/head",  HTTP_POST, handleZone);
  server.on("/zone/torso", HTTP_POST, handleZone);
  server.on("/blaster/state", HTTP_POST, handleBlasterState);
  server.begin();
  Serial.println("Web server ready.");

  enterState(IDLE);
}

void loop() {
  server.handleClient();
  updateStateMachine();
}
