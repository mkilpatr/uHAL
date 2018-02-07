std::vector<std::vector<uint32_t> > Spy_FIFO_Event(vector<uint32_t>& FIFO, vector<uint32_t>& FIFO_CHB, int Compare_Event_Spy){
    std::vector<std::vector<uint32_t>> Spy_Analyze(2);
    std::vector<uint32_t> Spy_Error;
    int Total_Events_Spy = 0;
    int Incorrect_Header_Spy = 0;
    int Incorrect_Trailer_Spy = 0;
    int Incorrect_Event_Num_Spy = 0;
    int Incorrect_ROC_Spy = 0;
    int ROC_Value_Spy = 0;
    uint32_t Current_Event_Num_Spy = 0;
    int Error_Count_Spy = 0;
    int Do_Error_Spy = 0;
    int Pixel_Spy = 0;
    int Pixel_Hit_Spy = 0;
    int Incorrect_Pixel_Spy = 0;

    int k_Spy = 0;
    int j_Spy = 0;
    int h_Spy = 0;
    int g_Spy = 0;
    int Next_step_Spy = 0;
    int Num_ROC_Spy = 0;
    int END_Event_Spy = 0;
    int Error_Loop_Spy = 0;
    int Start_Event_Spy = 0;
    int Stack_Spy[2];
    int Do_Stack_Spy = 0;
    int max_stack = 0;
    int NTP_Spy = 0;

    for( std::vector<unsigned int>::size_type i = 0; i != FIFO.size(); i++ )
	{    

		if(FIFO[i] == 0x00000000){
			break;
		}

		if(((FIFO[i] >> 4) & 0xF) == 0x8){
			Error_Count_Spy = 0;
			Error_Loop_Spy = 0;
		}
		
		if(((FIFO[i] >> 4) & 0xF) == 0x9)
		{
			j_Spy = i;
			Total_Events_Spy++;
			Current_Event_Num_Spy = ((FIFO[i - 1] & 0xF) << 4) | (FIFO[i] & 0xF);
			if(Current_Event_Num_Spy - Compare_Event_Spy != 1 && Compare_Event_Spy - Current_Event_Num_Spy != 255){
				Incorrect_Event_Num_Spy++;
				Error_Count_Spy++;
			}
			Compare_Event_Spy = Current_Event_Num_Spy;
		}

		if(((FIFO[i] >> 4) & 0xF) == 0x8 && Next_step_Spy == 0){ 
		  	Next_step_Spy = 1;
			Start_Event_Spy = i;
			Do_Stack_Spy = 0;
	                continue;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0x8 && Next_step_Spy != 0){
			Incorrect_Header_Spy++;
			Error_Count_Spy++;
			continue;
		}
		
		if(((FIFO[i] >> 4) & 0xF) == 0x9 && Next_step_Spy == 1){
		  	Next_step_Spy = 2;
			continue;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0x9 && Next_step_Spy != 1){
			Incorrect_Header_Spy++;
			Error_Count_Spy++;
			continue;
		}
		
		if(((FIFO[i] >> 4) & 0xF) == 0xA && Next_step_Spy == 2){
		  	Next_step_Spy = 3;
			continue;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0xA && Next_step_Spy != 2){
			Incorrect_Header_Spy++;
			Error_Count_Spy++;
			continue;
		}
		
		if(((FIFO[i] >> 4) & 0xF) == 0xB && Next_step_Spy == 3){
		  	Next_step_Spy = 4;
			continue;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0xB && Next_step_Spy != 3){
			Incorrect_Header_Spy++;
			Error_Count_Spy++;
			continue;
		}
		
		if(((FIFO[i] >> 4) & 0xF) == 0xC && Next_step_Spy == 4){
		  	Next_step_Spy = 5;
			Num_ROC_Spy = 0;

			if(FIFO[i] & 0x8){
				NTP_Spy = 1;
                cout << "There was a NTP at event " << dec << Current_Event_Num_Spy << endl;
			}
            else NTP_Spy = 0;

			if(FIFO[i] & 0x4){
				cout << "There was a TBM Reset in Spy FIFO at event " << dec << Current_Event_Num_Spy << endl;
			}
			if(FIFO[i] & 0x2){
				cout << "There was a ROC Reset in Spy FIFO at event " << dec << Current_Event_Num_Spy << endl;
			}

			continue;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0xC && Next_step_Spy != 4){
			Incorrect_Trailer_Spy++;
			Error_Count_Spy++;
			continue;
		}
		
		if(((FIFO[i] >> 4) & 0xF) == 0xD && Next_step_Spy == 5){
		  	Next_step_Spy = 6;
			continue;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0xD && Next_step_Spy != 5){
			Incorrect_Trailer_Spy++;
			Error_Count_Spy++;
			continue;
		}

		if(((FIFO[i] >> 4) & 0xF) == 0xE && Next_step_Spy == 6){
		  	Next_step_Spy = 7;

			if( ((FIFO[i] & 0x4) >> 2) == 0x1 ){
			  cout << "There was a PKAM Reset in Spy FIFO at Event " << dec << Current_Event_Num_Spy << endl;
			}

			Stack_Spy[0] = (FIFO[i] & 0x3);
			continue;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0xE && Next_step_Spy != 6){
			Incorrect_Trailer_Spy++;
			Error_Count_Spy++;
			continue;
		}
		
		if(((FIFO[i] >> 4) & 0xF) == 0xF && Next_step_Spy == 7){
			Next_step_Spy = 0;
			END_Event_Spy = i;
			Error_Loop_Spy = 1;
			Do_Stack_Spy = 1;
			Stack_Spy[1] = (Stack_Spy[0] << 4) | (FIFO[i] & 0xF);
            max_stack = std::max(max_stack, Stack_Spy[1]);

			if(ROC_Value_Spy != 1 && NTP_Spy != 1){
				Incorrect_ROC_Spy++;
				Error_Count_Spy++;
			}	
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0xF && (FIFO[i - 1] >> 4) != 0xF && Next_step_Spy != 7){
			Incorrect_Trailer_Spy++;
            Error_Count_Spy++;
			END_Event_Spy = i;
			Error_Loop_Spy = 1;
		}
		
		if(Stack_Spy[1] != 0x0 && Do_Stack_Spy == 1){
			//cout << "The Stack count is " << dec << Stack_Spy[1] << endl;
		}

		//if(Error_Count_Spy != 0 && Error_Loop_Spy == 1){
		//	cout << "Event = " << std::dec << Current_Event_Num_Spy << endl; // and Loop = " << std::dec << l << endl; 
		//	cout << "Total Number of Errors = " <<std::dec << Error_Count_Spy << endl;
		//	int Error_Spy = 0;

		//	for(Error_Spy = Start_Event_Spy; Error_Spy <= END_Event_Spy; Error_Spy++){
		//		cout << "FIFO[" << std::dec << Error_Spy << "] = " << std::hex << FIFO[Error_Spy] << endl;
		//		Spy_Error.push_back(FIFO[Error_Spy]);
		//	}

		//	for(Error_Spy = Start_Event_Spy; Error_Spy <= END_Event_Spy; Error_Spy++){
		//		cout << "FIFO_CHB[" << std::dec << Error_Spy << "] = " << std::hex << FIFO_CHB[Error_Spy] << endl;
		//	}
		//	Error_Loop_Spy = 0;
		//}

		if(((FIFO[i] >> 4) & 0xF) == 0x6){
			Pixel_Spy++;
		}
		else if(((FIFO[i] >> 4) & 0xF) == 0x7){
			Num_ROC_Spy++;
			ROC_Value_Spy = 0;
		}

		if(Num_ROC_Spy == 8){
			ROC_Value_Spy = 1;
		}
		else{
			ROC_Value_Spy = 0;
		}
	}

	Spy_Analyze[0].push_back(Compare_Event_Spy);
	Spy_Analyze[0].push_back(Total_Events_Spy);
	Spy_Analyze[0].push_back(Incorrect_Event_Num_Spy);
	Spy_Analyze[0].push_back(Incorrect_Header_Spy);
	Spy_Analyze[0].push_back(Incorrect_Trailer_Spy);
	Spy_Analyze[0].push_back(Incorrect_ROC_Spy);
	Spy_Analyze[0].push_back(Incorrect_Pixel_Spy);
	Spy_Analyze[0].push_back(max_stack);
	Spy_Analyze[1].insert(Spy_Analyze[1].begin(), Spy_Error.begin(), Spy_Error.end());

	if(Error_Count_Spy != 0){
		cout << "There have been " << std::dec << Incorrect_Event_Num_Spy << " Incorrect Event Numbers " << endl;
		cout << "There have been " << std::dec << Incorrect_Header_Spy << " Incorrect TBM Headers " << endl;
		cout << "There have been " << std::dec << Incorrect_Trailer_Spy << " Incorrect TBM Trailers " << endl;
		cout << "There have been " << std::dec << Incorrect_ROC_Spy << " incorrect Number of ROCs " << endl;
		cout << "There have been " << std::dec << Incorrect_Pixel_Spy << " incorrect Number of Pixels " << endl;
		cout << " " << endl;
	}

	return Spy_Analyze;
}

