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
  ConnectionManager manager ("file://../../Desktop/amc13_v1_1_0/amc13/etc/amc13/connectionSN43.xml");
  HwInterface hw=manager.getDevice ( "T2" );

  ValWord < uint32_t > mem = hw.getNode("STATUS.TTC.LAST_EVN").read();
  hw.dispatch();
  std::cout << "STATUS.TTC.LAST_EVN = " << std::hex << mem.value() << std::endl;
  
  mem = hw.getNode("STATUS.TTC.LAST_BCN").read();
  hw.dispatch();
  std::cout << "STATUS.TTC.LAST_BCN = " << std::hex << mem.value() << std::endl;

  mem = hw.getNode("STATUS.TTC.LAST_ORN").read();
  hw.dispatch();
  std::cout << "STATUS.TTC.LAST_ORN = " << std::hex << mem.value() << std::endl;

  return 0;
}
