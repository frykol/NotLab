cmake_minimum_required(VERSION 3.20) 
add_library(graphics test.cpp Shader.cpp)
target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(graphics PUBLIC glfw OpenGL::GL GLEW::GLEW)


