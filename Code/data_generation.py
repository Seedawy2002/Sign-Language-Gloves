import serial
import time

# Set up the serial connection (COM port may vary, check your Arduino IDE)
ser = serial.Serial('COM3', 115200, timeout=1)  # Open serial port at 115200 baudrate

start_time = time.time()*1000
while True:
    if time.time()*1000 - start_time > 50000:
        break
    ser.readline().decode('utf-8').rstrip()  # Read a line and strip newline

# Open a file to write data with UTF-8 encoding
with open("sensor_data.csv", "a", encoding="utf-8") as file:
    while True:
        char = input('Enter char: ') # should vary according to the gesture
        if char == '!':
            break
        start_time = time.time()*1000
        while True:
            if time.time()*1000 - start_time > 500:
                break  # Exit the loop after 10 secs
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').rstrip()  # Read a line and strip newline
                line += ',' + char
                print(line)  # Print line to console (optional)
                file.write(line + "\n")  # Write line to file
            file.flush()  # Ensure all data is written to the file

# Close the serial connection
ser.close()
