/*
 * Arduino Security System Workshop - COMPLETED VERSION
 * Equivalent to security_camera_GTLP.py for Raspberry Pi
 * 
 * This is the completed version with all functions implemented.
 * Use this as a reference for the student version.
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

// Distance thresholds (in cm)
int DISTANCE_THRESHOLD = 100;  // Alert trigger distance
int DANGER_ZONE = 50;          // Red alert zone
int WARNING_ZONE = 100;        // Yellow warning zone  
int SAFE_ZONE = 200;           // Green monitoring zone

// Alert settings
int CHECK_INTERVAL = 100;      // How often to check (milliseconds)
bool MOTION_REQUIRED = true;   // Require both motion AND distance for alert

// Optional variables for different distances (like Python version)
const int CLOSE_RANGE = 30;
const int MEDIUM_RANGE = 100;
const int FAR_RANGE = 200;

// ################ STUDENT FUNCTIONS - COMPLETED ################

String createSecurityMessage(float distance) {
  /*
   * Create custom security alert messages based on distance
   */
  String messages[] = {
    "🚨 CRITICAL: Intruder at " + String(distance, 1) + " cm!",
    "⚠️ WARNING: Motion detected at " + String(distance, 1) + " cm!",
    "📱 ALERT: Object approaching - " + String(distance, 1) + " cm away!",
    "🔒 SECURITY: Perimeter breach detected!",
    "🚪 ENTRANCE: Someone at the door - " + String(distance, 1) + " cm!"
  };
  
  // Return random message (like Python version with random module)
  int messageIndex = random(0, 5);
  return messages[messageIndex];
}

String checkSecurityZone(float distance) {
  /*
   * Create different alerts for different distances
   * Based on the GTLP Python version
   */
  if (distance < DANGER_ZONE) {
    return "🔴 RED ALERT: Too Close!";
  } else if (distance < WARNING_ZONE) {
    return "🟡 YELLOW ALERT: Warning Zone!";
  } else if (distance < SAFE_ZONE) {
    return "🟢 GREEN ZONE: Monitoring...";
  } else {
    return "✅ All Clear";
  }
}

String getDetailedStatus(float distance, bool motion) {
  /*
   * Additional function to provide detailed system status
   */
  String status = "System Status: ";
  
  if (motion && distance < DISTANCE_THRESHOLD) {
    status += "ACTIVE THREAT";
  } else if (motion) {
    status += "MOTION DETECTED (Out of range)";
  } else if (distance < DISTANCE_THRESHOLD) {
    status += "OBJECT DETECTED (No motion)";
  } else {
    status += "ALL CLEAR";
  }
  
  return status;
}

// ################ SETUP FUNCTIONS ################

// Variables
float distance;
bool motionDetected = false;
bool objectInRange = false;
int detectionCount = 0;
unsigned long lastDetectionTime = 0;
unsigned long lastCheckTime = 0;

void setup() {
  // Initialize random seed
  randomSeed(analogRead(0));
  
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
  
  Serial.println("Arduino Security System - COMPLETED VERSION");
  Serial.println("===========================================");
  Serial.println("Initializing security system...");
  
  // Startup LED sequence
  startupSequence();
  
  Serial.println("Waiting for PIR sensor to stabilize...");
  delay(5000); // Shorter delay for demo
  
  Serial.println("🔒 Security system armed and ready!");
  Serial.println("📊 Current configuration:");
  Serial.println("   Distance threshold: " + String(DISTANCE_THRESHOLD) + " cm");
  Serial.println("   Danger zone: " + String(DANGER_ZONE) + " cm");
  Serial.println("   Warning zone: " + String(WARNING_ZONE) + " cm");
  Serial.println("   Safe zone: " + String(SAFE_ZONE) + " cm");
  Serial.println("   Motion required: " + String(MOTION_REQUIRED ? "YES" : "NO"));
  Serial.println();
}

