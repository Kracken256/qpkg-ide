cmake_minimum_required(VERSION 3.15)
project(qpkg-ide)

if(NOT DEFINED CMAKE_BUILD_TYPE)
  message(FATAL_ERROR "CMAKE_BUILD_TYPE is not defined!")
endif()

if (NOT DEFINED CMAKE_SYSTEM_NAME)
  message(FATAL_ERROR "CMAKE_SYSTEM_NAME is not defined!")
endif()

add_subdirectory(libs)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_subdirectory(linux)
else()
  message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif()
