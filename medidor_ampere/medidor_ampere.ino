#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

float vetCorrente[300];

float vout = 0.0;
int value = 0;
float vin = 0.0;
float R1 = 100000.0; // resistance of R1 (10K) -see text!
float R2 = 10000.0; // resistance of R2 (1K) - see text!
void setup()
{
  Serial.begin(9600);
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  lcd.clear(); 
  lcd.print("Lendo Dados...");
  delay(2000);
}
 
void loop()
{
  double maior_Valor = 0;
  double valor_Corrente = 0;  
 
  float tensao = 3.67;
  float potencia = 0;
 
  for(int i = 0; i < 300; i++)
  {
    vetCorrente[i] = analogRead(A8);
    delayMicroseconds(600);
  }  
 
  for(int i = 0; i < 300; i++)
  {
    if(maior_Valor < vetCorrente[i])
    {
      maior_Valor = vetCorrente[i];
    }
  }  

  value = analogRead(A9);
  vout = (value * 5.0);
  vout = (vout /1024); // see text
  vin = (vout / (R2/(R1+R2))); 
  if (vin<0.09) {
    vin=0.0; //statement to quash undesired reading !
  }
  
  maior_Valor = maior_Valor * 0.004882812;
  valor_Corrente = maior_Valor - 2.5;
  valor_Corrente = valor_Corrente * 1000;
  valor_Corrente = valor_Corrente / 185;         //sensibilidade : 66mV/A para ACS712 30A / 185mV/A para ACS712 5A
  valor_Corrente = valor_Corrente / 1.41421356;
 
  Serial.print("Corrente = ");
  Serial.print(valor_Corrente);
  Serial.println(" A");
 
  potencia = valor_Corrente * tensao;
 
  Serial.print("Potencia: ");
  Serial.print(potencia);
  Serial.println(" W");


  Serial.print("Volt: ");
  Serial.print(vin);
  Serial.println(" V");

  

  Serial.print("Vout: ");
  Serial.print(vout);
  Serial.println(" V");

  Serial.print("Valor: ");
  Serial.print(value);
  Serial.println(" V");
  
  Serial.print(".");
  delay(500);
  Serial.print(".");
  delay(500);
  Serial.print(".");
  delay(500);
  if(valor_Corrente>0){
    lcd.setCursor(0, 0);
    lcd.print("Corrente:");
    lcd.print(valor_Corrente);
    lcd.print(" A");
    lcd.setCursor(0, 1);
    lcd.print("Potencia;");
    lcd.print(potencia);
    lcd.println(" W");
    Serial.println("");
  }
  
  
}
