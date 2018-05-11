#include <PrintEx.h>



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
  //ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  ucg.begin(UCG_FONT_MODE_SOLID);
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
  ucg.setFont(ucg_font_7x14B_mr);
  ucg.setColor(0, 50, 240);
  ucg.setPrintPos(0,15);
  ucg.print("RH:   ");
  ucg.setPrintPos(0,40);
  ucg.setFont(ucg_font_7x14B_mr);
  ucg.print("Temperature: "); 
  ucg.setColor(1, 0, 255, 255);
  ucg.setColor(0, 255, 255, 0);
  ucg.drawGradientLine(0, 22, 94, 0);

  ucg.setColor(0, 0, 255, 255);
  ucg.setColor(1, 255, 255, 0);
  ucg.drawGradientLine(0, 45, 94, 0);
}

void loop(void)
{
  double RH_current = sensor.readHumidity(); //Read sensor data as float
  RH_round=round(RH_current*100);           //Round current reading*100
  double Temp_current = sensor.readTemperature(); //Read sensor data as float
  Temp_round=round(Temp_current*100);           //Round current reading*100
  
  if(RH_round != RH_last || Temp_round != Temp_last) {
    if(RH_round != RH_last) {                 //If the rounded current reading and the last reading
      ucg.setFont(ucg_font_7x14B_mr);         //Set font to 11px tall solid font
      ucg.setPrintPos(40,15);                 //Set starting postion for updating display
      ucg.setColor(1, 0, 0, 0);               //Set background color to black

      if(RH_current < 50) {                   //If RH% is less than 50%
       ucg.setColor(0, 0, 255, 0);            //Set font color to green.
      } else {
       ucg.setColor(0, 255, 0, 0);            //Set font color to red 
      }
      char buffer[128];
      sprintf(buffer, "%7.2f%%", RH_current);
      Serial.println(buffer);
      ucg.drawString(40,15, 0, buffer);
      //ucg.print("%");                         //Print a % this could be moved to static text later.
      RH_last = RH_round;                     //Set to RH_last to rounded value
    }
    if(Temp_round != Temp_last) {
      ucg.setFont(ucg_font_10x20_mr);       //Set larger temp font for solid chars
      double TempF = Temp_current*1.8+32;      //Do math to convert C to F
      ucg.setColor(0, 255, 255, 255);         //Set color to white.  May add logic for color based on value later
      char tbuffer[128];
      sprintf(tbuffer, "%7.2f   ", TempF);
      Serial.println(tbuffer);
      ucg.setClipRange(15, 47, 65, 25);
      ucg.drawString(8,64, 0, tbuffer);
      ucg.undoClipRange();
      //ucg.print(TempF);                       //Update the current Temp reading to the display
      ucg.setPrintPos(84,64);                 //Set starting postion for updating display
      ucg.setFont(ucg_font_7x14B_mr);         //Set smaller solid font for units
      ucg.print("F");                        //Print a F this could be moved to static text later.
      Temp_last = Temp_round;                 //Set to Temp_last to rounded value
    }  
  } 
}
