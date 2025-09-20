#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "hardware_config.h"

// TFT_eSPIオブジェクト作成
TFT_eSPI tft = TFT_eSPI();

// 関数の前方宣言
void displayWelcomeScreen();
void colorTest();
void shapeTest();
void textTest();
void circularPatternTest();
void backlightTest();

void setup() {
  // シリアル通信開始
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32-S3 + GC9A01 テスト開始");
  
  // バックライトPWM設定
  ledcSetup(BACKLIGHT_CHANNEL, BACKLIGHT_FREQUENCY, BACKLIGHT_RESOLUTION);
  ledcAttachPin(DISPLAY_BL, BACKLIGHT_CHANNEL);
  ledcWrite(BACKLIGHT_CHANNEL, BACKLIGHT_MAX_DUTY); // 最大輝度
  
  // ディスプレイ初期化
  tft.init();
  tft.setRotation(0); // 0-3で回転設定
  tft.fillScreen(COLOR_BLACK);
  
  Serial.println("ディスプレイ初期化完了");
  Serial.printf("画面サイズ: %dx%d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
  
  // 初期テスト表示
  displayWelcomeScreen();
}

void loop() {
  // テストパターン1: カラーテスト
  colorTest();
  delay(3000);
  
  // テストパターン2: 図形テスト
  shapeTest();
  delay(3000);
  
  // テストパターン3: テキストテスト
  textTest();
  delay(3000);
  
  // テストパターン4: 円形パターン（丸型ディスプレイの特徴を活かす）
  circularPatternTest();
  delay(3000);
  
  // バックライト調光テスト
  backlightTest();
  delay(2000);
}

void displayWelcomeScreen() {
  tft.fillScreen(COLOR_BLACK);
  
  // タイトル表示
  tft.setTextColor(COLOR_WHITE, COLOR_BLACK);
  tft.setTextSize(2);
  tft.drawCentreString("ESP32-S3", SCREEN_WIDTH/2, 60, 2);
  tft.drawCentreString("GC9A01", SCREEN_WIDTH/2, 90, 2);
  tft.drawCentreString("TEST", SCREEN_WIDTH/2, 120, 2);
  
  // 外周円を描画
  tft.drawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_RADIUS-2, COLOR_WHITE);
  tft.drawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_RADIUS-5, COLOR_BLUE);
  
  Serial.println("ウェルカム画面表示完了");
}

void colorTest() {
  Serial.println("カラーテスト開始");
  
  // 基本色のテスト
  uint16_t colors[] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW, 
                       COLOR_CYAN, COLOR_MAGENTA, COLOR_WHITE, COLOR_BLACK};
  const char* colorNames[] = {"RED", "GREEN", "BLUE", "YELLOW", 
                             "CYAN", "MAGENTA", "WHITE", "BLACK"};
  
  for (int i = 0; i < 8; i++) {
    tft.fillScreen(colors[i]);
    tft.setTextColor(colors[i] == COLOR_BLACK ? COLOR_WHITE : COLOR_BLACK);
    tft.setTextSize(2);
    tft.drawCentreString(colorNames[i], SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 10, 2);
    delay(500);
  }
}

void shapeTest() {
  Serial.println("図形テスト開始");
  tft.fillScreen(COLOR_BLACK);
  
  // 中心座標
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;
  
  // 同心円を描画
  for (int r = 20; r <= 100; r += 20) {
    uint16_t color = random(0xFFFF);
    tft.drawCircle(centerX, centerY, r, color);
    delay(300);
  }
  
  delay(1000);
  
  // 塗りつぶし円
  tft.fillScreen(COLOR_BLACK);
  for (int r = 100; r >= 20; r -= 20) {
    uint16_t color = random(0xFFFF);
    tft.fillCircle(centerX, centerY, r, color);
    delay(300);
  }
  
  delay(1000);
  
  // 直線パターン
  tft.fillScreen(COLOR_BLACK);
  for (int i = 0; i < 360; i += 15) {
    float rad = i * PI / 180;
    int x = centerX + cos(rad) * 100;
    int y = centerY + sin(rad) * 100;
    uint16_t color = tft.color565(i % 256, (i * 2) % 256, (i * 3) % 256);
    tft.drawLine(centerX, centerY, x, y, color);
    delay(100);
  }
}

