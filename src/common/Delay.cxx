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

  ConnectionManager manager ("file:/settings/GLIB_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_11" );
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_9" );

  if(argc == 1){
	std::cout << "Pick a delay value and reset" << std::endl;
	std::cout << "A value of 0x2(7 downto 0 value) will reset" << std::endl;
	std::cout << "Then pick new value 0x(7 downto 0) for delay" << std::endl;
	return 0;
  }

  int Delay = strtol(argv[1], NULL, 16);
  
  hw.getNode( "Delay_Signal" ).write(Delay);
  ValWord< uint32_t > mem = hw.getNode ( "Delay_Signal" ).read();
  hw.dispatch();
  std::cout << "Delay_Signal = " << std::hex << mem.value() << std::endl;

  hw2.getNode( "Delay_Signal" ).write(Delay);
  ValWord< uint32_t > mem2 = hw2.getNode ( "Delay_Signal" ).read();
  hw2.dispatch();
  std::cout << "Delay_Signal = " << mem2.value() << std::endl;

  return 0;
}
