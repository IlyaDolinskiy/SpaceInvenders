cmake_minimum_required(VERSION 2.8) 

project(System) 

set(PATH Logger) 

set(CMAKE_INCLUDE_CURRENT_DIR ON)

aux_source_directory(${PATH} SOURCES) 
#message(STATUS "${SOURCES}")

add_library(libSystem ${SOURCES})