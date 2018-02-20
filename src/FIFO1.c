//ROOT include files
#include "TCanvas.h"
#include "TH2.h"
#include "TF1.h"
#include "TH2F.h"
#include "TColor.h"
#include "TStyle.h"
#include "TFile.h"
#include "TLegend.h"
#include "TString.h"
#include "TSystem.h"
#include "TLatex.h"
#include "Error_FIFO.c"

//Used to plot the pixel alive output and determine if each pixel got 1 hit
void Pixel_Alive_Plots(vector<vector<unsigned int>>& DCOL, vector<vector<unsigned int>>& PXL, vector<vector<unsigned int>>& Column, vector<vector<unsigned int>>& Row, vector<vector<unsigned int>>& Spy_Col, vector<vector<unsigned int>>& Spy_Row, vector<vector<unsigned int>>& DDR_Col, vector<vector<unsigned int>>& DDR_PXL, int MaxChannel, int fw_major, int fw_minor, int SRAM){

    TFile * fout;
	string fw_major_str = to_string(fw_major);
	string fw_minor_str = to_string(fw_minor);
    TString outputDir = "v" + fw_major_str + "_" + fw_minor_str + "_log";
    gSystem->mkdir(outputDir, true);
    TString fedname = "Pixel_Alive_v" + fw_major_str + "_" + fw_minor_str + "_fedtestfed";
	if(SRAM) fedname += "_SRAM";
	fout = new TFile(outputDir + "/" + fedname + TString(".root"), "Recreate");

    TH2F *h1[8];	
	TH2F *h2[8];
	TH2F *h3[8];
	TH2F *h4[384];
	TCanvas *Total;
	TCanvas *AllDDR;

	char *h1name = new char[8];
	char *h2name = new char[8];
	char *h3name = new char[8];
	char *h4name = new char[384];
	char *InputName = new char[8];
	char *SpyName = new char[8];
	char *FIFO1Name = new char[8];
	char *DDRName = new char[384];
	char *Totalname = new char[8];
	char *ALLDDRName = new char[48];
	char *ALLDDRTitle = new char[48];
	char *AllDDRTitle = new char[48];

	for(int i = 0; i < 8; i++){
		
		sprintf(h1name, "h1_%d",i);
		sprintf(h2name, "h2_%d",i);
		sprintf(h3name, "h3_%d",i);
		sprintf(InputName, "Row v. Column ROC%d", i+1);
		sprintf(SpyName, "Spy Row v. Spy Col ROC%d", i+1);
		sprintf(FIFO1Name, "PXL v. DCOL FIFO1 ROC%d", i+1);

		h1[i] = new TH2F(h1name,InputName,34,-0.5,33.5,166,-0.5,165.5);
		h1[i]->SetDirectory(fout);
		h1[i]->GetXaxis()->SetTitle("Column");
		h1[i]->GetYaxis()->SetTitle("Row");
		
		h2[i] = new TH2F(h2name,FIFO1Name,26,-0.5,25.5,160,1.5,161.5);
		h2[i]->SetDirectory(fout);
		h2[i]->GetXaxis()->SetTitle("DCOL");
		h2[i]->GetYaxis()->SetTitle("PXL");
		
		h3[i] = new TH2F(h3name,SpyName,34,-0.5,33.5,166,-0.5,165.5);
		h3[i]->SetDirectory(fout);
		h3[i]->GetXaxis()->SetTitle("Spy Col");
		h3[i]->GetYaxis()->SetTitle("Spy Row");
	}
    int j = 0;
	for(int i = 0; i < 384; i++){	
		sprintf(h4name, "h4_%d",i);
        if(i % 8 == 0) j++;
		sprintf(DDRName, "ROC%i CH %i", i%8+1, j);
		if(i % 8 <= 3) h4[i] = new TH2F(h4name,DDRName,26,-0.5,25.5,160,1.5,161.5);
        else h4[i] = new TH2F(h4name,"",26,-0.5,25.5,160,1.5,161.5);
        h4[i]->SetDirectory(fout);
		h4[i]->GetXaxis()->SetTitle("DCOL");
		h4[i]->GetYaxis()->SetTitle("PXL");
        h4[i]->GetYaxis()->SetTitleOffset(1.6);
    }

	for(int i = 0; i < 8; i++){
		for(uint32_t j = 8; j < Column[ i ].size(); j++)
			h1[i]->Fill(Column[ i ][ j ], Row[ i ][ j ]);
		for(uint32_t j = 0; j < DCOL[ i ].size(); j++)
			if(PXL[ i ][ j ] != 0) h2[i]->Fill(DCOL[ i ][ j ], PXL[ i ][ j ]);
		for(uint32_t j = 8; j < Spy_Col[ i ].size(); j++)
			h3[i]->Fill(Spy_Col[ i ][ j ], Spy_Row[ i ][ j ]);
	}
	for(int i = 0; i < 384; i++){
		for(uint32_t j = 0; j < DDR_Col[ i ].size(); j++)
			if(DDR_PXL[ i ][ j ] != 0) h4[i]->Fill(DDR_Col[ i ][ j ], DDR_PXL[ i ][ j ]);
	}
	
    TString TitleTotal = "Pixel Alive Total version " + fw_major_str + "_" + fw_minor_str;
	TString SaveName = "Pixel_Alive_Total_v" + fw_major_str + "_" + fw_minor_str;
    if(SRAM) SaveName += "_SRAM";
	Total = new TCanvas("Total",TitleTotal,200,10,700,500);
	Total->Divide(2,2);
	Total->Print(outputDir + "/" + SaveName + TString(".pdf["));

	for(int i = 0; i < 8; i++){	
		int Max_ddr   = h4[i]->GetMaximum();

        Total->cd(1);
		h1[i]->Draw("COLZ");
		h1[i]->SetMaximum(Max_ddr);
        h1[i]->Write();
        Total->cd(2);
        h3[i]->Draw("COLZ");
		h3[i]->SetMaximum(Max_ddr);
        h3[i]->Write();
		Total->cd(3);
        h2[i]->Draw("COLZ");
		h2[i]->SetMaximum(Max_ddr);
        h2[i]->Write();
		Total->cd(4);
        h4[i]->Draw("COLZ");
		h4[i]->SetMaximum(Max_ddr);
        h4[i]->Write();
		Total->Print(outputDir + "/" + SaveName + TString(".pdf"));
	}
    Total->Print(outputDir + "/" + SaveName + TString(".pdf]"));

    AllDDR = new TCanvas("AllDDR","DDR of all ROCs",200,10,700,500);
	gStyle->SetPadBorderMode(0);
    gStyle->SetFrameBorderMode(0);
    float small = 1e-5;
    AllDDR->Divide(4,2,small,small);
    string AllDDRName = "DDR_all_ROCs_v" + fw_major_str + "_" + fw_minor_str;
	if(SRAM) AllDDRName += "_SRAM";
	AllDDR->Print(outputDir + "/" + AllDDRName + TString(".pdf["));	
    
    for(int j = 0; j < MaxChannel; j++){
		int max0 = h4[j*8 + 0]->GetMaximum();
		int max1 = h4[j*8 + 1]->GetMaximum();
		int max2 = h4[j*8 + 2]->GetMaximum();
		int max3 = h4[j*8 + 3]->GetMaximum();
		int max4 = h4[j*8 + 4]->GetMaximum();
		int max5 = h4[j*8 + 5]->GetMaximum();
		int max6 = h4[j*8 + 6]->GetMaximum();
		int max7 = h4[j*8 + 7]->GetMaximum();
        int max = std::max(std::max(std::max(max0, max1), std::max(max2, max3)), std::max(std::max(max4, max5),std::max(max6, max7)));
		for(int i = 0; i < 8; i++){
            AllDDR->cd((i % 8) + 1);
            if((i % 8) <= 3){
                gPad->SetBottomMargin(small);
                if((i % 4) < 3) gPad->SetRightMargin(small);
                if((i % 4) > 0 && (i % 4) <= 3) gPad->SetLeftMargin(small);
            }
            else{
                gPad->SetTopMargin(small);
                if((i % 4) < 3) gPad->SetRightMargin(small);
                if((i % 4) > 0 && (i % 4) <= 3) gPad->SetLeftMargin(small);
                gPad->SetTickx();

            }
            gStyle->SetOptStat(0);
            //gStyle->SetPalette(100);
            if((i%4) == 3 && SRAM) h4[j*8 + i]->Draw("COLZ");
            else h4[j*8 + i]->Draw("COL");
			h4[j*8 + i]->SetMaximum(max);
			h4[j*8 + i]->Write();
            
            if(i == 3){
                TString cmsText = "CMS";
                int cmsTextFont = 61;
                float cmsTextSize = 0.75;
                TString extraText = "pixel preliminary";
                int extraTextFont = 52;
                float extraOverCmsTextSize  = 0.76;
                float extraTextSize = extraOverCmsTextSize*cmsTextSize;
                float H = AllDDR->GetWh();
                float W = AllDDR->GetWw();
                float l = AllDDR->GetLeftMargin();
                float t = AllDDR->GetTopMargin();
                float r = AllDDR->GetRightMargin();
                float b = AllDDR->GetBottomMargin();
                float relExtraDY = 2;
                float align_ = 23.0;
                float posX_ = 3;
                float posY_ = 177.5;
                TLatex latex;
                latex.SetTextFont(cmsTextFont);
                latex.SetTextSize(cmsTextSize*t);
                latex.SetTextAlign(align_);
                latex.DrawLatex(posX_, posY_, cmsText);
                latex.SetTextFont(extraTextFont);
                latex.SetTextAlign(align_);
                latex.SetTextSize(extraTextSize*t);
                latex.DrawLatex(posX_+3, posY_- 10, extraText);
    	    }	
		
        }
		AllDDR->Print(outputDir + "/" + AllDDRName + TString(".pdf"));	
	}
    AllDDR->Print(outputDir + "/" + AllDDRName + TString(".pdf]"));
}

