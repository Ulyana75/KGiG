#include <iostream>
#include <string.h>
#include "convertions.h"
typedef unsigned char uchar;

int width, height, a, format;
uchar* arr;
double* first;
double* second;
double* third;
int count_in, count_out;
char from[10], to[10], filename_in[100], filename_out[100];

int InputImage(int c, char* filename);
int OutputImage(int f, double* ar, char* filename);

void ClearMemory() {
    delete[] first;
    delete[] second;
    delete[] third;
}

int MakeArguments(int argc, char* argv[]) {

    if(argc != 11)
        return 1;

    for(int i = 0; i < argc; i++)
        if(strcmp(argv[i], "-f") == 0) {
            strcpy(from, argv[i + 1]);
            for(int j = 0; j < argc; j++)
                if(strcmp(argv[j], "-t") == 0) {
                    strcpy(to, argv[j + 1]);
                    for(int x = 0; x < argc; x++)
                        if(strcmp(argv[x], "-i") == 0) {
                            count_in = atoi(argv[x + 1]);
                            strcpy(filename_in, argv[x + 2]);
                            for(int y = 0; y < argc; y++)
                                if(strcmp(argv[y], "-o") == 0) {
                                    count_out = atoi(argv[y + 1]);
                                    strcpy(filename_out, argv[y + 2]);
                                    return 0;
                                }
                        }
                }
        }
    return 1;
}

int Convert() {

    if(strcmp(from, "RGB") == 0) {}
    else if(strcmp(from, "HSL") == 0)
        HSL_to_RGB(width, height, first, second, third);
    else if(strcmp(from, "HSV") == 0)
        HSV_to_RGB(width, height, first, second, third);
    else if(strcmp(from, "YCbCr.601") == 0)
        YCbCr601_to_RGB(width, height, first, second, third);
    else if(strcmp(from, "YCbCr.709") == 0)
        YCbCr709_to_RGB(width, height, first, second, third);
    else if(strcmp(from, "YCoCg") == 0)
        YCoCg_to_RGB(width, height, first, second, third);
    else if(strcmp(from, "CMY") == 0)
        CMY_to_RGB(width, height, first, second, third);
    else
        return 1;

    if(strcmp(to, "RGB") == 0) {}
    else if(strcmp(to, "HSL") == 0)
        RGB_to_HSL(width, height, first, second, third);
    else if(strcmp(to, "HSV") == 0)
        RGB_to_HSV(width, height, first, second, third);
    else if(strcmp(to, "YCbCr.601") == 0)
        RGB_to_YCbCr601(width, height, first, second, third);
    else if(strcmp(to, "YCbCr.709") == 0)
        RGB_to_YCbCr709(width, height, first, second, third);
    else if(strcmp(to, "YCoCg") == 0)
        RGB_to_YCoCg(width, height, first, second, third);
    else if(strcmp(to, "CMY") == 0)
        RGB_to_CMY(width, height, first, second, third);
    else
        return 1;

    return 0;
}

char* MakeName(int n, char* temp) {
    int i;
    char* answer;
    answer = new char[100];
    for(int j = 0; j < 100; j++)
        answer[j] = '\0';
    for(i = 0; i < strlen(temp); i++)
        if(temp[i] == '.')
            break;
    strncat(answer, temp, i);
    if(n == 1)
        strcat(answer, "_1.pgm");
    if(n == 2)
        strcat(answer, "_2.pgm");
    if(n == 3)
        strcat(answer, "_3.pgm");
    strcat(answer, "\0");
    return answer;
}

int main(int argc, char* argv[]) {

    if(MakeArguments(argc, argv)) {
        std::cerr << "Wrong arguments\n";
        return 1;
    }

    if(count_in == 1) {
        InputImage(3, filename_in);
        first = new double[width * height];
        second = new double[width * height];
        third = new double[width * height];
        int k = 0;
        for(int i = 0; i < width * height * 3 - 2; i += 3) {
            first[k] = (double)arr[i];
            second[k] = (double)arr[i + 1];
            third[k] = (double)arr[i + 2];
            k++;
        }
        delete[] arr;
    }

    else {
        InputImage(1, MakeName(1, filename_in));
        first = new double[width * height];
        second = new double[width * height];
        third = new double[width * height];
        for(int i = 0; i < width * height; i++)
            first[i] = (double)arr[i];
        delete[] arr;

        InputImage(1, MakeName(2, filename_in));
        for(int i = 0; i < width * height; i++)
            second[i] = (double)arr[i];
        delete[] arr;

        InputImage(1, MakeName(3, filename_in));
        for(int i = 0; i < width * height; i++)
            third[i] = (double)arr[i];
        delete[] arr;
    }

    if(Convert()) {
        std::cerr << "Wrong name of colorspace\n";
        ClearMemory();
        return 1;
    }

    if(count_out == 1) {
        OutputImage(6, first, filename_out);
    }

    else {
        OutputImage(5, first, MakeName(1, filename_out));
        OutputImage(5, second, MakeName(2, filename_out));
        OutputImage(5, third, MakeName(3, filename_out));
    }

    ClearMemory();
    return 0;
}

int InputImage(int c, char* filename) {
    FILE* fin = fopen(filename, "rb");
    if(fin == NULL) {
        std::cerr << "Cannot open file!\n";
        ClearMemory();
        return 1;
    }
    if(fscanf(fin, "P%d\n%d %d\n%d\n", &format, &width, &height, &a) != 4) {
        std::cerr << "Something wrong with input file.\n";
        fclose(fin);
        ClearMemory();
        return 1;
    }
    if(((count_in == 1) && (format != 6)) || ((count_in == 3) && (format != 5))) {
        std::cerr << "File has wrong format!\n";
        fclose(fin);
        ClearMemory();
        return 1;
    }
    arr = new uchar[height * width * c];
    fread(arr, 1, width * height * c, fin);
    fclose(fin);
    return 0;
}

int OutputImage(int f, double* ar, char* filename) {
    FILE* fout = fopen(filename, "wb");
    if(fout == NULL) {
        std::cerr << "Cannot create output file!\n";
        ClearMemory();
        return 1;
    }
    fprintf_s(fout, "P%d\n%d %d\n%d\n", f, width, height, a);
    if(count_out == 1)
        for(int i = 0; i < width * height; i++) {
            uchar one = (uchar) first[i];
            uchar two = (uchar) second[i];
            uchar three = (uchar) third[i];
            fwrite(&one, 1, 1, fout);
            fwrite(&two, 1, 1, fout);
            fwrite(&three, 1, 1, fout);
        }
    else
        for(int i = 0; i < width * height; i++) {
            uchar c = (uchar) ar[i];
            fwrite(&c, 1, 1, fout);
        }
    fclose(fout);
    return 0;
}