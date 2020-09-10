#include <Arduino.h>

// Arduino Nano SCL -> A5; SDA -> A4;
#include "U8glib.h"
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI
U8GLIB_SH1106_128X64 u8g(U8G_PI_I2C_OPTION); // I2C / TWI

long time = 0;


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
  Serial.begin(9600);
  pinMode(A0, INPUT);
  u8g.setFont(u8g_font_baby);
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
  if (num < 10)
  {
    char d = tmp[0];
    tmp[0] = '0';
    tmp[1] = d;
  }
}

void getTime(char *timeStr)
{

  long seconds = time / 1000;
  long s = seconds % 60;
  long m = (seconds / 60) % 60;
  long h = (seconds / (60 * 60));

  Serial.println("Sem formato");
  Serial.print(h);
  Serial.print(":");
  Serial.print(m);
  Serial.print(":");
  Serial.print(s);
  Serial.print("\n");

  char hh[2];
  char mm[2];
  char ss[2];

  toChar(h, hh);
  toChar(m, mm);
  toChar(s, ss);

  strcat(timeStr, hh);
  strcat(timeStr, ":");
  strcat(timeStr, mm);
  strcat(timeStr, ":");
  strcat(timeStr, ss);

  Serial.println("Depois");
  Serial.print(timeStr);
  Serial.print("\n");
}

void draw()
{
  char timeStr[10];
  u8g.drawStr(1, 20, "Hello World");
  char potenciometroText[10];
  dtostrf(getPercent(), 6, 2, potenciometroText);
  u8g.drawStr(1, 30, potenciometroText);
  //getTime(timeStr);
  //u8g.drawStr(1, 50, timeStr);
}

void loop()
{
  u8g.firstPage();
  do
  {
    draw();
  } while (u8g.nextPage());
  time = time + 500;
  delay(500);
}
