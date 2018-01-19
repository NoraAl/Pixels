// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (c) 2011,2012. Philipp Wagner <bytefish[at]gmx[dot]de>.
// Third party copyrights are property of their respective owners.

#ifndef __FRAMEREC_HPP__
#define __FRAMEREC_HPP__

#include "opencv2/face.hpp"
#include "opencv2/core.hpp"

using namespace cv;
using namespace cv::face;

class CV_EXPORTS_W EigenFrameRecognizer : public BasicFaceRecognizer
{
public:
    /**
    @param num_components The number of components (read: Eigenfaces) kept for this Principal
    Component Analysis. As a hint: There's no rule how many components (read: Eigenfaces) should be
    kept for good reconstruction capabilities. It is based on your input data, so experiment with the
    number. Keeping 80 components should almost always be sufficient.
    @param threshold The threshold applied in the prediction.

    ### Notes:

    -   Training and prediction must be done on grayscale images, use cvtColor to convert between the
        color spaces.
    -   **THE EIGENFACES METHOD MAKES THE ASSUMPTION, THAT THE TRAINING AND TEST IMAGES ARE OF EQUAL
        SIZE.** (caps-lock, because I got so many mails asking for this). You have to make sure your
        input data has the correct shape, else a meaningful exception is thrown. Use resize to resize
        the images.
    -   This model does not support updating.

    ### Model internal data:

    -   num_components see EigenFaceRecognizer::create.
    -   threshold see EigenFaceRecognizer::create.
    -   eigenvalues The eigenvalues for this Principal Component Analysis (ordered descending).
    -   eigenvectors The eigenvectors for this Principal Component Analysis (ordered by their
        eigenvalue).
    -   mean The sample mean calculated from the training data.
    -   projections The projections of the training data.
    -   labels The threshold applied in the prediction. If the distance to the nearest neighbor is
        larger than the threshold, this method returns -1.
     */
    CV_WRAP static Ptr<EigenFrameRecognizer> create(int num_components = 0, double threshold = std::numeric_limits<double>::max());
};

#endif //__OPENCV_FACEREC_HPP__
