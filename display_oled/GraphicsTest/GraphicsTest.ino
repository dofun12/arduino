#include "Arduino.h"
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 

void setup(void) {

#if defined(ARDUINO)
  pinMode(13, OUTPUT);           
  digitalWrite(13, HIGH);  
#endif
}

void loop(void) {
  u8g.println("Teste");
  

}
