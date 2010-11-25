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

else(MSVC)
  set(CMAKE_CXX_FLAGS "-Wall -g")
endif(MSVC)

include(${CMAKE_MODULE_PATH}/FindBison.cmake)

# add the current binary dir - where the generated files are
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/include/codegen
    ${CMAKE_CURRENT_SOURCE_DIR}/include/symboltable
    ${CMAKE_CURRENT_SOURCE_DIR}/include/msghandler
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

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/examples/working.cbp 
  ${CMAKE_CURRENT_BINARY_DIR}/in 
  COPYONLY)

message(STATUS "Found all required modules")
