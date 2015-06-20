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
const int num_class      = 8;
const int tm_train       = 2624;
const int tm_valid       = 656;
const string names[8]    = { "apple", "car", "cow", "cup", "dog", "horse", "pear", "tomato" };

template <typename T>
string to_string(T value){
  ostringstream os ;
  os << value ;
  return os.str() ;
}

/* -------------------------------------------------------------------------------------
 Split the line into filename and class
 line = string containing the line data, formatted as filename#class
 returns = 2 position vector of string, first the file and then the class
 -------------------------------------------------------------------------------------*/
vector<string> getFileNameClass(char *line)  {    
    string delimiter = "#";
    vector<string> results; 
    size_t pos = 0;
    std::string token;
    while ((pos = line.find(delimiter)) != string::npos) {
        token = line.substr(0, pos);
        results.push_back(token)
        line.erase(0, pos + delimiter.length());
    }
    return results;
}

/* -------------------------------------------------------------------------------------
 Read the first position of the descriptor file, getting the 
 filename = the file path for the feature vector byte set
 returns = the feature vector size
 -------------------------------------------------------------------------------------*/

int ReadFileFeatureVector1DBinSize(char *filename) {
    FeatureVector1D *fv=NULL;
    int n; 
    FILE *f;
    if ((f = fopen(filename, "rb")) == NULL) {
        printf("erro abrindo arquivo: %s\n", filename);
        exit(0);
    }
    fread(&n,sizeof(int),1,f);
    
    return n;     
}

/* -------------------------------------------------------------------------------------
 Read the feature vector from the file 
 filename  = the file path for the feature vector byte set
 returns = A array of double 
 -------------------------------------------------------------------------------------*/

double *ReadFileFeatureVector1DBin(char *filename) {
    FeatureVector1D *fv=NULL;
    int n; 
    FILE *f;
    if ((f = fopen(filename, "rb")) == NULL) {
        printf("erro abrindo arquivo: %s\n", filename);
        exit(0);
    }
    fread(&n,sizeof(int),1,f);
    
    double *fv = malloc(sizeof(double)*n);     

    fv = CreateFeatureVector1D(n);
    fread(fv,sizeof(double),n, f);
    fclose(f);

    return fv;
}

/* -------------------------------------------------------------------------------------
   Return class id based on the range of files
   The folder must contain the same number of sorted files for each class
   x         = current file number
   num_files = total files
   ---------------------------------------------------------------------------------- */
int getClassId(int x, int num_files){

    int id_class;
    if (x <= num_files)
        id_class = 0;  // apple
    else if (x <= (num_files*2) )
        id_class = 1;  // car
    else if (x <= (num_files*3) )
        id_class = 2;  // cow
    else if (x <= (num_files*4) )
        id_class = 3;  // cup
    else if (x <= (num_files*5) )
        id_class = 4;  // dog
    else if (x <= (num_files*6) )
        id_class = 5;  // horse
    else if (x <= (num_files*7) )
        id_class = 6;  // cpear
    else
        id_class = 7;  // tomato
    return id_class;
}

/* -------------------------------------------------------------------------------------
   Build the matrix data training, configurate and performs the SVM train.
   hist_size      = size of histogram file
   s_trainset_file   = data for training
   s_dir_svm     = svm result file
   ---------------------------------------------------------------------------------- */
void train_binary (int hist_size, string s_trainset_file, const char * s_dir_svm, double gamma){

    ifstream ifs(s_trainset_file);

    string line;

    while(std::getline(ifs, line))
    {
        // do whatever you want with the line here
    }
}

/* -------------------------------------------------------------------------------------
   Build the matrix data training, configurate and performs the SVM train.
   img_area      = size of histogram file
   s_dir_train   = data for training
   s_dir_svm     = svm result file
   ---------------------------------------------------------------------------------- */
