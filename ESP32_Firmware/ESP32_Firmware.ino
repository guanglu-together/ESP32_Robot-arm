#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include "web.h"
#include <WiFiUdp.h>

WiFiUDP udp;
const int udpPort = 8888;


// ⚠️ Wi-Fi ID and Password
const char* ssid = "";     
const char* password = ""; 

WebServer server(80);

// --- 动作录制与回放系统 ---
struct KeyFrame {
  int lr, fb, ud, grip;
};

const int MAX_FRAMES = 200; 
KeyFrame timeline[MAX_FRAMES];
int frameCount = 0;

enum Mode { MODE_NORMAL, MODE_RECORD, MODE_PLAY };
Mode currentMode = MODE_NORMAL;

unsigned long lastSampleTime = 0;
const int SAMPLE_INTERVAL = 100; 

int playIndex = 0;
unsigned long playStepTime = 0;
// ------------------------------

static const int servoPinLR = 4;
static const int servoPinFB = 5;
static const int servoPinUD = 6;
static const int servoPinGrip = 7;

Servo servoLR, servoFB, servoUD, servoGrip;

const int GRIP_OPEN = 170;
const int GRIP_CLOSE = 60;

int lr_target = 90;
int fb_target = 90;
int ud_target = 90;
int grip_target = GRIP_OPEN; 

int lr_current = 90;
int fb_current = 90;
int ud_current = 90;
int grip_current = GRIP_OPEN;

const int STEP_ARM = 2;   
const int STEP_GRIP = 5;  

void handleServo() {
  if (currentMode == MODE_PLAY) {
    server.send(200, "text/plain", "playing");
    return; 
  }

  if (server.hasArg("lr")) lr_target = constrain(server.arg("lr").toInt(), 0, 180);
  if (server.hasArg("fb")) fb_target = constrain(server.arg("fb").toInt(), 0, 180);
  if (server.hasArg("ud")) ud_target = constrain(server.arg("ud").toInt(), 0, 180);
  if (server.hasArg("grip")) {
    int g = server.arg("grip").toInt();
    grip_target = (g == 1) ? GRIP_CLOSE : GRIP_OPEN;
  }

  if (currentMode == MODE_RECORD) {
    unsigned long now = millis();
    if (now - lastSampleTime >= SAMPLE_INTERVAL) {
      if (frameCount < MAX_FRAMES) {
        timeline[frameCount].lr = lr_target;
        timeline[frameCount].fb = fb_target;
        timeline[frameCount].ud = ud_target;
        timeline[frameCount].grip = grip_target;
        frameCount++;
        lastSampleTime = now;
      } else {
        Serial.println("Memory full! Auto-stopping record.");
        currentMode = MODE_NORMAL; 
      }
    }
  }
  server.send(200, "text/plain", "ok");
}

void handleAction() {
  if (server.hasArg("cmd")) {
    String cmd = server.arg("cmd");
    if (cmd == "start_rec") {
      currentMode = MODE_RECORD;
      frameCount = 0; 
      lastSampleTime = millis();
      Serial.println(">>> Recording Started <<<");
    } 
    else if (cmd == "stop_rec") {
      currentMode = MODE_NORMAL;
      Serial.printf(">>> Recording Stopped. Total Frames: %d <<<\n", frameCount);
    } 
    else if (cmd == "play") {
      if (frameCount > 0) {
        currentMode = MODE_PLAY;
        playIndex = 0;
        playStepTime = millis();
        Serial.println(">>> Playback Started <<<");
      } else {
        Serial.println("No frames to play!");
      }
    }
  }
  server.send(200, "text/plain", "ok");
}

// ================= 【新增：视觉坐标处理接口】 =================
void handleVision() {
  // 如果正在录制或回放，视觉信号强制接管控制权，打断当前动作
  if (currentMode != MODE_NORMAL) {
    currentMode = MODE_NORMAL; 
  }

  if (server.hasArg("x") && server.hasArg("y")) {
    int pixelX = server.arg("x").toInt(); // 0 ~ 640
    int pixelY = server.arg("y").toInt(); // 0 ~ 480
    
    // 🧠 核心算法：像素坐标 -> 舵机角度映射 (线性映射)
    // X轴 (左右)：假设画面左边对应舵机 30度，右边对应 150度
    int lr_angle = map(pixelX, 0, 640, 30, 150); 
    
    // Y轴 (前后)：假设画面上边(远)对应 60度，下边(近)对应 140度
    int fb_angle = map(pixelY, 0, 480, 60, 140); 
    
    lr_target = constrain(lr_angle, 30, 150);
    fb_target = constrain(fb_angle, 60, 140);
    
    ud_target = 90; // 保持平视高度
    grip_target = GRIP_OPEN; // 保持张开

    server.send(200, "text/plain", "ok");
  }
}
// ========================================================

