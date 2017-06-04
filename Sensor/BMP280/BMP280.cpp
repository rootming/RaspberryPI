#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <sys/ioctl.h>  
#include <fcntl.h>  
#include <errno.h> 
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <iostream>
#include <math.h>
#include "BMP280.h"
using namespace std;
int fd;
uint16_t chip_addr;

int i2c_Read(int fd, uint16_t chip_addr, uint8_t *writebuf,  
            uint16_t writelen, uint8_t *readbuf, uint16_t readlen)  
{  
    int ret;  
    struct i2c_rdwr_ioctl_data ioctl_data;  
    if (writelen > 0) {  
        struct i2c_msg msgs[] = {  
            {  
             .addr = chip_addr,  
             .flags = 0,//写  
             .len = writelen,  
             .buf = writebuf,  
             },  
            {  
             .addr = chip_addr,  
             .flags = I2C_M_RD,//读  
             .len = readlen,  
             .buf = readbuf,  
             },  
        };  
  
        ioctl_data.nmsgs = 1;  
        ioctl_data.msgs  = &msgs[0];  
    //  ret = i2c_transfer(client->adapter, msgs, 2);  
        ret = ioctl(fd,I2C_RDWR,&ioctl_data);  
        if (ret < 0)  
            printf("f%s: i2c read error.\n",__func__);  
  
        ioctl_data.nmsgs = 1;  
        ioctl_data.msgs  = &msgs[1];  
    //  ret = i2c_transfer(client->adapter, msgs, 2);  
        ret = ioctl(fd,I2C_RDWR,&ioctl_data);  
        if (ret < 0)  
            printf("f%s: i2c read error.\n",__func__);  
    } else {  
        struct i2c_msg msgs[] = {  
            {  
             .addr = chip_addr,  
             .flags = I2C_M_RD,  
             .len = readlen,  
             .buf = readbuf,  
             },  
        };  
  
        ioctl_data.nmsgs = 1;  
        ioctl_data.msgs  = msgs;  
          
    //  ret = i2c_transfer(client->adapter, msgs, 1);  
        ret = ioctl(fd,I2C_RDWR,&ioctl_data);  
        if (ret < 0)  
            printf("f%s: i2c read error.\n",__func__);  
        }  
    return ret;  
}  
  
  
int i2c_Write(int fd, uint16_t chip_addr, uint8_t *writebuf, uint16_t writelen)  
{  
    int ret;  
  
    struct i2c_rdwr_ioctl_data ioctl_data;  
    struct i2c_msg msg[] = {  
        {  
         .addr = chip_addr,  
         .flags = 0,  
         .len = writelen,  
         .buf = writebuf,  
         },  
    };  
  
    ioctl_data.nmsgs = 1;  
    ioctl_data.msgs  = msg;  
  
//  ret = i2c_transfer(client->adapter, msgs, 1);  
    ret = ioctl(fd,I2C_RDWR,&ioctl_data);  
  
    if (ret < 0)  
        printf("f%s: i2c write error.\n",__func__);  
  
    return ret;  
}  
  
int read_reg8(uint8_t regaddr, uint8_t *regvalue)  
{  
    return i2c_Read(fd, chip_addr, &regaddr, 1, regvalue, 1);  
} 
  

void read_reg16(uint8_t regaddr, uint16_t *regvalue)  
{  
	read_reg8(regaddr, reinterpret_cast<uint8_t*>(regvalue));
	read_reg8(regaddr + 1, reinterpret_cast<uint8_t*>(regvalue) + 1);
} 

void read_reg24(uint8_t regaddr, uint32_t *regvalue)  
{  
	read_reg8(regaddr, reinterpret_cast<uint8_t*>(regvalue));
	read_reg8(regaddr + 1, reinterpret_cast<uint8_t*>(regvalue) + 1);
	read_reg8(regaddr + 2, reinterpret_cast<uint8_t*>(regvalue) + 2);
} 

int write_reg(uint8_t regaddr, uint8_t regvalue)  
{  
    unsigned char buf[2] = {0};  
    buf[0] = regaddr;  
    buf[1] = regvalue;  
      
    return i2c_Write(fd, chip_addr, buf, sizeof(buf));  
}  

/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/


BMP280::BMP280()
{ }


bool BMP280::begin(uint8_t a, uint8_t chipid) {

// open i2c device
  chip_addr = a;

  if ((fd = open("/dev/i2c-1",O_RDWR))< 0) {
        cerr << "I2C init error." << endl;
  }

// set slave address
  ioctl(fd, I2C_SLAVE_FORCE, a);

// read id
  uint8_t id = read8(BMP280_REGISTER_CHIPID);
  if (id != chipid) {
	printf("Read id:%#x, but not match with: %#x\n", id, chipid);
    return false;
  }
  printf("Read id:%#x\n", id);

  readCoefficients();
  write8(BMP280_REGISTER_CONTROL, 0x3F);
  return true;
}


