# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(
  # local includes
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/
  ${CMAKE_CURRENT_SOURCE_DIR}/include/symboltable/
  )

GENERATE_BISON_FLEX_SOURCES(
  ${CMAKE_CURRENT_SOURCE_DIR}/bison/cbp.y    "-v"
  ${CMAKE_CURRENT_SOURCE_DIR}/bison/cbp.lex  ""
  )

# add the current binary dir - where the generated files are
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  )

# grab all source files to build here
file(GLOB_RECURSE SRCS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  )

# some visual studio dance steps to make headers show up in the file view
if(WIN32)
  file(GLOB_RECURSE INCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
  set_source_files_properties(${INCS} PROPERTIES HEADER_FILE_ONLY true)
  # source_group(Headers FILES ${INCS})

  set(SRCS ${SRCS} ${INCS})
endif()

# the gamelogic FIX parser executable
add_library(parser ${SRCS} ${BF_SOURCES})
