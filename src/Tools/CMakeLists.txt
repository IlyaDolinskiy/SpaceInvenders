cmake_minimum_required(VERSION 2.8) 

project(System) 

set(PATH Logger) 

aux_source_directory(${PATH} SOURCES) 
#message(STATUS "${SOURCES}")

add_library(libSystem ${SOURCES})