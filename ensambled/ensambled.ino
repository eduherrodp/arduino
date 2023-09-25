/**** Dallas ****/
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float temperature = 0;

/**** Motor ****/
int speedPin=11;
int dirPin1=12;
int dirPin2=13;
int speedMotor=255; // Velocidad inicial

// Temperatura para encender motor
float temperaturaEncendido = 30.0;

/**** Botones ****/
const int incrementButtonPin = 2; // Pin del botón incrementar
const int decrementButtonPin = 3; // Pin del botón decrementar

int incrementState = 0; // Estado actual
int lastIncrementState = HIGH; // Estado anterior
int decrementState = 0; // Estado actual
int lastDecrementState = HIGH; // Estado anterior

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // Retardo de debounce en ms

/**** LCD ****/
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

  /**** Dallas ****/
  sensors.begin();
  
  /**** Motor ****/
  pinMode(speedPin, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);

  /**** Botones ****/
  pinMode(incrementButtonPin, INPUT_PULLUP);
  pinMode(decrementButtonPin, INPUT_PULLUP);

  /**** LCD ****/
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}

void loop() {
  /**** Dallas ****/

  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);

  /**** Motor ****/
  // Dirección de giro del motor
  digitalWrite(dirPin1, 1);
  digitalWrite(dirPin2, 0);

  // Inicio de velocidad al máximo (255)

  if (temperature > temperaturaEncendido) {
    /*  Si la temperatura leida por el sensor es mayor a la 
    *   temperatura definida para encender el motor, se enciende el motor
    */
    analogWrite(speedPin, speedMotor);

  } else {
    // En caso contrario, se apaga
    analogWrite(speedPin, 0);
    delay(50); // medio segundo de espera en lo que se apaga
  }

  /**** Botones ****/
  incrementState = digitalRead(incrementButtonPin);
  decrementState = digitalRead(decrementButtonPin);

  // Verificar si ha pasado suficiente tiempo desde el último rebote
  if (millis() - lastDebounceTime > debounceDelay) {
    // Comparar el estado del botón con su estado anterior
    if (incrementState != lastIncrementState) {
      if (incrementState == HIGH) {
        // Aumentaremos de 2 en 2 para que no sea lento 
        temperaturaEncendido = temperaturaEncendido + 2.0;
      }
      lastDebounceTime = millis();
    }

    if (decrementState != lastDecrementState) {
      if (decrementState == HIGH) {
        temperaturaEncendido = temperaturaEncendido - 2.0;
      }
      lastDebounceTime = millis();
    }
  }

  lastIncrementState = incrementState;
  lastDecrementState = decrementState;

  /**** LCD ****/
    // Clear the buffer
  display.clearDisplay();

  // Mostramos la T del sensor y la T límite
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  display.print("T = ");
  display.print(temperature);
  display.print(" T l = ");
  display.print(temperaturaEncendido);

  // Creamos la línea divisora 
  int lineaY = temperaturaEncendido;
  display.drawLine(0, lineaY, SCREEN_WIDTH -1, lineaY, WHITE);
  display.display();

  /**** Serial ****/

  Serial.print("T = ");
  Serial.print(temperature);
  Serial.print("| Limit T = ");
  Serial.println(temperaturaEncendido);
  delay(200); // Lectura cada 200 ms
  
}

