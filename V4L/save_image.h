#pragma once
#include <inttypes.h>


typedef struct tagBITMAPFILEHEADER{
      int16_t     bfType;                // the flag of bmp, value is "BM"
      int32_t    bfSize;                // size BMP file ,unit is bytes
      int32_t    bfReserved;            // 0
      int32_t    bfOffBits;             // must be 54

 }BITMAPFILEHEADER;


 typedef struct tagBITMAPINFOHEADER{
      int32_t    biSize;                // must be 0x28
      int32_t    biWidth;           //
      int32_t    biHeight;          //
      int16_t    biPlanes;          // must be 1
      int16_t    biBitCount;            //
      int32_t    biCompression;         //
      int32_t    biSizeImage;       //
      int32_t    biXPelsPerMeter;   //
      int32_t    biYPelsPerMeter;   //
      int32_t    biClrUsed;             //
      int32_t    biClrImportant;        //
 }BITMAPINFOHEADER;

void saveBMP(unsigned char *buffer, int w, int h, const char *filename);
void savePPM(unsigned char *buffer, int w, int h, const char *filename);
