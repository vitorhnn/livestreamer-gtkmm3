cmake_minimum_required(VERSION 3.2)
project(livestreamer-gtkmm3)

add_definitions(-std=c++14)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTKMM3 gtkmm-3.0 REQUIRED)
include_directories(${GTKMM3_INCLUDE_DIRS})


add_executable(livestreamer-gtkmm3 fileHelper.cpp main.cpp mainWindow.cpp addStreamDialog.cpp livestreamerProcess.cpp )
target_link_libraries(livestreamer-gtkmm3  ${GTKMM3_LIBRARIES} )

install(TARGETS livestreamer-gtkmm3 DESTINATION bin)
install(FILES ${PROJECT_SOURCE_DIR}/UI/mainWindow.glade DESTINATION share/${PROJECT_NAME}/UI)
