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
#include "Gray.c"
#include "Event_Analysis.c"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <typeinfo>
#include <list>
#include <stdlib.h>
#include <iterator>

void Delay_Time( int Delay_init );
void Test_Timeouts( int Delay1, int Delay2, int Loops);

//Create a pixel alive plot with DDR data and add it to the root file for analysis
void Pixel_Alive_Plots_DDR(int DCOL_DDR[], int PXL_DDR[]){

        TFile * fout;
        fout = new TFile("Pixel_Alive.root", "update");
        TH2F *h5 = new TH2F("h5","DCOL vs PXL DDR",29,-0.5,28.5,180,-0.5,179.5);
        h5->SetDirectory(fout);
        h5->GetXaxis()->SetTitle("DCOL");
        h5->GetYaxis()->SetTitle("PXL");

        for(Int_t i = 0; i < 4160; i++){
                h5->Fill(DCOL_DDR[ i ], PXL_DDR[ i ]);
        }

        TCanvas * c5 = new TCanvas("c5","Pixel Alive",200,10,700,500);
        h5->Draw("COLZ");
        h5->Write();
        //h4->SaveAs("Pixel_Alive_DDR.pdf","pdf");
}
       
//Used to change a header of trailer to any ouput desired by the user (12 bits)
//Can send it at any 8 bit frequency and to any channel '0' is all channels
void Marker_Error( int Which_Chan, int Marker_Type, int Marker_Value, int Marker_Rate){
	using namespace uhal;
        using namespace std;

        ConnectionManager manager ("file://test/dummy_connections_multi_chan.xml");
        HwInterface hw=manager.getDevice ( "GLIB.crate.slot_3" );   //this is to glib with the optical card
        HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card
        HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_9" );  //this is the glib with the debug card

        int Marker_Init = (Marker_Type << 30) | (Marker_Type << 28) | (Marker_Value << 16) | (Marker_Rate << 8) | (Marker_Rate);
        int Marker_Init_CHB = Marker_Value;
        int Number_Channels = 0;
        string Chan[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};

        if(Which_Chan == 0)
                Number_Channels = 0x11111111;
        else
                Number_Channels = Which_Chan;
        for (int i = 0; i < 8; i++){
                string Marker_Error ("Marker_Error_");
                string Marker_CHB ("Marker_Error_CHB_");
                if((Number_Channels & 0x1) == 1){
                        Marker_Error += Chan[i];
                        Marker_CHB += Chan[i];

                        hw.getNode(Marker_Error).write( Marker_Init );
                        ValWord < uint32_t > mem = hw.getNode( Marker_Error ).read();
                        hw.dispatch();

                        hw2.getNode(Marker_Error).write( Marker_Init );
                        ValWord < uint32_t > mem2 = hw2.getNode( Marker_Error ).read();
                        hw2.dispatch();

                        hw3.getNode(Marker_Error).write( Marker_Init );
                        ValWord < uint32_t > mem3 = hw3.getNode( Marker_Error ).read();
                        hw3.dispatch();

                        hw.getNode(Marker_CHB).write( Marker_Init_CHB );
                        mem = hw.getNode( Marker_CHB ).read();
                        hw.dispatch();

                        hw2.getNode(Marker_CHB).write( Marker_Init_CHB );
                        mem2 = hw2.getNode( Marker_CHB ).read();
                        hw2.dispatch();
                        
			hw3.getNode(Marker_CHB).write( Marker_Init_CHB );
                        mem3 = hw3.getNode( Marker_CHB ).read();
                        hw3.dispatch();
                }
                Number_Channels = Number_Channels >> 4;
	}
}

