#include "uhal/uhal.hpp"
#include "uhal/tests/tools.hpp"
#include <boost/filesystem.hpp>
#include "uhal/HwInterface.hpp"
#include <ctime>
#include <cstring>
#include "../Utils/Utilities.h"
#include "../HWDescription/PixFED.h"
#include "../HWInterface/PixFEDInterface.h"
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

void Set_ROCS( int roc_input);
void Set_Num_Hits( int hit_input);
void Scan_Pixels();
void Scan_ADC( int increment );
void Test_Hits(int input_loops);

int main()
{

  using namespace std;
  using namespace uhal;
 
  ConnectionManager manager ("file://test/dummy_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_3" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card

  std::cout << "    ******************************************    " << std::endl;
  std::cout << "    *                                        *    " << std::endl;
  std::cout << "    *               FED Tester               *    " << std::endl;
  std::cout << "    *                                        *    " << std::endl;
  std::cout << "    ******************************************    " << std::endl;
  std::cout << " " << std::endl;
  std::cout << "Testing Options:" << std::endl;
  std::cout << "  0. Run all tests" << std::endl;
  std::cout << "  1. Run pixel alive test" << std::endl;
  std::cout << "  2. Check resets" << std::endl;
  std::cout << "  3. Calibration mode" << std::endl;
  std::cout << "  4. Do some PKAM thing" << std::endl;
  std::cout << " " << std::endl;
  std::cout << "Which test would you like to run? ";

  unsigned int test_num;
  std::cin >> test_num;

  if (test_num == 0) {
    //Run ALL the tests!
    std::cout << "How many ROCS in each channel? (max 8) (0 = default) ";
    int roc_input;
    std::cin >> roc_input;
    Set_ROCS( roc_input);
    
    std::cout << "How many hits in each ROC? (max 16) (0 = default) ";
    int hit_input;
    std::cin >> hit_input;
    Set_Num_Hits( hit_input );
  }

  else if (test_num == 1) {
    //Run the pixel alive test
  }  

  else if (test_num == 2) {
    //Check resets
  }

  else if (test_num == 3) {
    //Calibration mode
  }

  else if (test_num == 4) {
    //Do some PKAM thing
  } 

  else if (test_num >= 5) {
    std::cout << "Not a valid test. Shutting down." << std::endl;
    return 0;
  }
 
  return 0;
}

void Set_ROCS(int roc_input)
{
  using namespace uhal;

  uint32_t Number_ROCS = 0x88;
  uint32_t inter_num;  
  ConnectionManager manager ("file://test/dummy_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_3" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card


  if (roc_input == 0) { }
  else if (roc_input <= 8) {
    inter_num = (roc_input << 4);
    Number_ROCS = inter_num | roc_input;
  }

  hw.getNode("ROC_Number").write( Number_ROCS );
  ValWord < uint32_t > mem = hw.getNode ( "ROC_Number" ).read();
  hw.dispatch();
  std::cout << "ROC_Number = " << std::hex << mem.value() << std::endl;

  hw2.getNode("ROC_Number").write( Number_ROCS );
  ValWord < uint32_t > mem2 = hw2.getNode ( "ROC_Number" ).read();
  hw2.dispatch();
  std::cout << "ROC_Number = " << std::hex << mem2.value() << std::endl;
	 
}

void Set_Num_Hits(int hit_input)
{
  using namespace uhal;
  ConnectionManager manager ("file://test/dummy_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_3" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card

  uint32_t Hits = 0x00000000;
  uint32_t inter_hits[8];

  if (hit_input == 0) { }
  else {
    for (int i = 0; i < 8; i++) {
      inter_hits[i] = hit_input << (4*(7-i));
    }
    Hits = inter_hits[0] | inter_hits[1] | inter_hits[2] | inter_hits[3] | inter_hits[4] | inter_hits[5] | inter_hits[6] | inter_hits[7];
  }

  ValWord < uint32_t > mem2;

  hw.getNode("CHA_Hits").write( Hits );
  ValWord < uint32_t > mem = hw.getNode ( "CHA_Hits" ).read();
  hw.dispatch();
  std::cout << "CHA_Hits = " << std::hex << mem.value() << std::endl;
  	
  hw.getNode("CHB_Hits").write( Hits );
  mem = hw.getNode ( "CHB_Hits" ).read();
  hw.dispatch();
  std::cout << "CHB_Hits = " << std::hex << mem.value() << std::endl;

  hw2.getNode("CHA_Hits").write( Hits );
  mem2 = hw2.getNode ( "CHA_Hits" ).read();
  hw2.dispatch();
  std::cout << "CHA_Hits = " << std::hex << mem2.value() << std::endl;
  	
  hw2.getNode("CHB_Hits").write( Hits );
  mem2 = hw2.getNode ( "CHB_Hits" ).read();
  hw2.dispatch();
  std::cout << "CHB_Hits = " << std::hex << mem2.value() << std::endl;
 
}

void Scan_Pixels() 
{
  using namespace std;
  using namespace uhal;
  ConnectionManager manager ("file://test/dummy_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_3" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card

  int j,k,i;
  int Max_Column = 0;
  int Max_Row = 0;
  int Hit_infoA_Start = 0;
  int Hit_infoB_Start = 0;
  ValWord < uint32_t > mem, mem2;
  
  string ROCSA[ 8 ] = {"CHA_ROC0", "CHA_ROC1", "CHA_ROC2", "CHA_ROC3", "CHA_ROC4", "CHA_ROC5", "CHA_ROC6", "CHA_ROC7"};
  string ROCSB[ 8 ] = {"CHB_ROC0", "CHB_ROC1", "CHB_ROC2", "CHB_ROC3", "CHB_ROC4", "CHB_ROC5", "CHB_ROC6", "CHB_ROC7"}; 
	
  Max_Column = 51;
  Max_Row = 79;
  Hit_infoA_Start = 0;
  Hit_infoB_Start = 0;

  int Hit_infoA_dummy = Hit_infoA_Start;
  int Hit_infoB_dummy = Hit_infoB_Start;

  for( j = 0; j < Max_Row; j++){
    int Hit_infoA = Hit_infoA_dummy;
    int Hit_infoB = Hit_infoB_dummy;
  		
    for( k = 0; k < Max_Column; k++){

      for( i = 0; i < 8; i++){
        hw.getNode( ROCSA[i] ).write(Hit_infoA);
	mem = hw.getNode ( ROCSA[i] ).read();
	hw.dispatch();
	std::cout << ROCSA[i] << " = " << std::hex << mem.value() << std::endl;

	hw.getNode( ROCSB[i] ).write(Hit_infoB);
	mem = hw.getNode ( ROCSB[i] ).read();
	hw.dispatch();
	std::cout << ROCSB[i] << " = " << std::hex << mem.value() << std::endl;

	hw2.getNode( ROCSA[i] ).write(Hit_infoA);
	mem2 = hw2.getNode ( ROCSA[i] ).read();
	hw2.dispatch();
	std::cout << ROCSA[i] << " = " << std::hex << mem2.value() << std::endl;

	hw2.getNode( ROCSB[i] ).write(Hit_infoB);
	mem2 = hw2.getNode ( ROCSB[i] ).read();
	hw2.dispatch();
	std::cout << ROCSB[i] << " = " << std::hex << mem2.value() << std::endl;		
      }	

      Hit_infoA = Hit_infoA + 0x00000100;
      Hit_infoB = Hit_infoB + 0x00000100;	

      if(strtol(argv[6], NULL, 16) == 1){
      system("/home/fectest/FEDtester/MrPixel/Desktop/amc13/AMC13Pixel_Emul_command.bash");	
      }
    }
  }

}

void Scan_ADC(int increment)
{
  using namespace uhal;
  using namespace std;
  ConnectionManager manager ("file://test/dummy_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_3" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card

  int i,j;
  int min_adc = 0;
  int max_adc = 0;
  ValWord < uint32_t > mem,mem2; 
  string ROCSA[ 8 ] = {"CHA_ROC0", "CHA_ROC1", "CHA_ROC2", "CHA_ROC3", "CHA_ROC4", "CHA_ROC5", "CHA_ROC6", "CHA_ROC7"};
  string ROCSB[ 8 ] = {"CHB_ROC0", "CHB_ROC1", "CHB_ROC2", "CHB_ROC3", "CHB_ROC4", "CHB_ROC5", "CHB_ROC6", "CHB_ROC7"};

  min_adc = 0;
  if (min_adc < 0) {
    min_adc = 0;
  }
  else if (min_adc > 255) {
    min_adc = 255;
  }
  max_adc = 255;
  if (max_adc < min_adc && max_adc != 0) {
    max_adc = min_adc;
    std::cout << "Max is less than min." << std::endl;
  }
  else if (max_adc > 255 || max_adc == 0) {
    max_adc = 255;
  }
   
  int hit_info = min_adc;

  for (i=min_adc;i<max_adc;i=i+increment){
    for (j=0;j<8;j++){
      hw.getNode( ROCSA[j] ).write(hit_info);
      mem = hw.getNode ( ROCSA[j] ).read();
      hw.dispatch();
      std::cout << ROCSA[j] << " = " << std::hex << mem.value() << std::endl;

      hw.getNode( ROCSB[j] ).write(hit_info);
      mem = hw.getNode ( ROCSB[j] ).read();
      hw.dispatch();
      std::cout << ROCSB[j] << " = " << std::hex << mem.value() << std::endl;

      hw2.getNode( ROCSA[j] ).write(hit_info);
      mem2 = hw2.getNode ( ROCSA[j] ).read();
      hw2.dispatch();
      std::cout << ROCSA[j] << " = " << std::hex << mem2.value() << std::endl;

      hw2.getNode ( ROCSB[j] ).write(hit_info);
      mem2 = hw2.getNode ( ROCSB[j] ).read();
      hw2.dispatch();
      std::cout << ROCSB[j] << " = " << std::hex << mem2.value() << std::endl;        
    }   
    
    hit_info = hit_info + (0x00000001*increment);
    std::cout << "Right here" << std::endl;
  }

}

void Test_Hits(int input_loops)
{

  const char* cHWFile = "home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription.xml";

  uhal::setLogLevelTo(uhal::Debug());

  // instantiate System Controller
  SystemController cSystemController;
  Amc13Controller  cAmc13Controller;

  // initialize map of settings so I can know the proper number of acquisitions and TBMs
  cSystemController.InitializeSettings(cHWFile, std::cout);

  // initialize HWdescription from XML, beware, settings have to be read first
  cAmc13Controller.InitializeAmc13( cHWFile, std::cout );
  cSystemController.InitializeHw(cHWFile, std::cout);

  // configure the HW
  cAmc13Controller.ConfigureAmc13( std::cout );
  cSystemController.ConfigureHw(std::cout );

  //cAmc13Controller.fAmc13Interface->StartL1A();
  //for (fAmc13Controller->BGO vector)
  cAmc13Controller.fAmc13Interface->EnableBGO(0);

  auto cSetting = cSystemController.fSettingsMap.find("NAcq");
  int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
  cSetting = cSystemController.fSettingsMap.find("BlockSize");
  int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;

  cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
  int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;

  cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
  int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;

  // get the board info of all boards and start the acquistion
  //for (auto& cFED : cSystemController.fPixFEDVector)
  //{
      auto& cFED = cSystemController.fPixFEDVector[0];
      auto& cFED2 = cSystemController.fPixFEDVector[1];
      auto& cFED3 = cSystemController.fPixFEDVector[2];
      cSystemController.fFEDInterface->getBoardInfo(cFED);
      cSystemController.fFEDInterface->findPhases(cFED, cChannelOfInterest);
  //}

  std::vector<unsigned int> FIFO;
  std::vector<unsigned int> FIFO_CHB;
  std::vector<unsigned int> Event_Number;
  unsigned int Event_one = 0;
  unsigned int Event_two = 0;

  int Total_Events = 0;
  int Incorrect_Header = 0;
  int Incorrect_Trailer = 0;
  int Incorrect_Event_Num = 0;
  int Incorrect_ROC = 0;
  int Incorrect_adc = 0;
  int Incorrect_col = 0;
  int Incorrect_row = 0;
  int Incorrect_pix_info = 0;
  int ROC_Value = 0;
  int Compare_Event = 0;
  int Loops = input_loops;
  uint32_t Current_Event_Num = 0;
  uint32_t Current_col_num = 0;
  uint32_t Current_row_num = 0;
  uint32_t Current_adc_num = 0;
  int Error_Count = 0;
  int Do_Error = 0;
  int Pixel = 0;
  int Pixel_Hit = 0;
  int Incorrect_Pixel = 0;
  std::fill(FIFO.begin(), FIFO.end(), 0);
  std::fill(FIFO_CHB.begin(), FIFO_CHB.end(), 0);

  int max_row = 80; //atoi(argv[3]); //80
  if (max_row > 80 || max_row == 0) {
  	max_row = 80;
  }
  int max_col = 52; //atoi(argv[4]); //52
  if (max_col > 52 || max_col == 0) {
  	max_col = 52;
  }
  int max_adc = 255; //atoi(argv[5]); //255
  if (max_adc > 255 || max_adc == 0) {
  	max_adc = 255;
  }
  string ROCSA[ 8 ] = {"CHA_ROC0", "CHA_ROC1", "CHA_ROC2", "CHA_ROC3", "CHA_ROC4", "CHA_ROC5", "CHA_ROC6", "CHA_ROC7"};
  string ROCSB[ 8 ] = {"CHB_ROC0", "CHB_ROC1", "CHB_ROC2", "CHB_ROC3", "CHB_ROC4", "CHB_ROC5", "CHB_ROC6", "CHB_ROC7"};

  int adc_loops = 0;
  int col_loops = 0;
  int row_loops = 0;

  int test_adc = 0;
  int test_col = 0;
  int test_row = 0;


  for (int l = 0; l < Loops; l++) 
  {
    if(l%5000 == 0){
      std::cout << "Loop Number = " << std::dec << l << std::endl;
    }     

    int Hit_infoA = 0;
    int Hit_infoB = 0;

    int row = 0;
    int col = 0;
    int adc = 0;

    for (row = 0; row < max_row; row++) {

      for (col = 0; col < max_col; col++) {

      	std::cout << "Starting row " << std::dec << row << ", column " << std::dec << col << std::endl;

        for (adc = 0; adc < max_adc; adc++) {

          for (int i = 0; i < 8; i++) {

            //hw.getNode( ROCSA[i] ).write(Hit_infoA);
            //mem = hw.getNode ( ROCSA[i] ).read();
            //hw.dispatch();
            cSystemController.fFEDInterface->WriteBoardReg(cFED2, ROCSA[i], Hit_infoA);
            cSystemController.fFEDInterface->WriteBoardReg(cFED3, ROCSA[i], Hit_infoA);
            //std::cout << ROCSA[i] << " = " << std::hex << mem.value() << std::endl;

            //hw.getNode( ROCSB[i] ).write(Hit_infoB);
            //mem = hw.getNode ( ROCSB[i] ).read();
            //hw.dispatch();
            cSystemController.fFEDInterface->WriteBoardReg(cFED2, ROCSB[i], Hit_infoB);
            cSystemController.fFEDInterface->WriteBoardReg(cFED3, ROCSB[i], Hit_infoB);
            //std::cout << ROCSB[i] << " = " << std::hex << mem.value() << std::endl;

          }
	  // for (auto& cFED : cSystemController.fPixFEDVector)
          // {

	  cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
	  //std::cout << "cFED = " << cFED << std::endl;

	  if(l == 0 && row == 0 && col == 0 && adc == 0){
	    sleep(4);
	    cAmc13Controller.fAmc13Interface->BurstL1A();
	    sleep(0.001);
          }
	  else{
	    cAmc13Controller.fAmc13Interface->BurstL1A();
	    sleep(0.001);
          }

          FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
          FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);

	  int k = 0;
	  int j = 0;
          int h = 0;
	  int g = 0;
	  int Next_step = 0;
	  int Next_step_roc = 1;
	  int Num_ROC = 0;
	  int END_Event = 0;
	  int Error_Loop = 0;
	  int Start_Event = 0;
	  int Stack[2];
	  int Do_Stack = 0;

	  for( std::vector<unsigned int>::size_type i = 0; i != FIFO.size(); i++ )
	  {

	    if(FIFO[i] == 0x00){
	      break;
	    }

	    if(FIFO[i] >> 4 == 0x8){
	      Error_Count = 0;
	      Error_Loop = 0;
	      h++;
	      g = END_Event - Start_Event;
	      if( h >= 2 ){
	        k = 1;
	      }
	      //std::cout << "****************** Start bitstream ****************" << std::endl;
	    }

	    // if (FIFO[i] >> 4 == 0x7){
	    //   std::cout << "0x" << FIFO[i] << " **********************" << std::endl;
	    // }
	    // else {
	    // 	 std::cout << "0x" << FIFO[i] << std::endl;
	    // }
						
	    if(FIFO[i] >> 4 == 0x9)
	    {
	      j = i;
	      Total_Events++;
	      Current_Event_Num = ((FIFO[i - 1] & 0xF) << 4) | (FIFO[i] & 0xF);
	      if(Current_Event_Num - Compare_Event != 1 && Compare_Event - Current_Event_Num != 255){
	        Incorrect_Event_Num++;
                Error_Count++;
	      }
	      Compare_Event = Current_Event_Num;
	    }

            // if (FIFO[i] >> 4 == 0x7 && Next_step_roc == 0){
	    //   Next_step_roc = 1;
	    //   continue;
	    // }
	    // else if (FIFO[i] >> 4 == 0x7 && Next_step_roc != 0){
	    // 	 Incorrect_pix_info++;
	    //   Error_Count++;
	    // 	 continue;
	    // }

	    if (FIFO[i] >> 4 == 0x1 && Next_step_roc == 1){
	      Next_step_roc = 2;
	      continue;
	    }
	    else if (FIFO[i] >> 4 == 0x1 && Next_step_roc != 1){
	      Incorrect_pix_info++;
	      Error_Count++;
	      Next_step_roc = 2;
	      continue;
	    }

            if (FIFO[i] >> 4 == 0x2 && Next_step_roc == 2) {
	      Current_col_num = ((FIFO[i - 1] & 0xF) << 2) | ((FIFO[i] & 0xF) >> 2);
	      test_col = col;
	      //std::cout << "col = " << std::dec << col << " Current_col_num = " << std::dec << Current_col_num << std::endl;
	      // std::cout << std::hex << FIFO[i-1] << " = " << std::dec << FIFO[i-1] << std::endl;
	      // std::cout << std::hex << FIFO[i] << " = " << std::dec << FIFO[i] << std::endl;
	      // std::cout << std::hex << FIFO[i] << " >> " << std::hex << (FIFO[i] & 0xF) << " >> " << std::hex << ((FIFO[i] & 0xF) >> 2) << std::endl;
	      // std::cout << std::hex << ((FIFO[i] & 0xF) >> 2) << " turns into " << std::hex << ((FIFO[i - 1] & 0xF) << 2) << " | " << std::hex << ((FIFO[i] & 0xF) >> 2) << " = " << std::dec << Current_col_num << " =? " << std::dec << col << std::endl;
	      // if (test_col = 36){
	      //   test_col = 16;
	      // }
	      if (Current_col_num != test_col){
	        Incorrect_col++;
		Error_Count++;
	      }
	      Next_step_roc = 3;
	    }
	    else if (FIFO[i] >> 4 == 0x2 && Next_step_roc != 2){
	      Incorrect_pix_info++;
	      Error_Count++;
	      Next_step_roc = 3;
	      continue;
	    }

            if (FIFO[i] >> 4 == 0x3 && Next_step_roc == 3){
	      Next_step_roc = 4;
	      continue;
	    }
	    else if (FIFO[i] >> 4 == 0x3 && Next_step_roc == 3){
	      Incorrect_pix_info++;
	      Error_Count++;
	      Next_step_roc = 4;
	      continue;
	    }

	    if (FIFO[i] >> 4 == 0x4 && Next_step_roc == 4) {
              Current_row_num = ((FIFO[i-1] & 0xF) << 3) | ((FIFO[i] & 0xF) >> 1);
	      test_row = row;
	      //std::cout << "Current_row_num = " << std::dec << Current_row_num << " row = " << std::dec << row << std::endl;
	      if (Current_row_num != test_row){
	        Incorrect_row++;
	        Error_Count++;
	      }
	      Next_step_roc = 5;
	    }
	    else if (FIFO[i] >> 4 == 0x4 && Next_step_roc != 4){
	      Incorrect_pix_info++;
	      Error_Count++;
	      Next_step_roc = 5;
	      continue;
	    }

	    if (FIFO[i] >> 4 == 0x5 && Next_step_roc == 5){
	      Next_step_roc = 6;
	      continue;
	    }
	    else if (FIFO[i] >> 4 == 0x5 && Next_step_roc != 5){
	      Incorrect_pix_info++;
	      Error_Count++;
	      Next_step_roc = 6;
	      continue;
	    }

	    if (FIFO[i] >> 4 == 0x6 && Next_step_roc == 6) {
              Current_adc_num = (((FIFO[i-2] & 0xF) & 0x1) << 7) | (((FIFO[i-1] & 0xF) >> 1) << 4) | (FIFO[i] & 0xF);
              test_adc = adc;
	      //std::cout << "Current_adc_num = " << std::dec << Current_adc_num << " adc = " << std::dec << adc << std::endl;
	      //if (adc == 36 && col == 0 && row == 0){
	      //  test_adc = 16;
	      //}
	      if (Current_adc_num != test_adc){
	        Incorrect_adc++;
		Error_Count++;
	      }
              Next_step_roc = 1;						 
            }
	    else if (FIFO[i] >> 4 == 6 && Next_step_roc != 6) {
	      Incorrect_pix_info++;
	      Error_Count++;
              Next_step_roc = 1;
	      continue;
            }

	    if(FIFO[i] >> 4 == 0x8 && Next_step == 0){ 
	      Next_step = 1;
	      Start_Event = i;
	      Do_Stack = 0;
	      continue;
            }
	    else if(FIFO[i] >> 4 == 0x8 && Next_step != 0){
	      Incorrect_Header++;
	      Error_Count++;
	      continue;
	    }
						
	    if(FIFO[i] >> 4 == 0x9 && Next_step == 1){
	      Next_step = 2;
	      continue;
	    }
	    else if(FIFO[i] >> 4 == 0x9 && Next_step != 1){
              Incorrect_Header++;
	      Error_Count++;
	      continue;
            }
						
	    if(FIFO[i] >> 4 == 0xA && Next_step == 2){
              Next_step = 3;
	      continue;
	    }
	    else if(FIFO[i] >> 4 == 0xA && Next_step != 2){
              Incorrect_Header++;
	      Error_Count++;
	      continue;
	    }
						
	    if(FIFO[i] >> 4 == 0xB && Next_step == 3){
	      Next_step = 4;
	      continue;
	    }
	    else if(FIFO[i] >> 4 == 0xB && Next_step != 3){
	      Incorrect_Header++;
	      Error_Count++;
	      continue;
            }
             
            if(FIFO[i] >> 4 == 0xC && Next_step == 4){
	      Next_step = 5;
	      Num_ROC = 0;

	      if(FIFO[i] & 0x8){
	        std::cout << "There was a NTP" << std::endl;
	      }

	      if(FIFO[i] & 0x4){
                std::cout << "There was a TBM Reset" << std::endl;
              }

              if(FIFO[i] & 0x2){
                std::cout << "There was a ROC Reset" << std::endl;
              }

	      continue;
	    }
	    else if(FIFO[i] >> 4 == 0xC && Next_step != 4){
	      Incorrect_Trailer++;
	      Error_Count++;
	      continue;
            }
						
	    if(FIFO[i] >> 4 == 0xD && Next_step == 5){
	      Next_step = 6;
	      continue;
	    }
	    else if(FIFO[i] >> 4 == 0xD && Next_step != 5){
	      Incorrect_Trailer++;
	      Error_Count++;
	      continue;
	    }

	    if(FIFO[i] >> 4 == 0xE && Next_step == 6){
	      Next_step = 7;
	      //std::cout << FIFO[i] << std::endl;
					
	      if( ((FIFO[i] & 0x4) >> 2) == 0x1 ){
	        std::cout << "There was a PKAM Reset at Event " << std::dec << Current_Event_Num << std::endl;
	      }

	      Stack[0] = (FIFO[i] & 0x3);
	      continue;
            }
	    else if(FIFO[i] >> 4 == 0xE && Next_step != 6){
	      Incorrect_Trailer++;
              Error_Count++;
              continue;
	    }

            if(FIFO[i] >> 4 == 0xF && Next_step == 7){
              Next_step = 0;
	      END_Event = i;
	      Error_Loop = 1;
              Do_Stack = 1;
	      Stack[1] = (Stack[0] << 4) | (FIFO[i] & 0xF);
							
	      if(ROC_Value != 1){
	        Incorrect_ROC++;
	        Error_Count++;
	       }	
             }
             else if(FIFO[i] >> 4 == 0xF && (FIFO[i - 1] >> 4) != 0xF && Next_step != 7){
	       Incorrect_Trailer++;
	       Error_Count++;
	       END_Event = i;
	       Error_Loop = 1;
	     }
						
	     if(Stack[1] != 0x0 && Do_Stack == 1){
	       std::cout << "The Stack count is " << std::dec << Stack[1] << std::endl;
	     }

	     if(Error_Count != 0 && Error_Loop == 1){
	       std::cout << "Event = " << std::dec << Current_Event_Num << " and Loop = " << std::dec << l << std::endl; 
	       std::cout << "Total Number of Errors = " <<std::dec << Error_Count << std::endl;
	       int Error = 0;

               for(Error = Start_Event; Error <= END_Event; Error++){
	         std::cout << "FIFO[" << std::dec << Error << "] = " << std::hex << FIFO[Error] << std::endl;
	       }

	       for(Error = Start_Event; Error <= END_Event; Error++){
	         std::cout << "FIFO_CHB[" << std::dec << Error << "] = " << std::hex << FIFO_CHB[Error] << std::endl;
               }
	       Error_Loop = 0;
	     }

	     if( (FIFO[i] >> 4) == 0x6){
	       Pixel++;
	     }
	     else if((FIFO[i] >> 4) == 0x7){
	       Num_ROC++;
	       ROC_Value = 0;
	     }
					
	     if(Num_ROC == 8){
	       ROC_Value = 1;
	     }
             else{
               ROC_Value = 0;
	     }

  	  } // Ends error check loop
      			
      	  //} // Ends crate component loop
   
	  Hit_infoA = Hit_infoA + 0x00000001;
          Hit_infoB = Hit_infoB + 0x00000001;

          adc_loops++;
             
        } // Ends adc loop

        Hit_infoA = Hit_infoA + 0x00000100 - (0x00000001 * adc_loops);
        Hit_infoB = Hit_infoB + 0x00000100 - (0x00000001 * adc_loops);

        col_loops++;
        adc_loops = 0;

      } // Ends column loop

      Hit_infoA = Hit_infoA + 0x00010000 - (0x00000100 * col_loops);
      Hit_infoB = Hit_infoB + 0x00010000 - (0x00000100 * col_loops);

      row_loops++;
      col_loops = 0;

    } // Ends row loop

    Hit_infoA = Hit_infoA - (0x00010000 * row_loops);
    Hit_infoB = Hit_infoB - (0x00010000 * row_loops);

    row_loops = 0;

  
  } // Ends Loops loop

  std::cout << "There have been " << std::dec << Total_Events << " Events" << std::endl;
  std::cout << "There have been " << std::dec << Incorrect_Event_Num << " Incorrect Event Numbers " << std::endl;
  std::cout << "There have been " << std::dec << Incorrect_Header << " Incorrect TBM Headers " << std::endl;
  std::cout << "There have been " << std::dec << Incorrect_Trailer << " Incorrect TBM Trailers " << std::endl;
  std::cout << "There have been " << std::dec << Incorrect_ROC << " incorrect Number of ROCs " << std::endl;
  std::cout << "There have been " << std::dec << Incorrect_Pixel << " incorrect Number of Pixels " << std::endl;
  std::cout << "There have been " << std::dec << Incorrect_pix_info << " incorrect Number of pix hit info " << std::endl;
  std::cout << "There have been " << std::dec << Incorrect_adc << " incorrect Number of adc values " << std::endl;
  std::cout << "There have been " << std::dec << Incorrect_col << " incorrect Number of columns " << std::endl;
  std::cout << "There have been " << std::dec << Incorrect_row << " incorrect Number of rows " << std::endl;
  
  exit(0);

}
