#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

// OLED配置
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define OLED_I2C_ADDRESS 0x3C

// 引脚定义
#define LIGHT_SENSOR_PIN 5     // 光敏电阻连接到 GPIO5
#define OLED_SDA_PIN 9         // OLED SDA 连接到 GPIO9
#define OLED_SCL_PIN 11        // OLED SCL 连接到 GPIO11

// WebSocket服务器端口
#define WS_PORT 81

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//logo
//logo
// 'E01213252520D649939DD4C8D0B32469', 128x32px
const unsigned char epd_bitmap_E01213252520D649939DD4C8D0B32469 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0xcf, 0xf7, 0x0f, 0xf0, 0x0f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0xcf, 0xf7, 0x9f, 0xf0, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0f, 0x9f, 0xff, 0x3f, 0xf0, 0x07, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x1f, 0xbf, 0xff, 0x3f, 0xe0, 0x03, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x1f, 0xbf, 0xfe, 0x3f, 0xe0, 0x01, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x3f, 0x7f, 0xfc, 0x7f, 0xc0, 0x01, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x3f, 0x7f, 0xb4, 0x7f, 0xc0, 0x01, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x03, 0xf8, 0x07, 0xfe, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x7c, 0xf9, 0xf0, 0xff, 0x80, 0x03, 0xfc, 0x01, 0xfe, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x7c, 0xfd, 0xf9, 0xff, 0x00, 0x07, 0xfc, 0x01, 0xfe, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xfd, 0xf9, 0xf3, 0xff, 0x00, 0x07, 0xfc, 0x03, 0xfe, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xf9, 0xfb, 0xf3, 0xfe, 0x00, 0x0f, 0xf8, 0x07, 0xfc, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0xf3, 0xfb, 0xe7, 0xfe, 0x00, 0x1f, 0xf8, 0x07, 0xfc, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0xf3, 0xf7, 0xe7, 0xfc, 0x00, 0x1f, 0xf0, 0x0f, 0xfc, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0xe7, 0xe7, 0xcf, 0xfc, 0x00, 0x3f, 0xf0, 0x0f, 0xf8, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x07, 0xe7, 0xef, 0xcf, 0xf8, 0x00, 0x3f, 0xe0, 0x1f, 0xf8, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x07, 0xef, 0xcf, 0x8f, 0xf8, 0x00, 0x3f, 0xe0, 0x3f, 0xf0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0f, 0xcf, 0xdf, 0x9f, 0xf0, 0x00, 0x3f, 0xc0, 0x7f, 0xf0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0f, 0xdf, 0x9f, 0x1f, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x1f, 0xbf, 0xbf, 0x3f, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x1f, 0xbf, 0x7e, 0x3f, 0xff, 0xfc, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x3f, 0x7f, 0x7e, 0x7f, 0xff, 0xfd, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x3f, 0x7e, 0x7c, 0x7f, 0xff, 0xf9, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x7e, 0xfc, 0xf8, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x7e, 0xfc, 0xf8, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// WiFi配置
const char* ap_ssid = "ESP32_Light_Detector";
const char* ap_password = "88888888";

// Web服务器和WebSocket服务器
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(WS_PORT);

// 系统状态变量
unsigned long updateInterval = 100;
unsigned long lastUpdateTime = 0;

bool systemPaused = false;
bool displayEnabled = true;  // 显示器开关状态

// 数据记录
int lightValue = 0;
int maxLightValue = 0;
int minLightValue = 4095;
unsigned long maxLightTime = 0;
unsigned long minLightTime = 0;

// 双缓冲显示变量
uint8_t frameBuffer[SCREEN_WIDTH * SCREEN_HEIGHT / 8];
uint8_t backBuffer[SCREEN_WIDTH * SCREEN_HEIGHT / 8];
bool currentBuffer = 0;
int currentColumn = 0;

