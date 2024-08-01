// Define pins for the sensors and LED
const int motionSensorPin = 2; // Interrupt pin for motion sensor
const int piezoSensorPin = 3; // Interrupt pin for piezo sensor
const int ledPin = 13; // Built-in LED

// Variables to track LED states
volatile bool motionLedState = false;
volatile bool piezoLedState = false;

// ISR for motion sensor
void handleMotionInterrupt() {
  motionLedState = !motionLedState; // Toggle motion LED state
  digitalWrite(ledPin, motionLedState); // Update the LED
}

// ISR for piezo sensor
void handlePiezoInterrupt() {
  piezoLedState = !piezoLedState; // Toggle piezo LED state
  digitalWrite(ledPin, piezoLedState); // Update the LED
}

void setup() {
  pinMode(motionSensorPin, INPUT);
  pinMode(piezoSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(motionSensorPin), handleMotionInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(piezoSensorPin), handlePiezoInterrupt, RISING);
  
  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  // Print sensor states to the Serial Monitor
  Serial.print("Motion Sensor: ");
  Serial.print(digitalRead(motionSensorPin));
  Serial.print(" | Piezo Sensor: ");
  Serial.println(digitalRead(piezoSensorPin));
  
  // Small delay to avoid flooding the Serial Monitor
  delay(500);
}
