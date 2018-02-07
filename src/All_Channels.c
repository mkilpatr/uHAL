void All_Resets( int Delay1, int Delay2, int Loops, int Choice_Input, int Loop_Delay1, int Loop_Delay2, int Random_Marker_Test){
	using namespace std;
	const char* cHWFile;

	if(Choice_Input == 0){
		std::string filename ("settings/HWDescription_Delay.xml");
		cHWFile = filename.c_str();
	}
	else if(Choice_Input == 1 || Choice_Input == 5){
		std::string filename ("settings/HWDescription_Resets.xml");
		cHWFile = filename.c_str();
	}
	else if(Choice_Input == 3){
		std::string filename ("settings/HWDescription_PKAM.xml");
		cHWFile = filename.c_str();
	}
	else if(Choice_Input == 4 ){
		std::string filename ("settings/HWDescription_DDR.xml");
		cHWFile = filename.c_str();
	}
	else if(Choice_Input == 6){
		std::string filename ("settings/HWDescription_Resets_CSR.xml");
		cHWFile = filename.c_str();
	}

	string ROCSA[ 8 ] = {"CHA_ROC0", "CHA_ROC1", "CHA_ROC2", "CHA_ROC3", "CHA_ROC4", "CHA_ROC5", "CHA_ROC6", "CHA_ROC7"};
	string ROCSB[ 8 ] = {"CHB_ROC0", "CHB_ROC1", "CHB_ROC2", "CHB_ROC3", "CHB_ROC4", "CHB_ROC5", "CHB_ROC6", "CHB_ROC7"};
	string index[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};

	uhal::setLogLevelTo(uhal::Debug());

	SystemController cSystemController;
	Amc13Controller  cAmc13Controller;

	cSystemController.InitializeSettings(cHWFile, cout);

	cAmc13Controller.InitializeAmc13( cHWFile, cout );
	cSystemController.InitializeHw(cHWFile, cout);

	auto cSetting = cSystemController.fSettingsMap.find("NAcq");
	int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
	cSetting = cSystemController.fSettingsMap.find("BlockSize");
	int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;

	cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
	int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 39;

	cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
	int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;

	cAmc13Controller.ConfigureAmc13( cout );
	cSystemController.ConfigureHw(cout );

	cout << "Configure OK" << endl;

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
	std::vector<uint32_t> FIFO1_marker;
	std::vector<uint32_t> Error_FIFO;
	std::vector<uint32_t> Spy_Array;
	std::vector<uint32_t> FIFO1_Array;
	std::vector<uint32_t> Error_Array;
	std::vector<uint32_t> DDR_Array;

	std::vector<std::vector<uint32_t>> Spy_Analyze(2);
	std::vector<uint32_t> FIFO1_Analyze;
	std::vector<uint32_t> Error_Analyze;
	std::vector<uint32_t> DDR_Analyze;
	std::vector<std::pair<std::string, uint32_t> > Counter_reg;
	typedef std::vector<std::pair<std::string, uint32_t> > vector_type;

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

	uint64_t TBM_Mask_1 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_1");
	uint64_t TBM_Mask_2 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_2");
	uint64_t TBM_Mask = (TBM_Mask_2 << 32) | TBM_Mask_1;


	cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
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
        else if( l == 2 && Choice_Input == 1){
			cAmc13Controller.fAmc13Interface->EnableBGO(2);
		    sleep(0.001);
		    cAmc13Controller.fAmc13Interface->DisableBGO(2);
		}
        else if( l == 1 && Choice_Input == 6){
            cAmc13Controller.fAmc13Interface->SendEC0();
		    sleep(0.001);
		}

        if(l > 0 && Choice_Input == 5){
	        cAmc13Controller.fAmc13Interface->EnableBGO(0);
	        sleep(0.001);
	        cAmc13Controller.fAmc13Interface->DisableBGO(0);
            sleep(0.001);
            Counter_reg = cSystemController.fFEDInterface->RegDump(cFED, 0);
            for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
                cout << pos->first << " " << pos->second << endl;
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
			int Random_ROCA = rand() % 4;
			int Random_ROCB = rand() % 4;
			int Hit_info = (PXL << 16) | (DCOL << 8) | ADC;

			int ROCA = 0 << (Random_ROCA * 8);
			int ROCB = 0 << (Random_ROCB * 8);
			int jA = Random_ROCA;
			int jB = Random_ROCB;
			string Chan_A;
			string Chan_B;
		
			if( l % 1000 == 0)
				cout << "Loop number " << l << endl;

			string fiber("Fiber_");	
			for(int i = 0; i < 8; i++){
				std::string cha_hits = "CHA_Hits_1";
				cha_hits = fiber + index[i] + "." + cha_hits;
				std::string chb_hits = "CHB_Hits_1";
				chb_hits = fiber + index[i] + "." + chb_hits;			

				Chan_A = fiber + index[i] + "." + ROCSA[jA];
				Chan_B = fiber + index[i] + "." + ROCSB[jB];

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
		Error_FIFO = cSystemController.fFEDInterface->readErrorFIFO_vec(cFED, true);
		cSystemController.fFEDInterface->readTTSState(cFED);
		DDR = cSystemController.fFEDInterface->ReadData(cFED, 0 );

		Spy_Analyze = Spy_FIFO_Event(FIFO, FIFO_CHB, Compare_Event_Spy);
		FIFO1_Analyze = FIFO1_Event(FIFO1[0], FIFO1[1], Compare_Event_FIFO1);
		Error_Analyze = Error_Event(Error_FIFO, Timeout_Error_Error, TBM_Mask, 0);
		DDR_Analyze = DDR_Event(DDR, Compare_Event_DDR, TBM_Mask, Timeout_Error_DDR, 0, 0);

		Compare_Event_Spy = Spy_Analyze[0][0];
		Total_Events_Spy += Spy_Analyze[0][1];
		Incorrect_Event_Num_Spy  += Spy_Analyze[0][2];
		Incorrect_Header_Spy += Spy_Analyze[0][3];
		Incorrect_Trailer_Spy  += Spy_Analyze[0][4];
		Incorrect_ROC_Spy += Spy_Analyze[0][5];
		Incorrect_Pixel_Spy += Spy_Analyze[0][6];

		if(l == (Loops - 1)){
			cout << "There have been " << std::dec << Total_Events_Spy << " Events" << endl;
			cout << "There have been " << std::dec << Incorrect_Event_Num_Spy << " Incorrect Event Numbers " << endl;
			cout << "There have been " << std::dec << Incorrect_Header_Spy << " Incorrect TBM Headers " << endl;
			cout << "There have been " << std::dec << Incorrect_Trailer_Spy << " Incorrect TBM Trailers " << endl;
			cout << "There have been " << std::dec << Incorrect_ROC_Spy << " incorrect Number of ROCs " << endl;
			cout << "There have been " << std::dec << Incorrect_Pixel_Spy << " incorrect Number of Pixels " << endl;
			cout << " " << endl;
		}

		Compare_Event_FIFO1 = FIFO1_Analyze[0];
		Total_Events_FIFO1 += FIFO1_Analyze[1];
		Incorrect_Event_Num_FIFO1 += FIFO1_Analyze[2];
		Incorrect_Header_FIFO1 += FIFO1_Analyze[3];
		Incorrect_Trailer_FIFO1 += FIFO1_Analyze[4];

		if(l == (Loops - 1)){
			cout << "There have been " << std::dec << Total_Events_FIFO1 << " Events" << endl;
	    		cout << "There have been " << std::dec << Incorrect_Event_Num_FIFO1 << " Incorrect Event Numbers " << endl;
	    		cout << "There have been " << std::dec << Incorrect_Header_FIFO1 << " Incorrect TBM Headers " << endl;
	    		cout << "There have been " << std::dec << Incorrect_Trailer_FIFO1 << " Incorrect TBM Trailers " << endl;
	    		cout << " " << endl;
		}

		Total_Events_Error += Error_Analyze[0];
		Incorrect_Event_Num_Error += Error_Analyze[1];
		Timeout_Error_Error += Error_Analyze[2];

		if(l == (Loops - 1)){
			cout << "There were " << Total_Events_Error << " Events in Error FIFO. " << endl;
      			cout << "There were " << Incorrect_Event_Num_Error << " Incorrect Event numbers." << endl;
      			cout << "There were " << Timeout_Error_Error << " timeout errors." << endl;
			cout << " " << endl;
		}

		Compare_Event_DDR = DDR_Analyze[0];
		Total_Events_DDR += DDR_Analyze[1];
		Incorrect_Event_Num_DDR += DDR_Analyze[2];
		Timeout_Error_DDR = DDR_Analyze[3];
		pixel_hit_DDR += DDR_Analyze[4];

		if(l == (Loops - 1)){
			cout << "There were " << Total_Events_DDR << " Events in DDR. " << endl;
			cout << "There were " << Incorrect_Event_Num_DDR << " Incorrect Event numbers." << endl;
			cout << "There were " << Timeout_Error_DDR << " timeout errors." << endl;
			cout << "There were " << pixel_hit_DDR << " pixel hits." << endl;
			cout << " " << endl;
		}
	}
    cAmc13Controller.fAmc13Interface->DisableBGO(1);
    cAmc13Controller.fAmc13Interface->DisableBGO(2);
}

void Test_Data_Header( int Inner_Loops, int Outer_Loops, int Choice, int SRAM ){
	using namespace std;
	using namespace uhal;
	const char* cHWFile;
	if(Choice == 0){
		std::string filename ("settings/HWDescription_Data_Header.xml");
		cHWFile = filename.c_str();
    }
	else if(Choice == 1){
		std::string filename ("settings/HWDescription_BX.xml");
		cHWFile = filename.c_str();
	}
	else if(Choice == 2){
		std::string filename ("settings/HWDescription_SRAM.xml");
		cHWFile = filename.c_str();
	}
    if(Choice == 3 || Choice == 4){
		std::string filename ("settings/HWDescription_Delay_single.xml");
		cHWFile = filename.c_str();
	}
	else if(Choice == 6){
		std::string filename ("settings/HWDescription_Large_Event.xml");
		cHWFile = filename.c_str();
	}

	uhal::setLogLevelTo(uhal::Debug());
	SystemController cSystemController;
	Amc13Controller  cAmc13Controller;
	
	cSystemController.InitializeSettings(cHWFile, cout);
	cAmc13Controller.InitializeAmc13( cHWFile, cout );
	cSystemController.InitializeHw(cHWFile, cout);
	
	auto cSetting = cSystemController.fSettingsMap.find("NAcq");
	int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
	cSetting = cSystemController.fSettingsMap.find("BlockSize");
	int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;
	
	cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
	int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
	
	cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
	int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
	
	cAmc13Controller.ConfigureAmc13( cout );
	cSystemController.ConfigureHw(cout );
	
	cout << "Configure OK" << endl;
	
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
	
	std::vector<std::vector<uint32_t>> Spy_Analyze(2);
	std::vector<uint32_t> Spy_Error;
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
	int TTSState = 0;
	std::vector<uint32_t> OOSTimeout_vec;
	int readState = 0;
	int readTBMFIFO = 0;
	int channel = 0;
	int L1A_Empty = 0;
	int Main_Empty = 0;
	int TBM_FIFO_31to0 = 0;
	int TBM_FIFO_47to32 = 0;

	uint64_t TBM_Mask_1 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_1");
	uint64_t TBM_Mask_2 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_2");
	uint64_t TBM_Mask = (TBM_Mask_2 << 32) | TBM_Mask_1;


	typedef std::vector<std::pair<std::string, uint32_t> > vector_type;

	cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
	cAmc13Controller.fAmc13Interface->SendEC0();
	cAmc13Controller.fAmc13Interface->EnableBGO(0);
	cAmc13Controller.fAmc13Interface->DisableBGO(0);

	for (int q = 0; q < Outer_Loops; q++){
		if(Choice == 0)
			Write_Data_Header(q);
		else if(Choice == 1){
			//BX_read = cAmc13Controller.fAmc13Interface->BXShifter(0x0, 0x2c, 0x50);
			//Delay_Time(255 - q);
			BX_input.push_back(255 - q);
			//cout << "BX reg: " << std::dec << BX_read << endl;
		} 		
		else if(Choice == 2){
			SRAM_Mode();			
		}		
		else if(Choice == 4){
			cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.OOS_NB_THRESH",255);
			cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.auto_mask_valid",1);
		}

		for (int l = 0; l < Inner_Loops; l++)
		{
			if(Choice != 5){
				Error_Buffer_Spy = Error_Buffer_Spy_init; 
				if(l%Inner_Loops == 0) cout << "Outer Loop = " << std::dec << q << endl;

				if(l == 0 && q == 0){
					cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
					sleep(4);
					cAmc13Controller.fAmc13Interface->BurstL1A();
				}
				else{
					cAmc13Controller.fAmc13Interface->BurstL1A();
				}
                sleep(0.01);	
        		FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
        		FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);
        		FIFO1 = cSystemController.fFEDInterface->readFIFO1_vec(cFED);
        		cSystemController.fFEDInterface->readErrorFIFO(cFED, true);
        		TTSState = cSystemController.fFEDInterface->readTTSState(cFED);
        		DDR = cSystemController.fFEDInterface->ReadData(cFED, 0 );

				if(Choice == 6){
					std::vector<std::pair<std::string, uint32_t> > Reg_Dump = cSystemController.fFEDInterface->RegDump(cFED, 0);
					for(vector_type::const_iterator pos = Reg_Dump.begin(); pos != Reg_Dump.end(); ++pos)
						cout << pos->first << " " << pos->second << endl;	
					//for(uint32_t i = 0; i != DDR.size(); i+=2)
					//cout << "DDR[" << i << "]: " << std::bitset<32>(DDR[i]) << " " << std::bitset<32>(DDR[i + 1]) << endl;
	
				}

				Spy_Analyze = Spy_FIFO_Event(FIFO, FIFO_CHB, Compare_Event_Spy);
				FIFO1_Analyze = FIFO1_Event(FIFO1[0], FIFO1[1], Compare_Event_FIFO1);
				Error_Analyze = Error_Event(Error_FIFO, Timeout_Error_Error, TBM_Mask, 0);
				DDR_Analyze = DDR_Event(DDR, Compare_Event_DDR, TBM_Mask, Timeout_Error_DDR, 0, 0);
			}
				
			if(Choice == 4 && TTSState != 8){
				OOSTimeout_vec = cSystemController.fFEDInterface->OOSTimeout(cFED);
				for(uint32_t j = 0; j != OOSTimeout_vec.size(); j++){
					cout << "OOS_NB_CH_" << 4*j     << ": " << ( OOSTimeout_vec[j] & 0xFF       ) << endl;
					cout << "OOS_NB_CH_" << 4*j + 1 << ": " << ((OOSTimeout_vec[j] <<  8) & 0xFF) << endl;
					cout << "OOS_NB_CH_" << 4*j + 2 << ": " << ((OOSTimeout_vec[j] << 16) & 0xFF) << endl;
					cout << "OOS_NB_CH_" << 4*j + 3 << ": " << ((OOSTimeout_vec[j] << 24) & 0xFF) << endl;
				}
			}

			Compare_Event_Spy = Spy_Analyze[0][0];
			Total_Events_Spy += Spy_Analyze[0][1];
			Incorrect_Event_Num_Spy  += Spy_Analyze[0][2];
			Incorrect_Header_Spy += Spy_Analyze[0][3];
			Incorrect_Trailer_Spy  += Spy_Analyze[0][4];
			Incorrect_ROC_Spy += Spy_Analyze[0][5];
			Incorrect_Pixel_Spy += Spy_Analyze[0][6];
			Spy_Error.insert(Spy_Error.end(), Spy_Analyze[1].begin(), Spy_Analyze[1].end());
			Error_Buffer_Spy_init = Incorrect_Event_Num_Spy + Incorrect_Header_Spy + Incorrect_Trailer_Spy + Incorrect_ROC_Spy;
			if(Error_Buffer_Spy_init - Error_Buffer_Spy != 0){
				Error_Event_Num.push_back(Compare_Event_Spy);
				Error_Data_Num.push_back(q);
			}
			
			if(q == (Outer_Loops - 1) && l == (Inner_Loops - 1)){
				cout << "There have been " << std::dec << Total_Events_Spy << " Events in Spy FIFO" << endl;
				cout << "There have been " << std::dec << Incorrect_Event_Num_Spy << " Incorrect Event Numbers " << endl;
				cout << "There have been " << std::dec << Incorrect_Header_Spy << " Incorrect TBM Headers " << endl;
				cout << "There have been " << std::dec << Incorrect_Trailer_Spy << " Incorrect TBM Trailers " << endl;
				cout << "There have been " << std::dec << Incorrect_ROC_Spy << " incorrect Number of ROCs " << endl;
				cout << "There have been " << std::dec << Incorrect_Pixel_Spy << " incorrect Number of Pixels " << endl;
				cout << " " << endl;
				for(uint32_t j = 0; j != Error_Event_Num.size(); j++){
					cout << "Error Event: " << std::hex << Error_Event_Num[j] << " Data: " << Error_Data_Num[j] << endl;
				}
				cout << "Read out all Error Events in Spy FIFO" << endl;
				for(uint32_t j = 0; j != Spy_Error.size(); j++)
					cout << "Spy_FIFO[" << std::dec << j << "]: " << std::hex << Spy_Error[j] << endl;
			}
      			
 			Compare_Event_FIFO1 = FIFO1_Analyze[0];
 			Total_Events_FIFO1 += FIFO1_Analyze[1];
 			Incorrect_Event_Num_FIFO1 += FIFO1_Analyze[2];
 			Incorrect_Header_FIFO1 += FIFO1_Analyze[3];
 			Incorrect_Trailer_FIFO1 += FIFO1_Analyze[4];

			if(q == (Outer_Loops - 1) && l == (Inner_Loops - 1)){
        			cout << "There have been " << std::dec << Total_Events_FIFO1 << " Events in FIFO1" << endl;
        			cout << "There have been " << std::dec << Incorrect_Event_Num_FIFO1 << " Incorrect Event Numbers " << endl;
        			cout << "There have been " << std::dec << Incorrect_Header_FIFO1 << " Incorrect TBM Headers " << endl;
        			cout << "There have been " << std::dec << Incorrect_Trailer_FIFO1 << " Incorrect TBM Trailers " << endl;
        			cout << " " << endl;
			}
      
			Total_Events_Error += Error_Analyze[0];
			Incorrect_Event_Num_Error += Error_Analyze[1];
			Timeout_Error_Error += Error_Analyze[2];
      
			if(q == (Outer_Loops - 1) && l == (Inner_Loops - 1)){
        			cout << "There were " << Total_Events_Error << " Events in Error FIFO. " << endl;
        			cout << "There were " << Incorrect_Event_Num_Error << " Incorrect Event numbers." << endl;
        			cout << "There were " << Timeout_Error_Error << " timeout errors." << endl;
        			cout << " " << endl;
			}
 
			Compare_Event_DDR = DDR_Analyze[0];       		
			Total_Events_DDR += DDR_Analyze[1];
			Incorrect_Event_Num_DDR += DDR_Analyze[2];
			Timeout_Error_DDR = DDR_Analyze[3];
			pixel_hit_DDR += DDR_Analyze[4];
			BX_read.push_back(DDR_Analyze[5]); 
      	
			if(q == (Outer_Loops - 1) && l == (Inner_Loops - 1)){
        			cout << "There were " << Total_Events_DDR << " Events in DDR. " << endl;
        			cout << "There were " << Incorrect_Event_Num_DDR << " Incorrect Event numbers." << endl;
        			cout << "There were " << Timeout_Error_DDR << " timeout errors." << endl;
        			cout << "There were " << pixel_hit_DDR << " pixel hits." << endl;
        			cout << " " << endl;
				if(Choice == 1){
					for(uint32_t i = 0; i != BX_read.size(); i++){
						if(BX_read[i] != 503)
							cout << "FED BX: " << BX_read[i] << " at L1A Delay " << BX_input[i] << "Event: " << i << endl;
					}
				}
				else if(Choice == 6){
					for(uint32_t it = 7; it != DDR_Analyze.size(); it++){
						if(((it - 7) % 8) == 0) channel++;
						cout << "Channel: " << channel << " ROC: " << ((it - 7) % 8 + 1) << " Num Hits: " << DDR_Analyze[it] << endl;
					}
				}
			}
		}//Inner Loops
		if(Choice == 3){ 
			cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.trailer_timeout_valid", 0);
			cAmc13Controller.fAmc13Interface->DisableMaskFEDSlot();
		}
	}//Outer Loops
}

