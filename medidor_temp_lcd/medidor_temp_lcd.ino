#include <LiquidCrystal.h>
#include <dht.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
const int pinoDHT = A5;
int valorLido;
dht DHT; 
int analogSoloSeco = 400;
int analogSoloMolhado = 150;
int percSoloSeco = 0;
int percSoloMolhado = 100;

void setup(){
 
 lcd.begin(16,2);
 lcd.setCursor(0, 0);
 lcd.clear(); 
 lcd.print("Lendo Dados...");
 delay(2000);
}

void printTemperatura(){
  DHT.read11(pinoDHT);
  lcd.setCursor(0, 0);
  lcd.print("Temp. Ambiente  ");
  lcd.setCursor(0, 1);
  lcd.print(DHT.temperature);
  lcd.print(" C.");
};



void printUmidade(){
  DHT.read11(pinoDHT);
  lcd.setCursor(0, 0);
  lcd.print("Umidade Ambiente");
  lcd.setCursor(0, 1);
  lcd.print(DHT.humidity);
  lcd.print("%");
};

void loop(){  
 printTemperatura();
 delay(5000);
 printUmidade();
 delay(5000);
}
