# project name

project(datatypes)

set( LIB_NAME ubdata_types)

FILE(GLOB_RECURSE LIB_SOURCES *.cpp )

set_source_files_properties( uboone_data_utils_openmp.cpp PROPERTIES COMPILE_FLAGS " -fopenmp")

#set(CMAKE_SHARED_LINKER_FLAGS  -unresolved-symbols=ignore-in-shared-libs)

add_library( ${LIB_NAME} SHARED ${LIB_SOURCES}  ${COMMON_PROJECT_LIBS})

if( ${qualifier} MATCHES "^c" )
  target_link_libraries (${LIB_NAME} -lcrypto)
else()
  target_link_libraries (${LIB_NAME} -lcrypto -lgomp)
endif()
#target_link_libraries (${LIB_NAME} -lgomp)

install_headers()

install_source()

install( TARGETS ${LIB_NAME}
         DESTINATION ${flavorqual_dir}/lib )
         
