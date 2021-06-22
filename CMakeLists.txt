cmake_minimum_required(VERSION 3.19)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 14)

include_directories(Common)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(LearnOpenGL
        Common/def.h
        Common/Shader.cpp
        Common/Shader.h
        Common/stb_image.h
        Common/common.cpp
        Common/common.h
        CoordinateSystems/CoordinateSystems.cpp
        )

target_link_libraries(LearnOpenGL PRIVATE OpenGL::GL)
target_link_libraries(LearnOpenGL PRIVATE glad::glad)
target_link_libraries(LearnOpenGL PRIVATE glfw)