void TTCHistoryCount( int TTCHistoryInt, int Loops){
    if(TTCHistoryInt != (Loops + 2) && TTCHistoryInt != (Loops + 1)){
        cout << "The number of commands in TTC History " << std::dec << TTCHistoryInt << " doesn't match the expected " << Loops + 2 << endl;
        throw QuitNow();
    }
}

void Test_TTC_History( int Loops ){
	using namespace std;
	const char* cHWFile;
	std::string filename ("settings/HWDescription_TTC_History.xml");
	cHWFile = filename.c_str();

	uhal::setLogLevelTo(uhal::Debug());
	SystemController cSystemController;
	Amc13Controller  cAmc13Controller;

	cSystemController.InitializeSettings(cHWFile, cout);
	cAmc13Controller.InitializeAmc13( cHWFile, cout );
	cSystemController.InitializeHw(cHWFile, cout);
	
	auto cSetting = cSystemController.fSettingsMap.find("NAcq");
	int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
	cSetting = cSystemController.fSettingsMap.find("BlockSize");
	int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;
	
	cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
	int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
	
	cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
	int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
	
	cAmc13Controller.ConfigureAmc13( cout );
	cSystemController.ConfigureHw(cout );
	
	cout << "Configure OK" << endl;

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

	std::vector<std::vector<uint32_t>> Spy_Analyze(2);
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

	cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
	cAmc13Controller.fAmc13Interface->SendEC0();
	//cAmc13Controller.fAmc13Interface->SendBGO();
	cAmc13Controller.fAmc13Interface->EnableBGO(0);
    cAmc13Controller.fAmc13Interface->DisableBGO(0);
    //cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.ttc.history_ttc_bc0_mask", 1);
    //cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.ttc.history_ttc_ec0_mask", 1);


	for (int l = 0; l < Loops; l++)
	{
		if(l != 0) cAmc13Controller.fAmc13Interface->SendBGO();

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
	
		if(l == (Loops - 1) || (l % 500 == 0 && l != 0)){
			//cAmc13Controller.fAmc13Interface->DumpHistory(512);

			//cout << "-> ttc.history_fifo_read_rdy :" << cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.ttc.history_fifo_read_rdy") << endl;
			//cout << "-> TTC_HISTORY_FIFO_wr_data_count :" << cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.ttc.history_wr_data_count") << endl;
			//
			//cout << "---> Force read" << endl;
			cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.ttc.history_fifo_force_read",1);
			int WordsNbToRead = cSystemController.fFEDInterface->ReadBoardReg(cFED,"pixfed_stat_regs.ttc.history_wr_data_count");
			//cout << "->" << WordsNbToRead << " words to read from ERR_FIFO..." << endl;
			std::vector< unsigned int > ERR_FIFO_rdData = cSystemController.fFEDInterface->ReadBlockBoardReg(cFED, "TTC_HISTORY_fifo", WordsNbToRead);
			for( int i = 0; i < WordsNbToRead; i++){
				//cout << std::hex << ERR_FIFO_rdData[i] << "Event :" << ((ERR_FIFO_rdData[i]&0xfffc0)>>6) << " BX: " << ((ERR_FIFO_rdData[i]&0xfff00000)>>20) << " B data: " << (ERR_FIFO_rdData[i]&0x3f) << endl;
				if((ERR_FIFO_rdData[i]&0x3f) == 0x1)
					EC_Reset_Num++;
				if((ERR_FIFO_rdData[i]&0x3f) == 0x14)
					TBM_Reset_Num++;
				if((ERR_FIFO_rdData[i]&0x3f) == 0x1C)
			    	ROC_Reset_Num++;
			}
			cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.ttc.history_fifo_read_done",1);
			usleep(10);
			//cout << "-> ttc.history_fifo_read_rdy :" << cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.ttc.history_fifo_read_rdy") << endl;
			cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.ttc.history_fifo_force_read",0);
			cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.ttc.history_fifo_read_done",0);
		}

	}//Loop loop

    TTCHistoryCount(TBM_Reset_Num, Loops);
	cout << "There have been " << std::dec << EC_Reset_Num << " ECRs" << endl;
	cout << "There have been " << std::dec << TBM_Reset_Num << " TBM Resets" << endl;
	cout << "There have been " << std::dec << ROC_Reset_Num << " ROC Resets" << endl;
}

