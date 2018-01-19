
# Scene PCA
This is an experiment on using eigenfaces algorithm for scene recognition.

## Dependencies
* OpenCV 3.3, [here](https://github.com/opencv/opencv). 
* Opencv_contrib, [here](https://github.com/opencv).

<hr>

## Build and Run

<h4>2. PCA Part</h4>
To run the PCA part afterward on the generated scene files, you can execute the following:  

```
./eigen 
```
Or for a specific scene test:
```
./eigen -test=285
```

<h4>1. Segmentation Part</h4>
Assuming that images are not generated (if they are already, it would be better if go directly to the next s) in the images directory, and hierarchy is as here; you can run the project by executing the following:

``` 
cd build
cmake ..
make 
```
Above runs SlicO,  and below runds Slic  

```
./slic -a=0
```

The output will be saved into data directory, as csv files.

If `images.csv` needed to be edited, run the python command from build directory, to ensure that the hierarchy structre is the same.
**Example:**

```
python ../create_csv.py ../images
```
<hr>

## Human Action Dataset
Human action dataset is a pretty small dataset with about 2400 scenes, producing about 4700 average and differential images. Scenes contain on average abotut 100 frames. All scenes were taken over homogeneous backgrounds with a static camera with 25fps frame rate. 
The database contains six types of human actions (walking, jogging, running, boxing, hand waving and hand clapping) performed several times by 25 subjects in four different scenarios: 
1.	outdoors.
2.	outdoors with scale variation.
3.	outdoors with different clothes.
4.	indoors (see figure below). 
<hr>

## Project Hierarchy
Below is the hierarchy for the repository. Files start with underscores are modified library's files, while those with double underscores are the two main project's files:
```
├── CMakeFiles.txt
├── __eigen.cpp
├── __eigen.hpp
├── __eigenHelpers.cpp
├── _framerec.hpp
├── _private.hpp
├── createCsv.py
├── build
│   ├── pixel
│   ├── eigen
│   ├── ...
├── data
│   ├── images.csv
├── images
│   ├── scene1.jpg
│   ├── scene2.jpg
│   ├── ...
├── results
│   ├── eigenVector1.jpg
│   ├── eigenVector2.jpg
│   ├── ...
├── readme.md
```

