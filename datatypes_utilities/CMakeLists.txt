#this is just so we have a nice sample code
set_source_files_properties(eventReader.cc PROPERTIES COMPILE_FLAGS "-Wno-unused-variable $<$<CXX_COMPILER_ID:GNU>:-Wno-unused-but-set-variable>")


cet_make_exec(NAME dumpEvents
              LIBRARIES
              PRIVATE
              uboonedaq_datatypes::datatypes
              Boost::serialization
             )

cet_make_exec(NAME dumpEventHeaders
              LIBRARIES
              PRIVATE
              uboonedaq_datatypes::datatypes
              Boost::serialization
             )

cet_make_exec(NAME fragmentReader
              LIBRARIES
              PRIVATE
              uboonedaq_datatypes::datatypes
              Boost::serialization
             )

cet_make_exec(NAME eventReader
              LIBRARIES
              PRIVATE
              uboonedaq_datatypes::datatypes
              Boost::serialization
             )

cet_make_exec(NAME exportRawFragments
              LIBRARIES
              PRIVATE
              uboonedaq_datatypes::datatypes
              Boost::serialization
             )

cet_make_exec(NAME dumpRawPMTCrateFrame
              LIBRARIES
              PRIVATE
              uboonedaq_datatypes::datatypes
              Boost::serialization
             )

cet_make_exec(NAME dumpRawTPCCrateFrame
              LIBRARIES
              PRIVATE
              uboonedaq_datatypes::datatypes
              Boost::serialization
             )

install_source()
