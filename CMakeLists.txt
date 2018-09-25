cmake_minimum_required(VERSION 3.12.1 FATAL_ERROR)

include(GNUInstallDirs)

project(gol VERSION 0.0.0.0 LANGUAGES C)

configure_file (
  "${PROJECT_SOURCE_DIR}/gol_config.h.in"
  "${PROJECT_BINARY_DIR}/gol_config.h"
  )

include_directories("${PROJECT_BINARY_DIR}")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(glut MODULE REQUIRED)
find_package(glew CONFIG REQUIRED)

add_library(gol gol.c)
add_executable(gol-cli gol_cli.c)
add_executable(gol-gui gol_gui.c)

target_link_libraries(gol-cli gol)
target_link_libraries(gol-gui gol GLUT::GLUT GLEW::GLEW)

install(TARGETS gol-cli gol-gui
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  )
