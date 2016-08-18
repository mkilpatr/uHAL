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
  
  //int max_read = 1;

  //TFile f("Bunch.root","new");
  //TH1F h1("Bunch_countHist","Number of Bunches in Orbit",3601,-0.5,3600.5);
  //h1.GetXaxis().SetTitle("Number of Bunches");
  //h1.GetYaxis().SetTitle("Count");

  ConnectionManager manager ("file://test/dummy_connections_multi_chan.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_11" );
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_3" );

  if(argc == 1) {
	std::cout << "Cal_Trigger is enabled by 0x100, disabled by 0x000" << std::endl;
  	return 0;
  }

  int Delay = strtol(argv[1], NULL, 16);
  
  hw.getNode( "Cal_Trigger" ).write(Delay);
  ValWord< uint32_t > mem = hw.getNode ( "Cal_Trigger" ).read();
  hw.dispatch();
  std::cout << "Cal_Trigger = " << std::hex << mem.value() << std::endl;

  hw2.getNode( "Cal_Trigger" ).write(Delay);
  ValWord< uint32_t > mem2 = hw2.getNode ( "Cal_Trigger" ).read();
  hw2.dispatch();
  std::cout << "Cal_Trigger = " << mem2.value() << std::endl;

  return 0;
}
