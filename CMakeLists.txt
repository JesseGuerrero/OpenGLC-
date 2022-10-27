cmake_minimum_required(VERSION 3.22)
project(CPPOpenGL)

set(CMAKE_CXX_STANDARD 23)
include("D:/Other/New folder/vcpkg/scripts/buildsystems/vcpkg.cmake")
include_directories("include")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(GLEW REQUIRED)

macro (add_executable_and_libs _name)
    # invoke built-in add_executable
    _add_executable(${ARGV})
    if (TARGET ${_name})
        target_link_libraries(${_name} PRIVATE GLUT::GLUT)
        target_link_libraries(${_name} PRIVATE ${OPENGL_gl_LIBRARY})
        target_link_libraries(${_name} PRIVATE assimp::assimp)
        target_link_libraries(${_name} PRIVATE GLEW::GLEW)
    endif()
endmacro()

add_executable_and_libs(01Window 01Window.cpp)
add_executable_and_libs(02Dot 02Dot.cpp)
add_executable_and_libs(03Triangle 03Triangle.cpp)
