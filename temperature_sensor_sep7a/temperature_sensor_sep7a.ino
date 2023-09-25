int lecturaADC = 0;
double voltajeLM35 = 0.0;
double TemperaturaLM35 = 0.0;

void setup() {
  pinMode(12,OUTPUT); //Salida a Alarma, aun sin uso
  Serial.begin(9600);
}

void loop() {
  /*
   * #1
   Lectura del canal 0 del ADC,
   recordando que el ADC de Arduino
   es de 10 Bits, el resultado se guardara
   en una variable int (16 bits).
      
   */
  lecturaADC = analogRead(A0); 
  

  /*
   * Se imprime el valor despues del siguiente texto.
   */
  Serial.print("El valor binario de salida en el sensor es: ");
  Serial.println(lecturaADC);
  

  /*
   * Convertir el valor binario a un voltaje que se guardara
   * en una variable del programa, dado que la variable lecturaADC
   * es entera, la operación lecturaADC/1023 regresará un valor entero.
   * La opción (a) funciona de esta forma, lo cual sería un error,
   * esto porque el resultado que imprimiria seria 0.0.
   * Para evitar este problema, se hace un "casting" a la variable entera.
   * Esta casting hará que el compilador considere a la operación,
   * como si fuera una variable double. La opción (b) considera este caso.
   */

    //Opcion (a)
      //  voltajeLM35 = (lecturaADC/1023)*5;
    //Opcion (b)
   voltajeLM35 = ((double)lecturaADC/1023)*5;
   Serial.print("El valor en voltaje de salida en el sensor es: ");
   Serial.println(voltajeLM35,4); //Imprime la variable double con 4 decimales


   TemperaturaLM35 = voltajeLM35/0.01;
   Serial.print("El valor en temperatura de salida en el sensor es: ");
   Serial.print(TemperaturaLM35,2); //Imprime la variable double con 2 decimales
   Serial.println(" °C");
  
  /*
   * Retardo de 3 segundos entre medicion.
   */
  delay(3000); 
   

}