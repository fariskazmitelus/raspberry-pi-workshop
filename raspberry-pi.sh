#!/bin/bash

# Colors for echo commands
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_color() {
    echo -e "${YELLOW}$1${NC}"
}

# Function to check if a command was successful
check_command() {
    if [ $? -ne 0 ]; then
        echo -e "${RED}Error: $1 failed.${NC}"
        return 1
    fi
    return 0
}

# Function to check if a step has been completed
step_completed() {
    if [ -f "/tmp/rpi_setup_$1" ]; then
        echo -e "${GREEN}Step '$1' already completed. Skipping...${NC}"
        return 0
    fi
    return 1
}

# Function to mark a step as completed
mark_step_completed() {
    touch "/tmp/rpi_setup_$1"
}

# Create a directory for the project
PROJECT_DIR="$HOME/raspberry_pi_project"
mkdir -p "$PROJECT_DIR"
cd "$PROJECT_DIR"

# Check and install Python if not present
if ! command -v python3 &> /dev/null; then
    print_color "Python3 not found. Installing..."
    sudo apt install -y python3 python3-pip python3-venv
    check_command "Python3 installation" || exit 1
fi

# Update and upgrade system
if ! step_completed "system_update"; then
    print_color "Updating and upgrading system packages..."
    sudo apt update && sudo apt full-upgrade -y
    if check_command "System update and upgrade"; then
        mark_step_completed "system_update"
    else
        echo -e "${RED}System update failed. Please check your internet connection and try again.${NC}"
        exit 1
    fi
fi

# Install system packages
if ! step_completed "system_packages"; then
    print_color "Installing system packages..."
    export DEBIAN_FRONTEND=noninteractive
    yes | sudo apt install -y code libatlas-base-dev build-essential rpi-connect
    if check_command "System package installation"; then
        mark_step_completed "system_packages"
    fi
fi

# Create virtual environment
if ! step_completed "venv_creation"; then
    print_color "Creating Python virtual environment..."
    python3 -m venv --system-site-packages venv
    source venv/bin/activate
    print_color "Virtual environment created and activated."
    pip install --upgrade pip
    mark_step_completed "venv_creation"
else
    source venv/bin/activate
fi

# Clone GitHub repositories
if ! step_completed "github_repos"; then
    print_color "Cloning GitHub repositories..."
    git clone https://github.com/raspberrypi/picamera2 || true
    git clone https://github.com/degzero/Python_ST7735.git || true
    mark_step_completed "github_repos"
fi

# Install Python packages individually
if ! step_completed "python_packages"; then
    print_color "Installing Python packages..."
    
    print_color "Installing RPi.GPIO..."
    pip install RPi.GPIO
    check_command "RPi.GPIO installation"
    
    print_color "Installing Adafruit_GPIO..."
    pip install Adafruit_GPIO
    check_command "Adafruit_GPIO installation"
    
    print_color "Installing numpy..."
    pip install numpy
    check_command "numpy installation"
    
    print_color "Installing pillow..."
    pip install pillow
    check_command "pillow installation"
    
    print_color "Installing tflite-runtime..."
    pip install tflite-runtime
    check_command "tflite-runtime installation"
    
    print_color "Installing opencv-python..."
    pip install opencv-python
    check_command "opencv-python installation"
    
    mark_step_completed "python_packages"
fi

# Install ST7735 library
if ! step_completed "st7735_library"; then
    print_color "Installing ST7735 library..."
    cd Python_ST7735
    python setup.py install
    if check_command "ST7735 library installation"; then
        mark_step_completed "st7735_library"
    fi
    cd "$PROJECT_DIR"
fi

# Clone workshop repository
if ! step_completed "workshop_repo"; then
    print_color "Cloning workshop repository..."
    git clone https://github.com/fariskazmitelus/raspberry-pi-workshop || true
    if check_command "Workshop repository cloning"; then
        mark_step_completed "workshop_repo"
    fi
fi

print_color "Script completed successfully!"
print_color "Important next steps:"
print_color "1. To activate the virtual environment in the future, run:"
print_color "   source $PROJECT_DIR/venv/bin/activate"
print_color "2. To set up Raspberry Pi Connect, run these commands:"
print_color "   rpi-connect on"
print_color "   rpi-connect signin"
print_color "3. Enable SPI interface manually using raspi-config:"
print_color "   sudo raspi-config"
print_color "   Navigate to 'Interface Options' > 'SPI' and enable it"

deactivate
