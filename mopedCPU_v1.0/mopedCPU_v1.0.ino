#include <TFT.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
#define cs 10
#define dc 9
#define rst 8

TFT TFTscreen = TFT(cs,dc,rst);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(){
  Serial.begin(9600);
  sensors.begin();
  TFTscreen.begin();
  TFTscreen.background(0,0,0); //black background
  TFTscreen.setTextSize(2);
}

void loop(){
  sensors.requestTemperatures();
  int engineTemperature = sensors.getTempCByIndex(0);
  Serial.print("Engine temperature: ");
  Serial.print(engineTemperature);
  Serial.print(" °C");
}
