#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin D6

const char* SSID = "xxxx"; //Seu SSID da Rede WIFI
const char* PASSWORD = "xxxx"; // A Senha da Rede WIFI
const char* MQTT_SERVER = "192.168.15.66"; //Broker do Mosquitto.org

int value = 0; //Contador de é incrementado de -20 a 50
long lastMsg = 0;
char msg[50];


int val = 0;
float Vout = 0.00;
float Vin = 0.00;
float R1 = 978000.00; // resistance of R1 (1M) 
float R2 = 98100.00; // resistance of R2 (100K)

WiFiClient CLIENT;
PubSubClient MQTT(CLIENT);
DHT dht(dht_dpin, DHTTYPE); 

//CONFIGURAÇÃO DA INTERFACE DE REDE
void setupWIFI() {
  dht.begin();
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Conectando na rede: ");
  Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
   Serial.print(".");
   delay(500);
  }
}
void setup(void) {
  Serial.begin(9600);
  setupWIFI(); 
  MQTT.setServer(MQTT_SERVER, 1883);
}
void reconectar() {
  while (!MQTT.connected()) {
    Serial.println("Conectando ao Broker MQTT.");
    if (MQTT.connect("ESP8266","xxxx","xxxx")) {
      Serial.println("Conectado com Sucesso ao Broker");
    } else {
      Serial.print("Falha ao Conectador, rc=");
      Serial.print(MQTT.state());
      Serial.println(" tentando se reconectar...");
      delay(3000);
    }
  }
}
void loop(void) {
  if (!MQTT.connected()) {
    reconectar();
  }
  MQTT.loop();
  //long now = millis();
  // Nessa função é onde a brincadeira acontece. A cada segundo ele PUBLICA aquele CONTADOR de -20 a 50 para o Broker MQTT(mosquitto.org)
  //if (now - lastMsg > 1000) {
    char dataA[5];
    char dataB[5];
    String(dht.readHumidity(),2).toCharArray(dataA,5);
    String(dht.readTemperature(),2).toCharArray(dataB,5);
    
    Serial.print("H float: ");
    Serial.print(dht.readHumidity());
    
    Serial.print(" H: ");
    Serial.print(dataA);

    Serial.print(" T float: ");
    Serial.print(dht.readTemperature());
    Serial.print(" T: ");
    Serial.print(dataB);
    Serial.println("");
    
    MQTT.publish("0/sensors/humidity", dataA);
    MQTT.publish("0/sensors/temperature",  dataB);
  //}
  delay(30000);
}
