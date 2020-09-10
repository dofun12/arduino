#include <LiquidCrystal.h> //INCLUSÃO DE BIBLIOTECA
#include <EEPROM.h>
#include <dht.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //PORTAS DO ARDUINO QUE SÃO UTILIZADAS PELO SHIELD KEYPAD DISPLAY LCD
const int pinoSolo1 = A8; //PINO UTILIZADO PELO SENSOR
const int pinoDHT = A9;
int valorLido; //VARIÁVEL QUE ARMAZENA O PERCENTUAL DE UMIDADE DO SOLO
dht DHT; 
int analogSoloSeco = 400; //VALOR MEDIDO COM O SOLO SECO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int analogSoloMolhado = 150; //VALOR MEDIDO COM O SOLO MOLHADO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int percSoloSeco = 0; //MENOR PERCENTUAL DO SOLO SECO (0% - NÃO ALTERAR)
int percSoloMolhado = 100; //MAIOR PERCENTUAL DO SOLO MOLHADO (100% - NÃO ALTERAR)

void setup(){
 
 lcd.begin(16,2); //SETA A QUANTIDADE DE COLUNAS(16) E O NÚMERO DE LINHAS(2) DO DISPLAY. EM SUMA: UMA MATRIZ DE 16 COLUNAS E 2 LINHAS
 //EEPROM.get(eeAddress, valor);
 lcd.setCursor(0, 0);
 lcd.clear(); 
 lcd.print("Lendo Dados...");
 delay(2000); //INTERVALO DE 2 SEGUNDOS
}

void printTemperatura(){
  DHT.read11(pinoDHT);
  lcd.setCursor(0, 0);
  lcd.print("Temp. Ambiente  ");
  lcd.setCursor(0, 1);
  lcd.print(DHT.temperature);
  lcd.print(" C.");
};

void printUmidadeSolo(){
  lcd.setCursor(0, 0);
  lcd.print("Umidade Solo    ");


  lcd.setCursor(0, 1);
  valorLido = constrain(analogRead(pinoSolo1),analogSoloMolhado,analogSoloSeco); //MANTÉM valorLido DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
  valorLido = map(valorLido,analogSoloMolhado,analogSoloSeco,percSoloMolhado,percSoloSeco); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS
  lcd.print(valorLido);
  lcd.print("%");//IMPRIME NO MONITOR SERIAL O PERCENTUAL DE UMIDADE DO SOLO
  lcd.print("                 ");
};

void printUmidade(){
  DHT.read11(pinoDHT);
  lcd.setCursor(0, 0);
  lcd.print("Umidade Ambiente");
  lcd.setCursor(0, 1);
  lcd.print(DHT.humidity);//IMPRIME NO MONITOR SERIAL O PERCENTUAL DE UMIDADE DO SOLO
  lcd.print("%");
};

void loop(){  
 printTemperatura();
 delay(5000);  //INTERVALO DE 1 SEGUNDO
 printUmidadeSolo();
 delay(5000);
 printUmidade();
 delay(5000);
}
