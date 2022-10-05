cmake_minimum_required(VERSION 3.16)
project(mqueue)

include(ExternalProject)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip 
)

FetchContent_MakeAvailable(googletest)



set(HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/emqueue.hpp)
set(SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_u_mqueue.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/tests/test_i_mqueue.cpp)
     

add_executable(mqueue_tests ${SRC_FILES})
set_target_properties(mqueue_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(mqueue_tests PRIVATE gtest_main)
target_include_directories(mqueue_tests PRIVATE ${ROOT_DIR}/src)


