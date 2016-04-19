#include <SPI.h>
#include <Ethernet.h>
//#include <Dhcp.h>
//#include <EthernetUdp.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};//mac地址
unsigned int localPort = 8888;     
IPAddress timeServer(132, 163, 4, 101); 
IPAddress ip(192, 168, 3, 117);//ip地址
IPAddress gw(192, 168, 3, 1);//gateway ip地址
const int NTP_PACKET_SIZE= 48; 
byte packetBuffer[ NTP_PACKET_SIZE]; 
EthernetUDP Udp;
/* 
  The circuit: 
 * LCD RS pin to digital pin 8 
 * LCD Enable pin to digital pin 9 
 * LCD D4 pin to digital pin 4 
 * LCD D5 pin to digital pin 5 
 * LCD D6 pin to digital pin 6 
 * LCD D7 pin to digital pin 7 
 * LCD BL pin to digital pin 10 
* KEY pin to analogl pin 0  
*/  
#include <LiquidCrystal.h>  
#include <TimerOne.h> 
#include <MsTimer2.h>
LiquidCrystal lcd1(8, 13, 9, 4, 5, 6, 7);  
LiquidCrystal lcd(8, 2, 9, 3, 5, 6, 7);  
char msgs[5][16] = {"Right Key OK ", 
                    "Up Key OK    ",                 
                    "Down Key OK  ",  
                    "Left Key OK  ",  
                    "Select Key OK" };   
int adc_key_val[5] ={50, 200, 400, 600, 800 }; 
int NUM_KEYS = 5; 
int adc_key_in; 
int key=-1; 
int oldkey=-1;

int BLK =1;
int bklight=128;

int ontimeset =-1;

#define INFO "SHINKI STATION"
char *sec[60]={"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51","52","53","54","55","56","57","58","59"};


int index=0;
int minindex=59;
int hourindex=12;   //这里来设定现在时间，目前设定为12：59：00

// Convert ADC value to key number 
int get_key(unsigned int input) 
{ 
    int k; 
     
    for (k = 0; k < NUM_KEYS; k++) 
    { 
      if (input < adc_key_val[k]) 
 { 
            return k; 
        } 
   } 
     
    if (k >= NUM_KEYS)k = -1;  // No valid key pressed  
    return k; 
} 
void ontimer1(){
         lcd.setCursor(3,1);
         lcd.print(sec[hourindex]);
         lcd.setCursor(5,1);
         lcd.print(":");
         lcd.setCursor(6,1);
         lcd.print(sec[minindex]);
         lcd.setCursor(8,1);
         lcd.print(":");
         lcd.setCursor(9,1);
         lcd.print(sec[index]);
        if(ontimeset>0)
        {   
          lcd.setCursor(12, 1); 
          lcd.print("*");
        }
        else
        {
          lcd.setCursor(12, 1); 
          lcd.print(" ");         
        }
        if(ontimeset<0)
        {   
         if (index==59)
         {
           index=-1;
           minindex++;
           if (minindex==60)
           {
             minindex=0;
             hourindex++;
             if (hourindex==24)
             {
               hourindex=0;
             }
           }

         }
          index++;
        }
}
void bklight_set(){
  //analogWrite(BLK,bklight);
}

unsigned long sendNTPpacket(IPAddress& address)
              {
              
                      memset(packetBuffer, 0, NTP_PACKET_SIZE); 
                    
                      packetBuffer[0] = 0b11100011;   
                      packetBuffer[1] = 0;     
                      packetBuffer[2] = 6;     
                      packetBuffer[3] = 0xEC;  
                     
                      packetBuffer[12]  = 49; 
                      packetBuffer[13]  = 0x4E;
                      packetBuffer[14]  = 49;
                      packetBuffer[15]  = 52;
                            
                      Udp.beginPacket(address, 123); 
                      Udp.write(packetBuffer,NTP_PACKET_SIZE);
                      Udp.endPacket(); 
              }
 
void setup (void) {
    lcd.clear();   
    lcd.begin(16, 2); 
    lcd.setCursor(1,0);
    lcd.print(INFO); 
   Ethernet.begin(mac);
      //return;
    
    Udp.begin(localPort);
    pinMode(BLK, OUTPUT);
     digitalWrite(BLK, 1); 
    // analogWrite(BLK,bklight);
    Timer1.initialize(1000000);
    Timer1.attachInterrupt( ontimer1 );   
            
}

