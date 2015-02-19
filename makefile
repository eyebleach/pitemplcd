all:
	gcc i2c.c lcd.c templcd.c -o templcd

install:
	cp templcd /usr/bin/templcd