//Able to change the delay time of the output to cause timeouts
//Currently the max is a delay of 255*clock
void Delay_Time( int Delay_init ){

	using namespace uhal;
   	ConnectionManager manager ("file://test/dummy_connections_multi_chan.xml");
  	HwInterface hw=manager.getDevice ( "GLIB.crate.slot_3" );   //this is to glib with the optical card
  	HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card
  	HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_9" );  //this is the glib with the debug card
	
	int Delay_Reset = 0x200;
	
	hw.getNode("Delay_Signal").write( Delay_Reset );
    	ValWord < uint32_t > mem = hw.getNode ( "Delay_Signal" ).read();
    	hw.dispatch();

	hw2.getNode("Delay_Signal").write( Delay_Reset );
        ValWord < uint32_t > mem2 = hw2.getNode ( "Delay_Signal" ).read();
        hw2.dispatch();
	
	hw3.getNode("Delay_Signal").write( Delay_Reset );
        ValWord < uint32_t > mem3 = hw3.getNode ( "Delay_Signal" ).read();
        hw3.dispatch();
	
	hw.getNode("Delay_Signal").write( Delay_init );
        mem = hw.getNode ( "Delay_Signal" ).read();
        hw.dispatch();
        std::cout << "Delay Signal = " << std::hex << mem.value() << std::endl;

        hw2.getNode("Delay_Signal").write( Delay_init );
        mem2 = hw2.getNode ( "Delay_Signal" ).read();
        hw2.dispatch();
        
	hw3.getNode("Delay_Signal").write( Delay_init );
        mem3 = hw3.getNode ( "Delay_Signal" ).read();
        hw3.dispatch();

}