std::vector<uint32_t> FIFO1_Event(vector<uint32_t>& FIFO1, vector<uint32_t>& FIFO1_marker, int Compare_Event_FIFO1){
    std::vector<uint32_t> FIFO1_Analyze;
    int Next_step_FIFO1 = 0;
    int Next_step_roc_FIFO1 = 1;
    int Num_ROC_FIFO1 = 0;
    int END_Event_FIFO1 = 0;
    int Error_Loop_FIFO1 = 0;
    int Start_Event_FIFO1 = 0;
    int Stack_FIFO1;
    int Roc_num_FIFO1 = 0;
    int Total_Events_FIFO1 = 0; //Error_Array[0]
    int Incorrect_Header_FIFO1 = 0; //Error_Array[2]
    int Incorrect_Trailer_FIFO1 = 0; //Error_Array[3]
    int Incorrect_Event_Num_FIFO1 = 0; //Error_Array[1]
    int Incorrect_ROC_FIFO1 = 0;
    int Incorrect_pix_info_FIFO1 = 0;
    int ROC_Value_FIFO1 = 0;
    //int Compare_Event_FIFO1 = 0;
    int Error_Count_FIFO1 = 0;
    int Pixel_FIFO1 = 0;
    int Pixel_Hit_FIFO1 = 0;
    int Incorrect_Pixel_FIFO1 = 0;
    uint32_t Current_Event_Num_FIFO1 = 0;
    std::vector<uint32_t> DCOL_out;
    std::vector<uint32_t> PXL_out;

    for ( uint32_t i = 0; i < FIFO1.size(); i++)
    {

      if (FIFO1[i] == 0x0)
        break;

      if (FIFO1_marker[i] == 8){
        //header
        //cout << "Event Num = " << std::dec << (FIFO1[i] && 0xff) << endl;
        Start_Event_FIFO1 = i;
        Error_Loop_FIFO1 = 0;
        Total_Events_FIFO1++;
        Current_Event_Num_FIFO1 = (FIFO1[i] & 0xFF);
        if(((Current_Event_Num_FIFO1 - Compare_Event_FIFO1) != 1) && ((Compare_Event_FIFO1 - Current_Event_Num_FIFO1) != 255)){
          Incorrect_Event_Num_FIFO1++;
          Error_Count_FIFO1++;
        }
        Compare_Event_FIFO1 = Current_Event_Num_FIFO1;         
        Next_step_FIFO1 = 1;
      }
      if (FIFO1_marker[i] == 0xc){
        //ROC header
        Roc_num_FIFO1 = ((FIFO1[i] >> 21) & 0x1f);
      }

      if (FIFO1_marker[i] == 0x1){
        Pixel_FIFO1++;
      }

      else if (FIFO1_marker[i] == 0x4){
        //TBM Trailer
        END_Event_FIFO1 = i;
        Error_Loop_FIFO1 = 1;
        if(Roc_num_FIFO1 != 8){
          Incorrect_ROC_FIFO1++;
          Error_Count_FIFO1++;
        }
        if((FIFO1[i] & 0x80) >> 7)
          cout << "There was a NTP" << endl;

        if((FIFO1[i] & 0x40) >> 6)
          cout << "There was a TBM Reset in FIFO1" << endl;

        if((FIFO1[i] & 0x20) >> 5)
          cout << "There was a ROC Reset in FIFO1" << endl;

        if((FIFO1[i] & 0x10) >> 4)
          cout << "Stack is full!" << endl;

        if(((FIFO1[i] & 0x40000) >> 18))
          cout << "There was a PKAM Reset in FIFO1 at Event " << std::dec << Current_Event_Num_FIFO1 << endl;

        Stack_FIFO1 = ((FIFO1[i] >> 12) & 0x3F);

        if(Stack_FIFO1 != 0)
          cout << "The stack count is " << std::dec << Stack_FIFO1 << endl;
      } 
      //if(Error_Loop_FIFO1 == 1 && Error_Count_FIFO1 != 0){
      //  cout << "Event = " << std::dec << Current_Event_Num_FIFO1 << endl; //" and Loop = " << std::dec << l << endl;
      //  cout << "Total Number of Errors = " <<std::dec << Error_Count_FIFO1 << endl;
      //  int Error_FIFO1 = 0;

      //  for(Error_FIFO1 = Start_Event_FIFO1; Error_FIFO1 <= END_Event_FIFO1; Error_FIFO1++){
      //    cout << "FIFO1[" << std::dec << Error_FIFO1 << "] = " << std::hex << FIFO1[Error_FIFO1] << endl;
      //  }

      //  //for(Error = Start_Event; Error <= END_Event; Error++){
      //  //cout << "FIFO_CHB[" << std::dec << Error << "] = " << std::hex << FIFO_CHB[Error] << endl;
      //  //}
      //  Error_Loop_FIFO1 = 0;  
      //}

    }// Check FIFO1 for Information

    FIFO1_Analyze.push_back(Compare_Event_FIFO1);
    FIFO1_Analyze.push_back(Total_Events_FIFO1);
    FIFO1_Analyze.push_back(Incorrect_Event_Num_FIFO1);
    FIFO1_Analyze.push_back(Incorrect_Header_FIFO1);
    FIFO1_Analyze.push_back(Incorrect_Trailer_FIFO1);

    //if(Error_Count_FIFO1 != 0){
    //	cout << "There have been " << std::dec << Total_Events_FIFO1 << " Events" << endl;
    //	cout << "There have been " << std::dec << Incorrect_Event_Num_FIFO1 << " Incorrect Event Numbers " << endl;
    //	cout << "There have been " << std::dec << Incorrect_Header_FIFO1 << " Incorrect TBM Headers " << endl;
    //	cout << "There have been " << std::dec << Incorrect_Trailer_FIFO1 << " Incorrect TBM Trailers " << endl;
    //	cout << " " << endl;
    //}

    return FIFO1_Analyze;
}

