#include <stdio.h>
#include <iostream>
#include <fstream>
#include "save_image.h"

void saveBMP(unsigned char *buffer, int w, int h, const char *filename)
{
    BITMAPFILEHEADER   bf;
    BITMAPINFOHEADER   bi;


    FILE *image = fopen(filename, "wb");
    if(!image){
        printf("open BMP error\n");
    }


    //Set BITMAPINFOHEADER
    bi.biSize = 40;
    bi.biWidth = w;
    bi.biHeight = h;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = 0;
    bi.biSizeImage = w * h * 3;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;


    //Set BITMAPFILEHEADER
    bf.bfType = 0x4d42;
    bf.bfSize = 54 + bi.biSizeImage;
    bf.bfReserved = 0;
    bf.bfOffBits = 54;

    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, image);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, image);
    fwrite(buffer, bi.biSizeImage, 1, image);
    fclose(image);
    printf("save done.\n");

}
void savePPM(unsigned char *buffer, int w, int h, const char *filename)
{
    using namespace std;
     ofstream file(filename, ios::binary | ios::trunc | ios::out);
     file << "P6\n" << w << " "<< h << "\n255\n";
     cout <<  "Target image pixel is :" <<  w * h << endl;
     file.write((const char *)buffer, w * h * 3);
     file.close();
}
