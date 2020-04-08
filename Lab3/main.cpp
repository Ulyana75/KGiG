#include <iostream>
#include <cmath>
#include <string.h>
#include <ctime>

typedef unsigned char uchar;

int format, width, height, a;
double* arr;
uchar* buffer;
double gamma_;
int gradient, diz, b;
int* p;
char filename_in[100], filename_out[100];


int InputImage();
int OutputImage();


void DrawGradient() {
    for(int i = 0; i < width; i++) {
        double pos = (double)i / (width - 1);
        int color = pos * 255;
        for(int j = 0; j < height; j++) {
            arr[j * width + i] = color;
        }
    }
}

double GammaCorrection(double Ipix, int flag) {           // 1 - прямая гамма, -1 - обратная
    double I = pow(Ipix / 255.0, pow(gamma_, flag));
    return I * 255;
}

double sRGB(double pix) {
    pix /= 255;
    if(pix <= 0.0031308)
        return 12.92 * pix * 255;
    else return (1.055 * pow(pix, 0.4167) - 0.055) * 255;
}

double sRGB_reverse(double pix) {
    pix /= 255;
    if(pix <= 0.04045)
        return pix / 12.92 * 255;
    else return pow((pix + 0.055) / 1.055, 2.4) * 255;
}

int OtherBitness(int color) {
    int buf = color;
    buf = buf >> (8 - b);
    if(b >= 4) {
        buf = buf << (8 - b);
        color = color >> b;
        return buf + color;
    }
    else {
        if(b == 3) {
            int new_col = buf;
            new_col = new_col << 3;
            new_col += buf;
            buf = buf >> 1;
            new_col = new_col << 2;
            return new_col + buf;
        }
        else {
            int new_col = 0;
            for(int i = 0; i < 8 / b; i++) {
                new_col = new_col << b;
                new_col += buf;
            }
            return new_col;
        }
    }
}

int FindNearestColor(int color) {
    int min_ = INT_MAX, j;
    for(int i = 0; i < 256; i++)
        if(abs(color - p[i]) < min_) {
            min_ = abs(color - p[i]);
            j = i;
        }
    return p[j];
}

void NoDithering() {
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            arr[i * width + j] = OtherBitness((int)arr[i * width + j]);
}

void OrderedDithering() {
    double M[8][8] = {
            {0, 48, 12, 60, 3, 51, 15, 63},
            {32, 16, 44, 28, 35, 19, 47, 31},
            {8, 56, 4, 52, 11, 59, 7, 55},
            {40, 24, 36, 20, 43, 27, 39, 23},
            {2, 50, 14, 62, 1, 49, 13, 61},
            {34, 18, 46, 30, 33, 17, 45, 29},
            {10, 58, 6, 54, 9, 57, 5, 53},
            {42, 26, 38, 22, 41, 25, 37, 21}
    };
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            M[i][j] = (M[i][j] + 1) / 64 - 0.5;
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++) {
            int color = (int)arr[i * width + j] + 255 * M[i % 8][j % 8];
            arr[i * width + j] = FindNearestColor(color);
        }
}

void RandomDithering() {
    srand(time(0));
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++) {
            int color = (int)arr[i * width + j] + 255 * ((rand() % 256 + 1) / 256.0 - 0.5);
            arr[i * width + j] = FindNearestColor(color);
        }
}

void Halftone() {
    double M[4][4] = {
            {7, 13, 11, 4},
            {12, 16, 14, 8},
            {10, 15, 6, 2},
            {5, 9, 3, 1},
    };
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            M[i][j] = M[i][j] / 16 - 0.5;
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++) {
            int color = (int)arr[i * width + j] + 255 * M[i % 4][j % 4];
            arr[i * width + j] = FindNearestColor(color);
        }
}

void FloydSteinberg() {
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++) {
            int color = (int)arr[i * width + j];
            int new_color = FindNearestColor(color);
            double error = color - new_color;
            arr[i * width + j] = new_color;
            if((j - 1 >= 0) && (i + 1 < height))
                arr[(i + 1) * width + j - 1] += error * 3 / 16;
            if(i + 1 < height)
                arr[(i + 1) * width + j] += error * 5 / 16;
            if((i + 1 < height) && (j + 1 < width))
                arr[(i + 1) * width + j + 1] += error * 1 / 16;
            if(j + 1 < width)
                arr[i * width + j + 1] += error * 7 / 16;
        }
}

void JarvisJudiceNinke() {
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++) {
            int color = (int)arr[i * width + j];
            int new_color = FindNearestColor(color);
            double error = color - new_color;
            arr[i * width + j] = new_color;
            if(i + 1 < height) {
                if(j - 2 >= 0)
                    arr[(i + 1) * width + j - 2] += error * 3 / 48;
                if(j - 1 >= 0)
                    arr[(i + 1) * width + j - 1] += error * 5 / 48;
                arr[(i + 1) * width + j] += error * 7 / 48;
                if(j + 1 < width)
                    arr[(i + 1) * width + j + 1] += error * 5 / 48;
                if(j + 2 < width)
                    arr[(i + 1) * width + j + 2] += error * 3 / 48;
            }
            if(i + 2 < height) {
                if(j - 2 >= 0)
                    arr[(i + 2) * width + j - 2] += error * 1 / 48;
                if(j - 1 >= 0)
                    arr[(i + 2) * width + j - 1] += error * 3 / 48;
                arr[(i + 2) * width + j] += error * 5 / 48;
                if(j + 1 < width)
                    arr[(i + 2) * width + j + 1] += error * 3 / 48;
                if(j + 2 < width)
                    arr[(i + 2) * width + j + 2] += error * 1 / 48;
            }
            if(j + 1 < width)
                arr[i * width + j + 1] += error * 7 / 48;
            if(j + 2 < width)
                arr[i * width + j + 2] += error * 5 / 48;
        }
}

