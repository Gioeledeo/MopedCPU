#include <MPU6050.h>
#include <dht.h>
#include <OneWire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <Wire.h>

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8
#define DHT11_PIN 7
#define DS18B20_PIN 2
#define modeSwitch A1

OneWire ds(DS18B20_PIN);
dht DHT;
MPU6050 mpu;

int pitch = 0; //wheelie or stoppie - Y (lateral axis)
int roll = 0; //turn - X (longitudinal axis)
int wheelieAngle = 0;
int maxWheelieAngle = 0;
int stoppieAngle = 0;
int maxStoppieAngle = 0;
int rightTurnAngle = 0;
int leftTurnAngle = 0;
int maxRightTurnAngle = 0;
int maxLeftTurnAngle = 0;
int grindPlate = false;
int wheelieTime = 0;
int topWheelieTime = 0;
int stoppieTime = 0;
int topStoppieTime = 0;

int footerRND;

int maxDHTTemp = 0;
int maxDHTHum = 0;
int minDHTTemp = 0;
int minDHTHum = 0;
int avgDHTTemp = 0;
int avgDHTHum = 0;

int engineTempColor = ST7735_WHITE;
int engineTempVal = 0;
int externalTemp = 0;
int externalHum = 0;
int maxEngineTemp = 0;
int minEngineTemp = 0;
int avgEngineTemp = 0;

//standard text size = 8 pixel (setTextSize(1))

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

void setup(void) {
  tft.initR(INITR_BLACKTAB);

  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;
  delay(10);

  mpu.calibrateGyro();
  mpu.setThreshold(1);
  
  int chkTH = DHT.read11(DHT11_PIN);
  int minDHTTemp = DHT.temperature;
  int minDHTHum = DHT.humidity;
}

void loop() {
  readSensorInit();
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

void readSensorInit(){
  dhtReadSensor();
  delay(10);
  engineTempSensor();
  delay(10);
  mpuGyroAcc();
  delay(10);
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
  footerRND = random(9);
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
    case 8:
    tft.print("TOO FAST TO LIVE     TOO YOUNG TO LIVE");
  }
}

void mpuGyroAcc(){
  int wheelieTime = 0;
  int stoppieTime = 0;
  Vector normAccel = mpu.readNormalizeAccel();
  Vector normGyro = mpu.readNormalizeGyro();
  pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
  roll = (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0)/M_PI;
  
  if(pitch<5){
    int wheelieAngle = pitch*-1;
  } else if(pitch>5){
    int stoppieAngle = pitch;
  } else if(pitch >= -5 && pitch <= 5){
    int wheelieAngle = pitch;
    int stoppieAngle = pitch;
  }
  if(roll>0){
    int rightTurnAngle = roll;
  } else if(roll<0){
    int leftTurnAngle = roll*-1;
  } else if(roll==0){
    int leftTurnAngle = roll;
    int rightTurnAngle = roll;
  }

  if(wheelieAngle >= 50){
    int grindPlate = true;
  } else if(wheelieAngle <50){
    int grindPlate = false;
  }
  
  if(rightTurnAngle > maxRightTurnAngle){
    int maxRightTurnAngle = rightTurnAngle;
  }
  if(leftTurnAngle > maxLeftTurnAngle){
    int maxLeftTurnAngle = leftTurnAngle;
  }
  if(wheelieAngle > maxWheelieAngle){
    int maxWheelieAngle = wheelieAngle;
  }
  if(stoppieAngle > maxStoppieAngle){
    int maxStoppieAngle = stoppieAngle;
  }

  while(pitch<5){
    delay(1000);
    int wheelieTime = wheelieTime +1;
  }
  if(wheelieTime > topWheelieTime){
    int topWheelieTime = wheelieTime;
  }
  while(pitch>5){
    delay(1000);
    int stoppieTime = stoppieTime +1;
  }
  if(stoppieTime > topStoppieTime){
    int topStoppieTime = stoppieTime;
  }
}

void engineTempSensor(){
  float engineTemp = getEngineTemp();
  int engineTempVal = engineTemp;
  
  if(engineTemp > maxEngineTemp){
    int maxEngineTemp = engineTempVal;
  }
  if(engineTemp < minEngineTemp){
    int minEngineTemp = engineTempVal;
  }
  
  if(engineTemp>=70.00){
    int engineTempColor = ST7735_RED;
  } else if(engineTemp>=50 && engineTemp<70){
    int engineTempColor = ST7735_YELLOW;
  } else if(engineTemp<50){
    int engineTempColor = ST7735_GREEN;
  }

  int avgEngineTemp = (maxEngineTemp+minEngineTemp)/2;
}

float getEngineTemp(){
  byte data[12];
  byte addr[8];
  
  if(!ds.search(addr)){
    ds.reset_search();
    return -1000;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0xBE);
  for(int i=0;i<9;i++){
    data[i] = ds.read();
  }
  ds.reset_search();
  byte MSB = data[1];
  byte LSB = data[0];

  float engineTempRead = ((MSB<<8)|LSB);
  float engineTempSum = engineTempRead/16;
  return engineTempSum;
}

