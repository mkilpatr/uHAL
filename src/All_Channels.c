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

void All_Resets( int Delay1, int Delay2, int Loops, int Choice_Input, int Loop_Delay1, int Loop_Delay2, int Random_Marker_Test){
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
	else if(Choice_Input == 4){
                std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_DDR.xml");
                cHWFile = filename.c_str();
        }

	string ROCSA[ 8 ] = {"CHA_ROC0_", "CHA_ROC1_", "CHA_ROC2_", "CHA_ROC3_", "CHA_ROC4_", "CHA_ROC5_", "CHA_ROC6_", "CHA_ROC7_"};
	string ROCSB[ 8 ] = {"CHB_ROC0_", "CHB_ROC1_", "CHB_ROC2_", "CHB_ROC3_", "CHB_ROC4_", "CHB_ROC5_", "CHB_ROC6_", "CHB_ROC7_"};
	string index[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};

	uhal::setLogLevelTo(uhal::Debug());

	SystemController cSystemController;
	Amc13Controller  cAmc13Controller;

	cSystemController.InitializeSettings(cHWFile, std::cout);

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
	//for (auto& cFED : cSystemController.fPixFEDVector)
	//{
		auto& cFED = cSystemController.fPixFEDVector[0];
		cSystemController.fFEDInterface->getBoardInfo(cFED);
		cSystemController.fFEDInterface->findPhases(cFED);
		cSystemController.fFEDInterface->Start (cFED);
	//}
	//if(Choice_Input == 4){
		auto& cFED2 = cSystemController.fPixFEDVector[1];
		auto& cFED3 = cSystemController.fPixFEDVector[2];
		auto& cFED4 = cSystemController.fPixFEDVector[3];
	//}

	std::vector<uint32_t> DDR;
	std::vector<uint32_t> FIFO;
	std::vector<uint32_t> FIFO_CHB;
	std::vector<std::vector<uint32_t>> FIFO1(2);
	std::vector<uint32_t> FIFO1_marker;
	std::vector<uint32_t> Error_FIFO;
	std::vector<uint32_t> Spy_Array;
	std::vector<uint32_t> FIFO1_Array;
	std::vector<uint32_t> Error_Array;
	std::vector<uint32_t> DDR_Array;
	std::fill(FIFO.begin(), FIFO.end(), 0);
	std::fill(FIFO_CHB.begin(), FIFO_CHB.end(), 0);
	//std::fill(FIFO1.begin(), FIFO1.end(), 0);
	//std::fill(FIFO1_marker.begin(), FIFO1_marker.end(), 0);
	std::fill(DDR.begin(), DDR.end(), 0);
	std::fill(Error_FIFO.begin(), Error_FIFO.end(), 0);
	std::fill(Spy_Array.begin(), Spy_Array.end(), 0);
	std::fill(FIFO1_Array.begin(), FIFO1_Array.end(), 0);
	std::fill(Error_Array.begin(), Error_Array.end(), 0);
	std::fill(DDR_Array.begin(), DDR_Array.end(), 0);

	std::vector<uint32_t> Spy_Analyze;
	std::vector<uint32_t> FIFO1_Analyze;
	std::vector<uint32_t> Error_Analyze;
	std::vector<uint32_t> DDR_Analyze;

	int Compare_Event_Spy = 0;
	int Total_Events_Spy = 0;
        int Incorrect_Event_Num_Spy = 0;
        int Incorrect_Header_Spy = 0;
        int Incorrect_Trailer_Spy = 0;
        int Incorrect_ROC_Spy = 0;
	int Compare_Event_FIFO1 = 0;
	int Total_Events_FIFO1 = 0;
        int Incorrect_Event_Num_FIFO1 = 0;
        int Incorrect_Header_FIFO1 = 0;
        int Incorrect_Trailer_FIFO1 = 0;
        int Incorrect_Pixel_Spy = 0;
	int Total_Events_Error = 0;
        int Incorrect_Event_Num_Error = 0;
        int Timeout_Error_Error = 0;
	int Compare_Event_DDR = 0;
	int Total_Events_DDR = 0;
        int Incorrect_Event_Num_DDR = 0;
        int Timeout_Error_DDR = 0;
	int pixel_hit_DDR = 0;

	std::cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << std::endl;
	cAmc13Controller.fAmc13Interface->SendEC0();
	cAmc13Controller.fAmc13Interface->EnableBGO(0);
	sleep(0.001);
	cAmc13Controller.fAmc13Interface->DisableBGO(0);

	for (int l = 0; l < Loops; l++){

		if( l == 1 && Choice_Input == 1){
			cAmc13Controller.fAmc13Interface->EnableBGO(1);
        		sleep(0.001);
        		cAmc13Controller.fAmc13Interface->DisableBGO(1);
		}

		if(Random_Marker_Test == 1){
			int Which_Marker = rand() % 3 + 1;
        		int New_Marker = Random_Marker( Which_Marker );
			int Random_Rate = rand() % 6 + 5;
			Marker_Error( 0x11111111, Which_Marker, New_Marker, Random_Rate);
		}

		if(Random_Marker_Test == 2){
			int DCOL = Random_DCOL();
			int PXL = Random_PXL( rand() % 52 );
			int ADC = rand() % 256;
			int Random_ROCA = rand() % 8;
			int Random_ROCB = rand() % 8;
			int Hit_info = (PXL << 16) | (DCOL << 8) | ADC;

			int ROCA = 1 << (Random_ROCA * 4);
			int ROCB = 1 << (Random_ROCB * 4);
			int jA = Random_ROCA;
			int jB = Random_ROCB;
			string Chan_A;
			string Chan_B;
		
			if( l % 1000 == 0)
				std::cout << "Loop number " << l << std::endl;
	
			for(int i = 0; i < 8; i++){
				std::string cha_hits = "CHA_Hits_";
    				cha_hits = cha_hits + index[i];
				std::string chb_hits = "CHB_Hits_";
    				chb_hits = chb_hits + index[i];			

				Chan_A = ROCSA[jA] + index[i];
              			Chan_B = ROCSB[jB] + index[i];

				cSystemController.fFEDInterface->WriteBoardReg(cFED2, cha_hits, ROCA);
				cSystemController.fFEDInterface->WriteBoardReg(cFED3, cha_hits, ROCA);
				cSystemController.fFEDInterface->WriteBoardReg(cFED2, chb_hits, ROCB);
                                cSystemController.fFEDInterface->WriteBoardReg(cFED3, chb_hits, ROCB);

				cSystemController.fFEDInterface->WriteBoardReg(cFED2, Chan_A, Hit_info);
              			cSystemController.fFEDInterface->WriteBoardReg(cFED2, Chan_B, Hit_info);
              			cSystemController.fFEDInterface->WriteBoardReg(cFED3, Chan_A, Hit_info);
              			cSystemController.fFEDInterface->WriteBoardReg(cFED3, Chan_B, Hit_info);

				cSystemController.fFEDInterface->WriteBoardReg(cFED4, cha_hits, ROCA);
				cSystemController.fFEDInterface->WriteBoardReg(cFED4, chb_hits, ROCB);
				cSystemController.fFEDInterface->WriteBoardReg(cFED4, Chan_A, Hit_info);
                                cSystemController.fFEDInterface->WriteBoardReg(cFED4, Chan_B, Hit_info);
			}	
		}

		//for (auto& cFED : cSystemController.fPixFEDVector){

			if(l == 0){
				cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
				if(Choice_Input != 4)
					Delay_Time(Delay1);
				sleep(4);
				cAmc13Controller.fAmc13Interface->BurstL1A();
			}
			else if(l == Loop_Delay1 || l == Loop_Delay2){
				if(Choice_Input != 4)
					Delay_Time(Delay2);
				sleep(0.04);
				cAmc13Controller.fAmc13Interface->BurstL1A();
			}

			else{
				if(Choice_Input != 4)
					Delay_Time(Delay1);
				sleep(0.04);
				cAmc13Controller.fAmc13Interface->BurstL1A();
			}

			sleep(0.001);

			FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
			FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);
			FIFO1 = cSystemController.fFEDInterface->readFIFO1_vec(cFED);
			//FIFO1_marker = cSystemController.fFEDInterface->readFIFO1Marker(cFED);
			Error_FIFO = cSystemController.fFEDInterface->readErrorFIFO_vec(cFED, true);
			cSystemController.fFEDInterface->readTTSState(cFED);
			DDR = cSystemController.fFEDInterface->ReadData(cFED, 0 );

		//}

		Spy_Analyze = Spy_FIFO_Event(FIFO, FIFO_CHB, Compare_Event_Spy);
		FIFO1_Analyze = FIFO1_Event(FIFO1[0], FIFO1[1], Compare_Event_FIFO1);
		Error_Analyze = Error_Event(Error_FIFO);
		DDR_Analyze = DDR_Event(DDR, Compare_Event_DDR);

		Compare_Event_Spy = Spy_Analyze[0];
		Total_Events_Spy += Spy_Analyze[1];
		Incorrect_Event_Num_Spy  += Spy_Analyze[2];
		Incorrect_Header_Spy += Spy_Analyze[3];
		Incorrect_Trailer_Spy  += Spy_Analyze[4];
		Incorrect_ROC_Spy += Spy_Analyze[5];
		Incorrect_Pixel_Spy += Spy_Analyze[6];

		if(l == (Loops - 1)){
			std::cout << "There have been " << std::dec << Total_Events_Spy << " Events" << std::endl;
			std::cout << "There have been " << std::dec << Incorrect_Event_Num_Spy << " Incorrect Event Numbers " << std::endl;
			std::cout << "There have been " << std::dec << Incorrect_Header_Spy << " Incorrect TBM Headers " << std::endl;
			std::cout << "There have been " << std::dec << Incorrect_Trailer_Spy << " Incorrect TBM Trailers " << std::endl;
			std::cout << "There have been " << std::dec << Incorrect_ROC_Spy << " incorrect Number of ROCs " << std::endl;
			std::cout << "There have been " << std::dec << Incorrect_Pixel_Spy << " incorrect Number of Pixels " << std::endl;
			std::cout << " " << std::endl;
		}

		Compare_Event_FIFO1 = FIFO1_Analyze[0];
		Total_Events_FIFO1 += FIFO1_Analyze[1];
		Incorrect_Event_Num_FIFO1 += FIFO1_Analyze[2];
		Incorrect_Header_FIFO1 += FIFO1_Analyze[3];
		Incorrect_Trailer_FIFO1 += FIFO1_Analyze[4];

		if(l == (Loops - 1)){
	    		std::cout << "There have been " << std::dec << Total_Events_FIFO1 << " Events" << std::endl;
	    		std::cout << "There have been " << std::dec << Incorrect_Event_Num_FIFO1 << " Incorrect Event Numbers " << std::endl;
	    		std::cout << "There have been " << std::dec << Incorrect_Header_FIFO1 << " Incorrect TBM Headers " << std::endl;
	    		std::cout << "There have been " << std::dec << Incorrect_Trailer_FIFO1 << " Incorrect TBM Trailers " << std::endl;
	    		std::cout << " " << std::endl;
		}

		Total_Events_Error += Error_Analyze[0];
		Incorrect_Event_Num_Error += Error_Analyze[1];
		Timeout_Error_Error += Error_Analyze[2];

		if(l == (Loops - 1)){
        		std::cout << "There were " << Total_Events_Error << " Events in Error FIFO. " << std::endl;
        		std::cout << "There were " << Incorrect_Event_Num_Error << " Incorrect Event numbers." << std::endl;
        		std::cout << "There were " << Timeout_Error_Error << " timeout errors." << std::endl;
			std::cout << " " << std::endl;
		}

		Compare_Event_DDR = DDR_Analyze[0];
		Total_Events_DDR += DDR_Analyze[1];
		Incorrect_Event_Num_DDR += DDR_Analyze[2];
		Timeout_Error_DDR += DDR_Analyze[3];
		pixel_hit_DDR += DDR_Analyze[4];

		if(l == (Loops - 1)){
			std::cout << "There were " << Total_Events_DDR << " Events in DDR. " << std::endl;
			std::cout << "There were " << Incorrect_Event_Num_DDR << " Incorrect Event numbers." << std::endl;
			std::cout << "There were " << Timeout_Error_DDR << " timeout errors." << std::endl;
			std::cout << "There were " << pixel_hit_DDR << " pixel hits." << std::endl;
			std::cout << " " << std::endl;
		}

	}
}