void ErrorTrailerWord(uint64_t Error_TBM, uint64_t Error_ROC, uint64_t TBM_Mask, int TBM_Enable = 0, int ROC_Enable = 0){
    if((Error_TBM != ~TBM_Mask) && TBM_Enable == 1){
    	cout << "TBM Resets in Error FIFO channels " << std::hex << "0x" << (~Error_TBM) << " do not match TBM Mask 0x" << TBM_Mask << endl;
    	throw QuitNow();
    }
    if((Error_ROC != ~TBM_Mask) && ROC_Enable == 1){
    	cout << "ROC Resets in Error FIFO channels " << std::hex << "0x" << (~Error_ROC) << " do not match TBM Mask 0x" << TBM_Mask << endl;
    	throw QuitNow();
    }
}

void ErrorTrailerWord(int Error_TBM, int Error_ROC, int TBMCount, int MarkerCount, int Error_TO, int TBM_Enable = 0, int ROC_Enable = 0){
    if(Error_TO != 0 && MarkerCount == 0){
        if((Error_TBM % (TBMCount - (Error_TO/2)) != 0) && TBM_Enable == 1){
        	cout << "TBM Resets in Error FIFO channels " << std::dec << (Error_TBM) << " do not match TBM Mask " << (TBMCount - (Error_TO/2)) << endl;
        	throw QuitNow();
        }
        if((Error_ROC % (TBMCount - (Error_TO/2)) != 0) && ROC_Enable == 1){
        	cout << "ROC Resets in Error FIFO channels " << std::dec<< (Error_ROC) << " do not match TBM Mask " << (TBMCount - (Error_TO/2)) << endl;
        	throw QuitNow();
        }
    }
    else if(MarkerCount != 48){
        if((Error_TBM % (TBMCount - MarkerCount) != 0) && (Error_TBM % TBMCount) != 0 && TBM_Enable == 1){
        	cout << "TBM Resets in Error FIFO channels " << std::dec << (Error_TBM) << " do not match TBM Mask " << (TBMCount - MarkerCount) << endl;
        	throw QuitNow();
        }
        if((Error_ROC % (TBMCount - MarkerCount) != 0) && (Error_ROC % TBMCount) != 0 && ROC_Enable == 1){
        	cout << "ROC Resets in Error FIFO channels " << std::dec<< (Error_ROC) << " do not match TBM Mask " << (TBMCount - MarkerCount) << endl;
        	throw QuitNow();
        }
    }
    else{
        if((Error_TBM % 48 != 0) && TBM_Enable == 1){
        	cout << "TBM Resets in Error FIFO channels " << std::dec << (Error_TBM) << " do not match TBM Mask " << TBMCount << endl;
        	throw QuitNow();
        }
        if((Error_ROC % 48 != 0) && ROC_Enable == 1){
        	cout << "ROC Resets in Error FIFO channels " << std::dec<< (Error_ROC) << " do not match TBM Mask " << TBMCount << endl;
        	throw QuitNow();
        }
    }
}

void ErrorEventNum(int Error_ENE, int Error_TO, int MarkerCount){
    if(Error_TO != 0){
	    if((Error_ENE % (Error_TO/2) != 0) && (MarkerCount == 0)){
	    	cout << "ENE in channels " << std::dec << (Error_ENE) << " do not match Timeout Error Count " << Error_TO << endl;
	    	throw QuitNow();
	    }
    }
    else if((Error_ENE % 48 != 0) && (MarkerCount == 0)){
		cout << "ENE in channels " << std::dec << (Error_ENE) << " do not match Marker Error Count " << MarkerCount << endl;
		throw QuitNow();
	}
    if(MarkerCount != 0){
        if(((Error_ENE % MarkerCount) != 0)){
	    	cout << "ENE in channels " << std::dec << (Error_ENE) << " do not match Marker Error Count " << MarkerCount << endl;
	    	throw QuitNow();
	    }
    }
}

void ErrorTimeout(int Error_TO, int MarkerCount){
	if((((Error_TO/2) % 48) != 0) && (MarkerCount == 0)){
		cout << "TO in channels " << std::dec << (Error_TO) << " do not match Marker Error Count" << MarkerCount << endl;
		throw QuitNow();
	}
    if(MarkerCount != 0){
        if((((Error_TO/2) % MarkerCount) != 0)){
	    	cout << "TO in channels " << std::dec << (Error_TO) << " do not match Marker Error Count" << MarkerCount << endl;
	    	throw QuitNow();
	    }
    }
}

void ErrorOVF(int Error_OVF, int MarkerCount){
	if(Error_OVF != 0){
        if(((Error_OVF % 48) != 0) && (MarkerCount == 0)){
	    	cout << "OVF Resets in channels " << std::dec << (Error_OVF) << " do not match Marker Error Count" << MarkerCount << endl;
	    	throw QuitNow();
	    }
        else if(((Error_OVF % MarkerCount) != 0)){
	    	cout << "OVF Resets in channels " << std::dec << (Error_OVF) << " do not match Marker Error Count" << MarkerCount << endl;
	    	throw QuitNow();
	    }
    }
}

