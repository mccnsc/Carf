#include <SPI.h>              
#include <mcp2515.h>          

/*** LCD Ekran için gerekli ayarlamalar yapıldı.****/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27,16,2); 


/****Değişkenler****/
int durum;
int potValue;
int sicaklik;
int distance;

/*****CAN BUS KONFİGÜRASYONLARI*****/
struct can_frame canMsg;
struct can_frame canMsg2;
struct can_frame canMsg3; 
struct can_frame canMsg4;
MCP2515 mcp2515(10);  // CS pini 10.pine takılmıştır.              



void setup() {
  lcd.init();       
  lcd.backlight();
  delay(1000);
    
  SPI.begin();                      
  
  Serial.begin(9600);                
  
  
  mcp2515.reset();      
  mcp2515.setBitrate(CAN_125KBPS,MCP_8MHZ); 
 
  /*****Filtre Konfigürasyonları*****/
  mcp2515.setConfigMode();                       
  mcp2515.setFilterMask(MCP2515::MASK0, false, 0xFFF0);
  mcp2515.setFilter(MCP2515::RXF0, false, 0x1881);   
  mcp2515.setFilter(MCP2515::RXF1, false, 0x1938);
  mcp2515.setFilterMask(MCP2515::MASK1, false, 0xFFFF);
  mcp2515.setFilter(MCP2515::RXF2, false, 0x0103); 
  mcp2515.setFilter(MCP2515::RXF3, false, 0x0104);
  mcp2515.setFilter(MCP2515::RXF4, false, 0x0106);
  mcp2515.setFilter(MCP2515::RXF5, false, 0x0107);

  mcp2515.setNormalMode();                  
}

void loop() 
{
  if ((mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)&& (mcp2515.readMessage(&canMsg2) == MCP2515::ERROR_OK) 
  && (mcp2515.readMessage(&canMsg3) == MCP2515::ERROR_OK)&& (mcp2515.readMessage(&canMsg4) == MCP2515::ERROR_OK))
  {
     durum = canMsg.data[0];
     distance = canMsg2.data[0];
     potValue = canMsg3.data[0];
     sicaklik = canMsg4.data[0];
       
   
      
     
     /****LED'in LCD'deki konumunu ayarladık****/
      lcd.setCursor(1,0);     
      lcd.print("LED");
      lcd.setCursor(2,1); 
      if(durum==0)
      lcd.print("OFF");
      if(durum==1)        
      lcd.print(" ON");
      
      /****LM35'İN'in LCD'deki konumunu ayarladık****/
      lcd.setCursor(6,0);     
      lcd.print("SIC");
      lcd.setCursor(6,1);
      lcd.print(sicaklik);

      /****POTANSİYOMETRE'NİN'in LCD'deki konumunu ayarladık****/
      lcd.setCursor(12,0);     
      lcd.print("POT");
      lcd.setCursor(12,1);
      lcd.print(potValue);
      

      
      }
      
      delay(250);
      lcd.clear();
    
  }
   
                