void Memory_Readout( int Loops, int Max_Loops){
	using namespace std;
	const char* cHWFile;
	std::string filename ("settings/HWDescription_Memory.xml");
	cHWFile = filename.c_str();
	
	uhal::setLogLevelTo(uhal::Debug());
	SystemController cSystemController;
	Amc13Controller  cAmc13Controller;
	
	cSystemController.InitializeSettings(cHWFile, cout);
	cAmc13Controller.InitializeAmc13( cHWFile, cout );
	cSystemController.InitializeHw(cHWFile, cout);
	
	auto cSetting = cSystemController.fSettingsMap.find("NAcq");
	int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
	cSetting = cSystemController.fSettingsMap.find("BlockSize");
	int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;
	
	cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
	int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
	
	cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
	int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
	
	cAmc13Controller.ConfigureAmc13( cout );
	cSystemController.ConfigureHw(cout );
	
	cout << "Configure OK" << endl;
	
	
	auto& cFED = cSystemController.fPixFEDVector[0];
	
	cSystemController.fFEDInterface->getBoardInfo(cFED);
	cSystemController.fFEDInterface->findPhases(cFED);
	cSystemController.fFEDInterface->Start (cFED);
	
	std::vector<unsigned int> FIFO;
	std::vector<unsigned int> FIFO_CHB;
	std::vector<unsigned int> DDR;
	std::vector<std::vector<unsigned int>> FIFO1;
	std::vector<uint32_t> Error_FIFO;
	std::vector<std::vector<uint32_t>> Spy_Analyze(2);
	std::vector<uint32_t> DDR_Analyze;

 	int Compare_Event_Spy = 0;
        int Total_Events = 0;
	int TBM_Reset_Num = 0;
	int Compare_Event_DDR = 0;

	uint64_t TBM_Mask_1 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_1");
	uint64_t TBM_Mask_2 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_2");
	uint64_t TBM_Mask = (TBM_Mask_2 << 32) | TBM_Mask_1;
	
	std::fill(Error_FIFO.begin(), Error_FIFO.end(), 0);
	std::fill(FIFO.begin(), FIFO.end(), 0);
	std::fill(FIFO_CHB.begin(), FIFO_CHB.end(), 0);
	std::fill(DDR.begin(), DDR.end(), 0);
	
	cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
	cAmc13Controller.fAmc13Interface->SendEC0();
	cAmc13Controller.fAmc13Interface->EnableBGO(0);
	sleep(0.001);
	cAmc13Controller.fAmc13Interface->DisableBGO(0);

	for (int j = 0; j < Max_Loops; j++){
		for (int l = 0; l < Loops; l++){
			if(l%5000 == 0) cout << "Loop Number = " << std::dec << l << endl;
			
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
			Compare_Event_Spy = Spy_Analyze[0][0];
			Total_Events += Spy_Analyze[0][1];
						
			if((l % (Loops - 1)) == 0 && l != 0){
				cSystemController.fFEDInterface->readTTSState(cFED);
				DDR = cSystemController.fFEDInterface->ReadData(cFED, 0 );
			}
		}//Loop loop
	}//j loop
		 
	cout << "There have been " << std::dec << Total_Events << " Events" << endl;
	cout << "There have been " << std::dec << TBM_Reset_Num << " TBM resets" << endl;			
}

