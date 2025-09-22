import PySimpleGUI as sg
import json
import serial
import sys
import csv
from os import path


SERIAL_PORT = '/dev/ttyUSB0'
BAUDRATE = 115200

SERIAL_PORT_TIMEOUT = 100

def read_line(line):
    # line = line.decode('ascii')
    line = line.strip()
    return line

def update_values():
    ser.write(str.encode('getmachinestatus\n'))
    line = read_line(ser.readline())
    if line:            
        try:
            # Print every line
            data = json.loads(line)
            if ('totalDispenseVolumeSinceFilterReset' in data):
                with open('machine_status.csv', 'a') as f:         
                    writer = csv.DictWriter(f, fieldnames=csv_columns)       
                    writer.writerow(data)
                for i in data:
                    window[i].update(str(data[i]))
        except:
            pass

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

ser.write(str.encode('getmachinestatus\n'))
line = read_line(ser.readline())

if line:            
    data = json.loads(line)
    if ('totalDispenseVolumeSinceFilterReset' in data):
        column1 = []
        column2 = []
        column3 = []
        csv_columns = []
        with open('machine_status.csv', 'a') as f:
            for key in data:
                csv_columns.append(key)
            writer = csv.DictWriter(f, fieldnames=csv_columns)
            if path.getsize('machine_status.csv') == 0:
                writer.writeheader()
            writer.writerow(data)
        for index, key in enumerate(data):
            
            #f.write("%s,%s\n"%(key,data[key]))
            #row = [sg.Text(i, size=(15,1)), sg.Text(str(data[i]), key=i, size=(15,1))]
            row = [sg.Text(key), sg.Text(str(data[key]), key=key, size=(10,1))]
            if index < 25:
                column1.append(row)
            elif index < 50:
                column2.append(row)
            else:
                column3.append(row)

       #  rows.append([sg.Button('Exit')])
        # sg.theme('Dark')
        layout = [[sg.Column(column1), sg.Column(column2), sg.Column(column3)]]      
        window = sg.Window('Davinci Status', layout, debugger_enabled=False)
        while True:  # Event Loop
            event, values = window.read(SERIAL_PORT_TIMEOUT)
            update_values()
            if event == sg.WIN_CLOSED or event == 'Exit':
                break
        window.close()
