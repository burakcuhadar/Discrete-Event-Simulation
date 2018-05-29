cmake_minimum_required(VERSION 3.8)
project(project2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Order.h Order.cpp Worker.h Worker.cpp Event.h Event.cpp CoffeeShop.h CoffeeShop.cpp)
add_executable(project2 ${SOURCE_FILES})