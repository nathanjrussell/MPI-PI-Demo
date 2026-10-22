cmake_minimum_required(VERSION 3.16)
project(MPI_PI_Demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(MPI REQUIRED COMPONENTS CXX)

add_library(pi src/pi.cpp)
target_include_directories(pi PUBLIC include)

add_executable(mpi_pi example/main.cpp)
target_link_libraries(mpi_pi PRIVATE pi MPI::MPI_CXX)

include(CTest)
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG f8d7d77c06936315286eb55f8de22cd23c188571 # v1.14.0
    )
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(pi_tests gtest/pi_test.cpp)
    target_link_libraries(pi_tests PRIVATE pi GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(pi_tests)
endif()
