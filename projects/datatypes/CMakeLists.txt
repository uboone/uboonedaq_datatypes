# project name
project(datatypes)

set( LIB_NAME udata_types)

FILE(GLOB_RECURSE LIB_SOURCES *.cpp )

set_source_files_properties(${LIB_SOURCES} PROPERTIES COMPILE_FLAGS "-Wno-unused-variable -Wno-deprecated-declarations -Wno-unused-function -Wno-sign-compare -Wno-unused-parameter -Wno-format -Wno-unused-but-set-variable -Wno-return-type")

add_library( ${LIB_NAME} SHARED ${LIB_SOURCES}  ${COMMON_PROJECT_LIBS} )

install_headers()
install_source()

install( TARGETS ${LIB_NAME}
         DESTINATION ${flavorqual_dir}/lib )

