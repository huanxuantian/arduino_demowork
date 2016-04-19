
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
unsigned int localPort = 8888;     
IPAddress timeServer(132, 163, 4, 101);
IPAddress ip(192, 168, 3, 117);//ip地址
IPAddress gw(192, 168, 3, 1);//gateway ip地址
EthernetServer server(80);//访问端口
String fakename="webtime sensor/shinki 1.0.0 (ATmega2560)";//装逼参数(伪装服务器）

const int NTP_PACKET_SIZE= 48; 
byte packetBuffer[ NTP_PACKET_SIZE]; 
EthernetUDP Udp;

#include <TimerOne.h> 
#include <MsTimer2.h>


int adc_key_val[5] ={ 50,200,400, 600,800}; 
int NUM_keyS = 5; 

#define UP     50//2
#define DOWN   51//3
#define LEFT   52//4
#define RIGHT  53//5

#define OK_KEY  2


int adc_key_in1; 
int adc_key_in2; 


int key1=-1; 
int key2=-1; 
int oldkey1=-1;
int oldkey2=-1;
int ok_key=-1;
int ok_key1=-1;
int oldok_key=-1;
int bklight=128;

char *sec[60]={"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51","52","53","54","55","56","57","58","59"};


int index=0;
int minindex=59;
int hourindex=12;   //这里来设定现在时间，目前设定为12：59：00

int ontimeset =-1;
int ontime_recv=-1;

#include "U8glib.h"

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The complete list of supported devices is here: http://code.google.com/p/u8glib/wiki/device

//U8GLIB_SSD1306_128X64 u8g(10, 11, 13, 12);	// SW SPI Com: SCK = 10, MOSI = 11, CS = 13, A0 = 12

U8GLIB_SSD1306_128X64 u8g(3, 5, 7, 6);  // SW SPI Com: SCK = 10, MOSI = 11, CS = 13, A0 = 12

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
    u8g.firstPage();  
  do {
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr( 24, 12, "Wait DHCP");
    //u8g.setFont(u8g_font_osb21);
    u8g.setFont(u8g_font_10x20); 
    u8g.drawStr( 0, 28, "             ");
    u8g.drawStr( 28, 28, "shinki");
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr( 36, 48, "1.0.0");
    
   } while( u8g.nextPage() );   
  //u8g.drawStr( 18, 38, "你好！");
}

