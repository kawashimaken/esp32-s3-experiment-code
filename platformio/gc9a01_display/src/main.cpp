#include <Arduino.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

// ESP32-S3 + GC9A01用の設定クラス
class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_GC9A01      _panel_instance;
  lgfx::Bus_SPI           _bus_instance;

public:
  LGFX(void)
  {
    {
      // SPIバスの設定
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI3_HOST;     // ESP32-S3のSPI3を使用
      cfg.spi_mode = 0;             // SPI通信モード (0 ~ 3)
      cfg.freq_write = 40000000;    // 送信時のSPIクロック (最大80MHz, 80MHzを整数で割った値に丸められます)
      cfg.freq_read  = 16000000;    // 受信時のSPIクロック
      cfg.spi_3wire  = true;        // 受信をMOSIピンで行う場合はtrueを設定
      cfg.use_lock   = true;        // トランザクションロックを使用する場合はtrueを設定
      cfg.dma_channel = SPI_DMA_CH_AUTO; // 使用するDMAチャンネル (0=DMA不使用 / 1=1ch / 2=2ch / SPI_DMA_CH_AUTO=自動設定)

      // SPIピン設定
      cfg.pin_sclk = 12;            // SPIのSCLKピン番号
      cfg.pin_mosi = 11;            // SPIのMOSIピン番号
      cfg.pin_miso = -1;            // SPIのMISOピン番号 (-1 = disable)
      cfg.pin_dc   = 9;             // SPIのD/Cピン番号  (-1 = disable)

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      // 表示パネルの設定
      auto cfg = _panel_instance.config();

      cfg.pin_cs           =    10;  // CSが接続されているピン番号   (-1 = disable)
      cfg.pin_rst          =     8;  // RSTが接続されているピン番号  (-1 = disable)
      cfg.pin_busy         =    -1;  // BUSYが接続されているピン番号 (-1 = disable)

      // 以下はパネル毎に設定する解像度
      cfg.panel_width      =   240;  // 実際に表示可能な幅
      cfg.panel_height     =   240;  // 実際に表示可能な高さ
      cfg.offset_x         =     0;  // パネルのX方向オフセット量
      cfg.offset_y         =     0;  // パネルのY方向オフセット量
      cfg.offset_rotation  =     0;  // 回転方向の値のオフセット 0~7 (4~7は上下反転)
      cfg.dummy_read_pixel =     8;  // ピクセル読出し前のダミーリードのビット数
      cfg.dummy_read_bits  =     1;  // ピクセル以外のデータ読出し前のダミーリードのビット数
      cfg.readable         =  true;  // データ読出しが可能な場合 trueに設定
      cfg.invert           = false;  // パネルの明暗が反転してしまう場合 trueに設定
      cfg.rgb_order        = false;  // パネルの赤と青が入れ替わってしまう場合 trueに設定
      cfg.dlen_16bit       = false;  // 16bitパラレルやSPIでデータ長を16bit単位で送信するパネルの場合 trueに設定
      cfg.bus_shared       =  true;  // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)

      _panel_instance.config(cfg);
    }

    setPanel(&_panel_instance);
  }
};

// LovyanGFXのインスタンス作成
LGFX display;

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("=== ESP32-S3 + GC9A01 LovyanGFX テスト ===");

  // ディスプレイ初期化
  display.init();
  Serial.println("ディスプレイ初期化完了");

  // 画面クリア
  display.fillScreen(TFT_BLACK);
  
  // 基本情報表示
  Serial.printf("画面サイズ: %dx%d\n", display.width(), display.height());
  
  // カラーテスト
  Serial.println("カラーテスト開始");
  
  display.fillScreen(TFT_RED);
  Serial.println("赤色表示");
  delay(1000);
  
  display.fillScreen(TFT_GREEN);
  Serial.println("緑色表示");
  delay(1000);
  
  display.fillScreen(TFT_BLUE);
  Serial.println("青色表示");
  delay(1000);
  
  // テキスト表示テスト
  display.fillScreen(TFT_BLACK);
  display.setTextColor(TFT_WHITE);
  display.setTextSize(3);
  display.drawCentreString("ESP32-S3", 120, 80, 1);
  display.setTextSize(2);
  display.drawCentreString("LovyanGFX", 120, 120, 1);
  display.drawCentreString("GC9A01", 120, 150, 1);
  
  Serial.println("初期テスト完了");
}

void loop() {
  // 図形描画デモ
  static unsigned long lastUpdate = 0;
  static int demoStep = 0;
  
  if (millis() - lastUpdate > 3000) {
    switch (demoStep) {
      case 0:
        // 同心円デモ
        display.fillScreen(TFT_BLACK);
        for (int r = 20; r <= 120; r += 20) {
          uint16_t color = display.color565(random(256), random(256), random(256));
          display.drawCircle(120, 120, r, color);
        }
        Serial.println("同心円デモ");
        break;
        
      case 1:
        // 塗りつぶし円デモ
        display.fillScreen(TFT_BLACK);
        for (int r = 100; r >= 20; r -= 20) {
          uint16_t color = display.color565(random(256), random(256), random(256));
          display.fillCircle(120, 120, r, color);
        }
        Serial.println("塗りつぶし円デモ");
        break;
        
      case 2:
        // 放射線デモ
        display.fillScreen(TFT_BLACK);
        for (int i = 0; i < 360; i += 15) {
          float rad = i * PI / 180;
          int x = 120 + cos(rad) * 100;
          int y = 120 + sin(rad) * 100;
          uint16_t color = display.color565(i % 256, (i * 2) % 256, (i * 3) % 256);
          display.drawLine(120, 120, x, y, color);
        }
        Serial.println("放射線デモ");
        break;
        
      case 3:
        // 時計風デモ
        display.fillScreen(TFT_BLACK);
        // 外枠
        display.drawCircle(120, 120, 110, TFT_WHITE);
        display.drawCircle(120, 120, 105, TFT_WHITE);
        
        // 時刻表示
        for (int hour = 1; hour <= 12; hour++) {
          float angle = (hour * 30 - 90) * PI / 180;
          int x = 120 + cos(angle) * 85;
          int y = 120 + sin(angle) * 85;
          display.setTextColor(TFT_YELLOW);
          display.setTextSize(2);
          display.drawCentreString(String(hour), x, y, 1);
        }
        
        // 針（現在時刻風）
        display.drawLine(120, 120, 120, 60, TFT_RED);   // 短針
        display.drawLine(120, 120, 170, 120, TFT_GREEN); // 長針
        display.fillCircle(120, 120, 5, TFT_WHITE);      // 中心点
        
        Serial.println("時計デモ");
        break;
    }
    
    demoStep = (demoStep + 1) % 4;
    lastUpdate = millis();
  }
}