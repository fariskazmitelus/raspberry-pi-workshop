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


## Imaging

1. Download and install Win32DiskImager (doesn't work on corporate laptops)
2. Plug existing Raspberry Pi SD Card into computer and turn it into an `.img` using Win32DiskImager
    - Choose device
    - Make a new file with `.img` extension and choose it
    - Click "read"

3. Download pishrink to reduce img size:
```
sudo apt update && sudo apt install -y wget parted gzip pigz xz-utils udev e2fsprogs zip jq uuid-runtime
wget https://raw.githubusercontent.com/Drewsif/PiShrink/master/pishrink.sh
chmod +x pishrink.sh
sudo mv pishrink.sh /usr/local/bin
```
4. Run pishrink on your image
```
sudo pishrink.sh ~/MyImage.img
```
5. Download factory-reset patch repo
```
git clone https://github.com/fariskazmi/raspberry-pi-factory-reset
```
6. Copy image to inside git repo
7. Patch img to include factory-reset capability
```
chmod +x create-factory-reset
sudo ./create-factory-reset -i <img> -a
```
8. Flash new .restore.img to Pi
9. You can now, on the pi, run to factory reset:
```
sudo su
/boot/factory_reset --reset
```