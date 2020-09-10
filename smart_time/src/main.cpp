#include <Arduino.h>

// Arduino Nano SCL -> A5; SDA -> A4;
#include "U8glib.h"
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI
U8GLIB_SH1106_128X64 u8g(U8G_PI_I2C_OPTION); // I2C / TWI

long time = 0;
long cooldown = 1000;
long lastSpell = 0;
long lastTimeSet = 0;


float getPercent()
{
  int maxValue = 715;
  int minValue = 0;
  float valorLido = map(analogRead(A0), maxValue, minValue, 100, 0);
  return valorLido;
}

void u8g_prepare(void)
{
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void setup()
{
  delay(5000);
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(12, INPUT);
  pinMode(11, INPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10,LOW);
  
  u8g.setColorIndex(1); // Instructs the display to draw with a pixel on.
}

void clearChar(char meuChar[])
{
  int size = strlen(meuChar);
  Serial.print("Meu char: ");
  Serial.print(meuChar);
  Serial.print(" Size: ");
  Serial.print(size);
  Serial.print("\n");
  for (int i = 0; i < size; i++)
  {
    meuChar[i] = '0';
  }
}

void toChar(int num, char *tmp)
{
  itoa(num, tmp, 10);
  char d;
  if (num < 10)
  {
    d = tmp[0];
    tmp[1] = d;
    tmp[0] = '0';
  }
  tmp[2] = '\0';
}

void getTime(char *timeStr)
{

  long seconds = time / 1000;
  long s = seconds % 60;
  long m = (seconds / 60) % 60;
  long h = (seconds / (60 * 60));

  char hh[3];
  char mm[3];
  char ss[3];

  toChar(h, hh);
  toChar(m, mm);
  toChar(s, ss);

  timeStr[0] = hh[0];
  timeStr[1] = hh[1];
  timeStr[2] = ':';
  timeStr[3] = mm[0];
  timeStr[4] = mm[1];
  timeStr[5] = ':';
  timeStr[6] = ss[0];
  timeStr[7] = ss[1];
  timeStr[8] = '\0';

  Serial.println("Depois");
  Serial.print(timeStr);
  Serial.print("\n");
}

void draw()
{
  char timeStr[10];
  u8g.setFont(u8g_font_baby);
  u8g.drawStr(20, 20, "Timer");
  char potenciometroText[10];
  dtostrf(getPercent(), 6, 2, potenciometroText);
  //u8g.drawStr(1, 30, potenciometroText);
  u8g.setFont(u8g_font_gdr12r);
  u8g.drawStr(20, 40, timeStr);
  int btn12 = digitalRead(12);
  int btn11 = digitalRead(11);
  
  if(btn11){
    time = time+1000;
    lastTimeSet = time;
    
  }
  if(btn12){
    time = 0;
    lastTimeSet = 0;
    lastSpell=cooldown;
    
  }

if(lastSpell==cooldown){
    getTime(timeStr);
    
    lastSpell = 0;
  }
  
  
}

void loop()
{
  u8g.firstPage();
  do
  {
    
    draw();
  } while (u8g.nextPage());
  if(time>=0){
    time = time - 500;
  }else{
    if(lastTimeSet>0){
      digitalWrite(10,HIGH);
      delay(getPercent()*60);
      digitalWrite(10,LOW);
      time = lastTimeSet;
    }
    
  }
  
  lastSpell = lastSpell+500;
  delay(500);
}
