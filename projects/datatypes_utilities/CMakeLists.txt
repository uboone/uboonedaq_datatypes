# project name
project(udata_utilities)

#this is just so we have a nice sample code
set_source_files_properties(eventReader.cc PROPERTIES COMPILE_FLAGS "-Wno-unused-variable -Wno-unused-but-set-variable")

add_executable(dumpEvents dumpEvents.cc)
add_executable(dumpEventHeaders dumpEventHeaders.cc)

add_executable(fragmentReader fragmentReader.cc)

add_executable(eventReader eventReader.cc)

add_executable(exportRawFragments exportRawFragments.cc)

add_executable(dumpRawPMTCrateFrame dumpRawPMTCrateFrame.cc)

add_executable(dumpRawTPCCrateFrame dumpRawTPCCrateFrame.cc)


target_link_libraries(fragmentReader
    ${COMMON_PROJECT_LIBS}
    ubdata_types
    )
    
target_link_libraries(dumpEvents
    ${COMMON_PROJECT_LIBS}
    ubdata_types
    )

target_link_libraries(dumpEventHeaders
    ${COMMON_PROJECT_LIBS}
    ubdata_types
    )

target_link_libraries(eventReader
    ${COMMON_PROJECT_LIBS}
    ubdata_types
    )

target_link_libraries(exportRawFragments
    ${COMMON_PROJECT_LIBS}
    ubdata_types
    )

target_link_libraries(dumpRawPMTCrateFrame
    ${COMMON_PROJECT_LIBS}
    ubdata_types
    )

target_link_libraries(dumpRawTPCCrateFrame
    ${COMMON_PROJECT_LIBS}
    ubdata_types
    )
        
install_headers()

install_source()

install( TARGETS dumpEvents dumpEventHeaders fragmentReader eventReader exportRawFragments dumpRawPMTCrateFrame dumpRawTPCCrateFrame
         DESTINATION ${flavorqual_dir}/bin )
