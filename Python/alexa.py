from time import *
import serial
import requests
import os
import subprocess

# constants

# For Kyle
# SERIALPORT = "COM4"
# TOKEN = "c5a36fae88b8defe5b6d8ced9de943b579dda47996f6006550f5079a3d7b3ef2"
# For Ian
SERIALPORT = "COM9"
TOKEN = "c1024d1ddb83511773d83060d1b43d2d4bc9c59a338f4f3d778725dd4931f5c2"

BAUD_RATE = 115200
USERS = ["Kyle", "Ian"]

# global variables
brightness = 1
headers = {
    "Authorization": "Bearer %s" % TOKEN,
}
ser=serial.Serial(SERIALPORT, BAUD_RATE, timeout=1)

# LIFX update function
def changeLight():
    payload = {
        "brightness": brightness,
    }
    response = requests.put('https://api.lifx.com/v1/lights/all/state', data=payload, headers=headers)

# brightness change functions
def increaseBrightness():
    global brightness
    brightness = brightness + 0.3 if brightness <= 0.7 else 1

def decreaseBrightness():
    global brightness
    brightness = brightness - 0.3 if brightness >= 0.3 else 0

# print loading dots
def printLoading():
    for i in range(10):
        sleep(2)
        print('.')

# voice recognition function
def voiceRec():
    subprocess.Popen('voicerec.bat', creationflags=subprocess.CREATE_NEW_CONSOLE)
    print("VoiceRec batch file initialized...")
    print("Initializing MAX78000 voice recognition program...")
    printLoading()
    print("MAX78000 voice recognition program initialized.")
    print("Available commands: {ON, OFF, UP, DOWN}")

    global brightness

    while True:
        s = ser.readline()
        try:
            if "UP" in s.decode():
                print("Command: UP")
                increaseBrightness()
                changeLight()
            elif "DOWN" in s.decode():
                print("Command: DOWN")
                decreaseBrightness()
                changeLight()
            elif "OFF" in s.decode():
                print("Command: OFF")
                brightness = 0
                changeLight()
            elif "ON" in s.decode():
                print("Command: ON")
                brightness = 1
                changeLight()
            elif "STOP" in s.decode():
                print("Command: STOP")
                os.system("TASKKILL /F /IM arm-none-eabi-gdb.exe")
                print('You have been logged out.')
                faceRec()
        except UnicodeDecodeError as e:
            print(str(e))
        sleep(0.01)

# face recognition function
def faceRec():
    subprocess.Popen('faceid.bat', creationflags=subprocess.CREATE_NEW_CONSOLE)
    print("FaceId batch file initialized...")
    print("Initializing MAX78000 FaceID...")
    printLoading()
    print("Please authenticate with FaceID.")
    while True:
        s = ser.readline()
        try:
            for user in USERS:
                if user in s.decode():
                    os.system("TASKKILL /F /IM arm-none-eabi-gdb.exe")
                    print('Authentication successful. Welcome, {}!'.format(user))
                    voiceRec()
        except UnicodeDecodeError as e:
            print(str(e))
        sleep(0.01)

print('--------------------------------------------------')
print("""This program changes the brightness of an LIFX smart bulb using voice recognition.
Please refer to the README for more details.""")
print('--------------------------------------------------')
print('Setting bulb to max brightness...')
changeLight()
print('Bulb initialized...')
faceRec()