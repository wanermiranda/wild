/*
 * descriptors.cpp
 *
 *  Created on: Jun 23, 2015
 *      Author: gorigan
 */
#include "../include/descriptors_helper.hpp"
/* -------------------------------------------------------------------------------------
 Split the line into filename and class
 line = string containing the line data, formatted as filename#class
 descriptor = descriptor name
 returns = 2 position vector of string, first the file and then the class
 -------------------------------------------------------------------------------------*/
vector<string> getFileNameClass(string line, string descriptor) {
	string delimiter = "#";
	vector<string> results;
	size_t pos = 0;
	std::string token;
	if ((pos = line.find(delimiter)) != string::npos) {
		token = line.substr(0, pos);
		token.replace(token.find("descriptor"), 10, descriptor);
		results.push_back(token);
		line.erase(0, pos + delimiter.length());
		results.push_back(line);
	}
	return results;
}

/* -------------------------------------------------------------------------------------
 Read the first position of the descriptor file, getting the
 filename = the file path for the feature vector byte set
 returns = the feature vector size
 -------------------------------------------------------------------------------------*/

int ReadFileFeatureVector1DBinSize(const char *filename) {
	int n;
	FILE *f;
	if ((f = fopen(filename, "rb")) == NULL) {
		printf("Read Bin Size, error opening: %s\n", filename);
		exit(0);
	}
	fread(&n, sizeof(int), 1, f);

	fclose(f);
	return n;
}

int *ReadFileFv1DBinChar(const char *filename) {
	int i, nbins;
	FILE *fp;
	uchar c;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Read Vector Char, error opening: %s\n", filename);
		exit(-1);
	}

	fscanf(fp, "%d\n", &nbins);
	int *results = (int *) malloc(sizeof(int) * nbins);

	for (i = 0; i < nbins; i++) {
		fscanf(fp, "%c", &c);
		results[i] = c - '0';
	}
	fclose(fp);

	return results;
}
/* -------------------------------------------------------------------------------------
 Read the feature vector from the file
 filename  = the file path for the feature vector byte set
 returns = A array of double
 -------------------------------------------------------------------------------------*/

double *ReadFileFv1DBinDouble(const char *filename) {
	int n;
	FILE *f;
	if ((f = fopen(filename, "rb")) == NULL) {
		printf("Read Vector Double, error opening: %s\n", filename);
		exit(0);
	}
	fread(&n, sizeof(int), 1, f);

	double *fv = (double *) malloc(sizeof(double) * n);

	fread(fv, sizeof(double), n, f);
	fclose(f);

	return fv;
}

float *ReadFileFv1DBinFloat(const char *filename) {
	int n;
	FILE *f;
	if ((f = fopen(filename, "rb")) == NULL) {
		printf("Read Vector Float, error opening: %s\n", filename);
		exit(0);
	}
	fread(&n, sizeof(int), 1, f);

	float *fv = (float *) malloc(sizeof(float) * n);

	fread(fv, sizeof(float), n, f);
	fclose(f);

	return fv;
}

int ReadFvChar1DBinSize(const char *filename) {
	int nbins;
	FILE *fp;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Read Vector Char, error opening: %s\n", filename);
		exit(-1);
	}

	fscanf(fp, "%d\n", &nbins);

	fclose(fp);

	return nbins;
}



float predict(int bin_size, const string& descriptor,
		const string& filename, const CvSVM& svm) {
	Mat predict(1, bin_size, CV_32FC1);
	// Some descriptors are written using float, instead of double.
	if (descriptor.compare("sasi") == 0) {
		double* fv_double;
		fv_double = ReadFileFv1DBinDouble(filename.c_str());
		for (int i = 0; i < bin_size; i++) {
			predict.at<float>(0, i) = fv_double[i];
		}
		free(fv_double);
	} else if (descriptor.compare("eoac") == 0) {
		float* fv_float;
		fv_float = ReadFileFv1DBinFloat(filename.c_str());
		for (int i = 0; i < bin_size; i++) {
			predict.at<float>(0, i) = fv_float[i];
		}
		free(fv_float);
	} else if (descriptor.compare("bic") == 0) {
		int* fv_int;
		fv_int = ReadFileFv1DBinChar(filename.c_str());
		for (int i = 0; i < bin_size; i++) {
			predict.at<float>(0, i) = fv_int[i];
		}
		free(fv_int);
	}

	return svm.predict(predict);

}

