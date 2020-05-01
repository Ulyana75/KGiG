//
// Created by user on 30.04.2020.
//

#ifndef LAB4_CONVERTIONS_H
#define LAB4_CONVERTIONS_H

void CMY_to_RGB(int width, int height, double* first, double* second, double* third);
void HSL_to_RGB(int width, int height, double* first, double* second, double* third);
void HSV_to_RGB(int width, int height, double* first, double* second, double* third);
void YCbCr601_to_RGB(int width, int height, double* first, double* second, double* third);
void YCbCr709_to_RGB(int width, int height, double* first, double* second, double* third);
void YCoCg_to_RGB(int width, int height, double* first, double* second, double* third);
void RGB_to_CMY(int width, int height, double* first, double* second, double* third);
void RGB_to_HSL(int width, int height, double* first, double* second, double* third);
void RGB_to_HSV(int width, int height, double* first, double* second, double* third);
void RGB_to_YCbCr601(int width, int height, double* first, double* second, double* third);
void RGB_to_YCbCr709(int width, int height, double* first, double* second, double* third);
void RGB_to_YCoCg(int width, int height, double* first, double* second, double* third);

#endif //LAB4_CONVERTIONS_H
