// Henry's Bench
// Arduino Nokia 5110 U8Glib Tutorial


#include "U8glib.h"

 

#define backlight 13
 
//Delcare the display and assign the pins
 
//U8GLIB_PCD8544 u8g(8, 4, 7, 5, 6);  // CLK=8, DIN=4, CE=7, DC=5, RST=6
 U8GLIB_PCD8544 u8g(8, 9, 11, 10, 12);  // CLK=8, DIN=4, CE=7, DC=5, RST=6

 
void draw() {
 
  u8g.setFont(u8g_font_profont11);  // select font
  u8g.drawStr(0, 15, "Nokia 5110..");  // put string of display at position X, Y
  u8g.drawStr(29, 35, "HELLO!!!!");
  
  
}
 
void setup() {
  // Set Backlight Intensity
  // 0 is Fully bright
  // 255 is off
  analogWrite(backlight, 80);
  //
  // Enter the U8Glib Picture Loop
  //
  
}
 
void loop() { 
 u8g.firstPage(); 
  do {
    draw();
  } while( u8g.nextPage() ); 
  //delay(100);
}
