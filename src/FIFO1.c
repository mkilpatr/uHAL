//ROOT include files
#include "TCanvas.h"
#include "TH2.h"
#include "TH2F.h"
#include "TColor.h"
#include "TStyle.h"
#include "TFile.h"
#include "DDR.c"

void Pixel_Alive_Plots(int DCOL[], int PXL[], int Column[], int Row[], int Spy_Col[], int Spy_Row[]){

	TFile * fout;
	fout = new TFile("Pixel_Alive.root", "update");
	TH2F *h1 = new TH2F("h1","Row v. Column",36,-0.5,35.5,251,-0.5,250.5);
	h1->SetDirectory(fout);
	h1->GetXaxis()->SetTitle("Column");
	h1->GetYaxis()->SetTitle("Row");

	TH2F *h2 = new TH2F("h2","PXL v. DCOL",31,-0.5,30.5,166,-0.5,165.5);
	h2->SetDirectory(fout);
	h2->GetXaxis()->SetTitle("DCOL");
	h2->GetYaxis()->SetTitle("PXL");

	TH2F *h3 = new TH2F("h3","Spy Row v. Spy Col",36,-0.5,35.5,251,-0.5,250.5);
        h3->SetDirectory(fout);
        h3->GetXaxis()->SetTitle("Spy Col");
        h3->GetYaxis()->SetTitle("Spy Row");

	for(Int_t i = 0; i < 4160; i++){
		h1->Fill(Column[ i ], Row[ i ]);
		h2->Fill(DCOL[ i ], PXL[ i ]);
		h3->Fill(Spy_Col[ i ], Spy_Row[ i ]);
	}
	
	TCanvas * c1 = new TCanvas("c1","Pixel Alive",200,10,700,500);
	h1->Draw("COLZ");
	h1->Write();
	//h1->SaveAs("Pixel_Alive_In.pdf","pdf");
	TCanvas * c2 = new TCanvas("c2","Pixel Alive Out",200,10,700,500);
	h2->Draw("COLZ");
	h2->Write();
	//h2->SaveAs("Pixel_Alive_Out.pdf","pdf");
	TCanvas * c3 = new TCanvas("c3","Pixel Alive Spy",200,10,700,500);
	h3->Draw("COLZ");
	h3->Write();
	//h3->SaveAs("Pixel_Alive_Spy.pdf","pdf");
}

