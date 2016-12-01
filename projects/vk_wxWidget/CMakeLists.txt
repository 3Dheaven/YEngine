cmake_minimum_required(VERSION 3.1)

project(YEngine)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/modules/")
message("CMAKE_MODULE_PATH : " ${CMAKE_MODULE_PATH})

# Set the name of the ouput binary
set(EXE_NAME YEngine)

# Add all files from a src folder
file(
	GLOB_RECURSE
	SRC_FILES
	src/*
)

# Find the required packages
find_package(GLEW REQUIRED)
find_package(wxWidgets REQUIRED)

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

# Set C++11
set (CMAKE_CXX_STANDARD 11)

# Setup binary output
add_executable(${EXE_NAME} ${SRC_FILES})

# Link dependencies
target_link_libraries(${EXE_NAME} ${GLEW_LIBRARY})
target_link_libraries(${EXE_NAME} vulkan-1)