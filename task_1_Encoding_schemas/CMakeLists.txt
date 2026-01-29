cmake_minimum_required(VERSION 3.16)
project(base64_lab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS 
    common_types.h 
    base64_strategy.h 
    encoder_service.h 
    decoder_service.h
    rle_strategy.h
    rle_service.h
)

# Base64 Targets
add_executable(base64-encode main_encode.cpp ${HEADERS})
target_include_directories(base64-encode PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(base64-decode main_decode.cpp ${HEADERS})
target_include_directories(base64-decode PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# RLE Targets
add_executable(rle-encode main_rle_encode.cpp ${HEADERS})
target_include_directories(rle-encode PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(rle-decode main_rle_decode.cpp ${HEADERS})
target_include_directories(rle-decode PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
