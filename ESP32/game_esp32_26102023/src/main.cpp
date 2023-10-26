#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define Resolucion 3.3 / 4095

int rADC = 0;
float voltaje = 0.0;

int plataformaX;
int plataformaY;
int plataformaWidth = 30;
int plataformaHeight = 3;
int plataformaSpeed = 2;

int potValue;
const int pot = 34;
const int botonPin = 18;
int pelotaX;
int pelotaY;
int pelotaSpeedX = 1;
int pelotaSpeedY = 1; // Cambio la dirección de la velocidad Y
int pelotaSize = 1;

bool juegoActivo = true;

unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 15; // Intervalo de actualización en milisegundos

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

  analogReadResolution(12); // Resolución del ADC de 12 bits - 4095 combinaciones
  analogSetPinAttenuation(pot, ADC_11db); // Rango de voltaje de 0 a 3.6V

  pinMode(pot, INPUT);

  // Inicializa la posición inicial de la plataforma
  plataformaX = (SCREEN_WIDTH - plataformaWidth) / 2;
  plataformaY = SCREEN_HEIGHT - 3;  // Puedes ajustar la posición inicial

  // Inicializa la posición inicial de la pelota
  pelotaX = SCREEN_WIDTH / 2;
  pelotaY = 10; // Inicializa la pelota en la parte superior de la pantalla
}

int nivel = 1;
int puntos = 0;
int vidas = 3;

int x1 = 50;
int x2 = 80;

void loop() {
  unsigned long currentTime = millis();

  if (juegoActivo) {
    // Actualiza el juego solo si ha pasado el tiempo de actualización
    if (currentTime - lastUpdateTime >= updateInterval) {
      lastUpdateTime = currentTime;

      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.print("lvl: ");
      display.print(nivel);
      display.setCursor(50, 0);
      display.print("pt: ");
      display.print(puntos);
      display.setCursor(90, 0);
      display.print("vi: ");
      display.print(vidas);

      display.drawLine(0, 10, SCREEN_WIDTH, 10, WHITE);

      // Lógica
      rADC = analogRead(pot);
      voltaje = rADC * Resolucion;
      Serial.print("Valor ADC: ");
      Serial.print(rADC);
      Serial.print(" - Voltaje: ");
      Serial.print(voltaje, 2);
      Serial.println(" V");

      potValue = analogRead(pot);
      plataformaX = map(potValue, 0, 4095, 0, SCREEN_WIDTH - plataformaWidth);

      // Mueve la pelota
      pelotaX += pelotaSpeedX;
      pelotaY += pelotaSpeedY;

      // Colisión con la plataforma
      if (pelotaY + pelotaSize >= plataformaY && pelotaX >= plataformaX && pelotaX <= plataformaX + plataformaWidth) {
        pelotaSpeedY = -pelotaSpeedY;
        pelotaSpeedX = random(-1, 2); // Dirección aleatoria en X: -1, 0 o 1
        puntos++;
        // Establecemos la posición de la pelota en la parte superior de la pantalla
        pelotaX = random(10, SCREEN_WIDTH - 10); // Posición aleatoria en X
        pelotaY = 10; // Vuelve a la parte superior
      }

      // Colisión con los límites laterales
      if (pelotaX + pelotaSize >= SCREEN_WIDTH || pelotaX <= 0) {
        pelotaSpeedX = -pelotaSpeedX;
      }

      // Limitar la plataforma dentro de los límites de la pantalla
      plataformaX = constrain(plataformaX, 0, SCREEN_WIDTH - plataformaWidth);

      // Limitar la pelota dentro de los límites de la pantalla
      pelotaX = constrain(pelotaX, 0, SCREEN_WIDTH - pelotaSize);
      pelotaY = constrain(pelotaY, 10, SCREEN_HEIGHT - pelotaSize);

      // Si la pelota se va por debajo de la pantalla, resta una vida y reinicia la posición de la pelota
      if (pelotaY >= SCREEN_HEIGHT) {
        vidas--;
        pelotaX = random(10, SCREEN_WIDTH - 10); // Posición aleatoria en X
        pelotaY = 10; // Vuelve a la parte superior
      }

      // Si se pierden todas las vidas, muestra "GAME OVER" y desactiva el juego
      if (vidas <= 0) {
        display.setTextSize(2);
        display.setCursor(10, 25);
        display.print("GAME OVER");
        juegoActivo = false;
      } else {
        // Si se alcanzan 5 puntos, sube de nivel y aumenta la velocidad de la pelota
        if (puntos >= 5) {
          nivel++;
          puntos = 0;
          pelotaSpeedX = random(-1, 2); // Dirección aleatoria en X: -1, 0 o 1
          pelotaSpeedY = random(1, 3); // Dirección aleatoria en Y: 1, 2 o 3
        }
      }

      // Dibuja la plataforma en la pantalla
      display.fillRect(plataformaX, plataformaY, plataformaWidth, plataformaHeight, SSD1306_WHITE);

      // Dibuja la pelota
      display.fillCircle(pelotaX, pelotaY, pelotaSize, SSD1306_WHITE);

      display.display();
    }
  } else {
    // Juego inactivo, muestra "GAME OVER" y espera a que se presione el botón para reiniciar
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 25);
    display.print("GAME OVER");
    
    // Verifica si se ha presionado el botón para reiniciar el juego
    if (digitalRead(botonPin) == LOW) {
      juegoActivo = true; 
      nivel = 1;
      puntos = 0;
      vidas = 3;
      pelotaSpeedX = random(-1, 2); // Dirección aleatoria en X: -1, 0 o 1
      pelotaSpeedY = random(1, 3); // Dirección aleatoria en Y: 1, 2 o 3
      pelotaX = random(10, SCREEN_WIDTH - 10); // Posición aleatoria en X
      pelotaY = 10; // Vuelve a la parte superior
    }
    
    display.display();
  }
}