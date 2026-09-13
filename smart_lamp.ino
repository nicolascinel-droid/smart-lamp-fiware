/*
  Projeto: Smart Lamp - FIWARE

  Autores:
  Nicolas Cinel
  Luis Fernando
  Leonrdo Formigari 
  Kelvin Lucas
  

  Projeto acadêmico desenvolvido utilizando ESP32, FIWARE,
  protocolo MQTT e sensor LDR para monitoramento de luminosidade
  e controle remoto do LED.

  Baseado no projeto FIWARE Descomplicado.
*/

#include <WiFi.h>
#include <PubSubClient.h>

// ================= CONFIGURAÇÕES =================

// Wi-Fi utilizado na simulação Wokwi
const char* default_SSID = "Wokwi-GUEST";
const char* default_PASSWORD = "";

// Broker MQTT - FIWARE na AWS
const char* default_BROKER_MQTT = "100.57.167.195";
const int default_BROKER_PORT = 1883;

// Tópicos MQTT da Smart Lamp
const char* default_TOPICO_SUBSCRIBE = "/TEF/lamp002/cmd";
const char* default_TOPICO_PUBLISH_1 = "/TEF/lamp002/attrs";
const char* default_TOPICO_PUBLISH_2 = "/TEF/lamp002/attrs/l";

// Identificação MQTT
const char* default_ID_MQTT = "fiware_002";

// LED onboard do ESP32
const int default_D4 = 2;

const char* topicPrefix = "lamp002";

char* SSID = const_cast<char*>(default_SSID);
char* PASSWORD = const_cast<char*>(default_PASSWORD);
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);

int BROKER_PORT = default_BROKER_PORT;

char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE);
char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1);
char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2);

char* ID_MQTT = const_cast<char*>(default_ID_MQTT);

int D4 = default_D4;

WiFiClient espClient;
PubSubClient MQTT(espClient);

char EstadoSaida = '0';

// Inicializa comunicação serial
void initSerial() {
  Serial.begin(115200);
}

// Inicializa conexão Wi-Fi
void initWiFi() {
  delay(10);

  Serial.println("------ Conexao WI-FI ------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);

  reconectWiFi();
}

// Reconecta ao Wi-Fi caso a conexão seja perdida
void reconectWiFi() {
  if (WiFi.status() == WL_CONNECTED)
    return;

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi conectado!");

  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());

  digitalWrite(D4, LOW);
}

// Configura o Broker MQTT
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

// Reconecta ao Broker MQTT
void reconnectMQTT() {
  while (!MQTT.connected()) {

    Serial.print("Tentando conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);

    if (MQTT.connect(ID_MQTT)) {

      Serial.println("Conectado ao Broker MQTT!");

      // ESP32 fica aguardando comandos enviados pelo FIWARE
      MQTT.subscribe(TOPICO_SUBSCRIBE);

    } else {

      Serial.println("Falha ao conectar no Broker.");
      Serial.println("Nova tentativa em 2 segundos...");

      delay(2000);
    }
  }
}

// Recebe os comandos ON e OFF enviados pelo FIWARE
void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  String msg;

  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("Mensagem recebida: ");
  Serial.println(msg);

  String onTopic = String(topicPrefix) + "@on|";
  String offTopic = String(topicPrefix) + "@off|";

  // Liga o LED
  if (msg.equals(onTopic)) {
    digitalWrite(D4, HIGH);
    EstadoSaida = '1';
    Serial.println("LED LIGADO");
  }

  // Desliga o LED
  if (msg.equals(offTopic)) {
    digitalWrite(D4, LOW);
    EstadoSaida = '0';
    Serial.println("LED DESLIGADO");
  }
}

// Verifica conexões Wi-Fi e MQTT
void VerificaConexoesWiFIEMQTT() {

  if (!MQTT.connected()) {
    reconnectMQTT();
  }

  reconectWiFi();
}

// Envia o estado atual do LED para o FIWARE
void EnviaEstadoOutputMQTT() {

  if (EstadoSaida == '1') {
    MQTT.publish(TOPICO_PUBLISH_1, "s|on");
    Serial.println("Led Ligado");
  }

  if (EstadoSaida == '0') {
    MQTT.publish(TOPICO_PUBLISH_1, "s|off");
    Serial.println("Led Desligado");
  }

  delay(1000);
}

// Configura o LED onboard como saída
void InitOutput() {
  pinMode(D4, OUTPUT);
  digitalWrite(D4, LOW);
}

// Faz a leitura do sensor LDR e envia a luminosidade ao FIWARE
void handleLuminosity() {

  const int potPin = 34;

  int sensorValue = analogRead(potPin);

  // Converte leitura do ESP32 para porcentagem de 0 a 100
  int luminosity = map(sensorValue, 0, 4095, 0, 100);

  String mensagem = String(luminosity);

  Serial.print("Valor da luminosidade: ");
  Serial.println(mensagem);

  // Publica o valor da luminosidade via MQTT
  MQTT.publish(TOPICO_PUBLISH_2, mensagem.c_str());
}

void setup() {

  InitOutput();
  initSerial();
  initWiFi();
  initMQTT();

  delay(2000);
}

void loop() {

  VerificaConexoesWiFIEMQTT();
  EnviaEstadoOutputMQTT();
  handleLuminosity();

  MQTT.loop();
}