void Test_Data_Header( int Inner_Loops, int Outer_Loops, int Choice ){
	using namespace std;
        const char* cHWFile;
	if(Choice == 0){
        	std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_Delay_single.xml");
		cHWFile = filename.c_str();
	}
	else if(Choice == 1){
		std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_BX.xml");
        	cHWFile = filename.c_str();
	}
	else if(Choice == 2){
		std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_SRAM.xml");
        	cHWFile = filename.c_str();
	}
	else if(Choice == 3){
		std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_Delay_single.xml");
                cHWFile = filename.c_str();
	}

        uhal::setLogLevelTo(uhal::Debug());
        SystemController cSystemController;
        Amc13Controller  cAmc13Controller;

        cSystemController.InitializeSettings(cHWFile, std::cout);
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

	auto& cFED = cSystemController.fPixFEDVector[0];
	auto& cFED1 = cSystemController.fPixFEDVector[1];
	auto& cFED2 = cSystemController.fPixFEDVector[2];
	auto& cFED3 = cSystemController.fPixFEDVector[3];

        cSystemController.fFEDInterface->getBoardInfo(cFED);
        cSystemController.fFEDInterface->findPhases(cFED);
        cSystemController.fFEDInterface->Start (cFED);

        std::vector<unsigned int> FIFO;
        std::vector<unsigned int> FIFO_CHB;
        std::vector<unsigned int> DDR;
        std::vector<std::vector<unsigned int>> FIFO1(2);
        std::vector<uint32_t> Error_FIFO;

        std::vector<uint32_t> Spy_Analyze;
        std::vector<uint32_t> FIFO1_Analyze;
        std::vector<uint32_t> Error_Analyze;
        std::vector<uint32_t> DDR_Analyze;
        std::vector<uint32_t> TTC_History;
	std::vector<uint32_t> Error_Event_Num;
	std::vector<uint32_t> Error_Data_Num;

	int Compare_Event_Spy = 0;
        int Total_Events_Spy = 0;
        int Incorrect_Event_Num_Spy = 0;
        int Incorrect_Header_Spy = 0;
        int Incorrect_Trailer_Spy = 0;
        int Incorrect_ROC_Spy = 0;
	int Error_Buffer_Spy = 0;
	int Error_Buffer_Spy_init = 0;
        int Compare_Event_FIFO1 = 0;
        int Total_Events_FIFO1 = 0;
        int Incorrect_Event_Num_FIFO1 = 0;
        int Incorrect_Header_FIFO1 = 0;
        int Incorrect_Trailer_FIFO1 = 0;
        int Incorrect_Pixel_Spy = 0;
        int Total_Events_Error = 0;
        int Incorrect_Event_Num_Error = 0;
        int Timeout_Error_Error = 0;
	int Compare_Event_DDR = 0;
        int Total_Events_DDR = 0;
        int Incorrect_Event_Num_DDR = 0;
        int Timeout_Error_DDR = 0;
        int pixel_hit_DDR = 0;	
	std::vector<uint32_t> BX_read;	
	std::vector<uint32_t> BX_input;

	std::cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << std::endl;
        cAmc13Controller.fAmc13Interface->SendEC0();
	cAmc13Controller.fAmc13Interface->EnableBGO(0);
        cAmc13Controller.fAmc13Interface->DisableBGO(0);

	for (int q = 0; q < Outer_Loops; q++){
		if(Choice == 0)
			Write_Data_Header(q);
		if(Choice == 1){
			//BX_read = cAmc13Controller.fAmc13Interface->BXShifter(0x0, 0x2c, 0x50);
			Delay_Time(255 - q);
			BX_input.push_back(255 - q);
			//std::cout << "BX reg: " << std::dec << BX_read << std::endl;
		} 		
		if(Choice == 2){
			//cSystemController.fFEDInterface->WriteBoardReg(cFED1, "Wea", 1);
			//cSystemController.fFEDInterface->WriteBoardReg(cFED1, "sys_regs", 0x10001);
			//cSystemController.fFEDInterface->WriteBoardReg(cFED2, "Wea", 1);
			//cSystemController.fFEDInterface->WriteBoardReg(cFED2, "sys_regs", 0x10001);
			//cSystemController.fFEDInterface->WriteBoardReg(cFED3, "Wea", 1);
			//cSystemController.fFEDInterface->WriteBoardReg(cFED3, "sys_regs", 0x10001);
			SRAM_Mode();			
		}		

		if(Choice == 3){
			std::cout << "Do Trailer timeout test" << std::endl;
			cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.tts.trailer_timeout_valid", 1);
		}

        	for (int l = 0; l < Inner_Loops; l++)
        	{
        		Error_Buffer_Spy = Error_Buffer_Spy_init; 
		       	if(l%5000 == 0){
        	                std::cout << "Outer Loop = " << std::dec << q << std::endl;
        	        }

			if(l == 0 && q == 0){
        	                cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
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
        	        //cSystemController.fFEDInterface->readErrorFIFO(cFED, true);
        	        cSystemController.fFEDInterface->readTTSState(cFED);
        	        DDR = cSystemController.fFEDInterface->ReadData(cFED, 0 );

			Spy_Analyze = Spy_FIFO_Event(FIFO, FIFO_CHB, Compare_Event_Spy);
        		FIFO1_Analyze = FIFO1_Event(FIFO1[0], FIFO1[1], Compare_Event_FIFO1);
        		Error_Analyze = Error_Event(Error_FIFO);
        		DDR_Analyze = DDR_Event(DDR, Compare_Event_DDR);
        		
			Compare_Event_Spy = Spy_Analyze[0];
        		Total_Events_Spy += Spy_Analyze[1];
        		Incorrect_Event_Num_Spy  += Spy_Analyze[2];
        		Incorrect_Header_Spy += Spy_Analyze[3];
        		Incorrect_Trailer_Spy  += Spy_Analyze[4];
        		Incorrect_ROC_Spy += Spy_Analyze[5];
        		Incorrect_Pixel_Spy += Spy_Analyze[6];
			Error_Buffer_Spy_init = Incorrect_Event_Num_Spy + Incorrect_Header_Spy + Incorrect_Trailer_Spy + Incorrect_ROC_Spy;
        		if(Error_Buffer_Spy_init - Error_Buffer_Spy != 0){
				Error_Event_Num.push_back(Compare_Event_Spy);
				Error_Data_Num.push_back(q);
			}
			
        		if(q == (Outer_Loops - 1) && l == (Inner_Loops - 1)){
        		        std::cout << "There have been " << std::dec << Total_Events_Spy << " Events" << std::endl;
        		        std::cout << "There have been " << std::dec << Incorrect_Event_Num_Spy << " Incorrect Event Numbers " << std::endl;
        		        std::cout << "There have been " << std::dec << Incorrect_Header_Spy << " Incorrect TBM Headers " << std::endl;
        		        std::cout << "There have been " << std::dec << Incorrect_Trailer_Spy << " Incorrect TBM Trailers " << std::endl;
        		        std::cout << "There have been " << std::dec << Incorrect_ROC_Spy << " incorrect Number of ROCs " << std::endl;
        		        std::cout << "There have been " << std::dec << Incorrect_Pixel_Spy << " incorrect Number of Pixels " << std::endl;
        		        std::cout << " " << std::endl;
        			for(uint32_t j = 0; j != Error_Event_Num.size(); j++){
					std::cout << "Error Event: " << std::hex << Error_Event_Num[j] << " Data: " << Error_Data_Num[j] << std::endl;
				}
			}
        		
        		Compare_Event_FIFO1 = FIFO1_Analyze[0];
        		Total_Events_FIFO1 += FIFO1_Analyze[1];
        		Incorrect_Event_Num_FIFO1 += FIFO1_Analyze[2];
        		Incorrect_Header_FIFO1 += FIFO1_Analyze[3];
        		Incorrect_Trailer_FIFO1 += FIFO1_Analyze[4];

        		if(q == (Outer_Loops - 1) && l == (Inner_Loops - 1)){
        		        std::cout << "There have been " << std::dec << Total_Events_FIFO1 << " Events" << std::endl;
        		        std::cout << "There have been " << std::dec << Incorrect_Event_Num_FIFO1 << " Incorrect Event Numbers " << std::endl;
        		        std::cout << "There have been " << std::dec << Incorrect_Header_FIFO1 << " Incorrect TBM Headers " << std::endl;
        		        std::cout << "There have been " << std::dec << Incorrect_Trailer_FIFO1 << " Incorrect TBM Trailers " << std::endl;
        		        std::cout << " " << std::endl;
        		}
        		
        		Total_Events_Error += Error_Analyze[0];
        		Incorrect_Event_Num_Error += Error_Analyze[1];
        		Timeout_Error_Error += Error_Analyze[2];
        		
        		if(q == (Outer_Loops - 1) && l == (Inner_Loops - 1)){
        		        std::cout << "There were " << Total_Events_Error << " Events in Error FIFO. " << std::endl;
        		        std::cout << "There were " << Incorrect_Event_Num_Error << " Incorrect Event numbers." << std::endl;
        		        std::cout << "There were " << Timeout_Error_Error << " timeout errors." << std::endl;
        		        std::cout << " " << std::endl;
        		}
 
			Compare_Event_DDR = DDR_Analyze[0];       		
        		Total_Events_DDR += DDR_Analyze[1];
        		Incorrect_Event_Num_DDR += DDR_Analyze[2];
        		Timeout_Error_DDR += DDR_Analyze[3];
        		pixel_hit_DDR += DDR_Analyze[4];
			BX_read.push_back(DDR_Analyze[5]); 
       		
        		if(q == (Outer_Loops - 1) && l == (Inner_Loops - 1)){
        		        std::cout << "There were " << Total_Events_DDR << " Events in DDR. " << std::endl;
        		        std::cout << "There were " << Incorrect_Event_Num_DDR << " Incorrect Event numbers." << std::endl;
        		        std::cout << "There were " << Timeout_Error_DDR << " timeout errors." << std::endl;
        		        std::cout << "There were " << pixel_hit_DDR << " pixel hits." << std::endl;
        		        std::cout << " " << std::endl;
				if(Choice == 1){
					for(uint32_t i = 0; i != BX_read.size(); i++){
						if(BX_read[i] != 503)
							std::cout << "FED BX: " << BX_read[i] << " at L1A Delay " << BX_input[i] << "Event: " << i << std::endl;
					}
				}
			}
		}//Inner Loops
		if(Choice == 3)
			cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.tts.trailer_timeout_valid", 1);
	}//Outer Loops
}

void Test_TTC_History( int Loops ){
	using namespace std;
        const char* cHWFile;
        std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_TTC_History.xml");
        cHWFile = filename.c_str();

	uhal::setLogLevelTo(uhal::Debug());
	SystemController cSystemController;
    	Amc13Controller  cAmc13Controller;

	cSystemController.InitializeSettings(cHWFile, std::cout);
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

	cAmc13Controller.fAmc13Interface->EnableTTCHistory();

    	auto& cFED = cSystemController.fPixFEDVector[0];

	cSystemController.fFEDInterface->getBoardInfo(cFED);
        cSystemController.fFEDInterface->findPhases(cFED);
        cSystemController.fFEDInterface->Start (cFED);

	std::vector<unsigned int> FIFO;
    	std::vector<unsigned int> FIFO_CHB;
    	std::vector<unsigned int> DDR;
    	std::vector<std::vector<unsigned int>> FIFO1(2);
    	std::vector<uint32_t> Error_FIFO;

	std::vector<uint32_t> Spy_Analyze;
        std::vector<uint32_t> FIFO1_Analyze;
        std::vector<uint32_t> Error_Analyze;
        std::vector<uint32_t> DDR_Analyze;
	std::vector<uint32_t> TTC_History;

        int Total_Events_Spy = 0;
        int Incorrect_Event_Num_Spy = 0;
        int Incorrect_Header_Spy = 0;
        int Incorrect_Trailer_Spy = 0;
        int Incorrect_ROC_Spy = 0;
        int Compare_Event_FIFO1 = 0;
        int Total_Events_FIFO1 = 0;
        int Incorrect_Event_Num_FIFO1 = 0;
        int Incorrect_Header_FIFO1 = 0;
        int Incorrect_Trailer_FIFO1 = 0;
        int Incorrect_Pixel_Spy = 0;
        int Total_Events_Error = 0;
        int Incorrect_Event_Num_Error = 0;
        int Timeout_Error_Error = 0;
        int Total_Events_DDR = 0;
        int Incorrect_Event_Num_DDR = 0;
        int Timeout_Error_DDR = 0;
        int pixel_hit_DDR = 0;
		
    	int Total_Events = 0;
    	int TBM_Reset_Num = 0;
	int EC_Reset_Num = 0;
	int ROC_Reset_Num = 0;

    	//std::fill(Error_FIFO.begin(), Error_FIFO.end(), 0);
    	//std::fill(FIFO.begin(), FIFO.end(), 0);
    	//std::fill(FIFO_CHB.begin(), FIFO_CHB.end(), 0);
    	//std::fill(DDR.begin(), DDR.end(), 0);
    	//std::fill(FIFO1.begin(), FIFO1.end(), 0);
    	//std::fill(FIFO1_marker.begin(), FIFO1_marker.end(), 0);

    	std::cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << std::endl;
    	cAmc13Controller.fAmc13Interface->SendEC0();
	//cAmc13Controller.fAmc13Interface->SendBGO();
	cAmc13Controller.fAmc13Interface->EnableBGO(0);
    	cAmc13Controller.fAmc13Interface->DisableBGO(0);

	for (int l = 0; l < Loops; l++)
    	{
        	if(l%5000 == 0){
                	std::cout << "Loop Number = " << std::dec << l << std::endl;
        	}

		if(l != 0){ 
                	//cAmc13Controller.fAmc13Interface->SendEC0();
			cAmc13Controller.fAmc13Interface->SendBGO();
			//usleep(1);
			//cAmc13Controller.fAmc13Interface->EnableBGO(0);
                	//cAmc13Controller.fAmc13Interface->DisableBGO(0);
        	}

		if(l == 0){
                	cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
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
                //cSystemController.fFEDInterface->readErrorFIFO(cFED, true);
                //cSystemController.fFEDInterface->readTTSState(cFED);
                //DDR = cSystemController.fFEDInterface->ReadData(cFED, 0 );
	
		if(l == (Loops - 1) || (l % 500 == 0 && l != 0)){
			cAmc13Controller.fAmc13Interface->DumpHistory(512);

			std::cout << "-> TTC_HISTORY_FIFO_read_rdy :" << cSystemController.fFEDInterface->ReadBoardReg(cFED, "TTC_HISTORY_FIFO_read_rdy") << std::endl;
        		std::cout << "-> TTC_HISTORY_FIFO_wr_data_count :" << cSystemController.fFEDInterface->ReadBoardReg(cFED, "TTC_HISTORY_wr_data_count") << std::endl;

        		std::cout << "---> Force read" << std::endl;
        		cSystemController.fFEDInterface->WriteBoardReg(cFED, "TTC_HISTORY.TTC_HISTORY_FIFO_force_read",1);
        		int WordsNbToRead = cSystemController.fFEDInterface->ReadBoardReg(cFED,"TTC_HISTORY_wr_data_count");
        		std::cout << "->" << WordsNbToRead << " words to read from ERR_FIFO..." << std::endl;
        		std::vector< unsigned int > ERR_FIFO_rdData = cSystemController.fFEDInterface->ReadBlockBoardReg(cFED, "TTC_HISTORY_FIFO", WordsNbToRead);
        		for( int i = 0; i < WordsNbToRead; i++){
                		std::cout << std::hex << ERR_FIFO_rdData[i] << "Event :" << ((ERR_FIFO_rdData[i]&0xfffc0)>>6) << " BX: " << ((ERR_FIFO_rdData[i]&0xfff00000)>>20) << " B data: " << (ERR_FIFO_rdData[i]&0x3f) << std::endl;
                		if((ERR_FIFO_rdData[i]&0x3f) == 0x1)
                        		EC_Reset_Num++;
				if((ERR_FIFO_rdData[i]&0x3f) == 0x14)
                                        TBM_Reset_Num++;
				if((ERR_FIFO_rdData[i]&0x3f) == 0x1C)
                                        ROC_Reset_Num++;
        		}
        		cSystemController.fFEDInterface->WriteBoardReg(cFED, "TTC_HISTORY.TTC_HISTORY_FIFO_read_done",1);
        		sleep(1);
        		std::cout << "-> TTC_HISTORY_FIFO_read_rdy :" << cSystemController.fFEDInterface->ReadBoardReg(cFED, "TTC_HISTORY_FIFO_read_rdy") << std::endl;
        		cSystemController.fFEDInterface->WriteBoardReg(cFED, "TTC_HISTORY.TTC_HISTORY_FIFO_force_read",0);
        		cSystemController.fFEDInterface->WriteBoardReg(cFED, "TTC_HISTORY.TTC_HISTORY_FIFO_read_done",0);
		}

	}//Loop loop

	std::cout << "There have been " << std::dec << EC_Reset_Num << " ECRs" << std::endl;
	std::cout << "There have been " << std::dec << TBM_Reset_Num << " TBM Resets" << std::endl;
	std::cout << "There have been " << std::dec << ROC_Reset_Num << " ROC Resets" << std::endl;
}

void Memory_Readout( int Loops, int Max_Loops){
	using namespace std;
        const char* cHWFile;
        std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_Memory.xml");
        cHWFile = filename.c_str();

        uhal::setLogLevelTo(uhal::Debug());
        SystemController cSystemController;
        Amc13Controller  cAmc13Controller;

        cSystemController.InitializeSettings(cHWFile, std::cout);
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


        auto& cFED = cSystemController.fPixFEDVector[0];

        cSystemController.fFEDInterface->getBoardInfo(cFED);
        cSystemController.fFEDInterface->findPhases(cFED);
        cSystemController.fFEDInterface->Start (cFED);

        std::vector<unsigned int> FIFO;
        std::vector<unsigned int> FIFO_CHB;
        std::vector<unsigned int> DDR;
        std::vector<std::vector<unsigned int>> FIFO1;
        std::vector<uint32_t> Error_FIFO;
	std::vector<uint32_t> Spy_Analyze;
	std::vector<uint32_t> DDR_Analyze;

 	int Compare_Event_Spy = 0;
        int Total_Events = 0;
	int TBM_Reset_Num = 0;
	int Compare_Event_DDR = 0;

        std::fill(Error_FIFO.begin(), Error_FIFO.end(), 0);
        std::fill(FIFO.begin(), FIFO.end(), 0);
        std::fill(FIFO_CHB.begin(), FIFO_CHB.end(), 0);
        std::fill(DDR.begin(), DDR.end(), 0);

        std::cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << std::endl;
        cAmc13Controller.fAmc13Interface->SendEC0();
        cAmc13Controller.fAmc13Interface->EnableBGO(0);
        sleep(0.001);
        cAmc13Controller.fAmc13Interface->DisableBGO(0);

	for (int j = 0; j < Max_Loops; j++){
        	for (int l = 0; l < Loops; l++)
        	{
        	        if(l%5000 == 0){
                	        std::cout << "Loop Number = " << std::dec << l << std::endl;
                	}

			if(l == 0 && j == 0){
                        	cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
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
                	cSystemController.fFEDInterface->readFIFO1_vec(cFED);

			Spy_Analyze = Spy_FIFO_Event(FIFO, FIFO_CHB, Compare_Event_Spy);
                	Compare_Event_Spy = Spy_Analyze[0];
			Total_Events += Spy_Analyze[1];

			if((l % (Loops - 1)) == 0 && l != 0){
				cSystemController.fFEDInterface->readTTSState(cFED);
				DDR = cSystemController.fFEDInterface->ReadData(cFED, 0 );
				DDR_Analyze = DDR_Event(DDR, Compare_Event_DDR);
				Compare_Event_DDR = DDR_Analyze[0];
			}
        	}//Loop loop
	}//j loop
 
 	std::cout << "There have been " << std::dec << Total_Events << " Events" << std::endl;
        std::cout << "There have been " << std::dec << TBM_Reset_Num << " TBM resets" << std::endl;			
}

void Random_Bit_Flip( int Loops, int Random_Threshold, int Random_Value){
	using namespace std;

        const char* cHWFile;

        std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_Delay.xml");
        cHWFile = filename.c_str();

        string ROCSA[ 8 ] = {"CHA_ROC0_", "CHA_ROC1_", "CHA_ROC2_", "CHA_ROC3_", "CHA_ROC4_", "CHA_ROC5_", "CHA_ROC6_", "CHA_ROC7_"};
        string ROCSB[ 8 ] = {"CHB_ROC0_", "CHB_ROC1_", "CHB_ROC2_", "CHB_ROC3_", "CHB_ROC4_", "CHB_ROC5_", "CHB_ROC6_", "CHB_ROC7_"};
        string index[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};

        uhal::setLogLevelTo(uhal::Debug());

        SystemController cSystemController;
        Amc13Controller  cAmc13Controller;

        cSystemController.InitializeSettings(cHWFile, std::cout);

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

	auto& cFED = cSystemController.fPixFEDVector[0];
        cSystemController.fFEDInterface->getBoardInfo(cFED);
        cSystemController.fFEDInterface->findPhases(cFED);
        cSystemController.fFEDInterface->Start (cFED);

	auto& cFED2 = cSystemController.fPixFEDVector[1];
        auto& cFED3 = cSystemController.fPixFEDVector[2];
        auto& cFED4 = cSystemController.fPixFEDVector[3];

	std::vector<uint32_t> DDR;
        std::vector<uint32_t> FIFO;
        std::vector<uint32_t> FIFO_CHB;
        std::vector<std::vector<uint32_t>> FIFO1(2);
        std::vector<uint32_t> Error_FIFO;
        std::vector<uint32_t> Spy_Array;
        std::vector<uint32_t> FIFO1_Array;
        std::vector<uint32_t> Error_Array;
        std::vector<uint32_t> DDR_Array;
        std::fill(FIFO.begin(), FIFO.end(), 0);
        std::fill(FIFO_CHB.begin(), FIFO_CHB.end(), 0);
        std::fill(DDR.begin(), DDR.end(), 0);
        std::fill(Error_FIFO.begin(), Error_FIFO.end(), 0);
        std::fill(Spy_Array.begin(), Spy_Array.end(), 0);
        std::fill(FIFO1_Array.begin(), FIFO1_Array.end(), 0);
        std::fill(Error_Array.begin(), Error_Array.end(), 0);
        std::fill(DDR_Array.begin(), DDR_Array.end(), 0);

        std::vector<uint32_t> Spy_Analyze;
        std::vector<uint32_t> FIFO1_Analyze;
        std::vector<uint32_t> Error_Analyze;
        std::vector<uint32_t> DDR_Analyze;

	int Compare_Event_Spy = 0;
        int Total_Events_Spy = 0;
        int Incorrect_Event_Num_Spy = 0;
        int Incorrect_Header_Spy = 0;
        int Incorrect_Trailer_Spy = 0;
        int Incorrect_ROC_Spy = 0;
        int Compare_Event_FIFO1 = 0;
        int Total_Events_FIFO1 = 0;
        int Incorrect_Event_Num_FIFO1 = 0;
        int Incorrect_Header_FIFO1 = 0;
        int Incorrect_Trailer_FIFO1 = 0;
        int Incorrect_Pixel_Spy = 0;
        int Total_Events_Error = 0;
        int Incorrect_Event_Num_Error = 0;
        int Timeout_Error_Error = 0;
	int Compare_Event_DDR = 0;
        int Total_Events_DDR = 0;
        int Incorrect_Event_Num_DDR = 0;
        int Timeout_Error_DDR = 0;
        int pixel_hit_DDR = 0;

        std::cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << std::endl;
        cAmc13Controller.fAmc13Interface->SendEC0();
        cAmc13Controller.fAmc13Interface->EnableBGO(0);
        sleep(0.001);
        cAmc13Controller.fAmc13Interface->DisableBGO(0);

	for (int l = 0; l < Loops; l++){
		for(int i = 0; i < 8; i++){
                        cSystemController.fFEDInterface->WriteBoardReg(cFED2, "Random_Threshold", Random_Threshold);
                        cSystemController.fFEDInterface->WriteBoardReg(cFED2, "Random_Reset", Random_Value);
                
		        cSystemController.fFEDInterface->WriteBoardReg(cFED3, "Random_Threshold", Random_Threshold);
                        cSystemController.fFEDInterface->WriteBoardReg(cFED3, "Random_Reset", Random_Value);

                        cSystemController.fFEDInterface->WriteBoardReg(cFED4, "Random_Threshold", Random_Threshold);
                        cSystemController.fFEDInterface->WriteBoardReg(cFED4, "Random_Reset", Random_Value);
                }

		if(l == 0){
                        cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
                        sleep(4);
                        cAmc13Controller.fAmc13Interface->BurstL1A();
                }
                else{
                        cAmc13Controller.fAmc13Interface->BurstL1A();
                }

                sleep(0.001);

                FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
                FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);
                FIFO1 = cSystemController.fFEDInterface->readFIFO1_vec(cFED);
                Error_FIFO = cSystemController.fFEDInterface->readErrorFIFO_vec(cFED, true);
                cSystemController.fFEDInterface->readTTSState(cFED);
                DDR = cSystemController.fFEDInterface->ReadData(cFED, 0 );

		Spy_Analyze = Spy_FIFO_Event(FIFO, FIFO_CHB, Compare_Event_Spy);
                FIFO1_Analyze = FIFO1_Event(FIFO1[0], FIFO1[1], Compare_Event_FIFO1);
                Error_Analyze = Error_Event(Error_FIFO);
                DDR_Analyze = DDR_Event(DDR, Compare_Event_DDR);

		Compare_Event_Spy = Spy_Analyze[0];
                Total_Events_Spy += Spy_Analyze[1];
                Incorrect_Event_Num_Spy  += Spy_Analyze[2];
                Incorrect_Header_Spy += Spy_Analyze[3];
                Incorrect_Trailer_Spy  += Spy_Analyze[4];
                Incorrect_ROC_Spy += Spy_Analyze[5];
                Incorrect_Pixel_Spy += Spy_Analyze[6];

                if(l == (Loops - 1)){
                        std::cout << "There have been " << std::dec << Total_Events_Spy << " Events" << std::endl;
                        std::cout << "There have been " << std::dec << Incorrect_Event_Num_Spy << " Incorrect Event Numbers " << std::endl;
                        std::cout << "There have been " << std::dec << Incorrect_Header_Spy << " Incorrect TBM Headers " << std::endl;
                        std::cout << "There have been " << std::dec << Incorrect_Trailer_Spy << " Incorrect TBM Trailers " << std::endl;
                        std::cout << "There have been " << std::dec << Incorrect_ROC_Spy << " incorrect Number of ROCs " << std::endl;
                        std::cout << "There have been " << std::dec << Incorrect_Pixel_Spy << " incorrect Number of Pixels " << std::endl;
                        std::cout << " " << std::endl;
                }

                Compare_Event_FIFO1 = FIFO1_Analyze[0];
                Total_Events_FIFO1 += FIFO1_Analyze[1];
                Incorrect_Event_Num_FIFO1 += FIFO1_Analyze[2];
                Incorrect_Header_FIFO1 += FIFO1_Analyze[3];
                Incorrect_Trailer_FIFO1 += FIFO1_Analyze[4];

                if(l == (Loops - 1)){
                        std::cout << "There have been " << std::dec << Total_Events_FIFO1 << " Events" << std::endl;
                        std::cout << "There have been " << std::dec << Incorrect_Event_Num_FIFO1 << " Incorrect Event Numbers " << std::endl;
                        std::cout << "There have been " << std::dec << Incorrect_Header_FIFO1 << " Incorrect TBM Headers " << std::endl;
                        std::cout << "There have been " << std::dec << Incorrect_Trailer_FIFO1 << " Incorrect TBM Trailers " << std::endl;
                        std::cout << " " << std::endl;
                }

                Total_Events_Error += Error_Analyze[0];
                Incorrect_Event_Num_Error += Error_Analyze[1];
                Timeout_Error_Error += Error_Analyze[2];

                if(l == (Loops - 1)){
                        std::cout << "There were " << Total_Events_Error << " Events in Error FIFO. " << std::endl;
                        std::cout << "There were " << Incorrect_Event_Num_Error << " Incorrect Event numbers." << std::endl;
                        std::cout << "There were " << Timeout_Error_Error << " timeout errors." << std::endl;
                        std::cout << " " << std::endl;
                }

		Compare_Event_DDR = DDR_Analyze[0];
                Total_Events_DDR += DDR_Analyze[1];
                Incorrect_Event_Num_DDR += DDR_Analyze[2];
                Timeout_Error_DDR += DDR_Analyze[3];
                pixel_hit_DDR += DDR_Analyze[4];

                if(l == (Loops - 1)){
                        std::cout << "There were " << Total_Events_DDR << " Events in DDR. " << std::endl;
                        std::cout << "There were " << Incorrect_Event_Num_DDR << " Incorrect Event numbers." << std::endl;
                        std::cout << "There were " << Timeout_Error_DDR << " timeout errors." << std::endl;
                        std::cout << "There were " << pixel_hit_DDR << " pixel hits." << std::endl;
                        std::cout << " " << std::endl;
		}
	}
}
