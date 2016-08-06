#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <assert.h>
#include <linux/videodev2.h>
#include <errno.h>
#include "video.h"

using namespace std;

extern int errno;

static unsigned int n_buffer = 0;  

Video::~Video()
{
    


}

int Video::openDevice()
{
    if((fd = open(device.c_str(), O_RDWR | O_NONBLOCK, 0)) == -1){
        cerr << "Open device error.\n";
        exit(1);
    }
    return fd;
}

bool Video::initDevice()
{

    struct v4l2_capability cap;  
    //video standard,such as PAL,NTSC  
//    struct v4l2_standard std;  
    //frame format  
    struct v4l2_format tv_fmt;  
    //check control  
//    struct v4l2_queryctrl query;  
    //detail control value  
    struct v4l2_fmtdesc fmt;  
    int ret;  
    //get the format of video supply  
    memset(&fmt, 0, sizeof(fmt));  
    fmt.index = 0;  
    //supply to image capture  
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    // show all format of supply  
    cout << "Support format:" << endl;  
    while(ioctl(fd, VIDIOC_ENUM_FMT, &fmt) == 0){  
        fmt.index++;  
        cout << "pixelformat = " << (fmt.pixelformat & 0xFF);
        cout <<  ((fmt.pixelformat >> 8) & 0xFF); 
        cout << ((fmt.pixelformat >> 16) & 0xFF); 
        cout << (fmt.pixelformat >> 24) << endl;
        cout << "description = " <<  fmt.description << endl;  
    }  
    //check video decive driver capability  
    ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);  
    if(ret < 0){  
        cerr << "Fail to ioctl VIDEO_QUERYCAP" << endl;
        exit(EXIT_FAILURE);  
    }  

    //judge wherher or not to be a video-get device  
    if(!(cap.capabilities & V4L2_BUF_TYPE_VIDEO_CAPTURE))  
    {  
        cerr <<  "The Current device is not a video capture device\n";  
        exit(-1);  
    }  

    //judge whether or not to supply the form of video stream  
    if(!(cap.capabilities & V4L2_CAP_STREAMING))  
    {  
        cerr  << "The Current device does not support streaming i/o\n";  
        exit(EXIT_FAILURE);  
    }  

    //set the form of camera capture data  
    tv_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    tv_fmt.fmt.pix.width = width;  
    tv_fmt.fmt.pix.height = height;  
    tv_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    tv_fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;  
    if (ioctl(fd, VIDIOC_S_FMT, &tv_fmt) < 0) {  
        cerr << "VIDIOC_S_FMT\n";  
        exit(-1);  
        close(fd);  
    }  
    return 0;  

}

int Video::initMmap()  
{  
    //to request frame cache, contain requested counts  
    struct v4l2_requestbuffers reqbufs;  
    //request V4L2 driver allocation video cache  
    //this cache is locate in kernel and need mmap mapping  
    memset(&reqbufs, 0, sizeof(reqbufs));  
    reqbufs.count = 4;  
    reqbufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    reqbufs.memory = V4L2_MEMORY_MMAP;  

    if(-1 == ioctl(fd, VIDIOC_REQBUFS, &reqbufs)){  
        cerr << "Fail to ioctl 'VIDIOC_REQBUFS'" << endl;  
        exit(EXIT_FAILURE);  
    }  

    n_buffer = reqbufs.count;  
    cerr << "n_buffer = " <<  n_buffer << endl;  
    usr_buf = (BUFTYPE *)calloc(reqbufs.count, sizeof(usr_buf));  
    if(usr_buf == NULL){  
        cerr << "Out of memory\n";  
        exit(-1);  
    }  

    //map kernel cache to user process   
    for(n_buffer = 0; n_buffer < reqbufs.count; ++n_buffer){  
        //stand for a frame  
        struct v4l2_buffer buf;  
        memset(&buf, 0, sizeof(buf));  
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
        buf.memory = V4L2_MEMORY_MMAP;  
        buf.index = n_buffer;  

        //check the information of the kernel cache requested   
        if(-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf)){  
            cerr << "Fail to ioctl : VIDIOC_QUERYBUF\n";  
            exit(EXIT_FAILURE);  
        }  

        usr_buf[n_buffer].length = buf.length;  
        usr_buf[n_buffer].start = (char *)mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);

        if(MAP_FAILED == usr_buf[n_buffer].start){  
            cerr << "Fail to mmap, code is:" << errno << endl ;
            exit(EXIT_FAILURE);  
        }  
    }  
    return 0;  
}  