void Random_Bit_Flip( int Loops, int Random_Threshold, int Random_Value){
	using namespace std;
	const char* cHWFile;
	
	std::string filename ("settings/HWDescription_Delay.xml");
	cHWFile = filename.c_str();
	
	string ROCSA[ 8 ] = {"CHA_ROC0_", "CHA_ROC1_", "CHA_ROC2_", "CHA_ROC3_", "CHA_ROC4_", "CHA_ROC5_", "CHA_ROC6_", "CHA_ROC7_"};
	string ROCSB[ 8 ] = {"CHB_ROC0_", "CHB_ROC1_", "CHB_ROC2_", "CHB_ROC3_", "CHB_ROC4_", "CHB_ROC5_", "CHB_ROC6_", "CHB_ROC7_"};
	string index[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};
	
	uhal::setLogLevelTo(uhal::Debug());
	
	SystemController cSystemController;
	Amc13Controller  cAmc13Controller;
	
	cSystemController.InitializeSettings(cHWFile, cout);
	
	cAmc13Controller.InitializeAmc13( cHWFile, cout );
	cSystemController.InitializeHw(cHWFile, cout);
	
	auto cSetting = cSystemController.fSettingsMap.find("NAcq");
	int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
	cSetting = cSystemController.fSettingsMap.find("BlockSize");
	int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;
	
	cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
	int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
	
	cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
	int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
	
	cAmc13Controller.ConfigureAmc13( cout );
	cSystemController.ConfigureHw(cout );
	
	cout << "Configure OK" << endl;

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
	
	std::vector<std::vector<uint32_t>> Spy_Analyze(2);
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
	
	uint64_t TBM_Mask_1 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_1");
	uint64_t TBM_Mask_2 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_2");
	uint64_t TBM_Mask = (TBM_Mask_2 << 32) | TBM_Mask_1;
	
	cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
	cAmc13Controller.fAmc13Interface->SendEC0();
	cAmc13Controller.fAmc13Interface->EnableBGO(0);
	sleep(0.001);
	cAmc13Controller.fAmc13Interface->DisableBGO(0);

	for (int l = 0; l < Loops; l++){
		cSystemController.fFEDInterface->WriteBoardReg(cFED2, "Random_Reset", 1);
		cSystemController.fFEDInterface->WriteBoardReg(cFED2, "Random_Threshold", Random_Threshold);
		cSystemController.fFEDInterface->WriteBoardReg(cFED2, "Random_Reset", 0);
            
		cSystemController.fFEDInterface->WriteBoardReg(cFED3, "Random_Reset", 1);
		cSystemController.fFEDInterface->WriteBoardReg(cFED3, "Random_Threshold", Random_Threshold);
		cSystemController.fFEDInterface->WriteBoardReg(cFED3, "Random_Reset", 0);
            
		cSystemController.fFEDInterface->WriteBoardReg(cFED4, "Random_Reset", 1);
		cSystemController.fFEDInterface->WriteBoardReg(cFED4, "Random_Threshold", Random_Threshold);
		cSystemController.fFEDInterface->WriteBoardReg(cFED4, "Random_Reset", 0);

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
		Error_Analyze = Error_Event(Error_FIFO, Timeout_Error_Error, TBM_Mask, 0);
		DDR_Analyze = DDR_Event(DDR, Compare_Event_DDR, TBM_Mask, Timeout_Error_DDR, 0, 0);

		Compare_Event_Spy = Spy_Analyze[0][0];
		Total_Events_Spy += Spy_Analyze[0][1];
		Incorrect_Event_Num_Spy  += Spy_Analyze[0][2];
		Incorrect_Header_Spy += Spy_Analyze[0][3];
		Incorrect_Trailer_Spy  += Spy_Analyze[0][4];
		Incorrect_ROC_Spy += Spy_Analyze[0][5];
		Incorrect_Pixel_Spy += Spy_Analyze[0][6];
		
		if(l == (Loops - 1)){
			cout << "There have been " << std::dec << Total_Events_Spy << " Events" << endl;
			cout << "There have been " << std::dec << Incorrect_Event_Num_Spy << " Incorrect Event Numbers " << endl;
			cout << "There have been " << std::dec << Incorrect_Header_Spy << " Incorrect TBM Headers " << endl;
			cout << "There have been " << std::dec << Incorrect_Trailer_Spy << " Incorrect TBM Trailers " << endl;
			cout << "There have been " << std::dec << Incorrect_ROC_Spy << " incorrect Number of ROCs " << endl;
			cout << "There have been " << std::dec << Incorrect_Pixel_Spy << " incorrect Number of Pixels " << endl;
			cout << " " << endl;
		}
		
		Compare_Event_FIFO1 = FIFO1_Analyze[0];
		Total_Events_FIFO1 += FIFO1_Analyze[1];
		Incorrect_Event_Num_FIFO1 += FIFO1_Analyze[2];
		Incorrect_Header_FIFO1 += FIFO1_Analyze[3];
		Incorrect_Trailer_FIFO1 += FIFO1_Analyze[4];
		
		if(l == (Loops - 1)){
			cout << "There have been " << std::dec << Total_Events_FIFO1 << " Events" << endl;
			cout << "There have been " << std::dec << Incorrect_Event_Num_FIFO1 << " Incorrect Event Numbers " << endl;
			cout << "There have been " << std::dec << Incorrect_Header_FIFO1 << " Incorrect TBM Headers " << endl;
			cout << "There have been " << std::dec << Incorrect_Trailer_FIFO1 << " Incorrect TBM Trailers " << endl;
			cout << " " << endl;
		}
		
		Total_Events_Error += Error_Analyze[0];
		Incorrect_Event_Num_Error += Error_Analyze[1];
		Timeout_Error_Error += Error_Analyze[2];
		
		if(l == (Loops - 1)){
			cout << "There were " << Total_Events_Error << " Events in Error FIFO. " << endl;
			cout << "There were " << Incorrect_Event_Num_Error << " Incorrect Event numbers." << endl;
			cout << "There were " << Timeout_Error_Error << " timeout errors." << endl;
			cout << " " << endl;
		}

		Compare_Event_DDR = DDR_Analyze[0];
		Total_Events_DDR += DDR_Analyze[1];
		Incorrect_Event_Num_DDR += DDR_Analyze[2];
		Timeout_Error_DDR = DDR_Analyze[3];
		pixel_hit_DDR += DDR_Analyze[4];
		
		if(l == (Loops - 1)){
			cout << "There were " << Total_Events_DDR << " Events in DDR. " << endl;
			cout << "There were " << Incorrect_Event_Num_DDR << " Incorrect Event numbers." << endl;
			cout << "There were " << Timeout_Error_DDR << " timeout errors." << endl;
			cout << "There were " << pixel_hit_DDR << " pixel hits." << endl;
			cout << " " << endl;
		}
	}
}

