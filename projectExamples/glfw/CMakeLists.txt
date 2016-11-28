cmake_minimum_required( VERSION 2.8 )

project( YEngine )
set(EXE_NAME YEngine)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# Sources.
file(
	GLOB_RECURSE
	SRC_FILES
	src/*
)

# find the required packages
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in : ${GLFW3_INCLUDE_DIR}")
find_package(GLEW REQUIRED)
message(STATUS "Found GLEW in : ${GLEW_INCLUDE_DIR}")
#find_package(ASSIMP REQUIRED)
#message(STATUS "Found ASSIMP in : ${ASSIMP_INCLUDE_DIR}")
#find_package(SOIL REQUIRED)
#message(STATUS "Found SOIL in : ${SOIL_INCLUDE_DIR}")
#find_package(GLM REQUIRED)
#message(STATUS "Found GLM in : ${GLM_INCLUDE_DIR}")

set(LIBS glfw3 opengl32 glew32s ) 

link_directories(${CMAKE_SOURCE_DIR}/dependencies/glew/lib/Release/x64)
link_directories(${CMAKE_SOURCE_DIR}/dependencies/glfw/lib)

include_directories(${CMAKE_SOURCE_DIR}/dependencies/glfw/include)	
include_directories(${CMAKE_SOURCE_DIR}/dependencies/glew/include)

add_executable( ${EXE_NAME} ${SRC_FILES} )
target_link_libraries(${EXE_NAME} ${LIBS})