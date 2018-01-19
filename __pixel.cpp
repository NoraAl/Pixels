
#include "_slicAlgorithm.hpp"

using namespace cv;
using namespace cv::slicNora;
// using namespace std;

//vector <Info> files;
vector <Meta> metadata;
static void saveAverage(Meta meta, int range);
static void readCsv(const string& filename, char separator = ',') ;

static const char *window_name = "Slic Superpixels";
static const char *keys =
    "{h help      | | help menu}"
    "{f file     | | file name}"
    "{a algorithm |1| Slic(0),Slico(1),MSlic(2)}";


int main(int argc, char **argv)
{
    CommandLineParser cmd(argc, argv, keys);
    if (cmd.has("help"))
    {
        cmd.about("This program demonstrates Slic superpixels using OpenCV class SuperpixelSlic.\n"
                  "If no image file is supplied, try to open a webcam.\n"
                  "Use [space] to toggle output mode, ['q' or 'Q' or 'esc'] to exit.\n");
        cmd.printMessage();
        return 0;
    }
    string imagesFile = cmd.get<String>("file");

    bool nofileSpecified = imagesFile.empty();
    if (nofileSpecified){
        imagesFile = "../meta.csv";
    }

    try
    {
        // array of files names to be processed
        readCsv(imagesFile);
    }

    catch (cv::Exception &e)
    {
        cerr << "Error opening file \"" << imagesFile << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }

    int total = 0;
    //range = 0;//0,1,2,3
    // string scene("_d1");
    for (int range = 0; range < 4; range++) {
        for (int i = 0; i < metadata.size(); i++) {

            // if no string equals to whatever in scene string, pass
            // if (metadata[i].filename.find(scene) == string::npos)
            //     continue;

            if (metadata[i].range[range].exist)
                saveAverage(metadata[i], range);
        }
    }

    cout << "There are " << total <<"frames"<<endl;

    return 0;
}


static void readCsv(const string& filename, char separator ) {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }
    // Info element;
    Meta element;
    int m = 0;
    string line, filefullname, classlabel, rangeStart[4], rangeEnd[4];
    while (getline(file, line)) {
      
        stringstream currentLine(line);
        getline(currentLine, classlabel, separator);
        getline(currentLine, filefullname, separator);
        getline(currentLine, rangeStart[0], separator);
        getline(currentLine, rangeEnd[0], separator);

        getline(currentLine, rangeStart[1], separator);
        getline(currentLine, rangeEnd[1], separator);

        getline(currentLine, rangeStart[2], separator);
        getline(currentLine, rangeEnd[2], separator);

        getline(currentLine, rangeStart[3], separator);
        getline(currentLine, rangeEnd[3]);

        if(!filefullname.empty() && !classlabel.empty()) {
            element.filename = "../video/"+filefullname+".avi";
            element.label = atoi(classlabel.c_str());
            
            element.range[0].exist = (atoi(rangeStart[0].c_str())==9999)?false:true;
            if (element.range[0].exist){
                element.range[0].start = atoi(rangeStart[0].c_str())-1;
                element.range[0].end = atoi(rangeEnd[0].c_str())-1;
            }

            element.range[1].exist = (atoi(rangeStart[1].c_str())==9999)?false:true;
            if (element.range[1].exist){
                element.range[1].start = atoi(rangeStart[1].c_str())-1;
                element.range[1].end = atoi(rangeEnd[1].c_str())-1;
            }

            element.range[2].exist = (atoi(rangeStart[2].c_str())==9999)?false:true;
            if (element.range[2].exist){

                element.range[2].start = atoi(rangeStart[2].c_str())-1;
                element.range[2].end = atoi(rangeEnd[2].c_str())-1;
            }

            element.range[3].exist = (atoi(rangeStart[3].c_str())==9999)?false:true;
            if (element.range[3].exist){
                element.range[3].start = atoi(rangeStart[3].c_str())-1;
                element.range[3].end = atoi(rangeEnd[3].c_str())-1;
            }

            metadata.push_back(element);
        }
    }

    cout << metadata.size()<<endl;
    for (int i = 0; i< 5; i++){
        cout << metadata[i].label<<"\t"<<metadata[i].range[0].start<<"->"<<metadata[i].range[0].end
        <<"\t\t"<<metadata[i].range[1].start<<"->"<<metadata[i].range[1].end
        <<"\t\t"<<metadata[i].range[2].start<<"->"<<metadata[i].range[2].end
        <<"\t\t"<<metadata[i].range[3].start<<"->"<<metadata[i].range[3].end
        <<"\t\t"<<metadata[i].filename<<endl;
    }
}


static void saveAverage(Meta meta, int range)
{
    VideoCapture cap(meta.filename);
    cout << meta.filename<<endl;

    // 'frame' holds the data of a single frame (pixels vs superpixels)
    Mat frame;
    // vector to read each frame in the scene; to average them later on
    vector< Mat > frames;

    // get where each scene in the range start at and set the VideoCapture to that frame
    int position = meta.range[range].start;
    cap.set(CV_CAP_PROP_POS_FRAMES, position);

    // go over the range of frames and push pack frame info to the frames (pixels vs superpixels)
    while (position <= meta.range[range].end)
    {
        if (!cap.read(frame)) // if not successful, break loop, read() decodes and captures the next frame.
            break;

        // type must be of a uchar, TO DO: FIX IT FOR ALL TYPES
        if (frame.type() == CV_8UC3){
            
            //convert to grayscale
            cvtColor(frame, frame, COLOR_RGB2GRAY);

        } else if (frame.type() != CV_8UC1){

            cout << "error!"<< endl;
            exit(1);
        }

        frames.push_back(frame);

        imshow("movie", frame);
        waitKey(1);
    }
       
    // if no frames in current scene, do nothing
    if (frames.size() == 0){
        cout << "Empty Scene.."<< endl;
        return;
    }

    /*  averaging   */

    Mat sum(frames[0].rows, frames[0].cols, CV_32F, Scalar(0));//overflow and floating point
    Mat diffs(frames[0].rows, frames[0].cols, CV_32F, Scalar(0));//overflow and floating point
    
    //  Mats for current and next scenes 
    Mat current(sum.size(), sum.type());
    Mat next(sum.size(), diffs.type());
    Mat result(sum.size(), diffs.type());

    for (int i = 0; i < frames.size(); i++)
    {
        
        frames[i].convertTo(current, sum.type());
        sum = sum + current;
        
        
        if (i < frames.size()-1){
            frames[i+1].convertTo(next, diffs.type());
            absdiff(next,current,result);
            diffs = diffs + result;
        }
    }
    

    // no need for averaging min max will do it
    double min, max;
    minMaxLoc(sum, &min, &max);
    Mat sumImage;
    if (min != max)
        sum.convertTo(sumImage, CV_8U,255.0/(max-min),-255.0*min/(max-min));

    minMaxLoc(diffs, &min, &max);
    Mat diffImage;
    if (min != max)
        diffs.convertTo(diffImage, CV_8U,255.0/(max-min),-255.0*min/(max-min));
    

    //  delete '../video/' and '.avi' from the file name
    std::size_t found = meta.filename.find("../video/");
    string str = meta.filename.erase( found, 9);
    found = str.find(".avi");
    str = str.erase( found, 4);

    string name = "../images/" + str + "_range[" + to_string(range)+ "]_label[" + to_string(meta.label) ;
  
    imwrite(name + "]avg.jpg", sumImage);
    imwrite(name + "]dif.jpg", diffImage);
}
