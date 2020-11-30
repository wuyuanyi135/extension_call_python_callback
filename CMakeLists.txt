# cmake_minimum_required(VERSION <specify CMake version here>)
project(extension_call_python_callback)
cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 11)
add_subdirectory(pybind11)
add_subdirectory(fmt)
pybind11_add_module(${PROJECT_NAME} src/main.cpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
