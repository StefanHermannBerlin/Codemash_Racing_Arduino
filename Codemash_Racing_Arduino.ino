#include <DualMC33926MotorShield.h>
DualMC33926MotorShield md;

float minHeartrate = 60;                          // pulse sensors highest speed at this heartrate
float maxHeartrate = 140;                         // pulse sensors highest speed at this heartrate
float minSpeed = 200;                             // the minimal speed the cars can drive
float maxSpeed = 400;                             // the maximal speed the cars can drive
float currentMaxSpeed[] = {0, 0};                 // storring the current Max speed information
float car1speed[] = {0, 0};                       // current speed of the cars

// motor pins: 4,7,8,9,10,12,A0,A1                // pins used by motor driver shield
int lightBarrierPins[] = {2, 3};                  // interupts for light barriers
int trottlePins[] = {A4, A5};
int trottle[] = {0, 0};                           // stores the current trottle condition

boolean lightBarrier[] = {false, false};          // light barrier variables

long outputTimer = 0;                             // timer variable for pulse output
long outputTimeout = 20;                          // time to next pulse info

long myTimer1 = 0;                                // timer for light barriers
long myTimer2 = 0;                                // timer for light barriers
//int timeout = 100;
int sensorTimeout = 300;                          // shortest time to break the light barrier again

int pulsePins[] = {A3, A2};                       // the pins where the puls sensors are connected to
int ledPins[] = {5, 6};                           // the pins of the leds
int threshold = 550;                              // threshold to differentiate heartbeat

float theBPMs[] = {66, 66};                       // default value for pulses

long thePulseStopwatch[] = {0, 0};                // helper variable for timeout
int thePulseMinTime = 500;                        // minimal possible time between beats
int thePulseMaxTime = 1500;                       // maximal possible time between beats

void setup() {
  Serial.begin(250000);                                                          // speed to talk to processing
  md.init();                                                                     // initialising of the motor shield
  pinMode(ledPins[0], OUTPUT);                                                   // led output pin
  pinMode(ledPins[1], OUTPUT);                                                   // led output pin

  pinMode(lightBarrierPins[0], INPUT);                                           // interrupt for light barrier
  pinMode(lightBarrierPins[1], INPUT);                                           // interrupt for light barrier
  attachInterrupt(digitalPinToInterrupt(lightBarrierPins[0]), carOn1, HIGH);     // interrupt for light barrier
  attachInterrupt(digitalPinToInterrupt(lightBarrierPins[1]), carOn2, HIGH);     // interrupt for light barrier

}

void loop() {
  updatePulseData();
  // Serial.print(theBPMs[0]); Serial.print(" - "); Serial.println(theBPMs[1]);

  for (int i = 0; i < 2; i++) {
    currentMaxSpeed[i] = minSpeed + (theBPMs[i] / maxHeartrate) * (maxSpeed - minSpeed);                            // calculate current possible max speed (0 to 400)
    trottle[i] = constrain(analogRead(trottlePins[i]), 400, 600);                                                   // read and constrain trottle
    trottle[i] = map(trottle[i], 600, 400, 0, 100);                                                                 // map trottle from 0 to 100
    car1speed[i] = map(trottle[i], 0, 100, 0, currentMaxSpeed[0]);                                                  // speed is map trottle from 0 to currentMaxSpeed
  }

  if (outputTimer + outputTimeout < millis()) {                                                                     // serial output to processing
    Serial.print("P1="); Serial.println(int(theBPMs[0]));                                                           // output bpm
    Serial.print("T1="); Serial.println(trottle[0]);                                                                // output trottle
    Serial.print("S1="); Serial.println(int(car1speed[0]));                                                         // output car speed
    Serial.print("X1="); Serial.println(int(currentMaxSpeed[0]));                                                   // output maximal possible car speed
    Serial.print("P2="); Serial.println(int(theBPMs[1]));                                                           // output bpm
    Serial.print("T2="); Serial.println(trottle[1]);                                                                // output trottle
    Serial.print("S2="); Serial.println(int(car1speed[1]));                                                         // output car speed
    Serial.print("X2="); Serial.println(int(currentMaxSpeed[1]));                                                   // output maximal possible car speed
    Serial.println("");
    outputTimer = millis();                                                                                         // reset output timer
  }
  stopIfFault();
  md.setM1Speed(car1speed[0]*(-1));
  md.setM2Speed(car1speed[1]*(1));


  delay(10);                                                                                                        // time to breathe
}


void carOn1() {                                         // interrupt function for light barriers
  if (myTimer1 + sensorTimeout < millis()) {            // timer
    myTimer1 = millis();                                // reset timer
    Serial.println("C1");                               // serial output
  }
}

void carOn2() {                                         // interrupt function for light barriers
  if (myTimer2 + sensorTimeout < millis()) {            // timer
    myTimer2 = millis();                                // reset timer
    Serial.println("C2");                               // serial output
  }
}

void stopIfFault()
{
  if (md.getFault())
  {
    Serial.println("fault");
    while (1);
  }
}
