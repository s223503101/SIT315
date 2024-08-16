// Define pins for the sensors and LED
const int motionSensorPin = 2; // Pin for PIR motion sensor
const int piezoSensorPin = 3; // Pin for piezo sensor
const int ultrasonicTrigPin = 4; // Trig pin for ultrasonic sensor
const int ultrasonicEchoPin = 5; // Echo pin for ultrasonic sensor
const int temperatureSensorPin = A1; // Analog pin for temperature sensor
const int flexSensorPin = A0; // Analog pin for flex sensor
const int ledPin = 13; // Built-in LED

// Variables to track LED states
volatile bool ledState = false;

// ISR for pin change interrupts (handles both motion and piezo sensors)
ISR(PCINT2_vect) {
  if (digitalRead(motionSensorPin) == HIGH || digitalRead(piezoSensorPin) == HIGH) {
    ledState = !ledState; // Toggle LED state
    digitalWrite(ledPin, ledState); // Update the LED
  }
}

// Timer interrupt routine
ISR(TIMER1_COMPA_vect) {
  // Your timer code here (e.g., toggle the LED)
  digitalWrite(ledPin, !digitalRead(ledPin)); // Toggle LED state
}

// Function to get distance from ultrasonic sensor
long getDistance() {
  digitalWrite(ultrasonicTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonicTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicTrigPin, LOW);
  
  long duration = pulseIn(ultrasonicEchoPin, HIGH);
  long distance = (duration * 0.034) / 2;
  return distance;
}

void setup() {
  pinMode(motionSensorPin, INPUT);
  pinMode(piezoSensorPin, INPUT);
  pinMode(ultrasonicTrigPin, OUTPUT);
  pinMode(ultrasonicEchoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  // Configure PCINT for motion sensor and piezo sensor pins
  PCICR |= (1 << PCIE2); // Enable PCINT2 interrupt
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19); // Enable interrupts for pins 2 and 3
  
  // Timer setup
  cli(); // Disable global interrupts
  TCCR1A = 0; // Set entire TCCR1A register to 0
  TCCR1B = 0; // Same for TCCR1B
  TCNT1 = 0; // Initialize counter value to 0
  // Set compare match register for 1Hz increments
  OCR1A = 15624; // = (16*10^6) / (1024*1) - 1 (must be <65536)
  // Turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // Enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei(); // Enable global interrupts
  
  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  // Read temperature sensor data
  int tempValue = analogRead(temperatureSensorPin);
  float temperature = tempValue * (5.0 / 1024.0) * 100.0; // Convert to Celsius
  
  // Read flex sensor data
  int flexValue = analogRead(flexSensorPin);
  
  // Get distance from ultrasonic sensor
  long distance = getDistance();
  
  // Print sensor states to the Serial Monitor
  Serial.print("Motion Sensor: ");
  Serial.print(digitalRead(motionSensorPin));
  Serial.print(" | Piezo Sensor: ");
  Serial.print(digitalRead(piezoSensorPin));
  Serial.print(" | Temperature: ");
  Serial.print(temperature);
  Serial.print(" C | Flex Value: ");
  Serial.print(flexValue);
  Serial.print(" | Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Small delay to avoid flooding the Serial Monitor
  delay(500);
}
