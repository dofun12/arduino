#include <Thermistor.h> //INCLUSÃO DA BIBLIOTECA
 
Thermistor temp(2); //VARIÁVEL DO TIPO THERMISTOR, INDICANDO O PINO ANALÓGICO (A2) EM QUE O TERMISTOR ESTÁ CONECTADO
void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}
void loop() {
  int temperature = temp.getTemp(); //Recebendo temperatura
  if(temperature > 28){
     digitalWrite(3, HIGH);
     digitalWrite(4, LOW);
     digitalWrite(5, LOW);
  }else if(temperature > 20 && temperature<=27){
     digitalWrite(3, LOW);
     digitalWrite(4, HIGH);
     digitalWrite(5, LOW);
  }else if(temperature <= 20){
     digitalWrite(3, LOW);
     digitalWrite(4, LOW);
     digitalWrite(5, HIGH);
  }
  digitalWrite(3, HIGH);
  delay(1000);
  digitalWrite(3, LOW);
  
  Serial.println(temperature);           //retorna o que foi lido
  delay(20000); //INTERVALO DE 20 SEGUNDOS
}
