#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

// ================================================
// ESP32-S3 + GC9A01 丸型液晶ディスプレイ ハードウェア設定
// ================================================

// ピン定義（コード内で参照するため）
#define DISPLAY_MOSI    11  // SDA/MOSI - シリアルデータ入力
#define DISPLAY_SCLK    12  // SCL/SCK  - シリアルクロック
#define DISPLAY_CS      10  // CS       - チップセレクト
#define DISPLAY_DC      9   // DC/RS    - データ/コマンド選択
#define DISPLAY_RST     8   // RST      - リセット
#define DISPLAY_BL      13  // BL       - バックライト制御

// アプリケーション用の定数
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 240
#define SCREEN_RADIUS 120

// バックライト制御設定
#define BACKLIGHT_CHANNEL    0
#define BACKLIGHT_FREQUENCY  5000
#define BACKLIGHT_RESOLUTION 8
#define BACKLIGHT_MAX_DUTY   255

// その他のGPIOピン（必要に応じて使用）
#ifndef LED_BUILTIN
#define LED_BUILTIN 48      // ESP32-S3内蔵LED
#endif
#define BUTTON_PIN  0       // BOOTボタン

// 色定義（RGB565フォーマット）
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0
#define COLOR_CYAN    0x07FF
#define COLOR_MAGENTA 0xF81F
#define COLOR_ORANGE  0xFD20
#define COLOR_PURPLE  0x8010

#endif // HARDWARE_CONFIG_H