void ErrorPKAM(int Error_PKAM, int Error_ROCNum, int Error_NTP){
	if(Error_PKAM != 0){
        if(Error_PKAM > 48){
            if((Error_ROCNum % (Error_PKAM - 48)) != 0){
	        	cout << "ROC number errors don't match number of PKAMs in channels " << std::dec << (Error_ROCNum) << " do not match " << Error_PKAM << endl;
	        	throw QuitNow();
	        }
            if((Error_NTP % (Error_PKAM - 48)) != 0){
	        	cout << "NTPs don't match the number of PKAMs in channels " << std::dec << (Error_NTP) << " do not match " << Error_PKAM << endl;
	        	throw QuitNow();
	        }
        }
        else{
	        if((Error_ROCNum % Error_PKAM) != 0){
	        	cout << "ROC number errors don't match number of PKAMs in channels " << std::dec << (Error_ROCNum) << " do not match " << Error_PKAM << endl;
	        	throw QuitNow();
	        }
            if((Error_NTP % Error_PKAM) != 0){
	        	cout << "NTPs don't match the number of PKAMs in channels " << std::dec << (Error_NTP) << " do not match " << Error_PKAM << endl;
	        	throw QuitNow();
	        }
        }
    }
}

void ErrorSameChannel(int Channel, int EventNum){
    cout << "There was the same Error in Error FIFO channel" << std::dec << Channel << " on the same Event" << EventNum << endl;
    throw QuitNow();
}

std::vector<uint32_t> Error_Event(vector<uint32_t>& Error_FIFO, int Timeout_Error, uint64_t TBM_Mask, int MarkerCount = 0){
    std::vector<uint32_t> Error_Analyze;
    int Total_Events_Error = 0;
    int Incorrect_Header_Error = 0;
    int Incorrect_Trailer_Error = 0;
    int Incorrect_Event_Num_Error = 0;
    int Stack_Error = 0;
    int Timeout_Error_Error = 0;
    int Error_count_OVF = 0;
	uint64_t Error_Marker30_1 = 0;
	uint64_t Error_Marker30_2 = 0;
	uint64_t Error_TBM_Resets_1 = 0;
	uint64_t Error_TBM_Resets_2 = 0;
	uint64_t Error_TBM_Resets = 0;
	uint64_t Error_ROC_Resets_1 = 0;
	uint64_t Error_ROC_Resets_2 = 0;
	uint64_t Error_ROC_Resets = 0;
	int TBM_Resets = 0;
	int ROC_Resets = 0;
	int TBM_Enable = 0;
	int ROC_Enable = 0;
    int TBM_Count = 0;
    uint64_t TBM_Mask_Buff = TBM_Mask;
    int Error_PKAM = 0;
    int Error_ROCNum = 0;
    int Error_NTP = 0;
    int PrePreviousEvent = 0;
    int PreviousEvent = 0;
    bool DoAnalyze = false;
    bool SameError = false;
    int Channel = 0;
    int Error_Marker29[48] = {};
    int Error_Marker31[48] = {};

    for(int j = 0; j < 48; j++){
        if(~TBM_Mask_Buff & 0x1) TBM_Count++;
        TBM_Mask_Buff = TBM_Mask_Buff >> 1;
    }

    for ( uint32_t i = 0; i != Error_FIFO.size(); i++){
        uint32_t cChannel_1 =   (Error_FIFO[i] & 0xFC000000) >> 26;
        uint32_t cMarker_1 =    (Error_FIFO[i] & 0x03E00000) >> 21;
        uint32_t cL1Actr_1 =    (Error_FIFO[i] & 0x001FE000) >> 13;
        uint32_t cErrorWord_1 = (Error_FIFO[i] & 0x00001FFF);

        if(Error_FIFO[i] == 0x0) break;

        if(cL1Actr_1 - PreviousEvent == 1 && PreviousEvent - cL1Actr_1 == 255){
            DoAnalyze = true;
            Timeout_Error += Timeout_Error_Error;
        }
        else DoAnalyze = false;
        PreviousEvent = cL1Actr_1;

        switch(cMarker_1){
            case 29 :{
                Timeout_Error_Error++;
                //cout << "There was a timeout at event " << std::dec << cL1Actr_1 << " in channel " << cChannel_1 << endl;
                Stack_Error = Error_FIFO[i] & 0x3F;
                //if(Stack_Error > 0)
                    //cout << "Stack is " << std::dec << Stack_Error << endl;
                if(Error_Marker29[cChannel_1 - 1] >= 2){
                    Channel = cChannel_1;
                    SameError = true;
                }
                Error_Marker29[cChannel_1 - 1]++;
            }
            case 31 :{
                //cout << "There was an Event Num error at event " << std::dec << cL1Actr_1 << " in channel " << cChannel_1 << endl;
                //cout << "Recieved event " << (cErrorWord_1 & 0xFF) << endl;
                Incorrect_Event_Num_Error++;
                if(Error_Marker31[cChannel_1 - 1] >= 3){
                    Channel = cChannel_1;
                    SameError = true;
                }
                Error_Marker31[cChannel_1 - 1]++;
            }
            case 30 :{
                if((Error_Marker30_2 << 32 | Error_Marker30_1) & (1 << (cChannel_1 - 1)) == 1){
                    Channel = cChannel_1;
                    SameError = true;
                }
                if(cChannel_1 <= 32) Error_Marker30_1 = (Error_Marker30_1 | (1 << (cChannel_1 - 1)));
                else                 Error_Marker30_2 = (Error_Marker30_2 | (1 << (cChannel_1 - 33)));

                if((cErrorWord_1 & 0x800) >> 11){
                    //cout << "There was a # of ROC Error at event " << cL1Actr_1 << " in channel " << cChannel_1 << endl;
                    Error_ROCNum++;
                }
                if((cErrorWord_1 & 0x200) >> 9 && (cErrorWord_1 & 0x100) >> 8){
                    //cout << "There was an OVF trailer inserted by Helmut " << cL1Actr_1 << " in channel " << cChannel_1 << endl;
                    Error_count_OVF++;
                }
                else if((cErrorWord_1 & 0x200) >> 9){
                    //cout << "There was a PKAM Reset at Event " << cL1Actr_1 << " in channel " << cChannel_1 << endl;
                    Error_PKAM++;
                }

                if((cErrorWord_1 & 0x80) >> 7){
                    //cout << "There was a NTP at event " << cL1Actr_1 << " in channel " << cChannel_1 << endl;
                    Error_NTP++;
                }

                if((cErrorWord_1 & 0x40) >> 6){
                    //cout << "There was a TBM Reset at event " << cL1Actr_1 << " in channel " << cChannel_1 << endl;
					if(cChannel_1 <= 32) Error_TBM_Resets_1 = (Error_TBM_Resets_1 | (1 << (cChannel_1 - 1)));
                    else                 Error_TBM_Resets_2 = (Error_TBM_Resets_2 | (1 << (cChannel_1 - 33)));
                    TBM_Resets++;
                    TBM_Enable = 1;
                }

                if((cErrorWord_1 & 0x20) >> 5){
                    //cout << "There was a ROC Reset at event " << cL1Actr_1 << " in channel " << cChannel_1 << endl;
					if(cChannel_1 <= 32) Error_ROC_Resets_1 = (Error_ROC_Resets_1 | (1 << (cChannel_1 - 1)));
                    else                 Error_ROC_Resets_2 = (Error_ROC_Resets_2 | (1 << (cChannel_1 - 33)));
				    ROC_Resets++;
                    ROC_Enable = 1;
                }

                if((cErrorWord_1 & 0x10) >> 4){
                    //cout << "Stack is full!" << endl;
                }
                if(Stack_Error != 0){
                    //cout << "The stack count is " << std::dec << Stack_Error << endl;
                }
            }
        }

        Total_Events_Error = cL1Actr_1;
        
        if(DoAnalyze || i == (Error_FIFO.size() - 1)){
            Error_TBM_Resets = (Error_TBM_Resets_2 << 32) | Error_TBM_Resets_1;
            Error_ROC_Resets = (Error_ROC_Resets_2 << 32) | Error_ROC_Resets_1;
            if(Error_count_OVF == 0 &&  MarkerCount == 0 && Timeout_Error == 0) ErrorTrailerWord(Error_TBM_Resets, Error_ROC_Resets, TBM_Mask, TBM_Enable, ROC_Enable);
            if(Error_count_OVF == 0 && (MarkerCount != 0 || Timeout_Error != 0)) ErrorTrailerWord(TBM_Resets, ROC_Resets, TBM_Count, MarkerCount, Timeout_Error, TBM_Enable, ROC_Enable);
            ErrorEventNum(Incorrect_Event_Num_Error, Timeout_Error, MarkerCount);
            ErrorTimeout(Timeout_Error, MarkerCount);
            ErrorOVF(Error_count_OVF, MarkerCount);
            ErrorPKAM(Error_PKAM, Error_ROCNum, Error_NTP);
            //if(SameError) ErrorSameChannel(Channel, cL1Actr_1);
            Error_Marker30_1 = 0;
            Error_Marker30_2 = 0;
            Error_TBM_Resets_1 = 0;
            Error_TBM_Resets_2 = 0;
            Error_ROC_Resets_1 = 0;
            Error_ROC_Resets_2 = 0;
            TBM_Enable = 0;
            ROC_Enable = 0;
            SameError = false;
            std::fill_n(Error_Marker29, 48, 0);
            std::fill_n(Error_Marker31, 48, 0);
        }
    }

	Error_Analyze.push_back(Total_Events_Error);
	Error_Analyze.push_back(Incorrect_Event_Num_Error);
	Error_Analyze.push_back(Timeout_Error);
	Error_Analyze.push_back(Error_count_OVF);

	return Error_Analyze;
}

