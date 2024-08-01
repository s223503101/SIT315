// Define the pin for the motion sensor and LED
const int sensorPin = 2; // Interrupt pin
const int ledPin = 13;

// Variable to track LED state
volatile bool ledState = false;

// Interrupt Service Routine
void handleInterrupt() {
  ledState = !ledState; // Toggle LED state
}

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  // Attach the interrupt
  attachInterrupt(digitalPinToInterrupt(sensorPin), handleInterrupt, RISING);
  
  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  // Update LED based on the state
  digitalWrite(ledPin, ledState);
  
  // Print sensor state
  Serial.println(digitalRead(sensorPin));
}
