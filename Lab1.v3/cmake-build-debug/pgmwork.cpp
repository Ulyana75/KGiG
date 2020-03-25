//
// Created by user on 18.02.2020.
//

#include "pgmwork.h"
#include <fstream>
using namespace std;

void Inverse(string str, int width, int height, int a, string filename, char* arr){
    ofstream fout(filename, ios::binary);
    if(!fout){
        cout << "Cannot create file!\n";
        return;
    }
    fout << str << "\n" << width << " " << height << "\n" << a << "\n";
    for (int i = 1; i <= height * width; i++)
        fout << char(255 - (int)arr[i]);
    fout.close();
}

void VerticalReflection(string str, int width, int height, int a, string filename, char* arr) {
    ofstream fout(filename, ios::binary);
    if(!fout){
        cout << "Cannot create file!\n";
        return;
    }
    fout << str << "\n" << width << " " << height << "\n" << a << "\n";
    for(int i = height - 1; i >= 0; i--)
        for(int j = 1; j <= width; j++)
            fout << arr[i * width + j];
    fout.close();
}

void HorisonReflection(string str, int width, int height, int a, string filename, char* arr) {
    ofstream fout(filename, ios::binary);
    if(!fout){
        cout << "Cannot create file!\n";
        return;
    }
    fout << str << "\n" << width << " " << height << "\n" << a << "\n";
    for(int i = 0; i < height; i++)
        for(int j = width; j >= 1; j--)
            fout << arr[i * width + j];
    fout.close();
}

void ClockwiseRotation(string str, int width, int height, int a, string filename, char* arr){
    ofstream fout(filename, ios::binary);
    if(!fout){
        cout << "Cannot create file!\n";
        return;
    }
    fout << str << "\n" << height << " " << width << "\n" << a << "\n";
    for(int i = 1; i <= width; i++)
        for(int j = height - 1; j >= 0; j--)
            fout << arr[j * width + i];
    fout.close();
}

void CounterclockwiseRotation(string str, int width, int height, int a, string filename, char* arr){
    ofstream fout(filename, ios::binary);
    if(!fout){
        cout << "Cannot create file!\n";
        return;
    }
    fout << str << "\n" << height << " " << width << "\n" << a << "\n";
    for(int i = width; i >= 1; i--)
        for(int j = 0; j < height; j++)
            fout << arr[j * width + i];
    fout.close();
}