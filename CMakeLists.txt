cmake_minimum_required(VERSION 2.8)

project( YEngine )
set(LIB_NAME YEngine)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")
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
)

GroupSources(src/yengine "//")

# Exclude ZERO_CHECK project
SET(CMAKE_SUPPRESS_REGENERATION TRUE)

# Set C++11
set (CMAKE_CXX_STANDARD 11)

# FIND REQUIRED PACKAGES
find_package(GLEW REQUIRED)
find_package(SOIL REQUIRED)
find_package(ASSIMP REQUIRED)

IF (NOT Vulkan_FOUND)
	MESSAGE("Searching for embedded vulkan library version ...")
	set (VULKAN_DIR ${CMAKE_MODULE_PATH}/../../dependencies/vulkan/)
	find_library(Vulkan_LIBRARY NAMES vulkan-1 vulkan PATHS ${VULKAN_DIR}/lib)
	IF (Vulkan_LIBRARY)
		set(Vulkan_FOUND ON)
		MESSAGE("Embedded Vulkan library version found : ${Vulkan_LIBRARY}")
	ENDIF()
	link_directories(${VULKAN_DIR}/lib) 
	include_directories(${VULKAN_DIR}/include)
ENDIF()

include_directories(${CMAKE_MODULE_PATH}/../../deps/glm/)
include_directories(${GLEW_INCLUDE_PATH})
include_directories(${SOIL_INCLUDE_DIR})
include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${CMAKE_MODULE_PATH}/../../deps/vulkan/include)
#include_directories(YEngine)
include_directories(src) # sources of yengine

#http://stackoverflow.com/questions/33400777/error-lnk2019-unresolved-external-symbol-main-referenced-in-function-int-cde
# force Unicode over Multi-byte
add_definitions(-DUNICODE -D_UNICODE)

#wxwidget seems to use some depracated functions : disable warnings on visual studio about it
add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)

add_library(${LIB_NAME} STATIC ${SRC_FILES})
set(LIBS opengl32 ${Vulkan_LIBRARY})  
target_link_libraries(${LIB_NAME} ${LIBS} ${GLEW_LIBRARY} ${SOIL_LIBRARY} ${ASSIMP_LIBRARY})
set_target_properties(${LIB_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
set_target_properties(${LIB_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT YEngine)

# Copy gelw32.dll 64 bits with .exe
set( CMAKE_CURRENT_BINARY_DIR "${PROJECT_SOURCE_DIR}/build/Debug/" )
add_custom_command(TARGET ${LIB_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_MODULE_PATH}/../../deps/glew/bin/Release/x64/glew32.dll ${CMAKE_CURRENT_BINARY_DIR})

# Copy assimp-vc140.dll 64 bits with .exe
set( CMAKE_CURRENT_BINARY_DIR "${PROJECT_SOURCE_DIR}/build/Debug/" )
add_custom_command(TARGET ${LIB_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_MODULE_PATH}/../../deps/assimp/lib/Release/assimp-vc140-mt.dll ${CMAKE_CURRENT_BINARY_DIR})