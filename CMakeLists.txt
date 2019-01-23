# Minimum cmake version (3.5 included in Ubuntu 16.04)
cmake_minimum_required(VERSION 3.5)

# Project name
project(tira-puristin)

# Include (headers)
include_directories(./src)

# Targets
add_executable(puristin ./src/main.cpp)
