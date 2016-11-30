cmake_minimum_required( VERSION 2.8 )

project( YEngine )
set(EXE_NAME YEngine)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/modules/")
message("CMAKE_MODULE_PATH : " ${CMAKE_MODULE_PATH})

# Sources.
file(
	GLOB_RECURSE
	SRC_FILES
	src/*
)

# find the required packages
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
#find_package(ASSIMP REQUIRED)
#find_package(SOIL REQUIRED)
#find_package(GLM REQUIRED)

set(LIBS glfw3 opengl32 glew32s ) 

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../dependencies/glew/lib/Release/x64)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../dependencies/glfw/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../dependencies/glfw/include)	
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../dependencies/glew/include)

add_executable( ${EXE_NAME} ${SRC_FILES} )
target_link_libraries(${EXE_NAME} ${LIBS})