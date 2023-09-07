# ======================================================================
#  uboonedaq_datatypes main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/lardataobj/ups/setup_for_development <-d|-p>
#  buildtool [-I /install/path] -bt[i] -jN
# ======================================================================

cmake_minimum_required (VERSION 3.20 FATAL_ERROR)

find_package(cetmodules REQUIRED)
project(uboonedaq_datatypes VERSION 6.22.06 LANGUAGES CXX)

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS CAUTIOUS
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic -Wno-unused-local-typedefs -D_GLIBCXX_USE_NANOSLEEP
)

cet_report_compiler_flags(REPORT_THRESHOLD VERBOSE)

find_package(Boost COMPONENTS serialization REQUIRED EXPORT)
find_package(OpenSSL REQUIRED)
find_package(OpenMP REQUIRED)

include(ArtMake)

# source
add_subdirectory(share)
add_subdirectory(datatypes)
add_subdirectory(datatypes_utilities)

# packaging utility
cet_cmake_config()
