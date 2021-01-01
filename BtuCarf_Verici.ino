#include <SPI.h>    // SPI kütüphanesini ekledik.     
#include <mcp2515.h>     //  mcp2515 kütüphanesini ekledik.
/***LED-BUTON**/
#define BUTON 4  
#define ledPin 7 
bool durum; 
/***MESAFE SENSÖRÜ**/
#define trig 2
#define echo 3
long duration, distance;
/***POTANSİYOMETRE**/
#define pot A1
int potValue; 
/***SICAKLIK SENSÖRÜ**/
int lm35Pin = A0;
int okunan_deger = 0;
float sicaklik_gerilim = 0;
float sicaklik = 0;

/**** CAN BUS MESAJLARINI TANIMLADIK*****/  
struct can_frame canMsg;
struct can_frame canMsg2;
struct can_frame canMsg3;
struct can_frame canMsg4;

MCP2515 mcp2515(10); // CS pinini 10.pine bağladık.

void setup() 
{
  Serial.begin(9600);
 
  SPI.begin();     // SPI'yı başlattık.         
/****CAN BUS KONFİGÜRASYONLARI****/
  mcp2515.reset(); // CAN Bus hattını boşalttık.
  mcp2515.setBitrate(CAN_125KBPS,MCP_8MHZ); // CAN Bus veri aktarma hızını yazdık.
  mcp2515.setNormalMode(); 

  canMsg.can_id  = 0x1935;  // Mesajlarımıza ID atadık.
  canMsg2.can_id  = 0x0105; 
  canMsg3.can_id  = 0x1938; 
  canMsg4.can_id  = 0x1938;          
  
  canMsg.can_dlc = 1; //Mesajlarımızın boyutlarını belirledik.
  canMsg2.can_dlc = 1;
  canMsg3.can_dlc = 1; 
  canMsg4.can_dlc = 1; 
  
  /****MESAFE SENSÖRÜ AYARLAMALARI****/
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  

  /****LED-BUTON AYARLAMALARI****/
  pinMode(BUTON,INPUT);
  pinMode(ledPin,OUTPUT);

                  
 
}

void loop() 
{

  /****LED-BUTON*****/
   durum= digitalRead(BUTON);
     if(durum==1)
     digitalWrite(ledPin,HIGH);      
     if(durum==0)
     digitalWrite(ledPin,LOW);
 
  /****MESAFE SENSÖRÜ*****/
  digitalWrite(trig, LOW);        
  delayMicroseconds(2);              
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);           
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration*.0343)/2; 
   
  

  /****POTANSİYOMETRE*****/  
  potValue = analogRead(A1);
  potValue = map(potValue,0,1023,0,255);
  

  /****Sıcaklık Sensörü*****/
  okunan_deger = analogRead(lm35Pin);
  sicaklik_gerilim = (okunan_deger / 1023.0)*5000;
  sicaklik = sicaklik_gerilim /10.0;
  
  
  canMsg.data[0] = durum;     //Gönderilecek verileri belirledik.
  canMsg2.data[0] = distance;  
  canMsg3.data[0] = potValue; 
  canMsg4.data[0] = sicaklik;      
  
  
  mcp2515.sendMessage(&canMsg);  // Mesajları gönderdik.
  mcp2515.sendMessage(&canMsg2); 
  mcp2515.sendMessage(&canMsg3);
  mcp2515.sendMessage(&canMsg4); 
  delay(200);
}