void DDROddEvent(vector<uint32_t>& DDR, int EventNum){
	if((DDR.size() % 2) == 1){
		cout << "The DDR has odd value of " << DDR.size() << " at Event " << EventNum << endl;
		throw QuitNow();
	}
}

void DDRTrailerWord(uint64_t DDR_TBM, uint64_t DDR_ROC, uint64_t TBM_Mask, int TBM_Enable = 0, int ROC_Enable = 0){
	if(((~DDR_TBM & 0xFFFFFFFFFFFF) != (TBM_Mask & 0xFFFFFFFFFFFF)) && TBM_Enable == 1){
		cout << "TBM Resets in DDR channels " << std::hex << "0x" << (DDR_TBM) << " do not match TBM Mask 0x" << TBM_Mask << endl;
		throw QuitNow();
	}
	if(((~DDR_ROC & 0xFFFFFFFFFFFF) != (TBM_Mask & 0xFFFFFFFFFFFF)) && ROC_Enable == 1){
		cout << "ROC Resets in DDR channels " << std::hex << "0x" << (DDR_ROC) << " do not match TBM Mask 0x" << TBM_Mask << endl;
		throw QuitNow();
	}
}

void DDRMaskedChannel(uint64_t DDR_Mask, uint64_t TBM_Mask, int TBM_Count, int SEU_Count, uint64_t SEU_Mask){
    if(SEU_Count == 0){
        if(((DDR_Mask & 0xFFFFFFFFFFFF) != TBM_Mask)){
	        cout << "Masked DDR channels " << std::hex << "0x" << DDR_Mask << " do not match TBM Mask 0x" << TBM_Mask << endl;
	        throw QuitNow();
	    }
    }
    else{
        if(((DDR_Mask & 0xFFFFFFFFFFFF) != SEU_Mask) && SEU_Mask != TBM_Mask && ((DDR_Mask & 0xFFFFFFFFFFFF) != TBM_Mask)){
	        cout << "Masked DDR channels " << std::hex << "0x" << DDR_Mask << " do not match SEU Mask 0x" << SEU_Mask << endl;
	        throw QuitNow();
	    }
    }
}

void DDRTrailerWord(int DDR_TBM, int DDR_ROC, int TBMCount, int MarkerCount, int DDR_TO, int TBM_Enable = 0, int ROC_Enable = 0){
    if(DDR_TO != 0 && MarkerCount == 0){
        if((DDR_TBM % (TBMCount - DDR_TO) != 0) && TBM_Enable == 1){
        	cout << "TBM Resets in DDR FIFO channels " << std::dec << (DDR_TBM) << " do not match TBM Mask " << (TBMCount - DDR_TO) << endl;
        	throw QuitNow();
        }
        if((DDR_ROC % (TBMCount - DDR_TO) != 0) && ROC_Enable == 1){
        	cout << "ROC Resets in DDR FIFO channels " << std::dec<< (DDR_ROC) << " do not match TBM Mask " << (TBMCount - DDR_TO) << endl;
        	throw QuitNow();
        }
    }
    else if(MarkerCount != 48){
        if((DDR_TBM % (TBMCount - MarkerCount) != 0 && DDR_TBM % TBMCount != 0) && TBM_Enable == 1){
        	cout << "TBM Resets in DDR FIFO channels " << std::dec << (DDR_TBM) << " do not match TBM Mask " << (TBMCount - MarkerCount) << endl;
        	throw QuitNow();
        }
        if((DDR_ROC % (TBMCount - MarkerCount) != 0 && DDR_ROC % TBMCount != 0) && ROC_Enable == 1){
        	cout << "ROC Resets in DDR FIFO channels " << std::dec<< (DDR_ROC) << " do not match TBM Mask " << (TBMCount - MarkerCount) << endl;
        	throw QuitNow();
        }
    }
    else{
        if((DDR_TBM % 48 != 0) && TBM_Enable == 1){
        	cout << "TBM Resets in DDR FIFO channels " << std::dec << (DDR_TBM) << " do not match TBM Mask " << TBMCount << endl;
        	throw QuitNow();
        }
        if((DDR_ROC % 48 != 0) && ROC_Enable == 1){
        	cout << "ROC Resets in DDR FIFO channels " << std::dec<< (DDR_ROC) << " do not match TBM Mask " << TBMCount << endl;
        	throw QuitNow();
        }
    }
}

