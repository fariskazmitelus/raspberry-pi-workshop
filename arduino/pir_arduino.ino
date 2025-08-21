/*
 * Arduino PIR Motion Sensor Workshop
 * Equivalent to pir.py for Raspberry Pi
 * 
 * This example shows how to use a PIR (Passive Infrared) motion sensor
 * to detect movement and count detections.
 */

// Pin definitions
const int PIR_PIN = 2;     // PIR sensor pin (use interrupt pin)
const int LED_PIN = 13;    // Built-in LED for visual feedback

// Variables
int movementCount = 0;
bool motionDetected = false;
bool previousMotion = false;
unsigned long lastMotionTime = 0;
const unsigned long RESET_TIMEOUT = 5000; // 5 seconds timeout (like Python version)

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set pin modes
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Turn off LED initially
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("Arduino PIR Motion Sensor Workshop Started!");
  Serial.println("Wave your hand in front of the PIR sensor to test detection.");
  Serial.println("Movement count will be displayed on each detection.");
  Serial.println();
  
  // Wait for PIR sensor to stabilize (usually takes 10-60 seconds)
  Serial.println("Waiting for PIR sensor to stabilize... (30 seconds)");
  delay(30000);
  Serial.println("PIR sensor ready!");
  Serial.println();
}

void loop() {
  // Read PIR sensor
  motionDetected = digitalRead(PIR_PIN);
  
  // Check for motion start (rising edge)
  if (motionDetected && !previousMotion) {
    // Motion just started
    movementCount++;
    lastMotionTime = millis();
    
    // Turn on LED
    digitalWrite(LED_PIN, HIGH);
    
    // Print detection
    Serial.print("Movement Detected! Count: ");
    Serial.println(movementCount);
  }
  
  // Check for motion end or timeout
  if (!motionDetected || (millis() - lastMotionTime > RESET_TIMEOUT)) {
    if (previousMotion || (millis() - lastMotionTime > RESET_TIMEOUT)) {
      // Motion ended or timed out
      digitalWrite(LED_PIN, LOW);
      
      if (millis() - lastMotionTime > RESET_TIMEOUT) {
        Serial.println("Resetting (timeout)");
      } else {
        Serial.println("Resetting (no motion)");
      }
    }
    motionDetected = false;
  }
  
  // Update previous state
  previousMotion = motionDetected;
  
  // Small delay
  delay(100);
}

/*
 * STUDENT EXERCISES:
 * 
 * 1. Change the RESET_TIMEOUT to different values and test
 * 2. Add a buzzer that sounds when motion is detected
 * 3. Add different LEDs for different types of alerts
 * 4. Create a "security mode" that logs motion events with timestamps
 * 5. Combine with the ultrasonic sensor for distance-based motion detection
 * 
 * WIRING:
 * PIR Motion Sensor (HC-SR501):
 * - VCC -> 5V (or 3.3V depending on your sensor)
 * - GND -> GND
 * - OUT -> Pin 2 (digital input)
 * 
 * Built-in LED is on Pin 13 (no additional wiring needed)
 * 
 * NOTES:
 * - PIR sensors need time to stabilize after power-on (10-60 seconds)
 * - Some PIR sensors have sensitivity and time delay adjustments (potentiometers)
 * - PIR sensors detect changes in infrared radiation (heat from moving objects)
 */