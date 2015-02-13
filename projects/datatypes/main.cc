#include "raw_data_access.h"

using namespace gov::fnal::uboone::datatypes;

raw_data_containter<raw_data_type> readFile(std::string const& fileName);

#define UNUSED(x) (void)(x)

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    auto data=readFile(argv[1]);
    ub_RawData raw_data(data.begin(),data.end());
    ub_TPC_CrateData_v6 crate(raw_data);
    std::cout <<std::endl <<crate.debugInfo();
}
