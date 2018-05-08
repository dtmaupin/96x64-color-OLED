

#include "Adafruit_Si7021.h"    //Adafruit Si7021 V 1.0.1
#include <SPI.h>                //Arduino SPI lib
#include "Ucglib.h"             //Ucglib by oliver V1.5.2


  float RH_current;  //set current reading floating point
  int RH_round;  //set int for rounded reading
  int RH_last;  //set int point number for comparions logic
  
  float Temp_current;  //set current reading floating point
  int Temp_round;  //set int for rounded reading
  int Temp_last;  //set int point number for comparions logic

Adafruit_Si7021 sensor = Adafruit_Si7021();  //Setup Sensor Type
Ucglib_SSD1331_18x96x64_UNIVISION_SWSPI ucg(/*sclk=*/ 52, /*data=*/ 51, /*cd=*/ 48, /*cs=*/ 49, /*reset=*/ 50);  //Setup Display and SPI pins

void (*resetFunc)(void) = 0;

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
    //asm volatile("nop");
  }

  Serial.println("Starting up sensor type: Si7021");
  
  if (!sensor.begin()) {
    Serial.println("Did not find Si7021 sensor!");
    resetFunc(); // reset
  }
  //Write fixed text to display
  //ucg.setRotate90();
  ucg.setFont(ucg_font_ncenR10_tr);
  ucg.setColor(0, 50, 240);
  ucg.setPrintPos(0,15);
  ucg.print("RH%: ");
  ucg.setPrintPos(0,40);
  ucg.print("Temp C: "); 
}

void loop(void)
{
  float RH_current = sensor.readHumidity(); //Read sensor data as float
  RH_round=round(RH_current*100);           //Round current reading*100
  float Temp_current = sensor.readTemperature(); //Read sensor data as float
  Temp_round=round(Temp_current*100);           //Round current reading*100
  if(RH_round != RH_last || Temp_round != Temp_last) {
    if(RH_round != RH_last) {                 //If the rounded current reading and the last reading
      ucg.setFont(ucg_font_ncenR10_tr);
      ucg.setPrintPos(40,15);                 //Set starting postion for updating display
      ucg.setColor(0, 0, 0);                  //Set color to black for clearing box
      ucg.drawBox(40, 0, 38, 20);             //Draw a box from 40,0 to 38,20 solid black
      ucg.setColor(0, 255, 0);                //Set color to green.  May add logic for color based on value later
      ucg.print(RH_current);                  //Update the current readint to the display
      ucg.print("%");                         //Print a % this could be moved to static text later.
      RH_last = RH_round;                     //Set to RH_last to rounded value
    }
    if(Temp_round != Temp_last) {
      ucg.setFont(ucg_font_ncenR14_tr);
      ucg.setPrintPos(20,64);                 //Set starting postion for updating display
      ucg.setColor(0, 0, 0);                  //Set color to black for clearing box
      ucg.drawBox(20, 48, 45, 64);             //Draw a box from 40,0 to 38,20 solid black
      ucg.setColor(255, 255, 255);                //Set color to green.  May add logic for color based on value later
      ucg.print(Temp_current);                  //Update the current readint to the display
      ucg.setFont(ucg_font_ncenR10_tr);
      ucg.print(" C");                         //Print a C this could be moved to static text later.
      Temp_last = Temp_round;                     //Set to Temp_last to rounded value
    }
    delay(5000);
    
  } else {
  delay(5000);                            //Delay 1 sec since the sensor updates fast enough for the display to flicker with new values
  }
}