// WebSocket事件处理函数
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] WebSocket客户端断开连接\n", num);
      break;
      
    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] WebSocket客户端连接: %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
      
      // 发送当前状态给新连接的客户端
      sendSensorDataToClient(num);
      break;
    }
    
    case WStype_TEXT: {
      Serial.printf("[%u] 收到WebSocket消息: %s\n", num, payload);
      
      // 解析JSON命令
      DynamicJsonDocument doc(256);
      DeserializationError error = deserializeJson(doc, payload);
      
      if (error) {
        Serial.println("JSON解析失败");
        return;
      }
      
      String command = doc["command"].as<String>();
      String value = doc["value"].as<String>();
      
      // 处理命令
      String response = processCommand(command, value);
      
      // 发送响应
      webSocket.sendTXT(num, response);
      
      // 如果命令改变了系统状态，广播更新
      if (command == "set_rate" || command == "pause" || command == "resume" || 
          command == "reset_peaks" || command == "display_off" || command == "display_on") {
        sendSensorDataToAllClients();
      }
      break;
    }
    
    default:
      break;
  }
}

// 处理控制命令（WebSocket和HTTP共用）
String processCommand(String command, String value) {
  String response = "{\"status\":\"ok\",\"command\":\"" + command + "\"";
  
  if (command == "set_rate") {
    int newInterval = value.toInt();
    if (newInterval >= 20 && newInterval <= 1000) {
      updateInterval = newInterval;
      response += ",\"message\":\"刷新率已设置为 " + value + "ms\"";
      Serial.println("  刷新率已设置为: " + value + "ms");
    } else {
      response = "{\"status\":\"error\",\"message\":\"刷新率超出范围 (20-1000ms)\"}";
      Serial.println("  错误：刷新率设置超出范围");
    }
  } 
  else if (command == "display_off") {
    displayEnabled = false;
    display.clearDisplay();
    display.display();
    response += ",\"message\":\"显示器已关闭\"";
    Serial.println("  显示器已关闭");
  }
  else if (command == "display_on") {
    displayEnabled = true;
    response += ",\"message\":\"显示器已恢复\"";
    Serial.println("  显示器已恢复");
  }
  else if (command == "pause") {
    systemPaused = true;
    response += ",\"message\":\"系统已暂停\"";
    Serial.println("  系统已暂停");
  }
  else if (command == "resume") {
    systemPaused = false;
    response += ",\"message\":\"系统已继续运行\"";
    Serial.println("  系统已恢复运行");
  }
  else if (command == "reboot") {
    response = "{\"status\":\"ok\",\"message\":\"重启中...\"}";
    Serial.println("  系统重启命令接收，3秒后重启...");
    delay(3000);
    ESP.restart();
  }
  else if (command == "reset_peaks") {
    maxLightValue = lightValue;
    minLightValue = lightValue;
    maxLightTime = millis();
    minLightTime = millis();
    response += ",\"message\":\"峰值已重置\"";
    Serial.println("  峰值数据已重置");
  }
  else {
    response = "{\"status\":\"error\",\"message\":\"未知命令: " + command + "\"}";
    Serial.println("  错误：未知命令");
  }
  
  response += "}";
  return response;
}

// 发送传感器数据到指定WebSocket客户端
void sendSensorDataToClient(uint8_t num) {
  DynamicJsonDocument doc(1024);
  doc["type"] = "sensor_data";
  doc["value"] = lightValue;
  doc["voltage"] = (lightValue * 3.3) / 4095.0;
  doc["max"] = maxLightValue;
  doc["min"] = minLightValue;
  doc["max_time"] = maxLightTime;
  doc["min_time"] = minLightTime;
  doc["interval"] = updateInterval;
  doc["paused"] = systemPaused;
  doc["clients"] = WiFi.softAPgetStationNum();
  doc["uptime"] = millis();
  doc["timestamp"] = millis();
  
  String jsonResponse;
  serializeJson(doc, jsonResponse);
  webSocket.sendTXT(num, jsonResponse);
}

// 广播传感器数据到所有连接的WebSocket客户端
void sendSensorDataToAllClients() {
  DynamicJsonDocument doc(1024);
  doc["type"] = "sensor_data";
  doc["value"] = lightValue;
  doc["voltage"] = (lightValue * 3.3) / 4095.0;
  doc["max"] = maxLightValue;
  doc["min"] = minLightValue;
  doc["max_time"] = maxLightTime;
  doc["min_time"] = minLightTime;
  doc["interval"] = updateInterval;
  doc["paused"] = systemPaused;
  doc["clients"] = WiFi.softAPgetStationNum();
  doc["uptime"] = millis();
  doc["timestamp"] = millis();
  
  String jsonResponse;
  serializeJson(doc, jsonResponse);
  webSocket.broadcastTXT(jsonResponse);
}

