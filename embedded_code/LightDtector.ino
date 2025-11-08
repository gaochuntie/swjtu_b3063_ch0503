#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED配置
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define OLED_I2C_ADDRESS 0x3C

// **引脚定义**
#define LIGHT_SENSOR_PIN 5     // 光敏电阻连接到 GPIO5 (ADC1通道4)
#define OLED_SDA_PIN 9         // OLED SDA 连接到 GPIO9
#define OLED_SCL_PIN 11        // OLED SCL 连接到 GPIO11

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ADC配置
const int adcMaxValue = 4095; // 12位ADC最大值
const int screenHeight = SCREEN_HEIGHT;

// 双缓冲相关变量
uint8_t frameBuffer[SCREEN_WIDTH * SCREEN_HEIGHT / 8]; // 主帧缓冲区，512字节
uint8_t backBuffer[SCREEN_WIDTH * SCREEN_HEIGHT / 8];  // 后台缓冲区，512字节
bool currentBuffer = 0; // 当前缓冲区标识

// 图形参数
// **关键修改1：刷新时间从200ms改为100ms**
const unsigned long updateInterval = 100; // 100ms更新间隔 (0.1秒)
unsigned long lastUpdateTime = 0;
int currentColumn = 0;

void setup() {
  Serial.begin(9600);
  
  // 初始化I2C总线并指定引脚
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  
  // 初始化OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("SSD1306分配失败，请检查连线！"));
    while(1);
  }
  
  // 初始化ADC为12位分辨率
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db); // 设置衰减为11dB，量程约0-3.3V
  
  // 初始化显示
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  
  // 清空双缓冲区
  memset(frameBuffer, 0, sizeof(frameBuffer));
  memset(backBuffer, 0, sizeof(backBuffer));
  
  // 显示初始信息
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Light Sensor Ready");
  display.setCursor(0,10);
  display.println("Rate: 0.1s/pixel");
  display.display();
  delay(1000);
  display.clearDisplay();
  
  Serial.println("光线强度探测器初始化完成 (刷新率: 0.1秒/像素)");
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentTime;
    
    // 读取光敏电阻值 (12位精度)
    int lightValue = analogRead(LIGHT_SENSOR_PIN);
    
    // 将ADC值映射到屏幕Y坐标 (0-31)，强光在上，弱光在下
    int yPosition = map(lightValue, 0, adcMaxValue, screenHeight-1, 0);
    yPosition = constrain(yPosition, 0, screenHeight-1);
    
    // 使用双缓冲：在后台缓冲区操作
    shiftGraphLeft(backBuffer);
    drawNewDataPoint(backBuffer, SCREEN_WIDTH-1, yPosition);
    
    // 交换缓冲区
    swapBuffers();
    
    // 更新显示
    updateDisplay();
    
    // 每128次采样输出一次调试信息（因为刷新速度加快）
    if (currentColumn % 128 == 0) {
      Serial.print("ADC值: ");
      Serial.print(lightValue);
      Serial.print(" | Y坐标: ");
      Serial.print(yPosition);
      Serial.print(" | 电压: ");
      Serial.print((lightValue * 3.3) / adcMaxValue, 3);
      Serial.println("V");
    }
    
    currentColumn++;
  }
}

// 将图形向左移动一列
void shiftGraphLeft(uint8_t* buffer) {
  for (int col = 0; col < SCREEN_WIDTH - 1; col++) {
    for (int page = 0; page < 4; page++) {
      int currentIndex = col + page * SCREEN_WIDTH;
      int nextIndex = (col + 1) + page * SCREEN_WIDTH;
      buffer[currentIndex] = buffer[nextIndex];
    }
  }
  // 清空最右侧一列
  for (int page = 0; page < 4; page++) {
    int lastColIndex = (SCREEN_WIDTH - 1) + page * SCREEN_WIDTH;
    buffer[lastColIndex] = 0;
  }
}

// 在指定位置绘制新的数据点
void drawNewDataPoint(uint8_t* buffer, int x, int y) {
  if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
  
  int page = y / 8;
  int bitPosition = y % 8;
  
  int bufferIndex = x + page * SCREEN_WIDTH;
  buffer[bufferIndex] |= (1 << bitPosition);
}

// 交换前后台缓冲区（实现双缓冲防闪烁）
void swapBuffers() {
  currentBuffer = !currentBuffer;
  if (currentBuffer) {
    memcpy(frameBuffer, backBuffer, sizeof(frameBuffer));
  } else {
    memcpy(backBuffer, frameBuffer, sizeof(frameBuffer));
  }
}

// 更新显示内容
void updateDisplay() {
  display.clearDisplay();
  
  // 将当前帧缓冲区内容复制到显示缓冲区
  for (int i = 0; i < (SCREEN_WIDTH * SCREEN_HEIGHT / 8); i++) {
    display.getBuffer()[i] = frameBuffer[i];
  }
  
  // **关键修改2：移除了网格绘制函数调用，只显示纯数据曲线**
  // 只添加简单的幅度标签
  // display.setTextSize(1);
  // display.setCursor(SCREEN_WIDTH - 16, 0);
  // display.print("Hi");
  // display.setCursor(SCREEN_WIDTH - 16, SCREEN_HEIGHT - 8);
  // display.print("Lo");
  
  display.display();
}