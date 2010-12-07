# static library for linking
set(CBP_MAIN_C "${CMAKE_CURRENT_SOURCE_DIR}/CBP_main.c" PARENT_SCOPE)
# build this, too. it's just easier to use :)
add_library(cbp_main "${CMAKE_CURRENT_SOURCE_DIR}/CBP_main.c")
