#include "raw_data_access.h"

using namespace gov::fnal::uboone::datatypes;	

raw_data_containter<raw_data_type> readFile(std::string const& fileName);

int main_test(int argc, char **argv)
{
	auto data=readFile("/Users/laptopuser/test1fragment.dat");
	ub_RawData raw_data(data.begin(),data.end());
	
	// dumpRawData(data);
	 ub_TPC_CrateData_v6 crate(raw_data);
	 
	 std::cout <<std::endl <<crate.debugInfo();
	 
	 
     std::cout << "Done" <<std::endl << std::flush;

	//auto size= crate.getMarkedRawCardsData().size();	
}
