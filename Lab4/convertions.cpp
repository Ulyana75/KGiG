//
// Created by user on 30.04.2020.
//

#include "convertions.h"
#include <cmath>
typedef unsigned char uchar;

void CMY_to_RGB(int width, int height, double* first, double* second, double* third) {
    for(int i = 0; i < width * height; i++) {
        first[i] = (1 - first[i] / 255.0) * 255;
        second[i] = (1 - second[i] / 255.0) * 255;
        third[i] = (1 - third[i] / 255.0) * 255;
    }
}

double f(double p, double q, double t) {
    if (t < 0)
        t += 1;
    if (t > 1)
        t -= 1;
    if (t < 1.0 / 6.0)
        return p + (q - p) * 6 * t;
    if (t < 1.0 / 2.0)
        return q;
    if (t < 2.0 / 3.0)
        return p + (q - p) * (2.0 / 3.0 - t) * 6;
    return p;
}

void HSL_to_RGB(int width, int height, double* first, double* second, double* third) {
    double r, g, b, h, s, l;
    for(int i = 0; i < width * height; i++) {
        h = first[i] / 255.0; s = second[i] / 255.0; l = third[i] / 255.0;
        if (s == 0) {
            r = l; g = l; b = l;
        }
        else {
            double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
            double p = 2 * l - q;
            r = f(p, q, h + 1.0 / 3.0);
            g = f(p, q, h);
            b = f(p, q, h - 1.0 / 3.0);
        }
        first[i] = r * 255;
        second[i] = g * 255;
        third[i] = b * 255;
    }
}

void HSV_to_RGB(int width, int height, double* first, double* second, double* third) {
    double h1, p, q, t, ff, r, g, b, h, s, v;
    int j;
    for(int i = 0; i < width * height; i++) {
        h = first[i] / 255.0 * 360; s = second[i] / 255.0; v = third[i] / 255.0;
        if (s <= 0.0) {
            first[i] = v * 255;
            second[i] = v * 255;
            third[i] = v * 255;
            continue;
        }
        h1 = h;
        if (h1 >= 360.0) h1 = 0;
        h1 /= 60.0;
        j = (int) h1;
        ff = h1 - j;
        p = v * (1.0 - s);
        q = v * (1.0 - (s * ff));
        t = v * (1.0 - (s * (1.0 - ff)));
        switch (j) {
            case 0:
                r = v;
                g = t;
                b = p;
                break;
            case 1:
                r = q;
                g = v;
                b = p;
                break;
            case 2:
                r = p;
                g = v;
                b = t;
                break;
            case 3:
                r = p;
                g = q;
                b = v;
                break;
            case 4:
                r = t;
                g = p;
                b = v;
                break;
            case 5:
            default:
                r = v;
                g = p;
                b = q;
                break;
        }
        first[i] = r * 255;
        second[i] = g * 255;
        third[i] = b * 255;
    }
}

void YCbCr601_to_RGB(int width, int height, double* first, double* second, double* third) {
    double y, cb, cr;
    for(int i = 0; i < width * height; i++) {
        y = first[i]; cb = second[i]; cr = third[i];
        first[i] = 298.082 * y / 256.0 + 408.583 * cr / 256.0 - 222.921;
        second[i] = 298.082 * y / 256.0 - 100.291 * cb / 256.0 - 208.120 * cr / 256.0 + 135.576;
        third[i] = 298.082 * y / 256.0 + 516.412 * cb / 256.0 - 276.836;
    }
}

void YCbCr709_to_RGB(int width, int height, double* first, double* second, double* third) {
    double y, cb, cr;
    for(int i = 0; i < width * height; i++) {
        y = first[i]; cb = second[i]; cr = third[i];
        first[i] = y + 1.402 * (cr - 128);
        second[i] = y - 0.344136 * (cb - 128) - 0.714136 * (cr - 128);
        third[i] = y + 1.772 * (cb - 128);
    }
}

void YCoCg_to_RGB(int width, int height, double* first, double* second, double* third) {
    double y, co, cg;
    for(int i = 0; i < width * height; i++) {
        y = first[i]; cg = second[i]; co = third[i];
        first[i] = y - cg + co;
        second[i] = y + cg;
        third[i] = y - cg - co;
    }
}