void Hits_Plots(vector<vector<unsigned int>>& Hits, int MaxChannel, int MaxEventNum, int fw_major, int fw_minor, int SRAM){

	TFile * fout;
	string fw_major_str = to_string(fw_major);
	string fw_minor_str = to_string(fw_minor);
    TString outputDir = "v" + fw_major_str + "_" + fw_minor_str + "_log";
    gSystem->mkdir(outputDir, true);
    TString HitsName = "Hits_v" + fw_major_str + "_" + fw_minor_str;
	if(SRAM) HitsName += "_fedtestfed_SRAM.root";
	else     HitsName += "_fedtestfed.root";
	fout = new TFile(outputDir + "/" + HitsName, "Recreate");

	TH1F *h5[384];	
	TCanvas *Total[384];
	TCanvas *AllDDR;

	char *h5name = new char[384];
	char* Hitsname = new char[384];
    char* glib1 = new char[8];
    char* glib2 = new char[8];
    char* glib3 = new char[8];

	for(int i = 0; i < 384; i++){
		
		sprintf(h5name, "h5_%d",i);
		sprintf(Hitsname, "Hits in ROC %d", i % 8 + 1);

		h5[i] = new TH1F(h5name,Hitsname,41,-0.5,40.5);
		h5[i]->SetDirectory(fout);
		h5[i]->GetXaxis()->SetTitle("Hits");
	}

	for(int i = 0; i < 384; i++){
		for(uint32_t j = 384; j < Hits[i].size(); j++){
			h5[i]->Fill(Hits[ i ][ j ]);
        }
		h5[i]->Write();
	}

	AllDDR = new TCanvas("Hits Total","Hits all ROCs",200,10,700,500);
	AllDDR->Divide(4,2);
	for(int i = 0; i < 8; i++){
        int max_glib1 = h5[i]->GetMaximum();
        int max_glib2 = h5[i + 8*11]->GetMaximum();
        int max_glib3 = h5[i + 8*17]->GetMaximum();
        
        int max = 0;
        if(max_glib1 >= max_glib2 && max_glib1 >= max_glib3) max = max_glib1;
        else if(max_glib2 >= max_glib3) max = max_glib2;
        else max = max_glib3;
       
        //max = max_glib2;
        AllDDR->cd(i + 1);
		h5[i + 8*17]->Draw();
		h5[i + 8*17]->SetLineColor(kRed);
		h5[i + 8*17]->SetMaximum(1.1*max);
		h5[i + 8*17]->Write();
	
        h5[i + 8*0]->Draw("same");
		h5[i + 8*0]->SetLineColor(kBlue);
		h5[i + 8*0]->SetMaximum(1.1*max);
		h5[i + 8*0]->Write();

		h5[i + 8*11]->Draw("same");
		h5[i + 8*11]->SetLineColor(kGreen);
		h5[i + 8*11]->SetMaximum(1.1*max);
		h5[i + 8*11]->Write();

        sprintf(glib1, "h5_%d", i       );
        sprintf(glib2, "h5_%d", i + 8*11);
        sprintf(glib3, "h5_%d", i + 8*17);

        if(i == 3){
            TString cmsText = "CMS";
            int cmsTextFont = 61;
            float cmsTextSize = 0.75;
            TString extraText = "pixel preliminary";
            int extraTextFont = 52;
            float extraOverCmsTextSize  = 0.76;
            float extraTextSize = extraOverCmsTextSize*cmsTextSize;
            float H = AllDDR->GetWh();
            float W = AllDDR->GetWw();
            float l = AllDDR->GetLeftMargin();
            float t = AllDDR->GetTopMargin();
            float r = AllDDR->GetRightMargin();
            float b = AllDDR->GetBottomMargin();
            float relExtraDY = 2;
            float align_ = 23.0;
            float posX_ = 20.0;
            float posY_ = max*0.8;
            TLatex latex;
            latex.SetTextFont(cmsTextFont);
            latex.SetTextSize(cmsTextSize*t);
            latex.SetTextAlign(align_);
            latex.DrawLatex(posX_, posY_, cmsText);
            latex.SetTextFont(extraTextFont);
            latex.SetTextAlign(align_);
            latex.SetTextSize(extraTextSize*t);
            latex.DrawLatex(posX_+7, posY_- max*0.06, extraText);
    	}	

        auto legend = new TLegend(0.6,0.75,0.75,0.9);
        legend->AddEntry(glib1, "GLIB 1", "l");
        legend->AddEntry(glib2, "GLIB 2", "l");
        legend->AddEntry(glib3, "GLIB 3", "l");
        legend->Draw();
    }
    TString HitsRocs = "Hits_in_ROCs_v" + fw_major_str + "_" + fw_minor_str;
	if(SRAM) HitsRocs += "_SRAM.pdf";
	else     HitsRocs += ".pdf";
	AllDDR->SaveAs(outputDir + "/" + HitsRocs);	
}

