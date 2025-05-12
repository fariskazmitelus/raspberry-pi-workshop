from gpiozero import MotionSensor

# Set the pin for the motion sensor!
pir = MotionSensor(pin = None)

# Keep track of how many times it detected movement
movement_count = 0


while True:
    pir.wait_for_motion()
    print("Movement Detected")
    pir.wait_for_no_motion(timeout = 5)
    print("Resetting")