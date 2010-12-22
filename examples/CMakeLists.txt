# yes, we do need assembler
enable_language(ASM-ATT)

# glob all cbp files in this dir
file(GLOB EXAMPLE_CBPS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cbp"
  )

message(STATUS "Copying gdb files:" )
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/function.gdb
  ${CMAKE_CURRENT_BINARY_DIR}/function.gdb
  COPYONLY)

message(STATUS "Generating examples:" )

# for each example file
foreach(EXAMPLE_CBP ${EXAMPLE_CBPS})

    message(STATUS " - ${EXAMPLE_CBP}" )

    # generate the assembler code using our cbp compiler
    add_custom_command(OUTPUT "${EXAMPLE_CBP}.s"
        COMMAND cbp.out "${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_CBP}"
        MAIN_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_CBP}"
        DEPENDS cbp.out
        COMMENT "Compiling example ${EXAMPLE_CBP} to assembler.."
      )

    # create executable using the kinda-dummy main.c
    add_executable("${EXAMPLE_CBP}.out" EXCLUDE_FROM_ALL ${CBP_MAIN_C} "${CMAKE_CURRENT_BINARY_DIR}/${EXAMPLE_CBP}.s")

endforeach()
