#include "U8glib.h"
#include "MsTimer2.h"
#define UP     50//2
#define DOWN   51//3
#define LEFT   52//4
#define RIGHT  53//5

#define LENGTH 150     // 蛇长度

struct FOOD {
  int x;       //食物横坐标
  int y;       //食物纵坐标
  int yes;     //判断是否需要出现食物 0：有食物；1：无食物
};
FOOD food = {25, 30, 0};  //食物位置必须为：横坐标i*4+1， 纵坐标i*4+3

struct SNAKE {
  int x[200];
  int y[200];
  int node;      //蛇的节数
  int dir;       //方向   
  int life;      //蛇的生命状态，0：活着；1：死亡
};
SNAKE snake = {{5,1}, {30,30}, 2, RIGHT, 0};

int score = 0;
int level = 1;
int gamespeed = 200;
int i;

unsigned long mouse_Lft;     //记录5次扫描周期内，高低电平变化次数
unsigned long new_mouse_Lft;
unsigned long old_mouse_Lft;
unsigned long mouse_Rht;      //记录5次扫描周期内，高低电平变化次数   
unsigned long new_mouse_Rht;
unsigned long old_mouse_Rht;
unsigned long mouse_Up;     //记录5次扫描周期内，高低电平变化次数
unsigned long new_mouse_Up;
unsigned long old_mouse_Up;
unsigned long mouse_Dwn;     //记录5次扫描周期内，高低电平变化次数
unsigned long new_mouse_Dwn;
unsigned long old_mouse_Dwn;
unsigned long keu_NUM=0;

//U8GLIB_SSD1306_128X64 u8g(10, 11, 13, 12);  // SW SPI Com: SCK = 10, MOSI = 11, CS = 13, A0 = 12

U8GLIB_SSD1306_128X64 u8g(2, 3, 6, 5);  // SW SPI Com: SCK = 10, MOSI = 11, CS = 13, A0 = 12

void key() {
  new_mouse_Up = digitalRead(UP);
  new_mouse_Dwn = digitalRead(DOWN);
  new_mouse_Lft = digitalRead(LEFT);
  new_mouse_Rht = digitalRead(RIGHT);
  Serial.println(new_mouse_Up);
  Serial.println(new_mouse_Dwn);
  Serial.println( new_mouse_Lft);
  Serial.println(new_mouse_Rht);
  if(new_mouse_Rht != old_mouse_Rht||new_mouse_Rht==1) {
    mouse_Rht++;
    old_mouse_Rht = new_mouse_Rht;
  }
  if(new_mouse_Lft != old_mouse_Lft||new_mouse_Lft==1) {
    mouse_Lft++;
    old_mouse_Lft = new_mouse_Lft;
  } 
  if(new_mouse_Up != old_mouse_Up||new_mouse_Up==1) {
    mouse_Up++;
    old_mouse_Up = new_mouse_Up;
  } 
  if(new_mouse_Dwn != old_mouse_Dwn||new_mouse_Dwn==1) {
    mouse_Dwn++;
    old_mouse_Dwn = new_mouse_Dwn;
  }
  if(mouse_Rht > 2){
    if(snake.dir != LEFT)
        snake.dir = RIGHT;
  }
  else if(mouse_Lft > 2){
    if(snake.dir != RIGHT)
       snake.dir = LEFT;
  }
  else if(mouse_Up > 2){
    if(snake.dir != DOWN)
       snake.dir = UP; 
  }
  else if(mouse_Dwn > 2){
    if(snake.dir != UP)
      snake.dir = DOWN;     
  }
  else
    snake.dir = snake.dir; 
  if(keu_NUM==4){
    mouse_Lft = 0;  //次数清零
    mouse_Rht = 0;
    mouse_Up = 0;
    mouse_Dwn = 0; 
    keu_NUM = 0;
  }
  else 
    keu_NUM++; 
}


void key1() {
  new_mouse_Up = digitalRead(UP);
  new_mouse_Dwn = digitalRead(DOWN);
  new_mouse_Lft = digitalRead(LEFT);
  new_mouse_Rht = digitalRead(RIGHT);
  
  if(new_mouse_Rht != old_mouse_Rht||new_mouse_Rht==1) {
    mouse_Rht++;
    old_mouse_Rht = new_mouse_Rht;
  }
  if(new_mouse_Lft != old_mouse_Lft||new_mouse_Lft==1) {
    mouse_Lft++;
    old_mouse_Lft = new_mouse_Lft;
  } 
  if(new_mouse_Up != old_mouse_Up||new_mouse_Up==1) {
    mouse_Up++;
    old_mouse_Up = new_mouse_Up;
  } 
  if(new_mouse_Dwn != old_mouse_Dwn||new_mouse_Dwn==1) {
    mouse_Dwn++;
    old_mouse_Dwn = new_mouse_Dwn;
  }
  if(new_mouse_Rht==1){
    //Serial.println("right");
    if(snake.dir != LEFT)
        snake.dir = RIGHT;
  }
  else if(new_mouse_Lft==1){
     //Serial.println("left");
    if(snake.dir != RIGHT)
       snake.dir = LEFT;
  }
  else if(new_mouse_Up==1){
   // Serial.println("up");
    if(snake.dir != DOWN)
       snake.dir = UP; 
    mouse_Up = 0;
  }
  else if(new_mouse_Dwn==1){
   // Serial.println("down");
    if(snake.dir != UP)
      snake.dir = DOWN;    
  }
  else
    snake.dir = snake.dir; 
}



