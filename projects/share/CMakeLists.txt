# project name
project(udata_share)

# ----------------------------------------------------------------------
# Build this project's library:

set( LIB_NAME udata_share)

include_directories ( $ENV{WINDRIVER_INC} )
set( WINDRIVERLIB "-L$ENV{WINDRIVER_LIB} -lwdapi1100 -lnevispcie")

set(LIB_SOURCES
event_types.cpp
)


art_make_library(LIBRARY_NAME udata_share SOURCE ${LIB_SOURCES} LIBRARIES ${COMMON_PROJECT_LIBS})


# ----------------------------------------------------------------------
# Install this project's headers, sources, library, and executables:
install_headers()
install_source()
install( TARGETS ${LIB_NAME}
         DESTINATION ${flavorqual_dir}/lib )