void Error_Counters(int Choice, int Variation, int Loops){ 
	using namespace std;
	const char* cHWFile;
	std::string filename ("settings/HWDescription_Counters.xml");
	cHWFile = filename.c_str();
	uhal::setLogLevelTo(uhal::Debug());
	
	SystemController cSystemController;
	Amc13Controller  cAmc13Controller;
	
	cSystemController.InitializeSettings(cHWFile, cout);
	cAmc13Controller.InitializeAmc13( cHWFile, cout );
	cSystemController.InitializeHw(cHWFile, cout);
	
	auto cSetting = cSystemController.fSettingsMap.find("NAcq");
	int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
	cSetting = cSystemController.fSettingsMap.find("BlockSize");
	int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;
	
	cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
	int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
	
	cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
	int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
	
	cAmc13Controller.ConfigureAmc13( cout );
	cSystemController.ConfigureHw(cout );
	
	cout << "Configure OK" << endl;

	auto& cFED = cSystemController.fPixFEDVector[0];
	cSystemController.fFEDInterface->getBoardInfo(cFED);
	cSystemController.fFEDInterface->findPhases(cFED);
	cSystemController.fFEDInterface->Start (cFED);

	auto& cFED1 = cSystemController.fPixFEDVector[1];
	auto& cFED2 = cSystemController.fPixFEDVector[2];
	auto& cFED3 = cSystemController.fPixFEDVector[3];

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
	
	std::vector<std::vector<uint32_t>> Spy_Analyze(2);
	std::vector<uint32_t> FIFO1_Analyze;
	std::vector<uint32_t> Error_Analyze;
	std::vector<uint32_t> DDR_Analyze;
	std::vector<std::pair<std::string, uint32_t> > Counter_reg;

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
    int Total_OVF_Error = 0;

	uint64_t TBM_Mask_1 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_1");
	uint64_t TBM_Mask_2 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_2");
	uint64_t TBM_Mask = (TBM_Mask_2 << 32) | TBM_Mask_1;
    int MarkerCount = Marker_Read();

	typedef std::vector<std::pair<std::string, uint32_t> > vector_type;

	cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
	cAmc13Controller.fAmc13Interface->SendEC0();
	cAmc13Controller.fAmc13Interface->EnableBGO(0);
	sleep(0.001);
	cAmc13Controller.fAmc13Interface->DisableBGO(0);

	if(Choice == 0 || Choice == 4 || Choice == 7){
		cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.trailer_timeout_valid", 0);
		cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.trailerTimeoutValue", 0xf0);
	}
    else if(Choice == 6) PKAM_Enable(1, 20);

	for (int l = 0; l < Loops; l++){

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

		if(Choice == 0){
            Counter_reg = cSystemController.fFEDInterface->EventErrorDump(cFED, 0);
            for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
                cout << pos->first << " " << pos->second << endl;
            Counter_reg = cSystemController.fFEDInterface->TOCountDump(cFED, 0);
            for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
                cout << pos->first << " " << pos->second << endl;
            Counter_reg = cSystemController.fFEDInterface->ENECountDump(cFED, 0);
        }
        else if(Choice == 1) Counter_reg = cSystemController.fFEDInterface->EventTimeoutDump(cFED, 0);
		else if(Choice == 2) Counter_reg = cSystemController.fFEDInterface->EventResyncAheadDump(cFED, 0);
		else if(Choice == 3) Counter_reg = cSystemController.fFEDInterface->TBMHeaderIDErrorDump(cFED, 0);
		else if(Choice == 4) Counter_reg = cSystemController.fFEDInterface->TBMTrailerIDErrorDump(cFED, 0);
		else if(Choice == 5) Counter_reg = cSystemController.fFEDInterface->OOSCountErrorDump(cFED, 0);
        else if(Choice == 6){ 
            Counter_reg = cSystemController.fFEDInterface->PKAMCountDump(cFED, 0);
            for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
                cout << pos->first << " " << pos->second << endl;
            Counter_reg = cSystemController.fFEDInterface->NTPCountDump(cFED, 0);
            for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
                cout << pos->first << " " << pos->second << endl;
            Counter_reg = cSystemController.fFEDInterface->ROC_NBR_ERRCountDump(cFED, 0);
        }
		else if(Choice == 7) Counter_reg = cSystemController.fFEDInterface->OVFCountDump(cFED, 0);
		
        for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
			cout << pos->first << " " << pos->second << endl;
        
		Spy_Analyze = Spy_FIFO_Event(FIFO, FIFO_CHB, Compare_Event_Spy);
		FIFO1_Analyze = FIFO1_Event(FIFO1[0], FIFO1[1], Compare_Event_FIFO1);
		Error_Analyze = Error_Event(Error_FIFO, Timeout_Error_Error, TBM_Mask, MarkerCount);
		DDR_Analyze = DDR_Event(DDR, Compare_Event_DDR, TBM_Mask, Timeout_Error_DDR, MarkerCount, 0);

		Compare_Event_Spy = Spy_Analyze[0][0];
		Total_Events_Spy += Spy_Analyze[0][1];
		Incorrect_Event_Num_Spy  += Spy_Analyze[0][2];
		Incorrect_Header_Spy += Spy_Analyze[0][3];
		Incorrect_Trailer_Spy  += Spy_Analyze[0][4];
		Incorrect_ROC_Spy += Spy_Analyze[0][5];
		Incorrect_Pixel_Spy += Spy_Analyze[0][6];
		
		if(l == (Loops - 1)){
			cout << "There have been " << std::dec << Total_Events_Spy << " Events" << endl;
			cout << "There have been " << std::dec << Incorrect_Event_Num_Spy << " Incorrect Event Numbers " << endl;
			cout << "There have been " << std::dec << Incorrect_Header_Spy << " Incorrect TBM Headers " << endl;
			cout << "There have been " << std::dec << Incorrect_Trailer_Spy << " Incorrect TBM Trailers " << endl;
			cout << "There have been " << std::dec << Incorrect_ROC_Spy << " incorrect Number of ROCs " << endl;
			cout << "There have been " << std::dec << Incorrect_Pixel_Spy << " incorrect Number of Pixels " << endl;
			cout << " " << endl;
		}
		
		Compare_Event_FIFO1 = FIFO1_Analyze[0];
		Total_Events_FIFO1 += FIFO1_Analyze[1];
		Incorrect_Event_Num_FIFO1 += FIFO1_Analyze[2];
		Incorrect_Header_FIFO1 += FIFO1_Analyze[3];
		Incorrect_Trailer_FIFO1 += FIFO1_Analyze[4];
		
		if(l == (Loops - 1)){
			cout << "There have been " << std::dec << Total_Events_FIFO1 << " Events" << endl;
			cout << "There have been " << std::dec << Incorrect_Event_Num_FIFO1 << " Incorrect Event Numbers " << endl;
			cout << "There have been " << std::dec << Incorrect_Header_FIFO1 << " Incorrect TBM Headers " << endl;
			cout << "There have been " << std::dec << Incorrect_Trailer_FIFO1 << " Incorrect TBM Trailers " << endl;
			cout << " " << endl;
		}
		
		Total_Events_Error += Error_Analyze[0];
		Incorrect_Event_Num_Error += Error_Analyze[1];
		Timeout_Error_Error += Error_Analyze[2];
	    Total_OVF_Error += Error_Analyze[3];

		if(l == (Loops - 1)){
			cout << "There were " << Total_Events_Error << " Events in Error FIFO. " << endl;
			cout << "There were " << Incorrect_Event_Num_Error << " Incorrect Event numbers." << endl;
			cout << "There were " << Timeout_Error_Error << " timeout errors." << endl;
			cout << "There were " << Total_OVF_Error << " OVF in Error FIFO." << endl;
			cout << " " << endl;
		}
		
		Compare_Event_DDR = DDR_Analyze[0];
		Total_Events_DDR += DDR_Analyze[1];
		Incorrect_Event_Num_DDR += DDR_Analyze[2];
		Timeout_Error_DDR = DDR_Analyze[3];
		pixel_hit_DDR += DDR_Analyze[4];
		
		if(l == (Loops - 1)){
			cout << "There were " << Total_Events_DDR << " Events in DDR. " << endl;
			cout << "There were " << Incorrect_Event_Num_DDR << " Incorrect Event numbers." << endl;
			cout << "There were " << Timeout_Error_DDR << " timeout errors." << endl;
			cout << "There were " << pixel_hit_DDR << " pixel hits." << endl;
			cout << " " << endl;
		}
	}

    Delay_Time(1);
    Marker_Error(0x55555555, 0, 0x7F8, 1);
}

void Stacked_Events(int Loops){ 
	using namespace std;
	const char* cHWFile;
	std::string filename ("settings/HWDescription_Stack.xml");
	cHWFile = filename.c_str();
	uhal::setLogLevelTo(uhal::Debug());
	
	SystemController cSystemController;
	Amc13Controller  cAmc13Controller;
	
	cSystemController.InitializeSettings(cHWFile, cout);
	cAmc13Controller.InitializeAmc13( cHWFile, cout );
	cSystemController.InitializeHw(cHWFile, cout);
	
	auto cSetting = cSystemController.fSettingsMap.find("NAcq");
	int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
	cSetting = cSystemController.fSettingsMap.find("BlockSize");
	int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;
	
	cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
	int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
	
	cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
	int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
	
	cAmc13Controller.ConfigureAmc13( cout );
	cSystemController.ConfigureHw(cout );
	
	cout << "Configure OK" << endl;

	auto& cFED = cSystemController.fPixFEDVector[0];
	cSystemController.fFEDInterface->getBoardInfo(cFED);
	cSystemController.fFEDInterface->findPhases(cFED);
	cSystemController.fFEDInterface->Start (cFED);

	auto& cFED1 = cSystemController.fPixFEDVector[1];
	auto& cFED2 = cSystemController.fPixFEDVector[2];
	auto& cFED3 = cSystemController.fPixFEDVector[3];

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
	
	std::vector<std::vector<uint32_t>> Spy_Analyze(2);
	std::vector<uint32_t> FIFO1_Analyze;
	std::vector<uint32_t> Error_Analyze;
	std::vector<uint32_t> DDR_Analyze;
	std::vector<std::pair<std::string, uint32_t> > Counter_reg;

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
    int Total_OVF_Error = 0;

	uint64_t TBM_Mask_1 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_1");
	uint64_t TBM_Mask_2 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_2");
	uint64_t TBM_Mask = (TBM_Mask_2 << 32) | TBM_Mask_1;
    int MarkerCount = Marker_Read();

	typedef std::vector<std::pair<std::string, uint32_t> > vector_type;

	cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
	cAmc13Controller.fAmc13Interface->SendEC0();
	cAmc13Controller.fAmc13Interface->EnableBGO(0);
	sleep(0.001);
	cAmc13Controller.fAmc13Interface->DisableBGO(0);

	for (int l = 0; l < Loops; l++){

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
		Error_Analyze = Error_Event(Error_FIFO, Timeout_Error_Error, TBM_Mask, MarkerCount);
		DDR_Analyze = DDR_Event(DDR, Compare_Event_DDR, TBM_Mask, Timeout_Error_DDR, MarkerCount, 0);

		Compare_Event_Spy = Spy_Analyze[0][0];
		Total_Events_Spy += Spy_Analyze[0][1];
		Incorrect_Event_Num_Spy  += Spy_Analyze[0][2];
		Incorrect_Header_Spy += Spy_Analyze[0][3];
		Incorrect_Trailer_Spy  += Spy_Analyze[0][4];
		Incorrect_ROC_Spy += Spy_Analyze[0][5];
		Incorrect_Pixel_Spy += Spy_Analyze[0][6];
		
		if(l == (Loops - 1)){
			cout << "There have been " << std::dec << Total_Events_Spy << " Events" << endl;
			cout << "There have been " << std::dec << Incorrect_Event_Num_Spy << " Incorrect Event Numbers " << endl;
			cout << "There have been " << std::dec << Incorrect_Header_Spy << " Incorrect TBM Headers " << endl;
			cout << "There have been " << std::dec << Incorrect_Trailer_Spy << " Incorrect TBM Trailers " << endl;
			cout << "There have been " << std::dec << Incorrect_ROC_Spy << " incorrect Number of ROCs " << endl;
			cout << "There have been " << std::dec << Incorrect_Pixel_Spy << " incorrect Number of Pixels " << endl;
			cout << " " << endl;
		}
		
		Compare_Event_FIFO1 = FIFO1_Analyze[0];
		Total_Events_FIFO1 += FIFO1_Analyze[1];
		Incorrect_Event_Num_FIFO1 += FIFO1_Analyze[2];
		Incorrect_Header_FIFO1 += FIFO1_Analyze[3];
		Incorrect_Trailer_FIFO1 += FIFO1_Analyze[4];
		
		if(l == (Loops - 1)){
			cout << "There have been " << std::dec << Total_Events_FIFO1 << " Events" << endl;
			cout << "There have been " << std::dec << Incorrect_Event_Num_FIFO1 << " Incorrect Event Numbers " << endl;
			cout << "There have been " << std::dec << Incorrect_Header_FIFO1 << " Incorrect TBM Headers " << endl;
			cout << "There have been " << std::dec << Incorrect_Trailer_FIFO1 << " Incorrect TBM Trailers " << endl;
			cout << " " << endl;
		}
		
		Total_Events_Error += Error_Analyze[0];
		Incorrect_Event_Num_Error += Error_Analyze[1];
		Timeout_Error_Error += Error_Analyze[2];
	    Total_OVF_Error += Error_Analyze[3];

		if(l == (Loops - 1)){
			cout << "There were " << Total_Events_Error << " Events in Error FIFO. " << endl;
			cout << "There were " << Incorrect_Event_Num_Error << " Incorrect Event numbers." << endl;
			cout << "There were " << Timeout_Error_Error << " timeout errors." << endl;
			cout << "There were " << Total_OVF_Error << " OVF in Error FIFO." << endl;
			cout << " " << endl;
		}
		
		Compare_Event_DDR = DDR_Analyze[0];
		Total_Events_DDR += DDR_Analyze[1];
		Incorrect_Event_Num_DDR += DDR_Analyze[2];
		Timeout_Error_DDR = DDR_Analyze[3];
		pixel_hit_DDR += DDR_Analyze[4];
		
		if(l == (Loops - 1)){
			cout << "There were " << Total_Events_DDR << " Events in DDR. " << endl;
			cout << "There were " << Incorrect_Event_Num_DDR << " Incorrect Event numbers." << endl;
			cout << "There were " << Timeout_Error_DDR << " timeout errors." << endl;
			cout << "There were " << pixel_hit_DDR << " pixel hits." << endl;
			cout << " " << endl;
		}
	}

    Marker_Error(0x55555555, 0, 0x7F8, 1);
}

