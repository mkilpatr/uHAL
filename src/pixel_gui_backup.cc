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
int Set_Cal(uint32_t cal_input);
int Test_Hits_Resets(int test_input);
void Test_Hits_Full(int loops_input, int col_start, int col_input, int row_start, int row_input, int adc_start, int adc_input, int choice_input);
void PKAM_Enable( int PKAM );
int Write_ROCs(int i, int Hit_infoA, int Hit_infoB);
void Test_Phases(int fiber_input, int time_input);
void Stack_Test(int loops_input, int choice_input);

int main()
{

  using namespace std;
  using namespace uhal;
 
  ConnectionManager manager ("file://test/dummy_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card

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
  std::cout << "  4. Enable PKAM Reset" << std::endl;
  std::cout << "  5. Run selective pixel tests" << std::endl;
  std::cout << "  6. Check stack and no token pass (NTP)" << std::endl;
  //std::cout << "  7. Check full stack" << std::endl;
  std::cout << "  7. Monitor phases" << std::endl;
  std::cout << " " << std::endl;
  std::cout << "Which test would you like to run? ";

  unsigned int test_num;
  std::cin >> test_num;

  std::cout << " " << std::endl;

  if (test_num == 0) {
    //Run ALL the tests!
    std::cout << "How many ROCS in each channel? (max 8) (0 = default) ";
    int roc_input;
    std::cin >> roc_input;
    Set_ROCS( roc_input);
    std::cout << " " << std::endl;
    
    std::cout << "How many hits in each ROC? (max 16) (0 = default) ";
    int hit_input;
    std::cin >> hit_input;
    Set_Num_Hits( hit_input );
    std::cout << " " << std::endl;

    std::cout << "Preparing to check Resets" << std::endl;
    sleep(2);
    std::cout << "Sending first TBM Reset" << std::endl;
    sleep(2);
    Test_Hits_Resets(1);
    std::cout << " " << std::endl;
    sleep(5);

    std::cout << "Sending second TBM Reset" << std::endl;
    sleep(2);
    Test_Hits_Resets(0);
    std::cout << " " << std::endl;
    sleep(5);

    std::cout << "Sending first ROC Reset" << std::endl;
    sleep(2);
    Test_Hits_Resets(2);
    std::cout << " " << std::endl;
    sleep(5);

    std::cout << "Sending second ROC Reset" << std::endl;
    sleep(2);
    Test_Hits_Resets(0);
    std::cout << " " << std::endl;
    sleep(5);

    std::cout << "Starting stack/NTP test" << std::endl;
    sleep(2);
    Set_ROCS(8);
    Set_Num_Hits(6);
    PKAM_Enable(20);
    Stack_Test(1,1);
    std::cout << " " << std::endl;
    sleep(5); 

    std::cout << "Starting pixel alive test" << std::endl;
    sleep(2);
    Set_ROCS( roc_input);
    Set_Num_Hits(hit_input);
    Test_Hits_Full(1,0,0,0,0,0,0,0);
    std::cout << " " << std::endl;    
    sleep(5);

    std::cout << "All tests complete! Thank you for using the FED Tester!" << std::endl;
  
    return 0;   
  }

  else if (test_num == 1) {
    //Run the pixel alive test
    std::cout << "Starting Pixel Alive" << std::endl;
    std::cout << "Setting 8 ROCS per channel" << std::endl;
    sleep(2);
    int roc_input = 8;
    Set_ROCS( roc_input );

    std::cout << "Setting 1 hit per ROC" << std::endl;
    sleep(2);
    int hit_input = 1;
    Set_Num_Hits( hit_input );
    
    std::cout << "Preparing to test each pixel!" << std::endl;
    sleep(2);
    Test_Hits_Full(1,0,0,0,0,0,0,0);

    std::cout << "Pixel alive test complete!" << std::endl;

    return 0;
  }  

  else if (test_num == 2) {
    //Check resets
    std::cout << "Testing Options:" << std::endl;
    std::cout << "  0. None. Send normal events" << std::endl;
    std::cout << "  1. Test TBM Reset" << std::endl;
    std::cout << "  2. Test ROC Reset" << std::endl;
    std::cout << "  3. Test PKAM Reset" << std::endl;
    std::cout << " " << std::endl;
    std::cout << "Which test would you like to run? ";
    int test_input;
    cin >> test_input;
    
    if (test_input == 3) {
      std::cout << "Make sure PKAM is enabled before running this test!" << std::endl;
      std::cout << "Has PKAM already been enabled? 1 = yes, 0 = no ";
      int PKAM_input;
      cin >> PKAM_input;
      if (PKAM_input == 0) {
        std::cout << "Make sure to enable PKAM in the main menu!" << std::endl;
        std::cout << "Exiting program." << std::endl;
        return 0;
      }
      else if (PKAM_input == 1) {
        std::cout << "Starting PKAM Reset" << std::endl;
        sleep(2);
        Set_ROCS(8);
        Set_Num_Hits(10);
        Test_Hits_Full(1,0,1,0,1,0,1,0);
        return 0;
      }
      else {
        std::cout << "That is not a valid input. Exiting program." << std::endl;
        return 0;
      }
    }
    else {
      Test_Hits_Resets( test_input );
      return 0;
    }  

    return 0;
  }

  else if (test_num == 3) {
    //Calibration mode
    std::cout << "Would you like to enable or disable the cal trigger?" << std::endl;
    std::cout << "0 = disable, 1 = enable ";
    uint32_t cal_input;
    cin >> cal_input;
    Set_Cal(cal_input);

    std::cout << "Cal Trigger set" << std::endl;
    return 0;
  }

  else if (test_num == 4) {
    //Do some PKAM thing
    std::cout << "How long do you want the PKAM reset?" << std::endl;
    std::cout << "Default is 5" << std::endl;
    std::cout << "A value of n will set the PKAM length to n*256 + 20 clock cycles" << std::endl;
    int PKAM;
    std::cin >> PKAM;
    PKAM_Enable( PKAM );

    std::cout << "PKAM Enabled!" << std::endl;
    return 0;
  } 

  else if (test_num == 5) {
    //Run selective pixel test
    std::cout << "How many ROCS in each channel? (max 8) (0 = default) ";
    int roc_input; 
    std::cin >> roc_input;
    Set_ROCS( roc_input );
    
    std::cout << "How many hits in each ROC? (max 16) (0 = default) ";
    int hit_input;
    std::cin >> hit_input;
    Set_Num_Hits( hit_input );

    std::cout << "What column would you like to start at? (max 52) ";
    int col_start;
    std::cin >> col_start;
    std::cout << "What column would you like to test up to? (max 52) (0 = max) ";
    int col_input;
    std::cin >> col_input;
    std::cout << "What row would you like to start at? (max 80) ";
    int row_start;
    std::cin >> row_start;
    std::cout << "What row would you like to test up to? (max 80) (0 = max) ";
    int row_input;
    std::cin >> row_input;
    std::cout << "What adc value would you like to start at? (max 255) ";
    int adc_start;
    std::cin >> adc_start;
    std::cout << "What adc value would you like to test up to? (max 255) (0 = max) ";
    int adc_input;
    std::cin >> adc_input;
    std::cout << "How many times would you like to loop this test? ";
    int loops_input;
    std::cin >> loops_input;
    Test_Hits_Full(loops_input, col_start, col_input, row_start, row_input, adc_start, adc_input, 0);

    return 0;
  }

  else if (test_num == 6) {
    //Check stack/NTP
    std::cout << "Checking the stack and no token pass" << std::endl;
    sleep(2);
    Set_ROCS(8);
    Set_Num_Hits(4);
    PKAM_Enable(20);
    //Test_Hits_Full(1,0,1,0,1,0,24,1);
    Stack_Test(1,1); 

    std::cout << "Stack/NTP test complete!" << std::endl;
    return 0;   
  }

  //else if (test_num == 7) {
  //  //Check full stack
  //  std::cout << "Checking full stack" << std::endl;
  //  sleep(2);
  //  Set_ROCS(8);
  //  Set_Num_Hits(1);
  //  PKAM_Enable(20);
  //  //Test_Hits_Full(1,0,1,0,1,0,0,2);
  //  Stack_Test(1,2);

  //  std::cout << "Full stack check complete!" << std::endl;
  //  return 0;
  //}

  else if (test_num == 7) {
    //Monitor phases
    std::cout << "Which fiber would you like to monitor? (-1 = current fiber) ";
    int fiber_input;
    cin >> fiber_input;
    std::cout << "How long would you like to monitor the phase for? ";
    int time_input;
    cin >> time_input;
    Test_Phases(fiber_input, time_input);
    
    std::cout << " " << std::endl;
    std::cout << "Phase Monitoring complete" << std::endl;
    return 0;
  }

  else if (test_num >= 8) {
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
  ConnectionManager manager ("file:/settings/GLIB_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card

  uint32_t ROC_Clk = 0x9898;  //Puts a value of 160 clk cycles in channel A & B

  if (roc_input == 0) { }
  else if (roc_input <= 8) {
    inter_num = (roc_input << 4);
    Number_ROCS = inter_num | roc_input;
  }

  std::string index[8] = {"0","1","2","3","4","5","6","7"};

  for (int i = 0; i<8; i++) {
    std::string roc_num = "ROC_Number_";
    roc_num = roc_num + index[i];

    std::string roc_clock = "ROC_CLK_";
    roc_clock = roc_clock + index[i];

    hw.getNode(roc_num).write( Number_ROCS );
    ValWord < uint32_t > mem = hw.getNode ( roc_num ).read();
    hw.dispatch();
    std::cout << roc_num << " = " << std::hex << mem.value() << std::endl;

    hw2.getNode(roc_num).write( Number_ROCS );
    ValWord < uint32_t > mem2 = hw2.getNode ( roc_num ).read();
    hw2.dispatch();
    std::cout << roc_num << " = " << std::hex << mem2.value() << std::endl;

    hw.getNode(roc_clock).write( ROC_Clk );
    mem = hw.getNode ( roc_clock ).read();
    hw.dispatch();
    std::cout << roc_clock << " = " << std::hex << mem.value() << std::endl;

    hw2.getNode(roc_clock).write( ROC_Clk );
    mem2 = hw2.getNode ( roc_clock ).read();
    hw2.dispatch();
    std::cout << roc_clock << " = " << std::hex << mem2.value() << std::endl;	 
  }
}

void Set_Num_Hits(int hit_input)
{
  using namespace uhal;
  ConnectionManager manager ("file:/settings/GLIB_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card

  uint32_t Hits = 0x00000000;
  uint32_t inter_hits[8];

  if (hit_input == 0) { }
  else {
    for (int i = 0; i < 8; i++) {
      inter_hits[i] = hit_input << (4*(7-i));
    }
    Hits = inter_hits[0] | inter_hits[1] | inter_hits[2] | inter_hits[3] | inter_hits[4] | inter_hits[5] | inter_hits[6] | inter_hits[7];
  }

  ValWord < uint32_t > mem,mem2;
  std::string index[8] = {"0","1","2","3","4","5","6","7"};

  for (int i = 0; i < 8; i++) {

    std::string cha_hits = "CHA_Hits_";
    cha_hits = cha_hits + index[i];

    std::string chb_hits = "CHB_Hits_";
    chb_hits = chb_hits + index[i];

    hw.getNode(cha_hits).write( Hits );
    mem = hw.getNode ( cha_hits ).read();
    hw.dispatch();
    std::cout << cha_hits << " = " << std::hex << mem.value() << std::endl;
  	
    hw.getNode(chb_hits).write( Hits );
    mem = hw.getNode ( chb_hits ).read();
    hw.dispatch();
    std::cout << chb_hits << " = " << std::hex << mem.value() << std::endl;

    hw2.getNode(cha_hits).write( Hits );
    mem2 = hw2.getNode ( cha_hits ).read();
    hw2.dispatch();
    std::cout << cha_hits << " = " << std::hex << mem2.value() << std::endl;
  	
    hw2.getNode(chb_hits).write( Hits );
    mem2 = hw2.getNode ( chb_hits ).read();
    hw2.dispatch();
    std::cout << chb_hits << " = " << std::hex << mem2.value() << std::endl;
 
  }

}

int Write_ROCs(int i, int Hit_infoA, int Hit_infoB){
	using namespace std;
	using namespace uhal;
	ConnectionManager manager ("file://test/dummy_connections.xml");
	HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
	HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
	
	string ROCSA[ 8 ] = {"CHA_ROC0", "CHA_ROC1", "CHA_ROC2", "CHA_ROC3", "CHA_ROC4", "CHA_ROC5", "CHA_ROC6", "CHA_ROC7"};
	string ROCSB[ 8 ] = {"CHB_ROC0", "CHB_ROC1", "CHB_ROC2", "CHB_ROC3", "CHB_ROC4", "CHB_ROC5", "CHB_ROC6", "CHB_ROC7"};

	hw.getNode( ROCSA[i] ).write(Hit_infoA);
        ValWord < uint32_t > mem = hw.getNode ( ROCSA[i] ).read();
        hw.dispatch();
        //std::cout << ROCSA[i] << " = " << std::hex << mem.value() << std::endl;

        hw.getNode( ROCSB[i] ).write(Hit_infoB);
        mem = hw.getNode ( ROCSB[i] ).read();
        hw.dispatch();
        //std::cout << ROCSB[i] << " = " << std::hex << mem.value() << std::endl;

        hw2.getNode( ROCSA[i] ).write(Hit_infoA);
        ValWord < uint32_t > mem2 = hw2.getNode ( ROCSA[i] ).read();
        hw2.dispatch();
        //std::cout << ROCSA[i] << " = " << std::hex << mem2.value() << std::endl;

        hw2.getNode( ROCSB[i] ).write(Hit_infoB);
        mem2 = hw2.getNode ( ROCSB[i] ).read();
        hw2.dispatch();
        //std::cout << ROCSB[i] << " = " << std::hex << mem2.value() << std::endl;	
}

void Scan_Pixels() 
{
  using namespace std;
  using namespace uhal;
  ConnectionManager manager ("file://test/dummy_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card

  int j,k,i;
  int Max_Column = 0;
  int Max_Row = 0;
  int Hit_infoA_Start = 0;
  int Hit_infoB_Start = 0;
  //int start = 0;
  //int temp[10] = {0};
  //int* information = temp;
  int row_loops = 0;
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
	Write_ROCs( i, Hit_infoA, Hit_infoB);
      }	

      //information = Test_Hits(information,start);

      //start = 1;
      Hit_infoA = Hit_infoA + 0x00000100;
      Hit_infoB = Hit_infoB + 0x00000100;	

    }

    row_loops++;
    Hit_infoA = Hit_infoA + (0x00010000 * row_loops);
    Hit_infoB = Hit_infoB + (0x00010000 * row_loops);
  }
}

void Scan_ADC(int increment)
{
  using namespace uhal;
  using namespace std;
  ConnectionManager manager ("file://test/dummy_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card

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
      Write_ROCs( j, hit_info, hit_info);
    }   
    
    hit_info = hit_info + (0x00000001*increment);
    std::cout << "Right here" << std::endl;
  }

}

void PKAM_Enable( int PKAM_init ){
  
  using namespace uhal;
  using namespace std;
  ConnectionManager manager ("file:/settings/GLIB_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card

  uint32_t PKAM_Value = 0x05;
  uint32_t PKAM_Constant = 0x14;
  uint32_t PKAM_Enable = 0x30000000;
  uint32_t PKAM = 0x0;

  if(PKAM_init == 0){
  	PKAM = (PKAM_Constant << 24) | (PKAM_Constant << 16) | (PKAM_Value << 8) | (PKAM_Value);
  }
  else{
	PKAM = (PKAM_Constant << 24) | (PKAM_Constant << 16) | (PKAM_init << 8) | (PKAM_init); 
  }
  
  std::string index[8] = {"0","1","2","3","4","5","6","7"};
  std::string temp1 = "PKAM_Reset_";
  std::string temp2 = "PKAM_Enable_";
  std::string PKAM_string;
  std::string Enable_string;
  
  for (int i = 0; i < 8; i++) {

    PKAM_string = temp1 + index[i];
    Enable_string = temp2 + index[i];
	
    hw.getNode(PKAM_string).write( PKAM );
    ValWord < uint32_t > mem = hw.getNode ( PKAM_string ).read();
    hw.dispatch();
    std::cout << "PKAM Reset " << std::dec << i << " was set to " << std::hex << (mem.value() & 0xF) << "*256 + 28 clock cycles" << std::endl;

    hw2.getNode(PKAM_string).write( PKAM );
    ValWord < uint32_t > mem2 = hw2.getNode ( PKAM_string ).read();
    hw2.dispatch();
    std::cout << "PKAM Reset " << std::dec << i << " was set to " << std::hex << (mem2.value() & 0xF) << "*256 + 28 clock cycles" << std::endl;

    hw.getNode(Enable_string).write( PKAM_Enable );
    mem = hw.getNode ( Enable_string ).read();
    hw.dispatch();
    //std::cout << "PKAM Enable = " << std::hex << mem.value() << std::endl;

    hw2.getNode(Enable_string).write( PKAM_Enable );
    mem2 = hw2.getNode ( Enable_string ).read();
    hw2.dispatch();
    //std::cout << "PKAM Enable = " << std::hex << mem2.value() << std::endl;

  }
}

int Set_Cal(uint32_t cal_input) {

  using namespace std;
  using namespace uhal;

  ConnectionManager manager ("file://test/dummy_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card

  ValWord < uint32_t > mem,mem2;

  if (cal_input > 1) {
    std::cout << "Not a valid option. Exiting program." << std::endl;
    return 0;
  }
  else {
 
    cal_input = 0x100 * cal_input;

    hw.getNode("Cal_Trigger").write( cal_input );
    mem = hw.getNode ( "Cal_Trigger" ).read();
    hw.dispatch();
    std::cout << "Cal Trigger set to " << std::hex << (mem.value() & 0xF) << std::endl;

    hw2.getNode("Cal_Trigger").write( cal_input );
    mem2 = hw2.getNode( "Cal_Trigger" ).read();
    hw2.dispatch();
    std::cout << "Cal Trigger set to " << std::hex << (mem.value() & 0xF) << std::endl;
  }

  if (cal_input == 0) {
    return 0;
  }

  std::cout << "Calibration options:" << std::endl;
  std::cout << " " << std::endl;
  std::cout << "  1. Calibrate Mode" << std::endl;
  std::cout << "  2. Sync Mode" << std::endl;
  std::cout << "  3. Reset Mode" << std::endl;
  std::cout << " " << std::endl;
  std::cout << "Which calibration mode would you like to test? ";
  int cal_choice;
  cin >> cal_choice;

  if (cal_choice == 1) {
    //Run Calibration mode test
  }
  else if (cal_choice == 2) {
    //Run Sync mode test
    std::string filename ("settings/HWDescription.xml");
    const char* cHWFile = filename.c_str();
  
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

    //get the board info of all boards and start the acquistion
    //for (auto& cFED : cSystemController.fPixFEDVector)
    //{
      auto& cFED = cSystemController.fPixFEDVector[0];
      cSystemController.fFEDInterface->getBoardInfo(cFED);
      cSystemController.fFEDInterface->findPhases(cFED);
    //}

    auto& cFED2 = cSystemController.fPixFEDVector[1];
    auto& cFED3 = cSystemController.fPixFEDVector[2];

    std::vector<unsigned int> FIFO;
    std::vector<unsigned int> FIFO_CHB;
    std::vector<unsigned int> Event_Number;
    unsigned int Event_one = 0;
    unsigned int Event_two = 0;

    int ROC_Value = 0;
    int Compare_Event = 0;
    int Loops = 1;
    uint32_t Current_Event_Num = 0;
    int Error_Count = 0;
    int Do_Error = 0;
    int Pixel = 0;
    int Pixel_Hit = 0;
    std::fill(FIFO.begin(), FIFO.end(), 0);
    std::fill(FIFO_CHB.begin(), FIFO_CHB.end(), 0);

    for (int l = 0; l < Loops; l++) {
      
      for (int i = 0; i < 8; i++) {
        //cSystemController.fFEDInterface->WriteBoardReg(cFED2, "sync_test" , 1);

	//cSystemController.fFEDInterface->WriteBoardReg(cFED3, "sync_test" , 1);
      }

      cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);   

      sleep(4);
      //cAmc13Controller.fAmc13Interface->BurstL1A();
      sleep(0.001);

      FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
      FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);
 
      for( std::vector<unsigned int>::size_type i = 0; i != FIFO.size(); i++ )
      {

        if (FIFO[i] == 0x0) {
          break;
        }

        if (((FIFO[i] >> 4) & 0xF) == 0x9) {
          if ((FIFO[i] & 0xF) == 0) {
            std::cout << "Sync test succesfull!" << std::endl;
          }
          else {
            std::cout << "Sync test failed" << std::endl;
            std::cout << "Event Num = " << (FIFO[i] & 0xF) << std::endl;
          }
        }

      }

    }

  return 0;

  }
  else if (cal_choice == 3) {
    //Run Reset mode test
  
    std::string filename ("settings/HWDescription.xml");
    const char* cHWFile = filename.c_str();
  
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

    //get the board info of all boards and start the acquistion
    //for (auto& cFED : cSystemController.fPixFEDVector)
    //{
      auto& cFED = cSystemController.fPixFEDVector[0];
      cSystemController.fFEDInterface->getBoardInfo(cFED);
      cSystemController.fFEDInterface->findPhases(cFED);
    //}

    auto& cFED2 = cSystemController.fPixFEDVector[1];
    auto& cFED3 = cSystemController.fPixFEDVector[2];

    std::vector<unsigned int> FIFO;
    std::vector<unsigned int> FIFO_CHB;
    std::vector<unsigned int> Event_Number;
    unsigned int Event_one = 0;
    unsigned int Event_two = 0;

    int ROC_Value = 0;
    int Compare_Event = 0;
    int Loops = 3;
    uint32_t Current_Event_Num = 0;
    int Error_Count = 0;
    int Do_Error = 0;
    int Pixel = 0;
    int Pixel_Hit = 0;
    std::fill(FIFO.begin(), FIFO.end(), 0);
    std::fill(FIFO_CHB.begin(), FIFO_CHB.end(), 0);

    for (int l = 0; l < Loops; l++) {
      
      if (l == 2) {
        //cSystemController.fFEDInterface->WriteBoardReg(cFED2, "reset_test", 1);
        //cSystemController.fFEDInterface->WriteBoardReg(cFED3, "reset_test", 1);
      }

      for (int i = 0; i < 8; i++) {
        //cSystemController.fFEDInterface->WriteBoardReg(cFED2, "sync_test" , 1);

	//cSystemController.fFEDInterface->WriteBoardReg(cFED3, "sync_test" , 1);
      }

      cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);   

      if (l == 0) {
        sleep(4);
        //cAmc13Controller.fAmc13Interface->BurstL1A();
        sleep(0.001);
      }
      else {
        cAmc13Controller.fAmc13Interface->BurstL1A();
        sleep(0.001);
      }

      FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
      FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);
 
      for( std::vector<unsigned int>::size_type i = 0; i != FIFO.size(); i++ )
      {

        if (FIFO[i] == 0x0) {
          break;
        }

        if (((FIFO[i] >> 4) & 0xF) == 0x9) {
          
          std::cout << "Event Number = " << (FIFO[i] & 0xF) << std::endl;

        }

      }

    }

  return 0;
  
  }
  else {
    std::cout << "Not a valid option. Exiting program" << std::endl;
    return 0;
  }


}


