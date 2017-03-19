/*
  Solder Fume Extractor - auto-activation, triggered by removing the soldering iron from it's holder

 */

#include <Adafruit_DotStar.h>

#define NUMPIXELS 42 // Number of LEDs in strip

Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int16_t sensorValue = 0;  // variable to store the value coming from the sensor
int coldCount = 0;
int count = 0;

unsigned long startOfHotSpell = 0;
unsigned long timeOfLastChange = 0;

boolean fanCurrentlyRunning = false;

const int Setter = 6;
const int Unsetter = 5;

const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

uint32_t color = 0x111111;


void setup() {
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  pinMode(Setter, OUTPUT);
  pinMode(Unsetter, OUTPUT);

  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);


  analogReference(AR_INTERNAL);
  analogReadResolution(12);

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP

  Serial.println("Well, setup completed...");

}

void loop() {
  Serial.print("About to read... ");
  Serial.println(count);
  sensorValue = analogRead(A0);
  Serial.print("A0=");
  Serial.println(sensorValue);

  readings[readIndex] = sensorValue;
  
  if (sensorValue >= 5) {
    if (coldCount == 0) {
      startOfHotSpell = millis();
    }
    
    coldCount = 0;
  } else {
    // cold
    coldCount++;
    
  }

  long lengthOfHotSpell = millis() - startOfHotSpell;

  Serial.print("coldCount=");
  Serial.println(coldCount);

//  if (coldCount >= 6) {
//    fanOn();
//  } else if (lengthOfHotSpell > 2000) {
//    fanOff();
//  }
  
  delay(500);

  fanOn();
  delay(5000);

  fanOff();
  delay(5000);
  
  count++;
}


void setFanState(boolean running) {
  if (fanCurrentlyRunning != running) {
    if (running) {
      fanOn();
    } else {
      fanOff();
    }
  }
}

void fanOn() {
  Serial.println("fan ON...");
  strip.setPixelColor(0, color); // 'On' pixel at head
  strip.show();                     // Refresh strip
  fan(Setter, HIGH);
}

void fanOff() {
  Serial.println("fan OFF...");
  strip.clear();
  strip.show();                     // Refresh strip
   
  fan(Unsetter, LOW);
}


void fan(int setOrUnset, int ledState) {
  digitalWrite(ledPin, ledState);
  pulse(setOrUnset);
  timeOfLastChange = millis();
}

void pulse(int pin) {
  digitalWrite(pin, HIGH);
  delay(15);
  digitalWrite(pin, LOW);
}
