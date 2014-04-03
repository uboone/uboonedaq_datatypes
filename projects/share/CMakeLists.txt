# project name
project(udata_share)

set( LIB_NAME udata_share)

FILE(GLOB_RECURSE LIB_SOURCES *.cpp )

add_library( ${LIB_NAME} SHARED ${LIB_SOURCES}  ${COMMON_PROJECT_LIBS} )

install_headers()
install_source()

install( TARGETS ${LIB_NAME}
         DESTINATION ${flavorqual_dir}/lib )