void train (int img_area, string s_dir_train, const char * s_dir_svm, double gamma){

    Mat training_mat(tm_train,img_area,CV_32FC1);
    Mat labels(tm_train, 0 , CV_32FC1);

    streampos size;
    char * memblock;

    int rows = 0;
    int num_files = tm_train/num_class;

    // Build training matrix
    for (int x = 1; x<=tm_train; x++) {

        ifstream histogram;
        string line;
        int id_class = getClassId(x,num_files);

        ostringstream count;
        count << setw(4) << setfill('0') << x;
        string filename = s_dir_train + count.str() + ".bin";

        histogram.open(filename.c_str(), ios::in|ios::binary|ios::ate);

        if (histogram.is_open()) {

            size     = histogram.tellg();
            memblock = new char [size];

            histogram.seekg (0, ios::beg);
            histogram.read (memblock, size);
            histogram.close();

            for (int i = 0; i<size; i++) {
                bitset<8>binarya(memblock[i]);
                training_mat.at<float>(rows,i) = binarya.to_ulong();
            }

            labels.push_back((float) id_class);
            rows++;
        }
    }

    // SVM parameters congif
    CvSVMParams param        = CvSVMParams();
    param.svm_type           = CvSVM::C_SVC;
    param.kernel_type        = CvSVM::RBF;
    param.term_crit.type     = CV_TERMCRIT_ITER +CV_TERMCRIT_EPS;
    param.class_weights      = NULL;
    param.degree             = 2;
    param.gamma              = gamma; //0.001;
    param.coef0              = 1;
    param.C                  = 7;
    param.nu                 = 0.0;
    param.p                  = 0.0;
    param.term_crit.max_iter = 1000000;
    param.term_crit.epsilon  = 1e-6;

    CvSVM svm(training_mat, labels, cv::Mat(), cv::Mat(), param);
    svm.save(s_dir_svm); // saving
}

/* -------------------------------------------------------------------------------------
   img_area      = size of histogram file
   s_dir_valid   = data
   s_dir_svm     = svm file
   s_dir_result  = prediction result
   ---------------------------------------------------------------------------------- */
void valid (int img_area, string s_dir_valid, const char * s_dir_svm, const char * s_dir_result){

    ofstream fresult;
    fresult.open(s_dir_result);
    int num_files = tm_valid/num_class;

    streampos size;
    char * memblock;

    CvSVM svm;
    svm.load(s_dir_svm);

    for (int x = 1; x<=tm_valid; x++) {

        ifstream histogram;
        string line;
        Mat predict(1,img_area,CV_32FC1);

        int id_class = getClassId(x, num_files);

        ostringstream count;
        count << setw(3) << setfill('0') << x;
        string filename = s_dir_valid + count.str() + ".bin";
        histogram.open(filename.c_str(), ios::in|ios::binary|ios::ate);

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

        float result = svm.predict(predict);
        fresult << names[id_class] << " = " << names[(int)result] << "\n";
    }
    fresult.close();
}

/* -------------------------------------------------------------------------------------
    Compiling (makefile - make) :
     g++ `pkg-config opencv --cflags` tp3.cpp -o tp3 `pkg-config opencv --libs`

    Run (makefile - make run) :
    ./tp3 $(ARG1) $(ARG2) $(ARG3) $(ARG4) $(ARG5)

   $(ARG1) = Descriptor (BIC, LCH, UNSER)
   $(ARG2) = svm file path
   $(ARG3) = result path
   $(ARG4) = Mode (TRAIN, VALID)
   $(ARG5) = histograms path
   ---------------------------------------------------------------------------------- */
int main (int argc, const char * argv[]){

    if (argc < 6) {
        printf ("Usage: ./tp3 descriptor dir_svm dir_result s_mode s_dir_files\n");
        exit(1);
    }


    string s_descriptor      (argv[1]);
    const char* s_dir_svm    (argv[2]);
    const char* s_dir_result (argv[3]);
    string s_mode            (argv[4]);
    string s_dir_files       (argv[5]);
    int img_area = 0;
    double gamma = 0;


    // Descriptor configs
    if (s_descriptor.compare("BIC") == 0){
        gamma = 0.01D;
        img_area = img_area_bic;
    }

    else if (s_descriptor.compare("LCH") == 0){
        gamma = 0.001D;
        img_area = img_area_lch;
    }

    else if (s_descriptor.compare("UNSER") == 0){
        gamma = 0.01D;
        img_area = img_area_unser;
    }

    else{
        cout << "Unpredicted descriptor. Try bic, lch or unser \n" << endl;
        return EXIT_FAILURE;
    }

    // Process
    if (s_mode.compare("TRAIN") == 0)
        train (img_area, s_dir_files, s_dir_svm, gamma);
    else
        valid (img_area, s_dir_files, s_dir_svm, s_dir_result);


    return EXIT_SUCCESS;
}
/* TODO LIST
1. Change from directory to filelist the inputs either from train or test
2. Extract SASI as a texture descriptor
3. Extract EOAC as a shape descriptor
*/ 