void dhtReadSensor(){
  int chkTH = DHT.read11(DHT11_PIN);
  int externalTemp = DHT.temperature;
  int externalHum = DHT.humidity;
  
  if(externalTemp > maxDHTTemp){
    int maxDHTTemp = externalTemp;
  }
  if(externalHum > maxDHTHum){
    int maxDHTHum = externalHum;
  }
  if(externalTemp < minDHTTemp){
    int minDHTTemp = externalTemp;
  }
  if(externalHum < minDHTHum){
    int minDHTHum = externalHum;
  }

  int avgDHTTemp = (maxDHTTemp+minDHTTemp)/2;
  int avgDHTHum = (maxDHTHum+minDHTHum)/2;
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
  tft.setTextColor(engineTempColor);
  tft.print(engineTempVal);
  tft.setCursor(0, 46);
  tft.setTextColor(ST7735_BLUE);
  tft.print("External temp: ");
  tft.setTextColor(ST7735_WHITE);
  tft.print(externalTemp);
  tft.setCursor(0, 56);
  tft.setTextColor(ST7735_BLUE);
  tft.print("External hum: ");
  tft.setTextColor(ST7735_WHITE);
  tft.print(externalHum);
  tft.setCursor(0, 66);
  tft.setTextColor(ST7735_BLUE);
  tft.print("SX turn angle: ");
  tft.setTextColor(ST7735_WHITE);
  tft.print(leftTurnAngle);
  tft.print(" GRD");
  tft.setCursor(0, 76);
  tft.setTextColor(ST7735_BLUE);
  tft.print("DX turn angle: ");
  tft.setTextColor(ST7735_WHITE);
  tft.print(rightTurnAngle);
  tft.print("GRAD");
  tft.setCursor(0, 86);
  tft.setTextColor(ST7735_BLUE);
  tft.print("Wheelie angle: ");
  tft.setTextColor(ST7735_WHITE);
  tft.print(wheelieAngle);
  tft.print(" GRD");
  tft.setCursor(0, 96);
  tft.setTextColor(ST7735_BLUE);
  tft.print("Stoppie angle: ");
  tft.setTextColor(ST7735_WHITE);
  tft.print(stoppieAngle);
  tft.print(" GRD");
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
  engineTemp();
  footerReport();
  delay(3500);
  tft.fillScreen(ST7735_BLACK);
  headerReport();
  externalTempHum();
  footerReport();
  delay(3500);
  tft.fillScreen(ST7735_BLACK);
  headerReport();
  turnAngle();
  footerReport();
  delay(3500);
  tft.fillScreen(ST7735_BLACK);
  headerReport();
  wheelie();
  footerReport();
  delay(3500);
  tft.fillScreen(ST7735_BLACK);
  headerReport();
  stoppie();
  footerReport();
  delay(3500);
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
  tft.print(maxEngineTemp);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(0, 105);
  tft.print("MIN: ");
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.print(minEngineTemp);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(0, 123);
  tft.print("AVG: ");
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.print(avgEngineTemp);
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
  tft.print(maxDHTTemp);
  tft.setCursor(52,80);
  tft.print((char)248);
  tft.setCursor(62,87);
  tft.print("C ");
  tft.print(maxDHTHum);
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
  tft.print(minDHTTemp);
  tft.setCursor(52,98);
  tft.print((char)248);
  tft.setCursor(62,105);
  tft.print("C ");
  tft.print(minDHTHum);
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
  tft.print(avgDHTTemp);
  tft.setCursor(52,116);
  tft.print((char)248);
  tft.setCursor(62,123);
  tft.print("C ");
  tft.print(avgDHTHum);
  tft.setTextSize(1);
  tft.print(" ");
  tft.setTextSize(2);
  tft.print("%");
}

void turnAngle(){
  tft.setCursor(6,45);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLUE);
  tft.print("TURN ANGLE");
  tft.setTextSize(1);
  tft.setCursor(10,65);
  tft.setTextColor(ST7735_MAGENTA);
  tft.print("LEFT");
  tft.setCursor(94,65);
  tft.print("RIGHT");
  tft.drawCircle(64, 126, 56, ST7735_WHITE);
  tft.drawFastVLine(63, 67, 65, ST7735_RED);
  tft.drawFastVLine(65, 67, 65, ST7735_RED);
  tft.fillRect(0, 120 , 128, 60, ST7735_BLACK);
  tft.setCursor(53, 126);
  tft.setTextColor(ST7735_YELLOW);
  tft.print("GRAD");
  tft.setTextSize(2);
  tft.setCursor(20,123);
  tft.setTextColor(ST7735_WHITE);
  tft.print(maxLeftTurnAngle);
  tft.setTextColor(ST7735_WHITE);  
  tft.setCursor(88,123);
  tft.print(maxRightTurnAngle);
}

void wheelie(){
  tft.setCursor(23,45);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLUE);
  tft.print("WHEELIE");
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(0, 97);
  tft.print("TOP ANGLE:  ");
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.print(maxWheelieAngle);
  tft.setTextSize(1);
  tft.print(" GRAD");
  tft.setCursor(0, 115);
  tft.setTextColor(ST7735_YELLOW);
  tft.print("TOP TIME: ");
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.print(topWheelieTime);
  tft.setTextSize(1);
  tft.print("  SEC");
  tft.setCursor(0,74);
  tft.setTextColor(ST7735_YELLOW);
  tft.print("GRIND PLATE: ");
  if(grindPlate==true){
    tft.setTextColor(ST7735_GREEN);
    tft.print("TRUE :D");
  }
  else if(grindPlate==false){
    tft.setTextColor(ST7735_RED);
    tft.print("FALSE :(");
  }
}

void stoppie(){
  tft.setCursor(23,45);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLUE);
  tft.print("STOPPIE");
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(0, 85);
  tft.print("TOP ANGLE:  ");
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.print(maxStoppieAngle);
  tft.setTextSize(1);
  tft.print(" GRAD");
  tft.setCursor(0, 102);
  tft.setTextColor(ST7735_YELLOW);
  tft.print("TOP TIME: ");
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.print(topStoppieTime);
  tft.setTextSize(1);
  tft.print("  SEC");
}
