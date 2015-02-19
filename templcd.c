#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "i2c.h"
#include "lcd.h"

// Print temperature (or whatever you like) to LCD1602 IIC/I2C/TWI, with
// Raspberry Pi Model B+, using sensor DS18B20.
// Copyright 2014 Patrik Hoggren

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

///This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License along
//with this program; if not, write to the Free Software Foundation, Inc.,
//51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#define I2C_FILE_NAME "/dev/i2c-1" // "/dev/i2c-0" for older models
#define SENSOR "/sys/bus/w1/devices/28-00043c6a6eff/w1_slave" //DS18B20 sensor

int main() {
	int i2c_dev;
	lcd lcd0;

	i2c_dev = open_i2c(I2C_FILE_NAME, 0x27); // 0x27 is the address of the i2c device

	if(i2c_dev < 0) {
		printf("Error: %d\n", i2c_dev);
	return 0;
	}

	lcd_init(&lcd0, i2c_dev);
	lcd_clear(&lcd0);

	FILE* fp;
	int temperatureraw;
	double temperature;
	char buf[16];

	fp = fopen(SENSOR,"r");
	fseek(fp, 69, SEEK_SET);
	fscanf(fp,"%d", &temperatureraw);
	fclose(fp);

	temperature = (double)temperatureraw / 1000;

	sprintf(buf, "Temp:%.2fc", temperature);
	lcd_print(&lcd0, buf, strlen(buf), 0);

	close_i2c(i2c_dev);

return 0;
}