void setup() {
  Serial.begin(9600);
  delay(500);
  
  Serial.println("==========================================");
  Serial.println("ESP32光线强度探测器 V2 - 启动中...");
  Serial.println("==========================================");
  
  // 初始化I2C
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  Serial.println("✓ I2C总线初始化完成");
  
  // 初始化OLED
  Serial.print("正在初始化OLED显示屏...");
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println("失败！");
    Serial.println("错误：SSD1306分配失败，请检查连线！");
    while(1);
  }
  Serial.println("成功！");

  // 初始化显示缓冲区
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  memset(frameBuffer, 0, sizeof(frameBuffer));
  memset(backBuffer, 0, sizeof(backBuffer));
  Serial.println("✓ 显示缓冲区初始化完成");

  // 显示图片（替换原来的启动信息）
display.clearDisplay();
// 使用drawBitmap函数显示图片，参数为：(x坐标, y坐标, 位图数据, 宽度, 高度, 颜色)
display.drawBitmap(0, 0, epd_bitmap_E01213252520D649939DD4C8D0B32469, 128, 32, SSD1306_WHITE);
display.display();
delay(3000); // 显示3秒

  // 显示启动信息
  // display.setTextSize(1);
  // display.setCursor(0,0);
  // display.println("Light Detector V2");
  // display.display();
  // delay(3000);
  
  // 初始化ADC
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  Serial.println("✓ ADC初始化完成（12位分辨率，0-3.3V量程）");
  
  
  
  // 初始化WiFi热点
  Serial.print("正在启动WiFi热点...");
  WiFi.mode(WIFI_AP);
  boolean result = WiFi.softAP(ap_ssid, ap_password);
  
  if (result) {
    Serial.println("成功！");
    Serial.print("  热点名称(SSID): ");
    Serial.println(ap_ssid);
    Serial.print("  IP地址: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("  已连接设备数: ");
    Serial.println(WiFi.softAPgetStationNum());
  } else {
    Serial.println("失败！");
    Serial.println("错误：无法启动WiFi热点");
    while(1);
  }
  
  // 初始化LittleFS文件系统
  Serial.print("正在挂载LittleFS文件系统...");
  if(!LittleFS.begin(true)){
    Serial.println("失败！");
    Serial.println("警告：LittleFS挂载失败，网页功能将不可用");
  } else {
    Serial.println("成功！");
    
    // 列出LittleFS中的文件
    File root = LittleFS.open("/");
    File file = root.openNextFile();
    Serial.println("LittleFS中的文件列表:");
    while(file){
      Serial.print("  ");
      Serial.print(file.name());
      Serial.print(" (");
      Serial.print(file.size());
      Serial.println(" 字节)");
      file = root.openNextFile();
    }
    root.close();
    
    // 检查HTML文件是否存在
    if(LittleFS.exists("/index.html")) {
      Serial.println("✓ 找到index.html文件");
    } else {
      Serial.println("✗ 未找到index.html文件，请通过ESP32 Sketch Data Upload工具上传");
    }
  }
  
  // 设置Web服务器路由
  server.on("/", HTTP_GET, serveHomePage);
  server.on("/data", HTTP_GET, getSensorData);  // 保留HTTP API以兼容
  server.on("/control", HTTP_POST, handleControl);  // 保留HTTP API以兼容
  server.on("/peak", HTTP_GET, getPeakData);
  server.on("/log", HTTP_GET, showSystemLog);
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("✓ HTTP服务器启动完成（端口80）");
  
  // 启动WebSocket服务器
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("✓ WebSocket服务器启动完成（端口81）");
  
  Serial.println("✓ 所有路由已注册:");
  Serial.println("  - GET  /         : 主页面");
  Serial.println("  - GET  /data     : 传感器数据API (HTTP兼容)");
  Serial.println("  - POST /control   : 控制命令API (HTTP兼容)");
  Serial.println("  - GET  /peak     : 峰值数据");
  Serial.println("  - GET  /log      : 系统日志");
  Serial.println("  - WS   ws://IP:81 : WebSocket实时数据推送");
  

  Serial.println("==========================================");
  Serial.println("系统启动完成！");
  Serial.println("请用设备连接WiFi热点并访问上述IP地址");
  Serial.println("WebSocket地址: ws://" + WiFi.softAPIP().toString() + ":81");
  Serial.println("==========================================");
  display.clearDisplay();
}

