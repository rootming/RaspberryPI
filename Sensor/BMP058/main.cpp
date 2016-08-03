#include <iostream>
#include <unistd.h>
#include "bmp085.h"


using namespace std;

int main(int argc, char *argv[])
{
    BMP085 bmp(BMP085_MODE_ULTRAHIGHRES);
    for(;;){
        cout << "Temperature:" << bmp.getTemperature() << endl;
        cout << "Pressure:" << bmp.getPressure() << endl;
        sleep(1);
    }
    return 0;
}