void textTest() {
  Serial.println("テキストテスト開始");
  tft.fillScreen(COLOR_BLACK);
  
  // 異なるサイズのテキスト
  tft.setTextColor(COLOR_WHITE);
  
  tft.setTextSize(1);
  tft.drawCentreString("Size 1 Text", SCREEN_WIDTH/2, 30, 1);
  
  tft.setTextSize(2);
  tft.drawCentreString("Size 2", SCREEN_WIDTH/2, 70, 2);
  
  tft.setTextSize(3);
  tft.drawCentreString("S3", SCREEN_WIDTH/2, 110, 2);
  
  // カラフルなテキスト
  const char* text = "COLOR TEST";
  uint16_t colors[] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW, 
                       COLOR_CYAN, COLOR_MAGENTA, COLOR_ORANGE, COLOR_PURPLE, 
                       COLOR_WHITE, COLOR_WHITE};
  
  tft.setTextSize(1);
  for (int i = 0; i < strlen(text); i++) {
    tft.setTextColor(colors[i]);
    tft.drawChar(text[i], 80 + i * 12, 170, 2);
  }
  
  // 数値表示テスト
  tft.setTextColor(COLOR_GREEN);
  tft.setTextSize(2);
  for (int i = 0; i <= 100; i += 10) {
    tft.fillRect(80, 200, 80, 20, COLOR_BLACK);
    tft.drawCentreString(String(i) + "%", SCREEN_WIDTH/2, 200, 2);
    delay(200);
  }
}

void circularPatternTest() {
  Serial.println("円形パターンテスト開始");
  tft.fillScreen(COLOR_BLACK);
  
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;
  
  // 時計の文字盤風パターン
  for (int hour = 0; hour < 12; hour++) {
    float angle = (hour * 30 - 90) * PI / 180; // 12時を上にするため-90度
    int x1 = centerX + cos(angle) * 90;
    int y1 = centerY + sin(angle) * 90;
    int x2 = centerX + cos(angle) * 100;
    int y2 = centerY + sin(angle) * 100;
    
    tft.drawLine(x1, y1, x2, y2, COLOR_WHITE);
    
    // 数字表示
    int textX = centerX + cos(angle) * 75;
    int textY = centerY + sin(angle) * 75;
    tft.setTextColor(COLOR_YELLOW);
    tft.setTextSize(1);
    tft.drawCentreString(String(hour == 0 ? 12 : hour), textX, textY, 2);
  }
  
  // 中心点
  tft.fillCircle(centerX, centerY, 5, COLOR_RED);
  
  // 秒針の動作をシミュレート
  for (int sec = 0; sec < 60; sec++) {
    // 前の秒針を消去
    if (sec > 0) {
      float prevAngle = ((sec - 1) * 6 - 90) * PI / 180;
      int prevX = centerX + cos(prevAngle) * 80;
      int prevY = centerY + sin(prevAngle) * 80;
      tft.drawLine(centerX, centerY, prevX, prevY, COLOR_BLACK);
    }
    
    // 新しい秒針を描画
    float angle = (sec * 6 - 90) * PI / 180;
    int x = centerX + cos(angle) * 80;
    int y = centerY + sin(angle) * 80;
    tft.drawLine(centerX, centerY, x, y, COLOR_RED);
    
    delay(100);
  }
}

void backlightTest() {
  Serial.println("バックライトテスト開始");
  tft.fillScreen(COLOR_WHITE);
  tft.setTextColor(COLOR_BLACK);
  tft.setTextSize(2);
  tft.drawCentreString("BACKLIGHT", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 20, 2);
  tft.drawCentreString("TEST", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 10, 2);
  
  // 暗くする
  for (int brightness = 255; brightness >= 50; brightness -= 10) {
    ledcWrite(BACKLIGHT_CHANNEL, brightness);
    delay(50);
  }
  
  delay(1000);
  
  // 明るくする
  for (int brightness = 50; brightness <= 255; brightness += 10) {
    ledcWrite(BACKLIGHT_CHANNEL, brightness);
    delay(50);
  }
  
  Serial.println("すべてのテスト完了");
}