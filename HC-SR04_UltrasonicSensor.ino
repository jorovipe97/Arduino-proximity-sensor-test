#include <Volume.h> // Permite modificar el volumen
// Libreria diseñada para leer solo el firstDown de un pulsador
#include <StairButton.h>
#include "musical_notes.h"
/*
  HC-SR04 Ping distance sensor:
  VCC to arduino 5v
  GND to arduino GND
  Echo to Arduino pin 9
  Trig to Arduino pin 8

  This sketch originates from Virtualmix: http://goo.gl/kJ8Gl
  Has been modified by Winkle ink here: http://winkleink.blogspot.com.au/2012/05/arduino-hc-sr04-ultrasonic-distance.html
  And modified further by ScottC here: http://arduinobasics.blogspot.com.au/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html
  on 10 Nov 2012.
  and modified further jose&romualdo.
*/

/*
   Si pongo una esponga metalica frente al ultrasonic sensor el sensor no bajara lo suficiente
*/





#define echoPin 8 // Echo Pin
#define trigPin 7 // Trigger Pin
#define speakerPin 10
#define onOffButton 4
#define ledPins 9

int maximumRange = 50; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long distance; // Duration used to calculate distance
long distanceMean; // Duracion y distancia usando los ultimos 5 medidas y promediando, esto es asi para suavisar el ruido

bool isGameOn = true; // el juego esta prendido?
StairButton powerButton(onOffButton); // Pin del boton
Volume vol;

void setup() {
  Serial.begin (9600);
  //vol.begin(); // After calling this, delay() and delayMicroseconds will no longer work
               // correctly! Instead, use vol.delay() and vol.delayMicroseconds() for
               // the correct timing
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(speakerPin, OUTPUT); // Use LED indicator (if required)
  pinMode(ledPins, OUTPUT);

  //vol.setMasterVolume(1.00); // Self-explanatory enough, right? Try lowering this value if the speaker is too loud! (0.00 - 1.00)
  delay(500);
  Serial.println("Empezo el juego luego de un retraso");
  delay(1000);
  Serial.println("Empezo el juego luego de dos retraso");
  //R2D2();
}
long durationMean(int meanWithLastNthSamples)
{
  long sumDurations = 0;
  for (int i = 0; i <= meanWithLastNthSamples; i++)
  {
    /* The following trigPin/echoPin cycle is used to determine the
      distance of the nearest object by bouncing soundwaves off of it. */
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    Serial.println("Pidiendo señal");

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); // Se necesita enviar una señal digital de 10us al ultrasonic sensor para que vuelva medir

    digitalWrite(trigPin, LOW);
    sumDurations += pulseIn(echoPin, HIGH);
  }
  return sumDurations / meanWithLastNthSamples;
}

long durationLastSample()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Se necesita enviar una señal digital de 10us al ultrasonic sensor para que vuelva medir

  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH);
}

void loop() {

  // vigila el boton de encendido del juego
  powerButton.updateToggleLogic(&isGameOn);
  // si el juego esta apagado?
  if (!isGameOn)
  {
    Serial.println("juego apagado");    
    analogWrite(ledPins, 0);
    return;
  }

  //distance = durationLastSample() / 58.2;
  distanceMean = durationMean(5) / 58.2;
  // Se pone un constrain a distanceMean para que el valor no se salga del rango esperado
  int ledIntensity = map(constrain(distanceMean, 10, 100), 0, 50, 254, 0);
  

  
  if (distanceMean <= maximumRange && distanceMean > minimumRange) {
    /* Send a negative number to computer and Turn LED ON
      to indicate "out of range" */
    analogWrite(ledPins, ledIntensity);
    r2D2(); 
    delay(1000);
  }
  else {
    /* Send the distance to the computer using Serial protocol, and
      turn LED OFF to indicate successful reading. */
    analogWrite(ledPins, 0);
    scale();
    delay(1000);    
  }

  //Delay 50ms before next reading.
  delay(50);
}


// --------------------------------
// ||||||||||||SOUNDS |||||||||||||
// --------------------------------
void beep (int speakrPin, float noteFrequency, long noteDuration)
{
  int x;
  // Convert the frequency to microseconds
  float microsecondsPerWave = 1000000 / noteFrequency;
  // Calculate how many HIGH/LOW cycles there are per millisecond
  float millisecondsPerCycle = 1000 / (microsecondsPerWave * 2);
  // Multiply noteDuration * number or cycles per millisecond
  float loopTime = noteDuration * millisecondsPerCycle;
  // Play the note for the calculated loopTime.
  for (x = 0; x < loopTime; x++)
  {
    digitalWrite(speakrPin, HIGH);
    delayMicroseconds(microsecondsPerWave);
    digitalWrite(speakrPin, LOW);
    delayMicroseconds(microsecondsPerWave);
  }
}

void scale()
{
  beep(speakerPin, note_C7, 500); //C: play the note C for 500ms
  beep(speakerPin, note_D7, 500); //D
  beep(speakerPin, note_E7, 500); //E
  beep(speakerPin, note_F7, 500); //F
  beep(speakerPin, note_G7, 500); //G
  beep(speakerPin, note_A7, 500); //A
  beep(speakerPin, note_B7, 500); //B
  beep(speakerPin, note_C8, 500); //C
}

void r2D2() {
  beep(speakerPin, note_A7, 100); //A
  beep(speakerPin, note_G7, 100); //G
  beep(speakerPin, note_E7, 100); //E
  beep(speakerPin, note_C7, 100); //C
  beep(speakerPin, note_D7, 100); //D
  beep(speakerPin, note_B7, 100); //B
  beep(speakerPin, note_F7, 100); //F
  beep(speakerPin, note_C8, 100); //C
  beep(speakerPin, note_A7, 100); //A
  beep(speakerPin, note_G7, 100); //G
  beep(speakerPin, note_E7, 100); //E
  beep(speakerPin, note_C7, 100); //C
  beep(speakerPin, note_D7, 100); //D
  beep(speakerPin, note_B7, 100); //B
  beep(speakerPin, note_F7, 100); //F
  beep(speakerPin, note_C8, 100); //C
}

void closeEncounters() {
  beep(speakerPin, note_Bb5, 300); //B b
  delay(50);
  beep(speakerPin, note_C6, 300); //C
  delay(50);
  beep(speakerPin, note_Ab5, 300); //A b
  delay(50);
  beep(speakerPin, note_Ab4, 300); //A b
  delay(50);
  beep(speakerPin, note_Eb5, 500); //E b
  delay(500);

  beep(speakerPin, note_Bb4, 300); //B b
  delay(100);
  beep(speakerPin, note_C5, 300); //C
  delay(100);
  beep(speakerPin, note_Ab4, 300); //A b
  delay(100);
  beep(speakerPin, note_Ab3, 300); //A b
  delay(100);
  beep(speakerPin, note_Eb4, 500); //E b
  delay(500);

  beep(speakerPin, note_Bb3, 300); //B b
  delay(200);
  beep(speakerPin, note_C4, 300); //C
  delay(200);
  beep(speakerPin, note_Ab3, 300); //A b
  delay(500);
  beep(speakerPin, note_Ab2, 300); //A b
  delay(550);
  beep(speakerPin, note_Eb3, 500); //E b
}
