#include <LiquidCrystal.h> //INCLUSÃO DE BIBLIOTECA
#include <EEPROM.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //PORTAS DO ARDUINO QUE SÃO UTILIZADAS PELO SHIELD KEYPAD DISPLAY LCD

int eeAddress = 0;
int valor = 0;
int limpo = 0;
int cycles =0;
void setup(){ 
 lcd.begin(16,2); //SETA A QUANTIDADE DE COLUNAS(16) E O NÚMERO DE LINHAS(2) DO DISPLAY. EM SUMA: UMA MATRIZ DE 16 COLUNAS E 2 LINHAS
 EEPROM.get(eeAddress, valor);
 lcd.setCursor(0, 0);
 lcd.clear(); 
 lcd.print("Iniciando");
}
void clearCount(){
    cycles = 0; 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Contando");
}
void loop(){
  
  if(limpo==0){
    clearCount();
    limpo =1;
  }
  
  lcd.setCursor(0, 1);
  
  
  EEPROM.put(eeAddress, valor);
  if ((analogRead(0)) < 800){ //SE A LEITURA DO PINO FOR MENOR QUE 800, FAZ 
    clearCount();
    valor=0;
  }
  if(cycles>10){
    valor++;
    cycles = 0;
    
    lcd.print(valor);
  }
  cycles++;
 delay(100); 
}
