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
  $ENV{ART_DIR}/Modules
  $ENV{CETBUILDTOOLS_DIR}/Modules
  ${CMAKE_MODULE_PATH}
  )

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT WERROR NO_UNDEFINED EXTRA_FLAGS -pedantic)

cet_have_qual( e2 )

if ( ${CET_HAVE_QUAL} )
  # -D_GLIBCXX_USE_NANOSLEEP is needed to make std::this_thread::sleep_for available.
  cet_add_compiler_flags(CXX -std=c++11 -D_GLIBCXX_USE_NANOSLEEP)
endif()

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
find_ups_product( cetbuildtools v3_04_04 )
find_ups_product( artdaq v1_00_00 )
find_ups_product( messagefacility v1_10_16 )
#find_ups_product( xmlrpc_c v1_25_18 )

find_ups_boost( v1_53_0 system
                        filesystem
                        program_options
                        regex
                        thread
                        date_time
                        unit_test_framework
                        serialization
 )

find_ups_product( ganglia              v3_2_0 )
find_ups_product( novadaqmonitorclient v3_00_00 )
find_ups_product( novadaqutilities     v3_00_00 )

cet_check_gcc()


# define MESSAGEFACILITY
find_library( MF_MESSAGELOGGER NAMES MF_MessageLogger
                              PATHS $ENV{MESSAGEFACILITY_LIB} )
find_library( MF_MESSAGESERVICE NAMES MF_MessageService
                              PATHS $ENV{MESSAGEFACILITY_LIB} )
find_library( MF_UTILITIES NAMES MF_Utilities
                              PATHS $ENV{MESSAGEFACILITY_LIB} )
find_library( MF_EXTENSIONS NAMES MF_ExtModules
                              PATHS $ENV{MESSAGEFACILITY_LIB} )

#define ARTDAQ LIBRARIES
find_library( ART_DAQ_RATE NAMES artdaq_DAQrate PATHS $ENV{ARTDAQ_LIB} )
find_library( ART_DAQ_DATA NAMES artdaq_DAQdata PATHS $ENV{ARTDAQ_LIB} )


include_directories ( $ENV{GANGLIA_INC})
include_directories ( $ENV{XERCES_C_INC})
include_directories ( $ENV{MESSAGEFACILITY_INC})

#find_library( SIGC    NAMES sigc-2.0  PATHS $ENV{LIBSIGCPP_LIB} NO_DEFAULT_PATH )

set( GANGLIALIB -L$ENV{GANGLIA_LIB} -lganglia)
find_library( MONITORCLIENTLIB NAMES novadaqmonitorclient PATHS $ENV{NOVADAQMONITORCLIENT_LIB} )
find_library( DAQUTILITIESLIB NAMES novadaqutilities PATHS $ENV{NOVADAQUTILITIES_LIB} )
find_library(XERCESCLIB NAMES xerces-c PATHS $ENV{XERCESCROOT}/lib)



# Debug streamer.
string(TOUPPER ${CMAKE_BUILD_TYPE} BTYPE_UC)
if(BTYPE_UC STREQUAL DEBUG)
  add_definitions(-DDEBUGME)
endif()

cet_have_qual( nompi )
if(NOT ${CET_HAVE_QUAL})
  set(WANT_MPI TRUE)
endif()

if(${WANT_MPI})
  find_package(MPI QUIET)
  if(MPI_FOUND)
    # Remove cxx library -- compiled with wrong version of GCC.
    LIST(REMOVE_AT MPI_LIBRARIES 0)
  endif()
endif()

message(STATUS "Project is ${product} ${version} ${qualifier}")

# macros for art_dictionary and simple_plugin
include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

set (COMMON_PROJECT_LIBS
  ${CETLIB}
  ${ARTDAQ_DAQDATA}
  ${ARTDAQ_DAQRATE}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_DATE_TIME_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${Boost_SERIALIZATION_LIBRARY}
  ${MF_MESSAGELOGGER}
  ${MF_UTILITIES}
#  ${MF_MESSAGESERVICE}
  ${MF_EXTENSIONS}
  ${GANGLIALIB}
  ${MONITORCLIENTLIB}
  ${DAQUTILITIESLIB}
  ${XERCESCLIB}
  ${FHICLCPP}
  -lpthread
 )

set_source_files_properties( $ENV{NOVADAQUTILITIES_INC}/NovaDAQUtilities/Runnable.h PROPERTIES COMPILE_FLAGS "-Wno-error=pedantic -Wno-pedantic" )


# source
add_subdirectory(share)
add_subdirectory(datatypes)
add_subdirectory(gps)

# ups - table and config files
add_subdirectory(ups)

# packaging utility
include(UseCPack)
