cmake_minimum_required(VERSION 2.8)
project( SVMRecognition )
find_package( OpenCV REQUIRED )
add_executable( SVMRecognition main.cpp SVM_Bc.cpp)
target_link_libraries( SVMRecognition ${OpenCV_LIBS} )
