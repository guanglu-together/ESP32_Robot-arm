/* -------------------------------------------------
Copyright (c)
Arduino project by Tech Talkies YouTube Channel.
https://www.youtube.com/@techtalkies1
-------------------------------------------------*/

#include <WiFi.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include "web.h" // Your HTML file

const char* ssid = "ESP32 Robot";
const char* password = "12345678";

WebServer server(80);

// Your ESP32-S3 DevKitC-1 GPIOs
static const int servoPinLR = 15;
static const int servoPinFB = 16;
static const int servoPinUD = 17;
static const int servoPinGrip = 18;

int lr_target = 90;
int fb_target = 90;
int ud_target = 90;
int grip_target = 0;

int lr_current = 90;
int fb_current = 90;
int ud_current = 90;
int grip_current = 0;

const int GRIP_OPEN = 180;
const int GRIP_CLOSE = 60;

const int STEP_ARM = 2;   // smooth arm motion
const int STEP_GRIP = 5;  // faster gripper

// --- NEW: Helper function for native ESP32 Core 3.x PWM ---
void setServoAngle(int pin, int angle) {
  angle = constrain(angle, 0, 180);
  // Standard micro servo pulse width: 500us to 2400us
  long pulse_us = map(angle, 0, 180, 500, 2400); 
  // Calculate duty cycle for 12-bit resolution (0-4095) and 50Hz (20,000us)
  uint32_t duty = (pulse_us * 4095) / 20000; 
  ledcWrite(pin, duty);
}
// ----------------------------------------------------------

void handleServo() {
  if (server.hasArg("lr"))
    lr_target = constrain(server.arg("lr").toInt(), 0, 180);

  if (server.hasArg("fb"))
    fb_target = constrain(server.arg("fb").toInt(), 0, 180);

  if (server.hasArg("ud"))
    ud_target = constrain(server.arg("ud").toInt(), 0, 180);

  if (server.hasArg("grip")) {
    int g = server.arg("grip").toInt();
    grip_target = (g == 1) ? GRIP_CLOSE : GRIP_OPEN;
  }

  Serial.printf("T -> lr:%d fb:%d ud:%d grip:%d\n",
                lr_target, fb_target, ud_target, grip_target);

  // Quick ACK
  server.send(200, "text/plain", "ok");
}

void setup() {
  Serial.begin(115200);

  // --- NATIVE ESP32 CORE 3.x PWM SETUP ---
  // ledcAttach automatically assigns independent channels
  ledcAttach(servoPinLR, 50, 12);
  ledcAttach(servoPinFB, 50, 12);
  ledcAttach(servoPinUD, 50, 12);
  ledcAttach(servoPinGrip, 50, 12);

  // Set initial positions to prevent sudden jerking
  setServoAngle(servoPinLR, lr_current);
  setServoAngle(servoPinFB, fb_current);
  setServoAngle(servoPinUD, ud_current);
  setServoAngle(servoPinGrip, grip_current);
  // ---------------------------------------

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  delay(1000);
  
  IPAddress IP = IPAddress(10, 10, 10, 1);
  IPAddress NMask = IPAddress(255, 255, 255, 0);
  WiFi.softAPConfig(IP, IP, NMask);
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  /* SETUP YOUR OWN WEB ENTRY POINTS */
  server.on("/servo", handleServo);
  
  // Route for root / web page
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", index_html);
  });
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  delay(2); // allow the cpu to switch to other tasks

  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate >= 20) {
    lastUpdate = millis();

    // Left / Right
    if (lr_current < lr_target) lr_current += STEP_ARM;
    else if (lr_current > lr_target) lr_current -= STEP_ARM;
    if (abs(lr_current - lr_target) < STEP_ARM) lr_current = lr_target;

    // Forward / Backward
    if (fb_current < fb_target) fb_current += STEP_ARM;
    else if (fb_current > fb_target) fb_current -= STEP_ARM;
    if (abs(fb_current - fb_target) < STEP_ARM) fb_current = fb_target;

    // Up / Down
    if (ud_current < ud_target) ud_current += STEP_ARM;
    else if (ud_current > ud_target) ud_current -= STEP_ARM;
    if (abs(ud_current - ud_target) < STEP_ARM) ud_current = ud_target;

    // Gripper
    if (grip_current < grip_target) grip_current += STEP_GRIP;
    else if (grip_current > grip_target) grip_current -= STEP_GRIP;
    if (abs(grip_current - grip_target) < STEP_GRIP) grip_current = grip_target;

    // ---- ACTUAL SERVO OUTPUTS HERE ----
    setServoAngle(servoPinLR, lr_current);
    setServoAngle(servoPinFB, fb_current);
    setServoAngle(servoPinUD, ud_current);
    setServoAngle(servoPinGrip, grip_current);
  }
}