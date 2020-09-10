#include "EmonLib.h" //INCLUSÃO DE BIBLIOTECA
#define AMOSTRAS 200
#define CURRENT_CAL 5.065 //VALOR DE CALIBRAÇÃO (DEVE SER AJUSTADO EM PARALELO COM UM MULTÍMETRO MEDINDO A CORRENTE DA CARGA)
const int pinoSensor = A0; //PINO ANALÓGICO EM QUE O SENSOR ESTÁ CONECTADO


EnergyMonitor emon1;


double readCurrentV2(){
  double Voltage = 0;
  double Amps = 0;
  int ACSoffset = 2500;
  int mVperAmp = 185;
  int RawValue = 0;
  int measures = 30;
  double total = 0.0;
  for(int m = 0;m<measures;m++){
    total = total+analogRead(A0);
    delay(10);
  }
  RawValue = (total/measures);
  Voltage = (RawValue / 1024.0) * 5000; // Gets you mV  
  Amps = ((Voltage - ACSoffset) / mVperAmp);
  if(Amps<0){
    Amps = Amps*(-1);
  }
  return (Amps);
}

double readCurrent(){
  emon1.calcVI(20, 2000); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS / TEMPO LIMITE PARA FAZER A MEDIÇÃO)
  return (emon1.Irms); //VARIÁVEL RECEBE O VALOR DE CORRENTE RMS OBTIDO
  /**
  
  **/
}

void setup() {
  Serial.begin(9600); 
  emon1.current(pinoSensor, CURRENT_CAL);
  // put your setup code here, to run once:

}

void loop() {
  Serial.print("Analog A0: ");
  Serial.print(analogRead(A0));
  Serial.println ("val");

  Serial.print("Corrente medida A: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(readCurrent()); //IMPRIME NA SERIAL O VALOR DE CORRENTE MEDIDA
  Serial.println("A"); //IMPRIME O TEXTO NA SERIAL

  Serial.print("Corrente medidaB: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print((readCurrentV2())); //IMPRIME NA SERIAL O VALOR DE CORRENTE MEDIDA
  Serial.println("A"); //IMPRIME O TEXTO NA SERIAL
  delay(2000);
  // put your main code here, to run repeatedly:

}
