#include <Adafruit_GFX.h>   
#include <Adafruit_ST7735.h>
#include <SPI.h>
 
#define TFT_CS 10      
#define TFT_DC 9        
#define TFT_RST 8

//standard text size = 8 pixel (setTextSize(1))

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

void setup(void) {
  tft.initR(INITR_BLACKTAB);  
 
  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;
  delay(10);
}

void loop() {
  displayMatrixSet();
}

void displayMatrixSet(){
  tft.setTextSize(1);
  tft.setCursor(17, 1);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(true);
  tft.print("PIAGGIO CIAO CPU");
  tft.setCursor(55,11);
  tft.setTextColor(ST7735_GREEN);
  tft.print("by GioeleDeo");
  tft.setCursor(0,26);
  tft.setTextColor(ST7735_YELLOW);
  tft.print("GENERAL VALUE: ");
  tft.setCursor(0,36);
  tft.setTextColor(ST7735_BLUE);
  tft.print("Engine temp: ");
  tft.setCursor(0,46);
  tft.print("External temp: ");
  tft.setCursor(0,56);
  tft.print("External hum: ");
  tft.setCursor(0,66);
  tft.print("SX turn angle: ");
  tft.setCursor(0,76);
  tft.print("DX turn angle: ");
  tft.setCursor(0,86);
  tft.print("Wheelie angle: ");
  tft.setCursor(0,96);
  tft.print("Stoppie angle: ");
  tft.setCursor(0,106);
  tft.print("Air filter hum: ");
  tft.setCursor(0,125);
  tft.setTextColor(ST7735_RED);
  tft.print("KM/H: ");
  tft.setCursor(60,125);
  tft.print("RPM: ");
  tft.setCursor(115,33);
  tft.setTextColor(ST7735_WHITE);
  tft.print((char)248);
  tft.setCursor(120,36);
  tft.print("C");
  tft.setCursor(115,43);
  tft.print((char)248);
  tft.setCursor(120,46);
  tft.print("C");
  tft.setCursor(120,56);
  tft.print("%");
  tft.setCursor(120,106);
  tft.print("%");
  tft.setTextSize(2);
  tft.setCursor(0,140);
  tft.print("000");
  tft.setCursor(60,140);
  tft.print("00000");
  delay(100);
}
