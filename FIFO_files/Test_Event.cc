#include <ctime>
#include <cstring>
#include "uhal/uhal.hpp"
#include "../Utils/Utilities.h"
#include "../HWDescription/PixFED.h"
#include "../HWInterface/PixFEDInterface.h"
#include "../System/SystemController.h"
#include "../AMC13/Amc13Controller.h"
#include "../Utils/Data.h"
#include "uhal/tests/tools.hpp"
#include <boost/filesystem.hpp>
#include "uhal/HwInterface.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <typeinfo>
#include <list>
#include <stdlib.h>
#include <iterator>

using namespace std;

void mypause()
{
    std::cout << "Press [Enter] to read FIFOs ...";
    std::cin.get();

}
int main(int argc, char* argv[] )
{

    const char* cHWFile = argv[1];
    std::cout << "HW Description File: " << cHWFile << std::endl;

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
    //for (auto& cFED : cSystemController.fPixFEDVector)
    //{
      /*for (auto& cFitel : cFED->fFitelVector)
        {
            //cSystemController.fFEDInterface->ReadLightOnFibre(cFitel);
            cSystemController.fFEDInterface->ReadADC(cFitel, cChannelOfInterest, true);
	    }*/
      
	auto& cFED = cSystemController.fPixFEDVector[0];
	cSystemController.fFEDInterface->getBoardInfo(cFED);
        cSystemController.fFEDInterface->findPhases(cFED, cChannelOfInterest);
    //}
	
    std::vector<unsigned int> FIFO;
    std::vector<unsigned int> FIFO_CHB;
    std::vector<unsigned int> Event_Number;
    //std::vector<unsigned int> Transparent_FIFO;
    unsigned int Event_one = 0;
    unsigned int Event_two = 0;
    int Total_Events = 0;
    int Incorrect_Header = 0;
    int Incorrect_Trailer = 0;
    int Incorrect_Event_Num = 0;
    int Incorrect_ROC = 0;
    int ROC_Value = 0;
    int Loops = atoi(argv[2]);
    uint32_t Current_Event_Num = 0;
    int Error_Count = 0;
    int Do_Error = 0;
    int Pixel = 0;
    int Pixel_Hit = 0;
    int Incorrect_Pixel = 0;
    std::fill(FIFO.begin(), FIFO.end(), 0);
    std::fill(FIFO_CHB.begin(), FIFO_CHB.end(), 0);


    for (int l = 0; l < Loops; l++)
    {
	if(l%5000 == 0){
		std::cout << "Loop Number = " << std::dec << l << std::endl;
	}     
 
        //for (auto& cFED : cSystemController.fPixFEDVector)
        //{

            cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);

            std::cout << "cFED = " << cFED << std::endl;

	    if(l == 0){
		sleep(4);
		cAmc13Controller.fAmc13Interface->BurstL1A();
		sleep(0.001);
	    }
	    else{
		cAmc13Controller.fAmc13Interface->BurstL1A();
		sleep(0.001);
	    }

            //cSystemController.fFEDInterface->readTransparentFIFO(cFED);
            FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
            FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);
            //cSystemController.fFEDInterface->readFIFO1(cFED);
            //cSystemController.fFEDInterface->readOSDWord(cFED, cROCOfInterest, cChannelOfInterest);
            // cSystemController.fFEDInterface->ReadData(cFED, 0 );

	    int k = 0;
	    int j = 0;
	    int h = 0;
	    int Next_step = 0;
	    int Num_ROC = 0;
	    int END_Event = 0;
	    int Error_Loop = 0;
	    int Start_Event = 0;
	    int Stack[2];
	    int Do_Stack = 0;

	    for( std::vector<unsigned int>::size_type i = 0; i != FIFO.size(); i++ )
	    {    

	      /*if( i <= 30 ){
		std::cout << "FIFO[" << std::dec << i << "] = " << std::hex << FIFO[i] << std::endl;
		std::cout << "FIFO_CHB[" << std::dec << i << "] = " << std::hex << FIFO_CHB[i] << std::endl;
	      }*/

		if(FIFO[i] == 0x00){
			break;
		}

		if(FIFO[i] >> 4 == 0x8){
			Error_Count = 0;
			Error_Loop = 0;
		}
		
		if(FIFO[i] >> 4 == 0x9)
		{
			j = i;
			h++;
		  	Total_Events++;
			Current_Event_Num = ((FIFO[i - 1] & 0xF) << 4) | (FIFO[i] & 0xF);
			if( h >= 2)
		  	{
				k = 1;
		 	}
		}

		if ((FIFO[j - 1] << 4) | (FIFO[j] & 0xF ) - (FIFO[j - i] << 4) | (FIFO[j - i + 1] & 0xF) == 1 && k == 1){
                        k = 0;
                }
                else if ((FIFO[j - 1] << 4) | (FIFO[j] & 0xF ) - (FIFO[j - i] << 4) | (FIFO[j - i + 1] & 0xF) != 1 && k == 1){
                        Incorrect_Event_Num++;
                        Error_Count++;
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
				cout << "There was a NTP" << endl;
			}

			if(FIFO[i] & 0x4){
                                cout << "There was a TBM Reset" << endl;
                        }

                        if(FIFO[i] & 0x2){
                                cout << "There was a ROC Reset" << endl;
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
			  std::cout << "There was a PKAM Reset at Event " << dec << Current_Event_Num << std::endl;
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

	      }

        //}

    }

    std::cout << "There have been " << std::dec << Total_Events << " Events" << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Event_Num << " Incorrect Event Numbers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Header << " Incorrect TBM Headers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Trailer << " Incorrect TBM Trailers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_ROC << " incorrect Number of ROCs " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Pixel << " incorrect Number of Pixels " << std::endl;
    
    exit(0);
}
