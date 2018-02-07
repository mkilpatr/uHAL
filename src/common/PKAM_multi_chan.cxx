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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//ROOT include files
//#include "TCanvas.h"
//#include "TStyle.h"
//#include "TH1.h"
//#include "TH1F.h"
//#include "TFile.h"

int main( int argc, char* argv[] ) {

  using namespace uhal;
  using namespace std;

  int Which_Chan = 0;
  int Number_Channels = 0;

  ConnectionManager manager ("file:/settings/GLIB_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_10" );
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_9" );

  if(argc == 1){
	std::cout << "What PKAM value do you want?" << std::endl;
	std::cout << "A value is n*256 + 28 clock cycles" << std::endl;
	std::cout << "Then need 1(0) to enable(disable)" << std::endl;
	return 0;
  }

  string Chan[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};
  int PKAM_value = 0;
  int PKAM_en = 0;

  if (atoi(argv[1]) == 0){
	Number_Channels = strtol(argv[2], NULL, 16);
	PKAM_value = strtol(argv[3], NULL, 16);
	PKAM_en = strtol(argv[4], NULL, 16);

	PKAM_value = 0x14140000 | (PKAM_value << 8) | (PKAM_value);
	PKAM_en = ((PKAM_en << 1) | PKAM_en) << 28;

        if(Number_Channels == 0)
        	Which_Chan = 0x11111111;
        else
                Which_Chan = Number_Channels;
	for (int i = 0; i < 8; i++){
                string PKAM_Reset ("PKAM_Reset_");
                string PKAM_Enable ("PKAM_Enable_");
                
		if((Number_Channels & 0x1) == 1){
			PKAM_Reset += Chan[i];
			PKAM_Enable += Chan[i]; 			

  			hw.getNode( PKAM_Reset ).write(PKAM_value);
  			ValWord< uint32_t > mem = hw.getNode ( PKAM_Reset ).read();
  			hw.dispatch();
  			std::cout << PKAM_Reset << " = " << std::hex << mem.value() << std::endl;

  			hw2.getNode( PKAM_Reset ).write(PKAM_value);
  			ValWord< uint32_t > mem2 = hw2.getNode ( PKAM_Reset ).read();
  			hw2.dispatch();
  			std::cout << PKAM_Reset << " = " << std::hex << mem.value() << std::endl;

  			hw.getNode( PKAM_Enable ).write(PKAM_en);
  			mem = hw.getNode ( PKAM_Enable ).read();
  			hw.dispatch();
  			std::cout << PKAM_Enable << " = " << std::hex << mem.value() << std::endl;
	
  			hw2.getNode( PKAM_Enable ).write(PKAM_en);
  			mem2 = hw.getNode ( PKAM_Enable ).read();
  			hw2.dispatch();
  			std::cout << PKAM_Enable << " = " << std::hex << mem.value() << std::endl;
		}
		Number_Channels = Number_Channels >> 4;
	}
  } 
 
  return 0;
}
