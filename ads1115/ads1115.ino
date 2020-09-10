#include <Wire.h>
#include <Adafruit_ADS1015.h>
 
Adafruit_ADS1115 ads(0x48);
float Voltage = 0.0;
float R1 = 10000000.0; // resistance of R1 (1000K) -see text!
float R2 = 1000000.0; // resistance of R2 (100K) - see text!
void setup(void)
{
//Wire.begin(D1,D2);
Serial.begin(9600);
Serial.println("Hello!");
 
Serial.println("Getting single-ended readings from AIN0..3");
Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");
ads.setGain(GAIN_ONE);
ads.begin();
}
 
void loop(void)
{
int16_t adc0, adc1, adc2, adc3;

adc0 = ads.readADC_SingleEnded(0);
adc1 = ads.readADC_SingleEnded(1);
adc2 = ads.readADC_SingleEnded(2);
adc3 = ads.readADC_SingleEnded(3);
Serial.print("A0: ");
Serial.println(analogRead(A0));
Serial.print("AIN0: ");
Serial.println(adc0);
Serial.print("AIN1: ");
Serial.println(adc1);
Serial.print("AIN2: ");
Serial.println(adc2);
Serial.print("AIN3: ");
Serial.println(adc3);
Serial.println(" ");
Voltage = (adc0 * 0.1875)/1000;
Voltage = (Voltage/(R2/(R1+R2)));
Serial.print("Voltage: ");
Serial.println(Voltage, 7);  
 
delay(1000);
}
