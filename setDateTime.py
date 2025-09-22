import datetime
import serial
import sys
import time
Import("env")

SERIAL_PORT = '/dev/ttyUSB0'
BAUDRATE = 115200


def set_time(ser):
    # Wait for second change
    last = datetime.datetime.now()
    while 1:
        now = datetime.datetime.now()
        if last.second != now.second:
            break
    now += datetime.timedelta(0, 1)

    # Write setdate time command
    ser.write(str.encode('setdatetime {},{},{},{},{},{},{}'.format(
        now.hour, now.minute, now.second, now.day, now.month, now.year, now.weekday()+1)))

    # Wait for second change
    newline = str.encode('\n')
    last = datetime.datetime.now()

    while 1:
        now = datetime.datetime.now()
        if last.second != now.second:
            break

    # Write newline
    ser.write(newline)

    print('setdatetime {},{},{},{},{},{},{}'.format(now.hour, now.minute,
          now.second, now.day, now.month, now.year, now.weekday()+1))

    ser.write(str.encode('factoryreset\n'))


def set_epoch(ser):
    epoch_time = int(time.time())
    ser.write(str.encode('setepochtime {}\n'.format(epoch_time)))
    print('setepochtime {}'.format(epoch_time))

def main(source, target, env):
    print('Setting DateTime...')

    ser = serial.Serial()
    ser.baudrate = int(BAUDRATE)
    ser.bytesize = 8
    ser.stopbits = 1
    ser.parity = serial.PARITY_NONE
    ser.timeout = 0.01
    ser.port = SERIAL_PORT
    try:
        ser.open()
        set_time(ser)
        set_epoch(ser)

    except serial.SerialException:
        print('Error: Cannot open serial port {}'.format(SERIAL_PORT))
        sys.exit(1)


if __name__ == '__main__':
    main()

env.AddPostAction("upload", main)
