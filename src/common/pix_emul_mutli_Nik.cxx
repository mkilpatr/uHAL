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
  int PKAM = 0;
  int ROC_Clk = 0;
  int Hits_CHA = 0;
  int Hits_CHB = 0;
  int Reset = 0;
  int fifo_tx = 0;
  int Number_Channels= 1;
  int Active_Channels = 0;
  string ROCSA[ 8 ] = {"CHA_ROC0_", "CHA_ROC1_", "CHA_ROC2_", "CHA_ROC3_", "CHA_ROC4_", "CHA_ROC5_", "CHA_ROC6_", "CHA_ROC7_"};
  string ROCSB[ 8 ] = {"CHB_ROC0_", "CHB_ROC1_", "CHB_ROC2_", "CHB_ROC3_", "CHB_ROC4_", "CHB_ROC5_", "CHB_ROC6_", "CHB_ROC7_"};
  string index[8] = {"0","1","2","3","4","5","6","7"};

  ConnectionManager manager ("file:/settings/GLIB_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
 
  if( argc == 1 ) {
  	std::cout << "What kind of Test?" << std::endl;
        std::cout << "First argument is always the number of channels" << std::endl;
        std::cout << "    Format: # of channels (int 1-8)" << std::endl;
	std::cout << "1. Set the number of ROCS in each channel" << std::endl;
  std::cout << "  How many active channels?" << std::endl;
  std::cout << "    Format: 0x(ch8)(ch7)(ch6)(ch5)(ch4)(ch3)(ch2)(ch1) with 0 = not active and 1 = active";
  	std::cout << "	How Many ROCS to you want?" << std::endl;
    	std::cout << "		Format: 0x(dataB)(dataA)(MatrixB)(MatrixA)(HitROCB)(HitROCA)(ROCSB)(ROCSA)" << std::endl;	
	std::cout << "2. Set the number of hits in each ROC" << std::endl;
  std::cout << "  How many active channels?" << std::endl;
  std::cout << "    Format: 0x(ch8)(ch7)(ch6)(ch5)(ch4)(ch3)(ch2)(ch1) with 0 = not active and 1 = active";
  	std::cout << "	How many hits do you want in CHA?" << std::endl;
    	std::cout << "		Format: 0x(ROC7)(ROC6)(ROC5)(ROC4)(ROC3)(ROC2)(ROC1)(ROC0)" << std::endl;
  	std::cout << "	How many hits do you want in CHB?" << std::endl;
    	std::cout << "		Format: 0x(ROC7)(ROC6)(ROC5)(ROC4)(ROC3)(ROC2)(ROC1)(ROC0)" << std::endl;
  	std::cout << "3. Scan through pixels" << std::endl;
    std::cout << "  How many active channels?" << std::endl;
    std::cout << "    Format: 0x(ch8)(ch7)(ch6)(ch5)(ch4)(ch3)(ch2)(ch1) with 0 = not active and 1 = active";
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
	std::cout << "9. Does somethin?" << std::endl;
	std::cout << "10. IPBus reset" << std::endl;
	std::cout << "  A value of 0x00 is disable and 0x10 is enable" << std::endl;
	std::cout << "11. Read the control register of the fitel config" << std::endl;
  std::cout << "12. Write a value to a specific ROC in a specific Channel." << std::endl;
  std::cout << "  Which Channel do you want to write to? (int 0-7)" << std::endl;
  std::cout << "  Which ROC Channel, CHA or CHB, do you want to write to? (int 0-1)" << std::endl;
  std::cout << "  Which specific ROC do you want to write to? (int 0-7)" << std::endl;
  std::cout << "  What pixel column, row, and adc value do you want to write?" << std::endl;
  std::cout << "    Format: 0x(0)(row2)(row1)(row0)(col1)(col0)(adc1)(adc0)" << std::endl;
  std::cout << "  How many hits do you want?" << std::endl;
  std::cout << "    Format: 0x(ROC7)(ROC6)(ROC5)(ROC4)(ROC3)(ROC2)(ROC1)(ROC0)" << std::endl;
	return 0;
  }

  if( atoi(argv[1]) >= 13 ){
    std::cout << "NOT A VALID ENTRY" << std::endl;
	return 0;
  }

  if( atoi(argv[1]) == 10){
	Reset = strtol(argv[2], NULL, 16);

	hw.getNode("User_Reset").write( Reset );
	ValWord < uint32_t > mem = hw.getNode( "User_Reset" ).read();
	hw.dispatch();
	std::cout << "User_Reset = " << std::hex << mem.value() << std::endl;
  }

  if( atoi(argv[1]) == 11){
	//fifo_tx = strtol(argv[2], NULL, 16);

	//hw.getNode("user_ctrl_regs").write( fifo_tx );
	ValWord < uint32_t > mem = hw.getNode("user_ctrl_regs").read();
	hw.dispatch();
	std::cout << "user_ctrl_regs = " << std::hex << mem.value() << std::endl;
  }

  Number_Channels = strtol(argv[1], NULL, 10);

  if( atoi(argv[2]) == 1 ){
	if(argc == 5){

  Active_Channels = strtol(argv[3], NULL, 2);
  Number_ROCS = strtol(argv[4], NULL, 16);
	
  for (int i=0; i<Number_Channels; i++){
    if (((Active_Channels >> i) & 1) == 1){

    std::string chan_num = index[i];
    std::string address = "ROC_Number_" + chan_num;

    	hw.getNode(address).write( Number_ROCS );
  	ValWord < uint32_t > mem = hw.getNode ( address ).read();
    	hw.dispatch();
    	std::cout << "ROC_Number_" << i << " = " << std::hex << mem.value() << std::endl;

  	hw2.getNode(address).write( Number_ROCS );
  	ValWord < uint32_t > mem2 = hw2.getNode ( address ).read();
    	hw2.dispatch();
    	std::cout << "ROC_Number_" << i << " = " << std::hex << mem2.value() << std::endl;
    }
  }
	
	return 0;
	}
	else {return 0;}
  }

  else if( atoi(argv[2]) == 2){
  	if(argc == 6){

  Active_Channels = strtol(argv[3], NULL, 2);
  Hits_CHA = strtol(argv[4], NULL, 16);
  Hits_CHB = strtol(argv[5], NULL, 16);
	
  for (int i=0; i < Number_Channels; i++){
    if (((Active_Channels >> i) & 0x1) == 1){

    std::string chan_num = index[i];
    std::string addressA = "CHA_Hits_" + chan_num;
    std::string addressB = "CHB_Hits_" + chan_num;

  	ValWord < uint32_t > mem2;

    	hw.getNode(addressA).write( Hits_CHA );
    	ValWord < uint32_t > mem = hw.getNode ( addressA ).read();
      	hw.dispatch();
      	std::cout << "CHA_Hits_" << i << " = " << std::hex << mem.value() << std::endl;
    	
      	hw.getNode(addressB).write( Hits_CHB );
    	mem = hw.getNode ( addressB ).read();
      	hw.dispatch();
      	std::cout << "CHB_Hits_" << i << " = " << std::hex << mem.value() << std::endl;

  	hw2.getNode(addressA).write( Hits_CHA );
    	mem2 = hw2.getNode ( addressA ).read();
      	hw2.dispatch();
      	std::cout << "CHA_Hits_" << i << " = " << std::hex << mem2.value() << std::endl;
    	
      	hw2.getNode(addressB).write( Hits_CHB );
    	mem2 = hw2.getNode ( addressB ).read();
      	hw2.dispatch();
      	std::cout << "CHB_Hits_" << i << " = " << std::hex << mem2.value() << std::endl;
      }
  }  

    }	
  }

  else if( atoi(argv[2]) == 9){
	int CHA_Event = strtol(argv[3], NULL, 16);
	int CHB_Event = strtol(argv[4], NULL, 16);

	int MUX = 0;
	
	int Buffer = 0x80000000;
	do{
		if( CHA_Event && Buffer == 1){
			
		}
	}
	while( Buffer != 0x1);

  }


  else if( atoi(argv[2]) == 3 ){

  Active_Channels = strtol(argv[3], NULL, 2);

  for (int chan = 0; chan < Number_Channels; chan++){
  if (((Active_Channels >> chan) & 0x1) == 1){
  	int j,k,i;
  	int Max_Column = 0;
  	int Max_Row = 0;
  	int Hit_infoA_Start = 0;
	int Hit_infoB_Start = 0;
  	ValWord < uint32_t > mem, mem2;
  	
  	Max_Column = strtol(argv[4], NULL, 16);
  	Max_Row = strtol(argv[5], NULL, 16);
  	Hit_infoA_Start = strtol(argv[6], NULL, 16);
	Hit_infoB_Start = strtol(argv[7], NULL, 16);

  	int Hit_infoA_dummy = Hit_infoA_Start;
	int Hit_infoB_dummy = Hit_infoB_Start;

  // ROCSA[ 8 ] = {"CHA_ROC0_", "CHA_ROC1_", "CHA_ROC2_", "CHA_ROC3_", "CHA_ROC4_", "CHA_ROC5_", "CHA_ROC6_", "CHA_ROC7_"};
  // ROCSB[ 8 ] = {"CHB_ROC0_", "CHB_ROC1_", "CHB_ROC2_", "CHB_ROC3_", "CHB_ROC4_", "CHB_ROC5_", "CHB_ROC6_", "CHB_ROC7_"};

  std::string chan_num = index[chan];
  for (int yes = 0; yes < 8; yes++){
    ROCSA[yes] = ROCSA[yes].substr(0,9);
    ROCSB[yes] = ROCSB[yes].substr(0,9);
    ROCSA[yes] = ROCSA[yes] + chan_num;
    ROCSB[yes] = ROCSB[yes] + chan_num;
  }

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

  			if(strtol(argv[8], NULL, 16) == 1){
  				system("/home/fectest/FEDtester/MrPixel/Desktop/amc13/AMC13Pixel_Emul_command.bash");	
  			}
  			else{
  				system("/home/fectest/FEDtester/MrPixel/Desktop/amc13/AMC13Pixel_Emul.bash");
  			}
  		}

  		Hit_infoA_dummy = Hit_infoA_Start + (0x00010000 * (j + 1));
  		Hit_infoB_dummy = Hit_infoB_Start + (0x00010000 * (j + 1));
  	}
  }
  }
  }

  else if( atoi(argv[2]) == 4 ){

  Active_Channels = strtol(argv[3], NULL, 2);

  for (int chan = 0; chan < Number_Channels; chan++){
  if (((Active_Channels >> chan) & 0x1) == 1){

  	int i;
  	int Hit_infoA = 0x00010100;
  	int Hit_infoB = 0x00010100;
  	ValWord < uint32_t > mem,mem2;
  	int Hit_Charge_Increase = 0;
  	
  	Hit_Charge_Increase = strtol(argv[4], NULL, 16);

    // ROCSA[ 8 ] = {"CHA_ROC0_", "CHA_ROC1_", "CHA_ROC2_", "CHA_ROC3_", "CHA_ROC4_", "CHA_ROC5_", "CHA_ROC6_", "CHA_ROC7_"};
    // ROCSB[ 8 ] = {"CHB_ROC0_", "CHB_ROC1_", "CHB_ROC2_", "CHB_ROC3_", "CHB_ROC4_", "CHB_ROC5_", "CHB_ROC6_", "CHB_ROC7_"};

    std::string chan_num = index[chan];
    for (int yes = 0; yes < 8; yes++){
      ROCSA[yes] = ROCSA[yes].substr(0,9);
      ROCSB[yes] = ROCSB[yes].substr(0,9);
      ROCSA[yes] = ROCSA[yes] + chan_num;
      ROCSB[yes] = ROCSB[yes] + chan_num;
    }

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

  		Hit_infoA = Hit_infoA + Hit_Charge_Increase;
  		Hit_infoB = Hit_infoB + Hit_Charge_Increase;
  	}
  	system("/home/fectest/FEDtester/MrPixel/Desktop/amc13/AMC13Pixel_Emul.bash");
  
  }
  }
    return 0;
  }

  else if( atoi(argv[2]) == 5 ){
  if(argc == 9 ) {  

  Active_Channels = strtol(argv[3], NULL, 2);  

  for (int chan = 0; chan < Number_Channels; chan++){
  if (((Active_Channels >> chan) & 0x1) == 1){

   std::string chan_num = index[chan];
   std::string addressA = "CHA_Hits_" + chan_num;
   std::string addressB = "CHB_Hits_" + chan_num;

   Hits_CHA = strtol(argv[7], NULL, 16);
   Hits_CHB = strtol(argv[8], NULL, 16);
   ValWord < uint32_t > mem4;

   hw.getNode(addressA).write( Hits_CHA );
   ValWord < uint32_t > mem3 = hw.getNode( addressA ).read();
   hw.dispatch();
   std::cout << addressA << " = " << std::hex << mem3.value() << std::endl;

   hw.getNode(addressB).write( Hits_CHB );
   mem3 = hw.getNode ( addressB ).read();
   hw.dispatch();
   std::cout << addressB << " = " << std::hex << mem3.value() << std::endl;

   hw2.getNode(addressA).write( Hits_CHA );
   mem4 = hw2.getNode ( addressA ).read();
   hw2.dispatch();
   std::cout << addressA << " = " << std::hex << mem4.value() << std::endl;

   hw2.getNode(addressB).write( Hits_CHB );
   mem4 = hw2.getNode ( addressB ).read();
   hw2.dispatch();
   std::cout << addressB << " = " << std::hex << mem4.value() << std::endl;

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

   // ROCSA[ 8 ] = {"CHA_ROC0_", "CHA_ROC1_", "CHA_ROC2_", "CHA_ROC3_", "CHA_ROC4_", "CHA_ROC5_", "CHA_ROC6_", "CHA_ROC7_"};
   // ROCSB[ 8 ] = {"CHB_ROC0_", "CHB_ROC1_", "CHB_ROC2_", "CHB_ROC3_", "CHB_ROC4_", "CHB_ROC5_", "CHB_ROC6_", "CHB_ROC7_"};

   for (int yes = 0; yes < 8; yes++){
      ROCSA[yes] = ROCSA[yes].substr(0,9);
      ROCSB[yes] = ROCSB[yes].substr(0,9);
      ROCSA[yes] = ROCSA[yes] + chan_num;
      ROCSB[yes] = ROCSB[yes] + chan_num;
   }

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
  }
 }
 }

  else if( atoi(argv[2]) == 6 ){
    if(argc == 4){
      PKAM = strtol(argv[3], NULL, 16);

      hw.getNode("PKAM_Reset").write( PKAM );
      ValWord < uint32_t > mem = hw.getNode ( "PKAM_Reset" ).read();
      hw.dispatch();
      std::cout << "PKAM Reset = " << std::hex << mem.value() << std::endl;

      hw2.getNode("PKAM_Reset").write( PKAM );
      ValWord < uint32_t > mem2 = hw2.getNode ( "PKAM_Reset" ).read();
      hw2.dispatch();
      std::cout << "PKAM Reset = " << std::hex << mem2.value() << std::endl;
      

      return 0;
    }
    else {return 0;}
  }

  else if( atoi(argv[2]) == 7 ){
  	if(argc == 4){
  	  ROC_Clk = strtol(argv[3], NULL, 16);

  	  hw.getNode("ROC_CLK").write( ROC_Clk );
  	  ValWord < uint32_t > mem = hw.getNode ( "ROC_CLK" ).read();
  	  hw.dispatch();
  	  std::cout << "ROC Clock = " << std::hex << mem.value() << std::endl;

  	  hw2.getNode("ROC_CLK").write( ROC_Clk );
  	  ValWord < uint32_t > mem2 = hw2.getNode ( "ROC_CLK" ).read();
  	  hw2.dispatch();
  	  std::cout << "ROC Clock = " << std::hex << mem2.value() << std::endl;

  	  return 0;
  	}
  	else {return 0;}
  }

  else if( atoi(argv[2]) == 8){
	
	ValWord < uint32_t > mem = hw.getNode ( "Firmware_date" ).read();
	hw.dispatch();
	std::cout << "Firmware Date is " << std::hex << mem.value() << std::endl;

	ValWord < uint32_t > mem2 = hw2.getNode ( "Firmware_date" ).read();
        hw2.dispatch();
        std::cout << "Firmware Date2 is " << std::hex << mem2.value() << std::endl;
}

else if( atoi(argv[2]) == 12){
  if (argc = 8){
    Active_Channels = strtol(argv[3], NULL, 10);
    int Active_Half = strtol(argv[4], NULL, 10);
    int Active_ROC = strtol(argv[5], NULL, 10);
    int Active_Hit_Info = strtol(argv[6], NULL, 16);
    int Active_Hits = strtol(argv[7], NULL, 16);

    int hit_info;

    for (int chan = 0; chan < Number_Channels; chan++){
      if (chan == Active_Channels){
        if (Active_Half == 0){
          for (int roc_count = 0; roc_count < 8; roc_count++){
            if (roc_count == Active_ROC){
              std::string chan_num = index[chan];
              std::string addressA = "CHA_Hits_" + chan_num;

              ValWord < uint32_t > mem4;

              hw.getNode(addressA).write( Active_Hits );
              ValWord < uint32_t > mem3 = hw.getNode( addressA ).read();
              hw.dispatch();
              std::cout << addressA << " = " << std::hex << mem3.value() << std::endl;

              hw2.getNode(addressA).write( Active_Hits );
              mem4 = hw2.getNode ( addressA ).read();
              hw2.dispatch();
              std::cout << addressA << " = " << std::hex << mem4.value() << std::endl;

              ValWord < uint32_t > mem,mem2; 

              for (int yes = 0; yes < 8; yes++){
                ROCSA[yes] = ROCSA[yes].substr(0,9);
                ROCSA[yes] = ROCSA[yes] + chan_num;
              }

              hit_info = Active_Hit_Info;

              std::cout << ROCSA[roc_count] << std::endl;

              hw.getNode( ROCSA[roc_count] ).write(hit_info);
              mem = hw.getNode ( ROCSA[roc_count] ).read();
              hw.dispatch();
              std::cout << ROCSA[roc_count] << " = " << std::hex << mem.value() << std::endl;

              hw2.getNode( ROCSA[roc_count] ).write(hit_info);
              mem2 = hw2.getNode ( ROCSA[roc_count] ).read();
              hw2.dispatch();
              std::cout << ROCSA[roc_count] << " = " << std::hex << mem2.value() << std::endl;

            }
          }
        }
        else if (Active_Half == 1){
          for (int roc_count = 0; roc_count < 8; roc_count++){
            if (roc_count == Active_ROC){
              std::string chan_num = index[chan];
              std::string addressB = "CHB_Hits_" + chan_num;

              ValWord < uint32_t > mem4;

              hw.getNode(addressB).write( Active_Hits );
              ValWord < uint32_t > mem3 = hw.getNode( addressB ).read();
              hw.dispatch();
              std::cout << addressB << " = " << std::hex << mem3.value() << std::endl;

              hw2.getNode(addressB).write( Active_Hits );
              mem4 = hw2.getNode ( addressB ).read();
              hw2.dispatch();
              std::cout << addressB << " = " << std::hex << mem4.value() << std::endl;

              ValWord < uint32_t > mem,mem2; 

              for (int yes = 0; yes < 8; yes++){
                ROCSB[yes] = ROCSB[yes].substr(0,9);
                ROCSB[yes] = ROCSB[yes] + chan_num;
              }

              hit_info = Active_Hit_Info;

              hw.getNode( ROCSB[roc_count] ).write(hit_info);
              mem = hw.getNode ( ROCSB[roc_count] ).read();
              hw.dispatch();
              std::cout << ROCSB[roc_count] << " = " << std::hex << mem.value() << std::endl;

              hw2.getNode( ROCSB[roc_count] ).write(hit_info);
              mem2 = hw2.getNode ( ROCSB[roc_count] ).read();
              hw2.dispatch();
              std::cout << ROCSB[roc_count] << " = " << std::hex << mem2.value() << std::endl;
            }
          }
        }
      }
    }

  }
}

return 0;
}
