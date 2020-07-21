import serial
import time
ArduinoUnoSerial = serial.Serial('com6',9600)
print (ArduinoUnoSerial.readline())
