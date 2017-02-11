cmake_minimum_required( VERSION 2.8 )

project( Application )
set(EXE_NAME Application)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/modules/")
message("CMAKE_MODULE_PATH : " ${CMAKE_MODULE_PATH})

message("PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}" )

macro(GroupSources curdir relativePath)

   file(GLOB children RELATIVE 
   ${PROJECT_SOURCE_DIR}/${relativePath}/${curdir}
   ${PROJECT_SOURCE_DIR}/${relativePath}${curdir}/*) 
   
   foreach(child ${children})

		if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${relativePath}/${curdir}/${child})
			   GroupSources(${curdir}/${child} ${relativePath})
		else()
			string(REPLACE "/" "\\" groupname ${curdir})
			source_group(${groupname} FILES
			${PROJECT_SOURCE_DIR}/${relativePath}/${curdir}/${child})
		endif()
    endforeach()
	
endmacro()

# Sources.
file(
	GLOB_RECURSE
	SRC_FILES
	src/*
	../../yengine/*
)

GroupSources(src "//")
GroupSources(yengine "/../../")

# Exclude ZERO_CHECK project
SET(CMAKE_SUPPRESS_REGENERATION TRUE)

# Set C++11
set (CMAKE_CXX_STANDARD 11)

# find the required packages
find_package(GLEW REQUIRED)
find_package(wxWidgets REQUIRED)
find_package(SOIL REQUIRED)

include_directories(${CMAKE_MODULE_PATH}/../../dependencies/glm/)
link_directories("$ENV{VULKAN_SDK}/Bin") 
include_directories("$ENV{VULKAN_SDK}/Include")
message("ENV{VULKAN_SDK} : " $ENV{VULKAN_SDK})
link_directories(${wxWidgets_LIB_DIR})
include_directories(${wxWidgets_ROOT_DIR}/include)
include_directories(${wxWidgets_ROOT_DIR}/include/msvc)
include_directories(${GLEW_INCLUDE_PATH})
link_directories(${wxWidgets_LIB_DIR})
include_directories(${wxWidgets_ROOT_DIR}/include)
include_directories(${wxWidgets_ROOT_DIR}/include/msvc)
include_directories(${SOIL_INCLUDE_DIR})

#http://stackoverflow.com/questions/33400777/error-lnk2019-unresolved-external-symbol-main-referenced-in-function-int-cde
# force Unicode over Multi-byte
add_definitions(-DUNICODE -D_UNICODE)

add_executable(${EXE_NAME} ${SRC_FILES})
set(LIBS opengl32 vulkan-1) 
target_link_libraries(${EXE_NAME} ${LIBS} ${GLEW_LIBRARY} ${SOIL_LIBRARY})
set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Application)