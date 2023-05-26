#include <SPI.h>
#include <ILI9341_due_config.h>
#include <ILI9341_due.h>
#include <SystemFont5x7.h>

#define TFT_DC 9
#define TFT_CS 10
#define TFT_MOSI 11
#define TFT_CLK 13
#define TFT_RST 7
#define TFT_MISO 12
#define up_btn1 6
#define down_btn1 5
#define up_btn2 4
#define down_btn2 3

#include "fonts/Arial_bold_14.h"

int x = 15;
int dx = 1;
int y = 120;
int stopping = 20;
int player1 = 150;
int player2 = 150;
int left_points = 0;
int right_points = 0;
bool collides_r_player_wall = false;
bool collides_l_player_wall = false;
bool collides_top = false;
bool collides_bottom = false;

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9341_due tft = ILI9341_due(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println(F("ILI9341 Test!")); 

  pinMode(up_btn1, INPUT);
  pinMode(down_btn1, INPUT);
  pinMode(up_btn2, INPUT);
  pinMode(down_btn2, INPUT);
 
  tft.begin();

  
  tft.setRotation(2); //2 QUER
  
  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print(F("Display Power Mode: 0x")); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print(F("MADCTL Mode: 0x")); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print(F("Pixel Format: 0x")); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print(F("Image Format: 0x")); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print(F("Self Diagnostic: 0x")); Serial.println(x, HEX); 

  tft.fillScreen(ILI9341_BLACK);
}



void player_1 (){

  if(player1 <= y && player1 + 30 >= y){
    if(x >= 15 && x <= 20){
      dx = 1;
      collides_l_player_wall = true;
      }
    }
  
  if (digitalRead(up_btn1) == HIGH) {
    
    // Taste "UP" gedrückt, Spieler 1 nach oben bewegen
     tft.fillRect(player1, 10, 30, 5, ILI9341_BLACK);  
    player1 -= 1;
     
  }
  if (digitalRead(down_btn1) == HIGH) {
    // Taste "DOWN" gedrückt, Spieler 1 nach unten bewegen
     tft.fillRect(player1, 10, 30, 5, ILI9341_BLACK);
    player1 += 1;
  }

  if (player1 < 0) {
    player1 = 0;
  }
  if (player1 > tft.width() ) {
    player1 = tft.width() ;
  }
   if (player1 > tft.width() - 30) {
    player1 = tft.width() - 30;
  }
  
  
 tft.fillRect(player1, 10, 30, 5, ILI9341_WHITE);
 }  

void player_2(){

  if(player2 <= y && player2 + 30 >= y){
    if(x <= tft.height() - 15 && x >= tft.height()-20){
      collides_r_player_wall = true;
      }
  }
  if (digitalRead(up_btn2) == HIGH) {
    
    // Taste "UP" gedrückt, Spieler 1 nach oben bewegen
     tft.fillRect(player2, tft.height() - 15, 30, 5, ILI9341_BLACK);
    player2 -= 1;
     
  }
  if (digitalRead(down_btn2) == HIGH) {
    // Taste "DOWN" gedrückt, Spieler 1 nach unten bewegen
     tft.fillRect(player2,  tft.height() - 15, 30, 5, ILI9341_BLACK);
    player2 += 1;
  }

  if (player2 < 0) {
    player2 = 0;
  }
  if (player2 > tft.width() ) {
    player2 = tft.width() ;
  }
   if (player2 > tft.width() - 30) {
    player2 = tft.width() - 30;
  }

  
 tft.fillRect(player2, tft.height() - 15, 30, 5, ILI9341_WHITE);


  
  }



   void ball() {
  tft.fillCircle(y, x, 6, ILI9341_BLACK);
  tft.fillCircle(y, x, 5, ILI9341_RED);
  delay(3);
  x += dx;
  
  if (x >= tft.height()) {
   collides_r_player_wall == true;
  }
    else if (x <= 6) {
   collides_l_player_wall == true;   
  }
 
  if (y >= tft.width()){
    collides_bottom =  true;
    } 

  if (y <= 6){
    collides_top = true;
    }  
}

void ball_move(){
 if (collides_r_player_wall == true) {
      x = -1;
    collides_r_player_wall = false;
  }
  if (collides_l_player_wall == true) {
     // abprallen  
    collides_l_player_wall = false;
  }
  if (collides_top == true) {
     // abprallen
     collides_top == false;
    }
   if (collides_bottom = true){
    // abprallen
     collides_bottom = false;
    } 

    
}
    

void score (){
   if (x >= tft.height()) {
    left_points ++;

  }
    else if (x <= 6) {
    right_points ++;   
  }
  
  tft.setRotation(3);
  tft.cursorTo(0, 0);
  tft.setFont(Arial_bold_14);
  tft.setTextLetterSpacing(5);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLUE);
  tft.printAligned("Score:" + String(left_points)+ "|" + String(right_points), gTextAlignTopCenter);
  tft.setRotation(2);
  
  }




void loop() {

  score();
  ball(); 
  player_1();
  ball_move();  
  player_2();
  ball_move();
}
