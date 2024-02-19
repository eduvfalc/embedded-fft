cmake_minimum_required(VERSION 3.16)
project(embedded-fft LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)
include_directories(tests/utils/include)

add_library(${PROJECT_NAME}_lib STATIC)

target_compile_features(${PROJECT_NAME}_lib PUBLIC cxx_std_20)

add_subdirectory(tests/utils)

if(UNIT_TEST)
    add_library(${PROJECT_NAME})
else()
    add_executable(${PROJECT_NAME})
endif()

target_sources(${PROJECT_NAME} PRIVATE src/example.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib)

if(UNIT_TEST)
    enable_testing()
    add_subdirectory(tests)
endif()