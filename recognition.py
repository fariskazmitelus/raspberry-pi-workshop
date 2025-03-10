#!/usr/bin/python3

import argparse
import cv2
import numpy as np
import tflite_runtime.interpreter as tflite
from PIL import Image
import ST7735 as TFT
import Adafruit_GPIO as GPIO
import Adafruit_GPIO.SPI as SPI
from picamera2 import MappedArray, Picamera2
import time
import sys

# Display configuration
WIDTH = 128
HEIGHT = 128
SPEED_HZ = 4000000
DC = 24
RST = 25
SPI_PORT = 0
SPI_DEVICE = 0

# Camera configuration
normalSize = (640, 480)
lowresSize = (320, 240)

rectangles = []

def initialize_display():
    """Initialize and test the TFT display"""
    try:
        disp = TFT.ST7735(
            DC,
            rst=RST,
            spi=SPI.SpiDev(
                SPI_PORT,
                SPI_DEVICE,
                max_speed_hz=SPEED_HZ),
            width=WIDTH,
            height=HEIGHT)
        disp.begin()
        disp.clear()
        disp.display()
        print("Display initialized successfully")
        return disp
    except Exception as e:
        print(f"Failed to initialize display: {e}")
        sys.exit(1)

def initialize_camera():
    """Initialize and test the camera"""
    try:
        picam2 = Picamera2()
        config = picam2.create_preview_configuration(
            main={"size": normalSize},
            lores={"size": lowresSize, "format": "YUV420"})
        picam2.configure(config)
        picam2.start()
        print("Camera initialized successfully")
        return picam2
    except Exception as e:
        print(f"Failed to initialize camera: {e}")
        sys.exit(1)

def ReadLabelFile(file_path):
    try:
        with open(file_path, 'r') as f:
            lines = f.readlines()
        ret = {}
        for line in lines:
            pair = line.strip().split(maxsplit=1)
            ret[int(pair[0])] = pair[1].strip()
        return ret
    except Exception as e:
        print(f"Error reading label file: {e}")
        return {}

def DrawRectangles(image):
    try:
        for rect in rectangles:
            rect_start = (int(rect[0] * 2) - 5, int(rect[1] * 2) - 5)
            rect_end = (int(rect[2] * 2) + 5, int(rect[3] * 2) + 5)
            cv2.rectangle(image, rect_start, rect_end, (0, 255, 0), 1)  # Thinner rectangle
            if len(rect) == 5:
                text = rect[4][:10]  # Limit text length
                font = cv2.FONT_HERSHEY_SIMPLEX
                cv2.putText(image, text, 
                          (int(rect[0] * 2) + 5, int(rect[1] * 2) + 5),
                          font, 0.5, (255, 255, 255), 1, cv2.LINE_AA)  # Smaller text
        return image
    except Exception as e:
        print(f"Error drawing rectangles: {e}")
        return image

def InferenceTensorFlow(image, interpreter, labels=None):
    global rectangles
    try:
        input_details = interpreter.get_input_details()
        output_details = interpreter.get_output_details()
        height = input_details[0]['shape'][1]
        width = input_details[0]['shape'][2]
        floating_model = input_details[0]['dtype'] == np.float32

        rgb = cv2.cvtColor(image, cv2.COLOR_GRAY2RGB)
        initial_h, initial_w, channels = rgb.shape

        picture = cv2.resize(rgb, (width, height))
        input_data = np.expand_dims(picture, axis=0)
        
        if floating_model:
            input_data = (np.float32(input_data) - 127.5) / 127.5

        interpreter.set_tensor(input_details[0]['index'], input_data)
        interpreter.invoke()

        detected_boxes = interpreter.get_tensor(output_details[0]['index'])
        detected_classes = interpreter.get_tensor(output_details[1]['index'])
        detected_scores = interpreter.get_tensor(output_details[2]['index'])
        num_boxes = interpreter.get_tensor(output_details[3]['index'])

        rectangles = []
        for i in range(int(num_boxes)):
            top, left, bottom, right = detected_boxes[0][i]
            classId = int(detected_classes[0][i])
            score = detected_scores[0][i]
            if score > 0.5:
                xmin = left * initial_w
                ymin = bottom * initial_h
                xmax = right * initial_w
                ymax = top * initial_h
                box = [xmin, ymin, xmax, ymax]
                rectangles.append(box)
                if labels:
                    print(f"Detected {labels[classId]} with confidence {score:.2f}")
                    rectangles[-1].append(labels[classId])
    except Exception as e:
        print(f"Error during inference: {e}")

def prepare_frame_for_display(frame):
    """Prepare the frame for display on the TFT"""
    try:
        # Convert color space
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        # Convert to PIL Image
        image = Image.fromarray(frame_rgb)
        # Resize and rotate
        image = image.rotate(90).resize((WIDTH, HEIGHT), Image.Resampling.LANCZOS)
        return image
    except Exception as e:
        print(f"Error preparing frame: {e}")
        return None

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--model', help='Path of the detection model.', required=True)
    parser.add_argument('--label', help='Path of the labels file.')
    args = parser.parse_args()

    # Initialize TFLite interpreter
    try:
        interpreter = tflite.Interpreter(model_path=args.model, num_threads=4)
        interpreter.allocate_tensors()
        print("TFLite model loaded successfully")
    except Exception as e:
        print(f"Failed to load TFLite model: {e}")
        sys.exit(1)

    # Load labels
    labels = ReadLabelFile(args.label) if args.label else None

    # Initialize display and camera
    disp = initialize_display()
    picam2 = initialize_camera()
    stride = picam2.stream_configuration("lores")["stride"]

    print("Starting main loop...")
    frame_count = 0
    last_time = time.time()

    try:
        while True:
            # Capture and process frame
            buffer = picam2.capture_buffer("lores")
            grey = buffer[:stride * lowresSize[1]].reshape((lowresSize[1], stride))
            
            # Run inference
            InferenceTensorFlow(grey, interpreter, labels)
            
            # Process main frame
            frame = picam2.capture_array()
            frame = DrawRectangles(frame)
            
            # Prepare and display frame
            display_image = prepare_frame_for_display(frame)
            if display_image:
                disp.display(display_image)

            # Calculate and print FPS every 30 frames
            frame_count += 1
            if frame_count % 30 == 0:
                current_time = time.time()
                fps = 30 / (current_time - last_time)
                print(f"FPS: {fps:.2f}")
                last_time = current_time

            time.sleep(0.1)  # Adjust this value if needed

    except KeyboardInterrupt:
        print("\nStopping gracefully...")
    except Exception as e:
        print(f"An error occurred: {e}")
    finally:
        picam2.stop()
        print("Camera stopped")

if __name__ == '__main__':
    main()