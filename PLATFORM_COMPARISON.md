# Platform Comparison: Raspberry Pi vs Arduino

This document shows the equivalent implementations between Raspberry Pi (Python) and Arduino (C++) for the workshop projects.

## Code Comparison Examples

### 1. Basic Ultrasonic Sensor Reading

#### Raspberry Pi (Python - using RPi.GPIO)
```python
import RPi.GPIO as GPIO
import time

TRIG = 4
ECHO = 17

GPIO.setmode(GPIO.BCM)
GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)

def measure_distance():
    GPIO.output(TRIG, True)
    time.sleep(0.00001)
    GPIO.output(TRIG, False)
    
    while GPIO.input(ECHO) == 0:
        start_time = time.time()
    
    while GPIO.input(ECHO) == 1:
        stop_time = time.time()
    
    time_elapsed = stop_time - start_time
    distance = (time_elapsed * 34300) / 2
    return distance
```

#### Arduino (C++)
```cpp
const int TRIG_PIN = 9;
const int ECHO_PIN = 10;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  
  if (duration == 0) {
    return -1; // No echo
  }
  
  float distance = (duration * 0.0343) / 2;
  return distance;
}
```

### 2. PIR Motion Detection

#### Raspberry Pi (Python - using gpiozero)
```python
from gpiozero import MotionSensor

pir = MotionSensor(pin=2)
movement_count = 0

while True:
    pir.wait_for_motion()
    print("Movement Detected")
    movement_count += 1
    pir.wait_for_no_motion(timeout=5)
    print("Resetting")
```

#### Arduino (C++)
```cpp
const int PIR_PIN = 2;
int movementCount = 0;
bool motionDetected = false;
bool previousMotion = false;

void setup() {
  pinMode(PIR_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  motionDetected = digitalRead(PIR_PIN);
  
  if (motionDetected && !previousMotion) {
    movementCount++;
    Serial.print("Movement Detected! Count: ");
    Serial.println(movementCount);
  }
  
  if (!motionDetected && previousMotion) {
    Serial.println("Resetting");
  }
  
  previousMotion = motionDetected;
  delay(100);
}
```

## Library Equivalents

| Function | Raspberry Pi | Arduino |
|----------|-------------|---------|
| **GPIO Control** | `GPIO.setup()`, `GPIO.output()` | `pinMode()`, `digitalWrite()` |
| **GPIO Reading** | `GPIO.input()` | `digitalRead()` |
| **Timing** | `time.sleep()`, `time.time()` | `delay()`, `millis()` |
| **Serial Output** | `print()` | `Serial.println()` |
| **PWM** | `GPIO.PWM()` | `analogWrite()` |
| **Interrupts** | GPIO library callbacks | `attachInterrupt()` |

## Feature Comparison

| Feature | Raspberry Pi | Arduino | Notes |
|---------|-------------|---------|-------|
| **Real-time Response** | Limited (Linux OS) | Excellent | Arduino has deterministic timing |
| **Multiple Tasks** | Easy (threading) | Manual (state machines) | Pi can run multiple Python scripts |
| **Internet Connectivity** | Built-in (WiFi/Ethernet) | Add-on modules | Pi has full TCP/IP stack |
| **File System** | Full Linux filesystem | EEPROM only | Pi can log to files easily |
| **Graphics/Display** | Complex displays supported | Simple LCD/LED only | Pi can drive HDMI monitors |
| **Computer Vision** | OpenCV, TensorFlow | Not practical | Pi has processing power for AI |
| **Power Consumption** | Higher (~3W) | Very Low (~0.05W) | Arduino better for battery projects |
| **Development** | Any text editor | Arduino IDE | Pi supports many IDEs |

## When to Choose Which Platform

### Choose Raspberry Pi When:
- You need computer vision or AI capabilities
- You want to process complex data or images
- You need internet connectivity and web interfaces
- You're building a permanent installation with power
- You want to learn full software development

### Choose Arduino When:
- You need precise timing and real-time response
- You're building battery-powered devices
- You want simple, reliable sensor monitoring
- You're learning electronics and embedded programming
- Cost and power consumption are critical

## Project Progression Path

### Learning Path 1: Start with Arduino
1. **Arduino basics** → Learn C++, digital I/O, sensors
2. **Simple projects** → Individual sensors, basic automation
3. **Combined systems** → Multi-sensor security system
4. **Upgrade to Pi** → Add camera, AI, networking

### Learning Path 2: Start with Raspberry Pi
1. **Python basics** → Learn programming, GPIO libraries
2. **Camera projects** → Image processing, object detection
3. **Sensor integration** → Add physical sensors to software
4. **Optimization** → Move time-critical parts to Arduino

### Advanced: Use Both Together
- **Arduino**: Real-time sensor monitoring, motor control
- **Raspberry Pi**: Data processing, decision making, networking
- **Communication**: Serial, I2C, or SPI between platforms

## Educational Outcomes

After completing both platforms, students will understand:
- Differences between microcontrollers and single-board computers
- When to use real-time vs. multitasking systems
- Hardware abstraction layers and GPIO programming
- Sensor interfacing and signal processing
- System design trade-offs (power, cost, performance)

This dual-platform approach provides a comprehensive foundation for embedded systems and IoT development.