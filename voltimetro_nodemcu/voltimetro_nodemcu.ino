#define analogInput A0

int val = 0;
float Vout = 0.00;
float Vin = 0.00;
float R1 = 978000.00; // resistance of R1 (1M) 
float R2 = 98100.00; // resistance of R2 (100K)

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
}

float lePorta(uint8_t portaAnalogica, int AMOSTRAS) {
  float total=0;  
  for (int i=0; i<AMOSTRAS; i++) {
    total += 1.0 * analogRead(portaAnalogica);
    delay(5);
  }
  return total / (float)AMOSTRAS;
}  

void loop() {
  float relacao = (((R1+R2)/R2)) + 0.90;
  val = lePorta(analogInput, 1000);//reads the analog input
  Vout = (val * 3.3) / 1024.00; // formula for calculating voltage out i.e. V+, here 5.00
  Vin = (Vout * relacao); // formula for calculating voltage in i.e. GND
  
  // put your main code here, to run repeatedly:
  Serial.println("Vout");
  Serial.println(Vin);
  Serial.println("Relacao");
  Serial.println(relacao);
  delay(500);
}
