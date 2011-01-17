# grab all source files to build here
file(GLOB_RECURSE SRCS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  )

# some visual studio dance steps to make headers show up in the file view
if(WIN32 OR APPLE)
  file(GLOB_RECURSE INCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${CBP_MAIN_DIR}/include/optimizer/*.h")
  set_source_files_properties(${INCS} PROPERTIES HEADER_FILE_ONLY true)
  # source_group(Headers FILES ${INCS})

  set(SRCS ${SRCS} ${INCS})
endif()

# the gamelogic FIX parser executable
add_library(optimizer ${SRCS})
