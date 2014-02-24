cmake_minimum_required (VERSION 2.6)

find_package(OpenCV REQUIRED)

add_executable(mainFilter selector.cpp HSVFilter.cpp Utils.cpp)

target_link_libraries(mainFilter ${OpenCV_LIBS})
