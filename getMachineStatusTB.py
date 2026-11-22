import os
import serial
import sys
import time
import json
from pathlib import Path
from tb_device_mqtt import TBDeviceMqttClient, TBPublishInfo

SERIAL_PORT = '/dev/ttyUSB0'
BAUDRATE = 115200


def load_dotenv(path):
    if not path.is_file():
        return
    for raw in path.read_text().splitlines():
        line = raw.strip()
        if not line or line.startswith("#"):
            continue
        if line.startswith("export "):
            line = line[len("export "):].strip()
        if "=" not in line:
            continue
        key, value = line.split("=", 1)
        key = key.strip()
        value = value.strip().strip('"').strip("'")
        if key and key not in os.environ:
            os.environ[key] = value


def require_thingsboard_config():
    load_dotenv(Path(__file__).resolve().parent / ".env")
    names = ("TB_BROKER_HOST", "TB_ACCESS_TOKEN")
    missing = [name for name in names if not os.environ.get(name, "").strip()]
    if missing:
        print(
            "Error: missing environment variable(s): " + ", ".join(missing),
            file=sys.stderr,
        )
        print(
            "Copy .env.example to .env and set them, or export them in the shell.",
            file=sys.stderr,
        )
        sys.exit(1)
    return os.environ["TB_BROKER_HOST"].strip(), os.environ["TB_ACCESS_TOKEN"].strip()


def read_line(line):
    line = line.decode('ascii')
    line = line.strip()
    return line

def main():
    broker_host, access_token = require_thingsboard_config()
    client = TBDeviceMqttClient(broker_host, access_token)

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
