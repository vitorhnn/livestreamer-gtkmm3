cmake_minimum_required(VERSION 3.2)
project(livestreamer-gui-thingy)

if ( CMAKE_COMPILER_IS_GNUC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
  add_definitions ( -D_DEBUG -Wall )
endif ()

add_definitions ( -std=c++14 )

FIND_PACKAGE( PkgConfig REQUIRED )
PKG_CHECK_MODULES(GTKMM3 gtkmm-3.0 REQUIRED)
include_directories( ${GTKMM3_INCLUDE_DIRS} )


add_executable( livegui ../main.cpp ../mainWindow.cpp ../addStreamDialog.cpp ../livestreamerProcess.cpp)
target_link_libraries( livegui  ${GTKMM3_LIBRARIES} )

