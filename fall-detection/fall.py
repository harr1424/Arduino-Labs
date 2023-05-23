"""
Author: JOHN HARRINGTON

This Python script will receive the data transmitted by the Arduino as programmed
in fall.ino 

This program requires the address of a Bluetooth module, and will receive data 
transmitted from this module. This data is transmitted as JSON, and converted 
here to a Python dictionary where keys correspond to IMU measurement types 
and values to measurement values. 

These values are used to perform calculations described in the following paper:
https://www.ncbi.nlm.nih.gov/pmc/articles/PMC6412321/

These values are then used to perform a fall detection algorithm described by 
the same paper. 

If a fall is detected, "FALL DETECTED" will be printed to std out. 
"""

import bluetooth
from ast import literal_eval
import re
import math

pattern = re.compile(r'{.*?}')

blth_addr = "98:D3:31:FD:C7:4A"  
socket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
socket.connect((blth_addr, 1))


def process_reading(buffer):
    match = re.search(pattern, buffer)
    start, end = match.span()
    reading = literal_eval(buffer[start:end])
    buffer = buffer[end:]
    return buffer, reading


def sag_vertical_angle(a_z, a_y):
    try:
        raw_angle = ((a_z / a_y) * (180 / math.pi))
        return math.degrees(math.atan(raw_angle))

    except ZeroDivisionError:
        return 0


def front_vertical_angle(a_x, a_y):
    try:
        raw_angle = ((a_x / a_y) * (180 / math.pi))
        return math.degrees(math.atan(raw_angle))
    
    except ZeroDivisionError:
        return 0


def sum_vec_acc(a_x, a_y, a_z):
    return math.sqrt((a_x ** 2) + (a_y ** 2) + (a_z ** 2))


def sum_vec_gyr(g_x, g_y):
    return math.sqrt((g_x ** 2) + (g_y ** 2))


def detect_fall(va_sag, va_frontal, svm_a, svm_g):
    if (va_frontal > 24.7 or va_sag > 24.7 ) and svm_a < 900 and svm_g > 47.3:
        return True
    else: 
        return False


def main():
    print("Listening for fall...")
    data = True
    buffer = ""
    while data:
        data = socket.recv(1024)
        buffer += str(data, encoding='ascii')
        # print("Buffer contents:", buffer)
        if re.search(pattern, buffer):
            buffer, measurement = process_reading(buffer)
            # print(measurement.get('a_x'), measurement.get('a_y'), measurement.get('a_z'), measurement.get('g_x'), measurement.get('g_y'))

            va_sag = sag_vertical_angle(measurement.get('a_z'), measurement.get('a_y'))
            va_frontal = front_vertical_angle(measurement.get('a_x'), measurement.get('a_y'))
            svm_a = sum_vec_acc(measurement.get('a_x'), measurement.get('a_y'), measurement.get('a_z'))
            svm_g = sum_vec_gyr(measurement.get('g_x'), measurement.get('g_y'))

            if detect_fall(va_sag, va_frontal, svm_a, svm_g):
                print("FALL DETECTED")


if __name__ == "__main__":
    main()