void Test_Hits_Full_FIFO1(int loops_input, int col_start, int col_input, int row_start, int row_input, int adc_start, int adc_input, int choice_input)
{

  using namespace std;
  using namespace uhal;
  
  const char* cHWFile;

  if (choice_input == 0) {
    std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription.xml");
    cHWFile = filename.c_str();
  }
  else if (choice_input == 1) {
    std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_Stack.xml");
    cHWFile = filename.c_str();
  }
  else if (choice_input == 2) {
    std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_Stack2.xml");
    cHWFile = filename.c_str();
  }

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

  // configure the HW
  cAmc13Controller.ConfigureAmc13( std::cout );
  cSystemController.ConfigureHw(std::cout );

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

  //get the board info of all boards and start the acquistion
  //for (auto& cFED : cSystemController.fPixFEDVector)
  //{
      auto& cFED = cSystemController.fPixFEDVector[0];
      cSystemController.fFEDInterface->getBoardInfo(cFED);
      cSystemController.fFEDInterface->findPhases(cFED);
  //}
 
  auto& cFED2 = cSystemController.fPixFEDVector[1];
  auto& cFED3 = cSystemController.fPixFEDVector[2];

  std::vector<unsigned int> FIFO;
  std::vector<unsigned int> FIFO1;
  std::vector<unsigned int> FIFO1_marker;
  std::vector<unsigned int> Event_Number;
  std::vector<string> Gray_Code = generateGrayArr();
  unsigned int Event_one = 0;
  unsigned int Event_two = 0;
  
  int Column[ 4199 ] = { NULL };
  int Row[ 4199 ] = { NULL };
  int Spy_Col[ 4199 ] = { NULL };
  int Spy_Row[ 4199 ] = { NULL };
  int DCOL[ 4199 ] = { NULL };
  int PXL[ 4199 ] = { NULL };
  int Max_Array = 0;
  int Total_Events = 0;
  int Incorrect_Header = 0;
  int Incorrect_Trailer = 0;
  int Incorrect_Event_Num = 0;
  int Incorrect_ROC = 0;
  int Incorrect_adc = 0;
  int Incorrect_col = 0;
  int Incorrect_row = 0;
  int Incorrect_pix_info = 0;
  int ROC_Value = 0;
  int Compare_Event = 0;
  int Loops = loops_input;
  uint32_t Current_Event_Num = 0;
  uint32_t Current_col_num = 0;
  uint32_t Current_row_num = 0;
  uint32_t Current_adc_num = 0;
  int Error_Count = 0;
  int Do_Error = 0;
  int Pixel = 0;
  int Pixel_Hit = 0;
  int Incorrect_Pixel = 0;
  int new_row = 0;
  int Hit_infoA_buff = 0;
  int Hit_infoB_buff = 0;
  int Hit_infoA_new = 0;
  int Hit_infoB_new = 0;

  std::fill(FIFO1.begin(), FIFO1.end(), 0);
  std::fill(FIFO1_marker.begin(), FIFO1_marker.end(), 0);
  std::fill(FIFO.begin(), FIFO.end(), 0);

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

  for (int l = 0; l < Loops; l++) 
  {
    int Hit_infoA = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);
    int Hit_infoB = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);

    int row = 0;
    int col = 0;
    int dcol = 0;
    int adc = 0;

    for (row = row_start; row < max_row; row++) {

      for (col = col_start; col < max_col; col++) {

	if(col % 2 == 0)
		dcol = col/2;

   	//std::cout << "Starting row " << std::dec << row << ", column " << std::dec << col << std::endl;

        for (adc = adc_start; adc < 8/*max_adc*/; adc+=8) {

          for (int j = 0; j < 8; j++) {

            for (int i = 0; i < 1; i++) {
              //Write_ROCs( i, Hit_infoA, Hit_infoB);

              ROCSA[j] = ROCSA[j] + index[i];
              ROCSB[j] = ROCSB[j] + index[i];

	      Hit_infoA_buff = Illegal_Col(dcol);
	      Hit_infoB_buff = Illegal_Col(dcol);
	      new_row = New_Row(row, col);
	      
              Hit_infoA_new = (new_row << 16) | (Hit_infoA_buff << 8) | (Hit_infoA & 0xFF);
              Hit_infoB_new = (new_row << 16) | (Hit_infoB_buff << 8) | (Hit_infoB & 0xFF);

	      cSystemController.fFEDInterface->WriteBoardReg(cFED2, ROCSA[j], Hit_infoA_new);
	      cSystemController.fFEDInterface->WriteBoardReg(cFED2, ROCSB[j], Hit_infoB_new);

	      cSystemController.fFEDInterface->WriteBoardReg(cFED3, ROCSA[j], Hit_infoA_new);
              cSystemController.fFEDInterface->WriteBoardReg(cFED3, ROCSB[j], Hit_infoB_new);
          
              ROCSA[j] = ROCSA[j].substr(0,9);
              ROCSB[j] = ROCSB[j].substr(0,9);
	
            }
            Hit_infoA = Hit_infoA + 0x00000001;
            Hit_infoB = Hit_infoB + 0x00000001;

            adc_loops++;
          }

	  cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);

	  //for (auto& cFED : cSystemController.fPixFEDVector)
  	  //{
            if(l == 0 && row == 0 && col == 0 && adc == 0){
	      sleep(8);
	      cAmc13Controller.fAmc13Interface->BurstL1A();
            }
            else{
	      cAmc13Controller.fAmc13Interface->BurstL1A();
            }
	    sleep(0.050);

	    FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
	    FIFO1 = cSystemController.fFEDInterface->readFIFO1_vec(cFED);
            FIFO1_marker = cSystemController.fFEDInterface->readFIFO1Marker(cFED);
	  //}
	  
          int Roc_num;
          int adc_adjust;
          int row_adjust;
          int col_adjust;
	  int n = (row*52) + col;
	  int k = 0;
          int j = 0;
          int h = 0;
          int Next_step = 0;
          int Next_step_roc = 1;
	  int Error_Count = 0;
          int Num_ROC = 0;
          int END_Event = 0;
          int Error_Loop = 0;
          int Start_Event = 0;
          int Stack;
          int Do_Stack = 0;
	  int ROC = 1;

          for ( uint32_t i = 0; i < FIFO.size(); i++){
                if (((FIFO[i] >> 4) & 0xF) == 0x2){
                       	Spy_Col[ n ] = ((FIFO[i - 1] << 2) & 0x3F) | ((FIFO[i] >> 2) & 0x3);
                }
                else if (((FIFO[i] >> 4) & 0xF) == 0x4){
                       	Spy_Row[ n ] = ((FIFO[i - 2] << 7) & 0x1FF) | ((FIFO[i - 1] << 3) & 0x7F) | ((FIFO[i] >> 1) & 0x7);
                }
	  }


	  for ( uint32_t i = 0; i < FIFO1.size(); i++)
          {
            if (FIFO1[i] == 0x0) {
              break;
            }

            if (FIFO1_marker[i] == 8 && Next_step == 0){
              //header
              //std::cout << "Event Num = " << std::dec << (FIFO1[i] && 0xff) << std::endl;
	      Start_Event = i;
	      Error_Loop = 0;
	      Total_Events++;
              Current_Event_Num = (FIFO1[i] & 0xFF);
              if(((Current_Event_Num - Compare_Event) != 1) && ((Compare_Event - Current_Event_Num) != 255)){
                Incorrect_Event_Num++;
                Error_Count++;
              }
              Compare_Event = Current_Event_Num;         
	      Next_step = 1;
	    }
	    else if (FIFO1_marker[i] == 8 && Next_step != 0){
		Incorrect_Header++;
                Error_Count++;
                continue;
	    }

            if (FIFO1_marker[i] == 0xc){
              //ROC header
              Roc_num = ((FIFO1[i] >> 21) & 0x1f);
            }

            if (FIFO1_marker[i] == 0x1) {
 	      Pixel++;

	      if(Roc_num == 1 && adc == 0){
	      	std::cout << "column = " << std::dec << col << ", row = " << row << std::endl;
              	Column[ n ] = (Hit_infoA_new >> 8) & 0x3F;
              	Row[ n ] = (Hit_infoA_new >> 16) & 0x1FF;
              	DCOL[ n ] = (FIFO1[i] >> 16) & 0x1F;
              	PXL[ n ] = (FIFO1[i] >> 8) & 0xFF;
        	Max_Array = n;

	      }
	      adc_adjust = adc + Roc_num - 1;
	      /*if ((FIFO1[i] & 0xff) != adc_adjust) {
                std::cout << "ADC error! Correct ADC = " << std::dec << adc_adjust << std::endl;
                std::cout << "ADC = " << std::dec << (FIFO1[i] & 0xff) << std::endl;
              }
		
              if (((FIFO1[i] >> 8) & 0xff) >= 105) {
                std::cout << "PXL out of range, PXL = " << std::dec << PXL << std::endl;
              }
              if (((FIFO1[i] >> 16) & 0x1f) >= 26) {
                std::cout << "DCOL out of range, DCOL =  " << std::dec << DCOL << std::endl;
              }*/
            }
	    else if (FIFO1_marker[i] == 0x4){
		//TBM Trailer
		END_Event = i;
		Error_Loop = 1;
		if(Roc_num != 8){
			Incorrect_ROC++;
                	Error_Count++;
		}
		if((FIFO1[i] & 0x80) >> 7){
                	std::cout << "There was a NTP" << std::endl;
              	}

              	if((FIFO1[i] & 0x40) >> 6){
                	std::cout << "There was a TBM Reset" << std::endl;
              	}

              	if((FIFO1[i] & 0x20) >> 5){
                	std::cout << "There was a ROC Reset" << std::endl;
              	}
		
		if((FIFO1[i] & 0x10) >> 4){
                	std::cout << "Stack is full!" << std::endl;
              	}
		
		if(((FIFO1[i] & 0x4000) >> 18)){
                	std::cout << "There was a PKAM Reset at Event " << std::dec << Current_Event_Num << std::endl;
              	}
		
		Stack = ((FIFO1[i] >> 8) & 0x3F);
	    	
	  	if(Stack != 0){
			std::cout << "The stack count is " << std::dec << Stack << std::endl;
	    	}
	    } 
	    if(Error_Loop == 1 && Error_Count != 0){
	       std::cout << "Event = " << std::dec << Current_Event_Num << " and Loop = " << std::dec << l << std::endl;
               std::cout << "Total Number of Errors = " <<std::dec << Error_Count << std::endl;
               int Error = 0;

               for(Error = Start_Event; Error <= END_Event; Error++){
                 std::cout << "FIFO1[" << std::dec << Error << "] = " << std::hex << FIFO1[Error] << std::endl;
               }

               //for(Error = Start_Event; Error <= END_Event; Error++){
                 //std::cout << "FIFO_CHB[" << std::dec << Error << "] = " << std::hex << FIFO_CHB[Error] << std::endl;
               //}
               Error_Loop = 0;	
	    }

          }// Check FIFO1 for Information

             
        } // Ends adc loop

        col_loops++;
        adc_loops = 0;

      } // Ends column loop

      row_loops++;
      col_loops = 0;

    } // Ends row loop

    row_loops = 0;
  
  } // Ends Loops loop

  //Pixel_Alive_In( Column, Row);  
  Pixel_Alive_Plots( DCOL, PXL, Column, Row, Spy_Col, Spy_Row);

 if (choice_input == 0) {
    std::cout << "There have been " << std::dec << Total_Events << " Events" << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Event_Num << " Incorrect Event Numbers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Header << " Incorrect TBM Headers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Trailer << " Incorrect TBM Trailers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_ROC << " incorrect Number of ROCs " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Pixel << " incorrect Number of Pixels " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_pix_info << " incorrect Number of pix hit info " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_adc << " incorrect Number of adc values " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_col << " incorrect Number of columns " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_row << " incorrect Number of rows " << std::endl;
  }
  else if (choice_input > 0) {
    std::cout << "There have been " << std::dec << Total_Events << " Events" << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Event_Num << " Incorrect Event Numbers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Header << " Incorrect TBM Headers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Trailer << " Incorrect TBM Trailers " << std::endl; 
    std::cout << "There have been " << std::dec << Incorrect_ROC << " incorrect Number of ROCs " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Pixel << " incorrect Number of Pixels " << std::endl;
  }
  
}

