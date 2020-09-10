#include <LiquidCrystal.h> //INCLUSÃO DE BIBLIOTECA
  
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //PORTAS DO ARDUINO QUE SÃO UTILIZADAS PELO SHIELD KEYPAD DISPLAY LCD

void cleanLcd(){
  lcd.setCursor(0,0);
  
}
  
void setup(){  
 lcd.begin(15,2); //SETA A QUANTIDADE DE COLUNAS(16) E O NÚMERO DE LINHAS(2) DO DISPLAY. EM SUMA: UMA MATRIZ DE 16 COLUNAS E 2 LINHAS
 lcd.setCursor(0,0); //SETA A POSIÇÃO EM QUE O CURSOR INCIALIZA(LINHA 1) 
 lcd.print("Sol lvl"); //ESCREVE A FRASE "APERTOU A TECLA" NA PRIMEIRA LINHA DO DISPLAY LCD  
}  
  
void loop(){  
 lcd.setCursor(0,1); //SETA A POSIÇÃO EM QUE O CURSOR RECEBE O TEXTO A SER MOSTRADO(LINHA 2)
 int sunLevel = analogRead(A14);
 lcd.print(analogRead(A14));
}
