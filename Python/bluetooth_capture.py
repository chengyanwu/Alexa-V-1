from time import *
import serial
import requests

# constants
SERIALPORT = "COM4"
TOKEN = "c5a36fae88b8defe5b6d8ced9de943b579dda47996f6006550f5079a3d7b3ef2"

def increaseBrightness(brightness):
    return brightness + 0.3 if brightness <= 0.7 else 1

def decreaseBrightness(brightness):
    return brightness - 0.3 if brightness >= 0.3 else 0

brightness = 1

headers = {
    "Authorization": "Bearer %s" % TOKEN,
}
payload = {
    "brightness": brightness,
}

print('This program changes the brightness of LIFX bulb using STM32L4 microcontroller')
print('Setting bulb to max brightness...')
response = requests.put('https://api.lifx.com/v1/lights/all/state', data=payload, headers=headers)
print(response.content)

ser=serial.Serial(SERIALPORT, 115200, timeout=1)

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
