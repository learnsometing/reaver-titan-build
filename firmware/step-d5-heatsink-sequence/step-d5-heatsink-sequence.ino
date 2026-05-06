#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <WebServer.h>
#include <math.h>

// Heatsink channels — rear to front
// Ch 0 (barrels) excluded from this prototype
#define CH_REAR      1
#define CH_MID_REAR  2
#define CH_MID       3
#define CH_MID_FRONT 4
#define CH_FRONT     5

#define IDLE_PWM  328    // ~8%
#define FULL_PWM  4095

const char* AP_SSID = "Reaver-Titan";
const char* AP_PASS = "warmaster40k";

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
WebServer server(80);

enum State { IDLE, PRIMING, FULL_CHARGE, DISCHARGING };
State state = IDLE;
unsigned long stateStart = 0;

void enterState(State s) {
  state = s;
  stateStart = millis();
}

void ch(int n, uint16_t val) {
  pwm.setPWM(n, 0, val);
}

uint16_t lerp16(uint16_t a, uint16_t b, float t) {
  if (t <= 0.0f) return a;
  if (t >= 1.0f) return b;
  return (uint16_t)((int)a + (int)((int)b - (int)a) * t);
}

// PRIME: overlapping forward ramp
// Each zone takes 600ms to reach full; next zone starts 300ms after previous
void tickPrime() {
  unsigned long t = millis() - stateStart;
  bool done = true;
  for (int i = 0; i < 5; i++) {
    long elapsed = (long)t - i * 300L;
    uint16_t val;
    if (elapsed <= 0)        { val = IDLE_PWM; done = false; }
    else if (elapsed >= 600) { val = FULL_PWM; }
    else                     { val = lerp16(IDLE_PWM, FULL_PWM, elapsed / 600.0f); done = false; }
    ch(i + 1, val);
  }
  if (done) enterState(FULL_CHARGE);
}

// FULL CHARGE: all zones oscillate 80–100%, hold 2s then auto-discharge
void tickFullCharge() {
  unsigned long t = millis() - stateStart;
  float osc = (sinf(2.0f * M_PI * t / 1500.0f) + 1.0f) / 2.0f;
  uint16_t val = lerp16(3276, FULL_PWM, osc);
  for (int i = 1; i <= 5; i++) ch(i, val);
  if (t >= 2000) enterState(DISCHARGING);
}

// DISCHARGE: staggered fade front-to-back
// Ch 5 fades first; Ch 1 fades last back to idle glow
void tickDischarge() {
  unsigned long t = millis() - stateStart;
  bool done = true;
  for (int i = 0; i < 5; i++) {
    int channel = 5 - i; // front first
    long elapsed = (long)t - i * 200L;
    uint16_t val;
    if (elapsed <= 0)        { val = FULL_PWM; done = false; }
    else if (elapsed >= 400) { val = IDLE_PWM; }
    else                     { val = lerp16(FULL_PWM, IDLE_PWM, elapsed / 400.0f); done = false; }
    ch(channel, val);
  }
  if (done) enterState(IDLE);
}

void tickIdle() {
  for (int i = 1; i <= 5; i++) ch(i, IDLE_PWM);
}

void updateAnimation() {
  switch (state) {
    case IDLE:        tickIdle();        break;
    case PRIMING:     tickPrime();       break;
    case FULL_CHARGE: tickFullCharge();  break;
    case DISCHARGING: tickDischarge();   break;
  }
}

const char* stateName() {
  switch (state) {
    case IDLE:        return "IDLE";
    case PRIMING:     return "PRIMING";
    case FULL_CHARGE: return "FULL CHARGE";
    case DISCHARGING: return "DISCHARGING";
    default:          return "UNKNOWN";
  }
}

const char INDEX_HTML[] PROGMEM = R"html(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: sans-serif; padding: 24px; max-width: 420px; margin: 0 auto; background: #111; color: #eee; }
    h2 { margin-bottom: 8px; }
    #state { font-size: 1.4em; color: #4caf50; font-weight: bold; margin-bottom: 32px; letter-spacing: 0.05em; }
    button { width: 100%; padding: 16px; font-size: 1.1em; border: none; border-radius: 6px; cursor: pointer; margin-bottom: 12px; }
    #fireBtn { background: #4caf50; color: #000; font-weight: bold; }
    #idleBtn { background: #333; color: #eee; }
  </style>
</head>
<body>
  <h2>Heatsink Sequence</h2>
  <div id="state">IDLE</div>
  <button id="fireBtn" onclick="fire()">FIRE</button>
  <button id="idleBtn" onclick="idle()">RESET TO IDLE</button>
  <script>
    function fire() { fetch('/fire'); }
    function idle() { fetch('/idle'); }
    setInterval(() => {
      fetch('/state').then(r => r.text()).then(s => {
        document.getElementById('state').textContent = s;
      });
    }, 200);
  </script>
</body>
</html>
)html";

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  pwm.begin();
  pwm.setPWMFreq(1000);

  tickIdle();

  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() { server.send(200, "text/html", INDEX_HTML); });
  server.on("/fire", []() {
    if (state == IDLE) enterState(PRIMING);
    server.send(200, "text/plain", "OK");
  });
  server.on("/idle", []() {
    enterState(IDLE);
    server.send(200, "text/plain", "OK");
  });
  server.on("/state", []() { server.send(200, "text/plain", stateName()); });
  server.begin();
  Serial.println("Ready.");
}

unsigned long lastTick = 0;

void loop() {
  server.handleClient();
  if (millis() - lastTick >= 20) {
    lastTick = millis();
    updateAnimation();
  }
}
