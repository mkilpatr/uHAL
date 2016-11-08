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
#include "FIFO_Spy.c"
#include "All_Channels.c"
//#include "FIFO1.c"
//#include "DDR.c"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <typeinfo>
#include <list>
#include <stdlib.h>
#include <iterator>

int main()
{

  using namespace std;
  using namespace uhal;
 
  //ConnectionManager manager ("file://test/dummy_connections.xml");
  //HwInterface hw=manager.getDevice ( "GLIB.crate.slot_3" );   //this is to glib with the optical card
  //HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card

  std::cout << "    ******************************************    " << std::endl;
  std::cout << "    *                                        *    " << std::endl;
  std::cout << "    *           FED Testing Program          *    " << std::endl;
  std::cout << "    *                  (FTP)                 *    " << std::endl;
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
  std::cout << "  8. Test FIFO1 Pixel Alive" << std::endl;
  std::cout << "  9. Test TBM/ROC/PKAM resets on FIFO1" << std::endl;
  std::cout << " 10. Test TBM/ROC/PKAM resets on DDR" << std::endl;
  std::cout << " 11. Test DDR Timeouts" << std::endl;
  std::cout << " 12. Test DDR Timeouts with Marker Errors" << std::endl;
  std::cout << " 13. Test DDR Pixel Alive" << std::endl;
  std::cout << " 14. Test TBM/ROC/PKAM resets on Error FIFO" << std::endl;
  std::cout << " 15. Test Timeouts on Error FIFO" << std::endl;
  std::cout << " 16. Test Timeouts with Marker errors on Error FIFO" << std::endl;
  std::cout << " 17. Check all resets in program" << std::endl;
  std::cout << " 18. Variable Marker Errors in different channels" << std::endl;
  std::cout << " 19. Random Bit flip generator" << std::endl;
  std::cout << " 20. Random ROC Hits generator" << std::endl;
  std::cout << " 21. TTC History test" << std::endl;
  std::cout << " 22. Memory Stress Test" << std::endl;
  std::cout << " 23. Random Bit Flip Test" << std::endl;
  std::cout << " 24. Scan all possible Header data values" << std::endl;
  std::cout << " 25. Write ROCS with pixel hits value" << std::endl;
  std::cout << " 26. Writing registers of the SRAM1" << std::endl;
  std::cout << " 27. Reading from SRAM1" << std::endl;
  std::cout << " 28. Writing registers of the SRAM2" << std::endl;
  std::cout << " 29. Reading from SRAM2" << std::endl;
  std::cout << " 30. Test the BX id of the FED" << std::endl;
  std::cout << " 31. Run SRAM Readout Events" << std::endl;
  std::cout << " 32. Trailer Timeout valid test" << std::endl;
  std::cout << " " << std::endl;
  std::cout << "Which test would you like to run? ";

  unsigned int test_num;
  std::cin >> test_num;

  std::cout << " " << std::endl;

  if (test_num == 1) {
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

  }  

  if (test_num == 2 || test_num == 0) {
    //Check resets
  
    if(test_num != 0){
    	std::cout << "Starting TBM and ROC reset in Spy FIFO" << std::endl;
    	sleep(2);
    	Set_ROCS(8);
	Set_Num_Hits( 0 );    
	Test_Hits_Resets( 1, 2 );
    	sleep(2);
  
    	std::cout << "How long do you want the PKAM reset?" << std::endl;
    	std::cout << "Default is 5" << std::endl;
    	std::cout << "A value of n will set the PKAM length to n*256 + 20 clock cycles" << std::endl;
    	int PKAM;
    	std::cin >> PKAM;
    	PKAM_Enable( PKAM );
    	std::cout << "Starting PKAM Reset" << std::endl;
    	sleep(2);
    	Set_Num_Hits(15);
    	Test_Hits_Resets( 0, 1 );
	//Test_Hits_Full(1,0,1,0,1,0,1,0);
    }
    else{
	std::cout << "Starting TBM and ROC reset in Spy FIFO" << std::endl;
        sleep(2);
        Set_ROCS(8);
   	Set_Num_Hits( 0 );
     	Test_Hits_Resets( 1, 2 );
        sleep(2);

        std::cout << "Starting PKAM reset in Spy FIFO" << std::endl;
        std::cout << "A value of 1 will set the PKAM length to 256 + 20 clock cycles" << std::endl;
        int PKAM = 1;
        PKAM_Enable( PKAM );
        sleep(2);
        Set_Num_Hits(10);
        Test_Hits_Resets( 0, 1 );
	//Test_Hits_Full(1,0,1,0,1,0,1,0);
    }
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

  if (test_num == 6 || test_num == 0) {
    //Check stack/NTP
    std::cout << "Checking the stack and no token pass" << std::endl;
    sleep(2);
    Set_ROCS(8);
    Set_Num_Hits(4);
    PKAM_Enable(20);
    //Test_Hits_Full(1,0,1,0,1,0,24,1);
    Stack_Test(1,1); 

    std::cout << "Stack/NTP test complete!" << std::endl;
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

  if (test_num == 7 || test_num == 0) {
    //Monitor phases
    if(test_num != 0){
	std::cout << "Which fiber would you like to monitor? (-1 = current fiber) " << std::endl;
    	int fiber_input;
    	cin >> fiber_input;
    	std::cout << "How long would you like to monitor the phase for? " << std::endl;
    	int time_input;
    	cin >> time_input;
    	Test_Phases(fiber_input, time_input);
    
    	std::cout << " " << std::endl;
    	std::cout << "Phase Monitoring complete" << std::endl;
    }
    else{
	for(int i = 0; i < 12; i++){
		std::cout << "Monitor fiber " << i << " for 10 seconds" << std::endl;
    		int time_input = 10;
    		Test_Phases(i, time_input);
	}
    	std::cout << " " << std::endl;
    	std::cout << "Phase Monitoring complete" << std::endl;
    }
  }

  if (test_num == 8 || test_num == 0) {
    int Num_Hits = 0x1;
    Set_ROCS(8);
    Set_Num_Hits(Num_Hits);
    cout << "Do you want to run SRAM mode?" << endl;
    int SRAM_Mode;
    cin >> SRAM_Mode;
    Test_Hits_Full_FIFO1(5, 0, 0, 0, 0, 0, 1, 0, Num_Hits, SRAM_Mode);
  }

  if (test_num == 9 || test_num == 0){
	if(test_num != 0){
		Set_ROCS(8);
		Set_Num_Hits(0);
		std::cout << "Test TBM and ROC reset in FIFO1" << std::endl;
		sleep(1);
		Test_Resets_FIFO1(2, 1);

		std::cout << "How long do you want the PKAM reset?" << std::endl;
    		std::cout << "Default is 5" << std::endl;
    		std::cout << "A value of n will set the PKAM length to n*256 + 20 clock cycles" << std::endl;
    		int PKAM;
    		std::cin >> PKAM;
		PKAM_Enable( PKAM );
	
		std::cout << "PKAM Enabled!" << std::endl;
		Set_Num_Hits(10);
		std::cout << "Test PKAM Reset" << std::endl;
		sleep(3);
		Test_Resets_FIFO1(1, 0);
		sleep(3);
	}
	else{
		Set_ROCS(8);
                Set_Num_Hits(0);
                std::cout << "Test TBM and ROC reset in FIFO1" << std::endl;
                sleep(1);
                Test_Resets_FIFO1(2, 1);

                std::cout << "How long do you want the PKAM reset?" << std::endl;
                std::cout << "Default is 5" << std::endl;
                std::cout << "A value of n will set the PKAM length to n*256 + 20 clock cycles" << std::endl;
                int PKAM = 1;
                PKAM_Enable( PKAM );
                Set_Num_Hits(10);
                std::cout << "Test PKAM Reset" << std::endl;
                sleep(3);
                Test_Resets_FIFO1(1, 0);
                sleep(3);

	}
  }

  if (test_num == 10 || test_num == 0){
	//test Resets on DDR
	if(test_num != 0){
		Set_ROCS(8);
		Set_Num_Hits(0);
		std::cout << "Test TBM and ROC reset on DDR" << std::endl;
		sleep(1);
		Test_Timeouts(32, 32, 2, 1);

		std::cout << "How long do you want the PKAM reset?" << std::endl;
        	std::cout << "Default is 5" << std::endl;
        	std::cout << "A value of n will set the PKAM length to n*256 + 20 clock cycles" << std::endl;
        	int PKAM;
        	std::cin >> PKAM;
        	PKAM_Enable( PKAM );

        	std::cout << "PKAM Enabled!" << std::endl;
        	Set_Num_Hits(10);
        	std::cout << "Test PKAM Reset" << std::endl;
        	sleep(3);
        	Test_Timeouts(32, 32, 1, 3);
        	sleep(3);
	}
	else{
		Set_ROCS(8);
        	Set_Num_Hits(0);
        	std::cout << "Test TBM and ROC reset on DDR" << std::endl;
        	sleep(1);
        	Test_Timeouts(32, 32, 2, 1);

        	std::cout << "Test PKAM reset on DDR" << std::endl;
        	std::cout << "A value of 1 will set the PKAM length such that 256 + 20 clock cycles" << std::endl;
        	int PKAM = 1;
        	PKAM_Enable( PKAM );
        	Set_Num_Hits(10);
        	sleep(3);
        	Test_Timeouts(32, 32, 1, 3);
        	sleep(3);
	}
  }

  if (test_num == 11 || test_num == 0){
	if(test_num != 0){
		std::cout << "What is the first delay time?" << std::endl;
		int Delay1;
		cin >> Delay1;
		std::cout << "What is the timeout time?" << std::endl;
		int Delay2;
		cin >> Delay2;
		std::cout << "How many events do you want to send? ( more than 3 )" << std::endl;
		int loops;
		cin >> loops;
		Test_Timeouts(Delay1, Delay2, loops, 0); //Sent events with two different timeouts and one event at a time

		std::cout << "Timeout Test Complete" << std::endl;
  	}
	else{
        	std::cout << "Set the delay to 255 for a single event" << std::endl;
		int Delay1 = 32;
        	int Delay2 = 255;
        	std::cout << "Send 4 events" << std::endl;
        	int loops = 4;
        	Test_Timeouts(Delay1, Delay2, loops, 0); //Sent events with two different timeouts and one event at a time

        	std::cout << "Timeout Test Complete" << std::endl;
	}
  }

  if (test_num == 12 || test_num == 0){
	if(test_num != 0){
		Set_Num_Hits(0);
		std::cout << "What channels would you like to test? (0 is all channels)" << std::endl;
		std::cout << "0x(ch7)(ch6)(ch5)(ch4)(ch3)(ch2)(ch1)(ch0)" << std::endl;
		int Which_Chan;
		cin >> std::hex >> Which_Chan;
		std::cout << "What Marker would you like to test?" << std::endl;
		std::cout << "1. TBM Header. 2. TBM Trailer. 3. ROC Header" << std::endl;
		int Marker_Type;
		cin >> Marker_Type;
		std::cout << "What is the new marker (12 bits)?" << std::endl;
		int Marker_Value;
		cin >> std::hex >> Marker_Value;
		std::cout << "What rate would you like to have it occur? (0 is every L1A)" << std::endl;
		int Marker_Rate;
		cin >> std::dec >> Marker_Rate;
		Marker_Error( Which_Chan, Marker_Type, Marker_Value, Marker_Rate); //Write to register to set the values of the Marker described in DDR.c
  		std::cout << "How many loops do you want to go through?" << std::endl;
		int loops;
		cin >> loops;
		Test_Timeouts( 32, 32, loops, 0); //Send events and look at output
		Marker_Error( Which_Chan, 0, Marker_Value, Marker_Rate); //Disable Marker mask such that markers are normal
	}
	else{
		for(int i = 1; i < 4; i++){
			Set_Num_Hits(0);
        		std::cout << "Test all channels" << std::endl;
        		int Which_Chan = 0x11111111;
        		switch(i){
				case 1: std::cout << "Testing TBM Header" << std::endl;
				case 2: std::cout << "Testing TBM Trailer" << std::endl;
				case 3: std::cout << "Testing ROC Header" << std::endl;
			}
        		std::cout << "Set new marker to 0x7E8" << std::endl;
        		int Marker_Value = 0x7E8;
        		std::cout << "Set the rate to every 3 L1As" << std::endl;
        		int Marker_Rate = 2;
        		Marker_Error( Which_Chan, i, Marker_Value, Marker_Rate); //Write to register to set the values of the Marker described in DDR.ci
        		int loops = 4;
        		Test_Timeouts( 32, 32, loops, 0); //Send events and look at output
        		Marker_Error( Which_Chan, 0, Marker_Value, Marker_Rate); //Disable Marker mask such that markers are normal
		}
	}
  }

  if (test_num == 13 || test_num == 0){
	//Add DDR pixel alive here.
	Set_ROCS(8);
    	Set_Num_Hits(1);
    	Test_Hits_Full_DDR(1, 0, 0, 0, 0, 0, 0, 0);
  }

  if (test_num == 14 || test_num == 0){
	//Test Resets of Error FIFO
	if(test_num != 0){
		Set_ROCS(8);
        	Set_Num_Hits(0);
        	std::cout << "Test TBM and ROC reset on Error FIFO" << std::endl;
        	sleep(1);
        	Test_Timeouts_Error(32, 32, 2, 1);
	
		std::cout << "How long do you want the PKAM reset?" << std::endl;
        	std::cout << "Default is 1" << std::endl;
        	std::cout << "A value of n will set the PKAM length to n*256 + 20 clock cycles" << std::endl;
        	int PKAM;
        	std::cin >> PKAM;
        	PKAM_Enable( PKAM );

        	std::cout << "PKAM Enabled!" << std::endl;
        	Set_Num_Hits(10);
        	std::cout << "Test PKAM Reset" << std::endl;
        	sleep(3);
        	Test_Timeouts_Error(32, 32, 1, 3);
        	sleep(3);
	}
	else{
		Set_ROCS(8);
        	Set_Num_Hits(0);
        	std::cout << "Test TBM and ROC reset on Error FIFO" << std::endl;
        	sleep(1);
        	Test_Timeouts_Error(32, 32, 2, 1);

		std::cout << " Test PKAM reset on Error FIFO " << std::endl;
        	std::cout << "A value of 1 will set the PKAM length such that 256 + 20 clock cycles" << std::endl;
        	int PKAM = 1;
        	PKAM_Enable( PKAM );

        	Set_Num_Hits(10);
        	sleep(3);
        	Test_Timeouts_Error(32, 32, 1, 3);
        	sleep(3);
	}
  }

  if (test_num == 15 || test_num == 0){
	if(test_num != 0){
		std::cout << "What is the first delay time?" << std::endl;
        	int Delay1;
        	cin >> Delay1;
        	std::cout << "What is the timeout time?" << std::endl;
        	int Delay2;
        	cin >> Delay2;
        	std::cout << "How many events do you want to send? ( more than 3 )" << std::endl;
        	int loops;
        	cin >> loops;
        	Test_Timeouts_Error(Delay1, Delay2, loops, 0); //Sent events with two different timeouts and one event at a time

        	std::cout << "Timeout Test Complete" << std::endl;
	}
  	else{
		std::cout << "Set delay for one event to 255 clocks" << std::endl;
        	int Delay1 = 32;
        	int Delay2 = 255;
        	std::cout << "Send 4 events" << std::endl;
        	int loops = 4;
        	Test_Timeouts_Error(Delay1, Delay2, loops, 0); //Sent events with two different timeouts and one event at a time

        	std::cout << "Timeout Test Complete" << std::endl;
	}
  }

  if (test_num == 16 || test_num == 0){
	if(test_num != 0){
		Set_Num_Hits(0);
        	std::cout << "What channels would you like to test? (0 is all channels)" << std::endl;
        	std::cout << "0x(ch7)(ch6)(ch5)(ch4)(ch3)(ch2)(ch1)(ch0)" << std::endl;
        	int Which_Chan;
        	cin >> std::hex >> Which_Chan;
        	std::cout << "What Marker would you like to test?" << std::endl;
        	std::cout << "1. TBM Header. 2. TBM Trailer. 3. ROC Header" << std::endl;
        	int Marker_Type;
        	cin >> Marker_Type;
        	std::cout << "What is the new marker (12 bits)?" << std::endl;
        	int Marker_Value;
        	cin >> std::hex >> Marker_Value;
        	std::cout << "What rate would you like to have it occur? (0 is every L1A)" << std::endl;
        	int Marker_Rate;
        	cin >> std::dec >> Marker_Rate;
        	Marker_Error( Which_Chan, Marker_Type, Marker_Value, Marker_Rate); //Write to register to set the values of the Marker described in DDR.c
        	std::cout << "How many loops do you want to go through?" << std::endl;
        	int loops;
        	cin >> loops;
        	Test_Timeouts_Error( 32, 32, loops, 0); //Send events and look at output
        	Marker_Error( Which_Chan, 0, Marker_Value, Marker_Rate); //Disable Marker mask such that markers are normal
	}
	else{
                for(int i = 1; i < 4; i++){
                        Set_Num_Hits(0);
                        std::cout << "Test all channels" << std::endl;
                        int Which_Chan = 0x11111111;
                        switch(i){
                                case 1: std::cout << "Testing TBM Header" << std::endl;
                                case 2: std::cout << "Testing TBM Trailer" << std::endl;
                                case 3: std::cout << "Testing ROC Header" << std::endl;
                        }
                        std::cout << "Set new marker to 0x7E8" << std::endl;
                        int Marker_Value = 0x7E8;
                        std::cout << "Set the rate to every 3 L1As" << std::endl;
                        int Marker_Rate = 2;
                        Marker_Error( Which_Chan, i, Marker_Value, Marker_Rate); //Write to register to set the values of the Marker described in DDR.ci
                        int loops = 4;
                        Test_Timeouts_Error( 32, 32, loops, 0); //Send events and look at output
                        Marker_Error( Which_Chan, 0, Marker_Value, Marker_Rate); //Disable Marker mask such that markers are normal
                }
        }
  }

  if (test_num == 17 || test_num == 0){
	if(test_num != 0){
		Set_ROCS(8);
        	Set_Num_Hits(0);
       		std::cout << "Test TBM and ROC reset on all FIFOs" << std::endl;
        	sleep(1);
        	All_Resets(32, 32, 2, 1, 1, 1, 0);

		std::cout << "How long do you want the PKAM reset?" << std::endl;
        	std::cout << "Default is 5" << std::endl;
        	std::cout << "A value of n will set the PKAM length to n*256 + 20 clock cycles" << std::endl;
        	int PKAM;
        	std::cin >> PKAM;
        	PKAM_Enable( PKAM );

        	std::cout << "PKAM Enabled!" << std::endl;
        	Set_Num_Hits(10);
        	std::cout << "Test PKAM Reset" << std::endl;
        	sleep(3);
        	All_Resets(32, 32, 1, 3, 1, 1, 0);
        	sleep(3);
	}
	else{
		Set_ROCS(8);
        	Set_Num_Hits(0);
        	std::cout << "Test TBM and ROC reset on all FIFOs" << std::endl;
        	sleep(1);
        	All_Resets(32, 32, 2, 1, 1, 1, 0);
	
        	int PKAM = 1;
        	PKAM_Enable( PKAM );
        	std::cout << "PKAM Enabled!" << std::endl;
        	Set_Num_Hits(10);
        	std::cout << "Test PKAM Reset" << std::endl;
        	sleep(3);
        	All_Resets(32, 32, 1, 3, 1, 1, 0);
        	sleep(3);
	}

  }

  if (test_num == 18 || test_num == 0){
	if(test_num != 0){
		std::cout << "How many different Marker Errors?" << std::endl;
		int Marker_Loops;
		cin >> std::dec >> Marker_Loops;
		for( int i = 0; i < Marker_Loops; i++){
			std::cout << "What channels would you like to test? (0 is all channels)" << std::endl;
        		std::cout << "0x(ch7)(ch6)(ch5)(ch4)(ch3)(ch2)(ch1)(ch0)" << std::endl;
        		int Which_Chan;
        		cin >> std::hex >> Which_Chan;
        		std::cout << "What Marker would you like to test?" << std::endl;
        		std::cout << "1. TBM Header. 2. TBM Trailer. 3. ROC Header" << std::endl;
        		int Marker_Type;
        		cin >> Marker_Type;
        		std::cout << "What is the new marker (12 bits)?" << std::endl;
        		int Marker_Value;
        		cin >> std::hex >> Marker_Value;
        		std::cout << "What rate would you like to have it occur? (0 is every L1A)" << std::endl;
        		int Marker_Rate;
        		cin >> std::dec >> Marker_Rate;
        		Marker_Error( Which_Chan, Marker_Type, Marker_Value, Marker_Rate); //Write to register to set the values of the Marker described in Marker_multi_chan.cxx
   		}

        	std::cout << "How many loops do you want to go through?" << std::endl;
        	int loops;
        	cin >> loops;
        	All_Resets( 32, 32, loops, 0, 1, 1, 0); //Send events and look at output
        	int Which_Chan = 0x11111111;
		int Marker_Value = 0x7F8;
		int Marker_Rate = 100;
		Marker_Error( Which_Chan, 0, Marker_Value, Marker_Rate); //Disable Marker mask such that markers are normal
  	}
	else{
        	int Marker_Loops = 1;
        	for( int i = 0; i < Marker_Loops; i++){
        	        int Which_Chan = 0x11111111;
        	        int Marker_Type = rand() % 3 + 1;
        	        int Marker_Value = rand() % 4095;
        	        int Marker_Rate = rand() % 5 + 1;
        	        Marker_Error( Which_Chan, Marker_Type, Marker_Value, Marker_Rate); //Write to register to set the values of the Marker described in Marker_multi_chan.cxx
        	}

        	std::cout << "How many loops do you want to go through?" << std::endl;
        	int loops = 1;
        	All_Resets( 32, 32, loops, 0, 1, 1, 0); //Send events and look at output
        	int Which_Chan = 0x11111111;
        	int Marker_Value = 0x7F8;
        	int Marker_Rate = 100;
        	Marker_Error( Which_Chan, 0, Marker_Value, Marker_Rate); //Disable Marker mask such that markers are normal
	}
  }

  if(test_num == 19 || test_num == 0){
	//random marker bit flip generator
	std::cout << "Generating Random Marker Error" << std::endl;
	int Which_Marker = rand() % 3 + 1;
	int New_Marker = Random_Marker( Which_Marker );
	
  	Marker_Error( 0x11111111, Which_Marker, New_Marker, 9);
	int loops;
	if(test_num != 0){
		std::cout << "How many loops?" << std::endl;
		std::cin >> loops;
	}
	else
		loops = 10;
	All_Resets( 32, 32, loops, 0, 1, 1, 1);
	Marker_Error( 0x11111111, 0, 0x7FC, 0);

  }

  if(test_num == 20 || test_num == 0){
	std::cout << "Random Hits in pixel" << std::endl;
	std::cout << "How many loops?" << std::endl;
	int loops;
	if(test_num != 0)
		std::cin >> loops;
	else
		loops = 10;
	All_Resets( 32, 32, loops, 4, 1, 1, 2);
  }

  if(test_num == 21 || test_num == 0){
	std::cout << "TTC History FIFO test" << std::endl;
	std::cout << "Filling with TBM Reset" << std::endl;
	std::cout << "How many events do you want to send?" << std::endl;
	int Loops;
	if(test_num != 0)
		std::cin >> Loops;
	else
		Loops = 100;
	Set_ROCS(8);
        Set_Num_Hits(0);
	Test_TTC_History( Loops );
  }

  if(test_num == 22 || test_num == 0){
	std::cout << "Memory Stress Test" << std::endl;
	int Loops;
	int Max_Loops;
	if(test_num != 0){
		std::cout << "How full do you want to fill DDR? (706 is about 90%)" << std::endl;
		std::cin >> Loops;
		std::cout << "How many times do you want to run the test?" << std::endl;
		std::cin >> Max_Loops;
	}
	else{
		Loops = 706;
		Max_Loops = 5;
	}
	Set_ROCS(8);
        Set_Num_Hits(2);
	Memory_Readout( Loops, Max_Loops);
  }

  if(test_num == 23 || test_num == 0){
	std::cout << "Random Bit Flip Test" << std::endl;
	int Loops;
	if(test_num != 0){
		std::cout << "Home many events do you want to send" << std::endl;
		std::cin >> Loops;
	}
	else 
		Loops = 10;
	int Random_Threshold = 0xFFFFFFFF;
	int Random_Value = rand() % 0xFFFFFFFF;
  	Random_Bit_Flip(Loops, Random_Threshold, Random_Value);
  }

  if(test_num == 24 || test_num == 0){
	std::cout << "Running test over all possible Header data value" << std::endl;
	int Inner_Loops = 32;
	int Outer_Loops = 256;
	Test_Data_Header( Inner_Loops, Outer_Loops, 0);
  }

  if(test_num == 25){
	for(int i = 0; i < 8; i++)
		Write_ROCs(i, 0x50401, 0x50401);
  }

  if(test_num == 26){
	//Testing Writing to the SRAM
	SRAM1_Write(3);
  }

  if(test_num == 27){
	SRAM1_Read(3);
  }

  if(test_num == 28){
	SRAM2_Write(3);
  }

  if(test_num == 29){
	SRAM2_Read(3);
  }

  if(test_num == 30 || test_num == 0){
	int Inner_Loops = 10;
	int Outer_Loops = 256;
	Test_Data_Header( Inner_Loops, Outer_Loops, 1);
  }

  if(test_num == 31){
	int Inner_Loops = 100;
        int Outer_Loops = 100;

	Delay_Time(200);
        Test_Data_Header( Inner_Loops, Outer_Loops, 2);
  }

  if(test_num == 32){
	Set_Num_Hits(0);
        std::cout << "What channels would you like to test? (0 is all channels)" << std::endl;
        std::cout << "0x(ch7)(ch6)(ch5)(ch4)(ch3)(ch2)(ch1)(ch0)" << std::endl;
        int Which_Chan;
        cin >> std::hex >> Which_Chan;
        std::cout << "What Marker would you like to test?" << std::endl;
        std::cout << "1. TBM Header. 2. TBM Trailer. 3. ROC Header" << std::endl;
        int Marker_Type;
        cin >> Marker_Type;
        std::cout << "What is the new marker (12 bits)?" << std::endl;
        int Marker_Value;
        cin >> std::hex >> Marker_Value;
        std::cout << "What rate would you like to have it occur? (0 is every L1A)" << std::endl;
        int Marker_Rate;
        cin >> std::dec >> Marker_Rate;
        Marker_Error( Which_Chan, Marker_Type, Marker_Value, Marker_Rate); //Write to register to set the values of the Marker described in DDR.c
        std::cout << "How many loops do you want to go through?" << std::endl;
        int Outer_Loops;
        cin >> Outer_Loops;
        int Inner_Loops = 1;
        Test_Data_Header( Inner_Loops, Outer_Loops, 3);
	Marker_Error( Which_Chan, 0, Marker_Value, Marker_Rate); //Disable Marker mask such that markers are normal
  }

  else if (test_num >= 33) {
    std::cout << "Not a valid test. Shutting down." << std::endl;
    return 0;
  }
 
  return 0;
}
