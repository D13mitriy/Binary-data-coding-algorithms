cmake_minimum_required(VERSION 3.16)

project(bit-wise LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bit-wise main.cpp
    base64.h base64.cpp
    base64_coder.cpp
    rle.h rle.cpp
    base64_decoder.cpp
    rle_coder.cpp
    rle_decoder.cpp)

include(GNUInstallDirs)
install(TARGETS bit-wise
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
