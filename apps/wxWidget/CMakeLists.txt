cmake_minimum_required( VERSION 2.8 )

project( Application )
set(EXE_NAME Application)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/modules/")
message("CMAKE_MODULE_PATH : " ${CMAKE_MODULE_PATH})

# Sources.
file(
	GLOB_RECURSE
	SRC_FILES
	src/*
)

file(
	GLOB_RECURSE
	SRC_FILES_ENGINE
	../../yengine/*
)

SET(CMAKE_SUPPRESS_REGENERATION TRUE)

# find the required packages
find_package(GLEW REQUIRED)
find_package(wxWidgets REQUIRED)

set(LIBS opengl32) 

#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../dependencies/glew/lib/Release/x64)
link_directories(${wxWidgets_LIB_DIR})

include_directories(${GLEW_INCLUDE_PATH})
include_directories(${wxWidgets_ROOT_DIR}/include)
include_directories(${wxWidgets_ROOT_DIR}/include/msvc)

#http://stackoverflow.com/questions/33400777/error-lnk2019-unresolved-external-symbol-main-referenced-in-function-int-cde

# force Unicode over Multi-byte
add_definitions(-DUNICODE -D_UNICODE)


add_executable(${EXE_NAME} ${SRC_FILES} ${SRC_FILES_ENGINE})
target_link_libraries(${EXE_NAME} ${LIBS} ${GLEW_LIBRARY} )
set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Application)
