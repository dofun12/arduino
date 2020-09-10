#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <ESP8266WiFi.h>  //essa biblioteca já vem com a IDE. Portanto, não é preciso baixar nenhuma biblioteca adicional
#include <ESP8266HTTPClient.h>

#define SSID_REDE     "xxxx"  //coloque aqui o nome da rede que se deseja conectar
#define SENHA_REDE    "xxxx"  //coloque aqui a senha da rede que se deseja conectar

char EnderecoAPIThingSpeak[] = "api.thingspeak.com";
String ChaveEscritaThingSpeak = "AXCWXHGJBV05RGEG";
Adafruit_ADS1115 ads(0x48);
WiFiClient client;
float MAX_BITS_ADS115 = 65536.0f; //16 bits
long lastConnectionTime;
long INTERVALO_ENVIO_THINGSPEAK = 600000;
int samplingCount = 1000;
float workingVoltage = 2.4;

int getIndexUV(float milliVolt);
void FazConexaoWiFi(void);
void EnviaInformacoesThingspeak(String StringDados);


/**
float readGUVA(){
 int s = samplingCount;
 float sum = 0;
  for(int i=0; i<s; i++) {
    float v = analogRead(A0);
    sum = v + sum;
    delay(2);
  }
  float sensor_value_average = sum / s;
  float mV = (((sensor_value_average/1024) * workingVoltage)/1000);
  return mV;
}**/

void FazConexaoWiFi(void)
{
    client.stop();
    Serial.println("Conectando-se à rede WiFi...");
    Serial.println();  
    delay(1000);
    WiFi.begin(SSID_REDE, SENHA_REDE);
 
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
 
    Serial.println("");
    Serial.println("WiFi connectado com sucesso!");  
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
 
    delay(1000);
}
void EnviaInformacoesThingspeak(String StringDados)
{
    
    if (client.connect(EnderecoAPIThingSpeak, 80))
    {         
        Serial.println("Enviando: "+StringDados);
        //faz a requisição HTTP ao ThingSpeak
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+ChaveEscritaThingSpeak+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(StringDados.length());
        client.print("\n\n");
        client.print(StringDados);
   
        lastConnectionTime = millis();
        Serial.println("- Informações enviadas ao ThingSpeak!");
        Serial.println("Retorno: ");
        while(client.available()){
          String line = client.readStringUntil('\r');
          Serial.println(line);
        }
     }   
}

void setup(void)
{
  
  Serial.begin(9600);
  lastConnectionTime = 0; 
  FazConexaoWiFi();
  Serial.println("Hello!");
   
  Serial.println("Getting single-ended readings from AIN0..3");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");
//ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
//ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
//ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
//ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
//ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  ads.begin();
}

int getIndexUV(float milliVolt){
  int indexes[] = {50,227,318,408,503,606,696,765,881,976,1079,1170};
  int lastIndex = 0;
  
  int indexesLength = 12;
  Serial.print("Length: ");
  Serial.print(indexesLength);
  Serial.println("");
  for(int i=0;i<indexesLength;i++){
    if(milliVolt>=indexes[i]){
       lastIndex = i;
    }
  }
  return lastIndex;
}

double getVolt(float analogValue){
  return ((analogValue/MAX_BITS_ADS115)*2.4);
}
 
void loop(void)
{
  int16_t adc0, adc1, adc2, adc3;
  
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);

  //float mvUV = readGUVA();
  //float indexUV = readIndexUVA(mvUV);
  double voltageADC0 = getVolt(adc0);
  double voltageADC1 = getVolt(adc1);
  float millVoltageADC0 = voltageADC0*1000;
  int indexUV = getIndexUV(millVoltageADC0);
  Serial.print("A0: ");
  Serial.println(analogRead(A0));
  Serial.print("AIN0: ");
  Serial.println(adc0);
  Serial.print("Voltage AIN0: ");
  Serial.println(voltageADC0);
  Serial.print("Millis : ");
  Serial.println(millVoltageADC0);
  Serial.print("UV Index : ");
  Serial.println(indexUV);
  Serial.print("AIN1: ");
  Serial.println(adc1);
  Serial.print("Voltage AIN1: ");
  Serial.println(voltageADC1);
  Serial.println("##################################");
  
  
  
  //Serial.print("mvUV: ");
  //Serial.println(mvUV);
  //Serial.print("indexUV: ");
  //Serial.println(indexUV);

  if(!client.connected() && ((millis() - lastConnectionTime > INTERVALO_ENVIO_THINGSPEAK) || lastConnectionTime == 0))
  {
      EnviaInformacoesThingspeak("field1="+(String)+adc0+"&field2="+(String)adc1+"&field3="+(String)+millVoltageADC0+"&field4="+(String)+indexUV);
  }
  delay(1000);
}