void Hits_Plots_All(vector<vector<unsigned int>>& Hits, int MaxChannel, int MaxEventNum, int fw_major, int fw_minor, int SRAM){

	TFile * fout;
	string fw_major_str = to_string(fw_major);
	string fw_minor_str = to_string(fw_minor);
    TString outputDir = "v" + fw_major_str + "_" + fw_minor_str + "_log";
    gSystem->mkdir(outputDir, true);
    TString HitsName = "Hits_All_v" + fw_major_str + "_" + fw_minor_str;
	if(SRAM) HitsName += "_fedtestfed_SRAM.root";
	else     HitsName += "_fedtestfed.root";
	fout = new TFile(outputDir + "/" + HitsName, "Recreate");

	TH1F *h_all[384];	
	TCanvas *AllDDR;

	char *h_allname = new char[384];
	char* Hitsname = new char[384];
        char* HitsTitle = new char[48];

        int j = 0;
	for(int i = 0; i < 384; i++){
		
		sprintf(h_allname, "h_all_%d",i);
		if(i % 8 == 0) j++;
		sprintf(Hitsname, "Hits in ROC %d CH %i", i % 8 + 1, j);

		if(i % 8 <= 3) h_all[i] = new TH1F(h_allname,Hitsname,41,-0.5,40.5);
                else           h_all[i] = new TH1F(h_allname,"",41,-0.5,40.5);
		h_all[i]->SetDirectory(fout);
		h_all[i]->GetXaxis()->SetTitle("Hits");
	}

	for(int i = 0; i < 384; i++){
		for(uint32_t j = 384; j < Hits[i].size(); j++){
			h_all[i]->Fill(Hits[ i ][ j ]);
        }
		h_all[i]->Write();
	}

	AllDDR = new TCanvas("Hits Total","Hits all ROCs",200,10,700,500);
	gStyle->SetPadBorderMode(0);
        gStyle->SetFrameBorderMode(0);
        float small = 1e-5;
        AllDDR->Divide(4,2,small,small);
	string AllDDRName = "Hits_all_ROCs_v" + fw_major_str + "_" + fw_minor_str;
	if(SRAM) AllDDRName += "_SRAM";
	AllDDR->Print(outputDir + "/" + AllDDRName + TString(".pdf["));	
    for(int j = 0; j < MaxChannel; j++){
	int max0 = h_all[j*8 + 0]->GetMaximum();
	int max1 = h_all[j*8 + 1]->GetMaximum();
	int max2 = h_all[j*8 + 2]->GetMaximum();
	int max3 = h_all[j*8 + 3]->GetMaximum();
	int max4 = h_all[j*8 + 4]->GetMaximum();
	int max5 = h_all[j*8 + 5]->GetMaximum();
	int max6 = h_all[j*8 + 6]->GetMaximum();
	int max7 = h_all[j*8 + 7]->GetMaximum();
        int max = std::max(std::max(std::max(max0, max1), std::max(max2, max3)), std::max(std::max(max4, max5),std::max(max6, max7)));

        for(int i = 0; i < 8; i++){
		AllDDR->cd((i % 8) + 1);
		if((i % 8) <= 3){
                    gPad->SetBottomMargin(small);
                    if((i % 4) < 3) gPad->SetRightMargin(small);
                    if((i % 4) > 0 && (i % 4) <= 3) gPad->SetLeftMargin(small);
		}
		else{
		    gPad->SetTopMargin(small);
		    if((i % 4) < 3) gPad->SetRightMargin(small);
		    if((i % 4) > 0 && (i % 4) <= 3) gPad->SetLeftMargin(small);
		    gPad->SetTickx();
		
		}

	h_all[j*8 + i]->Draw();
	h_all[j*8 + i]->SetLineColor(kRed);
	h_all[j*8 + i]->SetMaximum(1.1*max);
	h_all[j*8 + i]->Write();
	gStyle->SetOptStat(1111);

        if(i == 3){
            TString cmsText = "CMS";
            int cmsTextFont = 61;
            float cmsTextSize = 0.75;
            TString extraText = "pixel preliminary";
            int extraTextFont = 52;
            float extraOverCmsTextSize  = 0.76;
            float extraTextSize = extraOverCmsTextSize*cmsTextSize;
            float H = AllDDR->GetWh();
            float W = AllDDR->GetWw();
            float l = AllDDR->GetLeftMargin();
            float t = AllDDR->GetTopMargin();
            float r = AllDDR->GetRightMargin();
            float b = AllDDR->GetBottomMargin();
            float relExtraDY = 2;
            float align_ = 23.0;
            float posX_ = 20.0;
            float posY_ = max*0.8;
            TLatex latex;
            latex.SetTextFont(cmsTextFont);
            latex.SetTextSize(cmsTextSize*t);
            latex.SetTextAlign(align_);
            latex.DrawLatex(posX_, posY_, cmsText);
            latex.SetTextFont(extraTextFont);
            latex.SetTextAlign(align_);
            latex.SetTextSize(extraTextSize*t);
            latex.DrawLatex(posX_+7, posY_- max*0.06, extraText);
    	}	

    }
	AllDDR->Print(outputDir + "/" + AllDDRName + TString(".pdf"));
    }
    AllDDR->Print(outputDir + "/" + AllDDRName + TString(".pdf]"));
}

