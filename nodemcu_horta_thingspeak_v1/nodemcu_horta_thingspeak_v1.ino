#include <ESP8266WiFi.h>  //essa biblioteca já vem com a IDE. Portanto, não é preciso baixar nenhuma biblioteca adicional
#include <ESP8266HTTPClient.h>
#include <dht.h>
#include <ArduinoJson.h>

dht DHT;
//defines
#define LUZ D2
#define MOTOR D1
#define SSID_REDE     "xxxx"  //coloque aqui o nome da rede que se deseja conectar
#define SENHA_REDE    "xxxx"  //coloque aqui a senha da rede que se deseja conectar
#define DHT11_PIN D4

//constantes e variáveis globais
const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
int sensorValue = 0;  // value read from the pot
int outputValue = 0;  // value to output to a PWM pin
char EnderecoAPIThingSpeak[] = "api.thingspeak.com";
String ChaveEscritaThingSpeak = "thinkspeakKey";
long lastConnectionTime;
long lastRega = 0;
int ligar = 0;

int analogSoloSeco = 300; //VALOR MEDIDO COM O SOLO SECO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int analogSoloMolhado = 1024; //VALOR MEDIDO COM O SOLO MOLHADO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int percSoloSeco = 0; //MENOR PERCENTUAL DO SOLO SECO (0% - NÃO ALTERAR)
int percSoloMolhado = 100; //MAIOR PERCENTUAL DO SOLO MOLHADO (100% - NÃO ALTERAR)
boolean ligarBomba = false;
boolean ligarLuz = false;
int maxValorSolo = 1024;
int minValorSolo = 300;
int regaValue = 50;
long tempoRega = 5000;
long cooldownRega = 300000;
long INTERVALO_ENVIO_THINGSPEAK = 600000;
WiFiClient client;
 
//prototypes
void EnviaInformacoesThingspeak(String StringDados);
void FazConexaoWiFi(void);
void Regar(void);
void GetHora();
void ligarALuz();
void desligarLuz();
void initConfig();

/*
 * Implementações
 */
void ligarALuz(){
  if(ligarLuz){
    digitalWrite(LUZ, HIGH);
    Serial.println("Luz ligada");
  }
  
}

void desligarLuz(){
  digitalWrite(LUZ, LOW);
  Serial.println("Luz apagada");
}

void initConfig(){
  HTTPClient http;  //Declare an object of class HTTPClient
       
      http.begin("http://200.98.201.64/config.json");  //Specify request destination
      int httpCode = http.GET();                                                                  //Send the request
       
      if (httpCode == 200) { //Check the returning code
         // Parse response
         Serial.println("Refreshing configs...");
         DynamicJsonDocument doc(1024);
         deserializeJson(doc, http.getStream());
         ligarBomba = doc["ligarBomba"].as<boolean>();
         ligarLuz = doc["ligarLuz"].as<boolean>();
         maxValorSolo = doc["maxValorSolo"].as<int>();
         minValorSolo = doc["minValorSolo"].as<int>();
         regaValue = doc["regaValue"].as<int>();
         tempoRega = doc["tempoRega"].as<long>();
         cooldownRega = doc["cooldownRega"].as<long>();
         INTERVALO_ENVIO_THINGSPEAK = doc["intervaloThingSpeak"].as<long>();
      }
      http.end();
}

