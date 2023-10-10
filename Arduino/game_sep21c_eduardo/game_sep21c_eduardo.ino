#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int pot = A1;
int potValue;
int plataformaX;
int plataformaY;
int plataformaWidth = 30;
int plataformaHeight = 3;
int plataformaSpeed = 2;

int pelotaX;
int pelotaY;
int pelotaSpeedX = 1;
int pelotaSpeedY = -1;
int pelotaSize = 1;

const int botonPin = 2; // Botón conectado al pin D2

bool juegoActivo = true;

unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 15; // Intervalo de actualización en milisegundos

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  pinMode(pot, INPUT);

  // Configura el pin del botón como entrada con pull-up interno
  pinMode(botonPin, INPUT_PULLUP);

  // Inicializa la posición inicial de la plataforma
  plataformaX = (SCREEN_WIDTH - plataformaWidth) / 2;
  plataformaY = SCREEN_HEIGHT - 3;  // Puedes ajustar la posición inicial

  // Inicializa la posición inicial de la pelota
  pelotaX = SCREEN_WIDTH / 2;
  pelotaY = SCREEN_HEIGHT / 2 - 15; // Puedes ajustar la posición inicial de la pelota
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
      potValue = analogRead(pot);
      plataformaX = map(potValue, 0, 1023, 0, SCREEN_WIDTH - plataformaWidth);

      // Mueve la pelota
      pelotaX += pelotaSpeedX;
      pelotaY += pelotaSpeedY;

      // Colisión con la plataforma
      if (pelotaY + pelotaSize >= plataformaY && pelotaX >= plataformaX && pelotaX <= plataformaX + plataformaWidth) {
        pelotaSpeedY = -pelotaSpeedY;
        puntos++;
      }

      // Colisión con los límites laterales
      if (pelotaX + pelotaSize >= SCREEN_WIDTH || pelotaX <= 0) {
        pelotaSpeedX = -pelotaSpeedX;
      }

      // Colisión con el límite superior
      if (pelotaY <= 10) {
        pelotaSpeedY = -pelotaSpeedY;
      }

      // Si la pelota se va por debajo de la pantalla, resta una vida y reinicia la posición de la pelota
      if (pelotaY >= SCREEN_HEIGHT) {
        vidas--;
        pelotaX = SCREEN_WIDTH / 2;
        pelotaY = SCREEN_HEIGHT / 2 - 15;
      }

      // Si se pierden todas las vidas, muestra "GAME OVER" y desactiva el juego
      if (vidas <= 0) {
        display.setTextSize(2);
        display.setCursor(10, 25);
        display.print("GAME OVER");
        juegoActivo = false;
      } else {
        // Si se alcanzan 10 puntos, sube de nivel y aumenta la velocidad de la pelota
        if (puntos >= 10) {
          nivel++;
          puntos = 0;
          pelotaSpeedX++; // Aumenta la velocidad en el eje X
          pelotaSpeedY--; // Aumenta la velocidad en el eje Y
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
      pelotaSpeedX = 1;
      pelotaSpeedY = -1;
      pelotaX = SCREEN_WIDTH / 2;
      pelotaY = SCREEN_HEIGHT / 2 - 15;
    }
    
    display.display();
  }
}
// TODO: Eliminar la colisión de rebote de la pelota en la plataforma inferiors