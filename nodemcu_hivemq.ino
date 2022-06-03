
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

// CONFIGURAÇÕES DO WIFI
const char* ssid = "HELIOHOME";
const char* password = "h3l10home";

// CONFIGURAÇÕES DE CONEXÃO COM O BROKER HIVEMQ
const char* mqtt_server = "7f5f4c2627724c69ace02edae5d1f6c5.s1.eu.hivemq.cloud"; // replace with your broker url
const char* mqtt_username = "MACK32055781";
const char* mqtt_password = "1234mack";
const int mqtt_port =8883;


//=====================
//== CONFIGS GLOBAIS ==
//=====================

int valorluz = 0;
int valorluzfinal = 0;
WiFiClientSecure espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int sensor1 = 0;
const char* estadoC = 0;
int command1 =0;
const char* topico1= "MACK32055781_1";
const char*  topico2="MACK32055781_2";
const char* topico_comando="command1";



//===================
//== conectar wifi ==
//===================

void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}


//===============
//== reconnect ==
//===============

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266ClientMCK-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      client.subscribe(topico_comando);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

//===========
//== setup ==
//===========

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(1);
  setup_wifi();
  pinMode(2, OUTPUT);

  //CONEXÃO SEGURA OU INSEGURA (COM OU SEM CERTIFICADO)
  #ifdef ESP8266
    espClient.setInsecure();
  #else   // for the ESP32
    espClient.setCACert(root_ca);
  #endif
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}


//==========
//== loop ==
//==========

void loop() {

  if (!client.connected()) reconnect();
  client.loop();
  
  valorluz= analogRead (A0);
  valorluzfinal= valorluz-300;

  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    sensor1= valorluzfinal;
    if((valorluzfinal) < 700){
      estadoC = "FECHADA";
      }
    else {
      estadoC = "ABERTA";
      }
    publishMessage(topico1,String(sensor1),true);    
    publishMessage(topico2,String(estadoC),true);
    
  }
}

//============================ 
//=== PUBLICANDO MENSAGENS ===
//============================ 
void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true))
      Serial.println("Message publised ["+String(topic)+"]: "+payload);
}

//========================================  
//=== CALLBACK DAS MENSAGENS DO BROKER ===
//========================================  
void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];
  
  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);
    if( strcmp(topic,topico_comando) == 0){
     if (incommingMessage.equals("1")) digitalWrite(2, LOW);   // Turn the LED on 
     else digitalWrite(2, HIGH);  // Turn the LED off 
  }
}