// Convert ADC value to key1 number 
int get_key(unsigned int input) 
{ 
    int k; 
     
    for (k = 0; k < NUM_keyS; k++) 
    { 
      if (input < adc_key_val[k]) 
      { 
        if(k<2)
        {
           return 0;
        }
        else if(k>4)
        {
          return 1;
        }
        
            return k; 
       } 
   } 
     
    if (k >= NUM_keyS)k = -1;  // No valid key1 pressed  
    return k; 
} 
char m_str[20]={0};
char m_ip[20]={0};
void ontimer1(){
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
void key() {
  return;
    if(ontimeset<0)
    {
      ontimeset =1;
      index =0;
    }
    else 
    {
      ontimeset=-1;
    }
}
void ontimer2(){  
          //analogWrite(BLK,bklight);
        u8g.firstPage();  
        do {
           //u8g.setFont(u8g_font_unifont);
           u8g.drawFrame(0,1, 128,62);   //内边界
           u8g.drawFrame(0,0, 128,64);   //外边界
           u8g.setFont(u8g_font_6x10); 
           u8g.drawStr( 0, 12, "             ");
           u8g.drawStr( 8, 12, m_ip);
           u8g.setFont(u8g_font_10x20); 
           u8g.drawStr( 0, 36, "             ");
           u8g.drawStr( 8+10, 36, sec[hourindex]);
           u8g.drawStr( 30+10, 36, ":");
           u8g.drawStr( 42+10, 36, sec[minindex]);
           u8g.drawStr( 64+10, 36, ":");
           u8g.drawStr( 76+10, 36, sec[index]);
           u8g.setFont(u8g_font_6x10); 
           u8g.drawStr( 6, 48, "             ");
           u8g.drawStr( 6, 48, m_str);
                       
            if(ontimeset>0)
            {   
              u8g.drawStr( 120, 12, "*");
            }
            else
            {
              u8g.drawStr( 120, 12, " ");       
            }
        } while( u8g.nextPage() );     
        

      adc_key_in1 = analogRead(0);    // read the value from the sensor   
      key1 = get_key(adc_key_in1);  // convert into key1 press 
      
      adc_key_in2 = analogRead(1);    // read the value from the sensor   
      key2 = get_key(adc_key_in2);  // convert into key1 press 
      ok_key =9;
      ok_key1 = digitalRead(OK_KEY);
      if(oldok_key != ok_key1)
      {
        delay(10);
        ok_key1 = digitalRead(OK_KEY);    
        if(oldok_key != ok_key1)
        {
          oldok_key = ok_key1;
           ok_key =0;
          //sprintf(m_str,"ok org");
          if(ok_key1==0)
          {
            if(ontimeset<0)
            {
              ontimeset =1;
              index =0;
            }
            else 
            {
              ontimeset=-1;
            }
          }
        }
      }


//

      
    //ok at right

      if(adc_key_in1<300)
      {
        delay(10);
        adc_key_in1 = analogRead(0);
        if(adc_key_in1<300)//up
        {
            if(ontimeset>0)
            {
              hourindex++;
              if (hourindex==24) hourindex=0;
              index =0;
            }
            else
            {
              //digitalWrite(BLK, 1); 
              bklight = 220;      
              ok_key =2;               
            }
        }
        
      }
      else if(adc_key_in1>800)
      {
        delay(10);
        adc_key_in1 = analogRead(0);
        if(adc_key_in1>800)//down
        {
            if(ontimeset>0)
            { 
              if (hourindex==0) hourindex=24;
              hourindex--; 
              index =0;
            }
            else
            {
              //digitalWrite(BLK,0);
              bklight = 80;  
              ok_key =6;                    
            }
        }
      }

      if(adc_key_in2<300)
      {
        delay(10);
        adc_key_in2 = analogRead(1);
        if(adc_key_in2<300)//right
        {
            if(ontimeset>0)
            {
              minindex++;
              if (minindex==60) minindex=0;
              index =0;
            }
            else
            {
              if(adc_key_in1<300)//右上
              {
                ok_key=3;
              }
              else if(adc_key_in1>800)//右下
              {
                ok_key =5; 
              }
              else
              {
               ok_key =4;                
              }
               
            }
        }
        
      }
      else if(adc_key_in2>800)
      {
        delay(10);
        adc_key_in2 = analogRead(1);
        if(adc_key_in2>800)//left
        {
            if(ontimeset>0)
            { 
              if (minindex==0) minindex=60;
              minindex--;
              index =0;
            }
            else
            {
              if(adc_key_in1<300)//左上
              {
                ok_key=1;
              }
              else if(adc_key_in1>800)//左下
              {
                ok_key =7; 
              }
              else
              {
               ok_key =8;                
              }

            }
        }
      }
      if(adc_key_in1<300&&adc_key_in2>800)
      {
        
      }
      else if(adc_key_in1<300&&adc_key_in2>800)
      {
        ok_key=5;
      }
  
     
       
      sprintf(m_str,"x:%04d y:%04d,0x%02X",adc_key_in1,adc_key_in2,ok_key&0xff);

    

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


void setup(void) {

  

  
  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) 
    u8g.setColorIndex(255);     // white
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
    u8g.setColorIndex(3);         // max intensity
  else if ( u8g.getMode() == U8G_MODE_BW )
    u8g.setColorIndex(1);         // pixel on
                                              
    int count_dhcp=0;                                                                                  
      //Ethernet.begin(mac, ip, gw, gw);
    do    
   {
        draw();
      delay(1000);

      count_dhcp++;
      if(count_dhcp>5)break;
   }while(!Ethernet.begin(mac));//
  server.begin();
  Udp.begin(localPort);
    IPAddress localip = Ethernet.localIP();
  sprintf(m_ip,"ip:%d.%d.%d.%d",localip[0],localip[1],localip[2],localip[3]);
    //attachInterrupt(4, key, FALLING);//外部中断2触发按键中断//pin21//RISING//CHANGE
    pinMode(OK_KEY, INPUT_PULLUP);
    Timer1.initialize(1000000); 
    Timer1.attachInterrupt( ontimer1 );   
    MsTimer2::set(50,ontimer2);
    MsTimer2::start();

  
}

void loop(void) {




      if(ontime_recv<=0)
      {
          Udp.begin(localPort);
          delay(50);
          //sprintf(m_str,"sendtime");
          ontime_recv=0;
          sendNTPpacket(timeServer);
          delay(1000); 
          if ( Udp.parsePacket() ) {  
          sprintf(m_str,"recvtime");
            Udp.read(packetBuffer,NTP_PACKET_SIZE);  
            unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
            unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  
         
            unsigned long secsSince1900 = highWord << 16 | lowWord;  
            //Serial.print("Seconds since Jan 1 1900 = " );
            //Serial.println(secsSince1900);               
          
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
            //sprintf(m_str,"exittime");
            ontime_recv=1;
          }
          
    }

  if(ontime_recv<=0){return;}


  server.begin();
    EthernetClient client = server.available();
    //sprintf(m_str,"wait http cli");
  if (client) {
    //Serial.println("new client");
    String res="" ,query="",path="";
    int resend = 1;
    boolean currentLineIsBlank = true;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        /*
          char e_sock[20]={0};
          sprintf(e_sock,"%1d",client.getSocketNumber());
          u8g.drawStr( 110, 12, e_sock);
          */

        
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
            char attch_msg[120]={0};
            if(strcmp(path.c_str(),"/stick")==0)
            {
              if(strcmp(query.c_str(),"x")==0)
              {
                sprintf(attch_msg,"x=%d",adc_key_in1);
              }
              else if(strcmp(query.c_str(),"y")==0)
              {
                sprintf(attch_msg,"y=%d",adc_key_in2);
              }
            }
            /*
            char t_str[256];
            sprintf(t_str,"path:%s",path.c_str());
            memset(m_str,0,sizeof(m_str));
            memcpy(m_str,t_str,sizeof(m_str)-1);
            */
            
            
            client.println("HTTP/1.1 200 OK");
            client.println("Server: "+fakename);//装逼参数
            client.println("Content-Type: text/html");
            client.println("Connection: close");  
            client.println();
            client.println("<!DOCTYPE HTML><html><head></head><body><h1>200 Success</h1>GET:"+query+"<br />PATH:"+path+"<br />ATTCHMSG:"+attch_msg+"<hr /><p>INPUT:"+m_str+"</p><p>TIME:"+sec[hourindex]+":"+sec[minindex]+":"+sec[index]+"(+8 shanghai)</p><hr /><p>"+fakename+"</p></body></html>");
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

