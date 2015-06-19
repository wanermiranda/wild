/*------------------------------------------------------------------------------------
    tp3.cpp
    Ruth Keglevich
------------------------------------------------------------------------------------*/
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <bitset>

#include "cv.h"
#include "ml.h"
#include "highgui.h"

using namespace cv;
using namespace std;

const int img_area_bic   = 128;
const int img_area_lch   = 1024;
const int img_area_unser = 32;
const string names[8]    = { "apple", "car", "cow", "cup", "dog", "horse", "pear", "tomato" };

template <typename T>
string to_string(T value){
  ostringstream os ;
  os << value ;
  return os.str() ;
}

int main (int argc, const char * argv[]){

    string s_descriptor   (argv[1]);
    const char* s_dir_svm (argv[2]);
    string s_filename     (argv[3]);
    int img_area = 0;

    // Descriptor configs
    if (s_descriptor.compare("BIC") == 0)
        img_area = img_area_bic;

    else if (s_descriptor.compare("LCH") == 0)
        img_area = img_area_lch;

    else if (s_descriptor.compare("UNSER") == 0)
        img_area = img_area_unser;
    else{
        cout << "Unpredicted descriptor. Try bic, lch or unser \n" << endl;
        return EXIT_FAILURE;
    }

    // Load SVM
    CvSVM svm;
    svm.load(s_dir_svm);

    // Read file
    streampos size;
    char * memblock;
    ifstream histogram;
    string line;

    Mat predict(1,img_area,CV_32FC1);
    histogram.open(s_filename.c_str(), ios::in|ios::binary|ios::ate);

    if (histogram.is_open()) {

        size = histogram.tellg();
        memblock = new char [size];
        histogram.seekg (0, ios::beg);
        histogram.read (memblock, size);
        histogram.close();
        for (int i = 0; i<size; i++) {

            bitset<8>binarya(memblock[i]);
            predict.at<float>(0,i) = binarya.to_ulong();
        }
    }

    // Predict
    float result = svm.predict(predict);
    cout << "Result = " << names[(int)result] << "\n";
    return EXIT_SUCCESS;
}
