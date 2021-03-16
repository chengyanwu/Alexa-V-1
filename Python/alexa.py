from time import *
import serial
import requests
import os
import subprocess

# constants
SERIALPORT = "COM9"
TOKEN = "c1024d1ddb83511773d83060d1b43d2d4bc9c59a338f4f3d778725dd4931f5c2"

# variables
brightness = 1
headers = {
    "Authorization": "Bearer %s" % TOKEN,
}
payload = {
    "brightness": brightness,
}
ser=serial.Serial(SERIALPORT, 115200, timeout=1)

# brightness change functions
def increaseBrightness(brightness):
    return brightness + 0.3 if brightness <= 0.7 else 1

def decreaseBrightness(brightness):
    return brightness - 0.3 if brightness >= 0.3 else 0

# voice recognition function
def voiceRec():
    while True:
        s = ser.readline()
        try:
            print(s.decode())
            if "UP" in s.decode():
                brightness = increaseBrightness(brightness)
                payload = {
                    "brightness": brightness,
                }
                response = requests.put('https://api.lifx.com/v1/lights/all/state', data=payload, headers=headers)
                print(response.content)
            elif "DOWN" in s.decode():
                brightness = decreaseBrightness(brightness)
                payload = {
                    "brightness": brightness,
                }
                response = requests.put('https://api.lifx.com/v1/lights/all/state', data=payload, headers=headers)
                print(response.content)
            elif "OFF" in s.decode():
                brightness = 0
                payload = {
                    "brightness": brightness,
                }
                response = requests.put('https://api.lifx.com/v1/lights/all/state', data=payload, headers=headers)
                print(response.content)
            elif "ON" in s.decode():
                brightness = 1
                payload = {
                    "brightness": brightness,
                }
                response = requests.put('https://api.lifx.com/v1/lights/all/state', data=payload, headers=headers)
                print(response.content)
        except UnicodeDecodeError as e:
            print(str(e))
        sleep(0.01)

# face recognition function
def faceRec():
    print("Batch file initializing...")
    # subprocess.call([r'C:\Users\lemon\Documents\School\ece153b\test.bat'])
    subprocess.Popen('test.bat', creationflags=subprocess.CREATE_NEW_CONSOLE)
    print("Batch file initialized.")
    while True:
        s = ser.readline()
        try:
            print(s.decode())
            if "Ian" in s.decode():
                brightness = 0
                payload = {
                    "brightness": brightness,
                }
                response = requests.put('https://api.lifx.com/v1/lights/all/state', data=payload, headers=headers)

                print("Authenticated. Welcome, Kyle.")
                switchToVoiceRec()
        except UnicodeDecodeError as e:
            print(str(e))
        sleep(0.01)

# switch models function
def switchToVoiceRec():
    print("Switching to voice recognition")
    os.system("TASKKILL /F /IM arm-none-eabi-gdb.exe")
    subprocess.Popen('test2.bat', creationflags=subprocess.CREATE_NEW_CONSOLE)
    print("test2.bat executed")
    voiceRec()

# print('This program changes the brightness of LIFX bulb using STM32L4 microcontroller')
# print('Setting bulb to max brightness...')
# response = requests.put('https://api.lifx.com/v1/lights/all/state', data=payload, headers=headers)
# print(response.content)

faceRec()
