# project name
project(udata_utilities)

# ----------------------------------------------------------------------
# Build this project's library:

set( LIB_NAME udata_utilities)


set(LIB_SOURCES
card_utilities.cpp
)

FILE(GLOB_RECURSE LIB_SOURCES *.cpp )

set_source_files_properties(${LIB_SOURCES} PROPERTIES COMPILE_FLAGS "-Wno-unused-variable -Wno-deprecated-declarations -Wno-unused-function -Wno-sign-compare -Wno-unused-parameter -Wno-format -Wno-unused-but-set-variable -Wno-return-type")

add_library( ${LIB_NAME} SHARED ${LIB_SOURCES}  ${COMMON_PROJECT_LIBS} )
target_link_libraries( ${LIB_NAME} udata_types)

# ----------------------------------------------------------------------
# Install this project's headers, sources, library, and executables:

install_headers()
install_source()

install( TARGETS ${LIB_NAME}
         DESTINATION ${flavorqual_dir}/lib )

