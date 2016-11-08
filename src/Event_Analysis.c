std::vector<uint32_t> Spy_FIFO_Event(vector<uint32_t>& FIFO, vector<uint32_t>& FIFO_CHB, int Compare_Event_Spy){
    std::vector<uint32_t> Spy_Analyze;
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
				cout << "There was a NTP at event " << dec << Current_Event_Num_Spy << endl;
			}

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
			  std::cout << "There was a PKAM Reset in Spy FIFO at Event " << dec << Current_Event_Num_Spy << std::endl;
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
			
			if(ROC_Value_Spy != 1){
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
			cout << "The Stack count is " << dec << Stack_Spy[1] << endl;
		}

		if(Error_Count_Spy != 0 && Error_Loop_Spy == 1){
			std::cout << "Event = " << std::dec << Current_Event_Num_Spy << std::endl; // and Loop = " << std::dec << l << std::endl; 
			std::cout << "Total Number of Errors = " <<std::dec << Error_Count_Spy << std::endl;
			int Error_Spy = 0;

			for(Error_Spy = Start_Event_Spy; Error_Spy <= END_Event_Spy; Error_Spy++){
				std::cout << "FIFO[" << std::dec << Error_Spy << "] = " << std::hex << FIFO[Error_Spy] << std::endl;
			}

			for(Error_Spy = Start_Event_Spy; Error_Spy <= END_Event_Spy; Error_Spy++){
				std::cout << "FIFO_CHB[" << std::dec << Error_Spy << "] = " << std::hex << FIFO_CHB[Error_Spy] << std::endl;
			}
			Error_Loop_Spy = 0;
		}

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

	Spy_Analyze.push_back(Compare_Event_Spy);
	Spy_Analyze.push_back(Total_Events_Spy);
	Spy_Analyze.push_back(Incorrect_Event_Num_Spy);
	Spy_Analyze.push_back(Incorrect_Header_Spy);
	Spy_Analyze.push_back(Incorrect_Trailer_Spy);
	Spy_Analyze.push_back(Incorrect_ROC_Spy);
	Spy_Analyze.push_back(Incorrect_Pixel_Spy);

	if(Error_Count_Spy != 0){
		std::cout << "There have been " << std::dec << Incorrect_Event_Num_Spy << " Incorrect Event Numbers " << std::endl;
		std::cout << "There have been " << std::dec << Incorrect_Header_Spy << " Incorrect TBM Headers " << std::endl;
		std::cout << "There have been " << std::dec << Incorrect_Trailer_Spy << " Incorrect TBM Trailers " << std::endl;
		std::cout << "There have been " << std::dec << Incorrect_ROC_Spy << " incorrect Number of ROCs " << std::endl;
		std::cout << "There have been " << std::dec << Incorrect_Pixel_Spy << " incorrect Number of Pixels " << std::endl;
		std::cout << " " << std::endl;
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
        //std::cout << "Event Num = " << std::dec << (FIFO1[i] && 0xff) << std::endl;
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
          std::cout << "There was a NTP" << std::endl;

        if((FIFO1[i] & 0x40) >> 6)
          std::cout << "There was a TBM Reset in FIFO1" << std::endl;

        if((FIFO1[i] & 0x20) >> 5)
          std::cout << "There was a ROC Reset in FIFO1" << std::endl;

        if((FIFO1[i] & 0x10) >> 4)
          std::cout << "Stack is full!" << std::endl;

        if(((FIFO1[i] & 0x200) >> 9))
          std::cout << "There was a PKAM Reset in FIFO1 at Event " << std::dec << Current_Event_Num_FIFO1 << std::endl;

        Stack_FIFO1 = ((FIFO1[i] >> 12) & 0x3F);

        if(Stack_FIFO1 != 0)
          std::cout << "The stack count is " << std::dec << Stack_FIFO1 << std::endl;
      } 
      if(Error_Loop_FIFO1 == 1 && Error_Count_FIFO1 != 0){
        std::cout << "Event = " << std::dec << Current_Event_Num_FIFO1 << std::endl; //" and Loop = " << std::dec << l << std::endl;
        std::cout << "Total Number of Errors = " <<std::dec << Error_Count_FIFO1 << std::endl;
        int Error_FIFO1 = 0;

        for(Error_FIFO1 = Start_Event_FIFO1; Error_FIFO1 <= END_Event_FIFO1; Error_FIFO1++){
          std::cout << "FIFO1[" << std::dec << Error_FIFO1 << "] = " << std::hex << FIFO1[Error_FIFO1] << std::endl;
        }

        //for(Error = Start_Event; Error <= END_Event; Error++){
        //std::cout << "FIFO_CHB[" << std::dec << Error << "] = " << std::hex << FIFO_CHB[Error] << std::endl;
        //}
        Error_Loop_FIFO1 = 0;  
      }

    }// Check FIFO1 for Information

    FIFO1_Analyze.push_back(Compare_Event_FIFO1);
    FIFO1_Analyze.push_back(Total_Events_FIFO1);
    FIFO1_Analyze.push_back(Incorrect_Event_Num_FIFO1);
    FIFO1_Analyze.push_back(Incorrect_Header_FIFO1);
    FIFO1_Analyze.push_back(Incorrect_Trailer_FIFO1);

    if(Error_Count_FIFO1 != 0){
    	std::cout << "There have been " << std::dec << Total_Events_FIFO1 << " Events" << std::endl;
    	std::cout << "There have been " << std::dec << Incorrect_Event_Num_FIFO1 << " Incorrect Event Numbers " << std::endl;
    	std::cout << "There have been " << std::dec << Incorrect_Header_FIFO1 << " Incorrect TBM Headers " << std::endl;
    	std::cout << "There have been " << std::dec << Incorrect_Trailer_FIFO1 << " Incorrect TBM Trailers " << std::endl;
    	std::cout << " " << std::endl;
    }

    return FIFO1_Analyze;
}

