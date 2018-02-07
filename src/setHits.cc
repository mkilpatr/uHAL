#include "uhal/uhal.hpp"
#include "uhal/tests/tools.hpp"
#include <boost/filesystem.hpp>
#include "uhal/HwInterface.hpp"
#include <ctime>
#include <cstring>
#include "../Utils/Utilities.h"
#include "../HWDescription/PixFED.h"
#include "../HWInterface/PixFEDInterface.h"
#include "../HWDescription/PixGLIB.h"
#include "../HWInterface/PixGLIBInterface.h"
#include "../System/SystemController.h"
#include "../AMC13/Amc13Controller.h"
#include "../Utils/Data.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <typeinfo>
#include <list>
#include <stdlib.h>
#include <iterator>
#include <stdexcept>
#include "FIFO_Spy.c"
#include "feroltest.c"
#include "All_Channels.c"
#include "FED_summary.c"
//#include "test_glib.c"

int main( int argc, char* argv[] ){

  using namespace std;
  using namespace uhal;

  int NumHits1 = strtol(argv[1], NULL, 16);
  int NumHits2 = strtol(argv[2], NULL, 16);
  Set_Num_Hits(NumHits1, NumHits2, NumHits1, NumHits2, NumHits1, NumHits2, NumHits1, NumHits2, NumHits1, NumHits2, NumHits1, NumHits2, 0x55555555);

  return 0;
}