void Test_Resets_FIFO1(int loops_input, int choice_input){

  using namespace std;
  using namespace uhal;
  
  const char* cHWFile;

  if (choice_input == 0) {
    std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_PKAM.xml");
    cHWFile = filename.c_str();
  }
  else if (choice_input == 1) {
    std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_TBM.xml");
    cHWFile = filename.c_str();
  }
  else if (choice_input == 2) {
    std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_ROC.xml");
    cHWFile = filename.c_str();
  }

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

  // configure the HW
  cAmc13Controller.ConfigureAmc13( std::cout );
  cSystemController.ConfigureHw(std::cout );

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

  //get the board info of all boards and start the acquistion
  //for (auto& cFED : cSystemController.fPixFEDVector)
  //{
      auto& cFED = cSystemController.fPixFEDVector[0];
      cSystemController.fFEDInterface->getBoardInfo(cFED);
      cSystemController.fFEDInterface->findPhases(cFED);
  //}
 
  auto& cFED2 = cSystemController.fPixFEDVector[1];
  auto& cFED3 = cSystemController.fPixFEDVector[2];

  std::vector<unsigned int> FIFO;
  std::vector<unsigned int> FIFO1;
  std::vector<unsigned int> FIFO1_marker;
  std::vector<unsigned int> Event_Number;
  std::vector<string> Gray_Code = generateGrayArr();
  unsigned int Event_one = 0;
  unsigned int Event_two = 0;
  
  int Column[ 4199 ] = { NULL };
  int Row[ 4199 ] = { NULL };
  int Spy_Col[ 4199 ] = { NULL };
  int Spy_Row[ 4199 ] = { NULL };
  int DCOL[ 4199 ] = { NULL };
  int PXL[ 4199 ] = { NULL };
  int Max_Array = 0;
  int Total_Events = 0;
  int Incorrect_Header = 0;
  int Incorrect_Trailer = 0;
  int Incorrect_Event_Num = 0;
  int Incorrect_ROC = 0;
  int Incorrect_adc = 0;
  int Incorrect_col = 0;
  int Incorrect_row = 0;
  int Incorrect_pix_info = 0;
  int ROC_Value = 0;
  int Compare_Event = 0;
  int Loops = loops_input;
  uint32_t Current_Event_Num = 0;
  uint32_t Current_col_num = 0;
  uint32_t Current_row_num = 0;
  uint32_t Current_adc_num = 0;
  int Error_Count = 0;
  int Do_Error = 0;
  int Pixel = 0;
  int Pixel_Hit = 0;
  int Incorrect_Pixel = 0;
  int new_row = 0;
  int Hit_infoA_buff = 0;
  int Hit_infoB_buff = 0;
  int Hit_infoA_new = 0;
  int Hit_infoB_new = 0;

  std::fill(FIFO1.begin(), FIFO1.end(), 0);
  std::fill(FIFO1_marker.begin(), FIFO1_marker.end(), 0);
  std::fill(FIFO.begin(), FIFO.end(), 0);

  std::string index[8] = {"0","1","2","3","4","5","6","7"};

  for (int l = 0; l < Loops; l++) 
  {

    cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);

    //for (auto& cFED : cSystemController.fPixFEDVector)
    //{
    if(l == 0){
      sleep(8);
      cAmc13Controller.fAmc13Interface->BurstL1A();
    }
    else{
      cAmc13Controller.fAmc13Interface->BurstL1A(); 
    }
    sleep(0.050);

    FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
    FIFO1 = cSystemController.fFEDInterface->readFIFO1_vec(cFED);
    FIFO1_marker = cSystemController.fFEDInterface->readFIFO1Marker(cFED);
    //}

    int k = 0;
    int j = 0;
    int h = 0;
    int Next_step = 0;
    int Next_step_roc = 1;
    int Error_Count = 0;
    int Num_ROC = 0;
    int END_Event = 0;
    int Error_Loop = 0;
    int Start_Event = 0;
    int Stack;
    int Do_Stack = 0;
    int ROC = 1;
    int Roc_num = 0;

    for ( uint32_t i = 0; i < FIFO1.size(); i++)
    {
      if (FIFO1[i] == 0x0)
        break;

      if (FIFO1_marker[i] == 8 && Next_step == 0){
        //header
        //std::cout << "Event Num = " << std::dec << (FIFO1[i] && 0xff) << std::endl;
        Start_Event = i;
        Error_Loop = 0;
        Total_Events++;
        Current_Event_Num = (FIFO1[i] & 0xFF);
        if(((Current_Event_Num - Compare_Event) != 1) && ((Compare_Event - Current_Event_Num) != 255)){
          Incorrect_Event_Num++;
          Error_Count++;
        }
        Compare_Event = Current_Event_Num;         
        Next_step = 1;
      }
      else if (FIFO1_marker[i] == 8 && Next_step != 0){
        Incorrect_Header++;
        Error_Count++;
        continue;
      }

      if (FIFO1_marker[i] == 0xc){
        //ROC header
        Roc_num = ((FIFO1[i] >> 21) & 0x1f);
      }

      if (FIFO1_marker[i] == 0x1)
        Pixel++;

      else if (FIFO1_marker[i] == 0x4){
        //TBM Trailer
        END_Event = i;
        Error_Loop = 1;
        if(Roc_num != 8){
          Incorrect_ROC++;
          Error_Count++;
        }
        if((FIFO1[i] & 0x80) >> 7)
          std::cout << "There was a NTP" << std::endl;

        if((FIFO1[i] & 0x40) >> 6)
          std::cout << "There was a TBM Reset" << std::endl;

        if((FIFO1[i] & 0x20) >> 5)
          std::cout << "There was a ROC Reset" << std::endl;

        if((FIFO1[i] & 0x10) >> 4)
          std::cout << "Stack is full!" << std::endl;

        if(((FIFO1[i] & 0x4000) >> 18))
          std::cout << "There was a PKAM Reset at Event " << std::dec << Current_Event_Num << std::endl;

        Stack = ((FIFO1[i] >> 8) & 0x3F);

        if(Stack != 0)
          std::cout << "The stack count is " << std::dec << Stack << std::endl;
      } 
      if(Error_Loop == 1 && Error_Count != 0){
        std::cout << "Event = " << std::dec << Current_Event_Num << " and Loop = " << std::dec << l << std::endl;
        std::cout << "Total Number of Errors = " <<std::dec << Error_Count << std::endl;
        int Error = 0;

        for(Error = Start_Event; Error <= END_Event; Error++){
          std::cout << "FIFO1[" << std::dec << Error << "] = " << std::hex << FIFO1[Error] << std::endl;
        }

        //for(Error = Start_Event; Error <= END_Event; Error++){
        //std::cout << "FIFO_CHB[" << std::dec << Error << "] = " << std::hex << FIFO_CHB[Error] << std::endl;
        //}
        Error_Loop = 0;  
      }

    }// Check FIFO1 for Information

  } // Ends Loops loop

  std::cout << "There have been " << std::dec << Total_Events << " Events" << std::endl;
  std::cout << "There have been " << std::dec << Incorrect_Event_Num << " Incorrect Event Numbers " << std::endl;
  std::cout << "There have been " << std::dec << Incorrect_Header << " Incorrect TBM Headers " << std::endl;
  std::cout << "There have been " << std::dec << Incorrect_Trailer << " Incorrect TBM Trailers " << std::endl;

}
