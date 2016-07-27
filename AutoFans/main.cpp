//rootming@live.cn

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <wiringPi.h>
#include <softPwm.h>

using namespace std;

const int _FANS_PIN = 0;
const int _RANGE = 255;
const char *TEMPFILE = "/sys/class/thermal/thermal_zone0/temp";
const int _DELAY = 5000;

const double tempLevel[] = { 40.0, 50.0, 60.0, 70.0 };
const int fansLevel[] = { 80, 100, 180, 220 };

template <class T>  
T stringToNum(const string& str)  
{  
    istringstream iss(str);  
    T num;  
    iss >> num;  
    return num;      
}  

double getTemperature(void)
{   
    ifstream in;
	string temp;
	double temperature;
    in.open(TEMPFILE, ios::in);
	in >> temp;
	temperature = stringToNum<double>(temp) / 1000;
	cout << "CPU temperature:" << temperature << endl;
	return temperature;
}

void daemon(void)
{
	double temp;
	for(;;){
		temp = getTemperature();
		if(temp < tempLevel[0]){
			softPwmWrite (_FANS_PIN, _RANGE - fansLevel[0]);		//because I use PNP
			cout << "Fans speed level:1" << endl; 
		}else if(temp < tempLevel[1]){
			softPwmWrite (_FANS_PIN, _RANGE - fansLevel[1]);
			cout << "Fans speed level:2" << endl; 
		}else if(temp < tempLevel[2]){
			softPwmWrite (_FANS_PIN, _RANGE - fansLevel[2]);
			cout << "Fans speed level:3" << endl; 
		}else if(temp < tempLevel[3]){
			softPwmWrite (_FANS_PIN, _RANGE - fansLevel[3]);
			cout << "Fans speed level:4" << endl; 
		}else{
			softPwmWrite (_FANS_PIN, 0);
			cout << "Fans speed level:5" << endl; 
		}
		// std::this_thread::sleep_for (std::chrono::seconds(1));
		delay(_DELAY);
	}

}

int main()
{
	
    if(-1 == wiringPiSetup()){
		cerr << "WiringPi init failed." << endl;
		exit(1);
	}
	softPwmCreate(_FANS_PIN, _RANGE, _RANGE);
	//softPwmWrite(_FANS_PIN, _RANGE);
	daemon();
	return 0;
}
