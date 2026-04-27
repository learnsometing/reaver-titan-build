#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <WebServer.h>

const char* AP_SSID = "Reaver-Titan";
const char* AP_PASS = "warmaster40k";

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
WebServer server(80);

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Reaver Titan</title>
  <style>
    body { font-family: sans-serif; background: #111; color: #00ff00; text-align: center; padding: 2rem; }
    h1 { font-size: 1.5rem; margin-bottom: 2rem; }
    input[type=range] { width: 80%; accent-color: #00ff00; }
    #val { font-size: 1.2rem; margin-top: 0.5rem; }
  </style>
</head>
<body>
  <h1>Reaver Titan</h1>
  <p>Channel 0 Brightness</p>
  <input type="range" min="0" max="4095" value="0" oninput="setBrightness(this.value)">
  <p id="val">0</p>
  <script>
    function setBrightness(v) {
      document.getElementById('val').textContent = v;
      fetch('/brightness?v=' + v);
    }
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", INDEX_HTML);
}

void handleBrightness() {
  if (server.hasArg("v")) {
    int val = constrain(server.arg("v").toInt(), 0, 4095);
    pwm.setPWM(0, 0, val);
  }
  server.send(200, "text/plain", "OK");
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
  server.begin();
  Serial.println("Web server ready.");
}

void loop() {
  server.handleClient();
}
