cmake_minimum_required(VERSION 3.0)
project (gnuplot_cpp_example)
set (CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
add_executable(gnuplot_cpp_example example.cpp ../include/gnuplot.h)

add_executable(gnuplot_cpp_example2 example2.cpp)
