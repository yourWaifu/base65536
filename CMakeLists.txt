# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("base65536")

add_library(base65536 INTERFACE)
target_include_directories(base65536 INTERFACE .)
target_compile_features(base65536 INTERFACE cxx_constexpr)

add_executable(base65536_test "test.cpp")
target_compile_features(base65536_test PUBLIC cxx_std_20)
target_link_libraries(base65536_test PRIVATE base65536)