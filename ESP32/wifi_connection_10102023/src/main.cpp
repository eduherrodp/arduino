#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "BUAP_Estudiantes";
const char* password = "f85ac21de4";

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println("");
  Serial.println("");

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

void loop() { 

}
 