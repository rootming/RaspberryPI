#pragma once
#include <iostream>
#include <thread>
using namespace std;

#define _DEF_DEV "/dev/video0"
#define _IMAGE   "image%d"

class Video
{
    public:
        Video(const char *dev = _DEF_DEV): device(dev){}
        void start(int w, int h);
        void stop();
        ~Video();
    private:
        int fd;
        int width, height;
        string device;
        thread *th;

        typedef struct{  
            void *start;        
            int length;         
        }BUFTYPE;                   

        BUFTYPE *usr_buf;           

        void getDevInfo();
        int openDevice();
        void closeDevice();
        int initMmap();    
        bool initDevice();
        int startCapture();
        void stopCapture();
        int processImage(void *addr, int length);
        int readFrame();


};
