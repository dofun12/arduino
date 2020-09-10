#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#define analogInput A0

const char* SSID = "xxxx"; //Seu SSID da Rede WIFI
const char* PASSWORD = "xxxx"; // A Senha da Rede WIFI
const char* MQTT_SERVER = "192.168.15.66"; //Broker do Mosquitto.org

int val = 0;
float Vout = 0.00;
float Vin = 0.00;
float R1 = 978000.00; // resistance of R1 (1M) 
float R2 = 98100.00; // resistance of R2 (100K)

int value = 0; //Contador de é incrementado de -20 a 50
long lastMsg = 0;
char msg[50];
WiFiClient CLIENT;
PubSubClient MQTT(CLIENT);

float lePorta(uint8_t portaAnalogica, int AMOSTRAS) {
  float total=0;  
  for (int i=0; i<AMOSTRAS; i++) {
    total += 1.0 * analogRead(portaAnalogica);
    delay(5);
  }
  return total / (float)AMOSTRAS;
}  

//CONFIGURAÇÃO DA INTERFACE DE REDE
void setupWIFI() {
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

void doPub(){
  
  float relacao = (((R1+R2)/R2)) + 0.90;
  val = lePorta(analogInput, 1000);//reads the analog input
  Vout = (val * 3.3) / 1024.00; // formula for calculating voltage out i.e. V+, here 5.00
  Vin = (Vout * relacao); // formula for calculating voltage in i.e. GND
  
  char dataA[5];
  String(Vin,3).toCharArray(dataA,5);
  Serial.println("VIn sended:");
  Serial.println(dataA);

  Serial.println("Original:");
  Serial.println(Vin);
  MQTT.publish("4/sensors/voltimeter", dataA);
}

void loop(void) {
  if (!MQTT.connected()) {
    reconectar();
  }
  MQTT.loop();
  //long now = millis();
  // Nessa função é onde a brincadeira acontece. A cada segundo ele PUBLICA aquele CONTADOR de -20 a 50 para o Broker MQTT(mosquitto.org)
  //if (now - lastMsg > 1000) {
        
    doPub();
  //}
  delay(25000);
}
