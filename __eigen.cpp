
#include "__eigen.hpp"

static void read_csv(const string& filename, vector<Mat>& avg, vector<int>& avglabels,
                                             vector<Mat>& diff, vector<int>& difflabels,
                                             char separator = ',') ;




int main(int argc, const char *argv[]) {
    CommandLineParser parser(argc, argv, keys);
    parser.about("Application name v1.0.0");
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    int test = 3;

    // empty loadFile means trian and save model into default name 
    // existing train means train and save model into the given name 
    String modelFileName = parser.get<String>("loadFile");
    bool defaultFileName = modelFileName.empty();// no specific file given to load file from
    bool train = true;
    if (defaultFileName){
        modelFileName = parser.get<String>("train");
        defaultFileName = modelFileName.empty();//no specific file given to save model into
        if (defaultFileName){
            modelFileName = "../data/eigenfacesModel.yml";
            if ( parser.has("load"))
                train = false;
        }
    }
    //String outputFolder = parser.get<string>("output");
    int testNo = parser.get<int>("test");

    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }


    string outputFolder = "../results/";

    vector<Mat> avgTRimages, diffTRimages, avgTESTimages, diffTESTimages;
    vector <int > avgTRlabels, diffTRlabels, avgTESTlabels, diffTESTlabels;
    
  
    // Read in the data. This can fail if no valid
    // input filename is given.
    try {
        read_csv( "../data/training.csv", avgTRimages, avgTRlabels, diffTRimages, diffTRlabels);
        cout << BOLDMAGENTA<<"done..\n"<<RESET;
        read_csv( "../data/testing.csv", avgTESTimages, avgTESTlabels, diffTESTimages, diffTESTlabels);
        cout << BOLDGREEN<<"done..\n"<<RESET;
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    cout <<"here33.."<<endl;
    // Quit if there are not enough images for this demo.
    if(avgTRimages.size() <= 1) 
    {
        cout <<"here11.."<<endl;
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(Error::StsError, error_message);
    }
    // Get the height from the first image. We'll need this
    // later in code to reshape the images to their original
    // size:
    cout <<"here.."<<endl;
    int height = avgTRimages[0].rows;
    
    cout << "trainging..";
    Ptr<EigenFaceRecognizer> avgModel = EigenFaceRecognizer::create();
    Ptr<EigenFaceRecognizer> diffModel = EigenFaceRecognizer::create();
    cout << "trainging..";
    avgModel->train(avgTRimages, avgTRlabels);
     cout << "trainging..";
    diffModel->train(diffTRimages, diffTRlabels);
     cout << "trainging..";
    // The following line predicts the label of a given
    // test image:
    int predictedAvg = avgModel->predict(avgTESTimages[test]);
    int predictedDiff = diffModel->predict(diffTESTimages[test]);
    //
    // To get the confidence of a prediction call the model with:
    //
    //      int predictedLabel = -1;
    //      double confidence = 0.0;
    //      model->predict(testSample, predictedLabel, confidence);
    //
    string result_message = format("Predicted average class = %d / Actual class = %d.", predictedAvg, avgTESTlabels[test]);
    string result_message2 = format("Predicted average class = %d / Actual class = %d.", predictedDiff, diffTESTlabels[test]);
    cout << result_message<<endl<<result_message2 << endl;
    // Here is how to get the eigenvalues of this Eigenfaces model:
    //Mat eigenvalues = model->getEigenValues();
    // And we can do the same to display the Eigenvectors (read Eigenfaces):
    Mat avgW = avgModel->getEigenVectors();
    Mat diffW = diffModel->getEigenVectors();
    // Get the sample mean from the training data
   //Mat mean = model->getMean();
    // Display or save:

        imshow("meanAvg", norm_0_255(avgModel->getMean().reshape(1, avgTRimages[0].rows)));
        imshow("mean Difference", norm_0_255(diffModel->getMean().reshape(1, diffTRimages[0].rows)));
  
    // Display or save the Eigenfaces:
    for (int i = 0; i < min(10, avgW.cols); i++) {
        // string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
        // cout << msg << endl;
        // get eigenvector #i
        Mat ev = avgW.col(i).clone();
        // Reshape to original size & normalize to [0...255] for imshow.
        Mat grayscale = norm_0_255(ev.reshape(1, height));
        // Show the image & apply a Jet colormap for better sensing.
        Mat cgrayscale;
        applyColorMap(grayscale, cgrayscale, COLORMAP_JET);
        // Display or save:
        if(argc == 2) 
            imshow(format("eigenface_%d", i), cgrayscale);
    }

    for (int i = 0; i < min(10, diffW.cols); i++) {
        // string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
        // cout << msg << endl;
        // get eigenvector #i
        Mat ev = diffW.col(i).clone();
        // Reshape to original size & normalize to [0...255] for imshow.
        Mat grayscale = norm_0_255(ev.reshape(1, height));
        // Show the image & apply a Jet colormap for better sensing.
        Mat cgrayscale;
        applyColorMap(grayscale, cgrayscale, COLORMAP_JET);
        // Display or save:
        if(argc == 2) 
            imshow(format("eigenface_%d", i), cgrayscale);
    }

    
        waitKey(0);

    return 0;
}



/*  read .csv that contains the file names of the dataset   */
static void read_csv(const string& filename, vector<Mat>& avg, vector<int>& avglabels,
                                             vector<Mat>& diff, vector<int>& difflabels,
                                             char separator) 
{
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }
    string line;
    while (getline(file, line))
    {

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

            avg.push_back(imread(line, 0));
            avglabels.push_back(atoi(label.c_str()));
        }
        else
        {

            diff.push_back(imread(line, 0));
            difflabels.push_back(atoi(label.c_str()));
        }
        cout << line << "\t"<< label<< endl;
    }

    file.close();
 
}

/*  normalize the images in the dataset */
Mat norm_0_255(InputArray _src)
{
    Mat src = _src.getMat();

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
