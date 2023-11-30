// Pin configuration for each display
const int display1Cathode = 2;
const int display2Cathode = 9;
const int display3Cathode = 10;
const int display4Cathode = 13;

// Shared pins for display segments
const int pinA = 12;
const int pinB = 8;
const int pinC = 4;
const int pinD = 6;
const int pinE = 7;
const int pinF = 11;
const int pinG = 3;

// Button pin
const int buttonPin = A0;

// Time synchronization between display change and time since the last increment
const int _msBetweenDisplayChange = 3;
int _timeSinceLastIncrement = 0;

// Numbers to be displayed on the displays
int _thousands = 0, _hundreds = 0, _tens = 0, _units = 0;
int _maxThousands = 9, _maxHundreds = 9, _maxTens = 9, _maxUnits = 9;

void setup() {
  Serial.begin(9600);
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinF, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(display1Cathode, OUTPUT);
  pinMode(display2Cathode, OUTPUT);
  pinMode(display3Cathode, OUTPUT);
  pinMode(display4Cathode, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  IncrementByButton();

  VerifyThousandsHundredsTensAndUnits();
  Serial.println(String(_thousands) + "," + String(_hundreds) + "," + String(_tens) + "," + String(_units));
  PrintInDisplays(_thousands, _hundreds, _tens, _units);
}

// Increment units when the button is pressed
void IncrementByButton() {
  if (_timeSinceLastIncrement > 100) {
    int buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      _timeSinceLastIncrement = 0;
      _units++;
    }
  } else {
    _timeSinceLastIncrement += _msBetweenDisplayChange * 2;
  }
}

// Check and reset thousands, hundreds, tens, and units
void VerifyThousandsHundredsTensAndUnits() {
  if (_units > _maxUnits) {
    _tens++;
    _units = 0;
  }
  if (_tens > _maxTens) {
    _hundreds++;
    _tens = 0;
  }
  if (_hundreds > _maxHundreds) {
    _thousands++;
    _hundreds = 0;
  }
  if (_thousands > _maxThousands) {
    _thousands = 0;
  }
}

// Display numbers on each segment
void PrintInDisplays(int thousands, int hundreds, int tens, int units) {
  PrintNumberInDisplay(thousands, 4);
  delay(_msBetweenDisplayChange);
  PrintNumberInDisplay(hundreds, 3);
  delay(_msBetweenDisplayChange);
  PrintNumberInDisplay(tens, 2);
  delay(_msBetweenDisplayChange);
  PrintNumberInDisplay(units, 1);
  delay(_msBetweenDisplayChange);
}

// Display a number on a specific display
void PrintNumberInDisplay(int number, int displayNumber) {
  // Determine which display to turn on
  switch (displayNumber) {
    case 1:
      digitalWrite(display1Cathode, LOW);
      digitalWrite(display2Cathode, HIGH);
      digitalWrite(display3Cathode, HIGH);
      digitalWrite(display4Cathode, HIGH);
      break;
    case 2:
      digitalWrite(display1Cathode, HIGH);
      digitalWrite(display2Cathode, LOW);
      digitalWrite(display3Cathode, HIGH);
      digitalWrite(display4Cathode, HIGH);
      break;
    case 3:
      digitalWrite(display1Cathode, HIGH);
      digitalWrite(display2Cathode, HIGH);
      digitalWrite(display3Cathode, LOW);
      digitalWrite(display4Cathode, HIGH);
      break;
    case 4:
      digitalWrite(display1Cathode, HIGH);
      digitalWrite(display2Cathode, HIGH);
      digitalWrite(display3Cathode, HIGH);
      digitalWrite(display4Cathode, LOW);
      break;
  }

  // Draw the number on the display
  switch (number) {
    case 0:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, LOW);
      break;
    case 1:
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, LOW);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, LOW);
      digitalWrite(pinG, LOW);
      break;
    case 2:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, LOW);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinF, LOW);
      digitalWrite(pinG, HIGH);
      break;
    case 3:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, LOW);
      digitalWrite(pinG, HIGH);
      break;
    case 4:
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, LOW);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      break;
    case 5:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, LOW);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      break;
    case 6:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, LOW);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      break;
    case 7:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, LOW);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, LOW);
      digitalWrite(pinG, LOW);
      break;
    case 8:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      break;
    case 9:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      break;
  }
}