void setup() {
  Serial.begin(115200);
  
  // ================= 1. 连接家里路由器/手机热点 (STA 模式) =================
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  // ================= 2. 获取并打印真实 IP =================
  IPAddress myIP = WiFi.localIP(); 
  Serial.print("STA IP address: ");
  Serial.println(myIP);

  // ================= 3. 启动 mDNS =================
  if (MDNS.begin("robot-arm")) { 
    Serial.println("mDNS responder started. Access via: http://robot-arm.local");
    MDNS.addService("http", "tcp", 80); 
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  // ================= 4. 配置 WebServer =================
  server.on("/servo", handleServo);
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", index_html);
  });
  server.on("/action", handleAction); 
  server.on("/vision", handleVision); // 【新增】注册视觉接口

  // ================= 5. OTA 无线升级配置 =================
  ArduinoOTA.setHostname("robot-arm"); 

  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  
  ArduinoOTA.begin();
  Serial.println("OTA Ready!");
  // ========================================================

  server.begin();
  Serial.println("HTTP server started");

  // ================= 6. 初始化舵机 =================
  servoLR.attach(servoPinLR);
  servoFB.attach(servoPinFB);
  servoUD.attach(servoPinUD);
  servoGrip.attach(servoPinGrip);
  
  udp.begin(udpPort);
  Serial.print("UDP监听已启动，端口：");
  Serial.println(udpPort);

}

void loop() {
  ArduinoOTA.handle(); 
    // ================= 【新增：处理 UDP 视觉控制包】 =================
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char incomingPacket[255];
    int len = udp.read(incomingPacket, 254);
    if (len > 0) {
      incomingPacket[len] = 0; // 添加字符串结束符
      // 解析 Python 发来的 "lr,fb" 格式数据 (例如 "90,120")
      String data = String(incomingPacket);
      int commaIndex = data.indexOf(',');
      if (commaIndex > 0) {
        lr_target = constrain(data.substring(0, commaIndex).toInt(), 30, 150);
        fb_target = constrain(data.substring(commaIndex + 1).toInt(), 60, 140);
        ud_target = 90; 
        grip_target = GRIP_OPEN;
      }
    }
  }
  // ========================================================
  server.handleClient();
  delay(2);  
  
  if (currentMode == MODE_PLAY) {
      if (playIndex < frameCount) {
        if (millis() - playStepTime >= SAMPLE_INTERVAL) {
          lr_target = timeline[playIndex].lr;
          fb_target = timeline[playIndex].fb;
          ud_target = timeline[playIndex].ud;
          grip_target = timeline[playIndex].grip;
          
          playIndex++;
          playStepTime = millis();
        }
      } else {
        currentMode = MODE_NORMAL;
        Serial.println(">>> Playback Finished <<<");
      }
  }

  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate >= 20) {
    lastUpdate = millis();

    if (lr_current < lr_target) lr_current += STEP_ARM;
    else if (lr_current > lr_target) lr_current -= STEP_ARM;
    if (abs(lr_current - lr_target) < STEP_ARM) lr_current = lr_target;

    if (fb_current < fb_target) fb_current += STEP_ARM;
    else if (fb_current > fb_target) fb_current -= STEP_ARM;
    if (abs(fb_current - fb_target) < STEP_ARM) fb_current = fb_target;

    if (ud_current < ud_target) ud_current += STEP_ARM;
    else if (ud_current > ud_target) ud_current -= STEP_ARM;
    if (abs(ud_current - ud_target) < STEP_ARM) ud_current = ud_target;

    if (grip_current < grip_target) grip_current += STEP_GRIP;
    else if (grip_current > grip_target) grip_current -= STEP_GRIP;
    if (abs(grip_current - grip_target) < STEP_GRIP) grip_current = grip_target;

    servoLR.write(lr_current);
    servoFB.write(fb_current);
    servoUD.write(ud_current);
    servoGrip.write(grip_current);
  }
}