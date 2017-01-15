cmake_minimum_required(VERSION 3.1)

project(Application)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/modules/")
message("CMAKE_MODULE_PATH : " ${CMAKE_MODULE_PATH})

# Set the name of the ouput binary
set(EXE_NAME Application)

# Add all files from a src folder
file(
	GLOB_RECURSE
	SRC_FILES
	src/*
)

SET(CMAKE_SUPPRESS_REGENERATION TRUE)

# Find the required packages
find_package(GLEW REQUIRED)
find_package(wxWidgets REQUIRED)

include_directories(${CMAKE_MODULE_PATH}/../../dependencies/glm/)

# Add vulkan dependencies using environment variable from LunarG SDK Setup
link_directories("$ENV{VULKAN_SDK}/Bin") 
#link_directories("$ENV{VULKAN_SDK}/etc/explicit_layer.d")
include_directories("$ENV{VULKAN_SDK}/Include")

# Add wxWidgets dependencies
link_directories(${wxWidgets_LIB_DIR})
include_directories(${wxWidgets_ROOT_DIR}/include)
include_directories(${wxWidgets_ROOT_DIR}/include/msvc)

# Flags for debug build
set(
	CMAKE_CXX_FLAGS_DEBUG 
	"${CMAKE_CXX_FLAGS_DEBUG} -d -Wall"
)
# Flags for Release build
set(
	CMAKE_CXX_FLAGS_RELEASE 
	"${CMAKE_CXX_FLAGS_RELEASE} -Wall"
)

add_definitions(-DUNICODE -D_UNICODE)

# Set C++11
set (CMAKE_CXX_STANDARD 11)

# Setup binary output
add_executable(${EXE_NAME} ${SRC_FILES})

set(LIBS opengl32 vulkan-1) 
target_link_libraries(${EXE_NAME} ${LIBS} ${GLEW_LIBRARY})
set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Application)