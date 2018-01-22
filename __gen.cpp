/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */

#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace cv::face;
using namespace std;

static void trainEigen(string model , vector<Mat>& images, vector<int>& labels, int argc, string output_folder);

static Mat norm_0_255(InputArray _src) {
    Mat src = _src.getMat();
    // Create and return normalized image:
    Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

static void read_csv(const string& filename, 
                    vector<Mat>& images, vector<int>& labels,
                     vector<Mat>& dImages, vector<int>& dLabels, 
                    char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }
    string line;
    while (getline(file, line)) {
        if(!line.empty() ) {
            size_t found = line.find("label");
            if (found == string::npos)
            {
                cout << "error!" << '\n';
                exit(1);
            }
            string label = line.substr(found + 6, 1);
            
            found = line.find("avg");
            if (found != string::npos)
            {

                images.push_back(imread(line, 0));
                labels.push_back(atoi(label.c_str()));
                cout << line <<"\t\t"<<label<<endl;;
            }
            else
            {

                dImages.push_back(imread(line, 0));
                dLabels.push_back(atoi(label.c_str()));
                cout << line <<"\t\t"<<label<<"\tdiff"<<endl;;
            }
            
        }
        if (images.size() > 500){
            return ;
        }
    }
}

int main(int argc, const char *argv[]) {
    
    string output_folder = "..";
    if (argc == 2) {
        output_folder = string(argv[1]);
    }
   
   string train = "../data/training.csv";
    // These vectors hold the images and corresponding labels.
    vector<Mat> images, dImages, testImages, testDImages;
    vector<int> labels, dLabels, testLabels, testDlabels;


    try {
        read_csv(train, images,labels,dImages, dLabels);
        
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << train<< "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    // Quit if there are not enough images for this demo.
    if(images.size() <= 1) {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(Error::StsError, error_message);
    }

    
    

    trainEigen ("average ", images,  labels, argc, output_folder);


    if(argc == 1) {
        waitKey(0);
    }
    return 0;
}

static void trainEigen (string modelname , vector<Mat>& images, vector<int>& labels, int argc, string output_folder){
    int height = images[0].rows;
    cout << "size is "<<images.size()<<endl;

    Mat testSample = images[images.size() - 1];
    int testLabel = labels[labels.size() - 1];
    images.pop_back();
    labels.pop_back();


    cout << "creating model for training.."<<endl;
    Ptr<EigenFaceRecognizer> model = EigenFaceRecognizer::create();
    cout << "training the " << modelname<<"model.."<<endl;
    model->train(images, labels);
    cout << "training is done.."<<endl;
    // The following line predicts the label of a given
    // test image:
    int predictedLabel = model->predict(testSample);

    string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
    cout << result_message << endl;
    // Here is how to get the eigenvalues of this Eigenfaces model:
    Mat eigenvalues = model->getEigenValues();
    // And we can do the same to display the Eigenvectors (read Eigenfaces):
    Mat W = model->getEigenVectors();
    // Get the sample mean from the training data
    Mat mean = model->getMean();
    // Display or save:
    if(argc == 1) {
        imshow(modelname+"mean", norm_0_255(mean.reshape(1, images[0].rows)));
    } else {
        imwrite(format("%s/%smean.png", output_folder.c_str(),modelname.c_str()), norm_0_255(mean.reshape(1, images[0].rows)));
    }
    // Display or save the Eigenfaces:
    for (int i = 0; i < min(10, W.cols); i++) {
        string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
        cout << msg << endl;
        // get eigenvector #i
        Mat ev = W.col(i).clone();
        // Reshape to original size & normalize to [0...255] for imshow.
        Mat grayscale = norm_0_255(ev.reshape(1, height));
        // Show the image & apply a Jet colormap for better sensing.
        Mat cgrayscale;
        applyColorMap(grayscale, cgrayscale, COLORMAP_JET);
        // Display or save:
        if(argc == 1) {
            imshow(format("%seigenface_%d",modelname.c_str(), i), cgrayscale);
        } else {
            imwrite(format("%s/%seigenface_%d.png", output_folder.c_str(),modelname.c_str(), i), norm_0_255(cgrayscale));
        }
    }
}