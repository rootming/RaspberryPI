//rootming@live.cn
#include "draw.h"
#include <cstdlib>


Image::Image(uint32_t w, uint32_t h): width(w), height(h), size(w * h *3)
{
    image = new char[size];
    if(image == nullptr){
        cerr << "Memory Alloc error.\n";
        exit(1);
    }

}

void Image::save(string filename)
{
    ofstream file(filename.c_str(), ios::binary | ios::trunc | ios::out);
    file << "P6\n" << width << " "<< height << "\n255\n";
    cout <<  "Target image pixel is :" <<  size / 3 << endl;
    file.write(image, size);

}


uint32_t Draw::drawPixel(uint32_t i, uint32_t j)
{
    uint32_t tmp;
    double a = 0, b = 0, c = 0, d = 0, n = 0;
    while ((c = a * a) + (d = b * b) < 4 && n++ < 880) {
        b = 2 * a * b + j * 8e-9 - .645411;
        a = c - d + i * 8e-9 + .356888;
    }
    tmp = (uint32_t)(255 * pow((n - 80) / 800, 3.));
    tmp |= (uint32_t)(255 * pow((n - 80) / 800, .7)) << 8;
    tmp |= (uint32_t)(255 * pow((n - 80) / 800, .5)) << 16;
    return tmp;
}

void Draw::drawUnit(Pos_t &pos)
{
    uint32_t startX, startY, endX, endY;
    uint32_t x, y;
    uint32_t lx, ly;
    startX = pos.px;
    startY = pos.py;
//    lx = pos.lx;
//    ly = pos.ly;
    uint32_t pixel;
    endX = width > startX + unitLength ? startX + unitLength : width;
    endY = height > startY + unitLength ? startY + unitLength : height;
    for (y = startY, ly = pos.ly; y < endY; y++, ly++) {
        for (x = startX, lx = pos.lx; x < endX; x++, lx++) {
            pixel = drawPixel(lx, ly);
            memcpy(image + 3 * (width * y + x), &pixel, 3);
        }
    }

}

void Draw::calcUnitPosition(queue <Pos_t> *list,
                            uint32_t w, uint32_t h, uint32_t unitLineLength,
                            uint32_t loX, uint32_t loY)
{
    uint32_t tw, th, tl, tlx, tly;
    queue <Pos_t> *tlist;
    if(list == nullptr){
        tw = width;
        th = height;
        tl = unitLength;
        tlist = &posList;
        tlx = logicalX;
        tly = logicalY;
    }else{
        tw = w;
        th = h;
        tl = unitLineLength;
        tlist = list;
        tlx = loX;
        tly = loY;
    }
    cout << "Calculating postion...\n";
    uint32_t xCount = tw % tl ? tw / tl + 1 : tw / tl;
    uint32_t yCount = th % tl ? th / tl + 1 : th / tl;
    Pos_t temp;
    for (uint32_t i = 0; i < xCount; i++) {
        for (uint32_t j = 0; j < yCount; j++) {
            temp.px = i * tl;
            temp.py = j * tl;
            temp.lx = i * tl + tlx;
            temp.ly = j * tl + tly;
            tlist->push(temp);
        }
    }
    cout << "Calculate finish...\n";
}

void Draw::drawThread(int num)
{
    Pos_t temp;
    this_thread::sleep_for(std::chrono::milliseconds(num * 10));
    cout << "Thread:" << num << " is running..." << endl;
    for (;;) {
        mtx.lock();
        if (posList.empty()) {
            mtx.unlock();
            break;
        }
        temp = posList.front();
        posList.pop();
        mtx.unlock();
        drawUnit(temp);
    }
    this_thread::sleep_for(std::chrono::milliseconds(num * 10));
    cout << "Thread:" << num << " finished." << endl;
}

void Draw::start()
{
    if(posList.empty()){
        cerr << "Need calc Postition first." << endl;
        return;
    }
    threads = new thread[threadCount];
    for (int i = 0; i < threadCount; i++) {
            threads[i] = thread([=]{ drawThread(i); });
    }
    for(int i = 0; i < threadCount; i++)
            threads[i].join();

}

//int main()
//{
//    time_t start, write;

//    thread threads[_THREAD_LIMIT];

//    image = new char[_TARGET_SIDE * _TARGET_SIDE * 3];
//    if (image == NULL) {
//        cerr << "Alloc memory errror.\n";
//        exit(1);
//    }

//    calcUnitPosition();
//    start = time(NULL);
//    for (int i = 0; i < _THREAD_LIMIT; i++) {
//        threads[i] = thread(drawThread, i);
//    }

//    for (auto& it : threads)
//        it.join();

//    write = time(NULL);
//    cout << "Render ended, using " << write - start << "s." << endl;

//    cout << "Write ended, using " << time(NULL) - write << "s." << endl;
//    cin.get();
//    delete []image;
//    file.close();
//}
