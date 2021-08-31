#include <PubSubClient.h>
#include <WiFi.h>

const char* ssid = "xxxxx";
const char* password = "xxxxx";
const char* mqttServer = "www.host.com.br";
const int mqttPort = 9999;
const char* mqttUser = "xxxxx";
const char* mqttPassword = "xxxxx";
int LED_BUILTIN = 2;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    pinMode (LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("Connecting to WiFi:");
        Serial.println(ssid);
    }

    Serial.println("Connected to the WiFi network");
    Serial.println("");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    while (!client.connected()) {
        Serial.println("Connecting to MQTT…");
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), mqttUser, mqttPassword )) {
            Serial.println("connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }

    Serial.print("Tentando enviar a mensagem");
    client.publish("esp∕test", "Hello from ESP32");
    client.subscribe("esp/test");

}

boolean isValidChar(char c){
  String permitedChars = "1234567890";
  //Previne que seja apenas digitado chars de 0 a 9, da para colocar letras se quiser tbm
  
  int len = permitedChars.length();
  //Previne caracteres errados
  if ((' ' >= c) && (c >= '~')){
    return false;
  }
  for(int i=0;i<len;i++){
    char pc = permitedChars[i];
    if(c==pc) return true;
  }
  return false;
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String text = "";
    for (int i = 0; i < length; i++) {
        char c = (char)payload[i];
        Serial.print("Testando... ");
        Serial.print(c);
        if(isValidChar(c)){
          Serial.print("Passou \n");
          text+=c;
        }
    }
    text = text.substring(0,3); //Previne numeros maiores que 100
    Serial.println("Vamos fazer um jam");    
    Serial.print(text);    

    int number = text.toInt();
    if(number==0 || number<1){
      return;
    }
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(number*1000);
    digitalWrite(LED_BUILTIN, LOW);
    
    Serial.println();
    Serial.println(" — — — — — — — — — — — -");

}

void loop() {
    client.loop();
}