/**************************************************************************/
/*!
    @brief  Writes an 8 bit value over I2C/SPI
*/
/**************************************************************************/
void BMP280::write8(uint8_t reg, uint8_t value)
{
	write_reg(reg, value);
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C/SPI
*/
/**************************************************************************/
uint8_t BMP280::read8(uint8_t reg)
{
	uint8_t value;
	read_reg8(reg, &value);
	return value;
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit value over I2C/SPI
*/
/**************************************************************************/
uint16_t BMP280::read16(uint8_t reg)
{
	uint16_t value;
	read_reg16(reg, &value);
	return value;
}

uint16_t BMP280::read16_LE(uint8_t reg) {
  uint16_t temp = read16(reg);
  return (temp >> 8) | (temp << 8);
}

/**************************************************************************/
/*!
    @brief  Reads a signed 16 bit value over I2C/SPI
*/
/**************************************************************************/
int16_t BMP280::readS16(uint8_t reg)
{
  return (int16_t)read16(reg);
}

int16_t BMP280::readS16_LE(uint8_t reg)
{
  return (int16_t)read16_LE(reg);
}


/**************************************************************************/
/*!
    @brief  Reads a 24 bit value over I2C/SPI
*/
/**************************************************************************/
uint32_t BMP280::read24(uint8_t reg)
{
  uint32_t value;
  read_reg24(reg, &value);
  return value;
}

/**************************************************************************/
/*!
    @brief  Reads the factory-set coefficients
*/
/**************************************************************************/
void BMP280::readCoefficients(void)
{
    _bmp280_calib.dig_T1 = read16_LE(BMP280_REGISTER_DIG_T1);
    _bmp280_calib.dig_T2 = readS16_LE(BMP280_REGISTER_DIG_T2);
    _bmp280_calib.dig_T3 = readS16_LE(BMP280_REGISTER_DIG_T3);

    _bmp280_calib.dig_P1 = read16_LE(BMP280_REGISTER_DIG_P1);
    _bmp280_calib.dig_P2 = readS16_LE(BMP280_REGISTER_DIG_P2);
    _bmp280_calib.dig_P3 = readS16_LE(BMP280_REGISTER_DIG_P3);
    _bmp280_calib.dig_P4 = readS16_LE(BMP280_REGISTER_DIG_P4);
    _bmp280_calib.dig_P5 = readS16_LE(BMP280_REGISTER_DIG_P5);
    _bmp280_calib.dig_P6 = readS16_LE(BMP280_REGISTER_DIG_P6);
    _bmp280_calib.dig_P7 = readS16_LE(BMP280_REGISTER_DIG_P7);
    _bmp280_calib.dig_P8 = readS16_LE(BMP280_REGISTER_DIG_P8);
    _bmp280_calib.dig_P9 = readS16_LE(BMP280_REGISTER_DIG_P9);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
float BMP280::readTemperature(void)
{
  int32_t var1, var2;

  int32_t adc_T = read24(BMP280_REGISTER_TEMPDATA);
  adc_T >>= 4;

  var1  = ((((adc_T>>3) - ((int32_t)_bmp280_calib.dig_T1 <<1))) *
	   ((int32_t)_bmp280_calib.dig_T2)) >> 11;

  var2  = (((((adc_T>>4) - ((int32_t)_bmp280_calib.dig_T1)) *
	     ((adc_T>>4) - ((int32_t)_bmp280_calib.dig_T1))) >> 12) *
	   ((int32_t)_bmp280_calib.dig_T3)) >> 14;

  t_fine = var1 + var2;

  float T  = (t_fine * 5 + 128) >> 8;
  return T/100;
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
float BMP280::readPressure(void) {
  int64_t var1, var2, p;

  // Must be done first to get the t_fine variable set up
  readTemperature();

  int32_t adc_P = read24(BMP280_REGISTER_PRESSUREDATA);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)_bmp280_calib.dig_P6;
  var2 = var2 + ((var1*(int64_t)_bmp280_calib.dig_P5)<<17);
  var2 = var2 + (((int64_t)_bmp280_calib.dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)_bmp280_calib.dig_P3)>>8) +
    ((var1 * (int64_t)_bmp280_calib.dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)_bmp280_calib.dig_P1)>>33;

  if (var1 == 0) {
    return 0;  // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p<<31) - var2)*3125) / var1;
  var1 = (((int64_t)_bmp280_calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((int64_t)_bmp280_calib.dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)_bmp280_calib.dig_P7)<<4);
  return (float)p/256;
}

float BMP280::readAltitude(float seaLevelhPa) {
  float altitude;

  float pressure = readPressure(); // in Si units for Pascal
  pressure /= 100;

  altitude = 44330 * (1.0 - pow(pressure / seaLevelhPa, 0.1903));

  return altitude;
}
