cmake_minimum_required(VERSION 3.16)

include(GoogleTest)
find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}_test)
target_compile_features(${PROJECT_NAME}_test PUBLIC cxx_std_20)

set(GTEST_FILES
    TestDSPUtils.cpp
    TestFFT.cpp
)

target_sources(${PROJECT_NAME}_test
    PRIVATE
    ${GTEST_FILES}
)

target_link_libraries(${PROJECT_NAME}_test
    PRIVATE
    ${PROJECT_NAME}_lib
    GTest::gtest
    GTest::gmock
    GTest::gtest_main
)

add_test(NAME ${PROJECT_NAME}Test COMMAND ${PROJECT_NAME}_test)