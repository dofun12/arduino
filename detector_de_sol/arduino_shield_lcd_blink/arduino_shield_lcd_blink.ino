#include <LiquidCrystal.h> //INCLUSÃO DE BIBLIOTECA
  
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //PORTAS DO ARDUINO QUE SÃO UTILIZADAS PELO SHIELD KEYPAD DISPLAY LCD

void cleanLcd(){
  lcd.setCursor(0,0);
  
}
  
void setup(){  
 lcd.begin(16,2); //SETA A QUANTIDADE DE COLUNAS(16) E O NÚMERO DE LINHAS(2) DO DISPLAY. EM SUMA: UMA MATRIZ DE 16 COLUNAS E 2 LINHAS
}  
  
void loop(){  
  // Turn on the blinking cursor:
  lcd.print("EITA PORRA");
  delay(15000);
  lcd.noDisplay();
  
}