std::vector<uint32_t> Error_Event(vector<uint32_t> Error_FIFO){
	std::vector<uint32_t> Error_Analyze;
  	int Total_Events_Error = 0;
  	int Incorrect_Header_Error = 0;
  	int Incorrect_Trailer_Error = 0;
  	int Incorrect_Event_Num_Error = 0;
  	int Incorrect_ROC_Error = 0;
  	int ROC_Value_Error = 0;
  	int Stack_Error = 0;
  	int Error_Count_Error = 0;
  	int Compare_Event_Error = 0;
  	int Timeout_Error_Error = 0;

	for ( uint32_t i = 0; i != Error_FIFO.size(); i++){
		uint32_t cChannel = (Error_FIFO[i] & 0xFC000000) >> 26;
        	uint32_t cMarker = (Error_FIFO[i] & 0x03E00000) >> 21;
        	uint32_t cL1Actr = (Error_FIFO[i] & 0x001FE000) >> 13;
        	uint32_t cErrorWord = (Error_FIFO[i] & 0x00001FFF);

        	if(Error_FIFO[i] == 0x0)
        	        break;

        	switch(cMarker){
        	        case 29 :{
                	        Timeout_Error_Error++;
                        	std::cout << "There was a timeout at event " << std::dec << cL1Actr << " in channel " << cChannel << std::endl;
                        	Stack_Error = Error_FIFO[i] & 0x3F;
                        	if(Stack_Error > 0)
                        	        std::cout << "Stack is " << std::dec << Stack_Error << std::endl;
                	}
                	case 31 :{
                        	std::cout << "There was an Event Num error at event " << std::dec << cL1Actr << " in channel " << cChannel << std::endl;
                        	std::cout << "Expected event " << (cErrorWord & 0xFF) << std::endl;
                        	Incorrect_Event_Num_Error++;
                	}
                	case 30 :{
				if((cErrorWord & 0x800) >> 11){
                                        std::cout << "There was a # of ROC Error at event " << cL1Actr << " in channel " << cChannel << std::endl;
                                }
                                if((cErrorWord & 0x200) >> 9){
                                        std::cout << "There was a PKAM Reset at Event " << cL1Actr << " in channel " << cChannel << std::endl;
                                }
                        	if((cErrorWord & 0x80) >> 7){
                                	std::cout << "There was a NTP at event " << cL1Actr << " in channel " << cChannel << std::endl;
                        	}

                        	if((cErrorWord & 0x40) >> 6){
                                	std::cout << "There was a TBM Reset at event " << cL1Actr << " in channel " << cChannel << std::endl;
                        	}

                        	if((cErrorWord & 0x20) >> 5){
                                	std::cout << "There was a ROC Reset at event " << cL1Actr << " in channel " << cChannel << std::endl;
                        	}

                        	if((cErrorWord & 0x10) >> 4){
                                	std::cout << "Stack is full!" << std::endl;
                        	}
			if(Stack_Error != 0){
                        	        std::cout << "The stack count is " << std::dec << Stack_Error << std::endl;
                        	}
                	}
        	}

        	Total_Events_Error = cL1Actr;

    	}

	Error_Analyze.push_back(Total_Events_Error);
	Error_Analyze.push_back(Incorrect_Event_Num_Error);
	Error_Analyze.push_back(Timeout_Error_Error);

       // std::cout << "There were " << Total_Events_Error << " Events in Error FIFO. " << std::endl;
       // std::cout << "There were " << Incorrect_Event_Num_Error << " Incorrect Event numbers." << std::endl;
       // std::cout << "There were " << Timeout_Error_Error << " timeout errors." << std::endl;
       // 	std::cout << " " << std::endl;

	return Error_Analyze;
}

