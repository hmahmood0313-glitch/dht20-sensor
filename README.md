# Raspberry Pi DHT20 Temperature & Humidity Monitor

This is my first Raspberry Pi hardware/software project. I built it to learn how software can communicate directly with hardware instead of only writing programs that run on a computer.

For this project, I connected a DHT20 temperature and humidity sensor to a Raspberry Pi 5 and  wrote a "C" program that communicates with the sensor using I2C.

I also used VS Code Remote - SSH from my Windows computer. This allowed me to write and edit the code in VS Code while the code was actually being compiled and executed on the Raspberry Pi.
## Hardware Setup

This is the Raspberry Pi 5 connected to the DHT20 temperature and humidity sensor using I2C.
<img width="1200" height="1600" alt="image" src="https://github.com/user-attachments/assets/82046c43-dbb3-4d99-8417-575103fedced" />


## Hardware Used

- Raspberry Pi 5
- DHT20 temperature and humidity sensor
- Breadboard / jumper wires
- Raspberry Pi power supply
- Wi-Fi connection
## If the hostname was not working, I could also find the Raspberry Pi's local IP address and connect using:
ssh username@IP_ADDRESS

## VS Code Remote SSH
Instead of writing the program directly inside the Raspberry Pi terminal, I installed the Remote - SSH extension in VS Code.
I connected VS Code to my Raspberry Pi through SSH.
This means:
- VS Code runs on my Windows computer
- My source code is stored on the Raspberry Pi
- The VS Code terminal is connected to the Raspberry Pi
- GCC compiles the program on the Raspberry Pi
- The Raspberry Pi executes the program and communicates with the sensor
This made development much easier because I could use VS Code while still compiling directly for the Raspberry Pi.

## Enabling I2C

I used:
sudo raspi-config

and enabled I2C from the interface settings.
I then installed the I2C utilities:
sudo apt update
sudo apt install i2c-tools

To check whether the Raspberry Pi could detect the sensor, I used:
i2cdetect -y 1

The DHT20 appeared at:
0x38

Seeing 38 in the I2C table confirmed that the Raspberry Pi was communicating with the sensor.

## How the Program Works

The program communicates with the DHT20 through the Linux I2C interface.
The program first opens the Raspberry Pi I2C device:
/dev/i2c-1

It then selects the DHT20 at address:
0x38

To request a measurement, the Raspberry Pi sends the three-byte command:
0xAC 0x33 0x00

The program waits for the DHT20 to finish taking the measurement and then reads seven bytes back from the sensor.
The returned data contains:
- sensor status
- humidity data
- temperature data

The temperature and humidity values are not returned directly as normal decimal numbers. They are packed into bits across multiple bytes.
For humidity, the program combines the required bits into a raw 20-bit value.
For temperature, it does the same thing with another 20-bit value.
The raw humidity value is converted using:
Humidity = raw_humidity / 1048576 × 100

## The raw temperature value is converted using:
## Temperature = raw_temperature / 1048576 × 200 - 50

## Challenges
One problem I ran into was connecting to the Raspberry Pi remotely.
Sometimes the hostname was not found immediately, so I learned how to check the network connection and use the Raspberry Pi's local IP address instead.
I also spent time understanding why the sensor appeared as 0x38, how the measurement command worked, and why the program needed to wait before reading the result.
