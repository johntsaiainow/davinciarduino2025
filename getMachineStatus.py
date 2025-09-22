import serial
import sys
import json

SERIAL_PORT = '/dev/ttyUSB0'
BAUDRATE = 115200

def read_line(line):
    line = line.decode('ascii')
    line = line.strip()
    return line

def main():

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

    while 1:
        # Read line from serial port
        ser.write(str.encode('getmachinestatus\n'))
        line = read_line(ser.readline())
        if line:            
            try:
                # Print every line
                dict_json = json.loads(line)
                if ('totalDispenseVolumeSinceFilterReset' in dict_json):
                    for key in dict_json:
                        val = dict_json[key]
                        print(key + ': ' + str(val))
            except:
                continue

if __name__ == '__main__':
    main()