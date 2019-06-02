cmake_minimum_required(VERSION 3.13)
project(Crypt)

set(CMAKE_CXX_STANDARD 14)

set(INC_DIR /usr/local/include/)

include_directories(${INC_DIR})
link_libraries(NTL)

link_libraries(gmp)

add_executable(Crypt common.cpp test3.cpp)
target_link_libraries(Crypt NTL)
target_link_libraries(Crypt gmp)