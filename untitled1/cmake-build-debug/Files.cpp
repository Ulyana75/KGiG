#ifndef FILES_H
#define FILES_H

#pragma warning(disable: 4996)

struct size_of_pgm {
    int type;
    int h;
    int w;
    int depth;
};

float* OpenReadFile(char* filename, size_of_pgm& size, int& countByte);

float Gamma(float pixel, float gamma);

float sRGB(float pixel);

float BsRGB(float pixel);

void ReversePixel(float& pixel, float gamma);

void Output(char* output, float* arr, size_of_pgm& size, float gamma);

#endif
