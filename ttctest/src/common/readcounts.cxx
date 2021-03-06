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
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH1F.h"
#include "TFile.h"

int main() {

  using namespace uhal;
  using namespace std;
  
  //int max_read = 1;

  TFile f("Bunch.root","new");
  TH1F h1("Bunch_countHist","Number of Bunches in Orbit",3601,-0.5,3600.5);
  //h1.GetXaxis().SetTitle("Number of Bunches");
  //h1.GetYaxis().SetTitle("Count");

  ConnectionManager manager ("file://test/dummy_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_11" );

  ValWord< uint32_t > mem = hw.getNode ( "L1A_count" ).read();
  hw.dispatch();
  std::cout << "L1A_count = " << mem.value() << std::endl;

  ValWord< uint32_t > memBunch = hw.getNode ( "Bunch_count" ).read();
  hw.dispatch();
  std::cout << "Bunch_count = " << memBunch.value() << std::endl;

  mem = hw.getNode ( "Orbit_count" ).read();
  hw.dispatch();
  std::cout << "Orbit_count = " << mem.value() << std::endl;

  hw.getNode( "TBM_Reset" ).write(0x1);

  mem = hw.getNode ( "TBM_Reset").read();
  hw.dispatch();
  std::cout << "TBM_Reset = " << std::hex << mem.value() << std::endl;

  hw.getNode( "TBM_Reset" ).write(0x0);

  mem = hw.getNode ( "TBM_Reset").read();
  hw.dispatch();
  std::cout << "TBM_Reset = " << std::hex << mem.value() << std::endl;

  /*
  for(int i = 0; i < max_read; i++){
  	mem = hw.getNode ( "Bunch_count" ).read();
        hw.dispatch();
	Float_t Bunch = (Float_t) mem.value();
	h1.Fill(Bunch);
  }
  
  h1.Write();
  */
  return 0;
}
