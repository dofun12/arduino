#include "EmonLib.h" //INCLUSÃO DE BIBLIOTECA

#define AMOSTRAS 100
#define CURRENT_CAL 18.40 //VALOR DE CALIBRAÇÃO (DEVE SER AJUSTADO EM PARALELO COM UM MULTÍMETRO MEDINDO A CORRENTE DA CARGA)
const int pinoSensor = A0; //PINO ANALÓGICO EM QUE O SENSOR ESTÁ CONECTADO

EnergyMonitor emon1;

void setup() {
    Serial.begin(9600);
     emon1.current(pinoSensor, CURRENT_CAL);
}

float readVoltage(uint8_t portaAnalogica, float res1, float res2) {
  float total=0;  
  for (int i=0; i<AMOSTRAS; i++) {
    total += 1.0 * analogRead(portaAnalogica);
    delay(5);
  }
  float relacao = ((res1+res2)/res2);
  float tensaoArduino = 4.5;
  float volt = ((((total / (float)AMOSTRAS) * tensaoArduino) / 1023.0) * relacao);
  return volt;
}  

void loop() {
  float tensaoA1 = readVoltage(A1,10000000,1000000);
  int current;
  Serial.print("Tensao em A0: ");
  Serial.print(tensaoA1);
  Serial.println ("V");

  emon1.calcVI(17, 100); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS / TEMPO LIMITE PARA FAZER A MEDIÇÃO)
  double currentDraw = emon1.Irms; //VARIÁVEL RECEBE O VALOR DE CORRENTE RMS OBTIDO

  Serial.print("Corrente medida: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(currentDraw); //IMPRIME NA SERIAL O VALOR DE CORRENTE MEDIDA
  Serial.println("A"); //IMPRIME O TEXTO NA SERIAL
  delay(500);
}
