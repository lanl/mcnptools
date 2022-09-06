cmake_minimum_required(VERSION 3.12)

project(test_find_package_x11 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../..")
include(Backports/X11)

find_package(X11)

if(NOT X11_FOUND)
  message(WARNING "X11 not found!")
endif()
