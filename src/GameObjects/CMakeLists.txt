cmake_minimum_required(VERSION 2.8) 

project(GameObjects) 

set(PATH SpaceCrafts) 

aux_source_directory(${PATH} SOURCES) 

add_library(libGameObjects ${SOURCES})