void loop() {
  server.handleClient();
  webSocket.loop();  // 处理WebSocket事件
  
  if (!systemPaused) {
    unsigned long currentTime = millis();
    
    if (currentTime - lastUpdateTime >= updateInterval) {
      lastUpdateTime = currentTime;
      
      // 读取传感器
      lightValue = analogRead(LIGHT_SENSOR_PIN);
      
      // 更新峰值记录
      updatePeakValues(lightValue, currentTime);
      
      // 更新OLED显示（仅在显示器启用时）
      if (displayEnabled) {
        updateOLEDDisplay(lightValue);
      }
      
      // 通过WebSocket实时推送数据到所有连接的客户端
      sendSensorDataToAllClients();
      
      currentColumn++;
    }
  }
}

// Web页面服务函数
void serveHomePage() {
  Serial.print("[" + String(millis()) + "] 客户端请求主页 - ");
  Serial.print("客户端IP: ");
  Serial.println(server.client().remoteIP());
  
  if (!LittleFS.exists("/index.html")) {
    Serial.println("  错误：index.html文件不存在于LittleFS中");
    String errorPage = "<html><body><h1>错误：网页文件未找到</h1>";
    errorPage += "<p>请通过Arduino IDE的ESP32 Sketch Data Upload工具上传网页文件</p>";
    errorPage += "<p>当前时间: " + String(millis()) + "ms</p>";
    errorPage += "<p>光敏电阻当前值: " + String(lightValue) + "</p>";
    errorPage += "</body></html>";
    server.send(200, "text/html", errorPage);
    return;
  }
  
  File file = LittleFS.open("/index.html", "r");
  if (!file) {
    Serial.println("  错误：无法打开index.html文件");
    server.send(500, "text/plain", "无法打开网页文件");
    return;
  }
  
  Serial.println("  正在发送index.html文件，大小: " + String(file.size()) + " 字节");
  server.streamFile(file, "text/html");
  file.close();
  Serial.println("  文件发送完成");
}

// HTTP传感器数据API（保留以兼容）
void getSensorData() {
  Serial.print("[" + String(millis()) + "] 客户端请求传感器数据 (HTTP) - ");
  Serial.println("客户端IP: " + server.client().remoteIP().toString());
  
  DynamicJsonDocument doc(1024);
  doc["value"] = lightValue;
  doc["voltage"] = (lightValue * 3.3) / 4095.0;
  doc["max"] = maxLightValue;
  doc["min"] = minLightValue;
  doc["max_time"] = maxLightTime;
  doc["min_time"] = minLightTime;
  doc["interval"] = updateInterval;
  doc["paused"] = systemPaused;
  doc["clients"] = WiFi.softAPgetStationNum();
  doc["uptime"] = millis();
  
  String jsonResponse;
  serializeJson(doc, jsonResponse);
  server.send(200, "application/json", jsonResponse);
  
  Serial.println("  传感器数据已发送: value=" + String(lightValue) + 
                ", voltage=" + String((lightValue * 3.3) / 4095.0, 3) + "V");
}

// HTTP控制命令处理（保留以兼容）
void handleControl() {
  String command = server.arg("command");
  String value = server.arg("value");
  
  Serial.print("[" + String(millis()) + "] 控制命令接收 (HTTP) - ");
  Serial.print("命令: " + command);
  Serial.println(", 值: " + value);
  Serial.println("  客户端IP: " + server.client().remoteIP().toString());
  
  String response = processCommand(command, value);
  
  // 如果是重启命令，已经处理过了
  if (command == "reboot") {
    return;
  }
  
  server.send(200, "application/json", response);
}

