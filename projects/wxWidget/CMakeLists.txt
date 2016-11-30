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
if(COMPILER_MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

add_executable(${EXE_NAME} ${SRC_FILES})
target_link_libraries(${EXE_NAME} ${LIBS} ${GLEW_LIBRARY})