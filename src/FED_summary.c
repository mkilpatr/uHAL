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
#include <stdio.h>

#include "TCanvas.h"
#include "TH2.h"
#include "TF1.h"
#include "TH2F.h"
#include "THStack.h"
#include "TAttLine.h"
#include "TColor.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TFrame.h"
#include "TGaxis.h"
#include "TLatex.h"
#include "TString.h"
#include "TAttLine.h"
#include "TSystem.h"

std::vector<unsigned int> TextLoop(string TextFile){
    std::vector<unsigned int> StoreHits;
    ifstream in(TextFile);

    if(!in){
        cout << "Cannot open input file." << endl;
    }

    char str[255];
    while(in.getline(str, 255)){
        StoreHits.push_back(strtol(str, NULL, 16));
    }

    in.close();

    return StoreHits;

}

void FED_Summary_Plots(vector<unsigned int>& FED_ENE, vector<unsigned int>& FED_ETOE, float Run){

    TFile * fout;
    TString outputDr = "FED_Summary";
    gSystem->mkdir(outputDr, true);
    string RunNum = to_string(Run);
    TString FEDName = "FED_Summary" + RunNum + ".root";
    fout = new TFile(outputDr + "/" + FEDName, "Recreate");

    TH1F *h6;
    TH1F *h7;  
    TCanvas *Total;
    TCanvas *ENE;
    TCanvas *ETOE;
    TString FEDRocs = "FED_Summary";

    TString ENEName = "FED ENE Run " + RunNum;
    TString ETOEName = "FED ETOE Run " + RunNum;

    float xMax = 0.;
    float xMin = 9999999.;
    for (unsigned int i = 0; i < FED_ENE.size(); i++){
        if (FED_ENE[i] < xMin) xMin = FED_ENE[i];
        if (FED_ENE[i] > xMax) xMax = FED_ENE[i];
    }

    h6 = new TH1F("h6",ENEName,1000,xMin,xMax);
    h6->SetDirectory(fout);
    h6->GetXaxis()->SetTitle("ENEs");

    xMax = 0.;
    xMin = 9999999.;
    for (unsigned int i = 0; i < FED_ETOE.size(); i++){
        if (FED_ETOE[i] < xMin) xMin = FED_ETOE[i];
        if (FED_ETOE[i] > xMax) xMax = FED_ETOE[i];
    }

    h7 = new TH1F("h7",ETOEName,1000,xMin,xMax);
    h7->SetDirectory(fout);
    h7->GetXaxis()->SetTitle("ETOEs");

    for(uint32_t j = 0; j < FED_ENE.size(); j++) h6->Fill(FED_ENE[ j ]);
    for(uint32_t j = 0; j < FED_ETOE.size(); j++) h7->Fill(FED_ETOE[ j ]);

    ENE = new TCanvas("FED ENE",ENEName,200,10,700,500);
    ENE->cd();
    gPad->SetLogy();
    gPad->SetLogx();
    h6->Draw();
    h6->SetLineColor(kBlue);
    h6->Write();

    ETOE = new TCanvas("FED ETOE",ETOEName,200,10,700,500);
    ETOE->cd();
    gPad->SetLogy();
    gPad->SetLogx();
    h7->Draw();
    h7->SetLineColor(kRed);
    h7->Write();
    
    //gStyle->SetOptStat(1110);
    //auto legend = new TLegend(0.80,0.65,0.95,0.80);
    //legend->AddEntry("h6", "FED ENEs", "l");
    //legend->AddEntry("h7", "FED ETOS", "l");
    //legend->Draw();
    
    FEDRocs = "FED_Summary_ENEs_Run_" + RunNum + ".pdf";
    ENE->SaveAs(outputDr + "/" + FEDRocs);   
    FEDRocs = "FED_Summary_ETOEs_Run_" + RunNum + ".pdf";
    ETOE->SaveAs(outputDr + "/" + FEDRocs);   
}

