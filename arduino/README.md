# Arduino Workshop Extension

This directory contains Arduino versions of the Raspberry Pi workshop projects. The Arduino implementations focus on sensor interfacing and basic security logic, adapted for the Arduino's capabilities.

## Arduino vs Raspberry Pi

| Feature | Raspberry Pi | Arduino |
|---------|-------------|---------|
| Language | Python | C++ |
| OS | Linux | Bare metal |
| GPIO Library | `gpiozero`, `RPi.GPIO` | `digitalWrite()`, `digitalRead()` |
| Camera | Picamera2 + TensorFlow | Not applicable |
| Display | TFT with complex graphics | Simple LCD/LED indicators |
| Processing Power | High | Limited |
| Real-time | Good | Excellent |

## Projects Included

### 1. `ultrasonic_arduino.ino`
Arduino equivalent of `ultrasonic.py`
- Uses HC-SR04 ultrasonic sensor
- Detects objects entering/leaving range
- Customizable distance thresholds and messages

### 2. `pir_arduino.ino` 
Arduino equivalent of `pir.py`
- Uses PIR motion sensor (HC-SR501)
- Counts motion detection events
- LED feedback and timeout handling

### 3. `security_system_arduino.ino`
Arduino equivalent of `security_camera_STUDENT.py`
- Combines ultrasonic and PIR sensors
- Multi-zone security system (danger/warning/safe)
- Visual (LED) and audio (buzzer) alerts
- Student exercise version with TODO sections

### 4. `security_system_complete.ino`
Arduino equivalent of `security_camera_GTLP.py`
- Completed version with all functions implemented
- Advanced alert patterns and status reporting
- Reference implementation for students

## Hardware Requirements

### Basic Components
- Arduino Uno (or compatible)
- HC-SR04 Ultrasonic Distance Sensor
- PIR Motion Sensor (HC-SR501)
- Breadboard and jumper wires

### For Security System
- 3x LEDs (Red, Yellow, Green) with 220Ω resistors
- Piezo buzzer
- Additional jumper wires

### Optional Enhancements
- LCD display (16x2 I2C)
- Push button for system control
- Potentiometer for threshold adjustment
- Additional sensors (temperature, light, etc.)

## Wiring Diagrams

### HC-SR04 Ultrasonic Sensor
```
HC-SR04    Arduino
VCC    ->  5V
GND    ->  GND
Trig   ->  Pin 9
Echo   ->  Pin 10
```

### PIR Motion Sensor (HC-SR501)
```
PIR        Arduino
VCC    ->  5V (or 3.3V)
GND    ->  GND
OUT    ->  Pin 2
```

### Security System LEDs
```
LED Color   Arduino Pin   Resistor
Red      -> Pin 5      -> 220Ω -> GND
Yellow   -> Pin 6      -> 220Ω -> GND  
Green    -> Pin 7      -> 220Ω -> GND
```

### Buzzer
```
Buzzer     Arduino
Positive-> Pin 8
Negative-> GND
```

## Setup Instructions

### 1. Arduino IDE Setup
1. Download and install [Arduino IDE](https://www.arduino.cc/en/software)
2. Connect your Arduino to your computer via USB
3. Select your board: `Tools > Board > Arduino Uno`
4. Select your port: `Tools > Port > COM# (Arduino Uno)`

### 2. Library Installation
No additional libraries are required for basic projects. All code uses built-in Arduino functions.

### 3. Upload Process
1. Open the `.ino` file in Arduino IDE
2. Click the "Verify" button (✓) to compile
3. Click the "Upload" button (→) to upload to Arduino
4. Open Serial Monitor (`Tools > Serial Monitor`) to see output

## Educational Progression

### Beginner (Start Here)
1. **ultrasonic_arduino.ino** - Learn basic sensor reading
   - Understand `digitalWrite()` and `digitalRead()`
   - Learn about timing and `delayMicroseconds()`
   - Practice with Serial output

2. **pir_arduino.ino** - Learn about digital sensors and state changes
   - Understand edge detection (motion start/stop)
   - Learn about timeouts and timing
   - Practice with LED control

### Intermediate
3. **security_system_arduino.ino** - Combine multiple sensors
   - Learn about system integration
   - Practice with multiple inputs and outputs
   - Understand state machines and logic

### Advanced
4. **security_system_complete.ino** - Study complete implementation
   - Learn advanced patterns and techniques
   - Understand code organization
   - See professional Arduino coding practices

## Student Exercises

### Level 1: Basic Modifications
- Change distance thresholds and test behavior
- Customize alert messages  
- Modify LED blink patterns
- Adjust buzzer tones and durations

### Level 2: Feature Additions
- Add LCD display for status information
- Implement a disarm button
- Add data logging to EEPROM
- Create different security modes (home/away)

### Level 3: Advanced Projects
- Wireless communication (WiFi/Bluetooth)
- Mobile app integration
- Multiple sensor nodes
- Cloud data logging

## Troubleshooting

### Common Issues

1. **No Serial Output**
   - Check Serial Monitor baud rate (9600)
   - Verify Arduino is connected and port is correct

2. **Ultrasonic Sensor Not Working**
   - Check wiring (VCC to 5V, not 3.3V)
   - Verify trigger and echo pins
   - Ensure sensor has clear line of sight

3. **PIR Sensor Always Triggering**
   - Allow 30-60 seconds for stabilization
   - Check for air currents or heat sources
   - Adjust sensitivity potentiometer if available

4. **LEDs Not Working**
   - Check resistor connections (220Ω required)
   - Verify LED polarity (long leg = positive)
   - Test with simple blink sketch

5. **Buzzer Silent**
   - Check connections (positive to pin, negative to GND)
   - Try different tones and durations
   - Some buzzers are directional

### Code Debugging
- Use `Serial.print()` statements to debug
- Check variable values at different points
- Test one sensor at a time
- Use built-in LED (pin 13) for simple status indication

## Next Steps

After completing the Arduino workshop:
1. Try the Raspberry Pi version for comparison
2. Explore Arduino libraries for additional sensors
3. Learn about Arduino networking (WiFi, Ethernet)
4. Investigate real-time operating systems (FreeRTOS)
5. Move to more advanced microcontrollers (ESP32, STM32)

## Resources

- [Arduino Official Documentation](https://docs.arduino.cc/)
- [Arduino Language Reference](https://www.arduino.cc/reference/en/)
- [Sensor Datasheets and Tutorials](https://www.arduino.cc/en/Tutorial/HomePage)
- [Arduino Community Forums](https://forum.arduino.cc/)