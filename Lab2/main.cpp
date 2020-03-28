#include <iostream>
#include <algorithm>
#include <cmath>
#include <string.h>
#define _USE_MATH_DEFINES
using namespace std;
typedef unsigned char uchar;

uchar* arr;
int* buffer;
int format, width, height, a;
int bright;
bool dir;
double gamma_ = -1;
double fatness;
double xa, ya, xb, yb, xc, yc, xd, yd;
double alpha;
char filename_in[100], filename_out[100];

double GammaCorrection(double Ipix, int flag) {           // 1 - прямая гамма, -1 - обратная
    double I = pow(Ipix / 255, pow(gamma_, flag));
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

void Plot(int x, int y, double intensity) {
    if((x >= width) || (y >= height) || (intensity == 0) || (x < 0) || (y < 0))
        return;
    if(dir)
        swap(x, y);
    if(fatness < 1)
        intensity *= fatness;
    double back = 0;
    if(intensity != 1) {
        back = (int) arr[y * width + x];
        if(gamma_ != -1)
            back = GammaCorrection(back, 1);
        else back = sRGB_reverse(back);
    }
    int z = (back + ((double)(bright - back) * intensity));
    if(gamma_ != -1)
        buffer[y * width + x] = GammaCorrection(z, -1);
    else buffer[y * width + x] = sRGB(z);
}

void OutputImage() {
    FILE* fout = fopen(filename_out, "wb");
    if(fout == NULL) {
        std::cerr << "Cannot create output file!\n";
        return;
    }
    fprintf_s(fout, "P%d\n%d %d\n%d\n", format, width, height, a);
    fwrite(arr, 1, width * height, fout);
    fclose(fout);
}

void ChangeDirection(double &x0, double &y00, double &x, double &y) {
    dir = abs(y - y00) > abs(x - x0);
    if(dir) {
        swap(x, y);
        swap(y00, x0);
    }
    if(x0 > x) {
        swap(x, x0);
        swap(y, y00);
    }
}

void Bresenhem(double x0, double y00, double x, double y) {
    //printf("Im drawing (%f %f) - > (%f %f)\n", x0, y00, x, y);
    ChangeDirection(x0, y00, x, y);
    double dx = x - x0;
    double dy = y - y00;
    double delta = dx == 0 ? 0 : dy / dx;
    double y_curr = y00;
    for (int x_curr = x0; x_curr <= x; x_curr++) {
        Plot(x_curr, y_curr, 1 - (y_curr - (int) y_curr));
        if ((alpha * 180 / M_PI != 45) || (fatness <= 1))
            Plot(x_curr, y_curr + 1, y_curr - (int) y_curr);
        y_curr += delta;
    }
}

class Vector {
public:
    Vector(long double x_, long double y_) {
        x = x_;
        y = y_;
        FindL();
    }
    void FindL() {
        l = sqrt(x * x + y * y);
    }
    double operator*(const Vector &other) {
        return this->x * other.x + this->y * other.y;
    }
    Vector operator-() {
        Vector c = {-(this->x), -(this->y)};
        return c;
    }
    long double x;
    long double y;
    long double l;
};

bool Inside(double x, double y) {
    Vector ab = {xb - xa, yb - ya};
    Vector bd = {xd - xb, yd - yb};
    Vector dc = {xc - xd, yc - yd};
    Vector ca = {xa - xc, ya - yc};
    Vector ao = {x - xa, y - ya}; if(ao.l == 0) return true;
    Vector bo = {x - xb, y - yb}; if(bo.l == 0) return true;
    Vector Do = {x - xd, y - yd}; if(Do.l == 0) return true;
    Vector co = {x - xc, y - yc}; if(co.l == 0) return true;
    long double oab = acos(ao * ab / (ao.l * ab.l)) * 180 / M_PI;
    long double oac = acos(ao * (-ca) / (ao.l * ca.l)) * 180 / M_PI;
    long double oba = acos(bo * (-ab) / (bo.l * ab.l)) * 180 / M_PI;
    long double obd = acos(bo * bd / (bo.l * bd.l)) * 180 / M_PI;
    long double odc = acos(Do * dc / (Do.l * dc.l)) * 180 / M_PI;
    long double odb = acos(Do * (-bd) / (Do.l * bd.l)) * 180 / M_PI;
    long double ocd = acos(co * (-dc) / (co.l * dc.l)) * 180 / M_PI;
    long double oca = acos(co * ca / (co.l * ca.l)) * 180 / M_PI;
    long double sum = (180 - oab - oba) + (180 - obd - odb) + (180 - odc - ocd) +  (180 - oca - oac);
    if((y >= (yd - yc) / (xd - xc) * (x - xc) + yc - 1) && !((xa == xc) || (ya == yc)) && (alpha * 180 / M_PI == 45))
        return false;
    return (sum < 360.0000001) && (sum > 359.9999999);
}

void CheckIsRight() {
    for(int i = 1; i < height - 1; i++)
        for(int j = 1; j < width - 1; j++)
            if(buffer[i * width + j] == -1) {
                int count = 0;
                if(buffer[i * width + j + 1] != -1) count++;
                if(buffer[i * width + j - 1] != -1) count++;
                if(buffer[(i + 1) * width + j] != -1) count++;
                if(buffer[(i - 1) * width + j] != -1) count++;
                if(count == 4)
                    Plot(j, i, 1);
            }
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
    if(fscanf(fin, "P%d\n%d %d\n%d\n", &format, &width, &height, &a) != 4) {
        std::cerr << "Something wrong with input file.\n";
        return 1;
    }
    if(format != 5) {
        std::cerr << "File has wrong format!\n";
        return 1;
    }
    arr = new uchar[height * width];
    buffer = new int[height * width];
    for(int i = 0; i < height * width; i++)
        buffer[i] = -1;
    fread(arr, 1, width * height, fin);
    if(gamma_ == -1)
        bright = sRGB_reverse(bright);
    else bright = GammaCorrection(bright, 1);
    if(fatness > 1) {
        alpha = x - x0 == 0 ? 0 : (90 * M_PI / 180) - abs(atan((y - y00) / (x - x0)));
        if((y - y00) / (x - x0) > 0) {
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
        }
        else {
            xa = x0 - (fatness / 2) * cos(alpha);
            ya = y00 - (fatness / 2) * sin(alpha);
            xb = x - (fatness / 2) * cos(alpha);
            yb = y - (fatness / 2) * sin(alpha);
            xc = x0 + (fatness / 2) * cos(alpha);
            yc = y00 + (fatness / 2) * sin(alpha);
            xd = x + (fatness / 2) * cos(alpha);
            yd = y + (fatness / 2) * sin(alpha);
            if(xb < xa) {
                swap(xa, xb);
                swap(ya, yb);
                swap(xc, xd);
                swap(yc, yd);
            }
        }
        if(alpha * 180 / M_PI == 45) {
            dir = false;
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    FillRectangle(j, i);
        }
        //printf("A: %f %f\nB: %f %f\nC: %f %f\nD: %f %f\nalpha = %Lf\n", xa, ya, xb, yb, xc, yc, xd, yd, alpha * 180 / M_PI);
        if((xa == xc) && (xb == xd) && (y != y00)) {
            Bresenhem(xa + 1, ya, xb, yb);
            Bresenhem(xc + 1, yc, xd, yd);
        }
        else {
            Bresenhem(xa, ya, xb, yb);
            Bresenhem(xc, yc, xd, yd);
        }
        if((ya == yb) && (yc == yd) && (x0 != x)) {
            Bresenhem(xa, ya + 1, xc, yc);
            Bresenhem(xb, yb + 1, xd, yd);
        }
        else {
            Bresenhem(xa, ya, xc, yc);
            Bresenhem(xb, yb, xd, yd);
        }
        if(alpha * 180 / M_PI != 45) {
            dir = false;
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    FillRectangle(j, i);
        }
    }
    else Bresenhem(x0, y00, x, y);
    dir = false;
    CheckIsRight();
    for(int i = 0; i < height * width; i++)
        if(buffer[i] != -1)
            arr[i] = buffer[i];
    OutputImage();
    fclose(fin);
    delete[] arr;
    return 0;
}
