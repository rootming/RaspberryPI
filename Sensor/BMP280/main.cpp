/***************************************************************************
  This is a library for the BMP280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BMEP280 Breakout 
  ----> http://www.adafruit.com/products/2651

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required 
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#include <iostream>
#include <unistd.h>
#include "BMP280.h"

using namespace std;


BMP280 bmp; // I2C

int main(void) 
{
	if(!bmp.begin()) {
		cout << "Init fail" << endl;
	}
while(1) {
	cout << "Tempertature: " << (bmp.readTemperature() - 32) / 1.8 << "*C\n"; 
	cout << "Pressure: " << bmp.readPressure() << "\n";
	cout << "Alt: " << bmp.readAltitude(1013.25) << "\n";
	sleep(1);
}
	return 0;
}
