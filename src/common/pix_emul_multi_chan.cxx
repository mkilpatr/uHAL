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

int main( int argc, char* argv[] ){

  using namespace uhal;
  using namespace std;
  int Number_ROCS = 0;
  int PKAM = 0;
  int ROC_Clk = 0;
  int Hits_CHA = 0;
  int Hits_CHB = 0;
  int Reset = 0;
  int Which_Chan = 0;
  int Number_Channels = 0;
  string ROCSA[ 8 ] = {"CHA_ROC0_", "CHA_ROC1_", "CHA_ROC2_", "CHA_ROC3_", "CHA_ROC4_", "CHA_ROC5_", "CHA_ROC6_", "CHA_ROC7_"};
  string ROCSB[ 8 ] = {"CHB_ROC0_", "CHB_ROC1_", "CHB_ROC2_", "CHB_ROC3_", "CHB_ROC4_", "CHB_ROC5_", "CHB_ROC6_", "CHB_ROC7_"};
  string Chan[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};

  ConnectionManager manager ("file://test/dummy_connections_multi_chan.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_3" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card

  if( argc == 1 ) {
  std::cout << "What kind of Test?" << std::endl;
  std::cout << "0. Set the which Channel you would like to write" << std::endl;
  std::cout << "          Format: # Channel is 0x(ch7)(ch6)(ch5)(ch4)(ch3)(ch2)(ch1)(ch0)" << std::endl;    
  std::cout << "1. Set the number of ROCS in each channel" << std::endl;
  std::cout << "	How Many ROCS to you want?" << std::endl;
  std::cout << "		Format: 0x(dataB)(dataA)(MatrixB)(MatrixA)(HitROCB)(HitROCA)(ROCSB)(ROCSA)" << std::endl;	
  std::cout << "2. How many hits in each ROC" << std::endl;
  std::cout << "	How many hits do you want in CHA?" << std::endl;
  std::cout << "		Format: 0x(ROC7)(ROC6)(ROC5)(ROC4)(ROC3)(ROC2)(ROC1)(ROC0)" << std::endl;
  std::cout << "	How many hits do you want in CHB?" << std::endl;
  std::cout << "		Format: 0x(ROC7)(ROC6)(ROC5)(ROC4)(ROC3)(ROC2)(ROC1)(ROC0)" << std::endl;
  std::cout << "3. Scan through pixels" << std::endl;
  std::cout << "	Number of Pixel Columns? (6 bits)" << std::endl;
  std::cout << "	Number of Pixel Rows? (9 bits)" << std::endl;
  std::cout << "	Where do you want to start in CHA" << std::endl;
  std::cout << "		Format: 0x(24 downto 16 Row)(13 downto 8 Column)(7 downto 0 Charge)" << std::endl;
  std::cout << "	Where do you want to start in CHB" << std::endl;
  std::cout << "		Format: 0x(24 downto 16 Row)(13 downto 8 Column)(7 downto 0 Charge)" << std::endl;
  std::cout << "	What kind of AMC13 command do you want?" << std::endl;
  std::cout << "		1: Able to enter commands, Else: quits out of AMC13 every time" << std::endl;
  std::cout << "4. Increasing charge in each ROC" << std::endl;
  std::cout << "	How much do you want to increase the charge by?" << std::endl;
  std::cout << "5. Loop through adc values" << std::endl;
  std::cout << "	Which adc value do you want to start with? (8 bits)" << std::endl;
  std::cout << "	Which adc value do you want to end with? (8 bits) (Enter 0 to loop over all values)" << std::endl;
  std::cout << "  Increment adc value by how much?" << std::endl;  
  std::cout << "  How many hits do you want in CHA?" << std::endl;
  std::cout << "          Format: 0x(ROC7)(ROC6)(ROC5)(ROC4)(ROC3)(ROC2)(ROC1)(ROC0)" << std::endl;
  std::cout << "  How many hits do you want in CHB?" << std::endl;
  std::cout << "          Format: 0x(ROC7)(ROC6)(ROC5)(ROC4)(ROC3)(ROC2)(ROC1)(ROC0)" << std::endl; 
  std::cout << "6. Do You want a PKAM Reset?" << std::endl;
  std::cout << "  A value of 0x114000 will enable a reset and length 20" << std::endl;
  std::cout << "  A value of 0x014000 will disable" << std::endl;
  std::cout << "7. Do You want to set the ROC Clock?" << std::endl;
  std::cout << "  What value do you want to set it to? " << std::endl;
  std::cout << "  A vlue of 0xA0000000 will set the latency to 160" << std::endl;
  std::cout << "8. Check the Firmware date of each GLIB" << std::endl;
  //std::cout << "9. Does somethin?" << std::endl;  
  std::cout << "10. IPBus reset" << std::endl;
  std::cout << "  A value of 0x00 is disable and 0x10 is enable" << std::endl;
  std::cout << "11. Read the control register of the fitel config" << std::endl;
  return 0;
  }

  if( atoi(argv[1]) >= 12 ){
    std::cout << "NOT A VALID ENTRY" << std::endl;
    return 0;
  }

  if( atoi(argv[1]) == 10){
    Reset = strtol(argv[2], NULL, 16);

    hw.getNode("User_Reset_0").write( Reset );
    ValWord < uint32_t > mem = hw.getNode( "User_Reset_0" ).read();
    hw.dispatch();
    std::cout << "User_Reset_0 = " << std::hex << mem.value() << std::endl;
  }

  if( atoi(argv[1]) == 11){
    ValWord < uint32_t > mem = hw.getNode("user_ctrl_regs").read();
    hw.dispatch();
    std::cout << "user_ctrl_regs = " << std::hex << mem.value() << std::endl;
  }

  if (atoi(argv[1]) == 0){
    Number_Channels = strtol(argv[2], NULL, 16);

    if(Number_Channels == 0)
      Which_Chan = 286331153;
    else
      Which_Chan = Number_Channels;

    if( atoi(argv[3]) == 1 ){
      for (int i = 0; i < 8; i++){
	string ROC ("ROC_Number_");
        if((Number_Channels & 0x1) == 1){
          ROC += Chan[i];
          Number_ROCS = strtol(argv[4], NULL, 16);

          hw.getNode(ROC).write( Number_ROCS );
          ValWord < uint32_t > mem = hw.getNode (ROC).read();
          hw.dispatch();
          std::cout << ROC << " = " << std::hex << mem.value() << std::endl;

          hw2.getNode(ROC).write( Number_ROCS );
          ValWord < uint32_t > mem2 = hw2.getNode (ROC).read();
          hw2.dispatch();
          std::cout << ROC << " = " << std::hex << mem2.value() << std::endl;
        }
	Number_Channels = Number_Channels >> 4;  
      }
      return 0;
    }

    else if( atoi(argv[3]) == 2){
        Hits_CHA = strtol(argv[4], NULL, 16);
        Hits_CHB = strtol(argv[5], NULL, 16);
        ValWord < uint32_t > mem2;
        ValWord < uint32_t > mem;

        for (int i = 0; i < 8; i++){
            string CHA_Hits ("CHA_Hits_");
            string CHB_Hits ("CHB_Hits_");
	    if((Number_Channels & 0x1) == 1){
              CHA_Hits += Chan[i];
              CHB_Hits += Chan[i];

              hw.getNode(CHA_Hits).write( Hits_CHA );
              mem = hw.getNode (CHA_Hits).read();
              hw.dispatch();
              std::cout << CHA_Hits << " = " << std::hex << mem.value() << std::endl;

              hw.getNode(CHB_Hits).write( Hits_CHB );
              mem = hw.getNode (CHB_Hits).read();
              hw.dispatch();
              std::cout << CHB_Hits << " = " << std::hex << mem.value() << std::endl;

              hw2.getNode(CHA_Hits).write( Hits_CHA );
              mem2 = hw2.getNode (CHA_Hits).read();
              hw2.dispatch();
              std::cout << CHA_Hits << " = " << std::hex << mem2.value() << std::endl;
   
              hw2.getNode(CHB_Hits).write( Hits_CHB );
              mem2 = hw2.getNode (CHB_Hits).read();
              hw2.dispatch();
              std::cout << CHB_Hits << " = " << std::hex << mem2.value() << std::endl;
            }
	    Number_Channels = Number_Channels >> 4;
        }
    }

    else if( atoi(argv[3]) == 3 ){
      int j,k,i;
      int Max_Column = 0;
      int Max_Row = 0;
      int Hit_infoA_Start = 0;
      int Hit_infoB_Start = 0;
      int Hit_infoA_dummy = Hit_infoA_Start;
      int Hit_infoB_dummy = Hit_infoB_Start;
      string ROCs_in_A;
      string ROCs_in_B;
      ValWord < uint32_t > mem, mem2;

      Max_Column = strtol(argv[4], NULL, 16);
      Max_Row = strtol(argv[5], NULL, 16);
      Hit_infoA_Start = strtol(argv[6], NULL, 16);
      Hit_infoB_Start = strtol(argv[7], NULL, 16);

      for( j = 0; j < Max_Row; j++){
         int Hit_infoA = Hit_infoA_dummy;
         int Hit_infoB = Hit_infoB_dummy;

         for( k = 0; k < Max_Column; k++){
           for( i = 0; i < 8; i++){
      	     for (int p = 0; p < 8; p++){
        	if((Number_Channels & 0x1) == 1){
                	ROCs_in_A = ROCSA[i] + Chan[p];
                	ROCs_in_B = ROCSB[i] + Chan[p];

                	hw.getNode( ROCs_in_A ).write(Hit_infoA);
                	mem = hw.getNode (ROCs_in_A).read();
                	hw.dispatch();
                	std::cout << ROCs_in_A << " = " << std::hex << mem.value() << std::endl;

                	hw.getNode(ROCs_in_B).write(Hit_infoB);
                	mem = hw.getNode (ROCs_in_B).read();
                	hw.dispatch();
                	std::cout << ROCs_in_B << " = " << std::hex << mem.value() << std::endl;

                	hw2.getNode(ROCs_in_A).write(Hit_infoA);
                	mem2 = hw2.getNode (ROCs_in_A).read();
                	hw2.dispatch();
                	std::cout << ROCs_in_A << " = " << std::hex << mem2.value() << std::endl;

                	hw2.getNode(ROCs_in_B).write(Hit_infoB);
                	mem2 = hw2.getNode (ROCs_in_B).read();
                	hw2.dispatch();
                	std::cout << ROCs_in_B << " = " << std::hex << mem2.value() << std::endl;	
		}
		Number_Channels = Number_Channels >> 4;
	     }
	  }

        Hit_infoA = Hit_infoA + 0x00000100;
        Hit_infoB = Hit_infoB + 0x00000100;	

        if(strtol(argv[8], NULL, 16) == 1)
        	system("/home/fectest/FEDtester/MrPixel/Desktop/amc13/AMC13Pixel_Emul_command.bash");	
        else
                system("/home/fectest/FEDtester/MrPixel/Desktop/amc13/AMC13Pixel_Emul.bash");
        }    
        Hit_infoA_dummy = Hit_infoA_Start + (0x00010000 * (j + 1));
        Hit_infoB_dummy = Hit_infoB_Start + (0x00010000 * (j + 1));
            
     }
  }

    else if( atoi(argv[3]) == 4 ){
      int i;
      int Hit_infoA = 0x00010100;
      int Hit_infoB = 0x00010100;
      ValWord < uint32_t > mem,mem2;
      int Hit_Charge_Increase = 0;
      string ROCs_in_A;
      string ROCs_in_B;

      Hit_Charge_Increase = strtol(argv[4], NULL, 16);

      for (int p = 0; p < 8; p++){
        if((Number_Channels & 0x1) == 1){
          for( i = 0; i < 8; i++){
            ROCs_in_A = ROCSA[i] + Chan[p];
            ROCs_in_B = ROCSB[i] + Chan[p];

            hw.getNode( ROCs_in_A ).write(Hit_infoA);
            mem = hw.getNode (ROCs_in_A).read();
            hw.dispatch();
            std::cout << ROCs_in_A << " = " << std::hex << mem.value() << std::endl;

            hw.getNode(ROCs_in_B).write(Hit_infoB);
            mem = hw.getNode (ROCs_in_B).read();
            hw.dispatch();
            std::cout << ROCs_in_B << " = " << std::hex << mem.value() << std::endl;

            hw2.getNode(ROCs_in_A).write(Hit_infoA);
            mem2 = hw2.getNode (ROCs_in_A).read();
            hw2.dispatch();
            std::cout << ROCs_in_A << " = " << std::hex << mem2.value() << std::endl;

            hw2.getNode(ROCs_in_B).write(Hit_infoB);
            mem2 = hw2.getNode (ROCs_in_B).read();
            hw2.dispatch();
            std::cout << ROCs_in_B << " = " << std::hex << mem2.value() << std::endl;

            Hit_infoA = Hit_infoA + Hit_Charge_Increase;
            Hit_infoB = Hit_infoB + Hit_Charge_Increase;
          }
          system("/home/fectest/FEDtester/MrPixel/Desktop/amc13/AMC13Pixel_Emul.bash");
        }
	Number_Channels = Number_Channels >> 4;
      }
      return 0;
    }

    else if( atoi(argv[3]) == 5 ){
        Hits_CHA = strtol(argv[7], NULL, 16);
        Hits_CHB = strtol(argv[8], NULL, 16);
        ValWord < uint32_t > mem4;

        hw.getNode("CHA_Hits").write( Hits_CHA );
        ValWord < uint32_t > mem3 = hw.getNode( "CHA_Hits" ).read();
        hw.dispatch();
        std::cout << "CHA_HITS = " << std::hex << mem3.value() << std::endl;

        hw.getNode("CHB_Hits").write( Hits_CHB );
        mem3 = hw.getNode ( "CHB_Hits" ).read();
        hw.dispatch();
        std::cout << "CHB_Hits = " << std::hex << mem3.value() << std::endl;

        hw2.getNode("CHA_Hits").write( Hits_CHA );
        mem4 = hw2.getNode ( "CHA_Hits" ).read();
        hw2.dispatch();
        std::cout << "CHA_Hits = " << std::hex << mem4.value() << std::endl;

        hw2.getNode("CHB_Hits").write( Hits_CHB );
        mem4 = hw2.getNode ( "CHB_Hits" ).read();
        hw2.dispatch();
        std::cout << "CHB_Hits = " << std::hex << mem4.value() << std::endl;

        int i,j;
        int min_adc = 0;
        int max_adc = 0;
        ValWord < uint32_t > mem,mem2; 

        min_adc = strtol(argv[4], NULL, 16);

        if (min_adc < 0) {
          min_adc = 0;
        }
        else if (min_adc > 255) {
          min_adc = 255;
        }

        max_adc = strtol(argv[5], NULL, 16);

        if (max_adc < min_adc && max_adc != 0) {
          max_adc = min_adc;
          std::cout << "Max is less than min." << std::endl;
        }
        else if (max_adc > 255 || max_adc == 0) {
          max_adc = 255;
        }

        int hit_info = min_adc;
        int increment = strtol(argv[6],NULL,0);

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

        system("/home/fectest/FEDtester/MrPixel/Desktop/amc13/AMC13Pixel_Emul.bash");   
        }
    }

    else if( atoi(argv[3]) == 6 ){
      PKAM = strtol(argv[4], NULL, 16);

      for (int i = 0; i < 8; i++){
        string PKAM_Reset ("PKAM_Reset_");
	if((Number_Channels & 0x1) == 1){
          PKAM_Reset += Chan[i];

          hw.getNode(PKAM_Reset).write( PKAM );
          ValWord < uint32_t > mem = hw.getNode (PKAM_Reset).read();
          hw.dispatch();
          std::cout << PKAM_Reset << " = " << std::hex << mem.value() << std::endl;

          hw2.getNode(PKAM_Reset).write( PKAM );
          ValWord < uint32_t > mem2 = hw2.getNode (PKAM_Reset).read();
          hw2.dispatch();
          std::cout << PKAM_Reset << " = " << std::hex << mem2.value() << std::endl;
        }
	Number_Channels = Number_Channels >> 4;
      }

      return 0;
    }

    else if( atoi(argv[3]) == 7 ){
      ROC_Clk = strtol(argv[4], NULL, 16);

      for (int i = 0; i < 8; i++){
	string ROC_CLOCK ("ROC_CLK_");
        if((Number_Channels & 0x1) == 1){
          ROC_CLOCK += Chan[i];

          hw.getNode(ROC_CLOCK).write( ROC_Clk );
          ValWord < uint32_t > mem = hw.getNode (ROC_CLOCK).read();
          hw.dispatch();
          std::cout << "ROC Clock = " << std::hex << mem.value() << std::endl;

          hw2.getNode(ROC_CLOCK).write( ROC_Clk );
          ValWord < uint32_t > mem2 = hw2.getNode (ROC_CLOCK).read();
          hw2.dispatch();
          std::cout << ROC_CLOCK << " = " << std::hex << mem2.value() << std::endl;
        }
	Number_Channels = Number_Channels >> 4;
      }

      return 0;
    } 
  }     

  else if( atoi(argv[1]) == 8){
    ValWord < uint32_t > mem = hw.getNode ( "Firmware_date" ).read();
    hw.dispatch();
    std::cout << "Firmware Date is " << std::hex << mem.value() << std::endl;

    ValWord < uint32_t > mem2 = hw2.getNode ( "Firmware_date" ).read();
    hw2.dispatch();
    std::cout << "Firmware Date2 is " << std::hex << mem2.value() << std::endl;
  }

  return 0;
}