void DDREventNum(int DDR_ENE, int DDR_TO, int MarkerCount){
    if(DDR_TO != 0){
	    if((DDR_ENE % DDR_TO != 0) && (MarkerCount == 0)){
	    	cout << "ENE in channels " << std::dec << (DDR_ENE) << " do not match Timeout DDR Count " << DDR_TO << endl;
	    	throw QuitNow();
	    }
    }
    else if(MarkerCount != 0){
        if(((DDR_ENE % MarkerCount) != 0)){
	    	cout << "ENE in channels " << std::dec << (DDR_ENE) << " do not match Marker DDR Count " << MarkerCount << endl;
	    	throw QuitNow();
	    }
    }
    else{
        if((DDR_ENE % 48 != 0) && (MarkerCount == 0)){
	    	cout << "ENE in channels " << std::dec << (DDR_ENE) << " do not match Marker DDR Count " << MarkerCount << endl;
	    	throw QuitNow();
	    }
    }
}

void DDRSameChannelError(int Channel, int EventNum){
    cout << "There was the same Error in DDR channel " << std::dec << Channel << " on the same Event " << EventNum << endl;
    throw QuitNow();
}

void DDRTimeout(int DDR_TO, int MarkerCount){
    if(MarkerCount != 0){
        if(((DDR_TO % MarkerCount) != 0)){
        	cout << "TO Resets in channels " << std::dec << (DDR_TO) << " do not match Marker DDR Count" << MarkerCount << endl;
        	throw QuitNow();
        }
    }
}

void DDROVF(int DDR_OVF, int MarkerCount, int SEUCount){
	if(SEUCount != 0){
        if((DDR_OVF % SEUCount) != 0){
	    	cout << "OVF Resets in channels " << std::dec << (DDR_OVF) << " do not match SEU DDR Count" << SEUCount << endl;
	    	throw QuitNow();
	    }
    }
    else{
	    if(((DDR_OVF % 48) != 0) && (MarkerCount == 0)){
	    	cout << "OVF Resets in channels " << std::dec << (DDR_OVF) << " do not match Marker DDR Count" << MarkerCount << endl;
	    	throw QuitNow();
	    }
        if(MarkerCount != 0){
            if(((DDR_OVF % MarkerCount) != 0)){
	        	cout << "OVF Resets in channels " << std::dec << (DDR_OVF) << " do not match Marker DDR Count" << MarkerCount << endl;
	        	throw QuitNow();
	        }
        }
    }
}

void DDRPKAM(int DDR_PKAM, int DDR_ROCNum, int DDR_NTP){
	if(DDR_PKAM != 0){
        if(DDR_PKAM > 48){
            if((DDR_ROCNum % (DDR_PKAM - 48)) != 0){
	        	cout << "ROC number errors don't match number of PKAMs in channels " << std::dec << (DDR_ROCNum) << " do not match " << DDR_PKAM << endl;
	        	throw QuitNow();
	        }
            if((DDR_NTP % (DDR_PKAM - 48)) != 0){
	        	cout << "NTPs don't match the number of PKAMs in channels " << std::dec << (DDR_NTP) << " do not match " << DDR_PKAM << endl;
	        	throw QuitNow();
	        }
        }
        else{
            if((DDR_ROCNum % DDR_PKAM) != 0){
	        	cout << "ROC number errors don't match number of PKAMs in channels " << std::dec << (DDR_ROCNum) << " do not match " << DDR_PKAM << endl;
	        	throw QuitNow();
	        }
            if((DDR_NTP % DDR_PKAM) != 0){
	        	cout << "NTPs don't match the number of PKAMs in channels " << std::dec << (DDR_NTP) << " do not match " << DDR_PKAM << endl;
	        	throw QuitNow();
	        }
        }
    }
}

