/*
 * Arduino Ultrasonic Sensor Workshop
 * Equivalent to ultrasonic.py for Raspberry Pi
 * 
 * This example shows how to use an ultrasonic sensor (HC-SR04)
 * to detect objects and measure distance.
 */

// Pin definitions
const int TRIG_PIN = 9;    // Trigger pin
const int ECHO_PIN = 10;   // Echo pin

// Distance thresholds (in cm)
const int IN_RANGE_DISTANCE = 50;   // Object detected within this distance

// Variables
float distance;
bool objectInRange = false;
bool previousInRange = false;

// Messages - customize these!
String inRangeMessage = "Object detected in range!";
String outRangeMessage = "Object out of range - all clear!";

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set pin modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  Serial.println("Arduino Ultrasonic Sensor Workshop Started!");
  Serial.println("Place objects in front of the sensor to test detection.");
  Serial.println("Customize the messages above to personalize your alerts!");
  Serial.println();
}

void loop() {
  // Measure distance
  distance = measureDistance();
  
  // Check if object is in range
  objectInRange = (distance > 0 && distance <= IN_RANGE_DISTANCE);
  
  // Detect state changes (like wait_for_in_range and wait_for_out_of_range)
  if (objectInRange && !previousInRange) {
    // Object just entered range
    Serial.print(">>> ");
    Serial.println(inRangeMessage);
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  else if (!objectInRange && previousInRange) {
    // Object just left range
    Serial.print(">>> ");
    Serial.println(outRangeMessage);
  }
  
  // Update previous state
  previousInRange = objectInRange;
  
  // Small delay to avoid spam
  delay(100);
}

float measureDistance() {
  // Clear the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Send trigger pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the echo pin
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  
  // Calculate distance in cm
  // Speed of sound = 343 m/s = 0.0343 cm/microsecond
  // Distance = (duration / 2) * 0.0343
  if (duration == 0) {
    return -1; // No echo received (out of range)
  }
  
  float distance = (duration * 0.0343) / 2;
  return distance;
}

/*
 * STUDENT EXERCISES:
 * 
 * 1. Change the IN_RANGE_DISTANCE to different values and test
 * 2. Customize the inRangeMessage and outRangeMessage
 * 3. Add different distance zones (close, medium, far)
 * 4. Add an LED that lights up when object is detected
 * 5. Add a buzzer for audio alerts
 * 
 * WIRING:
 * HC-SR04 Ultrasonic Sensor:
 * - VCC -> 5V
 * - GND -> GND  
 * - Trig -> Pin 9
 * - Echo -> Pin 10
 */