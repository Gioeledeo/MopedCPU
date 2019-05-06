#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8
#define modeSwitch A1

int footerRND;

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
  if (analogRead(modeSwitch) >= 500) {
    tft.fillScreen(ST7735_BLACK);
    do {
      reportValMatrixSet();
    } while (analogRead(modeSwitch) >= 500);
  }
  else {
    tft.fillScreen(ST7735_BLACK);
    do {
      generalValMatrixSet();
    } while (analogRead(modeSwitch) <= 10);
  }
}

void headerReport(){
  tft.setTextSize(1);
  tft.setCursor(17, 1);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(true);
  tft.print("PIAGGIO CIAO CPU");
  tft.setCursor(55, 11);
  tft.setTextColor(ST7735_GREEN);
  tft.print("by GioeleDeo");
  tft.setCursor(0, 26);
  tft.setTextColor(ST7735_YELLOW);
  tft.print("TOUR'S REPORT VALUE: ");
}

void footerReport(){
  footerRND = random(8);
  tft.setTextSize(1);
  tft.setCursor(0,142);
  tft.setTextColor(ST7735_RED);
  switch(footerRND){
    case 0:
    tft.print("Se sei incerto, TIENIAPERTO !!");
    break;
    case 1: 
    tft.print("Sirena?! Su la targa e giu' in carena");
    break;
    case 2:
    tft.print("Prendigli la paletta e apri a manetta");
    break;
    case 3:
    tft.print("Figa e cavai NO BASTAMAI");
    break;
    case 4:
    tft.print("L'ultima modifica e' solo una leggenda");
    break;
    case 5:
    tft.print("4 tempi e' bene,     2 tempi e' meglio !");
    break;
    case 6:
    tft.print("Freni e paura,   mai avuti");
    break;
    case 7:
    tft.print("Vendo ruota anteriorecausa inutilizzo");
    break;
  }
}

void generalValMatrixSet() {
  tft.setTextSize(1);
  tft.setCursor(17, 1);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(true);
  tft.print("PIAGGIO CIAO CPU");
  tft.setCursor(55, 11);
  tft.setTextColor(ST7735_GREEN);
  tft.print("by GioeleDeo");
  tft.setCursor(0, 26);
  tft.setTextColor(ST7735_YELLOW);
  tft.print("GENERAL VALUE: ");
  tft.setCursor(0, 36);
  tft.setTextColor(ST7735_BLUE);
  tft.print("Engine temp: ");
  tft.setCursor(0, 46);
  tft.print("External temp: ");
  tft.setCursor(0, 56);
  tft.print("External hum: ");
  tft.setCursor(0, 66);
  tft.print("SX turn angle: ");
  tft.setCursor(0, 76);
  tft.print("DX turn angle: ");
  tft.setCursor(0, 86);
  tft.print("Wheelie angle: ");
  tft.setCursor(0, 96);
  tft.print("Stoppie angle: ");
  tft.setCursor(0, 106);
  tft.print("Air filter hum: ");
  tft.setCursor(0, 125);
  tft.setTextColor(ST7735_RED);
  tft.print("KM/H: ");
  tft.setCursor(60, 125);
  tft.print("RPM: ");
  tft.setCursor(115, 33);
  tft.setTextColor(ST7735_WHITE);
  tft.print((char)248);
  tft.setCursor(120, 36);
  tft.print("C");
  tft.setCursor(115, 43);
  tft.print((char)248);
  tft.setCursor(120, 46);
  tft.print("C");
  tft.setCursor(120, 56);
  tft.print("%");
  tft.setCursor(120, 106);
  tft.print("%");
  tft.setTextSize(2);
  tft.setCursor(0, 140);
  tft.print("000");
  tft.setCursor(60, 140);
  tft.print("00000");
  delay(100);
}

void reportValMatrixSet() {
  tft.fillScreen(ST7735_BLACK);
  headerReport();
  footerReport();
  engineTemp();
  delay(3000);
  tft.fillScreen(ST7735_BLACK);
  headerReport();
  footerReport();
  externalTempHum();
  delay(3000);
  /*
  tft.fillScreen(ST7735_BLACK);
  headerReport();
  turnAngle();
  delay(3000);
  tft.fillScreen(ST7735_BLACK);
  headerReport();
  wheelie();
  delay(3000);
  tft.fillScreen(ST7735_BLACK);
  headerReport();
  stoppie();
  delay(3000);
  tft.fillScreen(ST7735_BLACK);
  headerReport();
  airFilterHum();
  delay(3000);
  */
}

void engineTemp(){
  tft.setCursor(30, 45);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLUE);
  tft.print("ENGINE");
  tft.setCursor(43, 62);
  tft.print("TEMP");
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(0, 87);
  tft.print("MAX: ");
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.print("  000.00");
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(0, 105);
  tft.print("MIN: ");
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.print("  000.00");
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(0, 123);
  tft.print("AVG: ");
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.print("  000.00");
}

void externalTempHum(){
  tft.setCursor(20,45);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLUE);
  tft.print("EXTERNAL");
  tft.setCursor(5,62);
  tft.print("TEMP & HUM");
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(0, 87);
  tft.print("MAX: ");
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.print("00");
  tft.setCursor(52,80);
  tft.print((char)248);
  tft.setCursor(62,87);
  tft.print("C ");
  tft.print("00");
  tft.setTextSize(1);
  tft.print(" ");
  tft.setTextSize(2);
  tft.print("%");
  tft.setTextSize(1);
  tft.setCursor(0, 105);
  tft.setTextColor(ST7735_YELLOW);
  tft.print("MIN: ");
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.print("00");
  tft.setCursor(52,98);
  tft.print((char)248);
  tft.setCursor(62,105);
  tft.print("C ");
  tft.print("00");
  tft.setTextSize(1);
  tft.print(" ");
  tft.setTextSize(2);
  tft.print("%");
  tft.setCursor(0, 123);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.print("AVG: ");
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.print("00");
  tft.setCursor(52,116);
  tft.print((char)248);
  tft.setCursor(62,123);
  tft.print("C ");
  tft.print("00");
  tft.setTextSize(1);
  tft.print(" ");
  tft.setTextSize(2);
  tft.print("%");
}