void Sierra3() {
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++) {
            int color = (int)arr[i * width + j];
            int new_color = FindNearestColor(color);
            double error = color - new_color;
            arr[i * width + j] = new_color;
            if(i + 1 < height) {
                if(j - 2 >= 0)
                    arr[(i + 1) * width + j - 2] += error * 2 / 32;
                if(j - 1 >= 0)
                    arr[(i + 1) * width + j - 1] += error * 4 / 32;
                arr[(i + 1) * width + j] += error * 5 / 32;
                if(j + 1 < width)
                    arr[(i + 1) * width + j + 1] += error * 4 / 32;
                if(j + 2 < width)
                    arr[(i + 1) * width + j + 2] += error * 2 / 32;
            }
            if(i + 2 < height) {
                if(j - 1 >= 0)
                    arr[(i + 2) * width + j - 1] += error * 2 / 32;
                arr[(i + 2) * width + j] += error * 3 / 32;
                if(j + 1 < width)
                    arr[(i + 2) * width + j + 1] += error * 2 / 32;
            }
            if(j + 1 < width)
                arr[i * width + j + 1] += error * 5 / 32;
            if(j + 2 < width)
                arr[i * width + j + 2] += error * 3 / 32;
        }
}

void Atkinson() {
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++) {
            int color = (int)arr[i * width + j];
            int new_color = FindNearestColor(color);
            double error = color - new_color;
            arr[i * width + j] = new_color;
            if(i + 1 < height) {
                if(j - 1 >= 0)
                    arr[(i + 1) * width + j - 1] += error / 8;
                arr[(i + 1) * width + j] += error / 8;
                if(j + 1 < width)
                    arr[(i + 1) * width + j + 1] += error / 8;
            }
            if(i + 2 < height)
                arr[(i + 2) * width + j] += error / 8;
            if(j + 1 < width)
                arr[i * width + j + 1] += error / 8;
            if(j + 2 < width)
                arr[i * width + j + 2] += error / 8;
        }
}

int main(int argc, char* argv[]) {

    if(argc != 7) {
        std::cerr << "Wrong arguments!\n";
        return 1;
    }

    strcpy(filename_in, argv[1]);
    strcpy(filename_out, argv[2]);
    gradient = atoi(argv[3]);
    diz = atoi(argv[4]);
    b = atoi(argv[5]);
    gamma_ = atof(argv[6]);

    if(InputImage() == 1)
        return 1;

    if(gradient)
        DrawGradient();

    if(!gradient) {
        for (int i = 0; i < height * width; i++)
            if (gamma_ == 0)
                arr[i] = sRGB_reverse(arr[i]);
            else arr[i] = GammaCorrection(arr[i], 1);
    }

    p = new int[256];
    for(int i = 0; i < 256; i++)
        p[i] = OtherBitness(i);

    switch(diz) {
        case 0:
            NoDithering();
            break;
        case 1:
            OrderedDithering();
            break;
        case 2:
            RandomDithering();
            break;
        case 3:
            FloydSteinberg();
            break;
        case 4:
            JarvisJudiceNinke();
            break;
        case 5:
            Sierra3();
            break;
        case 6:
            Atkinson();
            break;
        case 7:
            Halftone();
            break;
        default:
            std::cerr << "Wrong number of dithering!\n";
            return 1;
    }

    for(int i = 0; i < height * width; i++) {
        if (gamma_ == 0)
            arr[i] = sRGB(arr[i]);
        else arr[i] = GammaCorrection(arr[i], -1);
    }

    OutputImage();

    delete [] arr;
    delete [] buffer;
    delete [] p;

    return 0;
}

int InputImage() {
    FILE* fin = fopen(filename_in, "rb");
    if(fin == NULL) {
        std::cerr << "Cannot open file!\n";
        return 1;
    }
    if(fscanf(fin, "P%d\n%d %d\n%d\n", &format, &width, &height, &a) != 4) {
        std::cerr << "Something wrong with input file.\n";
        fclose(fin);
        return 1;
    }
    if(format != 5) {
        std::cerr << "File has wrong format!\n";
        fclose(fin);
        return 1;
    }
    arr = new double[height * width];
    buffer = new uchar[height * width];
    fread(buffer, 1, width * height, fin);
    for(int i = 0; i < height * width; i++)
        arr[i] = (double)buffer[i];
    fclose(fin);
    return 0;
}

int OutputImage() {
    FILE* fout = fopen(filename_out, "wb");
    if(fout == NULL) {
        std::cerr << "Cannot create output file!\n";
        delete [] arr;
        delete [] buffer;
        delete [] p;
        return 1;
    }
    fprintf_s(fout, "P%d\n%d %d\n%d\n", format, width, height, a);
    for(int i = 0; i < height * width; i++)
        buffer[i] = (uchar)arr[i];
    fwrite(buffer, 1, width * height, fout);
    fclose(fout);
    return 0;
}