void SEU_Analyze( int type, int mode, int Which_Chan, int Loops ){
	using namespace std;
	const char* cHWFile;
	std::string filename ("settings/HWDescription_SEU.xml");
	cHWFile = filename.c_str();
	uhal::setLogLevelTo(uhal::Debug());
	
	SystemController cSystemController;
	Amc13Controller  cAmc13Controller;
	
	cSystemController.InitializeSettings(cHWFile, cout);
	cAmc13Controller.InitializeAmc13( cHWFile, cout );
	cSystemController.InitializeHw(cHWFile, cout);
	
	auto cSetting = cSystemController.fSettingsMap.find("NAcq");
	int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
	cSetting = cSystemController.fSettingsMap.find("BlockSize");
	int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;
	
	cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
	int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
	
	cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
	int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
	
	cAmc13Controller.ConfigureAmc13( cout );
	cSystemController.ConfigureHw(cout );
	
	cout << "Configure OK" << endl;

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
	std::vector<std::vector<uint32_t>> Spy_Analyze(2);
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
	int TTSState = 0;
	
	uint64_t TBM_Mask_1 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_1");
	uint64_t TBM_Mask_2 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_2");
	uint64_t SEU_Status_Mask_1 = 0;
	uint64_t SEU_Status_Mask_2 = 0;
	uint64_t SEU_Status_Mask   = 0;
    uint64_t TBM_Mask = (TBM_Mask_2 << 32) | TBM_Mask_1;
    int SEUCount = SEU_Read();	
	
    cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.TO_FEROL_MASK_CH_WORD_valid", 1);
	cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
	cAmc13Controller.fAmc13Interface->SendEC0();
	cAmc13Controller.fAmc13Interface->EnableBGO(0);
	sleep(0.001);
	cAmc13Controller.fAmc13Interface->DisableBGO(0);
	
	for (int l = 0; l < Loops; l++){

		if(mode == 0 && l == 1){
			int MODE = rand() % 3 + 1;
			int TYPE = rand() % 2;
			SEU(1, TYPE, MODE, Which_Chan);
		    SEUCount = SEU_Read();
        }
		else if(l == 1){
            SEU(1, type, mode, Which_Chan);
            usleep(1);
		    SEUCount = SEU_Read();
        }
		else{
			TTSState = cSystemController.fFEDInterface->readTTSState(cFED);
			SEU_Status_Mask_1 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.MASK_STATUS_CH_32to01");
			SEU_Status_Mask_2 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.MASK_STATUS_CH_48to33");
            SEU_Status_Mask = (SEU_Status_Mask_2 << 32) | SEU_Status_Mask_1;
            cout << "SEU_Status_Mask: 0x" << std::hex << SEU_Status_Mask << " TBM_Mask: 0x" << TBM_Mask << endl;
            if((TTSState == 2) && (SEU_Status_Mask != TBM_Mask)){
				for(int i = 0; i < 20; i++){
                  cAmc13Controller.fAmc13Interface->SendEC0();
                  cAmc13Controller.fAmc13Interface->EnableBGO(0);
                  sleep(0.001);
                  cAmc13Controller.fAmc13Interface->DisableBGO(0);
                }
                //SEU(1, type, mode, Which_Chan);
			}
            else if(TTSState == 2){
				cAmc13Controller.fAmc13Interface->SendEC0();
				cAmc13Controller.fAmc13Interface->EnableBGO(0);
				sleep(0.001);
				cAmc13Controller.fAmc13Interface->DisableBGO(0);
                sleep(1);
            }
            else if((TTSState == 8) && (SEU_Status_Mask != TBM_Mask)){
                cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.SEU_recovery_valid", 1);
                cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_1", TBM_Mask_1);
                cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_2", TBM_Mask_2);
				for(int i = 0; i < 20; i++){
                  cAmc13Controller.fAmc13Interface->SendEC0();
                  cAmc13Controller.fAmc13Interface->EnableBGO(0);
                  sleep(0.1);
                  cAmc13Controller.fAmc13Interface->DisableBGO(0);
                }
                SEU(0, type, mode, Which_Chan);
                sleep(5);
            }
		}
	
		if(l == 0){	
			cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
			sleep(4);
			cAmc13Controller.fAmc13Interface->BurstL1A();
		}
		else cAmc13Controller.fAmc13Interface->BurstL1A();
		sleep(0.001);
		
		//FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
		//FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);
		//FIFO1 = cSystemController.fFEDInterface->readFIFO1_vec(cFED);
		cSystemController.fFEDInterface->readErrorFIFO(cFED, true);
		//Error_FIFO = cSystemController.fFEDInterface->readErrorFIFO_vec(cFED, true);
		cSystemController.fFEDInterface->readTTSState(cFED);
		DDR = cSystemController.fFEDInterface->ReadData(cFED, 0 );
		
		    
		//Spy_Analyze = Spy_FIFO_Event(FIFO, FIFO_CHB, Compare_Event_Spy);
		//FIFO1_Analyze = FIFO1_Event(FIFO1[0], FIFO1[1], Compare_Event_FIFO1);
		Error_Analyze = Error_Event(Error_FIFO, Timeout_Error_Error, TBM_Mask);
		DDR_Analyze = DDR_Event(DDR, Compare_Event_DDR, TBM_Mask, Timeout_Error_DDR, 0, SEUCount, SEU_Status_Mask);
		
		//Compare_Event_Spy = Spy_Analyze[0][0];
		//Total_Events_Spy += Spy_Analyze[0][1];
		//Incorrect_Event_Num_Spy  += Spy_Analyze[0][2];
		//Incorrect_Header_Spy += Spy_Analyze[0][3];
		//Incorrect_Trailer_Spy  += Spy_Analyze[0][4];
		//Incorrect_ROC_Spy += Spy_Analyze[0][5];
		//Incorrect_Pixel_Spy += Spy_Analyze[0][6];
		//
		//if(l == (Loops - 1)){
		//	cout << "There have been " << std::dec << Total_Events_Spy << " Events" << endl;
		//	cout << "There have been " << std::dec << Incorrect_Event_Num_Spy << " Incorrect Event Numbers " << endl;
		//	cout << "There have been " << std::dec << Incorrect_Header_Spy << " Incorrect TBM Headers " << endl;
		//	cout << "There have been " << std::dec << Incorrect_Trailer_Spy << " Incorrect TBM Trailers " << endl;
		//	cout << "There have been " << std::dec << Incorrect_ROC_Spy << " incorrect Number of ROCs " << endl;
		//	cout << "There have been " << std::dec << Incorrect_Pixel_Spy << " incorrect Number of Pixels " << endl;
		//	cout << " " << endl;
		//}
		//
		//Compare_Event_FIFO1 = FIFO1_Analyze[0];
		//Total_Events_FIFO1 += FIFO1_Analyze[1];
		//Incorrect_Event_Num_FIFO1 += FIFO1_Analyze[2];
		//Incorrect_Header_FIFO1 += FIFO1_Analyze[3];
		//Incorrect_Trailer_FIFO1 += FIFO1_Analyze[4];
		//
		//if(l == (Loops - 1)){
		//	cout << "There have been " << std::dec << Total_Events_FIFO1 << " Events" << endl;
		//	cout << "There have been " << std::dec << Incorrect_Event_Num_FIFO1 << " Incorrect Event Numbers " << endl;
		//	cout << "There have been " << std::dec << Incorrect_Header_FIFO1 << " Incorrect TBM Headers " << endl;
		//	cout << "There have been " << std::dec << Incorrect_Trailer_FIFO1 << " Incorrect TBM Trailers " << endl;
		//	cout << " " << endl;
		//}
		
		Total_Events_Error += Error_Analyze[0];
		Incorrect_Event_Num_Error += Error_Analyze[1];
		Timeout_Error_Error += Error_Analyze[2];
		
		if(l == (Loops - 1)){
			cout << "There were " << Total_Events_Error << " Events in Error FIFO. " << endl;
			cout << "There were " << Incorrect_Event_Num_Error << " Incorrect Event numbers." << endl;
			cout << "There were " << Timeout_Error_Error << " timeout errors." << endl;
			cout << " " << endl;
		}
		
		Compare_Event_DDR = DDR_Analyze[0];
		Total_Events_DDR += DDR_Analyze[1];
		Incorrect_Event_Num_DDR += DDR_Analyze[2];
		Timeout_Error_DDR = DDR_Analyze[3];
		pixel_hit_DDR += DDR_Analyze[4];
		
		if(l == (Loops - 1)){
			cout << "There were " << Total_Events_DDR << " Events in DDR. " << endl;
			cout << "There were " << Incorrect_Event_Num_DDR << " Incorrect Event numbers." << endl;
			cout << "There were " << Timeout_Error_DDR << " timeout errors." << endl;
			cout << "There were " << pixel_hit_DDR << " pixel hits." << endl;
			cout << " " << endl;
		}
	}
    cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.TO_FEROL_MASK_CH_WORD_valid", 0);
	SEU(0, type, mode, 0);
}