void loop() {
  // Check if it's time for the next security check
  if (millis() - lastCheckTime >= CHECK_INTERVAL) {
    lastCheckTime = millis();
    
    // Get the distance from the ultrasonic sensor
    distance = measureDistance();
    
    // Read motion sensor
    motionDetected = digitalRead(PIR_PIN);
    
    // Check if something is closer than DISTANCE_THRESHOLD
    objectInRange = (distance > 0 && distance <= DISTANCE_THRESHOLD);
    
    // Check if motion was detected (if required)
    bool alertCondition;
    if (MOTION_REQUIRED) {
      alertCondition = objectInRange && motionDetected;
    } else {
      alertCondition = objectInRange;
    }
    
    // If conditions are met, create an alert!
    if (alertCondition) {
      detectionCount++;
      lastDetectionTime = millis();
      
      // Create security messages
      String securityMessage = createSecurityMessage(distance);
      String zoneStatus = checkSecurityZone(distance);
      String detailedStatus = getDetailedStatus(distance, motionDetected);
      
      // Print comprehensive alert
      Serial.println(">>> 🚨 SECURITY ALERT! <<<");
      Serial.println(securityMessage);
      Serial.println(zoneStatus);
      Serial.println(detailedStatus);
      Serial.println("📏 Distance: " + String(distance, 1) + " cm");
      Serial.println("👋 Motion: " + String(motionDetected ? "DETECTED" : "NOT DETECTED"));
      Serial.println("📊 Alert #" + String(detectionCount));
      Serial.println("⏰ Time: " + String(millis() / 1000) + "s");
      Serial.println("----------------------------------------");
      
      // Handle visual and audio alerts
      handleAlerts(distance);
      
      // Brief delay to avoid spam
      delay(500);
    } else {
      // No alert - show normal status
      handleNormalStatus(distance);
    }
    
    // Print status every 10 seconds when no alerts
    static unsigned long lastStatusTime = 0;
    if (millis() - lastStatusTime > 10000 && !alertCondition) {
      lastStatusTime = millis();
      Serial.println("📊 Status: " + getDetailedStatus(distance, motionDetected) + 
                    " | Distance: " + String(distance, 1) + " cm | " +
                    "Alerts: " + String(detectionCount));
    }
  }
}

void startupSequence() {
  // LED startup sequence
  for (int i = 0; i < 3; i++) {
    digitalWrite(RED_LED_PIN, HIGH);
    delay(200);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, HIGH);
    delay(200);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
    delay(200);
    digitalWrite(GREEN_LED_PIN, LOW);
  }
  
  // All LEDs on briefly
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(YELLOW_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, HIGH);
  tone(BUZZER_PIN, 1000, 100);
  delay(500);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
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
    
    // Sound buzzer with fast beeping pattern
    tone(BUZZER_PIN, 1200, 150);
    delay(100);
    noTone(BUZZER_PIN);
    delay(50);
    tone(BUZZER_PIN, 1200, 150);
    
  } else if (distance < WARNING_ZONE) {
    // YELLOW ALERT - Warning zone
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
    
    // Sound buzzer with medium beeping
    tone(BUZZER_PIN, 800, 200);
    
  } else if (distance < SAFE_ZONE) {
    // GREEN ALERT - Monitoring zone
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
    
    // Short beep to acknowledge detection
    tone(BUZZER_PIN, 600, 100);
  }
}

void handleNormalStatus(float distance) {
  // Turn off alert LEDs and buzzer
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  noTone(BUZZER_PIN);
  
  // Gentle green LED breathing effect to show system is active
  static unsigned long breatheTimer = 0;
  static bool breatheUp = true;
  static int brightness = 0;
  
  if (millis() - breatheTimer > 30) {
    breatheTimer = millis();
    
    if (breatheUp) {
      brightness += 3;
      if (brightness >= 100) {
        brightness = 100;
        breatheUp = false;
      }
    } else {
      brightness -= 3;
      if (brightness <= 10) {
        brightness = 10;
        breatheUp = true;
      }
    }
    
    analogWrite(GREEN_LED_PIN, brightness);
  }
}

/*
 * COMPLETED FEATURES:
 * ✅ Distance-based security zones (danger, warning, safe)
 * ✅ Motion detection requirement option
 * ✅ Custom security messages with randomization
 * ✅ Visual LED alerts (red, yellow, green)
 * ✅ Audio buzzer alerts with different patterns
 * ✅ Detection counting and statistics
 * ✅ Comprehensive status reporting
 * ✅ Startup sequence and system status
 * ✅ Breathing LED effect for standby mode
 * 
 * This demonstrates the same educational progression as the Python version
 * but adapted for Arduino's capabilities and constraints.
 */