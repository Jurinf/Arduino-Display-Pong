/*#include <SPI.h>
#include <ILI9341_due_config.h>
#include <ILI9341_due.h>
#include <SystemFont5x7.h>

#define TFT_DC 9
#define TFT_CS 10
#define TFT_MOSI 11
#define TFT_CLK 13
#define TFT_RST 7
#define TFT_MISO 12
#define up_btn1  5
#define setup_black 6
#define setup_red 5
#define setup_blue 4
#define setup_green 8
#define setup_yellow 3
#define down_btn1 6
#define up_btn2 4
#define down_btn2 3


#include "fonts/Arial_bold_14.h"

int x = 170;
int dx = 1;
int dy = 0;
int y = 120;
int ball_speed = 2;
int player_speed = 2;
int stopping = 20;
int player1 = 150;
int player2 = 150;
int left_points = 0;
int right_points = 0;
int win_points = 10;
int txt_color = ILI9341_WHITE;
int txt_bg_color = ILI9341_BLACK;
int bg_fill_color = ILI9341_BLACK;
bool collides_r_player = false;
bool collides_r_wall = false;
bool collides_l_wall = false;
bool collides_l_player = false;
bool collides_top = false;
bool collides_bottom = false;
bool game_running = false;
bool rules_running = true;
bool menu_running = false;
bool game_paused = false;
bool btn_state = LOW;
bool prev_btn_state = LOW;

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9341_due tft = ILI9341_due(TFT_CS, TFT_DC, TFT_RST);

int long_random(int seed)
{
    long p=10007;
      long q=100003;
     long n=p*q;
     long output;
     output=seed;
     while(1)
    {
        output=(output*output)%n;
        if(output<1000000000)
        {
            break;
        }
    }
    return output;
} 

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println(F("ILI9341 Test!")); 

  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(8, INPUT);
  

  
  if (digitalRead(setup_yellow) == HIGH){
      Serial.println("Yellow is HIGH");
      
  }
  else{
      Serial.println("Yellow is HIGH");
    }
  
 
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
  
   

  tft.fillScreen(bg_fill_color);
  menu_rule(); 
}



void Rules_Explanation_heading(){

    tft.setRotation(3);
    tft.cursorTo(0, 0);
    tft.setFont(Arial_bold_14);
    tft.setTextLetterSpacing(2);
     tft.setTextScale(2);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLUE);
    tft.fillRect(0, 0, 320, 30, ILI9341_BLUE);  
    tft.printAligned("Rules / Explanation ", gTextAlignTopCenter);
    tft.setTextLineSpacing (6);
    tft.setFont(SystemFont5x7);
    tft.drawLine(1, 47, 320, 47, ILI9341_WHITE );
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK );
    tft.setTextScale(2);
    tft.setTextLetterSpacing(1);
    tft.println("");
    tft.println("");
    tft.setTextLineSpacing (1);
  
  }

void Rules_Explanation_first_site(){
    
    tft.println("The game ends  when some-");
    tft.println("one have 10 points (if you");
    tft.setTextLineSpacing (5);
    tft.println("don't gonna change it)");
    tft.setTextLineSpacing (1);
    tft.println("The default ball / player");
    tft.println("speed is 2 (if you don't");
    tft.setTextLineSpacing (5);
    tft.println("gonna change it)");
    tft.setTextLineSpacing (1);
    tft.println("Press the yellow button to");
    tft.setTextLineSpacing (5);
    tft.println("go to the next side");
    tft.setTextLineSpacing (1);
        
  }
  
void Rules_Explanation_second_site(){
  while(rules_running == true){
      if (digitalRead(setup_yellow) == LOW){
        tft.fillScreen(bg_fill_color);
         Rules_Explanation_heading();
        tft.println("Press the green button to ");
        tft.println("go back in the following");
        tft.setTextLineSpacing (5);
        tft.println("settings");
        tft.setTextLineSpacing (1);
        tft.println("If you want to go back to ");
        tft.println("the first page press the");
        tft.println("blue");
        tft.setTextLineSpacing (5);
        tft.println("button");
        tft.setTextLineSpacing (1); 
        tft.println("Press the yellow button to");
        tft.setTextLineSpacing (5);
        tft.println("continue...");
        tft.setTextLineSpacing (1);
        while(rules_running == true){ 
          if (digitalRead(setup_yellow) == LOW){
            menu_running = true;
            rules_running = false;
            tft.fillScreen(bg_fill_color);
            
          }
          else if (digitalRead(setup_blue) == LOW){
            tft.fillScreen(bg_fill_color);
            menu_rule();
          }
        }
      }
   }
  } 

void Settings_heading(){
  tft.cursorTo(0, 0);
    tft.setFont(Arial_bold_14);
    tft.setTextLetterSpacing(7);
     tft.setTextScale(3);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLUE);
    tft.fillRect(0, 5, 320, 33, ILI9341_BLUE);  
    tft.printAligned("Settings", gTextAlignTopCenter);
    tft.setTextLineSpacing (6);
    tft.setFont(SystemFont5x7);
    tft.drawLine(1, 47, 320, 47, ILI9341_WHITE );
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK );
    tft.setTextScale(2);
    tft.setTextLetterSpacing(1);
    
    tft.println("");
    tft.println("");
    tft.setTextLineSpacing (1);

  }

void Settings_txt(){
    tft.setTextColor(txt_color, txt_bg_color);
    tft.println("Press the black button to");
    tft.setTextLineSpacing (5);
    tft.println("change the color settings");
    tft.setTextLineSpacing (1);
    
    tft.println("Press the red button to");
    tft.setTextLineSpacing (5);
    tft.println("change player / ball speed");
    tft.setTextLineSpacing (1);
    tft.println("Press the blue button to");
    tft.setTextLineSpacing (5);
    tft.println("change the score to win");
    tft.setTextLineSpacing (1);
    tft.println("Press the yellow button to" );
    tft.println("continue... (without chang-");
    tft.println("ing any settings)");
    while(menu_running == true){
      if (digitalRead(setup_yellow) == LOW){
        Settings_skip();
      }else if  (digitalRead(setup_red) == LOW){
        tft.cursorToXY( 0, 0 );
        tft.setTextColor(txt_color, txt_bg_color);
        Settings_ball_player_speed();
     } else if  (digitalRead(setup_black) == LOW){
        tft.cursorToXY( 0, 0 );
        tft.setTextColor(txt_color, txt_bg_color);
        tft.fillScreen(bg_fill_color);
        Settings_change_bg_fill_color();
         
     } else if  (digitalRead(setup_blue) == LOW){
         tft.cursorToXY( 0, 0 );
         tft.setTextColor(txt_color, txt_bg_color);
         tft.fillScreen(bg_fill_color);
         Settings_change_score();
     } 
   }
 }
  

void Settings_skip(){
  while(menu_running == true){
      if (digitalRead(setup_yellow) == LOW){
        tft.fillScreen(bg_fill_color);
         menu_running = false;
         game_running = true;

     }
     
    }     
  }

void Set_settings(){
  tft.fillScreen(bg_fill_color);
  tft.setTextColor(ILI9341_WHITE, ILI9341_NAVY ); 
  tft.printAligned("SET", gTextAlignTopCenter);
  tft.fillScreen(bg_fill_color); 
  }

void ball_speed_change(){
  tft.fillScreen(bg_fill_color);
  tft.cursorToXY( 0, 85 );      
         tft.println("What speed do you");
         tft.println("like the ball to have?");
         tft.println("(2 is default just go back when you want to keep it)");
         tft.println("black  red  blue  yellow");
         tft.println("  1     3     4      5  ");
         while(menu_running == true){ 
          if (digitalRead(setup_black) == LOW){
           ball_speed = 1;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt();
        } else if (digitalRead(setup_red) == LOW){
           ball_speed = 3;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt();
        } else if (digitalRead(setup_blue) == LOW){
           ball_speed = 4;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt(); 
        } else if (digitalRead(setup_yellow) == LOW){
           ball_speed = 5;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt(); 
        } else if (digitalRead(setup_green) == LOW){
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt(); 
        }                  
      }
    }

void player_speed_change(){
  tft.fillScreen(bg_fill_color);
  tft.cursorToXY( 0, 85 );      
         tft.println("What speed do you");
         tft.println("like the player to have?");
         tft.println("(2 is default just go back)");
         tft.println("when you want to keep it");
         tft.println("black  red  blue  yellow");
         tft.println("  1     3     4      5  ");
         while(menu_running == true){ 
          if (digitalRead(setup_black) == LOW){
           player_speed = 1;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt();
        } else if (digitalRead(setup_red) == LOW){
           player_speed = 3;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt();
        } else if (digitalRead(setup_blue) == LOW){
           player_speed = 4;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt(); 
        } else if (digitalRead(setup_yellow) == LOW){
           player_speed = 5;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt(); 
        } else if (digitalRead(setup_green) == LOW){
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt(); 
        }                  
     }
  }


void Settings_ball_player_speed(){
      while(menu_running == true){
        if (digitalRead(setup_red) == LOW){ //ball speed
            
         tft.fillScreen(bg_fill_color); 
         tft.cursorToXY(0, 85);
          tft.println("What do you want");
          tft.println("to change?");
          tft.println("black --> ball");
          tft.println("red --> player");
          while(menu_running == true){
             if (digitalRead(setup_black) == LOW){ //ball speed
                ball_speed_change();
             }
             else if  (digitalRead(setup_red) == LOW){ //ball speed
                player_speed_change();
             }
          }
       }
    }
 }  

void Settings_change_bg_fill_color(){
         tft.cursorToXY( 0, 85 );      
         tft.println("Which background color");
         tft.println("do you want?");
         tft.println("(Black is default just go)");
         tft.println("back with black buton when you want to keep it");
         tft.println("red  green blue  yellow");
         while(menu_running == true){ 
          if (digitalRead(setup_red) == LOW){
           bg_fill_color = ILI9341_RED ;
           txt_color = ILI9341_BLACK;
           txt_bg_color = ILI9341_RED;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt();
        } else if (digitalRead(setup_green) == LOW){
           bg_fill_color = ILI9341_GREEN;
           txt_color = ILI9341_BLACK;
           txt_bg_color = ILI9341_GREEN;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt();
        } else if (digitalRead(setup_blue) == LOW){
           bg_fill_color = ILI9341_LIGHTBLUE;
           txt_color = ILI9341_BLACK;
           txt_bg_color = ILI9341_LIGHTBLUE;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt(); 
        } else if (digitalRead(setup_yellow) == LOW){
           bg_fill_color = ILI9341_YELLOW;
           txt_color = ILI9341_BLACK;
           txt_bg_color = ILI9341_YELLOW;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt(); 
        } else if (digitalRead(setup_black) == LOW){
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt(); 
        }                  
     }
  }
  
void pause_start(){
  
  static bool btn_down = false;
  
  btn_state = digitalRead(setup_green);
  if (btn_state == LOW && btn_down == false)
  {
    game_running = !game_running;
    btn_down = true;
    tft.fillRect(113, 129, 15, 63, ILI9341_BLACK);
  }

  if (btn_state == HIGH)
  {
    btn_down = false;
  }


}

void Settings_change_score(){
    tft.cursorToXY( 0, 85 );      
         tft.println("With how much points");
         tft.println("do you want the");
         tft.println("game ends");
         tft.println("The default is 10 go back with");
         tft.println("green buton when you want to keep it");
         tft.println("black red  blue  yellow");
         tft.println(" 15    20   25     30");
         while(menu_running == true){ 
          if (digitalRead(setup_red) == LOW){ 
           win_points = 20;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt();
        } else if (digitalRead(setup_green) == LOW){
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt();
        } else if (digitalRead(setup_blue) == LOW){
           win_points = 25;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt();
        } else if (digitalRead(setup_yellow) == LOW){
           win_points = 30;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt();
        } else if (digitalRead(setup_black) == LOW){
           win_points = 15;
           tft.fillScreen(bg_fill_color);
           Set_settings();
           tft.fillScreen(bg_fill_color);
           Settings_heading();
           Settings_txt(); 
        }                  
     }   
}


void menu_rule(){ 
   
   
  //*************************RULES*************************
    
    Rules_Explanation_heading();
    Rules_Explanation_first_site();
    Rules_Explanation_second_site();
    
   //*************************MENU*************************
    
    Settings_heading();
    Settings_txt(); 
    tft.setTextScale(1);
}
    
  

void player_1 (){

  if(player1 <= y && player1 + 30 >= y){
    if(x >= 15 && x <= 20){
      collides_l_player = true;
      if(digitalRead(up_btn1) == HIGH){
        dy = (long_random(millis())%2 - 1);
      }
      
      if(digitalRead(down_btn1) == HIGH){
        dy = (long_random(millis())%2 - 1);
      }      
      
      }
    }
  
  if (digitalRead(up_btn1) == HIGH) {
    
    // Taste "UP" gedrückt, Spieler 1 nach oben bewegen
     tft.fillRect(player1, 10, 30, 5, bg_fill_color);  
    player1 -= player_speed;
     
  }
  if (digitalRead(down_btn1) == HIGH) {
    // Taste "DOWN" gedrückt, Spieler 1 nach unten bewegen
     tft.fillRect(player1, 10, 30, 5, bg_fill_color);
    player1 += player_speed;
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
      collides_r_player = true;
      if(digitalRead(up_btn2) == HIGH){
        dy = (long_random(millis())%2 - 1);
        }
      if(digitalRead(down_btn2) == HIGH){
        dy = (long_random(millis())%2 - 1);
        }        
      }
  }
  if (digitalRead(up_btn2) == HIGH) {
    
    // Taste "UP" gedrückt, Spieler 1 nach oben bewegen
     tft.fillRect(player2, tft.height() - 15, 30, 5, bg_fill_color);
     player2 -= player_speed;
     
  }
  if (digitalRead(down_btn2) == HIGH) {
    // Taste "DOWN" gedrückt, Spieler 1 nach unten bewegen
     tft.fillRect(player2,  tft.height() - 15, 30, 5, bg_fill_color);
     player2 += player_speed;
     
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
  tft.fillCircle(y, x, 12, bg_fill_color);
  tft.fillCircle(y, x, 5, ILI9341_WHITE);
  delay(3);
  x += dx * ball_speed;
  y += dy * ball_speed;
  
  if (x >= tft.height() - 7) {
   collides_r_wall = true;
   
  }
    else if (x <= 7) {
   collides_l_wall = true;
    
  }
 
  if (y >= tft.width() - 6){
    collides_bottom =  true;
    } 

  if (y <= 6){
    collides_top = true;
    }  
}

void ball_move(){
 if (collides_r_player == true) {
      dx = -dx;
     
    collides_r_player = false;
  }
  if (collides_l_player == true) {
      dx = -dx;
     
      collides_l_player = false;
  }
  if (collides_top == true) {
      dy = dy * (-1);
     collides_top = false;
    }
   if (collides_bottom == true){
      dy = dy * (-1);
     collides_bottom = false;
    } 

    
}
    
void score (){
   if (collides_r_wall == true) {
    tft.fillRect(0, tft.height() - 15 , tft.width(), 20, bg_fill_color);
    left_points ++;
    y = 120;
    x = 170;
    collides_r_wall = false;
  }

    if (collides_l_wall == true) {
    
    tft.fillRect(0, 0, tft.width(), 20, bg_fill_color);
    right_points ++;
    y = 120;
    x = 170;
    
    collides_l_wall = false;
  }
  
  tft.setRotation(3);
  tft.cursorTo(0, 0);
  tft.setFont(Arial_bold_14);
  tft.setTextLetterSpacing(5);
  tft.setTextColor(txt_color, txt_bg_color); 
  tft.printAligned("Score:" + String(left_points)+ "|" + String(right_points), gTextAlignTopCenter);
  tft.setRotation(2);

  if (left_points >= win_points){
    tft.fillScreen(bg_fill_color);
    tft.setTextColor(txt_color, txt_bg_color); 
    tft.setRotation(3);
    tft.printAligned("Left Wins", gTextAlignMiddleCenter);
    delay(5000);
    tft.cursorToXY(0, 85);
    tft.fillScreen(bg_fill_color);
    tft.println("Do you want to play again?");
    tft.println("Press red if yes");
    tft.println("Press black if no");
    while(game_running == true){ 
          if (digitalRead(setup_red) == LOW){
            left_points = 0;
            right_points = 0;
            tft.fillScreen(bg_fill_color);            
            game();
            tft.setRotation(2);
    }else if (digitalRead(setup_black) == LOW){
       tft.fillScreen(bg_fill_color);
       tft.printAligned("Bye", gTextAlignMiddleCenter);
      exit(0);
    }
  }
  }

  else if (right_points >= win_points){
    tft.fillScreen(bg_fill_color);
    tft.fillScreen(bg_fill_color);
    tft.setTextColor(txt_color, txt_bg_color);
    tft.setRotation(3); 
    tft.printAligned("Right Wins", gTextAlignMiddleCenter);
    delay(5000);
    tft.cursorToXY(0, 85);
    tft.fillScreen(bg_fill_color);
    tft.println("Do you want to play again?");
    tft.println("Press red if yes");
    tft.println("Press black if no");
    while(game_running == true){ 
     if (digitalRead(setup_red) == LOW){
            left_points = 0;
            right_points = 0;  
            tft.fillScreen(bg_fill_color);          
            game();
            break;
    }else if (digitalRead(setup_black) == LOW){
       tft.fillScreen(bg_fill_color);
       tft.printAligned("Bye", gTextAlignMiddleCenter);
       delay(5000);
       exit(0);
   }
  }
 }
} 


void game(){
  score();
  ball(); 
  player_1();
  ball_move();  
  player_2();
  ball_move();
  pause_start();
  }


void loop() {
  
  if (game_running == false){
    tft.setRotation(3);
    tft.cursorTo(0, 0);
    tft.setFont(Arial_bold_14);
    tft.setTextLetterSpacing(7);
    tft.setTextColor(ILI9341_WHITE, ILI9341_RED);
    tft.printAligned("Pause", gTextAlignMiddleCenter);
    tft.setRotation(2);
    pause_start();
    }
  else{
    game();
    }
}
*/
