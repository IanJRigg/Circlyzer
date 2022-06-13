cmake_minimum_required(VERSION 3.12)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set(TEST_RUNNER_NAME testRunner)
project(${TEST_RUNNER_NAME} VERSION 1.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(
    ${TEST_RUNNER_NAME} 
    test-phasors.cpp
    test-runner.cpp
)

target_link_libraries(
    ${TEST_RUNNER_NAME} 
    PRIVATE
        GTest::GTest
)

target_include_directories(
    ${TEST_RUNNER_NAME}
    PRIVATE
        ${SRC_DIR}
        ${INCLUDE_DIR}
)

include(GoogleTest)
gtest_discover_tests(${TEST_RUNNER_NAME})
