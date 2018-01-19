#include <opencv2/face.hpp>
#include "_framerec.hpp"
#include <set>
#include <limits>
#include <iostream>

using namespace cv;


inline Mat asRowMatrix(InputArrayOfArrays src, int rtype, double alpha=1, double beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    if(src.kind() != _InputArray::STD_VECTOR_MAT && src.kind() != _InputArray::STD_VECTOR_VECTOR) {
        String error_message = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< std::vector<...> >).";
        CV_Error(Error::StsBadArg, error_message);
    }
    // number of samples
    size_t n = src.total();
    // return empty matrix if no matrices given
    if(n == 0)
        return Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).total();
    // create data matrix
    Mat data((int)n, (int)d, rtype);
    // now copy data
    for(unsigned int i = 0; i < n; i++) {
        // make sure data can be reshaped, throw exception if not!
        if(src.getMat(i).total() != d) {
            String error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, d, src.getMat(i).total());
            CV_Error(Error::StsBadArg, error_message);
        }
        // get a hold of the current row
        Mat xi = data.row(i);
        // make reshape happy by cloning for non-continuous matrices
        if(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        } else {
            src.getMat(i).clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    return data;
}


// Turk, M., and Pentland, A. "Eigenframes for recognition.". Journal of
// Cognitive Neuroscience 3 (1991), 71–86.
class Eigenframes : public EigenFrameRecognizer
{

public:
    // Initializes an empty Eigenframes model.
    Eigenframes(int num_components = 0, double threshold = DBL_MAX)
        //: BasicFaceRecognizerImpl(num_components, threshold)
    {
        _num_components = num_components;
        _threshold = threshold;
    }

    // Computes an Eigenframes model with images in src and corresponding labels
    // in labels.
    void train(InputArrayOfArrays src, InputArray labels);

    // Send all predict results to caller side for custom result handling
    void predict(InputArray src, Ptr<PredictCollector> collector) const;
    String getDefaultName() const
    {
        return "opencv_Eigenframes";
    }
};

//------------------------------------------------------------------------------
// Eigenframes
//------------------------------------------------------------------------------
void Eigenframes::train(InputArrayOfArrays _src, InputArray _local_labels) {
    if(_src.total() == 0) {
        String error_message = format("Empty training data was given. You'll need more than one sample to learn a model.");
        CV_Error(Error::StsBadArg, error_message);
    } else if(_local_labels.getMat().type() != CV_32SC1) {
        String error_message = format("Labels must be given as integer (CV_32SC1). Expected %d, but was %d.", CV_32SC1, _local_labels.type());
        CV_Error(Error::StsBadArg, error_message);
    }
    // make sure data has correct size
    if(_src.total() > 1) {
        for(int i = 1; i < static_cast<int>(_src.total()); i++) {
            if(_src.getMat(i-1).total() != _src.getMat(i).total()) {
                String error_message = format("In the Eigenframes method all input samples (training images) must be of equal size! Expected %d pixels, but was %d pixels.", _src.getMat(i-1).total(), _src.getMat(i).total());
                CV_Error(Error::StsUnsupportedFormat, error_message);
            }
        }
    }
    // get labels
    Mat labels = _local_labels.getMat();
    // observations in row
    Mat data = asRowMatrix(_src, CV_64FC1);

    // number of samples
   int n = data.rows;
    // assert there are as much samples as labels
    if(static_cast<int>(labels.total()) != n) {
        String error_message = format("The number of samples (src) must equal the number of labels (labels)! len(src)=%d, len(labels)=%d.", n, labels.total());
        CV_Error(Error::StsBadArg, error_message);
    }
    // clear existing model data
    _labels.release();
    _projections.clear();
    // clip number of components to be valid
    if((_num_components <= 0) || (_num_components > n))
        _num_components = n;

    // perform the PCA
    PCA pca(data, Mat(), PCA::DATA_AS_ROW, _num_components);
    // copy the PCA results
    _mean = pca.mean.reshape(1,1); // store the mean vector
    _eigenvalues = pca.eigenvalues.clone(); // eigenvalues by row
    transpose(pca.eigenvectors, _eigenvectors); // eigenvectors by column
    // store labels for prediction
    _labels = labels.clone();
    // save projections
    for(int sampleIdx = 0; sampleIdx < data.rows; sampleIdx++) {
        Mat p = LDA::subspaceProject(_eigenvectors, _mean, data.row(sampleIdx));
        _projections.push_back(p);
    }
}

void Eigenframes::predict(InputArray _src, Ptr<PredictCollector> collector) const {
    // get data
    Mat src = _src.getMat();
    // make sure the user is passing correct data
    if(_projections.empty()) {
        // throw error if no data (or simply return -1?)
        String error_message = "This Eigenframes model is not computed yet. Did you call Eigenframes::train?";
        CV_Error(Error::StsError, error_message);
    } else if(_eigenvectors.rows != static_cast<int>(src.total())) {
        // check data alignment just for clearer exception messages
        String error_message = format("Wrong input image size. Reason: Training and Test images must be of equal size! Expected an image with %d elements, but got %d.", _eigenvectors.rows, src.total());
        CV_Error(Error::StsBadArg, error_message);
    }
    // project into PCA subspace
    Mat q = LDA::subspaceProject(_eigenvectors, _mean, src.reshape(1, 1));
    collector->init(_projections.size());
    //std::cout << _projections.size()<<"---"<<std::endl;
    for (size_t sampleIdx = 0; sampleIdx < _projections.size(); sampleIdx++) {
        double dist = norm(_projections[sampleIdx], q, NORM_L2);
        int label = _labels.at<int>((int)sampleIdx);
        collector->collect(label, dist);
    }
}

Ptr<EigenFrameRecognizer> EigenFrameRecognizer::create(int num_components, double threshold)
{
    return makePtr<Eigenframes>(num_components, threshold);
}