void Private_Resync(int Loops, int Delay){
	using namespace std;
	const char* cHWFile;

	std::string filename ("settings/HWDescription_Private_Resync.xml");
	cHWFile = filename.c_str();

	string ROCSA[ 8 ] = {"CHA_ROC0", "CHA_ROC1", "CHA_ROC2", "CHA_ROC3", "CHA_ROC4", "CHA_ROC5", "CHA_ROC6", "CHA_ROC7"};
	string ROCSB[ 8 ] = {"CHB_ROC0", "CHB_ROC1", "CHB_ROC2", "CHB_ROC3", "CHB_ROC4", "CHB_ROC5", "CHB_ROC6", "CHB_ROC7"};
	string index[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};

	uhal::setLogLevelTo(uhal::Debug());

	SystemController cSystemController;
	Amc13Controller  cAmc13Controller;

	cSystemController.InitializeSettings(cHWFile, cout);

	cAmc13Controller.InitializeAmc13( cHWFile, cout );
	cSystemController.InitializeHw(cHWFile, cout);

	auto cSetting = cSystemController.fSettingsMap.find("NAcq");
	int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
	cSetting = cSystemController.fSettingsMap.find("BlockSize");
	int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;

	cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
	int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 39;

	cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
	int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;

	cAmc13Controller.ConfigureAmc13( cout );
	cSystemController.ConfigureHw(cout );

	cout << "Configure OK" << endl;

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
	std::vector<uint32_t> FIFO1_marker;
	std::vector<uint32_t> Error_FIFO;
	std::vector<uint32_t> Spy_Array;
	std::vector<uint32_t> FIFO1_Array;
	std::vector<uint32_t> Error_Array;
	std::vector<uint32_t> DDR_Array;

	std::vector<std::vector<uint32_t>> Spy_Analyze(2);
	std::vector<uint32_t> FIFO1_Analyze;
	std::vector<uint32_t> Error_Analyze;
	std::vector<uint32_t> DDR_Analyze;
	std::vector<std::pair<std::string, uint32_t> > Counter_reg;
	typedef std::vector<std::pair<std::string, uint32_t> > vector_type;

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

	uint64_t TBM_Mask_1 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_1");
	uint64_t TBM_Mask_2 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_2");
	uint64_t TBM_Mask = (TBM_Mask_2 << 32) | TBM_Mask_1;

	cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
	cAmc13Controller.fAmc13Interface->SendEC0();
	cAmc13Controller.fAmc13Interface->EnableBGO(0);
	sleep(0.001);
	cAmc13Controller.fAmc13Interface->DisableBGO(0);

	//cAmc13Controller.fAmc13Interface->EnableSingleBGO(0);
	//cAmc13Controller.fAmc13Interface->SendBGO();
	
    for (int l = 0; l < Loops; l++){

        if( l % 1000 == 0) cout << "Loop number " << l << endl;
        if(l == 14){
            //cAmc13Controller.fAmc13Interface->SendBGO();
	        cAmc13Controller.fAmc13Interface->EnableBGO(0);
	        sleep(1);
	        cAmc13Controller.fAmc13Interface->DisableBGO(0);
            Delay_Time(1);
        }
        
        if(l == 0){
			cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
            Delay_Time(Delay);
			sleep(4);
			cAmc13Controller.fAmc13Interface->BurstL1A();
		}
		else{
			sleep(0.04);
			cAmc13Controller.fAmc13Interface->BurstL1A();
		}

		sleep(0.001);

	    cout << "RESYNC_MAX_TIME_DURATION: " << std::dec << cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.RESYNC_MAX_TIME_DURATION") << endl;
	    cout << "CNT_TTC_BC0: " << std::dec << cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.CNT_TTC_BC0") << endl;
	    cout << "CNT_TTC_EC0: " << std::dec << cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.CNT_TTC_EC0") << endl;
	    cout << "CNT_TTC_RESET: " << std::dec << cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.CNT_TTC_RESET") << endl;
	    cout << "CNT_TTC_CMD_STROBE: " << std::dec << cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.CNT_TTC_CMD_STROBE") << endl;
	    cout << "CNT_RESYNC_NBR: " << std::dec << cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.CNT_RESYNC_NBR") << endl;
        
        //FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
		//FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);
		//FIFO1 = cSystemController.fFEDInterface->readFIFO1_vec(cFED);
        cSystemController.fFEDInterface->TTCHistoryFIFO(cFED, true);
        Error_FIFO = cSystemController.fFEDInterface->readErrorFIFO_vec(cFED, true);
		cSystemController.fFEDInterface->readTTSState(cFED);
		DDR = cSystemController.fFEDInterface->ReadData(cFED, 0 );

		//Spy_Analyze = Spy_FIFO_Event(FIFO, FIFO_CHB, Compare_Event_Spy);
		//FIFO1_Analyze = FIFO1_Event(FIFO1[0], FIFO1[1], Compare_Event_FIFO1);
		Error_Analyze = Error_Event(Error_FIFO, Timeout_Error_Error, TBM_Mask);
		DDR_Analyze = DDR_Event(DDR, Compare_Event_DDR, TBM_Mask, Timeout_Error_DDR, 0, 0);

		//Compare_Event_Spy = Spy_Analyze[0][0];
		//Total_Events_Spy += Spy_Analyze[0][1];
		//Incorrect_Event_Num_Spy  += Spy_Analyze[0][2];
		//Incorrect_Header_Spy += Spy_Analyze[0][3];
		//Incorrect_Trailer_Spy  += Spy_Analyze[0][4];
		//Incorrect_ROC_Spy += Spy_Analyze[0][5];
		//Incorrect_Pixel_Spy += Spy_Analyze[0][6];

		//if(l == (Loops - 1)){
		//	cout << "There have been " << std::dec << Total_Events_Spy << " Events" << endl;
		//	cout << "There have been " << std::dec << Incorrect_Event_Num_Spy << " Incorrect Event Numbers " << endl;
		//	cout << "There have been " << std::dec << Incorrect_Header_Spy << " Incorrect TBM Headers " << endl;
		//	cout << "There have been " << std::dec << Incorrect_Trailer_Spy << " Incorrect TBM Trailers " << endl;
		//	cout << "There have been " << std::dec << Incorrect_ROC_Spy << " incorrect Number of ROCs " << endl;
		//	cout << "There have been " << std::dec << Incorrect_Pixel_Spy << " incorrect Number of Pixels " << endl;
		//	cout << " " << endl;
		//}

		//Compare_Event_FIFO1 = FIFO1_Analyze[0];
		//Total_Events_FIFO1 += FIFO1_Analyze[1];
		//Incorrect_Event_Num_FIFO1 += FIFO1_Analyze[2];
		//Incorrect_Header_FIFO1 += FIFO1_Analyze[3];
		//Incorrect_Trailer_FIFO1 += FIFO1_Analyze[4];

		//if(l == (Loops - 1)){
		//	cout << "There have been " << std::dec << Total_Events_FIFO1 << " Events" << endl;
	    //		cout << "There have been " << std::dec << Incorrect_Event_Num_FIFO1 << " Incorrect Event Numbers " << endl;
	    //		cout << "There have been " << std::dec << Incorrect_Header_FIFO1 << " Incorrect TBM Headers " << endl;
	    //		cout << "There have been " << std::dec << Incorrect_Trailer_FIFO1 << " Incorrect TBM Trailers " << endl;
	    //		cout << " " << endl;
		//}

		Total_Events_Error += Error_Analyze[0];
		Incorrect_Event_Num_Error += Error_Analyze[1];
		Timeout_Error_Error += Error_Analyze[2];

		if(l == (Loops - 1)){
			cout << "There were " << Total_Events_Error << " Events in Error FIFO. " << endl;
      			cout << "There were " << Incorrect_Event_Num_Error << " Incorrect Event numbers." << endl;
      			cout << "There were " << Timeout_Error_Error << " timeout errors." << endl;
			cout << " " << endl;
		}

		Compare_Event_DDR = DDR_Analyze[0];
		Total_Events_DDR += DDR_Analyze[1];
		Incorrect_Event_Num_DDR += DDR_Analyze[2];
		Timeout_Error_DDR = DDR_Analyze[3];
		pixel_hit_DDR += DDR_Analyze[4];

		if(l == (Loops - 1)){
			cout << "There were " << Total_Events_DDR << " Events in DDR. " << endl;
			cout << "There were " << Incorrect_Event_Num_DDR << " Incorrect Event numbers." << endl;
			cout << "There were " << Timeout_Error_DDR << " timeout errors." << endl;
			cout << "There were " << pixel_hit_DDR << " pixel hits." << endl;
			cout << " " << endl;
		}
	}
    cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.resync_mode", 0);
    cSystemController.fFEDInterface->WriteBoardReg(cFED, "pixfed_ctrl_regs.sw_TTS_PRQ", 0x2);
}


