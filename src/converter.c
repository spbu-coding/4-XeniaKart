#include "bmp.h"
#include "qdbmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINE_LENGTH	 6
#define THEIRS_LENGTH 8

int mine_implementation (char* filename, char* new_filename) {
	BMP_STRUCTURE* bmp;
	bmp = (BMP_STRUCTURE*)malloc(sizeof(BMP_STRUCTURE));
   	if (bmp == NULL)
	{
		fprintf(stderr, "Failed to allocate memory.\n");
		return -1;
	}

	int err = ReadBMP(bmp, filename);
	if(err != 0){
		FreeBMP(bmp);
		return err;
	}

	if (bmp->header.biBitCount == 24) {
        for (unsigned int i = 0; i < bmp->header.biSizeImage; i++) {
            bmp->data[i] = ~bmp->data[i];
        }
    } else {
        for (unsigned int i = 0; i < bmp->header.biClrUsed * 4; i++) {
            bmp->palette[i] = ~bmp->palette[i];
        }
    }

	WriteBMP(bmp, new_filename);
	FreeBMP(bmp);
	return 0;
}

int theirs_implementation (char* filename, char* new_filename) {
	UCHAR	r, g, b;
	UINT	width, height;
	UINT	x, y;
	BMP*	bmp;

	bmp = BMP_ReadFile(filename);
	BMP_CHECK_ERROR(stdout, -3);

	width = BMP_GetWidth(bmp);
	height = BMP_GetHeight(bmp);
	for (x = 0 ; x < width ; ++x)
	{
		for (y = 0 ; y < height ; ++y)
		{
			BMP_GetPixelRGB(bmp, x, y, &r, &g, &b);
			BMP_SetPixelRGB(bmp, x, y, ~r, ~g, ~b);
		}
	}
	BMP_WriteFile(bmp, new_filename);
	BMP_CHECK_ERROR(stdout, -3);

	BMP_Free(bmp);
	return 0;
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
        fprintf(stderr, "Use the format: %s --mine/--theirs <input file>.bmp <output file>.bmp\n", argv[0]);
    	return -1;
    }

    if(strncmp(argv[1], "--mine", MINE_LENGTH) == 0) {  
        return mine_implementation (argv[2], argv[3]);
    }
    else if(strncmp(argv[1], "--theirs", THEIRS_LENGTH) == 0) { 
        return theirs_implementation (argv[2], argv[3]);
    } else {
        fprintf(stderr, "Incorrect parameters entered. Use the format: %s --mine/--theirs <input file>.bmp <output file>.bmp\n", argv[0]);
        return -1;
    }
}