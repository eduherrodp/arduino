const byte motorA = 3;
const byte motorB = 4;
const byte enable = 10;
const byte trigg = 6;
const byte echo = 5;

byte velocidadInicial = 40;

// Configuración pantalla
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);
  pinMode(enable, OUTPUT);
  pinMode(trigg, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trigg, LOW);
  analogWrite(enable, velocidadInicial);

  // Setup pantalla
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}

void loop() {
  int distancia = mide_distancia();

  // Mapear la distancia a la velocidad en porcentaje
  int velocidad = map(distancia, 30, 100, 0, 100);

  velocidad = constrain(velocidad, 0, 100);

  control(distancia, velocidad+35);
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.print(" cm | Velocidad: ");
  Serial.print(velocidad);
  Serial.println("%");

  // Pantalla
  // Clear the buffer
  display.clearDisplay();

  // Mostramos la distancia del sensor y la velocidad del motor en porcentaje
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, SCREEN_HEIGHT / 2 - 5);
  display.print("Distancia: ");
  display.print(distancia);
  display.println(" cm");
  display.print("Velocidad: ");
  display.print(velocidad);
  display.println("%");

  // Dibujar la barra indicadora
  int barraWidth = map(velocidad, 0, 100, 0, SCREEN_WIDTH);
  display.fillRect(0, SCREEN_HEIGHT - 10, barraWidth, 10, WHITE);

  display.display();

  delay(200);
}

void control(int distancia, int vel) {
  if (distancia <= 30) {
    // Detener el motor completamente
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, LOW);
    analogWrite(enable, 0);
  } else if (distancia >= 100) {
    // Motor al máximo
    digitalWrite(motorA, HIGH);
    digitalWrite(motorB, LOW);
    analogWrite(enable, 255);
  } else {
    // Control proporcional en el rango intermedio
    digitalWrite(motorA, HIGH);
    digitalWrite(motorB, LOW);
    analogWrite(enable, map(vel, 0, 100, 0, 255));
  }
}

int mide_distancia() {
  gatillo();
  long tiempo = pulseIn(echo, HIGH);
  int distancia = tiempo / 58;
  return distancia;
}

void gatillo() {
  digitalWrite(trigg, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigg, LOW);
}