//Test resets on the DDR and Delay studies varying delays or incorrect markers
void Test_Timeouts( int Delay1, int Delay2, int Loops, int Choice_Input){


  using namespace std;

  const char* cHWFile;

  if(Choice_Input == 0){
  	std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_Delay.xml");
  	cHWFile = filename.c_str();
  }
  else if(Choice_Input == 1){
  	std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_Resets.xml");
  	cHWFile = filename.c_str();
  }
  else if(Choice_Input == 3){
  	std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_PKAM.xml");
  	cHWFile = filename.c_str();
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
    
    auto cSetting = cSystemController.fSettingsMap.find("NAcq");
    int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
    cSetting = cSystemController.fSettingsMap.find("BlockSize");
    int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;
    
    cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
    int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
    
    cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
    int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
    
    cAmc13Controller.ConfigureAmc13( std::cout );
    cSystemController.ConfigureHw(std::cout );
    
    std::cout << "Configure OK" << std::endl;

  // Can loop over all FEDs since we aren't writing to GLIB registers within the program
  // get the board info of all boards and start the acquistion
  for (auto& cFED : cSystemController.fPixFEDVector)
  {
    //auto& cFED = cSystemController.fPixFEDVector[0];
    cSystemController.fFEDInterface->getBoardInfo(cFED);
    cSystemController.fFEDInterface->findPhases(cFED);
    cSystemController.fFEDInterface->Start (cFED);
  }

  std::vector<uint32_t> DDR;
  std::vector<unsigned int> DDR_Analyze;
  std::fill(DDR.begin(), DDR.end(), 0);
  int Compare_Event_DDR = 0;
  int Total_Events_DDR = 0;
  int Incorrect_Event_Num_DDR = 0;
  int Timeout_Error_DDR = 0;
  int pixel_hit_DDR = 0;

  std::cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << std::endl;
  cAmc13Controller.fAmc13Interface->SendEC0();
  cAmc13Controller.fAmc13Interface->EnableBGO(0);
  cAmc13Controller.fAmc13Interface->DisableBGO(0);

  for (int l = 0; l < Loops; l++){

    if( l == 1 && Choice_Input == 1){
	cAmc13Controller.fAmc13Interface->EnableBGO(1);
  	cAmc13Controller.fAmc13Interface->DisableBGO(1);
    }

    for (auto& cFED : cSystemController.fPixFEDVector){

      if(l == 0){
  	cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
        if(Choice_Input == 0)
		Delay_Time(Delay1);
        sleep(4);
        cAmc13Controller.fAmc13Interface->BurstL1A();
      }
      else if(l == 1){
        if(Choice_Input == 0)
		Delay_Time(Delay2);
        sleep(0.04);
        cAmc13Controller.fAmc13Interface->BurstL1A();
      }

      else{
	if(Choice_Input == 0)
        	Delay_Time(Delay1);
        sleep(0.04);
        cAmc13Controller.fAmc13Interface->BurstL1A();
      }

      sleep(0.001);

      cSystemController.fFEDInterface->readSpyFIFO(cFED);
      cSystemController.fFEDInterface->readErrorFIFO(cFED, true);
      cSystemController.fFEDInterface->readTTSState(cFED);
      DDR = cSystemController.fFEDInterface->ReadData(cFED, 0 );

    }

    DDR_Analyze = DDR_Event(DDR, Compare_Event_DDR);
    Compare_Event_DDR = DDR_Analyze[0];
    Total_Events_DDR += DDR_Analyze[1];
    Incorrect_Event_Num_DDR += DDR_Analyze[2];
    Timeout_Error_DDR += DDR_Analyze[3];
    pixel_hit_DDR += DDR_Analyze[4];
  }
  
  std::cout << "There were " << Total_Events_DDR << " Events in DDR. " << std::endl;
  std::cout << "There were " << Incorrect_Event_Num_DDR << " Incorrect Event numbers." << std::endl;
  std::cout << "There were " << Timeout_Error_DDR << " timeout errors." << std::endl;
  std::cout << "There were " << pixel_hit_DDR << " pixel hits." << std::endl;
  std::cout << " " << std::endl; 

}

//Pixel alive for DDR sending the DCOL/PXL information
//Currently gets caught in infinite loop cause FED missed trigger or not full register
void Test_Hits_Full_DDR(int loops_input, int col_start, int col_input, int row_start, int row_input, int adc_start, int adc_input, int choice_input)
  {

  using namespace std;

  const char* cHWFile;

  std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_DDR.xml");
  cHWFile = filename.c_str();

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

  auto cSetting = cSystemController.fSettingsMap.find("NAcq");
  int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
  cSetting = cSystemController.fSettingsMap.find("BlockSize");
  int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;

  cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
  int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;

  cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
  int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;

  cAmc13Controller.ConfigureAmc13( std::cout );
  cSystemController.ConfigureHw(std::cout );

  std::cout << "Configure OK" << std::endl;

  //Need multiple FED ID's since we are writing to GLIB registers in the propgram
  auto& cFED = cSystemController.fPixFEDVector[0];
  cSystemController.fFEDInterface->getBoardInfo(cFED);
  cSystemController.fFEDInterface->findPhases(cFED);
  cSystemController.fFEDInterface->Start (cFED);

  auto& cFED2 = cSystemController.fPixFEDVector[1];
  auto& cFED3 = cSystemController.fPixFEDVector[2];
  auto& cFED4 = cSystemController.fPixFEDVector[3];

  std::vector<uint32_t> DDR;
  int Column[ 4199 ] = { NULL };
  int Row[ 4199 ] = { NULL };
  //int Spy_Col[ 4199 ] = { NULL };
  //int Spy_Row[ 4199 ] = { NULL };
  int DDR_DCOL[ 4199 ] = { NULL };
  int DDR_PXL[ 4199 ] = { NULL };
  int Max_Array = 0;
  int Compare_DCOL = 0;
  int Compare_PXL = 0;
  int Total_Events = 0;
  int Incorrect_Header = 0;
  int Incorrect_Trailer = 0;
  int Incorrect_Event_Num = 0;
  int Incorrect_ROC = 0;
  int ROC_Value = 0;
  int Stack = 0;
  uint32_t Current_Event_Num = 0;
  int Error_Count = 0;
  int Do_Error = 0;
  int Pixel = 0;
  int Pixel_Hit = 0;
  int Roc_num = 0;
  int Incorrect_Pixel = 0;
  int Compare_Event = 0;
  int new_row = 0;
  int Hit_infoA_buff = 0;
  int Hit_infoB_buff = 0;
  int Hit_infoA_new = 0;
  int Hit_infoB_new = 0;
  int Channel_Number = 0;
  int ROC_Identifier_1 = 0;
  int ROC_Identifier_2 = 0;
  int Timeout_Error = 0;
  int DDR_Event_Num_Error = 0;
  int pixel_hit = 0;
  int DCOL[ 4199 ] = { NULL };
  int PXL[ 4199 ] = { NULL };

  std::fill(DDR.begin(), DDR.end(), 0);

  std::cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << std::endl;
  cAmc13Controller.fAmc13Interface->SendEC0();
  cAmc13Controller.fAmc13Interface->EnableBGO(1);

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

  for (int l = 0; l < loops_input; l++){

    int Hit_infoA = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);
    int Hit_infoB = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);

    int row = 0;
    int col = 0;
    int dcol = 0;
    int adc = 0;

    for(int row = row_start; row < max_row; row++){

      for(int col = col_start; col < max_col; col++){
	
	//Changing COL to DCOL
        if(col % 2 == 0)
          dcol = col/2;

          std::cout << "Starting row " << std::dec << row << ", column " << std::dec << col << std::endl;

          for (adc = adc_start; adc < 8/*max_adc*/; adc+=8) {

            for (int j = 0; j < 8; j++) {

              for (int i = 0; i < 1; i++) {
                //Write_ROCs( i, Hit_infoA, Hit_infoB);
                ROCSA[j] = ROCSA[j] + index[i];
                ROCSB[j] = ROCSB[j] + index[i];

		//Here the loop infor is changed to the base 6 format that is required by the FED
                Hit_infoA_buff = Illegal_Col(dcol);
                Hit_infoB_buff = Illegal_Col(dcol);
                new_row = New_Row(row, col);

                Hit_infoA_new = (new_row << 16) | (Hit_infoA_buff << 8) | (Hit_infoA & 0xFF);
                Hit_infoB_new = (new_row << 16) | (Hit_infoB_buff << 8) | (Hit_infoB & 0xFF);

                cSystemController.fFEDInterface->WriteBoardReg(cFED2, ROCSA[j], Hit_infoA_new);
                cSystemController.fFEDInterface->WriteBoardReg(cFED2, ROCSB[j], Hit_infoB_new);

                cSystemController.fFEDInterface->WriteBoardReg(cFED3, ROCSA[j], Hit_infoA_new);
                cSystemController.fFEDInterface->WriteBoardReg(cFED3, ROCSB[j], Hit_infoB_new);

		cSystemController.fFEDInterface->WriteBoardReg(cFED4, ROCSA[j], Hit_infoA_new);
                cSystemController.fFEDInterface->WriteBoardReg(cFED4, ROCSB[j], Hit_infoB_new);

                ROCSA[j] = ROCSA[j].substr(0,9);
                ROCSB[j] = ROCSB[j].substr(0,9);

              }
              Hit_infoA = Hit_infoA + 0x00000001;
              Hit_infoB = Hit_infoB + 0x00000001;

              adc_loops++;
          }


          if(l == 0 && row == 0 && col == 0 && adc == 0){
             cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
             sleep(8);
             cAmc13Controller.fAmc13Interface->BurstL1A();
          }

          else{
             cAmc13Controller.fAmc13Interface->BurstL1A();
          }

          sleep(0.001);
          DDR = cSystemController.fFEDInterface->ReadData(cFED, 0 );

          int n = (row*52) + col;

          for ( uint32_t i = 0; i != DDR.size(); i++){
            //std::cout << "DDR[" << std::dec << i << "] = " << std::hex << DDR[i] << " " << DDR[ i + 1 ] << std::endl;

            if((DDR[i] >> 28) == 0x5){
              //Header
              //std::cout << BOLDGREEN << "Evt. ty " << ( (cWord >> 56) & 0xF ) << " L1A Id " << ( (cWord >> 32) & 0xFFFFFF) << " BX Id " << ( (cWord >> 20) & 0xFFF ) << " Source Id " << ( (cWord >> 8) & 0xFFF) << " FOV " << ( (cWord >> 4) & 0xF) << RESET << std::endl;
              Total_Events++;
              Current_Event_Num = ((DDR[i]) & 0xFFFFFF);
              if(Current_Event_Num - Compare_Event != 1 && Compare_Event - Current_Event_Num != 255){
                Incorrect_Event_Num++;
                Error_Count++;
              }
              Compare_Event = Current_Event_Num;    
            }

            else if((DDR[i] >> 28) == 0xa){
              //Trailer
              //std::cout << BOLDRED << "Evt. Length " << ( (cWord >> 32) & 0xFFFFFF ) << " CRC " << ( (cWord >> 16) & 0xFFFF) << RESET << std::endl;
            }
            else{
              //std::cout << "Channel " << ( (cWord1 >> 26) & 0x3F) << " ROC " <<  ( (cWord1 >> 21) & 0x1F) << " DC " << ( (cWord1 >> 16) & 0x1F) << " Pxl " << ( (cWord1 >> 8) & 0xFF) << " PH " << (cWord1 & 0xFF) << std::endl;
              //std::cout << "Channel " << ( (cWord2 >> 26) & 0x3F) << " ROC " <<  ( (cWord2 >> 21) & 0x1F) << " DC " << ( (cWord2 >> 16) & 0x1F) << " Pxl " << ( (cWord2 >> 8) & 0xFF) << " PH " << (cWord2 & 0xFF) << std::endl;
              Channel_Number = (DDR[i] >> 26) & 0x3F;
              ROC_Identifier_1 = (DDR[i] >> 21) & 0x1F;
              ROC_Identifier_2 = (DDR[i] >> 21) & 0x1F;
              if( ROC_Identifier_1 == 29){
                Timeout_Error++;
                std::cout << "There was a timeout at event " << std::dec << Current_Event_Num << " in channel " << Channel_Number << std::endl;
                Stack = DDR[i] & 0x3F;
              }
              else if( ROC_Identifier_1 == 31){
                DDR_Event_Num_Error++;
                std::cout << "There was an Event Num error at event " << std::dec << Current_Event_Num << " in channel " << Channel_Number << std::endl;
              }
              else if ( ROC_Identifier_1 == 1 && Channel_Number == 1 && ((DDR[ i - 1 ] >> 28) != 0xa) ){// && ROC_Identifier_1 <= 8){
                if (ROC_Identifier_2 == 8)
                  Roc_num = 1;
                else
                  Roc_num = 0;
                pixel_hit++;
                DCOL[ n ] = (DDR[i] >> 16) & 0x1F;
                PXL[ n ] = (DDR[i] >> 8) & 0xFF;
		Compare_DCOL = DCOL_Encode(Hit_infoA_buff);
                Compare_PXL = PXL_Encode(new_row);

              /*adc_adjust = adc + Roc_num - 1;
              if ((FIFO1[i] & 0xff) != adc_adjust) {
                std::cout << "ADC error! Correct ADC = " << std::dec << adc_adjust << std::endl;
                std::cout << "ADC = " << std::dec << (FIFO1[i] & 0xff) << std::endl;
              }*/

              if ( PXL[ n ] != Compare_PXL) {
                std::cout << "PXL Error, PXL input = " << std::dec << Compare_PXL << "PXL output = " << PXL[n] << std::endl;
              }
              if ( DCOL[ n ] != Compare_DCOL) {
                std::cout << "DCOL  Error, DCOL input = " << std::dec << Compare_DCOL << "DCOL ouput = " << DCOL[n] << std::endl;
              }
              
	      }
              else if ( ROC_Identifier_1 == 30){
                if(Roc_num != 1){
                  Incorrect_ROC++;
                  Error_Count++;
                }
                if((DDR[i] & 0x80) >> 7){
                  std::cout << "There was a NTP at event " << Current_Event_Num << std::endl;
                }

                if((DDR[i] & 0x40) >> 6){
                  std::cout << "There was a TBM Reset at event " << Current_Event_Num << std::endl;
                }

                if((DDR[i] & 0x20) >> 5){
                  std::cout << "There was a ROC Reset at event " << Current_Event_Num << std::endl;
                }

                if((DDR[i] & 0x10) >> 4){
                  std::cout << "Stack is full!" << std::endl;
                }

                if(((DDR[i] & 0x4000) >> 18)){
                  std::cout << "There was a PKAM Reset at Event " << std::dec << Current_Event_Num << std::endl;
                }

                if(Stack != 0){
                  std::cout << "The stack count is " << std::dec << Stack << std::endl;
                }
              }
            }
          }//Scan DDR
        }//adc loop
      }//col loop
    }//row loop
  }//Loops loop

  //Feed the info into the histograms
  Pixel_Alive_Plots_DDR(DCOL, PXL);
}
