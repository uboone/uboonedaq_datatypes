# project name
project(udata_utilities)

add_executable(dumpEvents dumpEvents.cc)

add_executable(fragmentReader fragmentReader.cc)


target_link_libraries(fragmentReader
    ${COMMON_PROJECT_LIBS}
    ubdata_types
    )
    
target_link_libraries(dumpEvents
    ${COMMON_PROJECT_LIBS}
    ubdata_types
    )

    
install_headers()

install_source()

install( TARGETS dumpEvents fragmentReader
         DESTINATION ${flavorqual_dir}/bin )
