# project name

project(datatypes)

set( LIB_NAME ubdata_types)

FILE(GLOB_RECURSE LIB_SOURCES *.cpp )

add_library( ${LIB_NAME} SHARED ${LIB_SOURCES}  ${COMMON_PROJECT_LIBS})

target_link_libraries (${LIB_NAME} -lcrypto)

install_headers()

install_source()

install( TARGETS ${LIB_NAME}
         DESTINATION ${flavorqual_dir}/lib )
         