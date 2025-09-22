import serial
import sys
import time
import json
import csv
import matplotlib.pyplot as plt

def read_line(line):
    line = line.decode()
    line = line.strip()
    return line

plt.ion()
fig=plt.figure(figsize=(20,10))

i=0
x=list()
y1=list()
y2=list()

SERIAL_PORT = '/dev/ttyUSB0'
BAUDRATE = 115200


ser = serial.Serial()
ser.baudrate = int(BAUDRATE)
ser.bytesize = 8
ser.stopbits = 1
ser.parity = serial.PARITY_NONE
ser.timeout = 0.01
ser.port = SERIAL_PORT
try:
    ser.open()
except serial.SerialException:
        print('Error: Cannot open serial port {}'.format(SERIAL_PORT))
        sys.exit(1)

#filename = 'ntc_record.csv'
#header = ['second(s)','NTC1 ','NTC2']
#with open(filename, 'a') as file:
#    csvwriter = csv.writer(file)
#    csvwriter.writerow(header)
#    csvwriter.writerow(["1","2","3"])
#    csvwriter.writerow(["4","5","6"])

while True:
    # Read line from serial port
    ser.write(str.encode('getmachinestatus\n'))
    line = ser.readline()
    line = read_line(line)
    if line:            
            try:
                # Print every line
                dict_json = json.loads(line)
                #print(line)
                print(dict_json["ntcOneCurrentTemp"])
                print(dict_json["ntcTwoCurrentTemp"])
                #time.sleep(1)
                x.append(i)
                y1.append(dict_json["ntcOneCurrentTemp"])
                y2.append(dict_json["ntcTwoCurrentTemp"])
                plt.title("NTC1 NTC2 Real Time plotting")
                plt.grid(axis = 'x')
                plt.plot(x,y1,'b',x,y2,'g')

                plt.xlabel("Time")
                plt.ylabel("Temperature")
                plt.ylim(-10,100)
                i= i+1
                plt.show()
                plt.pause(1)  # Note this correction

   

            except:
                continue
