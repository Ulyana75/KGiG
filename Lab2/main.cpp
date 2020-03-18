#include <iostream>
#include <algorithm>
#include <cmath>
#include <string.h>
using namespace std;
typedef unsigned char uchar;

const double pi = 3.141592653589793;
uchar** arr;
int** buffer;
int format, width, height, a;
int bright;
bool dir, dir2;
double gamma_ = -1;
double fatness;
double xa, ya, xb, yb, xc, yc, xd, yd;
char filename_in[100], filename_out[100];

double GammaCorrection(double Ipix, double Ibkgr) {
    if(Ipix == Ibkgr)
        return Ipix;
    double I = Ibkgr + (bright - Ibkgr) * pow((Ipix - Ibkgr) / (bright - Ibkgr), (double)1 / gamma_);
    return I;
}

double sRGB(double pix) {
    pix /= 255;
    if(pix <= 0.0031308)
        return 12.92 * pix * 255;
    else return (pow(1.055 * pix, 0.4167) - 0.055) * 255;
}

void Plot(int x, int y, double intensity) {
    if((x >= width) || (y >= height) || (intensity == 0) || (x < 0) || (y < 0))
        return;
    if(dir)
        swap(x, y);
    if(fatness < 1)
        intensity *= fatness;
    double back = 0;
    if(intensity != 1)
        back = (int)arr[y][x];
    int z = (back + ((double)(bright - back) * intensity));
    buffer[y][x] = z;
}

void OutputImage() {
    FILE* fout = fopen(filename_out, "wb");
    if(fout == NULL) {
        std::cerr << "Cannot create output file!\n";
        return;
    }
    fprintf_s(fout, "P%d\n%d %d\n%d\n", format, width, height, a);
    for(int i = 0; i < height; i++) {
        fwrite(arr[i], 1, width, fout);
    }
}

void ChangeDirection(double &x0, double &y00, double &x, double &y) {
    dir = abs(y - y00) > abs(x - x0);
    dir2 = false;
    if(dir) {
        swap(x, y);
        swap(y00, x0);
    }
    if(x0 > x) {
        dir2 = true;
        swap(x, x0);
        swap(y, y00);
    }
}

void Bresenhem(double &x0, double &y00, double &x, double &y) {
    ChangeDirection(x0, y00, x, y);
    bool done = false;
    double dx = x - x0;
    double dy = y - y00;
    double delta = dy / dx;
    double y_curr = y00;
    if(y - (int)y > 0) {
        for (int x_curr = x0; x_curr <= x; x_curr++) {
            Plot(x_curr, (int) y_curr, 1 - (y_curr - (int) y_curr));
            Plot(x_curr, (int) y_curr + 1, y_curr - (int) y_curr);
            y_curr += delta;
        }
        done = true;
    }
    if((x - (int)x > 0) || (!done)) {
        delta = dx / dy;
        double x_curr = x0;
        for (int y_cur = y00; y_cur <= y; y_cur++) {
            Plot((int) x_curr, y_cur, 1 - (x_curr - (int) x_curr));
            Plot((int) x_curr + 1, y_cur, x_curr - (int) x_curr);
            x_curr += delta;
        }
    }
    if(dir) {
        swap(x, y);
        swap(y00, x0);
    }
    if(dir2) {
        swap(x, x0);
        swap(y, y00);
    }
}

bool Inside(double x, double y) {
    return (y > (yb - ya) / (xb - xa) * (x - xa) + ya) && (y > (yd - yb) / (xd - xb) * (x - xb) + yb) &&
           (y < (yd - yc) / (xd - xc) * (x - xc) + yc) && (y < (yc - ya) / (xc - xa) * (x - xa) + ya);
}

void FillRectangle(double x, double y) {
    if(Inside(x, y))
        Plot(x, y, 1);
}

int main(int argc, char* argv[]) {
    if((argc < 9) || (argc > 10)) {
        std::cerr << "Wrong arguments!\n";
        return 1;
    }
    double x0, y00, x, y;
    strcpy(filename_in, argv[1]);
    strcpy(filename_out, argv[2]);
    bright = atoi(argv[3]);
    fatness = atof(argv[4]);
    x0 = atof(argv[5]);
    y00 = atof(argv[6]);
    x = atof(argv[7]);
    y = atof(argv[8]);
    if(argc == 10)
        gamma_ = atof(argv[9]);
    FILE* fin = fopen(filename_in, "rb");
    if(fin == NULL) {
        std::cerr << "Cannot open file!\n";
        return 1;
    }
    fscanf(fin, "P%d\n%d %d\n%d\n", &format, &width, &height, &a);
    arr = new uchar*[height];
    buffer = new int*[height];
    for(int i = 0; i < height; i++) {
        arr[i] = new uchar[width];
        buffer[i] = new int[width];
        fread(arr[i], 1, width, fin);
        for(int j = 0; j < width; j++)
            buffer[i][j] = -1;
    }
    if(fatness > 1) {
        double alpha = x - x0 == 0 ? 0 : (90 * pi / 180) - abs(atan((y - y00) / (x - x0)));
        xa = x0 - (fatness / 2) * cos(alpha);
        ya = y00 + (fatness / 2) * sin(alpha);
        xb = x0 + (fatness / 2) * cos(alpha);
        yb = y00 - (fatness / 2) * sin(alpha);
        xc = x - (fatness / 2) * cos(alpha);
        yc = y + (fatness / 2) * sin(alpha);
        xd = x + (fatness / 2) * cos(alpha);
        yd = y - (fatness / 2) * sin(alpha);
        if (xa > xc) {
            swap(xa, xc);
            swap(ya, yc);
            swap(xb, xd);
            swap(yb, yd);
        }
        Bresenhem(xa, ya, xb, yb);
        Bresenhem(xc, yc, xd, yd);
        Bresenhem(xa, ya, xc, yc);
        Bresenhem(xb, yb, xd, yd);
        dir = false;
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                FillRectangle(j, i);
    }
    else Bresenhem(x0, y00, x, y);
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            if(buffer[i][j] != -1) {
                if(gamma_ != -1)
                    arr[i][j] = (char) GammaCorrection(buffer[i][j], (int) arr[i][j]);
                else arr[i][j] = (char)sRGB(buffer[i][j]);
            }
    OutputImage();
    return 0;
}
