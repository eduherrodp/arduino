int led_1 = 3;
int led_2 = 4;
int led_3 = 5;
int led_4 = 6;
int led_5 = 7;
int leds[5] = {led_1, led_2, led_3, led_4, led_5};

int boton = 2;
bool estado_pulsador;
int diferencia = 0;
long tiempo_anterior;
long tiempo_actual;

void setup() {
  Serial.begin(9600);
  pinMode(boton, INPUT_PULLUP);
  
  for(int i = 0; i < 5; i++) { 
    pinMode(leds[i], OUTPUT);
  }
  attachInterrupt(digitalPinToInterrupt(boton), consulta_estado, RISING);
}

void loop() {
  for(int i = 0; i < 5; i++) { 
    int led_encender = leds[i];
    consulta_estado();
    delay(200);
    digitalWrite(led_encender, LOW);
  }
}

void encender(int led_encender) {
  
}

void consulta_estado() {
  diferencia = millis() - tiempo_anterior;
  tiempo_anterior = millis();
  Serial.println(diferencia);

  if (diferencia > 500) {
    for(int i = 0; i < 5; i++) { 
      digitalWrite(leds[i], HIGH);
    }
    Serial.println(" << ENCENDIDO >> ");
  }
}
