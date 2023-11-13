cmake_minimum_required(VERSION 3.17)

project(where-so LANGUAGES CXX)

add_executable(where-so ./where-so.cc)
add_library(findme SHARED ./findme.cc)

install(TARGETS where-so)
