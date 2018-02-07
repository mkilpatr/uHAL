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

int main()
{

  using namespace std;
  using namespace uhal;
 
  //ConnectionManager manager ("file://test/dummy_connections.xml");
  //HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
  //HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card

  cout << "    ******************************************    " << endl;
  cout << "    *                                        *    " << endl;
  cout << "    *           FED Testing Program          *    " << endl;
  cout << "    *                  (FTP)                 *    " << endl;
  cout << "    *                                        *    " << endl;
  cout << "    ******************************************    " << endl;
  cout << " " << endl;
  cout << "Testing Options:" << endl;
  cout << "  0. Run all tests" << endl;
  cout << "  1. Run pixel alive test" << endl;
  cout << "  2. Check resets" << endl;
  cout << "  3. Calibration mode" << endl;
  cout << "  4. Enable PKAM Reset" << endl;
  cout << "  5. Run selective pixel tests" << endl;
  cout << "  6. Monitor phases" << endl;
  cout << "  7. Check stack and no token pass (NTP)" << endl;
  cout << "  8. Pixel Alive All channels and plots" << endl;
  cout << "  9. Test TBM/ROC/PKAM resets on FIFO1" << endl;
  cout << " 10. Test TBM/ROC/PKAM resets on DDR" << endl;
  cout << " 11. Test DDR Timeouts" << endl;
  cout << " 12. Test DDR Timeouts with Marker Errors" << endl;
  cout << " 13. Test DDR Pixel Alive" << endl;
  cout << " 14. Test TBM/ROC/PKAM resets on Error FIFO" << endl;
  cout << " 15. Test Timeouts on Error FIFO" << endl;
  cout << " 16. Test Timeouts with Marker errors on Error FIFO" << endl;
  cout << " 17. Check all resets in program" << endl;
  cout << " 18. Variable Marker Errors in different channels" << endl;
  cout << " 19. Random Bit flip generator" << endl;
  cout << " 20. Random ROC Hits generator" << endl;
  cout << " 21. TTC History test" << endl;
  cout << " 22. Memory Stress Test" << endl;
  cout << " 23. Random Bit Flip Test" << endl;
  cout << " 24. Scan all possible Header data values" << endl;
  cout << " 25. Write ROCS with pixel hits value" << endl;
  cout << " 26. Writing registers of the SRAM" << endl;
  cout << " 27. Reading from SRAM" << endl;
  cout << " 28. TTC Reset" << endl;
  cout << " 29. Clear SRAM FIFOs" << endl;
  cout << " 30. Test the BX id of the FED" << endl;
  cout << " 31. Run SRAM Readout Events" << endl;
  cout << " 32. Trailer Timeout valid test" << endl;
  cout << " 33. OOS Multi-Channel Readout" << endl;
  cout << " 34. FEROL" << endl;
  cout << " 35. Send Very Large events" << endl;
  cout << " 36. Read GLIB TTS State" << endl;
  cout << " 37. Write RDY state" << endl;
  cout << " 38. Write BSY state" << endl;
  cout << " 40. Event Error Dump" << endl;
  cout << " 41. Event Timeout Dump" << endl;
  cout << " 42. Event Resync Ahead Dump" << endl;
  cout << " 43. TBM Header ID Error Dump" << endl;
  cout << " 44. TBM Trailer ID Error Dump" << endl;
  cout << " 45. OOS Timeout Error Dump" << endl;
  cout << " 46. Set Delay time" << endl;
  cout << " 47. Set Wrong Marker" << endl;
  cout << " 48. Ferol Test with Errors" << endl;
  cout << " 49. SEU Test" << endl;
  cout << " 50. Random SEU Test" << endl;
  cout << " 51. Test readout of L1 module emulation" << endl;
  cout << " 52. Ferol Test with PKAM Scanning" << endl;
  cout << " 53. Test the PKAM counters and the NTP counters" << endl;
  cout << " 54. Test the OVF counters" << endl;
  cout << " 55. FEROL Data Plots" << endl;
  cout << " 56. Private Resync Tests" << endl;
  cout << " 57. Test Marker Error While Idle" << endl;
  cout << " 58. Test Marker Error While Idle Scan" << endl;
  cout << " 59. Test Stack Count in TO Word" << endl;
  cout << " 60. Creat FED Summary Functions" << endl;
  cout << " 61. Ferol Test with SLINK Scanning" << endl;
  //cout << " 57. Test GLIB Config" << endl;
  cout << " " << endl;
  cout << "Which test would you like to run? ";

  unsigned int test_num;
  unsigned int start = 0;
  cin >> test_num;

  while(cin.fail() || test_num >= 62){
	cout << "Not a valid Entry" << endl;
	cout << "Must be an integer between 0 and 55..." << endl;
	cin.clear();
	cin.ignore(256, '\n');
	cout << "Which test would you like to run? ";
	cin >> test_num;
  }

  if(test_num == 0){
    cout << "Which Test do you want to start at?" << endl;
    cin >> start;
  
    while(cin.fail() || start >= 62){
      cout << "Not a valid Entry" << endl;
      cout << "Must be an integer between 0 and 55..." << endl;
      cin.clear();
      cin.ignore(256, '\n');
      cout << "Which test would you like to run? ";
      cin >> test_num;
    }
  }

  cout << " " << endl;
  int current_test = 0;
  cout << "Initializing GLIB..." << endl;
  int GLIB1_version = 0; //version 2 is 2 ROCS
  int GLIB2_version = 0; //version 1 is 4 ROCS
  int GLIB3_version = 0; //version 0 is 8 ROCS
  TBMVersion(GLIB1_version, GLIB2_version, GLIB3_version);
  cout << "Write TBM version" << endl;
  usleep(10);
  if(GLIB1_version == 0 && GLIB2_version == 0 && GLIB3_version == 0) Set_ROCS(8);
  else{
          if     (GLIB1_version == 0) Set_ROCS(1, 8);
          else if(GLIB1_version == 1) Set_ROCS(1, 4);
          else if(GLIB1_version == 2) Set_ROCS(1, 2);
          if     (GLIB2_version == 0) Set_ROCS(2, 8);
          else if(GLIB2_version == 1) Set_ROCS(2, 4);
          else if(GLIB2_version == 2) Set_ROCS(2, 2);
          if     (GLIB3_version == 0) Set_ROCS(3, 8);
          else if(GLIB3_version == 1) Set_ROCS(3, 4);
          else if(GLIB3_version == 2) Set_ROCS(3, 2);
  }
  PKAM_Enable(1, 20);
  Write_Data_Header(0);
  Marker_Error( 0x55555555, 0, 0x7F8, 1);
  Marker_Error_Idle( 0x55555555, 0, 0x7F8, 1, 1);
  Delay_Time(1);
  writeCSRMode(0x0);
  SRAM_Disable();
  SEU(0, 1, 1, 0);
  usleep(10);

try{

  if (test_num == 1) {
    current_test = 1;
    //Run the pixel alive test for Spy FIFO
    cout << "Starting Pixel Alive" << endl;
    cout << "Setting 1 hit per ROC" << endl;
    int hit_input = 1;
    Set_Num_Hits( hit_input );
    
    cout << "Preparing to test each pixel!" << endl;
    sleep(2);
    Test_Hits_Full(1,0,0,0,0,0,0,0);

    cout << "Pixel alive test complete!" << endl;

  }  

  if (test_num == 2) {
    //Check resets
    current_test = 2;
    cout << "Starting TBM and ROC reset in Spy FIFO" << endl;
    sleep(2);
    Set_Num_Hits( 0 );    
    Test_Hits_Resets( 1, 2 );
    sleep(2);
    
    cout << "How long do you want the PKAM reset?" << endl;
    cout << "Default is 5" << endl;
    cout << "A value of n will set the PKAM length to n*256 + 20 clock cycles" << endl;
    int PKAM;
    if(test_num != 0) cin >> PKAM;
    else PKAM = 1;
    PKAM_Enable( PKAM, 20 );
    cout << "Starting PKAM Reset" << endl;
    sleep(2);
    Set_Num_Hits(10);
    Test_Hits_Resets( 0, 1 );
    sleep(2);
  }

  if (test_num == 3) {
    //Calibration mode
    current_test = 3;
    cout << "Would you like to enable or disable the cal trigger?" << endl;
    cout << "0 = disable, 1 = enable ";
    uint32_t cal_input;
    cin >> cal_input;
    Set_Cal(cal_input);

    cout << "Cal Trigger set" << endl;
    return 0;
  }

  if (test_num == 4) {
    //Enable PKAM timeout
    current_test = 4;
	cout << "What PKAM Value do you want?" << endl;
    int PKAM_int;
    cin >> PKAM_int;
    PKAM_Enable(PKAM_int/256, PKAM_int % 256);

    cout << "PKAM Enabled!" << endl;
    return 0;
  } 

  if (test_num == 5) {
    //Run selective pixel test
    current_test = 5;
    cout << "How many hits in each ROC? (max 16) (0 = default) ";
    int hit_input;
    cin >> hit_input;
    Set_Num_Hits( hit_input );

    cout << "What column would you like to start at? (max 52) ";
    int col_start;
    cin >> col_start;
    cout << "What column would you like to test up to? (max 52) (0 = max) ";
    int col_input;
    cin >> col_input;
    cout << "What row would you like to start at? (max 80) ";
    int row_start;
    cin >> row_start;
    cout << "What row would you like to test up to? (max 80) (0 = max) ";
    int row_input;
    cin >> row_input;
    cout << "What adc value would you like to start at? (max 255) ";
    int adc_start;
    cin >> adc_start;
    cout << "What adc value would you like to test up to? (max 255) (0 = max) ";
    int adc_input;
    cin >> adc_input;
    cout << "How many times would you like to loop this test? ";
    int loops_input;
    cin >> loops_input;
    Test_Hits_Full(loops_input, col_start, col_input, row_start, row_input, adc_start, adc_input, 0);

    return 0;
  }

  if (test_num == 6 || (test_num == 0 && start <= 6)) {
    //Monitor phases
    current_test = 6;
    start = 7;
    if(test_num != 0){
        cout << "Which fiber would you like to monitor? (-1 = current fiber) " << endl;
   	    int fiber_input;
    	cin >> fiber_input;
    	cout << "How long would you like to monitor the phase for? " << endl;
    	int time_input;
    	cin >> time_input;
    	Test_Phases(fiber_input, time_input, test_num);
    
    	cout << " " << endl;
    	cout << "Phase Monitoring complete" << endl;
    }
    else{
        	int time_input = 10;
        	Test_Phases(0, time_input, test_num);
    }
    sleep(2);
    cout << " " << endl;
    cout << "Phase Monitoring complete" << endl;
  }

  if (test_num == 7 || (test_num == 0 && start == 7)) {
    //Check stack/NTP
    current_test = 7;
    start = 8;
    cout << "Checking the stack and no token pass" << endl;
    sleep(2);
    Set_Num_Hits(1);
    PKAM_Disable();
    Stack_Test(1,1); 
    sleep(2);

    cout << "Stack/NTP test complete!" << endl;
  }

  if (test_num == 8 || (test_num == 0 && start == 8)) {
    current_test = 8;
    start = 12;
    int Num_Hits = 0x1;
    Set_Num_Hits(Num_Hits);
    //Set_Num_Hits( 0x04050405, 0x05050505, 0x05060506, 0x06060606, 0x06070607, 0x07070707, 0x00005555);
    //Set_Num_Hits( 0x03040506, 0x04040506, 0x04050607, 0x05050607, 0x05060708, 0x06060708, 0x55550000);
    cout << "Do you want to run SRAM mode?" << endl;
    int SRAM;
    if(test_num != 0) cin >> SRAM;
    else SRAM = 0;
   
    cout << "Do you want to change PKAM value?" << endl;
    int PKAM_change;
    if(test_num != 0) cin >> PKAM_change;
    else PKAM_change = 0;
    if(PKAM_change){
	    cout << "What PKAM Value do you want?" << endl;
        int PKAM_int;
        cin >> PKAM_int;
        int PKAM_const = PKAM_int % 256;
        PKAM_Enable(PKAM_int/256, PKAM_const);
    }

    if(SRAM == 1) SRAM_Mode();
    else SRAM_Disable();
    ADCAsEvn(1);
    Test_Hits_Full_FIFO1(1, 0, 0, 0, 0, 0, 1, 0, Num_Hits, SRAM);
    sleep(2);
  }

  if (test_num == 9){
	current_test = 9;
    Set_Num_Hits(0);
	cout << "Test TBM and ROC reset in FIFO1" << endl;
	sleep(1);
	Test_Resets_FIFO1(2, 1);
    sleep(2);

	cout << "How long do you want the PKAM reset?" << endl;
    cout << "Default is 1" << endl;
    cout << "A value of n will set the PKAM length to n*256 + 20 clock cycles" << endl;
    int PKAM;
    if(test_num != 0) cin >> PKAM;
	else PKAM = 1;
	PKAM_Enable( PKAM, 20 );
	
	cout << "PKAM Enabled!" << endl;
	Set_Num_Hits(10);
	cout << "Test PKAM Reset" << endl;
	sleep(3);
	Test_Resets_FIFO1(1, 0);
	sleep(3);
  }

  if (test_num == 10 ){
	//test Resets on DDR
    current_test = 10;
	Set_Num_Hits(0);
	cout << "Test TBM and ROC reset on DDR" << endl;
	sleep(1);
	Test_Timeouts(1, 1, 2, 1);
    sleep(2);

	cout << "How long do you want the PKAM reset?" << endl;
    cout << "Default is 1" << endl;
    cout << "A value of n will set the PKAM length to n*256 + 20 clock cycles" << endl;
    int PKAM;
    if(test_num != 0) cin >> PKAM;
	else PKAM = 1;
    PKAM_Enable( PKAM, 20 );
    
    cout << "PKAM Enabled!" << endl;
    Set_Num_Hits(10);
    cout << "Test PKAM Reset" << endl;
    sleep(3);
    Test_Timeouts(1, 1, 1, 3);
    sleep(3);
  }

  if (test_num == 11){
    current_test = 11;
	cout << "What is the first delay time?" << endl;
	int Delay1;
	if(test_num != 0) cin >> Delay1;
	else Delay1 = 1;
	cout << "What is the timeout time?" << endl;
	int Delay2;
	if(test_num != 0) cin >> Delay2;
	else Delay2 = 255;
	cout << "How many events do you want to send? ( more than 3 )" << endl;
	int loops;
	if(test_num != 0) cin >> loops;
	else loops = 4;
	Test_Timeouts(Delay1, Delay2, loops, 0); //Sent events with two different timeouts and one event at a time
	cout << "Timeout Test Complete" << endl;
    sleep(2);
  }

  if (test_num == 12 || (test_num == 0 && start == 12)){
    current_test = 12;
    start = 14;
	if(test_num != 0){
		Set_Num_Hits(0);
		cout << "What channels would you like to test? (0 or 0x55555555 is all channels)" << endl;
		cout << "0x(ch7)(ch6)(ch5)(ch4)(ch3)(ch2)(ch1)(ch0)" << endl;
		int Which_Chan;
		cin >> std::hex >> Which_Chan;
		cout << "What Marker would you like to test?" << endl;
		cout << "1. TBM Header. 2. TBM Trailer. 3. ROC Header" << endl;
		int Marker_Type;
		cin >> Marker_Type;
		cout << "What is the new marker (12 bits)?" << endl;
		int Marker_Value;
		cin >> std::hex >> Marker_Value;
		cout << "What rate would you like to have it occur? (0 is every L1A)" << endl;
		int Marker_Rate;
		cin >> std::dec >> Marker_Rate;
		Marker_Error( Which_Chan, Marker_Type, Marker_Value, Marker_Rate); //Write to register to set the values of the Marker described in DDR.c
  		cout << "How many loops do you want to go through?" << endl;
		int loops;
		cin >> loops;
		Test_Timeouts( 1, 1, loops, 0, Which_Chan); //Send events and look at output
		Marker_Error( Which_Chan, 0, Marker_Value, Marker_Rate); //Disable Marker mask such that markers are normal
	}
	else{
		for(int i = 1; i < 4; i++){
			Set_Num_Hits(0);
        		cout << "Test all channels" << endl;
        		int Which_Chan = 0x11111111;
        		switch(i){
		    		case 1: cout << "Testing TBM Header" << endl;
		    		case 2: cout << "Testing TBM Trailer" << endl;
		    		case 3: cout << "Testing ROC Header" << endl;
		    	}
        		cout << "Set new marker to 0x7E8" << endl;
        		int Marker_Value = 0x7E8;
        		cout << "Set the rate to every 3 L1As" << endl;
        		int Marker_Rate = 2;
        		Marker_Error( Which_Chan, i, Marker_Value, Marker_Rate); //Write to register to set the values of the Marker described in DDR.ci
        		int loops = 4;
        		Test_Timeouts( 1, 1, loops, 0); //Send events and look at output
                sleep(2);
        		Marker_Error( Which_Chan, 0, Marker_Value, Marker_Rate); //Disable Marker mask such that markers are normal
		}
	}
  }

  if (test_num == 13){
	//Add DDR pixel alive here.
    current_test = 13;
    Set_Num_Hits(1);
    Test_Hits_Full_DDR(1, 0, 0, 0, 0, 0, 0, 0);
  }

  if (test_num == 14 || (test_num == 0 && start == 14)){
	//Test Resets of Error FIFO
    current_test = 14;
    start = 16;
    Set_Num_Hits(0);
    cout << "Test TBM and ROC reset on Error FIFO" << endl;
    sleep(1);
    Test_Timeouts_Error(32, 32, 2, 1);
    sleep(2);
	
	cout << "How long do you want the PKAM reset?" << endl;
    cout << "Default is 1" << endl;
    cout << "A value of n will set the PKAM length to n*256 + 20 clock cycles" << endl;
    int PKAM;
    if(test_num != 0) cin >> PKAM;
	else PKAM = 1;
    PKAM_Enable( PKAM, 20 );
    
    cout << "PKAM Enabled!" << endl;
    Set_Num_Hits(10);
    cout << "Test PKAM Reset" << endl;
    sleep(3);
    Test_Timeouts_Error(32, 32, 1, 3);
    sleep(3);
  }

  if (test_num == 15){
	current_test = 15;
    cout << "What is the first delay time?" << endl;
    int Delay1;
	if(test_num != 0) cin >> Delay1;
	else Delay1 = 32;
	cout << "What is the timeout time?" << endl;
	int Delay2;
	if(test_num != 0) cin >> Delay2;
	else Delay2 = 255;
	cout << "How many events do you want to send? ( more than 3 )" << endl;
	int loops;
	if(test_num != 0) cin >> loops;
	else loops = 10;
    Test_Timeouts_Error(Delay1, Delay2, loops, 0); //Sent events with two different timeouts and one event at a time
    cout << "Timeout Test Complete" << endl;
    sleep(2);
  }

  if (test_num == 16 || (test_num == 0 && start == 16)){
    current_test = 16;
    start = 17;
	if(test_num != 0){
		Set_Num_Hits(0);
        cout << "What channels would you like to test? (0 or 0x55555555 is all channels)" << endl;
        cout << "0x(ch7)(ch6)(ch5)(ch4)(ch3)(ch2)(ch1)(ch0)" << endl;
        int Which_Chan;
        cin >> std::hex >> Which_Chan;
        cout << "What Marker would you like to test?" << endl;
        cout << "1. TBM Header. 2. TBM Trailer. 3. ROC Header" << endl;
        int Marker_Type;
        cin >> Marker_Type;
        cout << "What is the new marker (12 bits)?" << endl;
        int Marker_Value;
        cin >> std::hex >> Marker_Value;
        cout << "What rate would you like to have it occur? (0 is every L1A)" << endl;
        int Marker_Rate;
        cin >> std::dec >> Marker_Rate;
        Marker_Error( Which_Chan, Marker_Type, Marker_Value, Marker_Rate); //Write to register to set the values of the Marker described in DDR.c
        cout << "How many loops do you want to go through?" << endl;
        int loops;
        cin >> loops;
        Test_Timeouts_Error( 32, 32, loops, 0); //Send events and look at output
        Marker_Error( Which_Chan, 0, Marker_Value, Marker_Rate); //Disable Marker mask such that markers are normal
	}
	else{
        for(int i = 1; i < 4; i++){
            Set_Num_Hits(0);
            cout << "Test all channels" << endl;
            int Which_Chan = 0x55555555;
            switch(i){
                case 1: cout << "Testing TBM Header" << endl;
                case 2: cout << "Testing TBM Trailer" << endl;
                case 3: cout << "Testing ROC Header" << endl;
            }
            cout << "Set new marker to 0x7E8" << endl;
            int Marker_Value = 0x7E8;
            cout << "Set the rate to every 3 L1As" << endl;
            int Marker_Rate = 2;
            Marker_Error( Which_Chan, i, Marker_Value, Marker_Rate); //Write to register to set the values of the Marker described in DDR.ci
            int loops = 4;
            Test_Timeouts_Error( 32, 32, loops, 0); //Send events and look at output
            sleep(2);
            Marker_Error( Which_Chan, 0, Marker_Value, Marker_Rate); //Disable Marker mask such that markers are normal
        }
    }
  }

  if (test_num == 17 || (test_num == 0 && start == 17)){
    current_test = 17;
    start = 21;
    Set_Num_Hits(0);
    writeCSRMode(0x2);
    cout << "Test TBM, ROC, and CSR reset on all FIFOs" << endl;
    sleep(1);
    All_Resets(1, 1, 2, 1, 1, 1, 0);
    All_Resets(1, 1, 2, 6, 1, 1, 0);
    writeCSRMode(0x0);
    sleep(2);

	cout << "How long do you want the PKAM reset?" << endl;
    cout << "Default is 5" << endl;
    cout << "A value of n will set the PKAM length to n*256 + 20 clock cycles" << endl;
    int PKAM;
    if(test_num != 0) cin >> PKAM;
	else PKAM = 1;
    PKAM_Enable( PKAM, 20 );
    
    cout << "PKAM Enabled!" << endl;
    Set_Num_Hits(10);
    cout << "Test PKAM Reset" << endl;
    sleep(3);
    All_Resets(1, 1, 1, 3, 1, 1, 0);
    sleep(3);
  }

  if (test_num == 18){ // || test_num == 0){ // Disable this is all run for now
    current_test = 18;
	if(test_num != 0){
		cout << "How many different Marker Errors?" << endl;
		int Marker_Loops;
		cin >> std::dec >> Marker_Loops;
		for( int i = 0; i < Marker_Loops; i++){
			cout << "What channels would you like to test? (0 or 0x55555555 is all channels)" << endl;
            cout << "0x(ch7)(ch6)(ch5)(ch4)(ch3)(ch2)(ch1)(ch0)" << endl;
            int Which_Chan;
            cin >> std::hex >> Which_Chan;
            cout << "What Marker would you like to test?" << endl;
            cout << "1. TBM Header. 2. TBM Trailer. 3. ROC Header" << endl;
            int Marker_Type;
            cin >> Marker_Type;
            cout << "What is the new marker (12 bits)?" << endl;
            int Marker_Value;
            cin >> std::hex >> Marker_Value;
            cout << "What rate would you like to have it occur? (0 is every L1A)" << endl;
            int Marker_Rate;
            cin >> std::dec >> Marker_Rate;
            Marker_Error( Which_Chan, Marker_Type, Marker_Value, Marker_Rate); //Write to register to set the values of the Marker described in Marker_multi_chan.cxx
   		}

        cout << "How many loops do you want to go through?" << endl;
        int loops;
        cin >> loops;
        All_Resets( 32, 32, loops, 0, 1, 1, 0); //Send events and look at output
        int Which_Chan = 0x55555555;
		int Marker_Value = 0x7F8;
		int Marker_Rate = 100;
		Marker_Error( Which_Chan, 0, Marker_Value, Marker_Rate); //Disable Marker mask such that markers are normal
  	}
	else{
        int Marker_Loops = 1;
        for( int i = 0; i < Marker_Loops; i++){
            int Which_Chan = 0x55555555;
            int Marker_Type = rand() % 3 + 1;
            int Marker_Value = rand() % 4095;
            int Marker_Rate = rand() % 5 + 1;
            Marker_Error( Which_Chan, Marker_Type, Marker_Value, Marker_Rate); //Write to register to set the values of the Marker described in Marker_multi_chan.cxx
        }

        cout << "How many loops do you want to go through?" << endl;
        int loops = 1;
        All_Resets( 32, 32, loops, 0, 1, 1, 0); //Send events and look at output
        sleep(2);
        int Which_Chan = 0x55555555;
        int Marker_Value = 0x7F8;
        int Marker_Rate = 100;
        Marker_Error( Which_Chan, 0, Marker_Value, Marker_Rate); //Disable Marker mask such that markers are normal
	}
  }

  if(test_num == 19 ){
	//random marker bit flip generator
	current_test = 19;
    cout << "Generating Random Marker Error" << endl;
	int Which_Marker = rand() % 3 + 1;
	int New_Marker = Random_Marker( Which_Marker );
	
  	Marker_Error( 0x55555555, Which_Marker, New_Marker, 9);
	int loops;
	if(test_num != 0){
		cout << "How many loops?" << endl;
		cin >> loops;
	}
	else loops = 10;
	All_Resets( 32, 32, loops, 0, 1, 1, 1);
    sleep(2);
	Marker_Error( 0x55555555, 0, 0x7FC, 0);

  }

  if(test_num == 20 ){
    current_test = 20;
	cout << "Random Hits in pixel" << endl;
	cout << "How many loops?" << endl;
	int loops;
	if(test_num != 0) cin >> loops;
	else loops = 10;
	All_Resets( 32, 32, loops, 4, 1, 1, 2);
    sleep(2);
  }

  if(test_num == 21 || (test_num == 0 && start == 21)){
    current_test = 21;
    start = 24;
	cout << "TTC History FIFO test" << endl;
	cout << "Filling with TBM Reset" << endl;
	cout << "How many events do you want to send?" << endl;
	int Loops;
	if(test_num != 0) cin >> Loops;
	else Loops = 50000;
    //Set_Num_Hits(0);
	Test_TTC_History( Loops );
    sleep(2);
  }

  if(test_num == 22 ){
    current_test = 22;
	cout << "Memory Stress Test" << endl;
	int Loops;
	int Max_Loops;
	if(test_num != 0){
		cout << "How full do you want to fill DDR? (706 is about 90%)" << endl;
		cin >> Loops;
		cout << "How many times do you want to run the test?" << endl;
		cin >> Max_Loops;
	}
	else{
		Loops = 706;
		Max_Loops = 5;
	}
	Set_ROCS(8);
    Set_Num_Hits(2);
	Memory_Readout( Loops, Max_Loops);
    sleep(2);
  }

  if(test_num == 23 ){
    current_test = 23;
	cout << "Random Bit Flip Test" << endl;
	int Loops;
	if(test_num != 0){
		cout << "How many events do you want to send" << endl;
		cin >> Loops;
	}
	else 
		Loops = 10;
	int Random_Threshold = 0xFFFFFFFF;
	int Random_Value = 1;
  	Random_Bit_Flip(Loops, Random_Threshold, Random_Value);
    sleep(2);
  }

  if(test_num == 24 || (test_num == 0 && start == 24)){
    current_test = 24;
    start = 30;
	cout << "Running test over all possible Header data value" << endl;
	int Inner_Loops = 32;
	int Outer_Loops = 256;
	Set_Num_Hits(0);
    Test_Data_Header( Inner_Loops, Outer_Loops, 0, 0);
    Write_Data_Header(0);
    sleep(2);
  }

  if(test_num == 25){
    current_test = 25;
	for(int i = 0; i < 8; i++)
		Write_ROCs(i, 0x50401, 0x50401);
  }

  if(test_num == 26){
    current_test = 26;
	cout << "What poisson distribution?" << endl;
	float Distribution;
	cin >> Distribution;
	//Writing to the SRAM
	cout << "Do you want to load from text file?" << endl;
    int loadText;
    cin >> loadText;
    std::vector<unsigned int> StoreHits;
    if(loadText == 1){
        int start = 0;
        StoreHits = TextLoop();
        SRAM1_Write_board1(GLIB1_version, start + (0x1FFFFF*0), StoreHits);
        SRAM1_Write_board2(GLIB1_version, start + (0x1FFFFF*1), StoreHits);
        SRAM1_Write_board3(GLIB1_version, start + (0x1FFFFF*2), StoreHits);
    }
    else{ 
        SRAM1_Write_board1(GLIB1_version);
	    SRAM1_Write_board2(GLIB2_version);
	    SRAM1_Write_board3(GLIB3_version);
    }
    //SRAM2_Write_board1(GLIB1_version, 14.25); 3.5625 hit/ROC
	//SRAM2_Write_board2(GLIB2_version, 3.275); 1.6375 hit/ROC
	//SRAM2_Write_board3(GLIB3_version, 0.77125);
	//SRAM2_Write_board1(GLIB1_version, 26.3);  //6.575 hit/ROC
	//SRAM2_Write_board2(GLIB2_version, 6.025); //3.0125 hit/ROC
	//SRAM2_Write_board3(GLIB3_version, 1.425);
	SRAM2_Write_board1(GLIB1_version, Distribution);//, PKAM);
	SRAM2_Write_board2(GLIB2_version, Distribution);//, PKAM);
	SRAM2_Write_board3(GLIB3_version, Distribution);//, PKAM);
	Clear_SRAM_FIFO();
  }

  if(test_num == 30 || (test_num == 0 && start == 30)){
    current_test = 30;
	start = 33;
    int Inner_Loops = 10;
	int Outer_Loops = 256;
	Test_Data_Header( Inner_Loops, Outer_Loops, 1, 0);
    sleep(2);
  }

  if(test_num == 31){
    current_test = 31;
	int Inner_Loops = 100;
    int Outer_Loops = 100;

	Delay_Time(200);
    Test_Data_Header( Inner_Loops, Outer_Loops, 2, 0);
  }

  if(test_num == 32){
    current_test = 32;
    Set_Num_Hits(0);
    cout << "What channels would you like to test? (0 is all channels)" << endl;
    cout << "0x(ch7)(ch6)(ch5)(ch4)(ch3)(ch2)(ch1)(ch0)" << endl;
    int Which_Chan;
    cin >> std::hex >> Which_Chan;
    cout << "What Marker would you like to test?" << endl;
    cout << "1. TBM Header. 2. TBM Trailer. 3. ROC Header" << endl;
    int Marker_Type;
    cin >> Marker_Type;
    cout << "What is the new marker (12 bits)?" << endl;
    int Marker_Value;
    cin >> std::hex >> Marker_Value;
    cout << "What rate would you like to have it occur? (0 is every L1A)" << endl;
    int Marker_Rate;
    cin >> std::dec >> Marker_Rate;
    Marker_Error( Which_Chan, Marker_Type, Marker_Value, Marker_Rate); //Write to register to set the values of the Marker described in GLIBFunctions.c
    cout << "How many loops do you want to go through?" << endl;
    int Outer_Loops;
    cin >> Outer_Loops;
    int Inner_Loops = 1;
    Test_Data_Header( Inner_Loops, Outer_Loops, 3, 0);
    sleep(2);
    Marker_Error( Which_Chan, 0, Marker_Value, Marker_Rate); //Disable Marker mask such that markers are normal
  }

  if(test_num == 33 || (test_num == 0 && start == 33)){
    current_test = 33;
	start = 35;
    int Inner_Loops = 100;
    int Outer_Loops = 100;
    Test_Data_Header( Inner_Loops, Outer_Loops, 4, 0);
    sleep(2);
  }

  if(test_num == 34){
    current_test = 34;
	cout << "What PKAM Value do you want? (0: Disable)" << endl;
    int PKAM_int = 2000;
    //cin >> PKAM_int;
    if(PKAM_int == 0) PKAM_Disable();
    else PKAM_Enable(PKAM_int/256, PKAM_int % 256);
    cout << "Do you want SRAM mode? (0: No 1: Yes)" << endl;
	int SRAM;
	cin >> SRAM;
	if(SRAM == 1) SRAM_Mode();
	else{ 
		SRAM_Disable();
		cout << "How many hits in each ROC?" << endl;
		int NumHits;
		cin >> NumHits;
		Set_Num_Hits(NumHits);
	}
	ADCAsEvn(1);
    Ferol_TBM_Header( 0, SRAM );
	ADCAsEvn(0);
  }

  if(test_num == 35 || (test_num == 0 && start == 35)){
    current_test = 35;
    start = 40;
    int Inner_Loops = 2;
    int Outer_Loops = 1;
    int Num_Hits;
    cout << "How many hits do you want?" << endl;
    if(test_num != 0) cin >> Num_Hits;
    else Num_Hits = 30;
    Set_Num_Hits(Num_Hits);
    PKAM_Disable();
    Test_Data_Header( Inner_Loops, Outer_Loops, 6, 0);
    sleep(2);
  }

  if(test_num == 36){
    current_test = 36;
	writeTTSState(8);
	readTTSState();
	writeTTSState(4);
	readTTSState();
	writeTTSState(8);
	readTTSState();
  }

  if(test_num == 37){
    current_test = 37;
	writeTTSState(8);
	readTTSState();
  }

  if(test_num == 38){
    current_test = 38;
	writeTTSState(4);
	readTTSState();
  }

  if(test_num == 39){
    current_test = 39;
	cout << "Start Andreas' version of Ferol Test" << endl;
	ferolTest();
  }

  if(test_num == 40 || (test_num == 0 && start == 40)){ 
    current_test = 40;
    start = 41;
    Set_Num_Hits(0);
	cout << "What Kind of Error Dump?" << endl;
	cout << "1. Marker Error (Trailer Error) 2. Delay Time" << endl;
	int option;
	if(test_num != 0) cin >> option;
    else option = 1;
    if(option == 1) Marker_Error(0x11111111, 1, 0x7EE, 4);
    if(option == 2) Delay_Time(255);
    Error_Counters(0, option, 10);
    sleep(2);
  }
  if(test_num == 41 || (test_num == 0 && start == 41)){
    current_test = 41;
    start = 42;
	cout << "What Kind of Trailer error?" << endl;
	cout << "1: Delay Time  2: Marker Error" << endl;
	int option;
	if(test_num != 0) cin >> option;
    else option = 2;
    if(option == 1) Delay_Time(255);
    if(option == 2) Marker_Error(0x11111111, 1, 0x7E8, 4);
    Error_Counters(1, option, 10);
    sleep(2);
  }

  if(test_num == 42 || (test_num == 0 && start == 42)){
    current_test = 42;
    start = 43;
	cout << "What channels should have the Marker Errors? (hex)" << endl;
	int Channels;
	if(test_num != 0) cin >> std::hex >>  Channels;
	else Channels = 0x55555555;
	Marker_Error(Channels, 1, 0x7E8, 4);
    Error_Counters(2, Channels, 10);
    sleep(2);
  }

  if(test_num == 43 || (test_num == 0 && start == 43)){
    current_test = 43;
    start = 44;
	cout << "What channels should have the Marker Errors? (hex)" << endl;
	int Channels;
	if(test_num != 0) cin >> std::hex >>  Channels;
	else Channels = 0x55555555;
    Marker_Error(Channels, 1, 0x7E8, 1);
    Error_Counters(3, Channels, 1);
    sleep(2);
  }

  if(test_num == 44 || (test_num == 0 && start == 44)){ 
    current_test = 44;
    start = 45;
    Marker_Error(0x11111111, 2, 0x7EE, 1);
    Error_Counters(4, 0, 10);
  }

  if(test_num == 45 || (test_num == 0 && start == 45)){ 
    current_test = 45;
    start = 49;
	cout << "What Kind of Trailer error?" << endl;
	cout << "1: Delay Time  2: Marker Error" << endl;
	int option;
	if(test_num != 0) cin >> option;
    else option = 1;
	if(option == 1) Delay_Time(255);
    if(option == 2) Marker_Error(0x11111111, 1, 0x7E8, 1);
    Error_Counters(5, option, 10);
  }

  if(test_num == 46){
    current_test = 46;
	cout << "What delay time?" << endl;
	int Delay;
	cin >> Delay;
	Delay_Time(Delay);
  }

  if(test_num == 47){
    current_test = 47;
	cout << "What channels would you like to test? (0 is all channels)" << endl;
    cout << "0x(ch7)(ch6)(ch5)(ch4)(ch3)(ch2)(ch1)(ch0)" << endl;
    int Which_Chan;
    cin >> std::hex >> Which_Chan;
    cout << "What Marker would you like to test?" << endl;
    cout << "0. Reset. 1. TBM Header. 2. TBM Trailer. 3. ROC Header" << endl;
    int Marker_Type;
    cin >> Marker_Type;
    cout << "What is the new marker (12 bits)?" << endl;
    int Marker_Value;
    cin >> std::hex >> Marker_Value;
    cout << "What rate would you like to have it occur? (0 is every L1A)" << endl;
    int Marker_Rate;
    cin >> std::dec >> Marker_Rate;
    Marker_Error( Which_Chan, Marker_Type, Marker_Value, Marker_Rate);
  }

  if(test_num == 48){
    current_test = 48;
	cout << "What PKAM Value do you want? 0: Disable Else: #" << endl;
    int PKAM_int;
    cin >> PKAM_int;
    if(PKAM_int != 0) PKAM_Enable(PKAM_int/256, PKAM_int % 256);
    else PKAM_Disable();
    cout << "Do you want SRAM mode? (0: No 1: Yes)" << endl;
	int SRAM;
	cin >> SRAM;
	if(SRAM == 1) SRAM_Mode();
	else{ 
		SRAM_Disable();
		cout << "How many hits in each ROC?" << endl;
		int NumHits;
		cin >> NumHits;
		Set_Num_Hits(NumHits);
	}
	cout << "Start Ferol Test with TBM Header Error" << endl;
	Delay_Time(1);
	ADCAsEvn(1);
	int Choice;
    cout << "What kind of error?" << endl;
    cout << " 1. Random channel Marker error" << endl;
    cout << " 2. TBM Header error" << endl;
    cout << " 3. Marker Error IDLE" << endl;
    cin >> Choice;
    Ferol_TBM_Header( Choice, SRAM );
	ADCAsEvn(0);
  }

  if(test_num == 49 || (test_num == 0 && start == 49)){
    current_test = 49;
    start = 53;
	//Set_Num_Hits(1);
    Set_Num_Hits( 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x55555555);
	cout << "Start SEU Run" << endl;
	int type, mode;
	uint32_t Which_Chan;
	if(test_num != 0){
		cout << "Which Type of signal do you want to stop? (1: low Else: high)" << endl;
		cin >> type;
		cout << "Which Marker do you want to trigger on? (1: TBM Header 2: TBM Trailer 3: ROC Header)" << endl;
		cin >> mode;
		cout << "Which channels do you want to do? (0x55555555 is all and every even bit is a channel)" << endl;
		cin >> std::hex >> Which_Chan;
	}
	else{
		mode = 1;
		type =  2;
		Which_Chan = 0x55555555;
	}
    int Loops = 40;
	SEU_Analyze(type, mode, Which_Chan, Loops);
  }

  if(test_num == 50){
    current_test = 50;
	cout << "Start SEU Run with Random Errors" << endl;
	int Random_Chan = 1 << 2*(rand() % 16); 
	SEU_Analyze(1, 0, Random_Chan, 1);
  }

  if(test_num == 51){
    current_test = 51;
	cout << "Test L1 Emulation Decoding" << endl;
	Set_Num_Hits( 1, 2, 1, 1, 1, 1, 0x55555555);
	ADCAsEvn(1);
	All_Resets(32, 32, 2, 1, 1, 1, 0);
	ADCAsEvn(0);
  }
  
  if(test_num == 52){
    current_test = 52;
	cout << "What PKAM Value do you want?" << endl;
    int PKAM_int;
    cin >> PKAM_int;
    PKAM_Enable(PKAM_int/256, PKAM_int % 256);
    cout << "Do you want SRAM mode? (0: No 1: Yes)" << endl;
	int SRAM;
	cin >> SRAM;
	if(SRAM == 1) SRAM_Mode();
	else{ 
		SRAM_Disable();
		cout << "How many hits in each ROC?" << endl;
		int NumHits;
		cin >> NumHits;
        Set_Num_Hits( 0x04050405, 0x05050505, 0x05060506, 0x06060606, 0x06070607, 0x07070707, 0x00005555);
        Set_Num_Hits( 0x03040506, 0x04040506, 0x04050607, 0x05050607, 0x05060708, 0x06060708, 0x55550000);
        //Set_Num_Hits( 0x04050506, 0x04050506, 0x04050506, 0x04050506, 0x04050506, 0x04050506, 0x55555555);
		//Set_Num_Hits(NumHits);
	}
    //Delay_Time(200);
	ADCAsEvn(1);
	Ferol_TBM_Header( true, false, SRAM );
	ADCAsEvn(0);
  }
  
  if(test_num == 53 || (test_num == 0 && start == 53)){ 
    current_test = 53;
	start = 54;
    ADCAsEvn(1);
    Set_Num_Hits(7);
    //Set_Num_Hits( 0x07070707, 0x07070707, 0x07070707, 0x07070707, 0x07070707, 0x07070707, 0x55555555);
    Error_Counters(6, 0, 2);
  }

  if(test_num == 54 || (test_num == 0 && start == 54)){ 
    current_test = 54;
    start = 56;
    //PKAM_Enable(255, 255);
	PKAM_Disable();
    ADCAsEvn(1);
    Set_Num_Hits(1);
    Marker_Error( 0x11111111, 2, 0x7F8, 1);
    //Set_Num_Hits( 0x70707171, 0x70707171, 0x70707171, 0x70707171, 0x70707171, 0x70707171, 0x00000001);
    Error_Counters(7, 0, 1);
  }

  if(test_num == 55) FEROL_Data_Throughputs(15, 10);

  if(test_num == 56 || (test_num == 0 && start == 56)){
    current_test = 56;
    start = 57;
    Set_Num_Hits( 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x55555555);
    cout << "What Delay do you want?" << endl;
    int Delay;
    if(test_num != 0) cin >> Delay;
    else Delay = 255;
    Private_Resync(20, Delay);
  }

  if(test_num == 57 || (test_num == 0 && start == 57)){
    current_test = 57;
    start = 59;
    Set_Num_Hits(0);
    Delay_Time(50);
    Marker_Error_Idle( 0x55555555, 1, 0x7FC, 3, 50);
    TBM_Header_Idle(2, 0);
  }

  if(test_num == 58){
    current_test = 58;
    Set_Num_Hits(0);
    TBM_Header_Idle(1, 1);
  }

  if(test_num == 59 || (test_num == 0 && start == 59)){
    current_test = 59;
    Set_Num_Hits( 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x55555555);
    Marker_Error(0x1, 1, 0x7E8, 3);
    Stacked_Events(2);
  }

  if(test_num == 60){
    std::vector<unsigned int> FED_ENE;
    std::vector<unsigned int> FED_ETOE;
    FED_ENE = TextLoop("FED_summary_ene.txt");
    FED_ETOE = TextLoop("FED_summary_etoe.txt");
    FED_Summary_Plots(FED_ENE, FED_ETOE, 300284.2);
  }

  if(test_num == 61){
    current_test = 61;
	cout << "What PKAM Value do you want?" << endl;
    int PKAM_int;
    cin >> PKAM_int;
    PKAM_Enable(PKAM_int/256, PKAM_int % 256);
	ADCAsEvn(1);
	Ferol_TBM_Header( false, true, 0 );
	ADCAsEvn(0);
  }
  
//  if(test_num == 58) GLIB_Test();
}
catch(QuitNow){
	cout << "FEDTester has Terminated at Test: "<< std::dec << current_test << endl;
}

  return 0;
}
