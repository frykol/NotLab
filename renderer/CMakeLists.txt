cmake_minimum_required(VERSION 3.20) 
add_library(graphics text.cpp Shader.cpp renderer.cpp figure.cpp figure_base.cpp)
target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenGL REQUIRED)
find_package(GLEW)


add_subdirectory(glfw)
add_subdirectory(glew)
add_subdirectory(freetype)
add_subdirectory(glm)

target_link_libraries(graphics PUBLIC glfw OpenGL::GL GLEW::GLEW freetype glm)

