//
// Created by user on 18.02.2020.
//

#include "ppmwork.h"
#include <fstream>
using namespace std;

void Inverse1(string str, int width, int height, int a, string filename, char* arr){
    ofstream fout(filename, ios::binary);
    if(!fout){
        cout << "Cannot create file!\n";
        return;
    }
    fout << str << "\n" << width << "\n" << height << "\n" << a << "\n";
    for (int i = 1; i <= height * width * 3; i++)
        fout << char(255 - (int)arr[i]);
}

void HorisonReflection1(string str, int width, int height, int a, string filename, char* arr) {
    ofstream fout(filename, ios::binary);
    if(!fout){
        cout << "Cannot create file!\n";
        return;
    }
    fout << str << "\n" << width << " " << height << "\n" << a << "\n";
    for(int i = height - 1; i >= 0; i--)
        for(int j = 1; j <= width * 3 - 2; j += 3)
            fout << arr[i * width * 3 + j] << arr[i * width * 3 + j + 1] << arr[i * width * 3 + j + 2];
    fout.close();
}

void VerticalReflection1(string str, int width, int height, int a, string filename, char* arr) {
    ofstream fout(filename, ios::binary);
    if(!fout){
        cout << "Cannot create file!\n";
        return;
    }
    fout << str << "\n" << width << " " << height << "\n" << a << "\n";
    for(int i = 0; i < height; i++)
        for(int j = width * 3; j >= 3; j -= 3)
            fout << arr[i * width * 3 + j - 2] << arr[i * width * 3 + j - 1] << arr[i * width * 3 + j];
    fout.close();
}

void ClockwiseRotation1(string str, int width, int height, int a, string filename, char* arr){
    ofstream fout(filename, ios::binary);
    if(!fout){
        cout << "Cannot create file!\n";
        return;
    }
    fout << str << "\n" << height << " " << width << "\n" << a << "\n";
    for(int i = 1; i < width * 3 - 1 ; i += 3)
        for(int j = height - 1; j >= 0; j--)
            fout << arr[j * width * 3 + i] << arr[j * width * 3 + i + 1] << arr[j * width * 3 + i + 2];
    fout.close();
}

void CounterclockwiseRotation1(string str, int width, int height, int a, string filename, char* arr){
    ofstream fout(filename, ios::binary);
    if(!fout){
        cout << "Cannot create file!\n";
        return;
    }
    fout << str << "\n" << height << " " << width << "\n" << a << "\n";
    for(int i = width * 3; i >= 2; i -= 3)
        for(int j = 0; j < height; j++)
            fout << arr[j * width * 3 + i - 2] << arr[j * width * 3 + i - 1] << arr[j * width * 3 + i];
    fout.close();
}