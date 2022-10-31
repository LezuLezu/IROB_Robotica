import serial
import time

if __name__ == "__main__":
    print("Press CTRL-C to stop program")
    with serial.Serial('/dev/ttyS0', baudrate=9600, timeout=3.0) as arduino:
       time.sleep(0.5)
       if arduino.isOpen():
        print("%s connected" % arduino.port)
        try:
            while True:
                cmd = input("Turn LED on or off [ON / OFF]: ")     
                if cmd == "ON":
                    arduino.write(b'1')
                if cmd == "OFF":
                    arduino.write(b'0')
                # time.sleep(0.5)
                while arduino.inWaiting() == 0:
                    print("Waiting for response...")
                    pass
                if arduino.inWaiting() > 0:
                    answer = arduino.readline().rstrip()
                    print(answer)
                    arduino.flushInput()
        except KeyboardInterrupt:
            print("Program stopped by user")
            arduino.close()
            print("Connection closed")
