#include "uhal/uhal.hpp"
#include "uhal/tests/tools.hpp"
#include <boost/filesystem.hpp>
#include "uhal/HwInterface.hpp"

#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>
#include <typeinfo>
#include <list>
#include <stdlib.h>

int main( int argc, char* argv[] ) {

	using namespace uhal;
	using namespace std;
	
	ConnectionManager manager ("file://test/dummy_connections.xml");
	HwInterface hw=manager.getDevice ( "GLIB.crate.slot_3" );   //this is to glib with the optical card
	HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card

	if( argc == 1 ) {
		std::cout << "1. Choose a Marker to insert a wrong header or trailer in the TBM/ROC." << std::endl;
		std::cout << "	A value of 0x5(27 downto 16 for Marker value)(15 downto 8 Clock)(7 downto 0 Clock to set freq) will set the TBM Header." << std::endl;
		std::cout << "	A value of 0xA(27 downto 16 for Marker value)(15 downto 8 Clock)(7 downto 0 Clock to set freq) will set the TBM Trailer." << std::endl;
		std::cout << "	A value of 0xF(27 downto 16 for Marker value)(15 downto 8 Clock)(7 downto 0 Clock to set freq) will set the ROC Header." << std::endl;
		return 0;
	}

	int Marker = 0;
	int Marker_Value = 0;
	int Marker_Value_CHB = 0;

	if( atoi(argv[1]) >=3){
		std::cout << "INVALID ENTRY" << std::endl;
		return 0;
	}

	if( atoi(argv[1]) == 1){
		Marker_Value = strtol(argv[2], NULL, 16);
		Marker_Value_CHB = (Marker_Value & 0xFFF0000);		

		hw.getNode("Marker_Error").write( Marker_Value );
		ValWord < uint32_t > mem = hw.getNode( "Marker_Error" ).read();
		hw.dispatch();
		std::cout << "Marker_Error = " << std::hex << mem.value() << std::endl;

		hw2.getNode("Marker_Error").write( Marker_Value );
                ValWord < uint32_t > mem2 = hw2.getNode( "Marker_Error" ).read();
                hw2.dispatch();
                std::cout << "Marker_Error = " << std::hex << mem2.value() << std::endl;

		hw.getNode("Marker_Error_CHB").write( Marker_Value_CHB );
                mem = hw.getNode( "Marker_Error_CHB" ).read();
                hw.dispatch();
                std::cout << "Marker_Error_CHB = " << std::hex << mem.value() << std::endl;

                hw2.getNode("Marker_Error_CHB").write( Marker_Value_CHB );
                mem2 = hw2.getNode( "Marker_Error_CHB" ).read();
                hw2.dispatch();
                std::cout << "Marker_Error_CHB = " << std::hex << mem2.value() << std::endl;
	}

	/*if( atoi(argv[1]) == 2){
		Marker = strtol(argv[2], NULL, 16);

                hw.getNode("Marker_Clk_value").write( Marker );
                ValWord < uint32_t > mem = hw.getNode( "Marker_Clk_value" ).read();
                hw.dispatch();
                std::cout << "Marker_Clk_value = " << std::hex << mem.value() << std::endl;

                hw2.getNode("Marker_Clk_value").write( Marker );
                ValWord < uint32_t > mem2 = hw2.getNode( "Marker_Clk_value" ).read();
                hw2.dispatch();
                std::cout << "Marker_Clk_value = " << std::hex << mem2.value() << std::endl;

	}*/
	
	return 0;
}
