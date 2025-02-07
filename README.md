# raspberry-pi-workshop

## Setup

```bash
sudo apt update
sudo apt full-upgrade
sudo apt install rpi-connect
rpi-connect on
rpi-connect signin
sudo apt install code
git clone https://github.com/raspberrypi/picamera2
cd picamera2
cd examples/tensorflow

# Look at the instructions for real_time_with_labels.py at the top of the file and then run
python3 real_time_with_labels.py --model mobilenet_v2.tflite --label coco_labels.txt
```