void PKAM_Plots(vector<unsigned int>& PKAM, int MaxEventNum, int fw_major, int fw_minor, int SRAM){

	TFile * fout;
	string fw_major_str = to_string(fw_major);
	string fw_minor_str = to_string(fw_minor);
    string PKAM_str = to_string(PKAMRead());
    TString outputDir = "v" + fw_major_str + "_" + fw_minor_str + "_log";
    gSystem->mkdir(outputDir, true);
    TString PKAMName = "PKAM_v" + fw_major_str + "_" + fw_minor_str;
	if(SRAM) PKAMName += "_SRAM";
    PKAMName += "_" + PKAM_str + "clks.root";
	fout = new TFile(outputDir + "/" + PKAMName, "Recreate");

	TH1F *h8;	
	TCanvas *AllDDR;
    TString PKAMTotal = "Total Number of PKAMs " + PKAM_str + "clks";

	h8 = new TH1F("h8",PKAMTotal,49,-0.5,48.5);
	h8->SetDirectory(fout);
	h8->GetXaxis()->SetTitle("PKAM");

	for(uint32_t j = 0; j < PKAM.size(); j++){
        h8->Fill(PKAM[j]);
    }
	h8->Write();

	AllDDR = new TCanvas("PKAM Total",PKAMTotal,200,10,700,500);
	AllDDR->cd();
	h8->Draw();
	h8->SetLineColor(kBlue);
	h8->Write();

    TString PKAMRocs = "PKAM_v" + fw_major_str + "_" + fw_minor_str;
	if(SRAM) PKAMRocs += "_SRAM";
    PKAMRocs += "_" + PKAM_str + "clks.pdf";
	AllDDR->SaveAs(outputDir + "/" + PKAMRocs);	
}

