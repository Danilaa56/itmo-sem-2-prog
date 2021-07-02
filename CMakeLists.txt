cmake_minimum_required(VERSION 3.17)
project(itmo_1_prog_2)

set(CMAKE_CXX_STANDARD 20)

add_library(files56 src/lib56/files.cpp src/lib56/files.h)
add_library(xml56 src/lib56/xml/XMLObject.cpp src/lib56/xml/XMLObject.h src/lib56/xml/XMLParser.cpp src/lib56/xml/XMLParser.h)

add_executable(lab1 src/lab1.cpp)
add_executable(lab2 src/lab2.cpp Polynom.cpp Polynom.h)

add_executable(lab3 src/lab3.cpp)
target_link_libraries (lab3 files56 xml56)

add_executable(lab4 src/lab4algo.cpp src/lab4algo.h src/lab4.cpp)

add_executable(lab5 src/lab5.cpp)

add_executable(lab6gl src/lab6/lab6gl.cpp src/lab6/camera.cpp src/lab6/camera.h src/lab6/CubeManager.cpp src/lab6/CubeManager.h src/lab6/Color.cpp src/lab6/Color.h)
target_link_libraries(lab6gl OpenGL32.Lib)

