/*
 * Arduino Security System Workshop
 * Equivalent to security_camera_STUDENT.py for Raspberry Pi
 * 
 * This example combines ultrasonic distance sensor and PIR motion sensor
 * to create a simple security system with different alert zones.
 * 
 * Note: This is a simplified version since Arduino cannot easily handle
 * camera processing and TensorFlow like Raspberry Pi can.
 */

// Pin definitions
const int TRIG_PIN = 9;        // Ultrasonic trigger pin
const int ECHO_PIN = 10;       // Ultrasonic echo pin
const int PIR_PIN = 2;         // PIR motion sensor pin
const int BUZZER_PIN = 8;      // Buzzer for alerts
const int RED_LED_PIN = 5;     // Red LED for danger zone
const int YELLOW_LED_PIN = 6;  // Yellow LED for warning zone
const int GREEN_LED_PIN = 7;   // Green LED for safe zone

// ################ STUDENT CONFIGURATION SECTION ################
// TODO: Set these values to customize your security system!

// Distance thresholds (in cm)
int DISTANCE_THRESHOLD = 100;  // Try starting with 100!
int DANGER_ZONE = 50;          // Red alert zone
int WARNING_ZONE = 100;        // Yellow warning zone  
int SAFE_ZONE = 200;           // Green monitoring zone

// Alert settings
int CHECK_INTERVAL = 100;      // How often to check (milliseconds)
bool MOTION_REQUIRED = true;   // Require both motion AND distance for alert

// ################ STUDENT FUNCTIONS TO MODIFY ################

String createSecurityMessage(float distance) {
  /*
   * TODO: Create your own security alert message!
   * 
   * You can use these variables in your message:
   * - distance: How far away the object is (in cm)
   * 
   * Examples:
   * return "ALERT! Object detected " + String(distance) + " cm away!";
   * return "INTRUDER ALERT! INTRUDER ALERT!";
   */
  
  // Delete this line and write your message here!
  return "Security alert triggered!";
}

String checkSecurityZone(float distance) {
  /*
   * TODO: Create different alerts for different distances!
   * 
   * Example:
   * if (distance < 50) {
   *   return "RED ALERT: Too Close!";
   * } else if (distance < 100) {
   *   return "YELLOW ALERT: Warning Zone!";
   * } else {
   *   return "GREEN ZONE: All Clear!";
   * }
   */
  
  // Delete 'return' and write your distance checks here!
  return "Zone check not implemented";
}

// ################ SETUP FUNCTIONS (Students don't need to modify these) ################

// Variables
float distance;
bool motionDetected = false;
bool objectInRange = false;
int detectionCount = 0;
unsigned long lastDetectionTime = 0;
unsigned long lastCheckTime = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set pin modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  
  // Turn off all LEDs and buzzer initially
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
  Serial.println("Arduino Security System Workshop Started!");
  Serial.println("===========================================");
  Serial.println("Waiting for PIR sensor to stabilize...");
  
  // Flash green LED during initialization
  for (int i = 0; i < 10; i++) {
    digitalWrite(GREEN_LED_PIN, HIGH);
    delay(500);
    digitalWrite(GREEN_LED_PIN, LOW);
    delay(500);
  }
  
  Serial.println("Security system ready!");
  Serial.println("Configure your settings in the STUDENT CONFIGURATION SECTION");
  Serial.println();
}

