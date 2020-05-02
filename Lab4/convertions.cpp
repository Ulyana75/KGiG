//
// Created by user on 30.04.2020.
//

#include "convertions.h"
#include <cmath>
#include <algorithm>
typedef unsigned char uchar;

double Cast(double value) {
    if (value > 255)
        return 255;
    if (value < 0)
        return 0;
    return value;
}

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
    double y, cb, cr, kr = 0.299, kb = 0.114, r, g, b;
    for(int i = 0; i < width * height; i++) {
        y = first[i] / 255.0; cb = second[i] / 255.0 * 2 - 1; cr = third[i] / 255.0 * 2 - 1;
        r = y + (1 - kr) * cr;
        g = y - kb * (1 - kb) * cb / (1 - kb - kr) - kr * (1 - kr) * cr / (1 - kb - kr);
        b = y + (1 - kb) * cb;
        first[i] = Cast(r * 255);
        second[i] = Cast(g * 255);
        third[i] = Cast(b * 255);
    }
}

void YCbCr709_to_RGB(int width, int height, double* first, double* second, double* third) {
    double y, cb, cr, kr = 0.2126, kb = 0.0722, r, g, b;
    for(int i = 0; i < width * height; i++) {
        y = first[i] / 255.0; cb = second[i] / 255.0 * 2 - 1; cr = third[i] / 255.0 * 2 - 1;
        r = y + (1 - kr) * cr;
        g = y - kb * (1 - kb) * cb / (1 - kb - kr) - kr * (1 - kr) * cr / (1 - kb - kr);
        b = y + (1 - kb) * cb;
        first[i] = Cast(r * 255);
        second[i] = Cast(g * 255);
        third[i] = Cast(b * 255);
    }
}

void YCoCg_to_RGB(int width, int height, double* first, double* second, double* third) {
    double y, co, cg;
    for(int i = 0; i < width * height; i++) {
        y = first[i] / 255.0; co = second[i] / 255.0 - 0.5; cg = third[i] / 255.0 - 0.5;
        first[i] = Cast((y - cg + co) * 255);
        second[i] = Cast((y + cg) * 255);
        third[i] = Cast((y - cg - co) * 255);
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
    double r, g, b, h, s, l, max_, min_;
    for(int i = 0; i < width * height; i++) {
        r = first[i] / 255.0; g = second[i] / 255.0; b = third[i] / 255.0;
        max_ = std::max(std::max(r, g), b);
        min_ = std::min(std::min(r, g), b);
        l = (max_ + min_) / 2.0;
        if((l == 0) || (max_ == min_))
            s = 0;
        else if(l <= 0.5)
            s = (max_ - min_) / (2 * l);
        else
            s = (max_ - min_) / (2 - 2 * l);
        if(max_ == min_)
            h = 0;
        if((max_ == r) && (g >= b))
            h = 60 * (g - b) / (max_ - min_);
        else if((max_ == r) && (g < b))
            h = 60 * (g - b) / (max_ - min_) + 360;
        else if(max_ == g)
            h = 60 * (b - r) / (max_ - min_) + 120;
        else if(max_ == b)
            h = 60 * (r - g) / (max_ - min_) + 240;
        first[i] = h * 255 / 360;
        second[i] = s * 255;
        third[i] = l * 255;

    }
}

void RGB_to_HSV(int width, int height, double* first, double* second, double* third) {
    double min_, max_, delta, r, g, b, h, s, v;
    for(int i = 0; i < width * height; i++) {
        r = first[i] / 255.0; g = second[i] / 255.0; b = third[i] / 255.0;
        max_ = std::max(std::max(r, g), b);
        min_ = std::min(std::min(r, g), b);
        v = max_;
        third[i] = v * 255;
        delta = max_ - min_;
        if (delta < 0.00001) {
            second[i] = 0;
            first[i] = 0;
            continue;
        }
        if (max_ > 0)
            s = (delta / max_);
        else {
            second[i] = 0;
            first[i] = 0;
            continue;
        }
        if (r >= max_)
            h = (g - b) / delta;
        else if (g >= max_)
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
    double r, g, b, kr = 0.299, kb = 0.114, y, cb, cr;
    for(int i = 0; i < width * height; i++) {
        r = first[i] / 255.0; g = second[i] / 255.0; b = third[i] / 255.0;
        y = kr * r + (1 - kb - kr) * g + kb * b;
        cb = (b - y) / (1 - kb) + 1;
        cr = (r - y) / (1 - kr) + 1;
        first[i] = Cast(y * 255);
        second[i] = Cast(cb * 255 / 2);
        third[i] = Cast(cr * 255 / 2);
    }
}

void RGB_to_YCbCr709(int width, int height, double* first, double* second, double* third) {
    double r, g, b, kr = 0.2126, kb = 0.0722, y, cb, cr;
    for(int i = 0; i < width * height; i++) {
        r = first[i] / 255.0; g = second[i] / 255.0; b = third[i] / 255.0;
        y = kr * r + (1 - kb - kr) * g + kb * b;
        cb = (b - y) / (1 - kb) + 1;
        cr = (r - y) / (1 - kr) + 1;
        first[i] = Cast(y * 255);
        second[i] = Cast(cb * 255 / 2);
        third[i] = Cast(cr * 255 / 2);
    }
}

void RGB_to_YCoCg(int width, int height, double* first, double* second, double* third) {
    double r, g, b;
    for(int i = 0; i < width * height; i++) {
        r = first[i] / 255.0; g = second[i] / 255.0; b = third[i] / 255.0;
        first[i] = Cast((0.25 * r + 0.5 * g + 0.25 * b) * 255);
        third[i] = Cast((-0.25 * r + 0.5 * g - 0.25 * b + 0.5) * 255);
        second[i] = Cast((0.5 * r - 0.5 * b + 0.5) * 255);
    }
}