//组成单位，4X4像素点的方块。
const uint8_t ele[] PROGMEM = {
  0xf0, //B1111000
  0xb0, //B1011000
  0xd0, //B1101000
  0xf0, //B1111000
};

//节显示，食物或蛇的构成最小单位 
void element(int x, int y) {
  u8g.drawBitmapP(x,y, 1,4, ele);
}



//游戏基本界面
void UI() {
  u8g.drawFrame(0,1, 102,62);   //内边界
  u8g.drawFrame(0,0, 102,64);   //外边界
  u8g.setFont(u8g_font_5x7);    //设置字体
  u8g.drawStr(104,12, "LEVEL"); //等级提示
  u8g.drawStr(104,40, "SCORE"); //分数提示
}

void printScore(int x, int y, int s) {
  u8g.setFont(u8g_font_6x10);
  u8g.setPrintPos(x, y);
  u8g.print(s);
}

void gmaeOver(){
  u8g.firstPage();
  do {
    UI();
    printScore(109, 22, level);
    printScore(109, 50, score);
    u8g.setFont(u8g_font_8x13);    //设置字体
    u8g.drawStr(15,35, "GAME OVER"); 
  }while(u8g.nextPage()); 
}
void gmaePrefect(){
  u8g.firstPage();
  do {
    UI();
    printScore(109, 22, level);
    printScore(109, 50, score);
    u8g.setFont(u8g_font_8x13);    //设置字体
    u8g.drawStr(23,35, "PERFECT"); 
  }while(u8g.nextPage()); 
}

void gamePlay() { 
    if(food.yes == 1){         //需要出现新食物
      food.x = random(25)*4 + 1;
      food.y = random(15)*4 + 2; 
      food.yes = 0;  //有食物
    }
    if((snake.x[0] == food.x) && (snake.y[0] == food.y)) {  //吃到食物
      snake.x[0] = food.x;
      snake.y[0] = food.y;      
      snake.node += 1;
      food.yes = 1;
      score += 1;
      level = score/10+1;
      gamespeed = 210 - level*10; 
    }
    for(i=snake.node-1;i>0;i--) {     //蛇的每个环节往前移动,
      snake.x[i] = snake.x[i-1];
      snake.y[i] = snake.y[i-1];
    }
    switch(snake.dir) {
      case RIGHT: 
          snake.x[0] += 4;
          if(snake.x[0]>=101) {
            snake.x[0] = 1;
          } break;
      case UP: 
          snake.y[0] -= 4;
          if(snake.y[0]<=1) {
            snake.y[0] = 58;
          } break;
       case LEFT: 
          snake.x[0] -= 4;
          if(snake.x[0]<=0) {
            snake.x[0] = 97;
          } break;
        case DOWN:
          snake.y[0] += 4;
          if(snake.y[0]>=62) {
            snake.y[0] = 2;
          } break;
    }    
    for(i=3;i<snake.node;i++){   //判断是否撞到自己
      if((snake.x[i] == snake.x[0]) && (snake.y[i] == snake.y[0])){
        snake.life = 1;          //游戏结束    
        break;
      }
    }   
  
}
void setup() {
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT); 
  //Serial.begin(9600);
  attachInterrupt(4, key1, CHANGE);//外部中断2触发按键中断//pin21
  //MsTimer2::set(10, key); //定时器中断扫描按键
  //MsTimer2::start( );
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_10x20);    //设置字体
    u8g.drawStr(34,25, "snake");
    u8g.setFont(u8g_font_7x13);
    u8g.drawStr(10,40, "www.yfrobot.com");   
  }while(u8g.nextPage());
  delay(2000);
}
void loop() {
  
  u8g.firstPage();
  do {
    UI();
    if(snake.life)
      gmaeOver(); 
    else if(level == 9)
      gmaePrefect();
    else
      for(i=0; i<snake.node;i++){
        element(snake.x[i], snake.y[i]);
      }
    element(food.x, food.y);
    printScore(109, 22, level);
    printScore(109, 50, score);
  }while(u8g.nextPage());
  gamePlay();
  delay(gamespeed);
}
