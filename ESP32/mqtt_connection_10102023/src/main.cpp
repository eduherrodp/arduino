#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

// WiFi credentials
const   char*   ssid        =   "BUAP_Estudiantes";
const   char*   password    =   "f85ac21de4";

// MQTT credentials
const   char*   mqtt_server =   "broker.emqx.io";
const   int     mqtt_port   =   1883;
const   char*   mqtt_user   =   "test";
const   char*   mqtt_pass   =   "test";
const   char*   mqtt_topic  =   "salida_x03014";

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

  client.loop();

  if (millis() - lastMsg > 2000) {
    lastMsg = millis();
    value++;
    String mes = "Valor -> " + String(value);
    mes.toCharArray(msg, 50);
    client.publish(mqtt_topic, msg);
    Serial.println("[Mensaje enviado]: " + mes);
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
  } 

  if((char) payload[0] == '0') {
    digitalWrite(BUILTIN_LED, LOW);
    Serial.println("LED OFF");
  } else if ((char) payload[0] == '1') {
    digitalWrite(BUILTIN_LED, HIGH);
    Serial.println("LED ON");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Intentando conexión MQTT...");
    String clientId = "iot_1_";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("Conexión a MQTT exitosa");
      client.publish(mqtt_topic, "primer mensaje");
      client.subscribe("entrada_x03014");
    } else {
      Serial.print("Fallo la conexion ");
      Serial.print(client.state());
      Serial.println(" Se intentara de nuevo en 5 segundos");
      delay(5000);
    }
  }
}