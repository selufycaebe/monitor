cmake_minimum_required(VERSION 3.14)

project(Monitor LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GoogleTest)
add_executable(Monitor
    tests/tst_casea.cpp
    tests/main.cpp
)
target_link_libraries(
    Monitor
    PRIVATE
    monitorlib
)
gtest_add_tests(TARGET  Monitor)

add_subdirectory(src)
