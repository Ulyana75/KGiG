#include <iostream>
#include <fstream>
#include "cmake-build-debug/pgmwork.h"
#include "cmake-build-debug/ppmwork.h"
using namespace std;

int width, height, a;
string str;
char* arr;
string filename_in, filename_out, command;

void ItsPGM(ifstream &fin) {
    arr = new char [height * width + 1];
    fin.read(arr, height * width + 1);
    if (fin)
        cout << "all characters read successfully.";
    else {
        cout << "error: only " << fin.gcount() << " could be read";
        return;
    }
    fin.close();
    if(command == "0") Inverse(str, width, height, a, filename_out, arr);
    else if(command == "1") HorisonReflection(str, width, height, a, filename_out, arr);
    else if(command == "2") VerticalReflection(str, width, height, a, filename_out, arr);
    else if(command == "3") ClockwiseRotation(str, width, height, a, filename_out, arr);
    else if(command == "4") CounterclockwiseRotation(str, width, height, a, filename_out, arr);
    else cout << "Wrong command!\n";
    delete[] arr;
}

void ItsPPM(ifstream &fin) {
    arr = new char [height * width * 3 + 1];
    fin.read(arr, height * width * 3 + 1);
    if (fin)
        std::cout << "all characters read successfully.";
    else {
        std::cout << "error: only " << fin.gcount() << " could be read";
        return;
    }
    fin.close();
    if(command == "0") Inverse1(str, width, height, a, filename_out, arr);
    else if(command == "1") HorisonReflection1(str, width, height, a, filename_out, arr);
    else if(command == "2") VerticalReflection1(str, width, height, a, filename_out, arr);
    else if(command == "3") ClockwiseRotation1(str, width, height, a, filename_out, arr);
    else if(command == "4") CounterclockwiseRotation1(str, width, height, a, filename_out, arr);
    else cout << "Wrong command!\n";
    delete[] arr;
}

int main(int argc, char* argv[])
{
    if(argc < 4){
        cout << "Wrong arguments!\n";
        return 1;
    }
    filename_in = argv[1];
    filename_out = argv[2];
    command = argv[3];
    ifstream fin(filename_in, ios::binary);
    if (!fin) {
        cout << "Cannot open file\n";
        return 1;
    }
    if(fin.good())
        fin >> str;
    else{
        cout << "Wrong\n";
        exit(1);
    }
    if(fin.good())
        fin >> width;
    else {
        cout << "Wrong\n";
        exit(1);
    }
    if(fin.good())
        fin >> height;
    else {
        cout << "Wrong\n";
        exit(1);
    }
    if(fin.good())
        fin >> a;
    else {
        cout << "Wrong\n";
        exit(1);
    }
    if((width <= 0) || (height <= 0) || (a != 255)){
        cout << "Something gone wrong!\n";
        exit(1);
    }
    if(str == "P5")
        ItsPGM(fin);
    else if(str == "P6")
        ItsPPM(fin);
    else {
        cout << "Wrong format!\n";
        return 1;
    }
    fin.close();
    return 0;
}
