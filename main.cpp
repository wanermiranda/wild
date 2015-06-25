/*------------------------------------------------------------------------------------
 tp3.cpp
 Ruth Keglevich
 Waner Miranda
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

#include "include/descriptors_helper.hpp"

using namespace cv;
using namespace std;


template<typename T>
string to_string(T value) {
	ostringstream os;
	os << value;
	return os.str();
}

std::string base_name(std::string const & path)
{
  return path.substr(path.find_last_of("/\\") + 1);
}

/* -------------------------------------------------------------------------------------
 Build the matrix data training, configurate and performs the SVM train.
 hist_size      = size of histogram file
 s_trainset_file   = data for training
 s_dir_svm     = svm result file
 ---------------------------------------------------------------------------------- */
void train_svm(string s_trainset_file, const char * s_dir_svm,
		string descriptor) {

	ifstream ifs(s_trainset_file.c_str());

	string line;

	vector<string> file_class;
	int bin_size;

	int class_id;
	string svm_state_file(s_dir_svm);

	svm_state_file.append("/");
	svm_state_file.append(base_name(s_trainset_file));
	svm_state_file.replace(svm_state_file.find(".set"), 4,
			"." + descriptor + ".xml");

	// Grabbing the first record data to create the dataset in Opencv and set some constant values
	if (std::getline(ifs, line)) {
		file_class = getFileNameClass(line, descriptor);
		if (descriptor.compare("bic") == 0) {
			bin_size = ReadFvChar1DBinSize(file_class[0].c_str());
		} else
			bin_size = ReadFileFeatureVector1DBinSize(file_class[0].c_str());

	}

	Mat training_mat(tm_train, bin_size, CV_32FC1);
	Mat labels(tm_train, 0, CV_32FC1);

	// Now, reading the whole dataset
	int rows = 0;
	do {
		file_class = getFileNameClass(line, descriptor);
		class_id = atoi(file_class[1].c_str());

		// Some descriptors are written using float, instead of double.
		if (descriptor.compare("sasi") == 0) {
			double *fv_double;
			fv_double = ReadFileFv1DBinDouble(file_class[0].c_str());
			for (int i = 0; i < bin_size; i++) {
				training_mat.at<float>(rows, i) = fv_double[i];
			}
			free(fv_double);
		} else if (descriptor.compare("eoac") == 0) {
			float *fv_float;
			fv_float = ReadFileFv1DBinFloat(file_class[0].c_str());
			for (int i = 0; i < bin_size; i++) {
				training_mat.at<float>(rows, i) = fv_float[i];
			}
			free(fv_float);

		} else if (descriptor.compare("bic") == 0) {
			int *fv_int;
			fv_int = ReadFileFv1DBinChar(file_class[0].c_str());
			for (int i = 0; i < bin_size; i++) {
				training_mat.at<float>(rows, i) = fv_int[i];
			}
			free(fv_int);
		}
		labels.push_back((float) class_id);
		rows++;
	} while (std::getline(ifs, line));

	CvSVM svm;

	CvParamGrid CvParamGrid_C(pow(2.0, -5), pow(2.0, 15), pow(2.0, 2));
	CvParamGrid CvParamGrid_gamma(pow(2.0, -15), pow(2.0, 3), pow(2.0, 2));
	if (!CvParamGrid_C.check() || !CvParamGrid_gamma.check())
		cout << "The grid is NOT VALID." << endl;
	CvSVMParams paramz;
	paramz.kernel_type = CvSVM::RBF;
	paramz.svm_type = CvSVM::C_SVC;
	paramz.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 0.000001);
	svm.train_auto(training_mat, labels, Mat(), Mat(), paramz, 10,
			CvParamGrid_C, CvParamGrid_gamma, CvSVM::get_default_grid(CvSVM::P),
			CvSVM::get_default_grid(CvSVM::NU),
			CvSVM::get_default_grid(CvSVM::COEF),
			CvSVM::get_default_grid(CvSVM::DEGREE), true);
	paramz = svm.get_params();
	cout << "gamma:" << paramz.gamma << endl;
	cout << "C:" << paramz.C << endl;

	svm.save(svm_state_file.c_str()); // saving
	ifs.close();
}



/* -------------------------------------------------------------------------------------
 Executes the valition based on the saved state
 s_validset_file   = dataset for validation
 s_dir_svm     = svm file
 s_dir_result  = prediction result
 ---------------------------------------------------------------------------------- */
void valid_svm(string s_validset_file, const char * s_dir_svm,
		string s_dir_result, string descriptor) {

	string line;

	ifstream ifs(s_validset_file.c_str());
	ofstream fresult;
	s_dir_result.append("/");
	s_dir_result.append(base_name(s_validset_file));
	s_dir_result.replace(s_dir_result.find(".set"), 4,
			"." + descriptor + ".res");

	fresult.open(s_dir_result.c_str());

	vector<string> file_class;
	int bin_size;
	int class_id;
	string svm_state_file(s_dir_svm);

//	svm_state_file.append("/");
//	svm_state_file.append(s_validset_file);
//	svm_state_file.replace(svm_state_file.find(".set"), 4,
//			"." + descriptor + ".xml");
//	svm_state_file.replace(svm_state_file.find("test"), 4, "train");

	// Grabbing the first record data to create the dataset in Opencv and set some constant values
	if (std::getline(ifs, line)) {
		file_class = getFileNameClass(line, descriptor);
		if (descriptor.compare("bic") == 0) {
			bin_size = ReadFvChar1DBinSize(file_class[0].c_str());
		} else
			bin_size = ReadFileFeatureVector1DBinSize(file_class[0].c_str());

	}
	CvSVM svm;
	svm.load(svm_state_file.c_str());

	// Now, reading the whole dataset
	while (std::getline(ifs, line)) {
		file_class = getFileNameClass(line, descriptor);
		class_id = atoi(file_class[1].c_str());

		float result = predict(bin_size, descriptor, file_class[0].c_str(), svm);
		fresult << names[class_id] << " = " << names[(int) result] << "\n";
	}

	ifs.close();
	fresult.close();
}

/* -------------------------------------------------------------------------------------
 Compiling (makefile - make) :
 g++ `pkg-config opencv --cflags` tp3.cpp -o tp3 `pkg-config opencv --libs`

 Run (makefile - make run) :
 ./tp3 $(ARG1) $(ARG2) $(ARG3) $(ARG4) $(ARG5)

 $(ARG1) = Descriptor (BIC, SASI, EOAC)
 $(ARG2) = svm file path
 $(ARG3) = result path
 $(ARG4) = Mode (TRAIN, VALID)
 $(ARG5) = histograms path
 ---------------------------------------------------------------------------------- */
int main(int argc, const char * argv[]) {

	if (argc < 6) {
		printf("Usage: ./tp3 descriptor dir_svm dir_result s_mode s_dataset\n");
		exit(1);
	}

	string s_descriptor(argv[1]);
	const char* s_dir_svm(argv[2]);
	const char* s_dir_result(argv[3]);
	string s_mode(argv[4]);
	string s_dataset(argv[5]);

	printf("Descriptor: %s \n", s_descriptor.c_str());
	// Process
	if (s_mode.compare("TRAIN") == 0) {
		train_svm(s_dataset, s_dir_svm, s_descriptor);
	} else
		valid_svm(s_dataset, s_dir_svm, s_dir_result, s_descriptor);

	return EXIT_SUCCESS;
}
