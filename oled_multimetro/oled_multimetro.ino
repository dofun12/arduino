
// Arduino Nano SCL -> A5; SDA -> A4;
#include "U8glib.h"
#include "EmonLib.h" //INCLUSÃO DE BIBLIOTECA
#define AMOSTRAS 100
#define CURRENT_CAL 15 //VALOR DE CALIBRAÇÃO (DEVE SER AJUSTADO EM PARALELO COM UM MULTÍMETRO MEDINDO A CORRENTE DA CARGA)
const int pinoSensor = A0; //PINO ANALÓGICO EM QUE O SENSOR ESTÁ CONECTADO

char charCurrent[8];
char charVoltage[8];
char charPotencia[8];
int cycle = 0;

EnergyMonitor emon1;
U8GLIB_SH1106_128X64 u8g(U8G_PI_I2C_OPTION);  // I2C / TWI 

float readVoltage(uint8_t portaAnalogica, float res1, float res2) {
  float total=0;  
  for (int i=0; i<AMOSTRAS; i++) {
    total += 1.0 * analogRead(portaAnalogica);
    delay(5);
  }
  float relacao = ((res1+res2)/res2);
  float tensaoArduino = 4.64;
  float volt = ((((total / (float)AMOSTRAS) * tensaoArduino) / 1023.0) * relacao);
  return volt;
}  
double readCurrentV2(){
  double Voltage = 0;
  double Amps = 0;
  int ACSoffset = 2500;
  int mVperAmp = 185;
  int RawValue = 0;
  int measures = 100;
  double total = 0.0;
  for(int m = 0;m<measures;m++){
    total = total+analogRead(A0);
    delay(10);
  }
  RawValue = (total/measures);
  Voltage = (RawValue / 1023.0) * 5000; // Gets you mV  
  Amps = ((Voltage - ACSoffset) / mVperAmp);
  if(Amps<0){
    //Amps = (Amps * (-1));
  }
  return (Amps);
}

double readCurrent(){
  emon1.calcVI(50, 500); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS / TEMPO LIMITE PARA FAZER A MEDIÇÃO)
  return (emon1.Irms); //VARIÁVEL RECEBE O VALOR DE CORRENTE RMS OBTIDO
  /**
  
  **/
}

void u8g_prepare(void) {
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}


void setup() {
 Serial.begin(9600);  
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
  u8g.setColorIndex(1); // Instructs the display to draw with a pixel on. 
  emon1.current(pinoSensor, CURRENT_CAL);
}
void updateVars(){
  
  float tensaoA1 = readVoltage(A1,10000000,1000000);
  double current = readCurrent();
  Serial.print("Tensao em A0: ");
  Serial.print(tensaoA1);
  Serial.println ("V");

  

  Serial.print("Analog A0: ");
  Serial.print(analogRead(A0));
  Serial.println ("val");

  Serial.print("Corrente medida: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(current); //IMPRIME NA SERIAL O VALOR DE CORRENTE MEDIDA
  Serial.println("A"); //IMPRIME O TEXTO NA SERIAL
  float potencia = (tensaoA1*current);
  dtostrf(current, 6, 2, charCurrent); // Leave room for too large numbers!
  dtostrf(tensaoA1, 6, 2, charVoltage); // Leave room for too large numbers!
  dtostrf(potencia, 6, 2, charPotencia); // Leave room for too large numbers!
}
void loop() {
    
  u8g.firstPage();
  do {  
    draw();
  } while( u8g.nextPage() );
  delay(500);   
  if(cycle>=6){
    updateVars();
    cycle=0;
  }
  cycle++;
}

void draw(){
  u8g.drawStr( 3, 0,"Volts:");
  u8g.drawStr( 3, 10,charVoltage);   
  u8g.drawStr( 3, 20,"Amps:");
  u8g.drawStr( 3, 30,charCurrent);
  u8g.drawStr( 3, 40,"Watts:");
  u8g.drawStr( 3, 50,charPotencia);
}
