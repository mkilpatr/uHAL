//ROOT include files
#include "TCanvas.h"
#include "TH2.h"
#include "TH2F.h"
#include "TColor.h"
#include "TStyle.h"
#include "TFile.h"
#include "Error_FIFO.c"

//Used to plot the pixel alive output and determine if each pixel got 1 hit
void Pixel_Alive_Plots(vector<vector<unsigned int>>& DCOL, vector<vector<unsigned int>>& PXL, vector<vector<unsigned int>>& Column, vector<vector<unsigned int>>& Row, vector<vector<unsigned int>>& Spy_Col, vector<vector<unsigned int>>& Spy_Row, vector<vector<unsigned int>>& DDR_Col, vector<vector<unsigned int>>& DDR_PXL, int n){

	TFile * fout;
	fout = new TFile("Pixel_Alive_v10_0_fedtestfed.root", "update");

        TH2F *h1[8];	
	TH2F *h2[8];
	TH2F *h3[8];
	TH2F *h4[8];
	TCanvas *Total[8];
	TCanvas *AllDDR;

	char *h1name = new char[8];
	char *h2name = new char[8];
	char *h3name = new char[8];
	char *h4name = new char[8];
	char *Totalname = new char[8];
	char *TitleTotal = new char[8];
	char *SaveName = new char[8];

	for(int i = 0; i < 8; i++){
		
		sprintf(h1name, "h1_%d",i);
		sprintf(h2name, "h2_%d",i);
		sprintf(h3name, "h3_%d",i);
		sprintf(h4name, "h4_%d",i);

		h1[i] = new TH2F(h1name,"Row v. Column",36,-0.5,35.5,301,-0.5,300.5);
		h1[i]->SetDirectory(fout);
		h1[i]->GetXaxis()->SetTitle("Column");
		h1[i]->GetYaxis()->SetTitle("Row");
		
		h2[i] = new TH2F(h2name,"PXL v. DCOL FIFO1",31,-0.5,30.5,166,-0.5,165.5);
		h2[i]->SetDirectory(fout);
		h2[i]->GetXaxis()->SetTitle("DCOL");
		h2[i]->GetYaxis()->SetTitle("PXL");
		
		h3[i] = new TH2F(h3name,"Spy Row v. Spy Col",36,-0.5,35.5,301,-0.5,300.5);
		h3[i]->SetDirectory(fout);
		h3[i]->GetXaxis()->SetTitle("Spy Col");
		h3[i]->GetYaxis()->SetTitle("Spy Row");
		
		h4[i] = new TH2F(h4name,"PXL v. DCOL DDR",31,-0.5,30.5,166,-0.5,165.5);
		h4[i]->SetDirectory(fout);
		h4[i]->GetXaxis()->SetTitle("DCOL");
		h4[i]->GetYaxis()->SetTitle("PXL");
	}

	for(int i = 0; i < 8; i++){
		for(uint32_t j = 0; j < Column[ i ].size(); j++)
			if(Row[ i ][ j ] != 0)
				h1[i]->Fill(Column[ i ][ j ], Row[ i ][ j ]);
		for(uint32_t j = 0; j < DCOL[ i ].size(); j++)
			if(PXL[ i ][ j ] != 0)
				h2[i]->Fill(DCOL[ i ][ j ], PXL[ i ][ j ]);
		for(uint32_t j = 0; j < Spy_Col[ i ].size(); j++)
			if(Spy_Row[ i ][ j ] != 0)
				h3[i]->Fill(Spy_Col[ i ][ j ], Spy_Row[ i ][ j ]);
		for(uint32_t j = 0; j < DDR_Col[ i ].size(); j++)
			if(DDR_PXL[ i ][ j ] != 0)
				h4[i]->Fill(DDR_Col[ i ][ j ], DDR_PXL[ i ][ j ]);
	}

	for(int i = 0; i < 8; i++){	
		sprintf(Totalname, "Total_%d",i);
		sprintf(TitleTotal, "Pixel Alive Total version 10.0 ROC%d",i);
		sprintf(SaveName, "Pixel_Alive_Total_v10_0_fedtestfed_ROC%d",i);

		h1[i]->Draw("COLZ");
		h1[i]->Write();
		h2[i]->Draw("COLZ");
		h2[i]->Write();
		h3[i]->Draw("COLZ");
		h3[i]->Write();
		h4[i]->Draw("COLZ");
        	h4[i]->Write();

		Total[i] = new TCanvas(Totalname,TitleTotal,200,10,700,500);
		Total[i]->Divide(2,2);
		Total[i]->cd(1);
		h1[i]->Draw("COLZ");
		Total[i]->cd(2);
        	h3[i]->Draw("COLZ");
		Total[i]->cd(3);
        	h2[i]->Draw("COLZ");
		Total[i]->cd(4);
        	h4[i]->Draw("COLZ");
		Total[i]->SaveAs(SaveName,"pdf");
	}
	AllDDR = new TCanvas("AllDDR","DDR of all ROCs",200,10,700,500);
	AllDDR->Divide(4,2);
	for(int i = 0; i < 8; i++){
		AllDDR->cd(i + 1);
		h4[i]->Draw("COLZ");
	}
	AllDDR->SaveAs("DDR_all_ROCs_v10.pdf");	
}