void Test_Hits_Full_FIFO1(int loops_input, int col_start, int col_input, int row_start, int row_input, int adc_start, int adc_input, int choice_input, int Num_Hits, int SRAM)
{

  using namespace std;
  using namespace uhal;
  const char* cHWFile;
  const char* cGLIBHWFile;
  std::string filenameGLIB ("settings/GLIBDescription.xml");
  cGLIBHWFile = filenameGLIB.c_str();
  if (choice_input == 0) {
    std::string filename ("settings/HWDescription_Pixel_Alive.xml");
    cHWFile = filename.c_str();
  }
  else if (choice_input == 1) {
    std::string filename ("settings/HWDescription_Stack.xml");
    cHWFile = filename.c_str();
  }
  else if (choice_input == 2) {
    std::string filename ("settings/HWDescription_Stack2.xml");
    cHWFile = filename.c_str();
  }

  string ROCSA[ 8 ] = {"CHA_ROC0", "CHA_ROC1", "CHA_ROC2", "CHA_ROC3", "CHA_ROC4", "CHA_ROC5", "CHA_ROC6", "CHA_ROC7"};
  string ROCSB[ 8 ] = {"CHB_ROC0", "CHB_ROC1", "CHB_ROC2", "CHB_ROC3", "CHB_ROC4", "CHB_ROC5", "CHB_ROC6", "CHB_ROC7"};

  uhal::setLogLevelTo(uhal::Debug());

  // instantiate System Controller
  SystemController cSystemController;
  Amc13Controller  cAmc13Controller;

  // initialize map of settings so I can know the proper number of acquisitions and TBMs
  cSystemController.InitializeSettings(cHWFile, cout);

  // initialize HWdescription from XML, beware, settings have to be read first
  cAmc13Controller.InitializeAmc13( cHWFile, cout );
  //cSystemController.InitializeHw(cGLIBHWFile, cout);
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
  //cSystemController.ConfigureGLIBHw(cout );
  cSystemController.ConfigureHw(cout );

  auto& cFED = cSystemController.fPixFEDVector[0];
  cSystemController.fFEDInterface->getBoardInfo(cFED);
  cSystemController.fFEDInterface->findPhases(cFED);
  cSystemController.fFEDInterface->Start (cFED);
 
  auto& cFED1 = cSystemController.fPixFEDVector[1];
  auto& cFED2 = cSystemController.fPixFEDVector[2];
  auto& cFED3 = cSystemController.fPixFEDVector[3];

  std::vector<unsigned int> FIFO;
  std::vector<std::vector<unsigned int>> FIFO1;
  std::vector<unsigned int> FIFO1_marker;
  std::vector<unsigned int> Error_FIFO;
  std::vector<unsigned int> DDR;
  
  std::vector<std::vector<unsigned int > > Column (8, vector<unsigned int>(8));
  std::vector<std::vector<unsigned int > > Row    (8, vector<unsigned int>(8));
  std::vector<std::vector<unsigned int > > Spy_Col(8, vector<unsigned int>(8));
  std::vector<std::vector<unsigned int > > Spy_Row(8, vector<unsigned int>(8));
  std::vector<std::vector<unsigned int > > DCOL   (8, vector<unsigned int>(8));
  std::vector<std::vector<unsigned int > > PXL    (8, vector<unsigned int>(8));
  std::vector<std::vector<unsigned int > > DDR_Col(384, vector<unsigned int>(384));
  std::vector<std::vector<unsigned int > > DDR_PXL(384, vector<unsigned int>(384));
  std::vector<std::vector<unsigned int > > Hits   (384, vector<unsigned int>(384));
  std::vector<unsigned int > DDR_size;
  std::vector<unsigned int > PKAM;
  int Compare_DCOL = 0;
  int Compare_PXL = 0;
  int Max_Array = 0;
  int Total_Events = 0;
  int Incorrect_Header = 0;
  int Incorrect_Trailer = 0;
  int Incorrect_Event_Num = 0;
  int TotalTimeout  = 0;
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
  int Event_Num = 0;
  int MaxChannel = 0;
  int MaxEventNum = 0;

  //std::fill(FIFO1_marker.begin(), FIFO1_marker.end(), 0);
  //std::fill(FIFO.begin(), FIFO.end(), 0);
  //std::fill(DDR.begin(), DDR.end(), 0);

  //for(int i = 0; i < 384; i++){
  //  std::fill(DDR_Col[i].begin(), DDR_Col[i].end(), 0); 
  //  std::fill(DDR_PXL[i].begin(), DDR_PXL[i].end(), 0); 
  //}

  cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
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
  int Roc_tot[384] = {};
  std::vector<unsigned int>  Evn_timeout;

  std::string index[8] = {"0","1","2","3","4","5","6","7"};
  std::string fiber ("Fiber_");
  //cSystemController.fFEDInterface->WriteBoardReg(cFED, "be_ctrl_regs.disable_BE", 1);

  for (int l = 0; l < Loops; l++) 
  {
    int Hit_infoA = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);
    int Hit_infoB = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);

    int row = 0;
    int col = 0;
    int dcol = 0;
    int adc = 0;
    int col_L1 = 0;
    int row_L1 = 0;

    //Start of pixel alive loop the row and column are transmitted into buffer that change the values to the allowed ones
    for (row = row_start; row < max_row; row++) {

      for (col = col_start; col < max_col; col++) {

        if(col % 2 == 0) dcol = col/2;

        //cout << "Starting row " << std::dec << row << ", column " << std::dec << col << endl;

        for (adc = adc_start; adc < 8/*max_adc*/; adc+=8) {

          int Hit_infoA = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);
          int Hit_infoB = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);

          if(SRAM == 0){
            for (int j = 0; j < 8; j++) {

          	  for (int i = 0; i < 8; i++) {
          	    //Writing the values of PXL/DCOL/ADC into the ROC register to be transmitted to the FED
		    

          	    string ROCA = fiber + index[i] + "." + ROCSA[j];
          	    string ROCB = fiber + index[i] + "." + ROCSB[j];

                Hit_infoA_buff = Illegal_Col(dcol);
                Hit_infoB_buff = Illegal_Col(dcol);
                new_row = New_Row(row, col);
                col_L1 = dcol;
                row_L1 = L1_row(row, col);

                //Hit values if emulating L1/L2
          	    //Hit_infoA_new = ((row_L1) << 16) | (col_L1 << 8) | (Hit_infoA & 0xFF);
          	    //Hit_infoB_new = ((row_L1) << 16) | (col_L1 << 8) | (Hit_infoB & 0xFF);

          	    Hit_infoA_new = (new_row << 16) | (Hit_infoA_buff << 8) | (Hit_infoA & 0xFF);
          	    Hit_infoB_new = (new_row << 16) | (Hit_infoB_buff << 8) | (Hit_infoB & 0xFF);

                cSystemController.fFEDInterface->WriteBoardReg(cFED1, ROCA, Hit_infoA_new);
                cSystemController.fFEDInterface->WriteBoardReg(cFED1, ROCB, Hit_infoB_new);

                cSystemController.fFEDInterface->WriteBoardReg(cFED2, ROCA, Hit_infoA_new);
                cSystemController.fFEDInterface->WriteBoardReg(cFED2, ROCB, Hit_infoB_new);
   
          	    Hit_infoA_new = (new_row << 16) | (Hit_infoA_buff << 8) | (Hit_infoA & 0xFF);
          	    Hit_infoB_new = (new_row << 16) | (Hit_infoB_buff << 8) | (Hit_infoB & 0xFF);
                cSystemController.fFEDInterface->WriteBoardReg(cFED3, ROCA, Hit_infoA_new);
                cSystemController.fFEDInterface->WriteBoardReg(cFED3, ROCB, Hit_infoB_new);
       
	
          	  }
          	  Hit_infoA = Hit_infoA + 0x00000001;
          	  Hit_infoB = Hit_infoB + 0x00000001;

          	  adc_loops++;
            }

            for(int j = 0; j < 8; j++){
              for(int i = 0; i < Num_Hits; i++){
                //L1 Emulation
                //Column[j].push_back(col);
                //Row[j].push_back(row);
                //FPIX Emulation
                Column[j].push_back(Hit_infoA_buff);
                Row[j].push_back(new_row);
              }
            }
          }//check for SRAM mode


            if(l == 0 && row == 0 && col == 0 && adc == 0){
	            cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
	            sleep(8);
	            cAmc13Controller.fAmc13Interface->BurstL1A();
            }
            else cAmc13Controller.fAmc13Interface->BurstL1A();
                sleep(0.050);

	        FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
	        FIFO1 = cSystemController.fFEDInterface->readFIFO1_vec(cFED);
            //cSystemController.fFEDInterface->readErrorFIFO(cFED, true);
            //FIFO1_marker = cSystemController.fFEDInterface->readFIFO1Marker(cFED);
            DDR = cSystemController.fFEDInterface->ReadData(cFED, 0 );
	  
            int Roc_num;
            int adc_adjust;
            int row_adjust;
            int col_adjust;
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
                if((((FIFO[0] >> 4) & 0xF) == 0x1) 
                || (((FIFO[0] >> 4) & 0xF) == 0x2) 
                || (((FIFO[0] >> 4) & 0xF) == 0x3) 
                || (((FIFO[0] >> 4) & 0xF) == 0x4) 
                || (((FIFO[0] >> 4) & 0xF) == 0x5) 
                || (((FIFO[0] >> 4) & 0xF) == 0x6))
                break;
			    int SCOL = 0, SROW = 0;
		        if(((FIFO[i] >> 4) & 0xF) == 0x7)
		        	Spy_ROC++;
		        if ((((FIFO[i] >> 4) & 0xF) == 0x2)){
				//L1 Emulation
		        	SCOL = ((FIFO[i - 1] & 0xe) << 1) | ((FIFO[i] & 0xc) >> 2);
		        	//FPIX Emulation
                    SCOL = ((FIFO[i - 1] & 0xf) << 2) | ((FIFO[i] & 0xc) >> 2);
		        	Spy_Col[Spy_ROC - 1].push_back(SCOL) ;
		        }
		        if (((FIFO[i] >> 4) & 0xF) == 0x4){
		        	//L1 Emulation
                    SROW = ((FIFO[i-2] & 0x3) << 6) | ((FIFO[i-1] & 0xe) << 2) | ((FIFO[i] & 0xe) >> 1);
		        	//FPIX Emulation
                    SROW = ((FIFO[i-2] & 0x3) << 7) | ((FIFO[i-1] & 0xf) << 3) | ((FIFO[i] & 0xe) >> 1);
		        	Spy_Row[Spy_ROC - 1].push_back(SROW);
	          	}
            }

	          for ( uint32_t i = 0; i < FIFO1[0].size(); i++){
              //cout << std::hex << FIFO1_marker[i] << ": " << FIFO1[i] << endl;
              if (FIFO1[0][i] == 0x0) break;

              if (FIFO1[1][i] == 8){
                //header
                //cout << "Event Num = " << std::dec << (FIFO1[i] && 0xff) << endl;
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

              if (FIFO1[1][i] == 0xc){
                //ROC header
                Roc_num = ((FIFO1[0][i] >> 21) & 0x1f);
              }

              if (FIFO1[1][i] == 0x1) {
 	              Pixel++;

	              if(Roc_num >= 1 && Roc_num <= 8){
                      if( col % 51 == 1 && Pixel % Num_Hits == 0) cout << "row = " << row << ", loop = " << l << endl;
                      DCOL[Roc_num - 1].push_back((FIFO1[0][i] >> 16) & 0x1F);
                      PXL[Roc_num - 1].push_back((FIFO1[0][i] >> 8) & 0xFF);
		              Compare_DCOL = DCOL_Encode(Hit_infoA_buff);
		              Compare_PXL = PXL_Encode(new_row);

                   }
	               adc_adjust = adc + Roc_num - 1;
	               if(SRAM == 0){
                    if (((FIFO1[0][i] & 0xff) != adc_adjust && (FIFO1[0][i] & 0xff) != Current_Event_Num) && Pixel % Num_Hits == 0) {
                   		  cout << "ADC error! Correct ADC = " << std::dec << adc_adjust << endl;
                   		  cout << "ADC = " << std::dec << (FIFO1[0][i] & 0xff) << endl;
                   		}
	           		     	
                   		if ( ((FIFO1[0][i] >> 8) & 0xFF) != Compare_PXL) {
                   		  cout << "PXL Error, PXL input = " << std::dec << Compare_PXL << " PXL output = " << ((FIFO1[0][i] >> 8) & 0xFF) << endl;
                   		}
                   		if ( ((FIFO1[0][i] >> 16) & 0x1F) != Compare_DCOL) {
                   		  cout << "DCOL  Error, DCOL input = " << std::dec << Compare_DCOL << " DCOL ouput = " << ((FIFO1[0][i] >> 16) & 0x1F) << endl;
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
                cout << "There was a NTP" << endl;
              }

              if((FIFO1[0][i] & 0x40) >> 6){
                cout << "There was a TBM Reset" << endl;
              }

              if((FIFO1[0][i] & 0x20) >> 5){
                cout << "There was a ROC Reset" << endl;
              }
		          
              if((FIFO1[0][i] & 0x10) >> 4){
                cout << "Stack is full!" << endl;
              }
		          
              if(((FIFO1[0][i] & 0x40000) >> 18)){
                cout << "There was a PKAM Reset at Event " << std::dec << Current_Event_Num << endl;
              }
		          
              Stack = ((FIFO1[0][i] >> 12) & 0x3F);
	        
              if(Stack != 0){
                cout << "The stack count is " << std::dec << Stack << endl;
      	      }
            } 
            if(Error_Loop == 1 && Error_Count != 0){
              cout << "Event = " << std::dec << Current_Event_Num << " and Loop = " << std::dec << l << endl;
              cout << "Total Number of Errors = " <<std::dec << Error_Count << endl;
              int Error = 0;
   
              for(Error = Start_Event; Error <= END_Event; Error++){
                cout << "FIFO1[" << std::dec << Error << "] = " << std::hex << FIFO1[0][Error] << endl;
              }
   
              //for(Error = Start_Event; Error <= END_Event; Error++){
                //cout << "FIFO_CHB[" << std::dec << Error << "] = " << std::hex << FIFO_CHB[Error] << endl;
              //}
              Error_Loop = 0;	
            }

          }// Check FIFO1 for Information

	        bool skip_head = false;
	        int tot_hits[384] = {};
            int TotalPKAM = 0;
            Event_Num++;
	        for ( uint32_t i = 0; i < DDR.size(); i++){
			ROC_DDR = (((DDR[i] >> 21) & 0x1F));
			if((DDR[i] >> 28) == 0x5 && skip_head == false && i == 0){ skip_head = true; MaxEventNum = (DDR[i] & 0xFFFFFF); continue;}
			if(skip_head == true) {skip_head = false; continue;}
			if(ROC_DDR == 29){
				Evn_timeout.push_back(DDR[i]);
				cout << "There was a timeout" << endl;
				TotalTimeout++;
			}
            if(ROC_DDR == 30 && ((DDR[i] >> 9) & 0x1) == 1) TotalPKAM++;
			if((DDR[i] >> 28) == 0xa && (((DDR[i] >> 21) & 0x1F) == 0x0)){
				break;
			} 
			if(ROC_DDR > 8 || ROC_DDR == 0) continue;
			
			int Channel = (DDR[i] >> 26) & 0x3F;	
			bool do_print = true;
			if ((Channel >= 1) || (Channel <= 48)){
				do_print = false;
				Roc_tot[((Channel - 1)*8) + (ROC_DDR - 1)] += 1;
				DDR_Col[((Channel - 1)*8) + (ROC_DDR - 1)].push_back((DDR[i] >> 16) & 0x1F);
				DDR_PXL[((Channel - 1)*8) + (ROC_DDR - 1)].push_back((DDR[i] >> 8) & 0xFF);
				tot_hits[((Channel - 1)*8) + (ROC_DDR - 1)] += 1;
					
				if(MaxChannel < Channel) MaxChannel = Channel;
                    if(SRAM == 0){
			      		if((((DDR[i] >> 8) & 0xFF) <= 1)){
			        			cout << "DDR[" << std::dec << i << "]: " << std::bitset<32>(DDR[i]) << endl;
			        		}
			        		if(((DDR[i] >> 8) & 0xFF) >= 162)
			        			cout << "DDR[" << std::dec << i << "]: " << std::bitset<32>(DDR[i]) << endl;
			        		if(DDR_PXL[((Channel - 1)*8) + (ROC_DDR - 1)].back() <= 1)
			        			cout << "DDR_pxl_vec: " << std::bitset<32>(DDR_PXL[((Channel - 1)*8) + (ROC_DDR - 1)].back()) << endl;
			        		if(DDR_PXL[((Channel - 1)*8) + (ROC_DDR - 1)].back() >= 162)
			        			cout << "DDR_pxl_vec: " << std::bitset<32>(DDR_PXL[((Channel - 1)*8) + (ROC_DDR - 1)].back()) << endl;
                    }
                }
                else if( (do_print == true) && (((DDR[i] >> 26) & 0x3F) == 0x1)){
                    for ( uint32_t i = 0; i < DDR.size(); i++)
                        cout << "DDR[" << std::dec << i << "]: " << std::bitset<32>(DDR[i]) << endl;
                }
            }// Check DDR for information
            PKAM.push_back(TotalPKAM);
            DDROddEvent(DDR, Event_Num);

            for(int i = 0; i < 384; i++){
                Hits[i].push_back(tot_hits[i]);
            }
        } // Ends adc loop

      col_loops++;
      adc_loops = 0;

      } // Ends column loop

      row_loops++;
      col_loops = 0;

    } // Ends row loop

    row_loops = 0;
  
  } // Ends Loops loop

  int fw_major = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.user_iphc_fw_id.fw_ver_nb");
  int fw_minor = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.user_iphc_fw_id.archi_ver_nb");
  Pixel_Alive_Plots( DCOL, PXL, Column, Row, Spy_Col, Spy_Row, DDR_Col, DDR_PXL, MaxChannel, fw_major, fw_minor, SRAM);
  Hits_Plots(Hits, MaxChannel, MaxEventNum, fw_major, fw_minor, SRAM);
  Hits_Plots_All(Hits, MaxChannel, MaxEventNum, fw_major, fw_minor, SRAM);
  cout << PKAMRead() << endl;
  PKAM_Plots(PKAM, MaxEventNum, fw_major, fw_minor, SRAM);
  int Channel = 0;
  for(int i = 0; i < 384; i++){
    if((i % 8) == 0) Channel++;
	cout << "Total Hits in Channel " << std::dec << Channel <<" ROC " << (i % 8) + 1 << ": " << Roc_tot[i] << endl;
  }

  for(int i = 0; i < Evn_timeout.size(); i++)
	cout << "Timeout at Events: " << bitset<32>(Evn_timeout[i]) << endl;

 if (choice_input == 0) {
    cout << "There have been " << std::dec << Total_Events << " Events" << endl;
    cout << "There have been " << std::dec << Incorrect_Event_Num << " Incorrect Event Numbers " << endl;
    cout << "There have been " << std::dec << TotalTimeout / 2 << " Timeouts" << endl;
    cout << "There have been " << std::dec << Incorrect_Header << " Incorrect TBM Headers " << endl;
    cout << "There have been " << std::dec << Incorrect_Trailer << " Incorrect TBM Trailers " << endl;
    cout << "There have been " << std::dec << Incorrect_ROC << " incorrect Number of ROCs " << endl;
    cout << "There have been " << std::dec << Incorrect_Pixel << " incorrect Number of Pixels " << endl;
    cout << "There have been " << std::dec << Incorrect_pix_info << " incorrect Number of pix hit info " << endl;
    cout << "There have been " << std::dec << Incorrect_adc << " incorrect Number of adc values " << endl;
    cout << "There have been " << std::dec << Incorrect_col << " incorrect Number of columns " << endl;
    cout << "There have been " << std::dec << Incorrect_row << " incorrect Number of rows " << endl;
  }
  else if (choice_input > 0) {
    cout << "There have been " << std::dec << Total_Events << " Events" << endl;
    cout << "There have been " << std::dec << Incorrect_Event_Num << " Incorrect Event Numbers " << endl;
    cout << "There have been " << std::dec << Incorrect_Header << " Incorrect TBM Headers " << endl;
    cout << "There have been " << std::dec << Incorrect_Trailer << " Incorrect TBM Trailers " << endl; 
    cout << "There have been " << std::dec << Incorrect_ROC << " incorrect Number of ROCs " << endl;
    cout << "There have been " << std::dec << Incorrect_Pixel << " incorrect Number of Pixels " << endl;
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
    std::string filename ("settings/HWDescription_PKAM.xml");
    cHWFile = filename.c_str();
  }
  else if (choice_input == 1) {
    std::string filename ("settings/HWDescription_Resets.xml");
    cHWFile = filename.c_str();
  }

  uhal::setLogLevelTo(uhal::Debug());

  // instantiate System Controller
  SystemController cSystemController;
  Amc13Controller  cAmc13Controller;

  // initialize map of settings so I can know the proper number of acquisitions and TBMs
  cSystemController.InitializeSettings(cHWFile, cout);

  // initialize HWdescription from XML, beware, settings have to be read first
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

  // configure the HW
  cAmc13Controller.ConfigureAmc13( cout );
  cSystemController.ConfigureHw(cout );

  //get the board info of all boards and start the acquistion
  //for (auto& cFED : cSystemController.fPixFEDVector)
  //{
      auto& cFED = cSystemController.fPixFEDVector[0];
      cSystemController.fFEDInterface->getBoardInfo(cFED);
      cSystemController.fFEDInterface->findPhases(cFED);
      cSystemController.fFEDInterface->Start(cFED);
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

  cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
  cAmc13Controller.fAmc13Interface->SendEC0();
  cAmc13Controller.fAmc13Interface->EnableBGO(0);
  sleep(0.001);
  cAmc13Controller.fAmc13Interface->DisableBGO(0);

  for (int l = 0; l < loops_input; l++) 
  {
    if( l == 1 && choice_input == 1){
	cAmc13Controller.fAmc13Interface->EnableBGO(1);
	sleep(0.001);
  	cAmc13Controller.fAmc13Interface->DisableBGO(1);
    }

    if(l == 0){
      cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
      sleep(4);
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
  cout << "There have been " << std::dec << Total_Events_FIFO1 << " Events" << endl;
  cout << "There have been " << std::dec << Incorrect_Event_Num_FIFO1 << " Incorrect Event Numbers " << endl;
  cout << "There have been " << std::dec << Incorrect_Header_FIFO1 << " Incorrect TBM Headers " << endl;
  cout << "There have been " << std::dec << Incorrect_Trailer_FIFO1 << " Incorrect TBM Trailers " << endl;
  cout << " " << endl;
}
