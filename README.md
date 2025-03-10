# raspberry-pi-workshop

## Setup
1. Clone this repo onto the pi
2. Run ```dos2unix raspberry-pi.sh```
3. Run ```chmod u+x raspberry-pi.sh```
4. Run ```./raspberry-pi.sh```
5. During initial updates, you may be prompted around 90% completion to make a choice. Just hit enter.
6. follow additional prompts given at the end of the script

## Script
Ensure the virtual environment is active when running either python script

```recognition.py``` must be moved in ```picamera2/examples/tensorflow``` to work. Use the following command to test it:
```python3 recognition.py --model mobilenet_v2.tflite --label coco_labels.txt```