void loop (void) {
    while(1)
    {
        sendNTPpacket(timeServer);
        delay(1000); 
        if ( Udp.parsePacket() ) {  
        
          Udp.read(packetBuffer,NTP_PACKET_SIZE);  
          lcd.print("udp ok");
          unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
          unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  
       
          unsigned long secsSince1900 = highWord << 16 | lowWord;  
          //Serial.print("Seconds since Jan 1 1900 = " );
          //Serial.println(secsSince1900);               
             lcd.setCursor(0,1);
             lcd.print("                ");
             lcd.setCursor(0,1);
            lcd.print("DHCP Udp ok");
        
         // Serial.print("Unix time = ");
       
          const unsigned long seventyYears = 2208988800UL;     
       
          unsigned long epoch = secsSince1900 - seventyYears;  
       
          //Serial.println(epoch);      
          
          //Serial.print("beijing time is "); 
      if((epoch % 86400L) / 3600+8>=24) 
                            {
                            //Serial.print((epoch % 86400L) / 3600+8-24);
                            hourindex=(epoch % 86400L) / 3600+8-24;
                            }
      if((epoch % 86400L) / 3600+8<24)
                            {                   
                            //Serial.print((epoch % 86400L) / 3600+8); 
                           hourindex=(epoch % 86400L) / 3600+8;//+8 
                            }
          //Serial.print(':');  
          if ( ((epoch % 3600) / 60) < 10 ) {
                             //Serial.print('0');
                             }
          //Serial.print((epoch  % 3600) / 60); 
          minindex=(epoch  % 3600) / 60;
          
          //Serial.print(':'); 
          if ( (epoch % 60) < 10 )
                                {
                                  //Serial.print('0');
                                }
          //Serial.println(epoch `); 
          index=(epoch % 60);
             lcd.setCursor(0,1);
             lcd.print("                ");
         lcd.setCursor(3,1);
         lcd.print(sec[hourindex]);
         lcd.setCursor(5,1);
         lcd.print(":");
         lcd.setCursor(6,1);
         lcd.print(sec[minindex]);
         lcd.setCursor(8,1);
         lcd.print(":");
         lcd.setCursor(9,1);
         lcd.print(sec[index]);
          break;
        }
}

             lcd.setCursor(0,0);
             lcd.print("                ");
             lcd.setCursor(0,0);
             lcd.print("TIME NET OK");
    while(1)
    {
            adc_key_in = analogRead(0);    // read the value from the sensor   
          key = get_key(adc_key_in);  // convert into key press 
          
          if (key != oldkey)   // if keypress is detected 
           { 
            delay(10);  // wait for debounce time 
            adc_key_in = analogRead(0);    // read the value from the sensor   
            key = get_key(adc_key_in);    // convert into key press 
            if (key != oldkey)     
            { 
              oldkey = key; 
              if (key >=0){ 
                   //lcd.print(msgs[key]); 
                   switch(key){
                    case 0://right
                      if(ontimeset>0)
                      {
                        minindex++;
                        if (minindex==60) minindex=0;
                        index =0;
                      }
                      break;
                    case 1://up
                      if(ontimeset>0)
                      {
                        hourindex++;
                        if (hourindex==24) hourindex=0;
                        index =0;
                      }
                      else
                      {
                        digitalWrite(BLK, 1); 
                        bklight = 220;                     
                      }
                      break;
                    case 2://down
                      if(ontimeset>0)
                      { 
                        if (hourindex==0) hourindex=24;
                        hourindex--; 
                        index =0;
                      }
                      else
                      {
                        digitalWrite(BLK,0);
                        bklight = 80;                     
                      }
                      break;
                    case 3://left
                     if(ontimeset>0)
                      { 
                        if (minindex==0) minindex=60;
                        minindex--;
                        index =0;
                      }
                      break;
                    case 4://select
                      if(ontimeset<0)
                      {
                        ontimeset =1;
                        index =0;
                      }
                      else 
                      {
                        ontimeset=-1;
                      }
                      break;
                      
                   }
              }
            } 
          }
          //analogWrite(BLK,bklight);

    }
    
    

  
}
