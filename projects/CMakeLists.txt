# ======================================================================
# Create build and install directories which are outside the source code directory
# Declare the install path from the cmake command line using either a relative or full path
#
#   cd .../path/to/build/directory
#   source .../path/to/uboonedaq/ups/setup_for_development [-d]
#   cmake [-DCMAKE_INSTALL_PREFIX=.../path/to/install/directory] $uboonedaq_SOURCE
#   make
#   make test
#   make install
#   make package
# ======================================================================
# use cmake 2.8 or later
cmake_minimum_required (VERSION 2.8)

PROJECT(uboonedaq-datatypes)

# cetbuildtools contains our cmake modules
SET ( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
IF (NOT CETBUILDTOOLS_VERSION)
    MESSAGE (FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules")
ENDIF()

set(CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/Modules
  $ENV{CETBUILDTOOLS_DIR}/Modules
  ${CMAKE_MODULE_PATH}
  )

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT WERROR NO_UNDEFINED EXTRA_FLAGS -pedantic)

cet_have_qual( e2 )

if ( ${CET_HAVE_QUAL} )
  cet_add_compiler_flags(CXX -std=c++11 -D_GLIBCXX_USE_NANOSLEEP)
endif()

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
find_ups_product( cetbuildtools v3_04_04 )

find_ups_boost( v1_53_0  serialization )

cet_check_gcc()


# Debug streamer.
string(TOUPPER ${CMAKE_BUILD_TYPE} BTYPE_UC)
if(BTYPE_UC STREQUAL DEBUG)
  add_definitions(-DDEBUGME)
endif()

message(STATUS "Project is ${product} ${version} ${qualifier}")

set (COMMON_PROJECT_LIBS 
  ${Boost_SERIALIZATION_LIBRARY} 
  )

# source
add_subdirectory(share)
add_subdirectory(datatypes)
add_subdirectory(datatypes_utilities)
add_subdirectory(gps)

# ups - table and config files
add_subdirectory(ups)

# packaging utility
include(UseCPack)