void GetHora(){
      
      HTTPClient http;  //Declare an object of class HTTPClient
       
      http.begin("http://worldtimeapi.org/api/timezone/America/Sao_Paulo.json");  //Specify request destination
      int httpCode = http.GET();                                                                  //Send the request
       
      if (httpCode == 200) { //Check the returning code
         // Parse response
         DynamicJsonDocument doc(1024);
         deserializeJson(doc, http.getStream());
          
          // Read values
         //Serial.println(doc["unixtime"].as<long>()/(24*60*60));
         const char* lastDatetime = doc["datetime"];
         Serial.println(lastDatetime);
         if(String(lastDatetime).indexOf("T07:")>0){
              ligar = 1;
         }else if(String(lastDatetime).indexOf("T08:")>0){
              ligar = 1;
         }else if(String(lastDatetime).indexOf("T09:")>0){
              ligar = 1;
         }else if(String(lastDatetime).indexOf("T10:")>0){
              ligar = 1;
         }else if(String(lastDatetime).indexOf("T11:")>0){
              ligar = 1;
         }else if(String(lastDatetime).indexOf("T12:")>0){
              ligar = 1;
         }else if(String(lastDatetime).indexOf("T13:")>0){
              ligar = 1;
         }else if(String(lastDatetime).indexOf("T14:")>0){
              ligar = 1;
         }else if(String(lastDatetime).indexOf("T15:")>0){
              ligar = 1;
         }else if(String(lastDatetime).indexOf("T16:")>0){
              ligar = 1;
         }else if(String(lastDatetime).indexOf("T17:")>0){
              ligar = 1;
         }else if(String(lastDatetime).indexOf("T18:")>0){
              ligar = 1;
         }else{
              ligar = 0;
         }
         
      }
      http.end();
}
 
//Função: envia informações ao ThingSpeak
//Parâmetros: String com a  informação a ser enviada
//Retorno: nenhum
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

void Regar(void){
  if(ligarBomba){
    if((millis() - lastRega) > cooldownRega || lastRega==0){
      Serial.println("Regando...");
      digitalWrite(MOTOR, HIGH);
      delay(tempoRega);
      digitalWrite(MOTOR, LOW);
      lastRega = millis();
    }
  }
  
}
//Função: faz a conexão WiFI
//Parâmetros: nenhum
//Retorno: nenhum
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
 
//Função: faz a leitura do nível de umidade
//Parâmetros: nenhum
//Retorno: umidade percentual (0-100)
//Observação: o ADC do NodeMCU permite até, no máximo, 3.3V. Dessa forma,
//            para 3.3V, obtem-se (empiricamente) 978 como leitura de ADC
void setup()
{  
    pinMode(LUZ,OUTPUT);
    pinMode(MOTOR,OUTPUT);

    digitalWrite(LUZ, HIGH);
    digitalWrite(MOTOR, HIGH);
    delay(300);
    digitalWrite(LUZ, LOW);
    digitalWrite(MOTOR, LOW);
    
    
    Serial.begin(9600);
    lastConnectionTime = 0; 
    FazConexaoWiFi();
    initConfig();
    Serial.println("Planta IoT com ESP8266 NodeMCU");
    GetHora();
}
 
//loop principal
void loop()
{
    int soloRead = analogRead(analogInPin);
    outputValue = constrain(soloRead,minValorSolo,maxValorSolo); //MANTÉM valorLido DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
    outputValue = map(outputValue,minValorSolo,maxValorSolo,percSoloSeco,percSoloMolhado); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS
    DHT.read11(DHT11_PIN);
    float TemperaturaLida = DHT.temperature;
    float UmidadePercentualLida = DHT.humidity;
    char FieldUmidade[11];
    
    Serial.print("Umidade ar: ");
    Serial.print((String)UmidadePercentualLida);
    Serial.print(" Temperatura Lida: ");
    Serial.print((String)TemperaturaLida);
    Serial.print(" Umidade Solo: ");
    Serial.print(outputValue);
    Serial.print(" Umidade Solo Anlog: ");
    Serial.print(soloRead);
    Serial.print(" Solo read: ");
    Serial.print(soloRead);
    Serial.println();

    //if(outputValue<=50 && outputValue> 0){
    if(outputValue<=50){
      Regar();
    }
    
    //Força desconexão ao ThingSpeak (se ainda estiver desconectado)
    if (client.connected())
    {
        client.stop();
        Serial.println("- Desconectado do ThingSpeak");
        Serial.println();
    }
     
    //verifica se está conectado no WiFi e se é o momento de enviar dados ao ThingSpeak
    if(!client.connected() && 
      (millis() - lastConnectionTime > INTERVALO_ENVIO_THINGSPEAK))
    {
        EnviaInformacoesThingspeak("field1="+(String)TemperaturaLida+"&field2="+(String)UmidadePercentualLida+"&field3="+(String)outputValue+"&field4="+(String)soloRead);
        GetHora();
        initConfig();
    }
    if(ligar>0){
      ligarALuz();
    }else{
      desligarLuz();
    }
    
    delay(10000);
}
