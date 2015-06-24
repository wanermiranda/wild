/*
 * descriptors_helper.hpp
 *
 *  Created on: Jun 23, 2015
 *      Author: gorigan
 */

#ifndef INCLUDE_DESCRIPTORS_HELPER_HPP_
#define INCLUDE_DESCRIPTORS_HELPER_HPP_


#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <bitset>
#include "cv.h"
#include "ml.h"
#include "highgui.h"


using namespace std;
using namespace cv;

const int num_class = 8;
const int tm_train = 2624;
const int tm_valid = 656;
const string names[8] = { "apple", "car", "cow", "cup", "dog", "horse", "pear",
		"tomato" };


/* -------------------------------------------------------------------------------------
 Split the line into filename and class
 line = string containing the line data, formatted as filename#class
 descriptor = descriptor name
 returns = 2 position vector of string, first the file and then the class
 -------------------------------------------------------------------------------------*/
vector<string> getFileNameClass(string line, string descriptor);

/* -------------------------------------------------------------------------------------
 Read the first position of the descriptor file, getting the
 filename = the file path for the feature vector byte set
 returns = the feature vector size
 -------------------------------------------------------------------------------------*/

int ReadFileFeatureVector1DBinSize(const char *filename);

int *ReadFileFv1DBinChar(const char *filename);
/* -------------------------------------------------------------------------------------
 Read the feature vector from the file
 filename  = the file path for the feature vector byte set
 returns = A array of double
 -------------------------------------------------------------------------------------*/

double *ReadFileFv1DBinDouble(const char *filename);

float *ReadFileFv1DBinFloat(const char *filename);

int ReadFvChar1DBinSize(const char *filename);


float predict(int bin_size, const string& descriptor,
		const string& filename, const CvSVM& svm);

#endif /* INCLUDE_DESCRIPTORS_HELPER_HPP_ */
