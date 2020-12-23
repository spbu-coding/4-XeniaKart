#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bmp.h"

void GetPxl(BMP_STRUCTURE* bmp, int32_t *x, int32_t *y, uint8_t *red, uint8_t *green, uint8_t *blue)
{
	uint8_t*	pixel;
	uint32_t	bytes_per_row;
	uint8_t		bytes_per_pixel;

	if ( bmp == NULL || *x  >= bmp->header.biWidth || *y >= bmp->header.biHeight)
	{
		return;
	}
	else
	{
		bytes_per_pixel = bmp->header.biBitCount >> 3;
		bytes_per_row = bmp->header.biSizeImage / bmp->header.biHeight;

		pixel = bmp->data + ((bmp->header.biHeight - *y - 1) * bytes_per_row + *x * bytes_per_pixel);

		if(bmp->palette != NULL)
		{
			pixel = bmp->palette + *pixel * 4;
		}
		if(red)	    *red = *(pixel + 2);
		if(green)	*green = *(pixel + 1);
		if(blue)	*blue = *(pixel + 0);
	}
}

int BMPComparer(char *filename1, char* filename2){

	BMP_STRUCTURE* 	bmp1;
	BMP_STRUCTURE* 	bmp2;

	bmp1 = (BMP_STRUCTURE*)malloc(sizeof(BMP_STRUCTURE));
	bmp2 = (BMP_STRUCTURE*)malloc(sizeof(BMP_STRUCTURE));

   	if (bmp1 == NULL && bmp2 == NULL)
	{
		fprintf(stderr, "Failed to allocate memory.\n");
		FreeBMP(bmp1);
		FreeBMP(bmp2);
		return -1;
	}
	
	int err = ReadBMP(bmp1, filename1);
	if (err != 0 ) { 
        free(bmp1);
        free(bmp2);
        return err;
    }
    err = ReadBMP(bmp2, filename2);
    if (err != 0){
    	FreeBMP(bmp1);
    	free(bmp2);
    	return err;
    }

	int countOfDifferentPxls = 0;

	if (bmp1->header.biWidth != bmp2->header.biWidth ||
		bmp1->header.biHeight != bmp2->header.biHeight ||
		bmp1->header.bfSize != bmp2->header.bfSize){
		fprintf(stderr, "The file sizes don't match.\n");
		FreeBMP(bmp1);
		FreeBMP(bmp2);
		return -1;
	}
	else{
        uint8_t red1, green1, blue1, red2, green2, blue2;
		for (int32_t x = 0; x < bmp1->header.biWidth; x++) {
            for (int32_t y = 0; y < bmp1->header.biHeight; y++){
            	GetPxl(bmp1, &x, &y, &red1, &green1, &blue1);
            	GetPxl(bmp2, &x, &y, &red2, &green2, &blue2);
            	if(red1 != red2 ||  green1 != green2 || blue1 != blue2) {
                    countOfDifferentPxls++;
                    fprintf(stderr, "%d %d\n", x, y);
                }
            	if (countOfDifferentPxls == 100){
                    FreeBMP(bmp1);
                    FreeBMP(bmp2);
                    return 1;
            	}
            }
        }
	}

    FreeBMP(bmp1);
    FreeBMP(bmp2);

	if(countOfDifferentPxls > 0){
		return 1;
	}
	else{
		return 0;
	}
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Use the format: %s <filename1>.bmp <filename2>.bmp\n", argv[0]);
        return -1;
    }

    return BMPComparer(argv[1], argv[2]);
}