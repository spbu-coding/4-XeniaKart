#include "bmp.h"

#define PRINT_ERR(err, ...) {fprintf(stderr, __VA_ARGS__); \
                            free(bmp); \
                            fclose(input_file); \
                            return err; }

int ReadBMP(BMP_STRUCTURE* bmp, char* filename) {

	FILE*	input_file;

    if (filename == NULL) {
        fprintf(stderr, "The filename must not be empty.\n");
        return -1;
    }

    input_file = fopen(filename, "rb");
    if (input_file == NULL){
   		fprintf(stderr, "File must exist.\n");
    	return -1;
    }

    fread(&bmp->header.bfType, 2, 1, input_file);
    fread(&bmp->header.bfSize, 4, 1, input_file);
    fread(&bmp->header.bfReserved1, 2, 1, input_file);
    fread(&bmp->header.bfReserved2, 2, 1, input_file);
    fread(&bmp->header.bfOffBits, 4, 1, input_file);

    fread(&bmp->header.biSize, 4, 1, input_file);
    fread(&bmp->header.biWidth, 4, 1, input_file);
    fread(&bmp->header.biHeight, 4, 1, input_file);
    fread(&bmp->header.biPlanes, 2, 1, input_file);
    fread(&bmp->header.biBitCount, 2, 1, input_file);
    fread(&bmp->header.biCompression, 4, 1, input_file);
    fread(&bmp->header.biSizeImage, 4, 1, input_file);
    fread(&bmp->header.biXPelsPerMeter, 4, 1, input_file);
    fread(&bmp->header.biYPelsPerMeter, 4, 1, input_file);
    fread(&bmp->header.biClrUsed, 4, 1, input_file);
    fread(&bmp->header.biClrImportant, 4, 1, input_file);


	if(bmp->header.bfType != 0x4D42) PRINT_ERR(-1, "Invalid format signature. Program supports only BMP (.bmp) bitmap format.\n")
	if(bmp->header.bfReserved1 != 0) PRINT_ERR(-2, "Reservation error 1. Reserved bytes; must be zero.\n")
	if(bmp->header.bfReserved2 != 0) PRINT_ERR(-2, "Reservation error 2. Reserved bytes; must be zero.\n")
	if(bmp->header.biSize != 40)     PRINT_ERR(-2, "BMP version error. Program supports only BMP version 3.\n")
    if(bmp->header.biWidth == 0)     PRINT_ERR(-2, "Image width error. Image width must not be zero.\n")
    if(bmp->header.biHeight == 0)    PRINT_ERR(-2, "Image height error. Image height must not be zero.\n")
    if(bmp->header.biPlanes != 1)    PRINT_ERR(-2, "Planes error. In BMP, only the value 1 is allowed.\n")
    if(bmp->header.biBitCount != 8 && bmp->header.biBitCount != 24) PRINT_ERR(-2, "Bit count error. The program supports only 8 and 24 bit images.\n")
    if(bmp->header.biCompression != 0) PRINT_ERR(-2, "Compression error. The program supports only uncompressed images.\n")
    if(bmp->header.biBitCount == 8 && bmp->header.biClrUsed > 256) PRINT_ERR(-2, "Palette error. The number of colors in an 8-bit image must be in the range from 1 to 256.\n")
    if(bmp->header.biBitCount == 8 && bmp->header.biClrImportant > bmp->header.biClrUsed) PRINT_ERR(-2, "The number of color indexes required to display a bitmap cannot exceed 256.\n")

    bmp->palette = NULL;
    if(bmp->header.biBitCount == 8){
    	bmp->palette = (uint8_t*)malloc(bmp->header.biClrUsed * 4);
    	if(bmp->palette == NULL) {
    	fprintf(stderr, "Failed to allocate memory for palette.\n");
    	free(bmp);
   		fclose(input_file);
		return -2;
		}

		fread(bmp->palette, sizeof(uint8_t), bmp->header.biClrUsed * 4, input_file);
    }

    bmp->data = (uint8_t*)malloc(bmp->header.biSizeImage);
   	if(bmp->data == NULL) {
    fprintf(stderr, "Failed to allocate memory for data.\n");
    free(bmp->palette);
    free(bmp);
   	fclose(input_file);
	return -2;
	}
	fread(bmp->data, sizeof(uint8_t), bmp->header.biSizeImage, input_file);

	fclose(input_file);
	return 0;
}

void WriteBMP(BMP_STRUCTURE* bmp, char* new_filename){

	FILE*	output_file;
	output_file = fopen(new_filename, "wb");

    fwrite(&bmp->header.bfType, 2, 1, output_file);
    fwrite(&bmp->header.bfSize, 4, 1, output_file);
    fwrite(&bmp->header.bfReserved1, 2, 1, output_file);
    fwrite(&bmp->header.bfReserved2, 2, 1, output_file);
    fwrite(&bmp->header.bfOffBits, 4, 1, output_file);

    fwrite(&bmp->header.biSize, 4, 1, output_file);
    fwrite(&bmp->header.biWidth, 4, 1, output_file);
    fwrite(&bmp->header.biHeight, 4, 1, output_file);
    fwrite(&bmp->header.biPlanes, 2, 1, output_file);
    fwrite(&bmp->header.biBitCount, 2, 1, output_file);
    fwrite(&bmp->header.biCompression, 4, 1, output_file);
    fwrite(&bmp->header.biSizeImage, 4, 1, output_file);
    fwrite(&bmp->header.biXPelsPerMeter, 4, 1, output_file);
    fwrite(&bmp->header.biYPelsPerMeter, 4, 1, output_file);
    fwrite(&bmp->header.biClrUsed, 4, 1, output_file);
    fwrite(&bmp->header.biClrImportant, 4, 1, output_file);

    fwrite(bmp->palette, sizeof(uint8_t), bmp->header.biClrUsed * 4, output_file);
    fwrite(bmp->data, sizeof(uint8_t), bmp->header.biSizeImage, output_file);

    fclose(output_file);
}

void FreeBMP(BMP_STRUCTURE* bmp){
	free(bmp->data);
 	free(bmp->palette);
    free(bmp);
}