void loop() {
  // Check if it's time for the next security check
  if (millis() - lastCheckTime >= CHECK_INTERVAL) {
    lastCheckTime = millis();
    
    // TODO #1: Get the distance from the ultrasonic sensor
    distance = measureDistance();
    
    // TODO #2: Read motion sensor
    motionDetected = digitalRead(PIR_PIN);
    
    // TODO #3: Check if something is closer than DISTANCE_THRESHOLD
    objectInRange = (distance > 0 && distance <= DISTANCE_THRESHOLD);
    
    // TODO #4: Check if motion was detected (if required)
    bool alertCondition;
    if (MOTION_REQUIRED) {
      alertCondition = objectInRange && motionDetected;
    } else {
      alertCondition = objectInRange;
    }
    
    // TODO #5: If conditions are met, create an alert!
    if (alertCondition) {
      detectionCount++;
      lastDetectionTime = millis();
      
      // Create security message
      String message = createSecurityMessage(distance);
      String zoneStatus = checkSecurityZone(distance);
      
      // Print alerts
      Serial.println(">>> SECURITY ALERT! <<<");
      Serial.println(message);
      Serial.println(zoneStatus);
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
      Serial.print("Motion: ");
      Serial.println(motionDetected ? "YES" : "NO");
      Serial.print("Detection count: ");
      Serial.println(detectionCount);
      Serial.println();
      
      // Handle visual and audio alerts
      handleAlerts(distance);
    } else {
      // No alert - show normal status
      handleNormalStatus(distance);
    }
    
    // Optional: Print status every 5 seconds when no alerts
    static unsigned long lastStatusTime = 0;
    if (millis() - lastStatusTime > 5000 && !alertCondition) {
      lastStatusTime = millis();
      Serial.print("Status: All clear - Distance: ");
      Serial.print(distance);
      Serial.print(" cm, Motion: ");
      Serial.println(motionDetected ? "YES" : "NO");
    }
  }
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
  if (duration == 0) {
    return -1; // No echo received
  }
  
  float distance = (duration * 0.0343) / 2;
  return distance;
}

void handleAlerts(float distance) {
  // Determine alert level based on distance
  if (distance < DANGER_ZONE) {
    // RED ALERT - Danger zone
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
    
    // Sound buzzer with fast beeping
    tone(BUZZER_PIN, 1000, 200);
    
  } else if (distance < WARNING_ZONE) {
    // YELLOW ALERT - Warning zone
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
    
    // Sound buzzer with medium beeping
    tone(BUZZER_PIN, 800, 100);
    
  } else if (distance < SAFE_ZONE) {
    // GREEN ALERT - Monitoring zone
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
    
    // No buzzer, just LED
  }
}

void handleNormalStatus(float distance) {
  // Turn off alert LEDs
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  
  // Gentle green LED breathing effect to show system is active
  static unsigned long breatheTimer = 0;
  static bool breatheUp = true;
  static int brightness = 0;
  
  if (millis() - breatheTimer > 20) {
    breatheTimer = millis();
    
    if (breatheUp) {
      brightness += 5;
      if (brightness >= 255) {
        brightness = 255;
        breatheUp = false;
      }
    } else {
      brightness -= 5;
      if (brightness <= 0) {
        brightness = 0;
        breatheUp = true;
      }
    }
    
    analogWrite(GREEN_LED_PIN, brightness);
  }
  
  // Turn off buzzer
  noTone(BUZZER_PIN);
}

/*
 * STUDENT EXERCISES:
 * 
 * 1. Implement the createSecurityMessage() function with custom messages
 * 2. Implement the checkSecurityZone() function with different zone alerts
 * 3. Adjust the distance thresholds for your environment
 * 4. Try different alert patterns (flashing LEDs, different buzzer tones)
 * 5. Add a "disarm" button to temporarily disable the system
 * 6. Add an LCD display to show current status and readings
 * 7. Log security events to EEPROM memory
 * 
 * WIRING:
 * HC-SR04 Ultrasonic Sensor:
 * - VCC -> 5V
 * - GND -> GND
 * - Trig -> Pin 9
 * - Echo -> Pin 10
 * 
 * PIR Motion Sensor (HC-SR501):
 * - VCC -> 5V
 * - GND -> GND
 * - OUT -> Pin 2
 * 
 * LEDs (with 220Ω resistors):
 * - Red LED -> Pin 5
 * - Yellow LED -> Pin 6  
 * - Green LED -> Pin 7
 * 
 * Buzzer:
 * - Positive -> Pin 8
 * - Negative -> GND
 */