void TBM_Header_Idle(int Loops, int Choice){
	using namespace std;
	const char* cHWFile;

	std::string filename ("settings/HWDescription_Marker_Idle.xml");
	cHWFile = filename.c_str();

	string ROCSA[ 8 ] = {"CHA_ROC0", "CHA_ROC1", "CHA_ROC2", "CHA_ROC3", "CHA_ROC4", "CHA_ROC5", "CHA_ROC6", "CHA_ROC7"};
	string ROCSB[ 8 ] = {"CHB_ROC0", "CHB_ROC1", "CHB_ROC2", "CHB_ROC3", "CHB_ROC4", "CHB_ROC5", "CHB_ROC6", "CHB_ROC7"};
	string index[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};

	uhal::setLogLevelTo(uhal::Debug());

	SystemController cSystemController;
	Amc13Controller  cAmc13Controller;

	cSystemController.InitializeSettings(cHWFile, cout);

	cAmc13Controller.InitializeAmc13( cHWFile, cout );
	cSystemController.InitializeHw(cHWFile, cout);

	auto cSetting = cSystemController.fSettingsMap.find("NAcq");
	int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
	cSetting = cSystemController.fSettingsMap.find("BlockSize");
	int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;

	cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
	int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 39;

	cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
	int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;

	cAmc13Controller.ConfigureAmc13( cout );
	cSystemController.ConfigureHw(cout );

	cout << "Configure OK" << endl;

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
	std::vector<uint32_t> FIFO1_marker;
	std::vector<uint32_t> Error_FIFO;
	std::vector<uint32_t> Spy_Array;
	std::vector<uint32_t> FIFO1_Array;
	std::vector<uint32_t> Error_Array;
	std::vector<uint32_t> DDR_Array;

	std::vector<std::vector<uint32_t>> Spy_Analyze(2);
	std::vector<uint32_t> FIFO1_Analyze;
	std::vector<uint32_t> Error_Analyze;
	std::vector<uint32_t> DDR_Analyze;
	std::vector<std::pair<std::string, uint32_t> > Counter_reg;
	typedef std::vector<std::pair<std::string, uint32_t> > vector_type;

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

	uint64_t TBM_Mask_1 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_1");
	uint64_t TBM_Mask_2 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_2");
	uint64_t TBM_Mask = (TBM_Mask_2 << 32) | TBM_Mask_1;
    int MarkerCount = Marker_Idle_Read();

    if(Choice == 1) Delay_Time(50);

	cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
	cAmc13Controller.fAmc13Interface->SendEC0();
	cAmc13Controller.fAmc13Interface->EnableBGO(0);
	sleep(0.001);
	cAmc13Controller.fAmc13Interface->DisableBGO(0);

    for (int l = 0; l < Loops; l++){
        
        if(Choice == 1){
	        cAmc13Controller.fAmc13Interface->SendEC0();
	        cAmc13Controller.fAmc13Interface->EnableBGO(0);
	        sleep(0.001);
	        cAmc13Controller.fAmc13Interface->DisableBGO(0);

            cout << "Loop: " << std::dec << l << endl;
            Marker_Error_Idle( 0x55555555, 1, 0x7FC, 3, 50 + l + 13);
            Marker_Idle_Counter_Read();
        }

        if(l == 0){
			cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
			sleep(4);
			cAmc13Controller.fAmc13Interface->BurstL1A();
		}
		else{
			sleep(0.04);
			cAmc13Controller.fAmc13Interface->BurstL1A();
		}

		sleep(0.001);

        FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
		FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);
		FIFO1 = cSystemController.fFEDInterface->readFIFO1_vec(cFED);
        Error_FIFO = cSystemController.fFEDInterface->readErrorFIFO_vec(cFED, true);
		cSystemController.fFEDInterface->readTTSState(cFED);
		DDR = cSystemController.fFEDInterface->ReadData(cFED, 0 );

        cout << "L1ACtr: " << cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.L1A_counter.L1ACnt") << endl;
        cout << "L1ACtr32b: " << cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.L1A_counter.L1ACnt32b") << endl;
        Counter_reg = cSystemController.fFEDInterface->PacketCountDump(cFED, 0);
        for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
            cout << pos->first << " " << pos->second << endl;
        
        Counter_reg = cSystemController.fFEDInterface->TrailerCountDump(cFED, 0);
        for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
            cout << pos->first << " " << pos->second << endl;
        


		Spy_Analyze = Spy_FIFO_Event(FIFO, FIFO_CHB, Compare_Event_Spy);
		FIFO1_Analyze = FIFO1_Event(FIFO1[0], FIFO1[1], Compare_Event_FIFO1);
		Error_Analyze = Error_Event(Error_FIFO, Timeout_Error_Error, TBM_Mask);
		DDR_Analyze = DDR_Event(DDR, Compare_Event_DDR, TBM_Mask, Timeout_Error_DDR, MarkerCount, 0);

		Compare_Event_Spy = Spy_Analyze[0][0];
		Total_Events_Spy += Spy_Analyze[0][1];
		Incorrect_Event_Num_Spy  += Spy_Analyze[0][2];
		Incorrect_Header_Spy += Spy_Analyze[0][3];
		Incorrect_Trailer_Spy  += Spy_Analyze[0][4];
		Incorrect_ROC_Spy += Spy_Analyze[0][5];
		Incorrect_Pixel_Spy += Spy_Analyze[0][6];

		if(l == (Loops - 1)){
			cout << "There have been " << std::dec << Total_Events_Spy << " Events" << endl;
			cout << "There have been " << std::dec << Incorrect_Event_Num_Spy << " Incorrect Event Numbers " << endl;
			cout << "There have been " << std::dec << Incorrect_Header_Spy << " Incorrect TBM Headers " << endl;
			cout << "There have been " << std::dec << Incorrect_Trailer_Spy << " Incorrect TBM Trailers " << endl;
			cout << "There have been " << std::dec << Incorrect_ROC_Spy << " incorrect Number of ROCs " << endl;
			cout << "There have been " << std::dec << Incorrect_Pixel_Spy << " incorrect Number of Pixels " << endl;
			cout << " " << endl;
		}

		Compare_Event_FIFO1 = FIFO1_Analyze[0];
		Total_Events_FIFO1 += FIFO1_Analyze[1];
		Incorrect_Event_Num_FIFO1 += FIFO1_Analyze[2];
		Incorrect_Header_FIFO1 += FIFO1_Analyze[3];
		Incorrect_Trailer_FIFO1 += FIFO1_Analyze[4];

		if(l == (Loops - 1)){
			cout << "There have been " << std::dec << Total_Events_FIFO1 << " Events" << endl;
	    		cout << "There have been " << std::dec << Incorrect_Event_Num_FIFO1 << " Incorrect Event Numbers " << endl;
	    		cout << "There have been " << std::dec << Incorrect_Header_FIFO1 << " Incorrect TBM Headers " << endl;
	    		cout << "There have been " << std::dec << Incorrect_Trailer_FIFO1 << " Incorrect TBM Trailers " << endl;
	    		cout << " " << endl;
		}

		Total_Events_Error += Error_Analyze[0];
		Incorrect_Event_Num_Error += Error_Analyze[1];
		Timeout_Error_Error += Error_Analyze[2];

		if(l == (Loops - 1)){
			cout << "There were " << Total_Events_Error << " Events in Error FIFO. " << endl;
      			cout << "There were " << Incorrect_Event_Num_Error << " Incorrect Event numbers." << endl;
      			cout << "There were " << Timeout_Error_Error << " timeout errors." << endl;
			cout << " " << endl;
		}

		Compare_Event_DDR = DDR_Analyze[0];
		Total_Events_DDR += DDR_Analyze[1];
		Incorrect_Event_Num_DDR += DDR_Analyze[2];
		Timeout_Error_DDR = DDR_Analyze[3];
		pixel_hit_DDR += DDR_Analyze[4];

		if(l == (Loops - 1)){
			cout << "There were " << Total_Events_DDR << " Events in DDR. " << endl;
			cout << "There were " << Incorrect_Event_Num_DDR << " Incorrect Event numbers." << endl;
			cout << "There were " << Timeout_Error_DDR << " timeout errors." << endl;
			cout << "There were " << pixel_hit_DDR << " pixel hits." << endl;
			cout << " " << endl;
		}
	}
}


