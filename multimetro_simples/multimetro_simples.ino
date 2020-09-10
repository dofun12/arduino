
double res1 = 10000000.0;
double res2 = 10000000.0;


void setup() {
   Serial.begin(9600); 

}

double readVoltage() {
  int AMOSTRAS = 10;
  float total=0;  
  for (int i=0; i<AMOSTRAS; i++) {
    total += 1.0 * analogRead(A1);
    delay(5);
  }
  double relacao = ((res1+res2)/res2);
  double tensaoArduino = 4.64;
  double volt = ((((total / (double)AMOSTRAS) * tensaoArduino) / 1023.0) * relacao);
  return volt;
}  

double readCurrentV2(){
  double Voltage = 0;
  double Amps = 0;
  int ACSoffset = 2500;
  int mVperAmp = 185;
  int RawValue = 0;
  int measures = 10;
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

void loop() {
  Serial.print("Analog A0: ");
  Serial.print(analogRead(A0));
  Serial.println ("val");

  Serial.print("Voltage: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print((readVoltage())); //IMPRIME NA SERIAL O VALOR DE CORRENTE MEDIDA
  Serial.println("V"); //IMPRIME O TEXTO NA SERIAL
  
  Serial.print("Corrente: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print((readCurrentV2())); //IMPRIME NA SERIAL O VALOR DE CORRENTE MEDIDA
  Serial.println("A"); //IMPRIME O TEXTO NA SERIAL

  
  delay(2000);
}
