project( YEngine )
cmake_minimum_required( VERSION 2.8.11 )

set( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{QT5_DIR} )
# remember to add `D:/Qt/Qt-5.7.0/5.7/msvc2015_64\bin` to %PATH%
set( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "D:/Qt/Qt-5.7.0/5.7/msvc2015_64" ) 
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )

find_package( Qt5Widgets REQUIRED )
qt5_add_resources( QRCS resources.qrc )
add_executable( ${PROJECT_NAME} main.cpp mainwindow.cpp vertex.h ${QRCS} )
target_link_libraries( ${PROJECT_NAME} Qt5::Widgets )
