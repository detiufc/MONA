#include <ESP8266WiFi.h> // Importa a Biblioteca ESP8266WiFi
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient

// WIFI
const char* SSID = "ENGENHARIA"; // SSID da Rede
const char* PASSWORD = "liukin1208"; // Senha da Rede

// MQTT
const char* BROKER_MQTT = "test.mosquitto.org"; // IP/URL DO BROKER MQTT
int BROKER_PORT = 1883; // Porta do Broker MQTT


// COORDENADAS - MINHA CASA
// Importante: substituir estas coordenadas pelas coordenadas geográficas da sua estação meteorológica
#define LAT "-3.722034"
#define LONG "-38.585472"
#define ID_MQTT  "-3.722034/-38.585472" //id mqtt definido como as coordenada geográficas para garantir unicidade

WiFiClient espClient; // Cria o objeto espClient

PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient

// Prototypes das funções utilizadas no decorrer do Firmware
void initSerial();
void initWiFi();
void initMQTT();
void readData();

void setup() {
  initSerial();
  initWiFi();
  initMQTT();
}
// Função para iniciar a comunicação serial
void initSerial() {
  Serial.begin(19200);
}
// Função para iniciar a Conexão com a rede WiFi
void initWiFi() {
   delay(10);
  //Serial.println();
  //Serial.print("Conectando-se em: ");
  //Serial.println(SSID);
  
  WiFi.begin(SSID, PASSWORD); // Conecta na Rede Wireless
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    //Serial.print(".");
  }
  //Serial.println();
  //Serial.print("Conectado na Rede ");
  //Serial.print(SSID);
  //Serial.println(" | IP ");
  //Serial.println(WiFi.localIP());
}
// Funcão para se conectar ao Broker MQTT
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT); // Informa o broker e porta utilizados
}

// Função para se reconectar ao Broker MQTT caso cair a comunicação
void reconnectMQTT() {
  while (!MQTT.connected()) {
    if (MQTT.connect(ID_MQTT)) {
    } else {
      delay(2000);
    }
  }
}

// Função para se reconectar a rede WiFi caso caia a comunicação com a rede
void recconectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
}
void readData() {
  if (Serial.available()) {
    String sensores = "";
    while (Serial.available()) {
      char c = (char)Serial.read();
      if (c != '\n') {
        sensores += c;
      }
      delay(10);
    }

    String payload = LAT;
    payload += ",";
    payload += LONG;
    payload += ",";
    payload += sensores;

    char payloadChar[payload.length()];
    payload.toCharArray(payloadChar, payload.length() + 1); // Converte a String para Char Array
    // Publica no tópico EMCOL todas as informações contidas na variável payload.
    MQTT.publish("home/temperatura", payloadChar);
  }
}

void loop() {
  if (!MQTT.connected()) {
    reconnectMQTT(); // Caso o ESP se desconecte do Broker, ele tenta se reconectar ao Broker
  }
  recconectWiFi(); // Caso o ESP perca conexão com a rede WiFi, ele tenta se reconetar a rede.
  readData();
  MQTT.loop();
}
