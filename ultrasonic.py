from gpiozero import DistanceSensor

ultrasonic = None
in_range_message = None
out_range_message = None

while True:
    ultrasonic.wait_for_in_range()
    print(in_range_message)
    ultrasonic.wait_for_out_of_range()
    print(out_range_message)