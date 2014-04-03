# project name
project(udata_utilities)

# ----------------------------------------------------------------------
# Build this project's library:

set( LIB_NAME udata_utilities)


set(LIB_SOURCES
card_utilities.cpp
)

include_directories ( $ENV{WINDRIVER_INC} )
set(WINDRIVERLIB "-L$ENV{WINDRIVER_LIB} -lwdapi1100 -lnevispcie")
find_ups_product( symmetricom        v2_00_00 )
include_directories ( $ENV{SYMMETRICOM_INC} )
set(SYMMETRICOMLIB "-L$ENV{SYMMETRICOM_LIB} -lbcsdk")

# The inclusion of gps/symm.h, necessary for stuffing the GPS time into the crate_Header, drags with it complaints
# about windriver.h functions that don't compile cleanly under gcc10+. We must add all these lines to
# get stuff to compile therefore.

set_source_files_properties(card_utilities.cpp PROPERTIES COMPILE_FLAGS "-Wno-unused-variable -Wno-deprecated-declarations -Wno-unused-function -Wno-sign-compare -Wno-unused-parameter -Wno-format -Wno-unused-but-set-variable -Wno-return-type")

art_make_library(LIBRARY_NAME ${LIB_NAME} SOURCE ${LIB_SOURCES}
LIBRARIES udata_types ${CMAKE_DL_LIBS})


#add_executable(write_read write_read.cpp)

#target_link_libraries(write_read
#    udata_share 
#    ${LIB_NAME}
#    ${COMMON_PROJECT_LIBS}
#    ${CMAKE_DL_LIBS} )

# ----------------------------------------------------------------------
# Install this project's headers, sources, library, and executables:

install_headers()
install_source()

install( TARGETS ${LIB_NAME}
         DESTINATION ${flavorqual_dir}/lib )

