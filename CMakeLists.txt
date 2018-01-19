cmake_minimum_required(VERSION 2.8)


project (eigen)
project (pixels)

find_package( OpenCV 3.3.0 REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( eigen __eigen.cpp __eigenHelpers.cpp _eigenframes.cpp )
add_executable( pixels __pixel.cpp)

target_link_libraries( eigen ${OpenCV_LIBS})
target_link_libraries( pixels  ${OpenCV_LIBS})




