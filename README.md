# AirBeats: Arduino Code for Portable Air Drumming

## Overview
This repository contains the Arduino sketch for the AirBeats project. AirBeats uses two MPU-6050 sensors embedded in 3D-printed drumsticks to detect motion and translate it into MIDI signals—letting you drum anywhere without a physical kit.

## Features
- **Real-Time Motion Detection:** Uses accelerometers and gyroscopes from two MPU-6050 sensors to detect striking motions.  
- **MIDI Output:** Translates sensor data into MIDI events so you can trigger drum sounds in any Digital Audio Workstation (DAW) or sampler.  
- **Modular Design:** Easily tweak acceleration thresholds and drum notes to suit your style or add new triggers.

## Hardware Requirements
- **Arduino Board** (e.g., Arduino Leonardo, Micro, or any board supporting MIDI over USB)  
- **Two MPU-6050 Sensors** (one for each drumstick)  
- **Wiring:** Connect each sensor to the Arduino via I²C (SDA → SDA, SCL → SCL).  
- **3D-Printed Drumsticks** (optional but recommended for best results)

## Software Setup
1. Install the following libraries (if not already available):
   - [Wire](https://www.arduino.cc/en/reference/wire) (Arduino I²C library)
   - [MPU6050 library](https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050) (or an equivalent)
   - [MIDIUSB](https://www.arduino.cc/reference/en/libraries/midiusb/) for sending MIDI events over USB
2. Open the `.ino` file in the Arduino IDE.
3. Select the correct board and port under **Tools** → **Board** and **Port**.
4. Upload the code to your Arduino.

## Usage
- After uploading, open your DAW or MIDI monitoring tool to confirm incoming MIDI signals.
- Swing the drumsticks to generate drum hits:
  - **Snare**, **hi-hat**, **bass**, and **crash** are configured as default MIDI notes.
- Adjust thresholds in the code if you want to fine-tune the motion sensitivity.