// 系统日志页面
void showSystemLog() {
  Serial.print("[" + String(millis()) + "] 客户端请求系统日志 - ");
  Serial.println("客户端IP: " + server.client().remoteIP().toString());
  
  String logPage = "<html><head><meta charset='UTF-8'><title>系统日志</title></head><body>";
  logPage += "<h1>ESP32光线探测器 - 系统状态</h1>";
  logPage += "<h2>系统信息</h2>";
  logPage += "<p>运行时间: " + String(millis() / 1000) + " 秒</p>";
  logPage += "<p>当前光照值: " + String(lightValue) + "</p>";
  logPage += "<p>刷新间隔: " + String(updateInterval) + "ms</p>";
  logPage += "<p>系统状态: " + String(systemPaused ? "已暂停" : "运行中") + "</p>";
  logPage += "<p>连接设备数: " + String(WiFi.softAPgetStationNum()) + "</p>";
  logPage += "<p>WebSocket连接数: " + String(webSocket.connectedClients()) + "</p>";
  logPage += "<p>显示器状态: " + String(displayEnabled ? "开启" : "关闭") + "</p>";
  
  logPage += "<h2>峰值记录</h2>";
  logPage += "<p>最大值: " + String(maxLightValue) + " (时间: " + String(maxLightTime) + "ms)</p>";
  logPage += "<p>最小值: " + String(minLightValue) + " (时间: " + String(minLightTime) + "ms)</p>";
  
  logPage += "<h2>LittleFS文件系统</h2>";
  if (LittleFS.exists("/index.html")) {
    File file = LittleFS.open("/index.html", "r");
    logPage += "<p>index.html: 存在 (" + String(file.size()) + " 字节)</p>";
    file.close();
  } else {
    logPage += "<p>index.html: <span style='color:red'>不存在</span></p>";
  }
  
  logPage += "<p><a href='/'>返回主页面</a></p>";
  logPage += "</body></html>";
  
  server.send(200, "text/html", logPage);
  Serial.println("  系统日志页面已发送");
}

// 404错误处理
void handleNotFound() {
  String message = "文件未找到\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\n方法: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\n参数: ";
  message += server.args();
  message += "\n";
  
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  Serial.print("[" + String(millis()) + "] 404错误 - ");
  Serial.println("请求的URI不存在: " + server.uri());
  Serial.println("  客户端IP: " + server.client().remoteIP().toString());
  
  server.send(404, "text/plain", message);
}

// 峰值数据API
void getPeakData() {
  DynamicJsonDocument doc(512);
  doc["current_max"] = maxLightValue;
  doc["current_min"] = minLightValue;
  doc["max_time"] = maxLightTime;
  doc["min_time"] = minLightTime;
  
  String jsonResponse;
  serializeJson(doc, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

// 更新峰值记录
void updatePeakValues(int value, unsigned long timestamp) {
  if (value > maxLightValue) {
    maxLightValue = value;
    maxLightTime = timestamp;
  }
  if (value < minLightValue) {
    minLightValue = value;
    minLightTime = timestamp;
  }
}

// OLED显示函数
void updateOLEDDisplay(int value) {
  int yPosition = map(value, 0, 4095, SCREEN_HEIGHT-1, 0);
  yPosition = constrain(yPosition, 0, SCREEN_HEIGHT-1);
  
  shiftGraphLeft(backBuffer);
  drawNewDataPoint(backBuffer, SCREEN_WIDTH-1, yPosition);
  swapBuffers();
  
  display.clearDisplay();
  for (int i = 0; i < (SCREEN_WIDTH * SCREEN_HEIGHT / 8); i++) {
    display.getBuffer()[i] = frameBuffer[i];
  }
  
  // 显示系统状态信息
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("L:");
  display.print(value);
  display.setCursor(64, 0);
  display.print("Max:");
  display.print(maxLightValue);
  
  display.display();
}

// 图形处理函数
void shiftGraphLeft(uint8_t* buffer) {
  for (int col = 0; col < SCREEN_WIDTH - 1; col++) {
    for (int page = 0; page < 4; page++) {
      int currentIndex = col + page * SCREEN_WIDTH;
      int nextIndex = (col + 1) + page * SCREEN_WIDTH;
      buffer[currentIndex] = buffer[nextIndex];
    }
  }
  for (int page = 0; page < 4; page++) {
    int lastColIndex = (SCREEN_WIDTH - 1) + page * SCREEN_WIDTH;
    buffer[lastColIndex] = 0;
  }
}

void drawNewDataPoint(uint8_t* buffer, int x, int y) {
  if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
  int page = y / 8;
  int bitPosition = y % 8;
  int bufferIndex = x + page * SCREEN_WIDTH;
  buffer[bufferIndex] |= (1 << bitPosition);
}

void swapBuffers() {
  currentBuffer = !currentBuffer;
  if (currentBuffer) {
    memcpy(frameBuffer, backBuffer, sizeof(frameBuffer));
  } else {
    memcpy(backBuffer, frameBuffer, sizeof(backBuffer));
  }
}
