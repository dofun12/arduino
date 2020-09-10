#include "EmonLib.h"

EnergyMonitor emon1;

int pino_sct = A0;

void setup() {
   Serial.begin(9600);
  // put your setup code here, to run once:
  emon1.current(pino_sct,60);
  
}

void loop() {
  emon1.calcVI(20,2000); 
  double Irms = emon1.calcIrms(1480);
  Serial.println("Corrente : ");
  Serial.println(Irms); // Irms
  Serial.println(analogRead(A0));
  
  delay(1000);
}
