cmake_minimum_required(VERSION 2.8) 

project(GameObjects) 

set(PATH SpaceCrafts) 

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

aux_source_directory(${PATH} SOURCES) 

qt5_wrap_cpp(QT_WRAPPED_SRC ${SOURCES})

add_library(libGameObjects ${SOURCES})