std::vector<uint32_t> DDR_Event(vector<uint32_t> DDR, int Compare_Event_DDR){
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
	//int Compare_Event_DDR = 0;
	int Channel_Number_1_DDR = 0;
	int Channel_Number_2_DDR = 0;
	int ROC_Identifier_1_DDR = 0;
	int ROC_Identifier_2_DDR = 0;
	int Timeout_Error_DDR = 0;
	int DDR_Event_Num_Error_DDR = 0;
	int pixel_hit_DDR = 0;
	int DCOL_DDR = 0;
	int PXL_DDR = 0;
	int Head_DDR = 0;
	int BX = 0;

	for ( uint32_t i = 0; i != DDR.size(); i+=2){

		if(((DDR[i] >> 28) == 0x5) && Head_DDR == 0){
			//Header
			//std::cout << BOLDGREEN << "Evt. ty " << ( (cWord >> 56) & 0xF ) << " L1A Id " << ( (cWord >> 32) & 0xFFFFFF) << " BX Id " << ( (cWord >> 20) & 0xFFF ) << " Source Id " << ( (cWord >> 8) & 0xFFF) << " FOV " << ( (cWord >> 4) & 0xF) << RESET << std::endl;
			Head_DDR = 1;
			Total_Events_DDR++;
			Current_Event_Num_DDR = ((DDR[i]) & 0xFFFFFF);
			BX = (DDR[i + 1] >> 20) & 0xFFF;
			std::cout << "BX: " << std::dec << BX << std::endl;
			if(Current_Event_Num_DDR - Compare_Event_DDR != 1 && Compare_Event_DDR - Current_Event_Num_DDR != 255){
				Incorrect_Event_Num_DDR++;
				Error_Count_DDR++;
			}
			Compare_Event_DDR = Current_Event_Num_DDR;		
		}

		else if((DDR[i] >> 28) == 0xa){
			//Trailer
			Head_DDR = 0;
			//std::cout << BOLDRED << "Evt. Length " << ( (cWord >> 32) & 0xFFFFFF ) << " CRC " << ( (cWord >> 16) & 0xFFFF) << RESET << std::endl;
		}
		else{
			//std::cout << "Channel " << ( (cWord1 >> 26) & 0x3F) << " ROC " <<  ( (cWord1 >> 21) & 0x1F) << " DC " << ( (cWord1 >> 16) & 0x1F) << " Pxl " << ( (cWord1 >> 8) & 0xFF) << " PH " << (cWord1 & 0xFF) << std::endl;
			//std::cout << "Channel " << ( (cWord2 >> 26) & 0x3F) << " ROC " <<  ( (cWord2 >> 21) & 0x1F) << " DC " << ( (cWord2 >> 16) & 0x1F) << " Pxl " << ( (cWord2 >> 8) & 0xFF) << " PH " << (cWord2 & 0xFF) << std::endl;
			Channel_Number_1_DDR = (DDR[i] >> 26) & 0x3F;
			Channel_Number_2_DDR = (DDR[i + 1] >> 26) & 0x3F;
			ROC_Identifier_1_DDR = (DDR[i] >> 21) & 0x1F;
			ROC_Identifier_2_DDR = (DDR[i + 1] >> 21) & 0x1F;
			//std::cout << "ROC_1 = " << std::dec << ROC_Identifier_1 << " ROC_2 = " << ROC_Identifier_2 << std::endl;
			if( ROC_Identifier_1_DDR == 29){
				Timeout_Error_DDR++;
				std::cout << "There was a timeout at event " << std::dec << Current_Event_Num_DDR << " in channel " << Channel_Number_1_DDR << std::endl;
				Stack_DDR = DDR[i] & 0x3F;
			}
			else if( ROC_Identifier_2_DDR == 29){
                                Timeout_Error_DDR++;
                                std::cout << "There was a timeout at event " << std::dec << Current_Event_Num_DDR << " in channel " << Channel_Number_2_DDR << std::endl;
                                //Stack_DDR = DDR[i] & 0x3F;
                        }
			else if( ROC_Identifier_1_DDR == 31){
				DDR_Event_Num_Error_DDR++;
				std::cout << "There was an Event Num error at event " << std::dec << Current_Event_Num_DDR << " in channel " << Channel_Number_1_DDR << std::endl;
			}
			else if( ROC_Identifier_2_DDR == 31){
                                DDR_Event_Num_Error_DDR++;
                                std::cout << "There was an Event Num error at event " << std::dec << Current_Event_Num_DDR << " in channel " << Channel_Number_2_DDR << std::endl;
                        }
			else if ( (ROC_Identifier_1_DDR == 1) && (Channel_Number_1_DDR == 1) && ((DDR[i - 1] >> 28) == 0xa)){ // && ROC_Identifier_1 <= 8){
				if (ROC_Identifier_2_DDR == 8)
					Roc_num_DDR = 1;
				else
					Roc_num_DDR = 0;
				pixel_hit_DDR++;
				DCOL_DDR = (DDR[i] >> 16) & 0x1F;
				PXL_DDR = (DDR[i] >> 8) & 0xFF;
				//	std::cout << "DCOL = " << std::dec << DCOL << " PXL = " << PXL << std::endl;
			}
			if ( ROC_Identifier_2_DDR == 30){
                                if(Roc_num_DDR != 1){
                                        Incorrect_ROC_DDR++;
                                        Error_Count_DDR++;
                                }
                                if((DDR[i + 1] & 0x800) >> 11){
                                std::cout << "There was a # of ROC error at event " << Current_Event_Num_DDR << " in channel " << Channel_Number_2_DDR << std::endl;
                                }
                                if((DDR[i + 1] & 0x200) >> 9){
                                        std::cout << "There was a PKAM Reset at event " << Current_Event_Num_DDR << " in channel " << Channel_Number_2_DDR << std::endl;
                                }
				if((DDR[i + 1] & 0x80) >> 7){
                                        std::cout << "There was a NTP at event " << Current_Event_Num_DDR << std::endl;
                                }

                                if((DDR[i + 1] & 0x40) >> 6){
                                        std::cout << "There was a TBM Reset in DDR at event " << Current_Event_Num_DDR << " in channel " << Channel_Number_2_DDR << std::endl;
                                }

                                if((DDR[i + 1] & 0x20) >> 5){
                                        std::cout << "There was a ROC Reset in DDR at event " << Current_Event_Num_DDR << " in channel " << Channel_Number_2_DDR << std::endl;
                                }

                                if((DDR[i + 1] & 0x10) >> 4){
                                        std::cout << "Stack is full!" << std::endl;
                                }

                                if(((DDR[i + 1] & 0x4000) >> 18)){
                                        std::cout << "There was a PKAM Reset in DDR at Event " << std::dec << Current_Event_Num_DDR << std::endl;
                                }

                                if(Stack_DDR != 0){
                                        std::cout << "The stack count is " << std::dec << Stack_DDR << std::endl;
                                }
                        }
			if ( ROC_Identifier_1_DDR == 30){
				if(Roc_num_DDR != 1){
					Incorrect_ROC_DDR++;
					Error_Count_DDR++;
				}
				if((DDR[i] & 0x800) >> 11){
                                        std::cout << "There was a # of ROC error at event " << Current_Event_Num_DDR << " in channel " << Channel_Number_1_DDR << std::endl;
                                }
                                if((DDR[i] & 0x200) >> 9){
                                        std::cout << "There was a PKAM Reset at event " << Current_Event_Num_DDR << " in channel " << Channel_Number_1_DDR << std::endl;
                                }
				if((DDR[i] & 0x80) >> 7){
					std::cout << "There was a NTP at event " << Current_Event_Num_DDR << std::endl;
				}

				if((DDR[i] & 0x40) >> 6){
					std::cout << "There was a TBM Reset in DDR at event " << Current_Event_Num_DDR << " in channel " << Channel_Number_1_DDR << std::endl;
				}

				if((DDR[i] & 0x20) >> 5){
					std::cout << "There was a ROC Reset in DDR at event " << Current_Event_Num_DDR << " in channel " << Channel_Number_1_DDR << std::endl;
				}

				if((DDR[i] & 0x10) >> 4){
					std::cout << "Stack is full!" << std::endl;
				}

				if(((DDR[i] & 0x4000) >> 18)){
					std::cout << "There was a PKAM Reset in DDR at Event " << std::dec << Current_Event_Num_DDR << std::endl;
				}

				if(Stack_DDR != 0){
					std::cout << "The stack count is " << std::dec << Stack_DDR << std::endl;
				}
			}
		}
	}
	DDR_Analyze.push_back(Current_Event_Num_DDR);
	DDR_Analyze.push_back(Total_Events_DDR);
	DDR_Analyze.push_back(Incorrect_Event_Num_DDR);
	DDR_Analyze.push_back(Timeout_Error_DDR);
	DDR_Analyze.push_back(pixel_hit_DDR);
	DDR_Analyze.push_back(BX);
	
	//std::cout << "There were " << Total_Events_DDR << " Events in DDR. " << std::endl;
	//std::cout << "There were " << Incorrect_Event_Num_DDR << " Incorrect Event numbers." << std::endl;
	//std::cout << "There were " << Timeout_Error_DDR << " timeout errors." << std::endl;
	//std::cout << "There were " << pixel_hit_DDR << " pixel hits." << std::endl;
	//std::cout << " " << std::endl;

	return DDR_Analyze;
}
