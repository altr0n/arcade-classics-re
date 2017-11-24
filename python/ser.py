import time
import os
import serial
import struct
import sys
import binascii

pages = []
outfile = open("eeprom.bin",'wb')

ser = serial.Serial("/dev/{}".format(sys.argv[1]),115200,timeout=5)
start_msg = ser.readline()
while start_msg.strip() != "READY":
    start_msg = ser.readline()
ser.write("")
print start_msg
total = 0xFFFF
x = 0xFFFE
while(x > 0):
    print "On page: {} of {}".format(x,total)
    data = ser.readline()
    if data.strip() == "READY":
        ser.write("plz")
        print data
    else:
        outfile.write(binascii.unhexlify(data.strip()))
        x -= 1
outfile.close()