void Test_Hits_Full_FIFO1(int loops_input, int col_start, int col_input, int row_start, int row_input, int adc_start, int adc_input, int choice_input, int Num_Hits, int SRAM)
{

  using namespace std;
  using namespace uhal;
  const char* cHWFile;

  if (choice_input == 0) {
    std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_Pixel_Alive.xml");
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

  auto& cFED = cSystemController.fPixFEDVector[0];
  cSystemController.fFEDInterface->getBoardInfo(cFED);
  cSystemController.fFEDInterface->findPhases(cFED);
  cSystemController.fFEDInterface->Start (cFED);
 
  auto& cFED2 = cSystemController.fPixFEDVector[1];
  auto& cFED3 = cSystemController.fPixFEDVector[2];
  auto& cFED4 = cSystemController.fPixFEDVector[3];

  std::vector<unsigned int> FIFO;
  std::vector<std::vector<unsigned int>> FIFO1;
  std::vector<unsigned int> FIFO1_marker;
  std::vector<unsigned int> DDR;
  
  std::vector<std::vector<unsigned int > > Column (8, vector<unsigned int>(8));
  std::vector<std::vector<unsigned int > > Row    (8, vector<unsigned int>(8));
  std::vector<std::vector<unsigned int > > Spy_Col(8, vector<unsigned int>(8));
  std::vector<std::vector<unsigned int > > Spy_Row(8, vector<unsigned int>(8));
  std::vector<std::vector<unsigned int > > DCOL   (8, vector<unsigned int>(8));
  std::vector<std::vector<unsigned int > > PXL    (8, vector<unsigned int>(8));
  std::vector<std::vector<unsigned int > > DDR_Col(8, vector<unsigned int>(8));
  std::vector<std::vector<unsigned int > > DDR_PXL(8, vector<unsigned int>(8));
  int n = 0;
  int Compare_DCOL = 0;
  int Compare_PXL = 0;
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

  //std::fill(Column.begin(), Column.end(), 0);
  //std::fill(Row.begin(), Row.end(), 0);
  //std::fill(Spy_Col.begin(), Spy_Col.end(), 0);
  //std::fill(Spy_Row.begin(), Spy_Row.end(), 0);
  //std::fill(DCOL.begin(), DCOL.end(), 0);
  //std::fill(PXL.begin(), PXL.end(), 0);
  //std::fill(DDR_Col.begin(), DDR_Col.end(), 0);
  //std::fill(DDR_PXL.begin(), DDR_PXL.end(), 0);
  //std::fill(FIFO1.begin(), FIFO1.end(), 0);
  std::fill(FIFO1_marker.begin(), FIFO1_marker.end(), 0);
  std::fill(FIFO.begin(), FIFO.end(), 0);
  std::fill(DDR.begin(), DDR.end(), 0);

  std::cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << std::endl;
  cAmc13Controller.fAmc13Interface->SendEC0();
  cAmc13Controller.fAmc13Interface->EnableBGO(0);
  cAmc13Controller.fAmc13Interface->DisableBGO(0);

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

  //cSystemController.fFEDInterface->WriteBoardReg(cFED, "be_ctrl_regs.disable_BE", 1);

  if(SRAM == 1) SRAM_Mode();
  else SRAM_Disable();

  for (int l = 0; l < Loops; l++) 
  {
    int Hit_infoA = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);
    int Hit_infoB = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);

    int row = 0;
    int col = 0;
    int dcol = 0;
    int adc = 0;

    //Start of pixel alive loop the row and column are transmitted into buffer that change the values to the allowed ones
    for (row = row_start; row < max_row; row++) {

      for (col = col_start; col < max_col; col++) {

	if(col % 2 == 0)
		dcol = col/2;

   	//std::cout << "Starting row " << std::dec << row << ", column " << std::dec << col << std::endl;

        for (adc = adc_start; adc < 8/*max_adc*/; adc+=8) {

	  int Hit_infoA = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);
    	  int Hit_infoB = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);

	  if(SRAM == 0){
          	for (int j = 0; j < 8; j++) {

          	  for (int i = 0; i < 8; i++) {
          	    //Writing the values of PXL/DCOL/ADC into the ROC register to be transmitted to the FED

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
   
	  	    cSystemController.fFEDInterface->WriteBoardReg(cFED4, ROCSA[j], Hit_infoA_new);
          	    cSystemController.fFEDInterface->WriteBoardReg(cFED4, ROCSB[j], Hit_infoB_new);
       
          	    ROCSA[j] = ROCSA[j].substr(0,9);
          	    ROCSB[j] = ROCSB[j].substr(0,9);
	
          	  }
          	  Hit_infoA = Hit_infoA + 0x00000001;
          	  Hit_infoB = Hit_infoB + 0x00000001;

          	  adc_loops++;
          	}

	  	for(int j = 0; j < 8; j++){
	  		for(int i = 0; i < Num_Hits; i++){
	  	      	Column[j].push_back(Hit_infoA_buff);
          			Row[j].push_back(new_row);
	  		}
	  	}
	  }//check for SRAM mode

	  //for (auto& cFED : cSystemController.fPixFEDVector)
  	  //{
            if(l == 0 && row == 0 && col == 0 && adc == 0){
	      cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
	      sleep(8);
	      cAmc13Controller.fAmc13Interface->BurstL1A();
            }
            else{
	      cAmc13Controller.fAmc13Interface->BurstL1A();
            }
	    sleep(0.050);

	    FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
	    FIFO1 = cSystemController.fFEDInterface->readFIFO1_vec(cFED);
            //FIFO1_marker = cSystemController.fFEDInterface->readFIFO1Marker(cFED);
	    DDR = cSystemController.fFEDInterface->ReadData(cFED, 0 );
	  //}
	  
          int Roc_num;
          int adc_adjust;
          int row_adjust;
          int col_adjust;
	  n = ((l*4160) + (row*52) + col);
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
	  int Spy_ROC = 0;
	  int ROC_DDR = 0;

	  for ( uint32_t i = 0; i < FIFO.size(); i++){
		if((((FIFO[0] >> 4) & 0xF) == 0x1) || (((FIFO[0] >> 4) & 0xF) == 0x2) || (((FIFO[0] >> 4) & 0xF) == 0x3) || (((FIFO[0] >> 4) & 0xF) == 0x4) || (((FIFO[0] >> 4) & 0xF) == 0x5) || (((FIFO[0] >> 4) & 0xF) == 0x6))
			break;
		if(((FIFO[i] >> 4) & 0xF) == 0x7)
			Spy_ROC++;
		if ((((FIFO[i] >> 4) & 0xF) == 0x2)){
			int SCOL = ((FIFO[i - 1] & 0xF) << 2) | ((FIFO[i] & 0xF) >> 2);
			Spy_Col[Spy_ROC - 1].push_back(SCOL) ;
		}
		if (((FIFO[i] >> 4) & 0xF) == 0x4){
			int SROW = ((FIFO[i-2] & 0x3) << 7) | ((FIFO[i-1] & 0xF) << 3) | ((FIFO[i] & 0xF) >> 1);
			Spy_Row[Spy_ROC - 1].push_back(SROW);
	  	}
		if(Spy_ROC == 8)
			break;
	  }

	  for ( uint32_t i = 0; i < FIFO1[0].size(); i++)
          {
    	    //std::cout << std::hex << FIFO1_marker[i] << ": " << FIFO1[i] << std::endl;
            if (FIFO1[0][i] == 0x0) {
              break;
            }

            if (FIFO1[1][i] == 8){
              //header
              //std::cout << "Event Num = " << std::dec << (FIFO1[i] && 0xff) << std::endl;
	      Start_Event = i;
	      Error_Loop = 0;
	      Total_Events++;
              Current_Event_Num = (FIFO1[0][i] & 0xFF);
              if(((Current_Event_Num - Compare_Event) != 1) && ((Compare_Event - Current_Event_Num) != 255)){
                Incorrect_Event_Num++;
                Error_Count++;
              }
              Compare_Event = Current_Event_Num;         
	      Next_step = 1;
	    }
	    /*else if (FIFO1_marker[i] == 8 && Next_step != 0){
		Incorrect_Header++;
                Error_Count++;
                continue;
	    }*/

            if (FIFO1[1][i] == 0xc){
              //ROC header
              Roc_num = ((FIFO1[0][i] >> 21) & 0x1f);
            }

            if (FIFO1[1][i] == 0x1) {
 	      Pixel++;

	      if(Roc_num >= 1 && Roc_num <= 8){
		if( col % 51 == 1 && Pixel % Num_Hits == 0)
	      		std::cout << "row = " << row << ", loop = " << l << std::endl;
              	DCOL[Roc_num - 1].push_back((FIFO1[0][i] >> 16) & 0x1F);
              	PXL[Roc_num - 1].push_back((FIFO1[0][i] >> 8) & 0xFF);
		Compare_DCOL = DCOL_Encode(Hit_infoA_buff);
		Compare_PXL = PXL_Encode(new_row);

	      }
	      adc_adjust = adc + Roc_num - 1;
	      if(SRAM == 0){
	      	if ((FIFO1[0][i] & 0xff) != adc_adjust && Pixel % Num_Hits == 0) {
              	  std::cout << "ADC error! Correct ADC = " << std::dec << adc_adjust << std::endl;
              	  std::cout << "ADC = " << std::dec << (FIFO1[0][i] & 0xff) << std::endl;
              	}
	      	  
              	if ( ((FIFO1[0][i] >> 8) & 0xFF) != Compare_PXL) {
              	  std::cout << "PXL Error, PXL input = " << std::dec << Compare_PXL << " PXL output = " << ((FIFO1[0][i] >> 8) & 0xFF) << std::endl;
              	}
              	if ( ((FIFO1[0][i] >> 16) & 0x1F) != Compare_DCOL) {
              	  std::cout << "DCOL  Error, DCOL input = " << std::dec << Compare_DCOL << " DCOL ouput = " << ((FIFO1[0][i] >> 16) & 0x1F) << std::endl;
              	}
	      }
	    }
            
	    else if (FIFO1[1][i] == 0x4){
		//TBM Trailer
		END_Event = i;
		Error_Loop = 1;
		if(Roc_num != 8){
			Incorrect_ROC++;
                	Error_Count++;
		}
		if((FIFO1[0][i] & 0x80) >> 7){
                	std::cout << "There was a NTP" << std::endl;
              	}

              	if((FIFO1[0][i] & 0x40) >> 6){
                	std::cout << "There was a TBM Reset" << std::endl;
              	}

              	if((FIFO1[0][i] & 0x20) >> 5){
                	std::cout << "There was a ROC Reset" << std::endl;
              	}
		
		if((FIFO1[0][i] & 0x10) >> 4){
                	std::cout << "Stack is full!" << std::endl;
              	}
		
		if(((FIFO1[0][i] & 0x40000) >> 18)){
                	std::cout << "There was a PKAM Reset at Event " << std::dec << Current_Event_Num << std::endl;
              	}
		
		Stack = ((FIFO1[0][i] >> 12) & 0x3F);
	    	
	  	if(Stack != 0){
			std::cout << "The stack count is " << std::dec << Stack << std::endl;
	    	}
	    } 
	    if(Error_Loop == 1 && Error_Count != 0){
	       std::cout << "Event = " << std::dec << Current_Event_Num << " and Loop = " << std::dec << l << std::endl;
               std::cout << "Total Number of Errors = " <<std::dec << Error_Count << std::endl;
               int Error = 0;

               for(Error = Start_Event; Error <= END_Event; Error++){
                 std::cout << "FIFO1[" << std::dec << Error << "] = " << std::hex << FIFO1[0][Error] << std::endl;
               }

               //for(Error = Start_Event; Error <= END_Event; Error++){
                 //std::cout << "FIFO_CHB[" << std::dec << Error << "] = " << std::hex << FIFO_CHB[Error] << std::endl;
               //}
               Error_Loop = 0;	
	    }

          }// Check FIFO1 for Information

	  for ( uint32_t i = 0; i < DDR.size(); i++){
		ROC_DDR = (((DDR[i] >> 21) & 0x1F));
		if((((DDR[i] >> 21) & 0x1F) == 29) && (((DDR[i] >> 26) & 0x3F) == 0x1)){
			std::cout << "There was a timeout" << std::endl;
		}
		if ((((DDR[i] >> 26) & 0x3F) == 0x1) && ((DDR[ i - 1 ] >> 28) != 0xa) && ((DDR[ i - 1 ] >> 28) != 0x5) && (ROC_DDR > 0) && (ROC_DDR <= 8)){
                        DDR_Col[ROC_DDR - 1].push_back((DDR[i] >> 16) & 0x1F);
                        DDR_PXL[ROC_DDR - 1].push_back((DDR[i] >> 8) & 0xFF);
			if((((DDR[i] >> 8) & 0xFF) <= 1)){
				std::cout << "DDR[" << std::dec << i << "]: " << std::bitset<32>(DDR[i]) << std::endl;
			}
			if(((DDR[i] >> 8) & 0xFF) >= 162)
				std::cout << "DDR[" << std::dec << i << "]: " << std::bitset<32>(DDR[i]) << std::endl;
			if(DDR_PXL[ROC_DDR - 1].back() <= 1)
				std::cout << "DDR_pxl_vec: " << std::bitset<32>(DDR_PXL[ROC_DDR - 1].back()) << std::endl;
			if(DDR_PXL[ROC_DDR - 1].back() >= 162)
				std::cout << "DDR_pxl_vec: " << std::bitset<32>(DDR_PXL[ROC_DDR - 1].back()) << std::endl;
                }

          }// Check DDR for information

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
  Pixel_Alive_Plots( DCOL, PXL, Column, Row, Spy_Col, Spy_Row, DDR_Col, DDR_PXL, n);

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

//Test Resets by sending one event at a time and reading output
//Can do any reset depending on option
//Trying to convert to single HWDescription file but currently doesn't work - Matt is currently working on it
void Test_Resets_FIFO1(int loops_input, int choice_input){

  using namespace std;
  using namespace uhal;
  
  const char* cHWFile;

  if (choice_input == 0) {
    std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_PKAM.xml");
    cHWFile = filename.c_str();
  }
  else if (choice_input == 1) {
    std::string filename ("/home/fectest/FEDtester/MrPixel/build/ttctest/settings/HWDescription_Resets.xml");
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
  auto& cFED4 = cSystemController.fPixFEDVector[3];

  std::vector<uint32_t> FIFO;
  std::vector<std::vector<uint32_t>> FIFO1(2);
  std::vector<uint32_t> FIFO1_marker;
  std::vector<uint32_t> FIFO1_Analyze;

  int Total_Events_FIFO1 = 0; //Error_Array[0]
  int Incorrect_Header_FIFO1 = 0; //Error_Array[2]
  int Incorrect_Trailer_FIFO1 = 0; //Error_Array[3]
  int Incorrect_Event_Num_FIFO1 = 0; //Error_Array[1]
  int Compare_Event_FIFO1 = 0;

  std::cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << std::endl;
  cAmc13Controller.fAmc13Interface->SendEC0();
  cAmc13Controller.fAmc13Interface->EnableBGO(0);
  sleep(0.001);
  cAmc13Controller.fAmc13Interface->DisableBGO(0);

  for (int l = 0; l < loops_input; l++) 
  {
    if( l == 1 ){
	cAmc13Controller.fAmc13Interface->EnableBGO(0);
  	cAmc13Controller.fAmc13Interface->DisableBGO(0);
    }

    if(l == 0){
      cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
      sleep(8);
      cAmc13Controller.fAmc13Interface->BurstL1A();
    }
    else{
      cAmc13Controller.fAmc13Interface->BurstL1A(); 
    }
    sleep(0.050);

    FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
    FIFO1 = cSystemController.fFEDInterface->readFIFO1_vec(cFED);

    //Error_Array = FIFO1_Event(FIFO1, FIFO1_marker);
    FIFO1_Analyze = FIFO1_Event(FIFO1[0], FIFO1[1], Compare_Event_FIFO1);
    Total_Events_FIFO1 += FIFO1_Analyze[1];
    Incorrect_Header_FIFO1 += FIFO1_Analyze[2];
    Incorrect_Trailer_FIFO1 += FIFO1_Analyze[3];
    Incorrect_Event_Num_FIFO1 += FIFO1_Analyze[4];

  } // Ends Loops loop
  std::cout << "There have been " << std::dec << Total_Events_FIFO1 << " Events" << std::endl;
  std::cout << "There have been " << std::dec << Incorrect_Event_Num_FIFO1 << " Incorrect Event Numbers " << std::endl;
  std::cout << "There have been " << std::dec << Incorrect_Header_FIFO1 << " Incorrect TBM Headers " << std::endl;
  std::cout << "There have been " << std::dec << Incorrect_Trailer_FIFO1 << " Incorrect TBM Trailers " << std::endl;
  std::cout << " " << std::endl;
}
