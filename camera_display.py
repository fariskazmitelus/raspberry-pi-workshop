from PIL import Image
import ST7735 as TFT
import Adafruit_GPIO as GPIO
import Adafruit_GPIO.SPI as SPI
from picamera2 import Picamera2
import time

# Display configuration for 1.44" display
WIDTH = 128
HEIGHT = 128
SPEED_HZ = 4000000

# Raspberry Pi configuration
DC = 24
RST = 25
SPI_PORT = 0
SPI_DEVICE = 0

# Initialize display
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

# Initialize camera
picam2 = Picamera2()
preview_config = picam2.create_preview_configuration(
    main={"size": (640, 480)})
picam2.configure(preview_config)
picam2.start()

try:
    while True:
        frame = picam2.capture_array()
        image = Image.fromarray(frame)
        image = image.rotate(90).resize((WIDTH, HEIGHT))
        disp.display(image)
        time.sleep(0.1)

except KeyboardInterrupt:
    print("Stopping...")
    picam2.stop()