void Video::getDevInfo()
{
    struct v4l2_capability cap;
    int ret;
    ret = ioctl(fd,VIDIOC_QUERYCAP,&cap);
    if(ret != 0){
        cerr << "Get device info error." << endl;
    }

    cout << "Driver Name:" << cap.driver << endl;
    cout << "Card Name:" << cap.card << endl;
    cout << "Bus info:" << cap.bus_info << endl;

}
int Video::startCapture()  
{  
    unsigned int i;  
    enum v4l2_buf_type type;  
    //place the kernel cache to a queue  
    for(i = 0; i < n_buffer; i++){  
        struct v4l2_buffer buf;  
        memset(&buf, 0, sizeof(buf));  
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
        buf.memory = V4L2_MEMORY_MMAP;  
        buf.index = i;  

        if(-1 == ioctl(fd, VIDIOC_QBUF, &buf)){  
            cerr << "Fail to ioctl 'VIDIOC_QBUF'" << endl;  
            exit(EXIT_FAILURE);  
        }  
    }  

    //start capture data  
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    if(-1 == ioctl(fd, VIDIOC_STREAMON, &type)){  
        cout << "i=" << i << endl;  
        cerr << "VIDIOC_STREAMON" << endl;  
        close(fd);  
        exit(EXIT_FAILURE);  
    }  
    return 0;  
}  

int Video::processImage(void *addr, int length)  
{  
    FILE *fp;  
    static int num = 0;  
    char image_name[20];  

    sprintf(image_name, _IMAGE, num++);  
    if((fp = fopen(image_name, "w")) == NULL){  
        cerr << "Fail to fopen" << endl;  
        exit(EXIT_FAILURE);  
    }  
    fwrite(addr, length, 1, fp);  
    usleep(500);  
    fclose(fp);  
    return 0;  
}  
   
int Video::readFrame()  
{  
    struct v4l2_buffer buf;  
//    unsigned int i;  
    memset(&buf, 0, sizeof(buf));  
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    buf.memory = V4L2_MEMORY_MMAP;  
    //put cache from queue  
    if(-1 == ioctl(fd, VIDIOC_DQBUF, &buf)){
        cerr << "Fail to ioctl 'VIDIOC_DQBUF'" << endl;  
        exit(EXIT_FAILURE);  
    }  

    assert(buf.index < n_buffer);  
    //read process space's data to a file  
    processImage(usr_buf[buf.index].start, usr_buf[buf.index].length);  
    if(-1 == ioctl(fd, VIDIOC_QBUF,&buf)){  
        cerr << "Fail to ioctl 'VIDIOC_QBUF'" << endl;  
        exit(EXIT_FAILURE);  
    }  
    return 1;  
}  

void Video::stopCapture()  
{  
    enum v4l2_buf_type type;  
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    if(-1 == ioctl(fd,VIDIOC_STREAMOFF, &type))  
    {  
        cerr << "Fail to ioctl 'VIDIOC_STREAMOFF'" << endl;  
        exit(EXIT_FAILURE);  
    }  
}  
   
void Video::closeDevice()  
{  
    unsigned int i;  
    for(i = 0;i < n_buffer; i++){  
        if(-1 == munmap(usr_buf[i].start, usr_buf[i].length)){  
            exit(-1);  
        }  
    }  

    free(usr_buf);  

    if(-1 == close(fd)){  
        cerr << "Fail to close fd" << endl;  
        exit(EXIT_FAILURE);  
    }  
    return;  
}  

void Video::start(int w, int h)
{
    width = w;
    height = h;
    openDevice();
    getDevInfo();
    initDevice();
    initMmap();  
    startCapture();
    th = new thread([=]() {
            int count = 10;  
            while(count-- > 0){  
                for(;;){  
                    fd_set fds;  
                    struct timeval tv;  
                    int r;  

                    FD_ZERO(&fds);  
                    FD_SET(fd,&fds);  

                    /*Timeout*/  
                    tv.tv_sec = 2;  
                    tv.tv_usec = 0;  
                    r = select(fd + 1,&fds,NULL,NULL,&tv);  

                    if(-1 == r){  
                        if(EINTR == errno)  
                            continue;  
                        perror("Fail to select");  
                        exit(EXIT_FAILURE);  
                    }  

                    if(0 == r){  
                        fprintf(stderr,"select Timeout\n");  
                        exit(-1);  
                    }  

                    if(readFrame())  
                        break;  
                }  
            }  
    });

}

void Video::stop()
{
    th->join();
    stopCapture();
    closeDevice();
}

