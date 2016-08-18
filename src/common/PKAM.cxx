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

int main() {

  using namespace uhal;
  using namespace std;
  
  //int max_read = 1;

  //TFile f("Bunch.root","new");
  //TH1F h1("Bunch_countHist","Number of Bunches in Orbit",3601,-0.5,3600.5);
  //h1.GetXaxis().SetTitle("Number of Bunches");
  //h1.GetYaxis().SetTitle("Count");

  ConnectionManager manager ("file://test/dummy_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_11" );
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_3" );

  
  hw.getNode( "PKAM_Reset" ).write(0x14140000);
  ValWord< uint32_t > mem = hw.getNode ( "PKAM_Reset" ).read();
  hw.dispatch();
  std::cout << "PKAM_Reset = " << std::hex << mem.value() << std::endl;

  hw2.getNode( "PKAM_Reset" ).write(0x14140000);
  ValWord< uint32_t > mem2 = hw2.getNode ( "PKAM_Reset" ).read();
  hw2.dispatch();
  std::cout << "PKAM_Reset = " << std::hex << mem.value() << std::endl;

  hw.getNode( "PKAM_Enable" ).write(0x00000000);
  mem = hw.getNode ( "PKAM_Enable" ).read();
  hw.dispatch();
  std::cout << "PKAM_Enable = " << std::hex << mem.value() << std::endl;

  hw2.getNode( "PKAM_Enable" ).write(0x00000000);
  mem2 = hw.getNode ( "PKAM_Enable" ).read();
  hw2.dispatch();
  std::cout << "PKAM_Enable = " << std::hex << mem.value() << std::endl;
  
  return 0;
}