std::vector<uint32_t> DDR_Event(std::vector<uint32_t>& DDR, int Compare_Event_DDR, uint64_t TBM_Mask, int Timeout_Error = 0, int MarkerCount = 0, int SEUCount = 0, uint64_t SEU_Mask = 0){
	using namespace std;
	std::vector<uint32_t> DDR_Analyze;
	int Total_Events_DDR = 0;
	int Incorrect_Event_Num_DDR = 0;
	int Incorrect_ROC_DDR = 0;
	int ROC_Value_DDR = 0;
	int Stack_DDR = 0;
	uint32_t Current_Event_Num_DDR = 0;
	int Error_Count_DDR = 0;
	int Pixel_DDR = 0;
	int Pixel_Hit_DDR = 0;
	int Roc_num_DDR = 0;
	int Incorrect_Pixel_DDR = 0;
	int Channel_Number   = 0;
	int Channel_Number_1 = 0;
	int Channel_Number_2 = 0;
	int ROC_Identifier_1 = 0;
	int ROC_Identifier_2 = 0;
	int Timeout_Error_DDR = 0;
	int Previous_Timeout_Error_DDR = Timeout_Error;
	int Event_Num_Error_DDR = 0;
	int Event_Num_Error = 0;
	int pixel_hit_DDR = 0;
	int DCOL_DDR = 0;
	int PXL_DDR = 0;
	int Head_DDR = 0;
	int BX = 0;
	std::vector<std::vector<unsigned int> > Num_Hits_DDR(48, vector<unsigned int>(48));
	uint32_t Hit_buffer1 = 0;
	uint32_t Hit_buffer2 = 0;
	uint32_t pixel_hit = 0;
	int Event_Size = 0;
	uint64_t DDR_Marker30_1 = 0;
	uint64_t DDR_Marker30_2 = 0;
	uint64_t DDR_Marker31_1 = 0;
	uint64_t DDR_Marker31_2 = 0;
	uint64_t DDR_TBM_Resets_1 = 0;
	uint64_t DDR_TBM_Resets_2 = 0;
	uint64_t DDR_TBM_Resets = 0;
	uint64_t DDR_ROC_Resets_1 = 0;
	uint64_t DDR_ROC_Resets_2 = 0;
	uint64_t DDR_ROC_Resets = 0;
	uint64_t DDR_Masked_Channels_1 = 0;
	uint64_t DDR_Masked_Channels_2 = 0;
	uint64_t DDR_Masked_Channels = 0;
    int TBM_Resets = 0;
	int ROC_Resets = 0;
	int TBM_Enable = 0;
	int ROC_Enable = 0;
    int OVF_DDR = 0;
    int DDR_PKAM = 0;
    int DDR_ROCNum = 0;
    int DDR_NTP = 0;
    int TBM_Count = 0;
    int DDR_Marker29[48] = {};
    uint64_t TBM_Mask_Buff = TBM_Mask;
    bool SameError = false;
    bool MaskEnable = false;
    
    for(int j = 0; j < 48; j++){
        if(~TBM_Mask_Buff & 0x1) TBM_Count++;
        TBM_Mask_Buff = TBM_Mask_Buff >> 1;
    }


	for ( uint32_t i = 0; i < DDR.size(); i+=2){

        //cout << "DDR[" << std::dec << i << "]: " << std::bitset<32>(DDR[i]) << " " << std::bitset<32>(DDR[i + 1]) << endl;

		if(((DDR[i] >> 28) == 0x5) && Head_DDR == 0){
			//Header
			Head_DDR = 1;
            SameError = false;
            MaskEnable = false;
            std::fill_n(DDR_Marker29, 48, 0);
            Total_Events_DDR++;
			Current_Event_Num_DDR = ((DDR[i]) & 0xFFFFFF);
			BX = (DDR[i + 1] >> 20) & 0xFFF;
            if(Current_Event_Num_DDR - Compare_Event_DDR != 1 && Compare_Event_DDR - Current_Event_Num_DDR != 255){
				Incorrect_Event_Num_DDR++;
				Error_Count_DDR++;
			}
			Compare_Event_DDR = Current_Event_Num_DDR;		
        }

		else if(((DDR[i] >> 28) == 0xa) && (((DDR[i] >> 21) & 0x1F) == 0x0)){
			//Trailer
			Head_DDR = 0;
            Timeout_Error += (Timeout_Error_DDR/2);
            DDR_TBM_Resets = (DDR_TBM_Resets_2 << 32) | DDR_TBM_Resets_1;
            DDR_ROC_Resets = (DDR_ROC_Resets_2 << 32) | DDR_ROC_Resets_1;
            DDR_Masked_Channels = (DDR_Masked_Channels_2 << 32) | DDR_Masked_Channels_1;
            Timeout_Error_DDR = 0;
            if(SEUCount == 0 && OVF_DDR == 0 && MarkerCount == 0 && Timeout_Error == 0) DDRTrailerWord(DDR_TBM_Resets, DDR_ROC_Resets, TBM_Mask, TBM_Enable, ROC_Enable);
            if(SEUCount == 0 && OVF_DDR == 0 && MarkerCount != 0) DDRTrailerWord(TBM_Resets, ROC_Resets, TBM_Count, MarkerCount, Timeout_Error, TBM_Enable, ROC_Enable);
            if(MaskEnable) DDRMaskedChannel(DDR_Masked_Channels, TBM_Mask, TBM_Count, SEUCount, SEU_Mask);
            DDREventNum(Event_Num_Error_DDR, Previous_Timeout_Error_DDR, MarkerCount);
            DDRTimeout(Timeout_Error_DDR, MarkerCount);
            DDROVF(OVF_DDR, MarkerCount, SEUCount);
            DDRPKAM(DDR_PKAM, DDR_ROCNum, DDR_NTP);
            //if(SameError) DDRSameChannelError(Channel_Number, Current_Event_Num_DDR);
            Previous_Timeout_Error_DDR = Timeout_Error;
            Event_Num_Error_DDR = 0;
            DDR_Marker30_1 = 0;
            DDR_Marker30_2 = 0;
            DDR_TBM_Resets_1 = 0;
            DDR_TBM_Resets_2 = 0;
            DDR_ROC_Resets_1 = 0;
            DDR_ROC_Resets_2 = 0;
            DDR_Masked_Channels_1 = 0;
            DDR_Masked_Channels_2 = 0;
		    TBM_Enable = 0;
            ROC_Enable = 0;
        }
		else{
			Channel_Number_1 = (DDR[i] >> 26) & 0x3F;
			Channel_Number_2 = (DDR[i + 1] >> 26) & 0x3F;
			ROC_Identifier_1 = (DDR[i] >> 21) & 0x1F;
			ROC_Identifier_2 = (DDR[i + 1] >> 21) & 0x1F;
			if( ROC_Identifier_1 == 25){
                MaskEnable = true;
                if(Channel_Number_1 <= 32) DDR_Masked_Channels_1 = (DDR_Masked_Channels_1 | (1 << (Channel_Number_1 - 1)));
                else                       DDR_Masked_Channels_2 = (DDR_Masked_Channels_2 | (1 << (Channel_Number_1 - 33)));
                    
            }
			if( ROC_Identifier_2 == 25){
                MaskEnable = true;
                if(Channel_Number_2 <= 32) DDR_Masked_Channels_1 = (DDR_Masked_Channels_1 | (1 << (Channel_Number_2 - 1)));
                else                       DDR_Masked_Channels_2 = (DDR_Masked_Channels_2 | (1 << (Channel_Number_2 - 33)));
			}
			if( ROC_Identifier_1 == 29){
                if(DDR_Marker29[Channel_Number_1 - 1] >= 2){
                    Channel_Number = Channel_Number_1;
                    SameError = true;
                }
                Timeout_Error_DDR++;
                DDR_Marker29[Channel_Number_1 - 1]++;
			}
			if( ROC_Identifier_2 == 29){
                if(DDR_Marker29[Channel_Number_2 - 1] >= 2){
                    Channel_Number = Channel_Number_2;
                    SameError = true;
                }
                Timeout_Error_DDR++;
                DDR_Marker29[Channel_Number_2 - 1]++;
				Stack_DDR = DDR[i] & 0x3F;
			}
			if( ROC_Identifier_2 == 31){
                if((DDR_Marker31_2 << 32 | DDR_Marker31_1) & (1 << (Channel_Number_2 - 1)) == 1){
                    Channel_Number = Channel_Number_2;
                    SameError = true;
                }
                if(Channel_Number_1 <= 32) DDR_Marker31_1 = (DDR_Marker31_1 | (1 << (Channel_Number_2 - 1)));
                else                       DDR_Marker31_2 = (DDR_Marker31_2 | (1 << (Channel_Number_2 - 33)));
				
				Event_Num_Error_DDR++;
			}
			if( ROC_Identifier_1 == 31){
                if((DDR_Marker31_2 << 32 | DDR_Marker31_1) & (1 << (Channel_Number_1 - 1)) == 1){
                    Channel_Number = Channel_Number_1;
                    SameError = true;
                }
                if(Channel_Number_1 <= 32) DDR_Marker31_1 = (DDR_Marker31_1 | (1 << (Channel_Number_1 - 1)));
                else                       DDR_Marker31_2 = (DDR_Marker31_2 | (1 << (Channel_Number_1 - 33)));
				
				Event_Num_Error_DDR++;
			}
			if ( (ROC_Identifier_1 == 1) && (Channel_Number_1 == 1) && ((DDR[i - 1] >> 28) == 0xa)){ // && ROC_Identifier_1 <= 8){
				if (ROC_Identifier_2 == 8)
					Roc_num_DDR = 1;
				else
					Roc_num_DDR = 0;
				pixel_hit_DDR++;
				DCOL_DDR = (DDR[i] >> 16) & 0x1F;
				PXL_DDR = (DDR[i] >> 8) & 0xFF;
			}
			if( (Channel_Number_1 >= 1 && Channel_Number_1 <= 48) ){
				if ( ((ROC_Identifier_1 >=1) && (ROC_Identifier_1 <= 8)) && (ROC_Identifier_1 != 30) && (ROC_Identifier_1 != 26)){
					Num_Hits_DDR[Channel_Number_1 - 1].at((ROC_Identifier_1 - 1)) = Num_Hits_DDR[Channel_Number_1 - 1][ROC_Identifier_1 - 1] + 1;
				}
			}
			if((Channel_Number_1 >= 1 && Channel_Number_1 <= 48)){
				if ( ((ROC_Identifier_2 >=1) && (ROC_Identifier_2 <= 8)) && (ROC_Identifier_2 != 30) && (ROC_Identifier_2 != 26)){
					Num_Hits_DDR[Channel_Number_2 - 1].at((ROC_Identifier_2 - 1)) = Num_Hits_DDR[Channel_Number_1 - 1][ROC_Identifier_1 - 1] + 1;
				}
			}
			if ( ROC_Identifier_2 == 30){
                if((DDR_Marker30_2 << 32 | DDR_Marker30_1) & (1 << (Channel_Number_2 - 1)) == 1){
                    cout << "Marker30: " << std::hex << (DDR_Marker30_2 << 32 | DDR_Marker30_1) << " Channel: " << std::dec << Channel_Number_2 << endl;
                    Channel_Number = Channel_Number_2;
                    SameError = true;
                }
                if(Channel_Number_1 <= 32) DDR_Marker30_1 = (DDR_Marker30_1 | (1 << (Channel_Number_2 - 1)));
                else                       DDR_Marker30_2 = (DDR_Marker30_2 | (1 << (Channel_Number_2 - 33)));
				
                if(Roc_num_DDR != 1){
					Incorrect_ROC_DDR++;
					Error_Count_DDR++;
				}
        		if((DDR[i + 1] & 0x800) >> 11){
        		    DDR_ROCNum++;
                }

        		if((DDR[i + 1] & 0x200) >> 9 && (DDR[i + 1] & 0x100) >> 8){
        			OVF_DDR++;
                }
                else if((DDR[i + 1] & 0x200) >> 9){
        		    DDR_PKAM++;
                }

                if((DDR[i + 1] & 0x80) >> 7){
        		    DDR_NTP++;
                }

        		if((DDR[i + 1] & 0x40) >> 6){
                    if(Channel_Number_2 <= 32) DDR_TBM_Resets_1 = (DDR_TBM_Resets_1 | (1 << (Channel_Number_2 - 1)));
                    else                       DDR_TBM_Resets_2 = (DDR_TBM_Resets_2 | (1 << (Channel_Number_2 - 33)));
                    TBM_Resets++;
                    TBM_Enable = 1;
        		}

        		if((DDR[i + 1] & 0x20) >> 5){
                    if(Channel_Number_2 <= 32) DDR_ROC_Resets_1 = (DDR_ROC_Resets_1 | (1 << (Channel_Number_2 - 1)));
                    else                       DDR_ROC_Resets_2 = (DDR_ROC_Resets_2 | (1 << (Channel_Number_2 - 33)));
                    ROC_Resets++;
                    ROC_Enable = 1;
        		}

        		if((DDR[i + 1] & 0x10) >> 4){
                    cout << "Stack is full!" << endl;
        		}

        		if(Stack_DDR != 0){
                    //cout << "The stack count is " << std::dec << Stack_DDR << endl;
        		}
			}
			if ( ROC_Identifier_1 == 30){
                if((DDR_Marker30_2 << 32 | DDR_Marker30_1) & (1 << (Channel_Number_1 - 1)) == 1){
                    cout << "Marker30: " << std::hex << (DDR_Marker30_2 << 32 | DDR_Marker30_1) << " Channel: " << std::dec << Channel_Number_1 << endl;
                    Channel_Number = Channel_Number_1;
                    SameError = true;
                }
                if(Channel_Number_1 <= 32) DDR_Marker30_1 = (DDR_Marker30_1 | (1 << (Channel_Number_1 - 1)));
                else                       DDR_Marker30_2 = (DDR_Marker30_2 | (1 << (Channel_Number_1 - 33)));

				if(Roc_num_DDR != 1){
					Incorrect_ROC_DDR++;
					Error_Count_DDR++;
				}
				if((DDR[i] & 0x800) >> 11){
				    DDR_ROCNum++;
                }

				if((DDR[i] & 0x200) >> 9 && (DDR[i] & 0x100) >> 8){
                    OVF_DDR++;
				}
                else if((DDR[i] & 0x200) >> 9){
				    DDR_PKAM++;
                }
				
                if((DDR[i] & 0x80) >> 7){
				    DDR_NTP++;
                }

				if((DDR[i] & 0x40) >> 6){
                    if(Channel_Number_1 <= 32) DDR_TBM_Resets_1 = (DDR_TBM_Resets_1 | (1 << (Channel_Number_1 - 1)));
                    else                       DDR_TBM_Resets_2 = (DDR_TBM_Resets_2 | (1 << (Channel_Number_1 - 33)));
                    TBM_Resets++;
					TBM_Enable = 1;
				}

				if((DDR[i] & 0x20) >> 5){
                    if(Channel_Number_1 <= 32) DDR_ROC_Resets_1 = (DDR_ROC_Resets_1 | (1 << (Channel_Number_1 - 1)));
                    else                       DDR_ROC_Resets_2 = (DDR_ROC_Resets_2 | (1 << (Channel_Number_1 - 33)));
                    ROC_Resets++;
					ROC_Enable = 1;
				}

				if((DDR[i] & 0x10) >> 4){
					cout << "Stack is full!" << endl;
				}

				if(Stack_DDR != 0){
					//cout << "The stack count is " << std::dec << Stack_DDR << endl;
				}
			}
		}
    }
	DDROddEvent(DDR, Current_Event_Num_DDR);

	DDR_Analyze.push_back(Current_Event_Num_DDR);
	DDR_Analyze.push_back(Total_Events_DDR);
	DDR_Analyze.push_back(Incorrect_Event_Num_DDR);
	DDR_Analyze.push_back(Timeout_Error);
	DDR_Analyze.push_back(pixel_hit_DDR);
	DDR_Analyze.push_back(BX);
	DDR_Analyze.push_back(Event_Size);
	for(uint32_t i = 0; i != 48; i++){
		for(uint32_t it = 0; it != 8; it++)
			DDR_Analyze.push_back(Num_Hits_DDR[i][it]);
	}

	return DDR_Analyze;
}

