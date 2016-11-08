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

  ConnectionManager manager ("file://test/dummy_connections_multi_chan_test.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_11" );
  //HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_3" );

  int Random_Thres = strtol(argv[1], NULL, 16);

  hw.getNode( "Random_Threshold" ).write(Random_Thres);
  ValWord<uint32_t> mem = hw.getNode ( "Random_Threshold" ).read();
  hw.dispatch();
  std::cout << "Threshold = " << mem.value() << std::endl;

  for(int i = 0; i < 100; i++){
  	mem = hw.getNode ( "Random_Number" ).read();
  	hw.dispatch();
  	std::cout << "Random Number = " << mem.value() << std::endl;
  	sleep(0.25);
  }

  return 0;
}
