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

#include "../include/descriptors_helper.hpp"

template<typename T>
string to_string(T value) {
	ostringstream os;
	os << value;
	return os.str();
}


string exec_cmd(string cmd) {
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}

int main(int argc, const char * argv[]) {

	if (argc < 3) {
		printf("Usage: ./predict descriptor svm_state_file file_name\n");
		exit(1);
	}


	string s_descriptor(argv[1]);
	const char* s_svm_state(argv[2]);
	string s_filename(argv[3]);

	// Load SVM
	CvSVM svm;
	svm.load(s_svm_state);

	string ppm_img = s_filename;
	if (s_filename.find("jpg") != string::npos)
		ppm_img.replace(s_filename.find("jpg"), 3, "ppm");
	else if (s_filename.find("png") != string::npos)
		ppm_img.replace(s_filename.find("png"), 3, "ppm");
	else {
		printf("Incorrect image type, try png or jpg");
		exit(2);
	}

	string bin_file = ppm_img;
	bin_file.replace(bin_file.find("ppm"), 3, s_descriptor + ".bin");

	string convert_cmd = "convert " + s_filename + " " + ppm_img;
	printf(exec_cmd(convert_cmd).c_str());
//	if (system(convert_cmd.c_str())) {
//		printf("Error while converting image to .ppm. %s ", convert_cmd.c_str());
//		exit(3);
//	}
	string extractor_cmd = "./extractors/" + s_descriptor + "_extraction " + ppm_img + " " + bin_file;

	printf(exec_cmd(extractor_cmd).c_str());
//	if (system(convert_cmd.c_str())) {
//		printf("Error extracting descriptor. %s", extractor_cmd.c_str());
//		exit(3);
//	}
	// Read file
	streampos size;
	ifstream histogram;
	string line;

	// Grabbing the first record data to create the dataset in Opencv and set some constant values

	int bin_size = 0;
	if (s_descriptor.compare("bic") == 0) {
		bin_size = ReadFvChar1DBinSize(bin_file.c_str());
	} else
		bin_size = ReadFileFeatureVector1DBinSize(bin_file.c_str());

	float result = predict(bin_size, s_descriptor, bin_file, svm);

	cout << "Result = " << names[(int) result] << "\n";
	return EXIT_SUCCESS;
}
