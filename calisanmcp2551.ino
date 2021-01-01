#include <SPI.h>
#include <mcp2515.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2); 

struct can_frame canMsg; 
MCP2515 mcp2515(10);

const int ledPin = 7;
//boolean ledON = 1;

void setup() {
  lcd.init();
  lcd.backlight();
  delay(1000);

  SPI.begin();

  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);

  mcp2515.reset();

  mcp2515.setBitrate(CAN_125KBPS,MCP_8MHZ); 

 /* mcp2515.setConfigMode();                       //B6D4   1011 0110 1101 0100
  mcp2515.setFilterMask(MCP2515::MASK0, false, 0xFFF0);// 0001 1000 1000 0001
  mcp2515.setFilter(MCP2515::RXF0, false, 0x1881);    //  0000 0011 1100 1101
  mcp2515.setFilter(MCP2515::RXF1, false, 0x1938);
  mcp2515.setFilterMask(MCP2515::MASK1, false, 0xFFFF);// 0001 1001 0011 1000
  mcp2515.setFilter(MCP2515::RXF2, false, 0x0103); 
  mcp2515.setFilter(MCP2515::RXF3, false, 0x0104);
  mcp2515.setFilter(MCP2515::RXF4, false, 0x0106);
  mcp2515.setFilter(MCP2515::RXF5, false, 0x0107);*/

  mcp2515.setNormalMode();
}

void loop() 
{
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)
  {
    int x = canMsg.data[0];

      if(x%2==1)
      digitalWrite(ledPin,HIGH);
      if(x%2==0)
      digitalWrite(ledPin,LOW); 

     Serial.println(x);
     lcd.setCursor(1,0);
      lcd.print("LED STATUS");
      lcd.setCursor(2,1); 
      if(x==0)
     lcd.print("OFF");
      if(x==1)
      lcd.print(" ON");
      //lcd.print(x);
      }
      delay(250);
      //lcd.clear();

  }
