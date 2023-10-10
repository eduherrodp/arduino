const byte motorA = 3; 
const byte motorB = 4; 
const byte enable = 10;

byte velocidadInicial = 40;
void setup() {
  Serial.begin(9600);
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);

  analogWrite(enable, velocidadInicial);
}

void loop() {
  control(true,velocidadInicial);
  velocidadInicial++;
  delay(200);
  Serial.println(velocidadInicial);
}

void control(bool sentido, byte vel) {
  if(sentido == true) {
    digitalWrite(motorA, HIGH);
    digitalWrite(motorB, LOW);
  } else {
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, HIGH);
  }
  analogWrite(enable, vel);
}