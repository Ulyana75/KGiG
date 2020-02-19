//
// Created by user on 18.02.2020.
//

#ifndef LAB1_V3_PGMWORK_H
#define LAB1_V3_PGMWORK_H

#include <string>
#include <iostream>
using namespace std;

void Inverse(string str, int width, int height, int a, string filename, char* arr);                      //0
void HorisonReflection(string str, int width, int height, int a, string filename, char* arr);            //1
void VerticalReflection(string str, int width, int height, int a, string filename, char* arr);           //2
void ClockwiseRotation(string str, int width, int height, int a, string filename, char* arr);            //3
void CounterclockwiseRotation(string str, int width, int height, int a, string filename, char* arr);     //4

#endif //LAB1_V3_PGMWORK_H