void Test_Hits_Full(int loops_input, int col_start, int col_input, int row_start, int row_input, int adc_start, int adc_input, int choice_input)
{

  using namespace std;
  using namespace uhal;

  const char* cHWFile;

  if (choice_input == 0) {
    std::string filename ("settings/HWDescription.xml");
    cHWFile = filename.c_str();
  }
  else if (choice_input == 1) {
    std::string filename ("settings/HWDescription_Stack.xml");
    cHWFile = filename.c_str();
  }
  else if (choice_input == 2) {
    std::string filename ("settings/HWDescription_Stack2.xml");
    cHWFile = filename.c_str();
  }

  string ROCSA[ 8 ] = {"CHA_ROC0_", "CHA_ROC1_", "CHA_ROC2_", "CHA_ROC3_", "CHA_ROC4_", "CHA_ROC5_", "CHA_ROC6_", "CHA_ROC7_"};
  string ROCSB[ 8 ] = {"CHB_ROC0_", "CHB_ROC1_", "CHB_ROC2_", "CHB_ROC3_", "CHB_ROC4_", "CHB_ROC5_", "CHB_ROC6_", "CHB_ROC7_"};

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

  //get the board info of all boards and start the acquistion
  //for (auto& cFED : cSystemController.fPixFEDVector)
  //{
      auto& cFED = cSystemController.fPixFEDVector[0];
      cSystemController.fFEDInterface->getBoardInfo(cFED);
      cSystemController.fFEDInterface->findPhases(cFED);
  //}
 
  auto& cFED2 = cSystemController.fPixFEDVector[1];
  auto& cFED3 = cSystemController.fPixFEDVector[2];

  std::vector<unsigned int> FIFO;
  std::vector<unsigned int> FIFO_CHB;
  std::vector<unsigned int> FIFO1;
  std::vector<unsigned int> FIFO1_marker;
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
  int Loops = loops_input;
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
  std::fill(FIFO1.begin(), FIFO1.end(), 0);
  std::fill(FIFO1_marker.begin(), FIFO1_marker.end(), 0);

  int max_row = row_input; //atoi(argv[3]); //80
  if (max_row > 80 || max_row == 0) {
  	max_row = 80;
  }
  int max_col = col_input; //atoi(argv[4]); //52
  if (max_col > 52 || max_col == 0) {
  	max_col = 52;
  }
  int max_adc = adc_input; //atoi(argv[5]); //255
  if (max_adc > 255 || max_adc == 0) {
  	max_adc = 255;
  }

  int adc_loops = 0;
  int col_loops = 0;
  int row_loops = 0;

  int test_adc = 0;
  int test_col = 0;
  int test_row = 0;

  int num_ROC = -1;

  std::string index[8] = {"0","1","2","3","4","5","6","7"};

  for (int l = 0; l < Loops; l++) 
  {
    if(l%5000 == 0){
      std::cout << "Loop Number = " << std::dec << l << std::endl;
    }     

    int Hit_infoA = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);
    int Hit_infoB = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);

    int row = 0;
    int col = 0;
    int adc = 0;

    for (row = row_start; row < max_row; row++) {

      for (col = col_start; col < max_col; col++) {

      	std::cout << "Starting row " << std::dec << row << ", column " << std::dec << col << std::endl;

        for (adc = adc_start; adc < max_adc; adc+=8) {

          for (int j = 0; j < 8; j++) {

            for (int i = 0; i < 8; i++) {
              //Write_ROCs( i, Hit_infoA, Hit_infoB); 

              ROCSA[j] = ROCSA[j] + index[i];
              ROCSB[j] = ROCSB[j] + index[i];
	
	      cSystemController.fFEDInterface->WriteBoardReg(cFED2, ROCSA[j], Hit_infoA);
	      cSystemController.fFEDInterface->WriteBoardReg(cFED2, ROCSB[j], Hit_infoB);

	      cSystemController.fFEDInterface->WriteBoardReg(cFED3, ROCSA[j], Hit_infoA);
              cSystemController.fFEDInterface->WriteBoardReg(cFED3, ROCSB[j], Hit_infoB);
          
              ROCSA[j] = ROCSA[j].substr(0,9);
              ROCSB[j] = ROCSB[j].substr(0,9);

            }

            Hit_infoA = Hit_infoA + 0x00000001;
            Hit_infoB = Hit_infoB + 0x00000001;

            adc_loops++;
 
          }

	  //for (auto& cFED : cSystemController.fPixFEDVector)
  	  //{
  	    cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);   

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
            FIFO1 = cSystemController.fFEDInterface->readFIFO1_vec(cFED);
            FIFO1_marker = cSystemControllet.fFEDInterface->readFIFO1Marker(cFED);
	  //}

	  int k = 0;
	  int j = 0;
          int h = 0;
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

	    if(((FIFO[i] >> 4) & 0xF) == 0x8){
	      Error_Count = 0;
	      Error_Loop = 0;
	      h++;
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
	    				
	    if(((FIFO[i] >> 4) & 0xF) == 0x9)
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

            //int test_fif = ((FIFO[i] >> 4) & 0xF);
            //std::cout << "FIFO[i] = " << std::hex << test_fif << std::endl;

	    if (((FIFO[i] >> 4) & 0xF) == 0x1 && Next_step_roc == 1) {
              Next_step_roc = 2;
              num_ROC++;
	      continue;
	    }
	    else if (((FIFO[i] >> 4) & 0xF) == 0x1 && Next_step_roc != 1){
	      Incorrect_pix_info++;
	      Error_Count++;
	      Next_step_roc = 2;
	      continue;
	    }

            if (((FIFO[i] >> 4) & 0xF) == 0x2 && Next_step_roc == 2) {
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
	    else if (((FIFO[i] >> 4) & 0xF) == 0x2 && Next_step_roc != 2){
	      Incorrect_pix_info++;
	      Error_Count++;
	      Next_step_roc = 3;
	      continue;
	    }

            if (((FIFO[i] >> 4) & 0xF) == 0x3 && Next_step_roc == 3){
	      Next_step_roc = 4;
	      continue;
	    }
	    else if (((FIFO[i] >> 4) & 0xF) == 0x3 && Next_step_roc == 3){
	      Incorrect_pix_info++;
	      Error_Count++;
	      Next_step_roc = 4;
	      continue;
	    }

	    if (((FIFO[i] >> 4) & 0xF) == 0x4 && Next_step_roc == 4) {
              Current_row_num = ((FIFO[i-1] & 0xF) << 3) | ((FIFO[i] & 0xF) >> 1);
	      test_row = row;
	      //std::cout << "Current_row_num = " << std::dec << Current_row_num << " row = " << std::dec << row << std::endl;
	      if (Current_row_num != test_row){
	        Incorrect_row++;
	        Error_Count++;
	      }
	      Next_step_roc = 5;
	    }
	    else if (((FIFO[i] >> 4) & 0xF) == 0x4 && Next_step_roc != 4){
	      Incorrect_pix_info++;
	      Error_Count++;
	      Next_step_roc = 5;
	      continue;
	    }

	    if (((FIFO[i] >> 4) & 0xF) == 0x5 && Next_step_roc == 5){
	      Next_step_roc = 6;
	      continue;
	    }
	    else if (((FIFO[i] >> 4) & 0xF) == 0x5 && Next_step_roc != 5){
	      Incorrect_pix_info++;
	      Error_Count++;
	      Next_step_roc = 6;
	      continue;
	    }

	    if (((FIFO[i] >> 4) & 0xF) == 0x6 && Next_step_roc == 6) {
              Current_adc_num = (((FIFO[i-2] & 0xF) & 0x1) << 7) | (((FIFO[i-1] & 0xF) >> 1) << 4) | (FIFO[i] & 0xF);
              Current_adc_num = Current_adc_num;
              test_adc = adc_loops - (8 - num_ROC);
	      //std::cout << "Current_adc_num = " << std::dec << Current_adc_num << " adc = " << std::dec << adc << std::endl;
	      //if (Current_adc_num == 36){
	      //  test_adc = 16;
	      //}
	      if (Current_adc_num != test_adc){
	        Incorrect_adc++;
		Error_Count++;
	      }
              Next_step_roc = 1;						 
            }
	    else if (((FIFO[i] >> 4) & 0xF) == 6 && Next_step_roc != 6) {
	      Incorrect_pix_info++;
	      Error_Count++;
              Next_step_roc = 1;
	      continue;
            }

	    if(((FIFO[i] >> 4) & 0xF) == 0x8 && Next_step == 0){ 
	      Next_step = 1;
	      Start_Event = i;
	      Do_Stack = 0;
	      continue;
            }
	    else if(((FIFO[i] >> 4) & 0xF) == 0x8 && Next_step != 0){
	      Incorrect_Header++;
	      Error_Count++;
	      continue;
	    }
						
	    if(((FIFO[i] >> 4) & 0xF) == 0x9 && Next_step == 1){
	      Next_step = 2;
	      continue;
	    }
	    else if(((FIFO[i] >> 4) & 0xF) == 0x9 && Next_step != 1){
              Incorrect_Header++;
	      Error_Count++;
	      continue;
            }
						
	    if(((FIFO[i] >> 4) & 0xF) == 0xA && Next_step == 2){
              Next_step = 3;
	      continue;
	    }
	    else if(((FIFO[i] >> 4) & 0xF) == 0xA && Next_step != 2){
              Incorrect_Header++;
	      Error_Count++;
	      continue;
	    }
						
	    if(((FIFO[i] >> 4) & 0xF) == 0xB && Next_step == 3){
	      Next_step = 4;
	      continue;
	    }
	    else if(((FIFO[i] >> 4) & 0xF) == 0xB && Next_step != 3){
	      Incorrect_Header++;
	      Error_Count++;
	      continue;
            }
             
            if(((FIFO[i] >> 4) & 0xF) == 0xC && Next_step == 4){
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
	    else if(((FIFO[i] >> 4) & 0xF) == 0xC && Next_step != 4){
	      Incorrect_Trailer++;
	      Error_Count++;
	      continue;
            }
						
	    if(((FIFO[i] >> 4) & 0xF) == 0xD && Next_step == 5){
	      Next_step = 6;
              if ((FIFO[i] & 0x1) == 0x1) {
                std::cout << "Stack is full!" << std::endl;
              }
	      continue;
	    }
	    else if(((FIFO[i] >> 4) & 0xF) == 0xD && Next_step != 5){
	      Incorrect_Trailer++;
	      Error_Count++;
	      continue;
	    }

	    if(((FIFO[i] >> 4) & 0xF) == 0xE && Next_step == 6){
	      Next_step = 7;
	      //std::cout << FIFO[i] << std::endl;
					
	      if( ((FIFO[i] & 0x4) >> 2) == 0x1 ){
	        std::cout << "There was a PKAM Reset at Event " << std::dec << Current_Event_Num << std::endl;
	      }
              num_ROC = -1;

	      Stack[0] = (FIFO[i] & 0x3);
	      continue;
            }
	    else if(((FIFO[i] >> 4) & 0xF) == 0xE && Next_step != 6){
	      Incorrect_Trailer++;
              Error_Count++;
              num_ROC = -1;
              continue;
	    }

            if(((FIFO[i] >> 4) & 0xF) == 0xF && Next_step == 7){
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
             else if(((FIFO[i] >> 4) & 0xF) == 0xF && ((FIFO[i - 1] >> 4)) & 0xF != 0xF && Next_step != 7){
	       Incorrect_Trailer++;
	       Error_Count++;
	       END_Event = i;
	       Error_Loop = 1;
	     }
						
	     if(Stack[1] != 0x0 && Do_Stack == 1){
	       std::cout << "The Stack count is " << std::dec << Stack[1] << std::endl;
	     }

	     if(Error_Count != 0 && Error_Loop == 1 && choice_input == 0){
	       std::cout << "Event = " << std::dec << Current_Event_Num << " and Loop = " << std::dec << l << std::endl; 
	       std::cout << "Total Number of Errors = " <<std::dec << Error_Count << std::endl;
	       int Error = 0;

               //for(Error = Start_Event; Error <= END_Event; Error++){
	       //  std::cout << "FIFO[" << std::dec << Error << "] = " << std::hex << FIFO[Error] << std::endl;
	       //}

	       //for(Error = Start_Event; Error <= END_Event; Error++){
	       //  std::cout << "FIFO_CHB[" << std::dec << Error << "] = " << std::hex << FIFO_CHB[Error] << std::endl;
               //}
	       Error_Loop = 0;
	     }

	     if(((FIFO[i] >> 4) & 0xF) == 0x6){
	       Pixel++;
	     }
	     else if(((FIFO[i] >> 4) & 0xF) == 0x7){
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
   
	  //Hit_infoA = Hit_infoA + 0x00000008;
          //Hit_infoB = Hit_infoB + 0x00000008;

          //adc_loops++;
             
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

  if (choice_input == 0) {
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
  }
  else if (choice_input > 0) {
    std::cout << "There have been " << std::dec << Total_Events << " Events" << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Event_Num << " Incorrect Event Numbers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Header << " Incorrect TBM Headers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Trailer << " Incorrect TBM Trailers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_ROC << " incorrect Number of ROCs " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Pixel << " incorrect Number of Pixels " << std::endl;
  }
  //exit(0);

}

int Test_Hits_Resets( int test_input )
{

  using namespace std;

  const char* cHWFile;

  if ( test_input == 0) {
    //std::cout << "Testing normal event." << std::endl;
    //sleep(2);
    std::string filename ("settings/HWDescription_PKAM.xml");
    cHWFile = filename.c_str();
  }
  else if ( test_input == 1) {
    //std::cout << "Testing TBM Reset." << std::endl;
    //sleep(2);
    std::string filename ("settings/HWDescription_TBM.xml");
    cHWFile = filename.c_str();
  }
  else if ( test_input == 2) {
    //std::cout << "Testing ROC Reset." << std::endl;
    //sleep(2);
    std::string filename ("settings/HWDescription_ROC.xml");
    cHWFile = filename.c_str();
  }
  else if ( test_input >= 3) {
    std::cout << "Not a valid option. Exiting program" << std::endl;
    return 0;
  }

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

    std::cout << "Configure OK" << std::endl;

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
    for (auto& cFED : cSystemController.fPixFEDVector)
    {
      /*for (auto& cFitel : cFED->fFitelVector)
        {
            //cSystemController.fFEDInterface->ReadLightOnFibre(cFitel);
            cSystemController.fFEDInterface->ReadADC(cFitel, cChannelOfInterest, true);
	    }*/

	//auto& cFED = cSystemController.fPixFEDVector[0];
	cSystemController.fFEDInterface->getBoardInfo(cFED);
        cSystemController.fFEDInterface->findPhases(cFED);
	//cSystemController.fFEDInterface->Start (cFED);
    }
	
    std::vector<unsigned int> FIFO;
    std::vector<unsigned int> FIFO_CHB;
    std::vector<unsigned int> TransparentFIFO;
    std::vector<unsigned int> Event_Number;
    
    unsigned int Event_one = 0;
    unsigned int Event_two = 0;
    int Total_Events = 0;
    int Incorrect_Header = 0;
    int Incorrect_Trailer = 0;
    int Incorrect_Event_Num = 0;
    int Incorrect_ROC = 0;
    int ROC_Value = 0;
    int Loops = 1;
    uint32_t Current_Event_Num = 0;
    int Error_Count = 0;
    int Do_Error = 0;
    int Pixel = 0;
    int Pixel_Hit = 0;
    int Incorrect_Pixel = 0;
    int Compare_Event = 0;
    
    std::fill(FIFO.begin(), FIFO.end(), 0);
    std::fill(FIFO_CHB.begin(), FIFO_CHB.end(), 0);


    for (int l = 0; l < Loops; l++)
    {
	if(l%5000 == 0){
		std::cout << "Loop Number = " << std::dec << l << std::endl;
	}     
 
        for (auto& cFED : cSystemController.fPixFEDVector)
        {

            cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);

	    if(l == 0){
		sleep(4);
		cAmc13Controller.fAmc13Interface->BurstL1A();
                cAmc13Controller.fAmc13Interface->DisableBGO(0);
		sleep(0.001);
	    }
	    else{
		cAmc13Controller.fAmc13Interface->BurstL1A();
		cAmc13Controller.fAmc13Interface->DisableBGO(0);
                sleep(0.001);
	    }

            //cSystemController.fFEDInterface->readTransparentFIFO(cFED);
            FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
            FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);
            cSystemController.fFEDInterface->readFIFO1(cFED);
            //cSystemController.fFEDInterface->readOSDWord(cFED, cROCOfInterest, cChannelOfInterest);
     	    //cSystemController.fFEDInterface->readErrorFIFO(cFED, true);
	    //cSystemController.fFEDInterface->readTTSState(cFED);
            //cSystemController.fFEDInterface->ReadData(cFED, 0 );

	    /*for( std::vector<unsigned int>::size_type n = 0; n != FIFO.size(); n++){ 
		if( FIFO[n] != 0 )
			std::cout << "FIFO[" << std::dec << n << "] = " << std::hex << FIFO[n] << std::endl;
	      }*/

	    int k = 0;
	    int j = 0;
	    int h = 0;
	    int g = 0;
	    int Next_step = 0;
	    int Num_ROC = 0;
	    int END_Event = 0;
	    int Error_Loop = 0;
	    int Start_Event = 0;
	    int Stack[2];
	    int Do_Stack = 0;

	    for( std::vector<unsigned int>::size_type i = 0; i != FIFO.size(); i++ )
	    {    

		if(FIFO[i] == 0x00000000){
			break;
		}

		if(((FIFO[i] >> 4) & 0xF) == 0x8){
			Error_Count = 0;
			Error_Loop = 0;
		}
		
		if(((FIFO[i] >> 4) & 0xF) == 0x9)
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

		if(((FIFO[i] >> 4) & 0xF) == 0x8 && Next_step == 0){ 
		  	Next_step = 1;
			Start_Event = i;
			Do_Stack = 0;
                        continue;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0x8 && Next_step != 0){
			Incorrect_Header++;
			Error_Count++;
			continue;
		}
		
		if(((FIFO[i] >> 4) & 0xF) == 0x9 && Next_step == 1){
		  	Next_step = 2;
			continue;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0x9 && Next_step != 1){
			Incorrect_Header++;
			Error_Count++;
			continue;
		}
		
		if(((FIFO[i] >> 4) & 0xF) == 0xA && Next_step == 2){
		  	Next_step = 3;
			continue;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0xA && Next_step != 2){
			Incorrect_Header++;
			Error_Count++;
			continue;
		}
		
		if(((FIFO[i] >> 4) & 0xF) == 0xB && Next_step == 3){
		  	Next_step = 4;
			continue;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0xB && Next_step != 3){
			Incorrect_Header++;
			Error_Count++;
			continue;
		}
		
		if(((FIFO[i] >> 4) & 0xF) == 0xC && Next_step == 4){
		  	Next_step = 5;
			Num_ROC = 0;

			if(FIFO[i] & 0x8){
				cout << "There was a NTP at event " << dec << Current_Event_Num << endl;
			}

			if(FIFO[i] & 0x4){
                                cout << "There was a TBM Reset at event " << dec << Current_Event_Num << endl;
                        }
			if(FIFO[i] & 0x2){
                                cout << "There was a ROC Reset at event " << dec << Current_Event_Num << endl;
                        }

			continue;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0xC && Next_step != 4){
			Incorrect_Trailer++;
			Error_Count++;
			continue;
		}
		
		if(((FIFO[i] >> 4) & 0xF) == 0xD && Next_step == 5){
		  	Next_step = 6;
			continue;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0xD && Next_step != 5){
			Incorrect_Trailer++;
			Error_Count++;
			continue;
		}

		if(((FIFO[i] >> 4) & 0xF) == 0xE && Next_step == 6){
		  	Next_step = 7;
	
			if( ((FIFO[i] & 0x4) >> 2) == 0x1 ){
			  std::cout << "There was a PKAM Reset at Event " << dec << Current_Event_Num << std::endl;
			}

			Stack[0] = (FIFO[i] & 0x3);
			continue;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0xE && Next_step != 6){
			Incorrect_Trailer++;
			Error_Count++;
			continue;
		}
		
		if(((FIFO[i] >> 4) & 0xF) == 0xF && Next_step == 7){
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
		else if(((FIFO[i] >> 4) & 0xF) == 0xF && (FIFO[i - 1] >> 4) != 0xF && Next_step != 7){
			Incorrect_Trailer++;
                        Error_Count++;
			END_Event = i;
			Error_Loop = 1;
		}
		
		if(Stack[1] != 0x0 && Do_Stack == 1){
			cout << "The Stack count is " << dec << Stack[1] << endl;
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

		if(((FIFO[i] >> 4) & 0xF) == 0x6){
			Pixel++;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0x7){
			Num_ROC++;
			ROC_Value = 0;
		}
	
		if(Num_ROC == 8){
			ROC_Value = 1;
		}
		else{
			ROC_Value = 0;
		}

	      }

        }

    }

    std::cout << "There have been " << std::dec << Total_Events << " Events" << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Event_Num << " Incorrect Event Numbers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Header << " Incorrect TBM Headers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Trailer << " Incorrect TBM Trailers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_ROC << " incorrect Number of ROCs " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Pixel << " incorrect Number of Pixels " << std::endl;
 
   // exit(0);
    return 0;
}

void Test_Phases (int fiber_input, int time_input)
{

  const char* cHWFile;
  std::string filename ("settings/HWDescription.xml");
  cHWFile = filename.c_str();

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

  auto cSetting = cSystemController.fSettingsMap.find("NAcq");
  int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
  cSetting = cSystemController.fSettingsMap.find("BlockSize");
  int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;

  int cChannelOfInterest;
  if (fiber_input < 0) {
    cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
    cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
  }
  else {
    cChannelOfInterest = fiber_input;
  }

  cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
  int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;

  // get the board info of all boards and start the acquistion
  for (auto& cFED : cSystemController.fPixFEDVector)
  {
    cSystemController.fFEDInterface->getBoardInfo(cFED);
    /*for (auto& cFitel : cFED->fFitelVector)
    {
      cSystemController.fFEDInterface->ReadADC(cFitel, cChannelOfInterest, true);
    }*/
    cSystemController.fFEDInterface->getBoardInfo(cFED);
    cSystemController.fFEDInterface->findPhases2(cFED, cChannelOfInterest);
  }

  std::cout << "Monitoring Phases for selected Channel of Interest for " << time_input << " seconds ... " << std::endl << std::endl;
  std::cout << BOLDGREEN << "FIBRE CTRL_RDY CNTVAL_Hi CNTVAL_Lo   pattern:                     S H1 L1 H0 L0   W R" << RESET << std::endl;
  for(int i = 0; i < time_input/10; i++)
  {
    for (auto& cFED : cSystemController.fPixFEDVector)
    {
      cSystemController.fFEDInterface->monitorPhases(cFED, cChannelOfInterest);

    }
  }

}

void Stack_Test (int loops_input, int choice_input)
{

  using namespace std;
  using namespace uhal;

  const char* cHWFile;

  if (choice_input == 0) {
    std::string filename ("settings/HWDescription.xml");
    cHWFile = filename.c_str();
  }
  else if (choice_input == 1) {
    std::string filename ("settings/HWDescription_Stack.xml");
    cHWFile = filename.c_str();
  }
  else if (choice_input == 2) {
    std::string filename ("settings/HWDescription_Stack2.xml");
    cHWFile = filename.c_str();
  }

  string ROCSA[ 8 ] = {"CHA_ROC0_", "CHA_ROC1_", "CHA_ROC2_", "CHA_ROC3_", "CHA_ROC4_", "CHA_ROC5_", "CHA_ROC6_", "CHA_ROC7_"};
  string ROCSB[ 8 ] = {"CHB_ROC0_", "CHB_ROC1_", "CHB_ROC2_", "CHB_ROC3_", "CHB_ROC4_", "CHB_ROC5_", "CHB_ROC6_", "CHB_ROC7_"};

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

  //get the board info of all boards and start the acquistion
  //for (auto& cFED : cSystemController.fPixFEDVector)
  //{
      auto& cFED = cSystemController.fPixFEDVector[0];
      cSystemController.fFEDInterface->getBoardInfo(cFED);
      cSystemController.fFEDInterface->findPhases(cFED);
  //}
 
  auto& cFED2 = cSystemController.fPixFEDVector[1];
  auto& cFED3 = cSystemController.fPixFEDVector[2];

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
  int Loops = loops_input;
  uint32_t Current_Event_Num = 0;
  uint32_t Current_col_num = 0;
  uint32_t Current_row_num = 0;
  uint32_t Current_adc_num = 0;
  int Error_Count = 0;
  int Do_Error = 0;
  int Pixel = 0;
  int Pixel_Hit = 0;
  int Incorrect_Pixel = 0;
  sleep(.0001);
  std::fill(FIFO.begin(), FIFO.end(), 0);
  std::fill(FIFO_CHB.begin(), FIFO_CHB.end(), 0);

  int test_adc = 0;
  int test_col = 0;
  int test_row = 0;

  int num_ROC = -1;

  int max_stack = 0;

  std::string index[8] = {"0","1","2","3","4","5","6","7"};

  for (int l = 0; l < Loops; l++) 
  {
    if(l%5000 == 0){
      std::cout << "Loop Number = " << std::dec << l << std::endl;
    }         

    //for (auto& cFED : cSystemController.fPixFEDVector)
    //{
      cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);   

      if(l == 0){
        sleep(4);
	cAmc13Controller.fAmc13Interface->BurstL1A();
	sleep(0.001);
      }
      else{
        cAmc13Controller.fAmc13Interface->BurstL1A();
	sleep(0.001);
      }

      //sleep(.001);
      FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
      FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);
    //}

    int k = 0;
    int j = 0;
    int h = 0;
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

      //std::cout << "FIFO[" << std::dec << i << "] = " << std::hex << (FIFO[i] & 0xFF) << std::endl;

      if(((FIFO[i] >> 4) & 0xF) == 0x8){
        Error_Count = 0;
	Error_Loop = 0;
	h++;
        if( h >= 2 ){
	  k = 1;
	}
      }

      if(((FIFO[i] >> 4) & 0xF) == 0x9)
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

      if(((FIFO[i] >> 4) & 0xF) == 0x8 && Next_step == 0){ 
        Next_step = 1;
	Start_Event = i;
	Do_Stack = 0;
	continue;
      }
      else if(((FIFO[i] >> 4) & 0xF) == 0x8 && Next_step != 0){
	Incorrect_Header++;
	Error_Count++;
	continue;
      }
						
      if(((FIFO[i] >> 4) & 0xF) == 0x9 && Next_step == 1){
	Next_step = 2;
	continue;
      }
      else if(((FIFO[i] >> 4) & 0xF) == 0x9 && Next_step != 1){
        Incorrect_Header++;
	Error_Count++;
	continue;
      }
						
      if(((FIFO[i] >> 4) & 0xF) == 0xA && Next_step == 2){
        Next_step = 3;
	continue;
      }
      else if(((FIFO[i] >> 4) & 0xF) == 0xA && Next_step != 2){
        Incorrect_Header++;
	Error_Count++;
	continue;
      }
						
      if(((FIFO[i] >> 4) & 0xF) == 0xB && Next_step == 3){
	Next_step = 4;
	continue;
      }
      else if(((FIFO[i] >> 4) & 0xF) == 0xB && Next_step != 3){
	Incorrect_Header++;
	Error_Count++;
	continue;
      }
         
      if(((FIFO[i] >> 4) & 0xF) == 0xC && Next_step == 4){
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
      else if(((FIFO[i] >> 4) & 0xF) == 0xC && Next_step != 4){
        Incorrect_Trailer++;
	Error_Count++;
	continue;
      }
	    				
      if(((FIFO[i] >> 4) & 0xF) == 0xD && Next_step == 5){
	Next_step = 6;
        if (FIFO[i] & 0x1) {
          std::cout << "Stack is full!" << std::endl;
        }
	continue;
      }
      else if(((FIFO[i] >> 4) & 0xF) == 0xD && Next_step != 5){
	Incorrect_Trailer++;
	Error_Count++;
	continue;
      }

      if(((FIFO[i] >> 4) & 0xF) == 0xE && Next_step == 6){
	Next_step = 7;
	//std::cout << FIFO[i] << std::endl;
	  			
	if( ((FIFO[i] & 0x4) >> 2) == 0x1 ){
	  std::cout << "There was a PKAM Reset at Event " << std::dec << Current_Event_Num << std::endl;
	}
        num_ROC = -1;

	Stack[0] = (FIFO[i] & 0x3);
	continue;
      }
      else if(((FIFO[i] >> 4) & 0xF) == 0xE && Next_step != 6){
	Incorrect_Trailer++;
        Error_Count++;
        num_ROC = -1;
        continue;
      }

      if(((FIFO[i] >> 4) & 0xF) == 0xF && Next_step == 7){
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
      else if(((FIFO[i] >> 4) & 0xF) == 0xF && ((FIFO[i - 1] >> 4)) & 0xF != 0xF && Next_step != 7){
        Incorrect_Trailer++;
	Error_Count++;
	END_Event = i;
	Error_Loop = 1;
      }
	    				
      if(Stack[1] != 0x0 && Do_Stack == 1){
        std::cout << "The Stack count is " << std::dec << Stack[1] << std::endl;
        if (Stack[1] > max_stack) {
          max_stack = Stack[1];
        }
      }

      if(Error_Count != 0 && Error_Loop == 1 && choice_input == 0){
        std::cout << "Event = " << std::dec << Current_Event_Num << " and Loop = " << std::dec << l << std::endl; 
	std::cout << "Total Number of Errors = " <<std::dec << Error_Count << std::endl;
	int Error = 0;

        //for(Error = Start_Event; Error <= END_Event; Error++){
	//  std::cout << "FIFO[" << std::dec << Error << "] = " << std::hex << FIFO[Error] << std::endl;
	//}

	//for(Error = Start_Event; Error <= END_Event; Error++){
	//  std::cout << "FIFO_CHB[" << std::dec << Error << "] = " << std::hex << FIFO_CHB[Error] << std::endl;
        //}
	Error_Loop = 0;
      }

      if(((FIFO[i] >> 4) & 0xF) == 0x6){
        Pixel++;
      }
      else if(((FIFO[i] >> 4) & 0xF) == 0x7){
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
      			
  } // Ends Loops loop

  std::cout << "There have been " << std::dec << Total_Events << " Events" << std::endl;
  //std::cout << "There have been " << std::dec << Incorrect_Event_Num << " Incorrect Event Numbers " << std::endl;
  //std::cout << "There have been " << std::dec << Incorrect_Header << " Incorrect TBM Headers " << std::endl;
  //std::cout << "There have been " << std::dec << Incorrect_Trailer << " Incorrect TBM Trailers " << std::endl;
  //std::cout << "There have been " << std::dec << Incorrect_ROC << " incorrect Number of ROCs " << std::endl;
  //std::cout << "There have been " << std::dec << Incorrect_Pixel << " incorrect Number of Pixels " << std::endl;

  std::cout << "Max stack was " << std::dec << max_stack << std::endl;

  //exit(0);

}
