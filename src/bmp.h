#pragma once 

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> 

typedef struct BMP_HEADER {
    //BITMAPFILEHEADER
    uint16_t 	bfType;
    uint32_t 	bfSize;
    uint16_t 	bfReserved1;
    uint16_t	bfReserved2;
    uint32_t 	bfOffBits;
    //BITMAPINFOHEADER
    uint32_t 	biSize;
    int32_t		biWidth;
    int32_t 	biHeight;
    uint16_t 	biPlanes;
    uint16_t	biBitCount;
    uint32_t 	biCompression;
    uint32_t 	biSizeImage;
    int32_t 	biXPelsPerMeter;
    int32_t 	biYPelsPerMeter;
    uint32_t 	biClrUsed;
    uint32_t 	biClrImportant;

} 	BMP_HEADER;

typedef struct BMP_STRUCTURE {
    BMP_HEADER		header;
    uint8_t*		palette;
    uint8_t*		data;
} 	BMP_STRUCTURE;

void FreeBMP(BMP_STRUCTURE* bmp);
int ReadBMP(BMP_STRUCTURE* bmp, char* filename);
void WriteBMP(BMP_STRUCTURE* bmp, char* new_filename);

