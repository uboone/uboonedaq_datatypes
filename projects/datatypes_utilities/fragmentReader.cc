#include "datatypes/raw_data_access.h"
#include "datatypes/uboone_data_fragment_metadata.h"
#include <vector>
using namespace gov::fnal::uboone::datatypes;

bool readFile(std::string const& fileName, raw_data_containter<raw_data_type>& data);

#define UNUSED(x) (void)(x)

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    raw_data_containter<raw_data_type> data;
    if(!readFile(argv[1],data))
        return 0;
    ub_RawData raw_data(data.begin(),data.end());
    
    ub_TPC_CrateData_v6 crate(raw_data);    
    std::cout <<std::endl <<crate.debugInfo();
}
