//for w5100 model
#include <SPI.h>
#include <Ethernet.h>
/*
* ethernet spi interface 
* SCK pin to digtal pin 13
* MISO pin to digtal pin 12
* MOSI pin to digtal pin 11
* SS for Ethernet to digtal pin 10
* SS for SD card to digtal pin 4
*/
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};//mac地址
IPAddress ip(192, 168, 3, 117);//ip地址
IPAddress gw(192, 168, 3, 1);//gateway ip地址
EthernetServer server(80);//访问端口
String fakename="Nginx/1.8.0 (Ubuntu 12.04 LTS)";//装逼参数(伪装服务器）
/* 
  The circuit: 
 * LCD RS pin to digital pin 8 
 * LCD RW pin to digital pin 2
 * LCD Enable pin to digital pin 9 
 * LCD D4 pin to digital pin 3
 * LCD D5 pin to digital pin 5 
 * LCD D6 pin to digital pin 6 
 * LCD D7 pin to digital pin 7 
 * LCD BL pin to digital pin 1
* KEY pin to analogl pin 0  
*/  
#include <LiquidCrystal.h>  
#include <TimerOne.h> 
#include <MsTimer2.h>
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
              lcd.setCursor(0, 1); 
            lcd.print("                ");
            lcd.setCursor(0, 1); 
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
void ontimer2(){  

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
                   lcd.setCursor(0,0); 
                    lcd.print("ip:"); 
                    lcd.print(Ethernet.localIP()); 
              }
            } 
          }
          //analogWrite(BLK,bklight);
          

}
void bklight_set(){
  //analogWrite(BLK,bklight);
}
 
void setup (void) {
  //Serial.begin(9600);
  /*
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  */
  Ethernet.begin(mac, ip, gw, gw);
  server.begin();
  //Serial.print("ip:");
  //Serial.println(Ethernet.localIP());
  lcd.clear();
  lcd.begin(16, 2); 
  lcd.setCursor(0,0);   
  lcd.print("ip:"); 
  lcd.print(Ethernet.localIP()); 
  //Serial.println("lcd init");
    pinMode(BLK, OUTPUT);
     digitalWrite(BLK, 1); 
    // analogWrite(BLK,bklight);
    Timer1.initialize(1000000);
    Timer1.attachInterrupt( ontimer1 );   
    MsTimer2::set(20,ontimer2);
    MsTimer2::start();
            
}

void loop (void) {

    EthernetClient client = server.available();
  if (client) {
    //Serial.println("new client");
    String res="" ,query="",path="";
    int resend = 1;
    boolean currentLineIsBlank = true;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        ////Serial.write(c);
        //只读取一行
        if (c != '\n' && resend) {
          res += c;
        } else {
          resend = 0;
        }
 
        if (c == '\n' && currentLineIsBlank) {
          //判断GET头完整性
          if ((res.indexOf("GET ") != -1) && (res.indexOf(" HTTP") != -1)) {
            //判断是否存在get参数
            if (res.indexOf('?') != -1) {
              query = res.substring(res.indexOf('?') + 1, res.indexOf(" HTTP"));
              path = res.substring(res.indexOf("GET ") + 4, res.indexOf('?'));
            } else {
              path = res.substring(res.indexOf("GET ") + 4, res.indexOf(" HTTP"));
            }
             
            //Serial.println(res);
            //Serial.println("GET:"+query);
            //Serial.println("path:"+path);
            
            lcd.setCursor(0, 0); 
            lcd.print("                ");
            lcd.setCursor(0, 0); 
          
            lcd.print("path:"+path);
            
            client.println("HTTP/1.1 200 OK");
            client.println("Server: "+fakename);//装逼参数
            client.println("Content-Type: text/html");
            client.println("Connection: close");  
            client.println();
            client.println("<!DOCTYPE HTML><html><head></head><body><h1>200 Success</h1>GET:"+query+"<br />PATH:"+path+"<br /><p>TIME:"+sec[hourindex]+":"+sec[minindex]+":"+sec[index]+"(+8 shanghai)</p><hr /><p>"+fakename+"</p></body></html>");
            break;
          } else {
            //GET头不完整 返回错误信息
            client.println("HTTP/1.1 403 Forbidden");
            client.println("Server: "+fakename);//装逼参数(伪装服务器）
            client.println("Content-Type: text/html");
            client.println("Connection: close");  
            client.println();
            client.println("<!DOCTYPE HTML><html><head></head><body><h1>403 Forbidden</h1><hr /><p>"+fakename+"</p></body></html>");
            break;
          }
        }
          if (c == '\n') {
            // you're starting a new line
            currentLineIsBlank = true;
          }
          else if (c != '\r') {
            // you've gotten a character on the current line
            currentLineIsBlank = false;
          }
        }
      }
      // give the web browser time to receive the data
      delay(1);
      // close the connection:
      client.stop();
      //Serial.println("client disconnected");
    
       
    }
    
    

  
}
