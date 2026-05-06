#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <WebServer.h>

// Ch A: single 6.8Ω  (~307mA peak)
// Ch B: 6.8Ω + 6.8Ω in series (~154mA peak)
#define CH_A 0
#define CH_B 1

const char* AP_SSID = "Reaver-Titan";
const char* AP_PASS = "warmaster40k";

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
WebServer server(80);

const char INDEX_HTML[] PROGMEM = R"html(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: sans-serif; padding: 24px; max-width: 420px; margin: 0 auto; background: #111; color: #eee; }
    h2 { margin-bottom: 24px; }
    .channel { margin-bottom: 28px; }
    .channel label { display: block; margin-bottom: 6px; font-size: 0.9em; color: #aaa; }
    .channel input[type=range] { width: 100%; accent-color: #4caf50; }
    .channel .readout { font-size: 1.1em; margin-top: 4px; }
    .pct { color: #4caf50; font-weight: bold; }
    .raw { color: #666; font-size: 0.85em; margin-left: 8px; }
  </style>
</head>
<body>
  <h2>Brightness Compare</h2>

  <div class="channel">
    <label>Ch A — 6.8Ω (~307mA peak)</label>
    <input type="range" min="0" max="4095" value="4095" oninput="set('a', this.value)">
    <div class="readout">
      <span class="pct" id="a-pct">100%</span>
      <span class="raw" id="a-raw">4095</span>
    </div>
  </div>

  <div class="channel">
    <label>Ch B — 13.6Ω (~154mA peak)</label>
    <input type="range" min="0" max="4095" value="4095" oninput="set('b', this.value)">
    <div class="readout">
      <span class="pct" id="b-pct">100%</span>
      <span class="raw" id="b-raw">4095</span>
    </div>
  </div>

  <script>
    function set(ch, val) {
      const pct = Math.round(val / 4095 * 100);
      document.getElementById(ch + '-pct').textContent = pct + '%';
      document.getElementById(ch + '-raw').textContent = val;
      fetch('/set?ch=' + ch + '&val=' + val);
    }
  </script>
</body>
</html>
)html";

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  pwm.begin();
  pwm.setPWMFreq(1000);
  pwm.setPWM(CH_A, 0, 4095);
  pwm.setPWM(CH_B, 0, 4095);

  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() {
    server.send(200, "text/html", INDEX_HTML);
  });

  server.on("/set", []() {
    String ch = server.arg("ch");
    uint16_t val = (uint16_t)constrain(server.arg("val").toInt(), 0, 4095);
    if (ch == "a") pwm.setPWM(CH_A, 0, val);
    else if (ch == "b") pwm.setPWM(CH_B, 0, val);
    server.send(200, "text/plain", "OK");
  });

  server.begin();
  Serial.println("Ready.");
}

void loop() {
  server.handleClient();
}
