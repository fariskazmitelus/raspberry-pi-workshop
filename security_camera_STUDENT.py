#!/usr/bin/python3

################ SETUP CODE (Students don't need to modify this) ################
import argparse
import cv2
import numpy as np
import tflite_runtime.interpreter as tflite
from PIL import Image
import ST7735 as TFT
import Adafruit_GPIO.SPI as SPI
from picamera2 import Picamera2
import time
import sys
import RPi.GPIO as GPIO
import random  # For random alert messages!

# Display and camera setup (Don't worry about these details!)
WIDTH = 128
HEIGHT = 128
SPEED_HZ = 4000000
DC = 24
RST = 25
SPI_PORT = 0
SPI_DEVICE = 0
normalSize = (640, 480)
lowresSize = (320, 240)
rectangles = []

################ STUDENT CONFIGURATION SECTION ################
# TODO: Set these values to customize your security camera!

# How close (in cm) does someone need to be to trigger the alert?
DISTANCE_THRESHOLD = None  # Try starting with 100!

# How sure should the camera be that it sees a person? (0.0 to 1.0)
# TODO: Start with a low value and adjust it to reduce false detections
CONFIDENCE_THRESHOLD = 0.3  # Try different values between 0.3 and 0.9

# How often should we check for movement? (in seconds)
CHECK_INTERVAL = 0.1

################ STUDENT FUNCTIONS TO MODIFY ################

def create_security_message(distance):
    """
    TODO: Create your own security alert message!
    
    You can use these variables in your message:
    - distance: How far away the person is (in cm)
    """
    # Delete 'pass' and write your message here!
    pass

def check_security_zone(distance):
    """
    TODO: Create different alerts for different distances!
    """
    # Delete 'pass' and write your distance checks here!
    pass

################ SETUP FUNCTIONS (Students don't need to modify these) ################

# Ultrasonic sensor pins
TRIG = 4
ECHO = 17

def setup_ultrasonic():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(TRIG, GPIO.OUT)
    GPIO.setup(ECHO, GPIO.IN)

def measure_distance():
    GPIO.output(TRIG, True)
    time.sleep(0.00001)
    GPIO.output(TRIG, False)

    start_time = time.time()
    stop_time = time.time()

    while GPIO.input(ECHO) == 0:
        start_time = time.time()

    while GPIO.input(ECHO) == 1:
        stop_time = time.time()

    time_elapsed = stop_time - start_time
    distance = (time_elapsed * 34300) / 2
    return distance

def initialize_display():
    try:
        disp = TFT.ST7735(
            DC, rst=RST,
            spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE, max_speed_hz=SPEED_HZ),
            width=WIDTH, height=HEIGHT
        )
        disp.begin()
        disp.clear()
        disp.display()
        return disp
    except Exception as e:
        print(f"Failed to initialize display: {e}")
        sys.exit(1)

def initialize_camera():
    try:
        picam2 = Picamera2()
        config = picam2.create_preview_configuration(
            main={"size": normalSize},
            lores={"size": lowresSize, "format": "YUV420"}
        )
        picam2.configure(config)
        picam2.start()
        return picam2
    except Exception as e:
        print(f"Failed to initialize camera: {e}")
        sys.exit(1)

def ReadLabelFile(file_path):
    with open(file_path, 'r') as f:
        return {int(key): value.strip() for key, value in 
                (line.strip().split(maxsplit=1) for line in f)}

def DrawRectangles(image):
    for rect in rectangles:
        start_point = (int(rect[0] * 2) - 5, int(rect[1] * 2) - 5)
        end_point = (int(rect[2] * 2) + 5, int(rect[3] * 2) + 5)
        cv2.rectangle(image, start_point, end_point, (0, 255, 0), 1)
        if len(rect) == 5:
            cv2.putText(image, rect[4][:10], 
                      (int(rect[0] * 2) + 5, int(rect[1] * 2) + 5),
                      cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)
    return image

def InferenceTensorFlow(image, interpreter, labels):
    global rectangles
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    height = input_details[0]['shape'][1]
    width = input_details[0]['shape'][2]

    rgb = cv2.cvtColor(image, cv2.COLOR_GRAY2RGB)
    initial_h, initial_w, _ = rgb.shape
    picture = cv2.resize(rgb, (width, height))
    input_data = np.expand_dims(picture, axis=0)

    if input_details[0]['dtype'] == np.float32:
        input_data = (np.float32(input_data) - 127.5) / 127.5

    interpreter.set_tensor(input_details[0]['index'], input_data)
    interpreter.invoke()

    # Get detection results
    detected_boxes = interpreter.get_tensor(output_details[0]['index'])
    detected_classes = interpreter.get_tensor(output_details[1]['index'])
    detected_scores = interpreter.get_tensor(output_details[2]['index'])
    num_boxes = interpreter.get_tensor(output_details[3]['index'])

    rectangles = []
    for i in range(int(num_boxes)):
        if detected_scores[0][i] > CONFIDENCE_THRESHOLD:
            if labels[int(detected_classes[0][i])] == 'person':
                top, left, bottom, right = detected_boxes[0][i]
                xmin = left * initial_w
                ymin = bottom * initial_h
                xmax = right * initial_w
                ymax = top * initial_h
                rectangles.append([xmin, ymin, xmax, ymax, 'person'])
                return True
    return False

def prepare_frame_for_display(frame):
    frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    image = Image.fromarray(frame_rgb)
    return image.rotate(90).resize((WIDTH, HEIGHT), Image.Resampling.LANCZOS)

################ MAIN PROGRAM ################

def main():
    # Setup code (don't worry about this part!)
    parser = argparse.ArgumentParser()
    parser.add_argument('--model', help='Path of the detection model.', required=True)
    parser.add_argument('--label', help='Path of the labels file.', required=True)
    args = parser.parse_args()

    interpreter = tflite.Interpreter(model_path=args.model, num_threads=4)
    interpreter.allocate_tensors()
    labels = ReadLabelFile(args.label)

    disp = initialize_display()
    picam2 = initialize_camera()
    setup_ultrasonic()
    stride = picam2.stream_configuration("lores")["stride"]

    print("Starting security camera...")

    # Variables to track detections (optional)
    detection_count = 0
    last_detection_time = time.time()
    
    # You code here!
    try:
        while True:
            #1 TODO: Get the distance from the ultrasonic sensor
            
            #2 Capture and process camera image
            buffer = picam2.capture_buffer("lores")
            grey = buffer[:stride * lowresSize[1]].reshape((lowresSize[1], stride))

            #3 TODO: Check if something is closer than DISTANCE_THRESHOLD
            # HINT: Use an if statement with distance and DISTANCE_THRESHOLD
            if True:
                #4 TODO: Check if a person was detected
                person_detected = False  # Replace this line!
                
                #5 TODO: If a person is detected and they're too close, create an alert!
            
            
            # Update the display (don't worry about this part!)
            frame = picam2.capture_array()
            frame = DrawRectangles(frame)
            display_image = prepare_frame_for_display(frame)
            if display_image:
                disp.display(display_image)

            # Wait before next check
            time.sleep(CHECK_INTERVAL)

    except KeyboardInterrupt:
        print("\nStopping security camera...")
    finally:
        picam2.stop()
        GPIO.cleanup()
        print("Camera stopped and GPIO cleaned up")

if __name__ == '__main__':
    main()