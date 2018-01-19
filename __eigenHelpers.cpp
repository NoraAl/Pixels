#include "__eigen.hpp"

Mat norm_0_255(InputArray _src) {
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

int  readCsv( vector<Mat>& images, vector<int>& labels, char separator) {
    ifstream metafile("../data/__metadata.csv", ifstream::in);
    string value; int columns;
    if (getline(metafile, value, ',')){
        columns = atoi(value.c_str());
    }
    cout << columns << " superpixels is being read."<<endl;

    ifstream xfile("../data/_x.csv", ifstream::in);
    ifstream yfile("../data/_y.csv", ifstream::in);
    ifstream c1file("../data/_color1.csv", ifstream::in);
    ifstream c2file("../data/_color2.csv", ifstream::in);
    ifstream c3file("../data/_color3.csv", ifstream::in);
    ifstream labelsfile("../data/_labels.csv", ifstream::in);
    
    if (!xfile || !yfile || !c1file || !c2file || !c3file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }

    string xline,yline,c1line,c2line,c3line;
    
    while (getline(xfile, xline) && getline(yfile, yline) && getline(c1file, c1line) && getline(c2file, c2line)&& getline(c3file, c3line))
    {
        // Now inside each line we need to seperate the cols

        Mat image (5,columns, CV_32F);//**
        int label;
        int j = 0;

        stringstream xstream(xline), ystream(yline), c1stream(c1line), c2stream(c2line), c3stream(c3line) ;
        
        string x,y,c1,c2,c3,l;
        getline(labelsfile, l);
        
            
        label = atoi(l.c_str());
       
        while (getline(xstream, x, ','))
        {
            
            if (!getline(ystream,y, ','))
                ;//error
                
            if (!getline(c1stream,c1, ','))
                ;//error
                //cout <<"c1"<<endl;
            if (!getline(c2stream,c2, ','))
                ;//error
                //cout <<"c2"<<endl;
            if (!getline(c3stream,c3, ','))
                ;//error
                //cout <<"c3--"<<j<<endl;
            
            
            
            image.at<float>(0,j) = atof(x.c_str());
            image.at<float>(1,j) = atof(y.c_str());
            image.at<float>(2,j) = atof(c1.c_str());
            image.at<float>(3,j) = atof(c2.c_str());
            image.at<float>(4,j) = atof(c3.c_str());

            j++;
        }
        // add the row to the complete data vector
        images.push_back(image);
        labels.push_back(label);
    
    }
    // close all files
    xfile.close(); yfile.close(); c1file.close(); c2file.close(); c3file.close(); labelsfile.close();
    return images.size();

}

int  readTrainedCsv( vector<Mat>& images, vector<Mat>& diffImages, vector<int>& labels, char separator) {
    cout << "Reading training images.."<<endl;
    ifstream metafile("../data/centsmeta.csv", ifstream::in);
    string value; int columns;
    if (getline(metafile, value, ',')){
        columns = atoi(value.c_str());
    }
    metafile.close();

    ifstream centroidsfile("../data/cents.csv", ifstream::in);
    ifstream labelsfile("../data/centslabels.csv", ifstream::in);

    ifstream diffsfile("../data/diff.csv", ifstream::in);
    
    if (!centroidsfile || !labelsfile || !diffsfile ) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }

    string line, diffline;
    
    while (getline(centroidsfile, line) && getline(diffsfile, diffline) )
    {
        // Now inside each line we need to seperate the cols

        Mat image (1,columns, CV_32F), diffImage(1,columns, CV_32F);//**
        int label;

        stringstream stream(line), diffStream(diffline);
        string point, diffpoint, l;
        
        // read labels
        getline(labelsfile, l);   
        label = atoi(l.c_str());

        int i = 0;
       
        while (getline(stream, point, ',') && getline(diffStream, diffpoint, ','))
        {
            float p = atof(point.c_str());
            float diffp = atof(diffpoint.c_str());
            
            image.at<float>(0,i) = p;
            diffImage.at<float>(0,i) = diffp;

            i++;

        }
        
        
        // add the row to the complete data vector
        images.push_back(image);
        diffImages.push_back(diffImage);
        labels.push_back(label);
    
    }
    // close all files
    centroidsfile.close();
    cout << "Training images are read."<<endl;
    return images.size();

}

