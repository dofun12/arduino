#include <LiquidCrystal.h> //INCLUSÃO DE BIBLIOTECA
#include <EEPROM.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //PORTAS DO ARDUINO QUE SÃO UTILIZADAS PELO SHIELD KEYPAD DISPLAY LCD

int eeAddress = 0;
int valor = 0;
int limpo = 0;
void setup(){ 
 lcd.begin(16,2); //SETA A QUANTIDADE DE COLUNAS(16) E O NÚMERO DE LINHAS(2) DO DISPLAY. EM SUMA: UMA MATRIZ DE 16 COLUNAS E 2 LINHAS
 EEPROM.get(eeAddress, valor);
 lcd.setCursor(0, 0);
 lcd.clear(); 
 lcd.print("Iniciando");
}  
void loop(){
  delay(10000);
  if(limpo==0){
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Contando");
    limpo =1;
  }
  
  lcd.setCursor(0, 1);
  lcd.print(valor*10);
  valor++;
  EEPROM.put(eeAddress, valor);
}
