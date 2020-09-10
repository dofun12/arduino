#include <ESP8266WiFi.h>  //essa biblioteca já vem com a IDE. Portanto, não é preciso baixar nenhuma biblioteca adicional
#include <ESP8266HTTPClient.h>
#include <dht.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <ArduinoJson.h>

#define SSID_REDE     "xxxx"  //coloque aqui o nome da rede que se deseja conectar
#define SENHA_REDE    "xxxx"  //coloque aqui a senha da rede que se deseja conectar

Adafruit_ADS1115 ads(0x48);
WiFiClient client;

void FazConexaoWiFi(void);
void GetHora(void);


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

void GetHora(){
      
      HTTPClient http;  //Declare an object of class HTTPClient
       
      http.begin("http://192.168.15.50:8080/api/time/getHour");  //Specify request destination
      int httpCode = http.GET();                                                                  //Send the request
      Serial.print("Http code:");
      Serial.println(httpCode);
      
      if (httpCode > 0) { //Check the returning code
         // Parse response
         Serial.println("String: ");
         String input = http.getString();
         Serial.println(http.getString());
         
         DynamicJsonDocument doc(1024);
         deserializeJson(doc, input);
          
          // Read values
         //Serial.println(doc["unixtime"].as<long>()/(24*60*60));
         const char* data = doc["data"];
         Serial.println(doc["data"].as<int>());
      }
      http.end();
}

void setup() {
  ads.begin();
  Serial.begin(9600);
  FazConexaoWiFi();
  
}

void loop() {
  GetHora();

  int16_t adc0, adc1, adc2, adc3;
     
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);
  
  Serial.print("AIN0: ");
  Serial.println(adc0);
  Serial.print("AIN1: ");
  Serial.println(adc1);
  Serial.print("AIN2: ");
  Serial.println(adc2);
  Serial.print("AIN3: ");
  Serial.println(adc3);
  Serial.println(" ");
  delay(10000);
  
  // put your main code here, to run repeatedly:

}
