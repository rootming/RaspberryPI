#include <iostream>
#include <inttypes.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

using namespace std;

#define BMP085_ADDRESS                (0x77)

/*=========================================================================s*/
enum {
    BMP085_REGISTER_CAL_AC1            = 0xAA,  // R   Calibration data (16 bits)
    BMP085_REGISTER_CAL_AC2            = 0xAC,  // R   Calibration data (16 bits)
    BMP085_REGISTER_CAL_AC3            = 0xAE,  // R   Calibration data (16 bits)
    BMP085_REGISTER_CAL_AC4            = 0xB0,  // R   Calibration data (16 bits)
    BMP085_REGISTER_CAL_AC5            = 0xB2,  // R   Calibration data (16 bits)
    BMP085_REGISTER_CAL_AC6            = 0xB4,  // R   Calibration data (16 bits)
    BMP085_REGISTER_CAL_B1             = 0xB6,  // R   Calibration data (16 bits)
    BMP085_REGISTER_CAL_B2             = 0xB8,  // R   Calibration data (16 bits)
    BMP085_REGISTER_CAL_MB             = 0xBA,  // R   Calibration data (16 bits)
    BMP085_REGISTER_CAL_MC             = 0xBC,  // R   Calibration data (16 bits)
    BMP085_REGISTER_CAL_MD             = 0xBE,  // R   Calibration data (16 bits)
    BMP085_REGISTER_CHIPID             = 0xD0,
    BMP085_REGISTER_VERSION            = 0xD1,
    BMP085_REGISTER_SOFTRESET          = 0xE0,
    BMP085_REGISTER_CONTROL            = 0xF4,
    BMP085_REGISTER_TEMPDATA           = 0xF6,
    BMP085_REGISTER_PRESSUREDATA       = 0xF6,
    BMP085_REGISTER_READTEMPCMD        = 0x2E,
    BMP085_REGISTER_READPRESSURECMD    = 0x34
};
/*=========================================================================*/

/*=========================================================================
    MODE SETTINGS
    -----------------------------------------------------------------------*/
typedef enum
{
    BMP085_MODE_ULTRALOWPOWER          = 0,
    BMP085_MODE_STANDARD               = 1,
    BMP085_MODE_HIGHRES                = 2,
    BMP085_MODE_ULTRAHIGHRES           = 3
} bmp085_mode_t;
/*=========================================================================*/

/*=========================================================================
    CALIBRATION DATA
    -----------------------------------------------------------------------*/
typedef struct
{
    int16_t  ac1;
    int16_t  ac2;
    int16_t  ac3;
    uint16_t ac4;
    uint16_t ac5;
    uint16_t ac6;
    int16_t  b1;
    int16_t  b2;
    int16_t  mb;
    int16_t  mc;
    int16_t  md;
} bmp085_calib_data;
/*=========================================================================*/

class BMP085
{
public:
    BMP085(bmp085_mode_t mode);
    ~BMP085(){}
    float getTemperature(void);
    float getPressure(void);
    float pressureToAltitude(float seaLvel, float atmospheric);
    float seaLevelForAltitude(float altitude, float atmospheric);
    // Note that the next two functions are just for compatibility with old
    // code that passed the temperature as a third parameter.  A newer
    // calculation is used which does not need temperature.
    float pressureToAltitude(float seaLevel, float atmospheric, float temp);
    float seaLevelForAltitude(float altitude, float atmospheric, float temp);

private:
    int32_t computeB5(int32_t ut);
    int32_t _sensorID;

};
