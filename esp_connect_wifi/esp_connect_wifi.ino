#include <ESP8266WiFi.h>  //essa biblioteca já vem com a IDE. Portanto, não é preciso baixar nenhuma biblioteca adicional
#include <dht.h>


dht DHT;
//defines
#define RELAY D0
#define SSID_REDE     "xxxx"  //coloque aqui o nome da rede que se deseja conectar
#define SENHA_REDE    "xxxx"  //coloque aqui a senha da rede que se deseja conectar
#define INTERVALO_ENVIO_THINGSPEAK  300000  //intervalo entre envios de dados ao ThingSpeak (em ms)
#define DHT11_PIN D4

//constantes e variáveis globais
const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
int sensorValue = 0;  // value read from the pot
int outputValue = 0;  // value to output to a PWM pin
char EnderecoAPIThingSpeak[] = "api.thingspeak.com";
String ChaveEscritaThingSpeak = "thinkspeakKey";
long lastConnectionTime;
int analogSoloSeco = 600; //VALOR MEDIDO COM O SOLO SECO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int analogSoloMolhado = 300; //VALOR MEDIDO COM O SOLO MOLHADO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int percSoloSeco = 0; //MENOR PERCENTUAL DO SOLO SECO (0% - NÃO ALTERAR)
int percSoloMolhado = 100; //MAIOR PERCENTUAL DO SOLO MOLHADO (100% - NÃO ALTERAR)
 
WiFiClient client;
 
//prototypes
void EnviaInformacoesThingspeak(String StringDados);
void FazConexaoWiFi(void);
void Regar(void);
 
/*
 * Implementações
 */
 
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
     }   
}

void Regar(void){
  digitalWrite(RELAY, HIGH);
  delay(500);
  digitalWrite(RELAY, LOW);
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

    pinMode(RELAY,OUTPUT);
    Serial.begin(9600);
    lastConnectionTime = 0; 
    FazConexaoWiFi();
    Serial.println("Planta IoT com ESP8266 NodeMCU");
}
 
//loop principal
void loop()
{
    int soloRead = analogRead(analogInPin);
    outputValue = constrain(soloRead,analogSoloMolhado,analogSoloSeco); //MANTÉM valorLido DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
    outputValue = map(outputValue,analogSoloMolhado,analogSoloSeco,percSoloMolhado,percSoloSeco); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS
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
    Serial.println();

    if(outputValue<=60){
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
        EnviaInformacoesThingspeak("field1="+(String)TemperaturaLida+"&field2="+(String)UmidadePercentualLida+"&field3="+(String)outputValue);
    }
     digitalWrite(D0, HIGH);
    delay(500);
    digitalWrite(D0, LOW);
    delay(10000);
}
