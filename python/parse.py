import os
import sys
import struct

o = open('full-dump.bin','wb')
f = open('full-dump.txt').readlines()
for line in f:
    vals = line.strip().split('-')
    for val in vals:
        if len(val) > 0:
            try:
                o.write(struct.pack('B',int(val,16))[0])        
            except Exception as e:
                print val
                continue
o.close()

