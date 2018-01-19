#ifndef EIGEN_H
#define EIGEN_H

#include <iostream>
#include <fstream>
#include <sstream>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "_framerec.hpp"
#include "__colors.hpp"


using namespace cv;
using namespace std;

const String keys =
        "{help h usage ?    |   | print this message   }"
        "{load              |   | use the existing model in the data directory       }"
        "{loadFile          |   | use a given existing model}"
        "{train             |   | save training results into a file\n\t\t\033[1m\033[30mEx:\033[0m ./eigen -train=../data   }"
        "{test              |4 | id of the image to be tested\n\t\t\033[1m\033[30mEx:\033[0m ./eigen -test=44     }"
        "{output            |   | output folder for the results       }"
        
        ;


Mat norm_0_255(InputArray _src);
int  readCsv( vector<Mat>& images, vector<int>& labels, char separator = ',');
int  readTrainedCsv( vector<Mat>& images,vector<Mat>& diffImages, vector<int>& labels, char separator = ',');

#endif