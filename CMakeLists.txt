cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(sge)
set(CXX g++)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 17)
set(OpenGL_GL_PREFERENCE GLVND)

add_compile_options("-fPIC")

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

# Add .lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Define the executable
add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES} src/Util/Timer.hpp src/Graphics/Rectangle.hpp src/Graphics/RectangleDefaults.hpp src/Graphics/Text.hpp src/Graphics/TextDefaults.hpp src/Graphics/Button.hpp src/Graphics/ButtonDefaults.hpp src/Graphics/ButtonClickManager.cpp src/Graphics/ButtonClickManager.hpp src/Engine.hpp)

find_package(OpenGL REQUIRED)

# GLM
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# GLFW
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# freetype in lib folder, may be changed to somethin nicer like up there
find_library(FREETYPE freetype lib)

# GLAD
add_library(GLAD "thirdparty/glad.c")

# Put all libraries into a variable
set(LIBS glfw GL GLAD ${FREETYPE} dl)

# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
	"/usr/include"
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})
