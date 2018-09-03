cmake_minimum_required(VERSION 3.0)
project(Opengl)

set(CMAKE_CXX_STANDARD 11)
set(APP _VERSION_MAJOR 1)
set(APP _VERSION_MINOR 0)

file(GLOB PROJECT_SOURCES ./src/*.cpp)
set(APP_TARGET opengl.app)

add_executable(${APP_TARGET} ${PROJECT_SOURCES})

# glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

target_link_libraries(
	${APP_TARGET}
	glfw
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
)
