#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <opencv2/core.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "__colors.hpp"

using namespace std;
using namespace cv;


struct FrameRange{
    bool exist;
    int start;
    int end;
};
//enum Scene{outdoors, outdoorsMovingCamer, outdoorsWithClothes,indoors};
struct Meta{
    int label;
    //Scene scene;
    string filename;
    FrameRange range[4];
};

struct Info{
    string filename;
    int label;
}; 

#endif