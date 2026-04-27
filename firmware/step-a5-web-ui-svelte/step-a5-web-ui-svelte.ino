#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <WebServer.h>
#include "../ui/ui.h"

const char* AP_SSID = "Reaver-Titan";
const char* AP_PASS = "warmaster40k";

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
WebServer server(80);

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

void handleZone() {
  String zone = server.uri().substring(6); // strip "/zone/"
  String state = server.arg("state");
  Serial.printf("zone/%s -> %s\n", zone.c_str(), state.c_str());
  server.send(200, "text/plain", "OK");
}

void handleBlasterState() {
  String state = server.arg("state");
  Serial.printf("blaster/state -> %s\n", state.c_str());
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
  server.on("/zone/head", HTTP_POST, handleZone);
  server.on("/zone/torso", HTTP_POST, handleZone);
  server.on("/blaster/state", HTTP_POST, handleBlasterState);
  server.begin();
  Serial.println("Web server ready.");
}

void loop() {
  server.handleClient();
}
