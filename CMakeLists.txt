cmake_minimum_required(VERSION 2.6)
project(CBP)
set(CBP_VERSION v0.1)

# where there be modules
set(CBP_MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CBP_MAIN_DIR}/modules)

INCLUDE(MacroEnsureOutOfSourceBuild)
macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build. Please create a separate build directory and run 'cmake /path/to/${PROJECT_NAME} [options]' there.")

message(STATUS "")
message(STATUS "Generating CBP build files..")
message(STATUS "")

find_package(SDL)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc -D_SCL_SECURE_NO_WARNINGS")
  add_definitions(-DWIN32)
  add_definitions(-DMSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)

  find_path(UNISTD_DIR unistd.h
    PATHS "C:/gnuwin32/include"
  )
  message(STATUS "MSVC: Adding ${UNISTD_DIR} to include path")
  include_directories(
    ${UNISTD_DIR}
  )
elseif(APPLE)
    add_definitions(-DAPPLE)
    set(CMAKE_CXX_FLAGS "-Wall -g")
elseif(CYGWIN)
    add_definitions(-DWIN32)
	add_definitions(-DCYGWIN)
    set(CMAKE_CXX_FLAGS "-Wall -g")		
elseif(UNIX)
    add_definitions(-DUNIX)
    set(CMAKE_CXX_FLAGS "-Wall -g")
else()
    set(CMAKE_CXX_FLAGS "-Wall -g")
endif()

include(${CMAKE_MODULE_PATH}/FindBison.cmake)

# add the current binary dir - where the generated files are
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nodes
    ${CMAKE_CURRENT_SOURCE_DIR}/include/codegen
    ${CMAKE_CURRENT_SOURCE_DIR}/include/symboltable
    ${CMAKE_CURRENT_SOURCE_DIR}/include/msghandler
    ${CMAKE_CURRENT_SOURCE_DIR}/include/exceptions
  )

if(PARSERS_FOUND)
  message(STATUS "Found Bison and Flex executables!")

  message(STATUS "Adding /parser")
  add_subdirectory(parser)
else(PARSERS_FOUND)
  # Opps, didn't find the parsers
  message(SEND_ERROR "The Bison parsing tools can't be found.")
endif(PARSERS_FOUND)

add_subdirectory(codegen)

if(WIN32)
  add_executable(cbp main.cpp)
  target_link_libraries(cbp parser codegen)
else(WIN32)
  add_executable(cbp.out main.cpp)
  target_link_libraries(cbp.out parser codegen)
endif(WIN32)

# Doesn't work with Win32+MSVC, don't know why
if(NOT MSVC)
  add_subdirectory(cbp_main)
  add_subdirectory(examples)
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/examples/working.cbp
  ${CMAKE_CURRENT_BINARY_DIR}/in
  COPYONLY)

message(STATUS "Found all required modules")
