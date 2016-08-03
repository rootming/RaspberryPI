#include "ms5611.h"
#include <iostream>
#include <unistd.h>

using namespace std;

int main(void)
{
    MS5611 sensor;
    for(;;){
        cout << "Pressure:" << sensor.readPressure() << endl;
        cout << "Temperature:" << sensor.readTemperature() << endl;
        sleep(1);
    }
    return 0;
}
