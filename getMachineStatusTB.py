import serial
import sys
import time
import json
from tb_device_mqtt import TBDeviceMqttClient, TBPublishInfo

client = TBDeviceMqttClient("192.168.1.36", "wQnk6IsIerDkh0sVIhAt")
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
        time.sleep(3)
        # Read line from serial port
        ser.write(str.encode('getmachinestatus\n'))
        line = read_line(ser.readline())
        if line:            
            try:
                # Print every line
                dict_json = json.loads(line)
                #if ('totalDispenseVolumeSinceFilterReset' in dict_json):
                #    for key in dict_json:
                #        val = dict_json[key]
                #        print(key + ': ' + str(val))

                client.connect()
                # Sending telemetry without checking the delivery status
                client.send_telemetry(dict_json) 
                # Sending telemetry and checking the delivery status (QoS = 1 by default)
                result = client.send_telemetry(dict_json)
                # get is a blocking call that awaits delivery status  
                success = result.get() == TBPublishInfo.TB_ERR_SUCCESS
                # Disconnect from ThingsBoard
                client.disconnect()
            except:
                continue

if __name__ == '__main__':
    main()
