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
  int Number_ROCS = 0;
  int Hits_CHA = 0;
  int Hits_CHB = 0;
  string ROCSA[ 8 ] = {"CHA_ROC0", "CHA_ROC1", "CHA_ROC2", "CHA_ROC3", "CHA_ROC4", "CHA_ROC5", "CHA_ROC6", "CHA_ROC7"};
  string ROCSB[ 8 ] = {"CHB_ROC0", "CHB_ROC1", "CHB_ROC2", "CHB_ROC3", "CHB_ROC4", "CHB_ROC5", "CHB_ROC6", "CHB_ROC7"};

  ConnectionManager manager ("file://test/dummy_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_11" );
 
  if( argc == 1 ) {
	std::cout << "What kind of Test?" << std::endl;
        std::cout << "1. Set the number of ROCS" << std::endl;
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
	std::cout << "5. Scan through pixels in a specific ROC in a specific Channel" << std::endl;
	std::cout << "	Number of Pixel Columns? (6 bits)" << std::endl;
        std::cout << "	Number of Pixel Rows? (9 bits)" << std::endl;
        std::cout << "	Where do you want to start in CHA" << std::endl;
        std::cout << "		Format: 0x(24 downto 16 Row)(13 downto 8 Column)(7 downto 0 Charge)" << std::endl;
        std::cout << "	Where do you want to start in CHB" << std::endl;
        std::cout << "		Format: 0x(24 downto 16 Row)(13 downto 8 Column)(7 downto 0 Charge)" << std::endl;
	std::cout << "	Which ROC in CHA do you want to scan through?" << std::endl;
	std::cout << "	Which ROC in CHB do you want to scan through?" << std::endl;
	return 0;
  }

  if( atoi(argv[1]) >= 6 ){
	std::cout << "NOT A VALID ENTRY" << std::endl;
	return 0;
  }

  if( atoi(argv[1]) == 1 ){
	if(argc == 3){
		Number_ROCS = strtol(argv[2], NULL, 16);

  		hw.getNode("ROC_Number").write( Number_ROCS );
		ValWord < uint32_t > mem = hw.getNode ( "ROC_Number" ).read();
  		hw.dispatch();
  		std::cout << "ROC_Number = " << std::hex << mem.value() << std::endl;
		return 0;
	}
	else {return 0;}
  }

  else if( atoi(argv[1]) == 2){
	if(argc == 4){
		Hits_CHA = strtol(argv[2], NULL, 16);
        	Hits_CHB = strtol(argv[3], NULL, 16);

		hw.getNode("CHA_Hits").write( Hits_CHA );
		ValWord < uint32_t > mem = hw.getNode ( "CHA_Hits" ).read();
  		hw.dispatch();
  		std::cout << "CHA_Hits = " << std::hex << mem.value() << std::endl;
	
  		hw.getNode("CHB_Hits").write( Hits_CHB );
		mem = hw.getNode ( "CHB_Hits" ).read();
  		hw.dispatch();
  		std::cout << "CHB_Hits = " << std::hex << mem.value() << std::endl;
  	}	
  }
  else if( atoi(argv[1]) == 3 ){
	int j,k,i;
	int Max_Column = 0;
	int Max_Row = 0;
	int Hit_infoA_Start = 0;
        int Hit_infoB_Start = 0;
	ValWord < uint32_t > mem;
	
	Max_Column = strtol(argv[2], NULL, 16);
	Max_Row = strtol(argv[3], NULL, 16);
	Hit_infoA_Start = strtol(argv[4], NULL, 16);
        Hit_infoB_Start = strtol(argv[5], NULL, 16);

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
			}	
			Hit_infoA = Hit_infoA + 0x00000100;
			Hit_infoB = Hit_infoB + 0x00000100;	
			if(strtol(argv[6], NULL, 16) == 1){
				system("/home/MrPixel/Desktop/amc13_v1_1_0/AMC13Pixel_Emul_command.bash");	
			}
			else{
				system("/home/MrPixel/Desktop/amc13_v1_1_0/AMC13Pixel_Emul.bash");
			}
		}
		Hit_infoA_dummy = Hit_infoA_Start + (0x00010000 * (j + 1));
		Hit_infoB_dummy = Hit_infoB_Start + (0x00010000 * (j + 1));
	}
  }

  else if( atoi(argv[1]) == 4 ){
  	int i;
	int Hit_infoA = 0x00010100;
	int Hit_infoB = 0x00010100;
	ValWord < uint32_t > mem;
	int Hit_Charge_Increase = 0;
	
	Hit_Charge_Increase = strtol(argv[2], NULL, 16);

	for( i = 0; i < 8; i++){
		hw.getNode( ROCSA[i] ).write(Hit_infoA);
                mem = hw.getNode ( ROCSA[i] ).read();
                hw.dispatch();
                std::cout << ROCSA[i] << " = " << std::hex << mem.value() << std::endl;

		hw.getNode( ROCSB[i] ).write(Hit_infoB);
                mem = hw.getNode ( ROCSB[i] ).read();
                hw.dispatch();
                std::cout << ROCSB[i] << " = " << std::hex << mem.value() << std::endl;
		
		Hit_infoA = Hit_infoA + Hit_Charge_Increase;
		Hit_infoB = Hit_infoB + Hit_Charge_Increase;
	}
	system("/home/MrPixel/Desktop/amc13_v1_1_0/AMC13Pixel_Emul.bash");
  }

  else if( atoi(argv[1]) == 5 ){
	int j,k;
        int Max_Column = 0;
        int Max_Row = 0;
        int Hit_infoA_Start = 0;
        int Hit_infoB_Start = 0;
	int ROC_A = 0;
	int ROC_B = 0;
        ValWord < uint32_t > mem;

        Max_Column = strtol(argv[2], NULL, 16);
        Max_Row = strtol(argv[3], NULL, 16);
        Hit_infoA_Start = strtol(argv[4], NULL, 16);
        Hit_infoB_Start = strtol(argv[5], NULL, 16);
	ROC_A = strtol(argv[6], NULL, 16);
        ROC_B = strtol(argv[7], NULL, 16);

        int Hit_infoA_dummy = Hit_infoA_Start;
        int Hit_infoB_dummy = Hit_infoB_Start;

	for( j = 0; j < Max_Row; j++){
                int Hit_infoA = Hit_infoA_dummy;
                int Hit_infoB = Hit_infoB_dummy;

                for( k = 0; k < Max_Column; k++){

                        hw.getNode( ROCSA[ ROC_A ] ).write(Hit_infoA);
                        mem = hw.getNode ( ROCSA[ ROC_A ] ).read();
                        hw.dispatch();
                        std::cout << ROCSA[ ROC_A ] << " = " << std::hex << mem.value() << std::endl;

                        hw.getNode( ROCSB[ ROC_B ] ).write(Hit_infoB);
                        mem = hw.getNode ( ROCSB[ROC_B ] ).read();
                        hw.dispatch();
                        std::cout << ROCSB[ ROC_B ] << " = " << std::hex << mem.value() << std::endl;
                        Hit_infoA = Hit_infoA + 0x00000100;
                        Hit_infoB = Hit_infoB + 0x00000100;
                        system("/home/MrPixel/Desktop/amc13_v1_1_0/AMC13Pixel_Emul.bash");
                }
                Hit_infoA_dummy = Hit_infoA_Start + (0x00010000 * (j + 1));
                Hit_infoB_dummy = Hit_infoB_Start + (0x00010000 * (j + 1));
        }

  }
  else{
	return 0;
  }

return 0;
}
