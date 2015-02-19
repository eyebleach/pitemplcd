# PiWeather
=======

A simple, portable weather monitoring Pi for personal use.
Thanks to all people who have contributed to the Pi-community! Sharing is caring.

[How it looks](http://s27.postimg.org/5fbygsjsj/IMG_3087.jpg)

Use at your own risk!

### Material
* Raspberry Pi model B+ running Raspbian
* DS18B20 thermal probe
* LCD1602 IIC/I2C/TWI Serial LCD Display

### Part 1: The hardware setup

    sudo nano /etc/modules

Add i2c-dev, w1-gpio and w1-therm (if it isnt already there)

-------------

##### WARNING! This is for model B+, make sure you connect to the right pins!

Now shutdown your Raspberry Pi and connect the LCD and the Temperature sensor.

Google for examples if you are unsure.

Connect the LCD with the i2c-addon:
>SDA -> GPIO2 (pin 3)
>
>SCL -> GPIO3 (pin 5)
>
>VCC -> 5.0 V (pin 2)
>
>GND -> GROUND(pin 6)


Connect the tempsensor:
>POWER  -> 3.3 V (pin 1) bridged with data with a 4.7k ohm resistor
>
>DATA   -> GPIO4 (pin 7)
>
>GROUND -> GROUND(pin 9) 

-----------

Boot your Pi and install these two with following command:

    sudo apt-get install python-smbus i2c-tools -y

Add user `pi` to the `i2c`-group:

    sudo useradd -G i2c pi

Then reboot your Raspberry pi:

    sudo reboot

Run this command to check if your LCD is properly connected:

    sudo i2cdetect -y 1
    (sudo i2cdetect -y 0 for older model)

Sample output:

         0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
    00:          -- -- -- -- -- -- -- -- -- -- -- -- -- 
    10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    20: -- -- -- -- -- -- -- 27 -- -- -- -- -- -- -- -- 
    30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    70: -- -- -- -- -- -- -- --        

As the output shows, my LCD is connected on adress 0x27. This value will you have to insert into the code later on.


Run following commands to check if your temperature sensor is properly connected:

    cd /sys/bus/w1/devices/
    ls

cd into the directory with the sensors unique serial number and output from file w1_slave

    cd 20-003298324d4 (example)
    cat w1_slave

Output:

    70 01 55 00 7f ff 0c 10 7d : crc=7d YES
    70 01 55 00 7f ff 0c 10 7d t=23000

The numbers after the "t=" is your temperature in celsius (divide by 1000).


### Part 2: The software setup

I've used a library called `liblcm1602` for the display that is created by *[Wargio](https://github.com/wargio/liblcm1602)*.

Download i2c.c, i2c.h, lcd.c, lcd.h and put in the same directory as this application, then compile:

Make sure you edit ```templcd.c``` (line 24, 31) to match your system, then compile the C files.

    make
    sudo make install

Now schedule these programs to run every 3 min (or whatever you like)

   crontab -e

Add this to at the bottom line, make sure you enter the right paths:

    */3 * * * * templcd

Run the programs one time to init them, or wait for cron to run them.
