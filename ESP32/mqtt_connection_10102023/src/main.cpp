#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

// WiFi credentials
const   char*   ssid        =   "BUAP_Estudiantes";
const   char*   password    =   "f85ac21de4";

// MQTT credentials
const   char*   mqtt_server =   "a9880d69.ala.us-east-1.emqxsl.com";
const   int     mqtt_port   =   8044;
const   char*   mqtt_user   =   "test";
const   char*   mqtt_pass   =   "test";

// Messages control
long lastMsg = 0;
char msg[50];
int value = 0;

WiFiClient espClient;
PubSubClient client(espClient);

// Prototype function
void setup_wifi();
void callback(char*,byte*,unsigned int);
void reconnect();

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (client.connected() == false) {
    reconnect();
  }
}

void setup_wifi(){
  Serial.println("Conectando a -> ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(250);
  }

  Serial.println("");
  Serial.println("Conexion exitosa");
  Serial.print("Mi IP es -> ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido bajo el topico -> ");
  Serial.println(topic);

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  } Serial.println();
}

void reconnect() {
  
}