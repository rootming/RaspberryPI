#pragma once

#include <iostream>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <queue>
#include <cstring>
#include <math.h>

using namespace std;

struct Pos_t
{
    uint32_t px, py;    //physical postion
    uint32_t lx, ly;    //logic postion
    Pos_t() {}
    Pos_t(uint32_t x, uint32_t y) : px(x), py(y), lx(x), ly(y) { }
};

void partImage(int count);
void drawUnit(Pos_t &pos);
void calcUnitPosition(Pos_t &start);
void drawThread(int num);


class Image
{
public:
    Image(uint32_t w, uint32_t h);
    void save(string filename);

protected:
    uint32_t width, height;
    uint64_t size;
    char *image;
private:
    void copyFromArray(Image &);

};

class Draw: public Image
{
public:
    Draw(uint32_t w = 9600, uint32_t h = 9600): Image(w, h)
    {
        logicalX = 0;
        logicalY = 0;
        setUnitLength();
        setThreadNumber();
    }
    void setThreadNumber(int count = 2){ threadCount = count; }
    void setUnitLength(uint32_t length = 128){ unitLength = length; }
    void setLogicalPost(uint32_t x, uint32_t y){ logicalX = x, logicalY = y; }
    void calcUnitPosition(queue <Pos_t> *list = nullptr,
                          uint32_t w = 0, uint32_t h = 0, uint32_t unitLineLength = 0,
                          uint32_t loX = 0, uint32_t loY = 0
            );
    void start();

private:

    mutex mtx;
//    int num;
    int threadCount;
    uint32_t unitLength;
    uint32_t logicalX, logicalY;
    queue <Pos_t> posList;
    thread *threads;
    uint32_t drawPixel(uint32_t i, uint32_t j);
    void drawUnit(Pos_t &pos);
    void drawThread(int num);




};
