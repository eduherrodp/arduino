// #include <Arduino.h>

// #define PinIN 18
// #define PinOUT 2

// void setup() {
//     pinMode(PinOUT, OUTPUT);
//     pinMode(PinIN, INPUT_PULLDOWN);
// }

// void loop() {
//     if(digitalRead(PinIN) == HIGH){
//         digitalWrite(PinOUT, HIGH);
//     } else {
//         digitalWrite(PinOUT, LOW);
//     } 
// }

#include <Arduino.h>

#define PinADC 34
#define Resolucion 3.3/4095
// #define Resolucion 3.3/2047  // Resolución del ADC de 11 bits - 2047 combinaciones
// #define Resolucion 3.3/4095  // Resolución del ADC de 12 bits - 4095 combinaciones
// #define Resolucion 3.3/1023  // Resolución del ADC de 10 bits - 1023 combinaciones
// #define Resolucion 3.3/511   // Resolución del ADC de 9 bits - 511 combinaciones


int rADC = 0;
float voltaje = 0.0;

void setup() {
    Serial.begin(9600);
    
    analogReadResolution(12); // Resolución del ADC de 12 bits - 4095 combinaciones
    analogSetPinAttenuation(PinADC, ADC_11db); // Rango de voltaje de 0 a 3.6V

    /*
     * La atenuación de 0dB (ADC_0db) es el rango de voltaje de 0 a 1.1V
     * La atenuación de 2.5dB (ADC_2_5db) es el rango de voltaje de 0 a 1.5V
     * La atenuación de 6dB (ADC_6db) es el rango de voltaje de 0 a 2.2V
     * La atenuación de 11dB (ADC_11db) es el rango de voltaje de 0 a 3.9V
     * 
     * A una atenuación de 11 dB, el voltaje máximo está limitado por VDDA, no por el voltaje de escala completa
    */
}
void loop() {
  rADC = analogRead(PinADC);
  voltaje = rADC * Resolucion;
  Serial.print("Valor ADC: ");
  Serial.print(rADC);
  Serial.print(" - Voltaje: ");
  Serial.print(voltaje,2);
  Serial.println(" V");
  delay(1000);
}