void RGB_to_CMY(int width, int height, double* first, double* second, double* third) {
    for(int i = 0; i < width * height; i++) {
        first[i] = (1 - first[i] / 255.0) * 255;
        second[i] = (1 - second[i] / 255.0) * 255;
        third[i] = (1 - third[i] / 255.0) * 255;
    }
}

void RGB_to_HSL(int width, int height, double* first, double* second, double* third) {
    double r, g, b, h, s, l, max, min;
    for(int i = 0; i < width * height; i++) {
        r = first[i] / 255.0; g = second[i] / 255.0; b = third[i] / 255.0;
        max = __max(__max(r, g), b);
        min = __min(__min(r, g), b);
        l = (max + min) / 2.0;
        if((l == 0) || (max == min))
            s = 0;
        else if(l <= 0.5)
            s = (max - min) / (2 * l);
        else
            s = (max - min) / (2 - 2 * l);
        if(max == min)
            h = 0;
        if((max == r) && (g >= b))
            h = 60 * (g - b) / (max - min);
        else if((max == r) && (g < b))
            h = 60 * (g - b) / (max - min) + 360;
        else if(max == g)
            h = 60 * (b - r) / (max - min) + 120;
        else if(max == b)
            h = 60 * (r - g) / (max - min) + 240;
        first[i] = h * 255 / 360;
        second[i] = s * 255;
        third[i] = l * 255;

    }
}

void RGB_to_HSV(int width, int height, double* first, double* second, double* third) {
    double min, max, delta, r, g, b, h, s, v;
    for(int i = 0; i < width * height; i++) {
        r = first[i] / 255.0; g = second[i] / 255.0; b = third[i] / 255.0;
        max = __max(__max(r, g), b);
        min = __min(__min(r, g), b);
        v = max;
        third[i] = v * 255;
        delta = max - min;
        if (delta < 0.00001) {
            second[i] = 0;
            first[i] = 0;
            continue;
        }
        if (max > 0)
            s = (delta / max);
        else {
            second[i] = 0;
            first[i] = 0;
            continue;
        }
        if (r >= max)
            h = (g - b) / delta;
        else if (g >= max)
            h = 2.0 + (b - r) / delta;
        else
            h = 4.0 + (r - g) / delta;

        h *= 60.0;

        if (h < 0)
            h += 360.0;
        first[i] = h * 255 / 360;
        second[i] = s * 255;
    }
}

void RGB_to_YCbCr601(int width, int height, double* first, double* second, double* third) {
    double r, g, b;
    for(int i = 0; i < width * height; i++) {
        r = first[i]; g = second[i]; b = third[i];
        first[i] = 16 + 65.738 * r / 256.0 + 129.057 * g / 256.0 + 25.064 * b / 256.0;
        second[i] = 128 - 37.945 * r / 256.0 - 74.494 * g / 256.0 + 112.439 * b / 256.0;
        third[i] = 128 + 112.439 * r / 256.0 - 94.154 * g / 256.0 - 18.285 * b / 256.0;
    }
}

void RGB_to_YCbCr709(int width, int height, double* first, double* second, double* third) {
    double r, g, b;
    for(int i = 0; i < width * height; i++) {
        r = first[i]; g = second[i]; b = third[i];
        first[i] = 0.299 * r + 0.587 * g + 0.114 * b;
        second[i] = 128 - 0.168736 * r - 0.331264 * g + 0.5 * b;
        third[i] = 128 + 0.5 * r - 0.418688 * g - 0.081312 * b;
    }
}

void RGB_to_YCoCg(int width, int height, double* first, double* second, double* third) {
    double r, g, b;
    for(int i = 0; i < width * height; i++) {
        r = first[i]; g = second[i]; b = third[i];
        first[i] = 0.25 * r + 0.5 * g + 0.25 * b;
        second[i] = -0.25 * r + 0.5 * g - 0.25 * b;
        third[i] = 0.5 * r - 0.5 * b;
    }
}