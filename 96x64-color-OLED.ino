/*

  18 Feb 2014:
    ROM Size	Font Mode
    9928		NONE
    10942		TRANSPARENT	(1014 Bytes)
    11650		SOLID  (1712 Bytes)
    12214		TRANSPARENT+SOLID
  
  
*/

#include "Adafruit_Si7021.h"
#include <SPI.h>
#include "Ucglib.h"

  float RH;
  float RH_current;
  float RH_Logic;
  double TempC;
  long lastreading = "0";
Adafruit_Si7021 sensor = Adafruit_Si7021();

    
/*
  Hardware SPI Pins:
    Arduino Uno		sclk=13, data=11
    Arduino Due		sclk=76, data=75
    Arduino Mega	sclk=52, data=51
    
  >>> Please uncomment (and update) one of the following constructors. <<<  
*/

Ucglib_SSD1331_18x96x64_UNIVISION_SWSPI ucg(/*sclk=*/ 52, /*data=*/ 51, /*cd=*/ 48, /*cs=*/ 49, /*reset=*/ 50);

void setup(void)
{
  delay(1000);
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  //ucg.begin(UCG_FONT_MODE_SOLID);
  ucg.clearScreen();

  //Si2021 Section
  Serial.begin(115200);

  // wait for serial port to open
  while (!Serial) {
    delay(10);
  }

  Serial.println("Si7021 test!");
  Serial.print("RH Debug Value:"); Serial.println(RH);
  
  if (!sensor.begin()) {
    Serial.println("Did not find Si7021 sensor!");
    while (true);
  }
}

void loop(void)
{
  //ucg.setRotate90();
  
  
  ucg.setFont(ucg_font_ncenR10_tr);
  ucg.setColor(0, 50, 240);
  ucg.setPrintPos(0,15);
  ucg.print("RH%: "); 
  float RH_current = sensor.readHumidity();
  Serial.print("RH_Logic before if: "); Serial.println(RH_Logic); //More debug
  Serial.print("RH_current before if: "); Serial.println(RH_current); //More debug
  if(RH_current != RH_Logic) {
    ucg.setColor(0, 0, 0);
    ucg.drawBox(40, 0, 38, 20);
    ucg.setColor(0, 255, 0);
    ucg.print(RH_current);
    ucg.print("%");
    Serial.print("RH_current: "); Serial.println(RH_current); // Serial debug info
    RH_Logic = RH_current;
    Serial.print("RH_Logic: "); Serial.println(RH_Logic); //More debug
    delay(1000);    
  } else {
  //RH = sensor.readHumidity();
  //float TempC=sensor.readTemperature();
  delay(1000);  
  
  }
  Serial.print("RH_Logic after if: "); Serial.println(RH_Logic); //More debug
  Serial.print("RH_current after if: "); Serial.println(RH_current); //More debug
}
