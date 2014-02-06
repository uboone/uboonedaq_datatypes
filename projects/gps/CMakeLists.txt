# project name
project(udata_gps)

# ----------------------------------------------------------------------
# Build this project's library:

set( LIB_NAME udata_gps)

find_ups_product( windriver        v11_00_00 )
include_directories ( $ENV{WINDRIVER_INC} )
set( WINDRIVERLIB "-L$ENV{WINDRIVER_LIB} -lwdapi1100 -lnevispcie")

find_ups_product( symmetricom        v2_00_00 )
include_directories ( $ENV{SYMMETRICOM_INC} )
set( BCSYMLIB "-L$ENV{SYMMETRICOM_LIB} -lbcsdk")

# ----------------------------------------------------------------------
# Install this project's headers, sources, library, and executables:

install_headers()
