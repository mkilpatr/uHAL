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

float ferolTheory( int Hit ){
    float Theory = 1600*100/((Hit)*8*48);
    return Theory;
}

void FEROL_Data_Throughputs(int fw_major, int fw_minor){
    TFile * fout;
    string fw_major_str = to_string(fw_major);
    string fw_minor_str = to_string(fw_minor);
    string DataName_Str;
    TString outputDir = "v" + fw_major_str + "_" + fw_minor_str + "_log";
    gSystem->mkdir(outputDir, true);
    DataName_Str = "FEROL_Data_v" + fw_major_str + "_" + fw_minor_str + ".root";
    const char* DataName = DataName_Str.c_str();
    fout = new TFile(outputDir + "/" + DataName, "Recreate");
   
    string DataTitle_str = "FEROL Data Throughput";
    const char * DataTitle = DataTitle_str.c_str();
    float Data_5G[6]    = {1.257,   2.488,   3.732,   3.954,  3.95,   3.953};
    float Trigger_5G[8] = {100,     100,     100,     79.677, 53.209, 39.98};
    float Data_x[6]     = {1,       2,       3,       4,      6,      8};
    //Data from FED v15.1
    float Data_SRAM[8]   = {1.254,   2.487,   3.741,   4.017,  4.195,  4.330,  4.412,  4.444};
    float Trigger_SRAM[8]= {99.975,  99.783,  99.244,  81.236, 67.973, 58.821, 52.664, 50.966};
    float Data_x_10G[8]  = {1,       2,       3,       4,      5,      6,      7,      8};
    float Data_10G[8]    = {1.255,   2.488,   3.712,   4.016,  4.196,  4.329,  4.417,  4.462};
    float Trigger_10G[8] = {99.873,  99.959,  99.429,  81.076, 67.846, 58.630, 52.659, 49.826};
    //Data from FED v13.19
    float Data_SRAM_v13[8]   = {1.147,   2.335,   3.542,   3.985,  4.145,  4.314,  4.403,  4.459};
    float Trigger_SRAM_v13[8]= {99.119,  100,     99.909,  84.439, 70.476, 60.642, 53.033, 49.637};
    float Data_x_10G_v13[8]  = {1,       2,       3,       4,      5,      6,      7,      8};
    float Data_10G_v13[8]    = {1.263,   2.487,   3.694,   4.022,  4.203,  4.285,  4.427,  4.484};
    float Trigger_10G_v13[8] = {99.671,  99.823,  99.94,   81.282, 67.987, 58.199, 51.998, 48.286};
    //Data from FED v15.5 with periodic trigger
    float Data_periodic[8]   = {1.251,   2.485,   3.732,   4.013,  4.194,  4.331,  4.416,  4.484};
    float Trigger_periodic[8]= {100.,    100.,    100.,    81.195, 67.903, 58.525, 51.853, 48.01};
    float Data_x_v15_5[8]    = {1,       2,       3,       4,      5,      6,      7,      8};
    float fine_x[27]         = {0.125,   0.25,    0.375,   0.5,    0.625,  0.75,   0.875,  1.,
                                1.125,   1.25,    1.375,   1.5,    1.625,  1.75,   1.875,  2.,
                                2.125,   2.25,    2.375,   2.5,    2.625,  2.75,   2.875,  3.,
                                3.125,   3.25,    3.375};
    float Trigger_fine[27]   = {100.,    100.,    100.,    100.,   100.,   100.,   100.,   100.,
                                100.,    100.,    100.,    100.,   100.,   100.,   100.,   100.,
                                100.,    100.,    100.,    100.,   100.,   100.,   100.,   100.,
                                98.795,  96.896,  95.047};
    float Data_fine[27]      = {0.128,   0.231,   0.333,   0.436,  0.641,  0.846,  1.052,  1.257,
                                1.359,   1.462,   1.564,   1.667,  1.872,  2.078,  2.283,  2.488,
                                2.591,   2.693,   2.796,   2.898,  3.104,  3.322,  3.527,  3.731,
                                3.783,   3.809,   3.834};
    float Data_v15_8_x[14]   = {1,              2,              3,              4,              5,              6,              7,
                                8,              9,              10,             11,             12,             13,             14};
    float Trig_v15_8[14]     = {99.899,         99.716,         99.764,         99.725,         83.024,         69.206,         59.812,
                                54.636,         52.963,         51.728,         51.057,         50.236,         49.7,           49.567};
    float Data_v15_8[14]     = {1.243,          2.481,          3.721,          4.966,          5.12,           5.127,          5.125,
                                5.129,          5.127,          5.127,          5.129,          5.130,          5.130,          5.133};
    float Trig_v15_9[14]     = {100.687,        100.710,        100.024,        99.768,         82.457,         69.109,
                                59.949,         54.997,         52.602,         52.048,         51.008,         50.366,
                                49.930,         49.613};
    float Data_v15_9[14]     = {1.258,          2.460,          3.715,          4.980,          5.101,          5.116,
                                5.115,          5.121,          5.119,          5.119,          5.124,          5.123,                                     5.122,          5.121};
    float Trig_v15_10[14]    = {99.492,         99.385,         100.019,        99.875,         81.984,         68.780,         59.538,
                                54.516,         52.219,         51.831,         50.902,         50.445,         50.028,         49.107};
    float Data_v15_10[14]    = {1.255,          2.499,          3.735,          4.944,          5.078,          5.092,          5.095,
                                5.102,          5.102,          5.103,          5.102,          5.104,          5.104,          5.106};
    float Trig_Theory_x[14]  = {1,              2,              3,              4,              5,              6,              7,
                                8,              9,              10,             11,             12,             13,             14};
    float Trig_Theory[14]    = {ferolTheory(1), ferolTheory(2), ferolTheory(3), ferolTheory(4), ferolTheory(5), ferolTheory(6), ferolTheory(7),
                                ferolTheory(8), ferolTheory(9), ferolTheory(10),ferolTheory(11),ferolTheory(12),ferolTheory(13),ferolTheory(14)};
    float scaleFactor = 0.05;
    float data_10_per_x[1] = {10.};
    float data_10_per[1]   = {4.564};
    float trig_10_per[1]   = {scaleFactor*45.429};
    float data_10_rand_x[1] = {10.};
    float data_10_rand[1]   = {4.535};
    float trig_10_rand[1]   = {scaleFactor*45.856};
    float data_12_per_x[1] = {12.};
    float data_12_per[1]   = {4.579};
    float trig_12_per[1]   = {scaleFactor*44.913};
    float data_12_rand_x[1] = {12.};
    float data_12_rand[1]   = {4.563};
    float trig_12_rand[1]   = {scaleFactor*44.938};
    float data_14_per_x[1] = {14.};
    float data_14_per[1]   = {4.593};
    float trig_14_per[1]   = {scaleFactor*44.187};
    float data_14_rand_x[1] = {14.};
    float data_14_rand[1]   = {4.583};
    float trig_14_rand[1]   = {scaleFactor*44.252};
    float SRAM_PU70_x[1]   = {1.99};
    float SRAM_PU70_Data[1]= {2.43};
    float SRAM_PU70_Trig[1] ={100};
    float SRAM_PU130_x[1]   = {3.67};
    float SRAM_PU130_Data[1] ={3.919};
    float SRAM_PU130_Trig[1] ={90.002};

    TCanvas *Total = new TCanvas("Data Total", DataTitle, 200, 10, 700, 500);
    TPad *pad = new TPad("pad","",0,0,1,1);
    pad->SetGrid();
    pad->Draw();
    pad->cd();

    TH1F *hr = Total->DrawFrame(0,0,14,6);
    hr->SetXTitle("Emulated Hits (ROC/Channel)");
    hr->SetYTitle("Data Throughput (Gb/s)");
    hr->SetTitle(DataTitle);
    hr->SetTitleFont(42);
    hr->SetLabelFont(42);



    //TString cmsText = "CMS";
    //int cmsTextFont = 61;
    //float cmsTextSize = 0.75;
    //TString extraText = "pixel preliminary";
    //int extraTextFont = 52;
    //float extraOverCmsTextSize  = 0.76;
    //float extraTextSize = extraOverCmsTextSize*cmsTextSize;
    //float H = pad->GetWh();
    //float W = pad->GetWw();
    //float l = pad->GetLeftMargin();
    //float t = pad->GetTopMargin();
    //float r = pad->GetRightMargin();
    //float b = pad->GetBottomMargin();
    //float relExtraDY = 1.2;
    //float align_ = 23.0;
    //float posX_ = 5.0;
    //float posY_ = 5.0;
    //TLatex latex;
    //latex.SetTextFont(cmsTextFont);
    //latex.SetTextSize(cmsTextSize*t);
    //latex.SetTextAlign(align_);
    //latex.DrawLatex(posX_, posY_, cmsText);
    //latex.SetTextFont(extraTextFont);
    //latex.SetTextAlign(align_);
    //latex.SetTextSize(extraTextSize*t);
    //latex.DrawLatex(posX_+3.3, posY_- relExtraDY*cmsTextSize*t, extraText);
    
    TGraph *h10 = new TGraph(6, Data_x, Data_5G);
    h10->SetLineColor(kBlack);
    h10->SetLineWidth(3);
    h10->SetLineStyle(1);
    h10->Write();
    
    TGraph *h10_1 = new TGraph(1, SRAM_PU70_x, SRAM_PU70_Data);
    h10_1->SetMarkerColor(kAzure - 10);
    h10_1->SetMarkerStyle(22);
    h10_1->SetMarkerSize(2);
    h10_1->Write();

    TGraph *h10_2 = new TGraph(1, SRAM_PU130_x, SRAM_PU130_Data);
    h10_2->SetMarkerColor(kMagenta - 4);
    h10_2->SetMarkerStyle(22);
    h10_2->SetMarkerSize(2);
    h10_2->Write();
    
    //FED v15.5 periodic and random triggers
    TGraph *h10_3 = new TGraph(1, data_10_rand_x, data_10_rand);
    h10_3->SetMarkerColor(kGreen);
    h10_3->SetMarkerStyle(20);
    h10_3->SetMarkerSize(1);
    h10_3->Write();
    
    TGraph *h10_4 = new TGraph(1, data_12_rand_x, data_12_rand);
    h10_4->SetMarkerColor(kGreen);
    h10_4->SetMarkerStyle(20);
    h10_4->SetMarkerSize(1);
    h10_4->Write();
    
    TGraph *h10_5 = new TGraph(1, data_14_rand_x, data_14_rand);
    h10_5->SetMarkerColor(kGreen);
    h10_5->SetMarkerStyle(20);
    h10_5->SetMarkerSize(1);
    h10_5->Write();
    
    TGraph *h10_6 = new TGraph(1, data_10_per_x, data_10_per);
    h10_6->SetMarkerColor(kMagenta);
    h10_6->SetMarkerStyle(20);
    h10_6->SetMarkerSize(1);
    h10_6->Write();
    
    TGraph *h10_7 = new TGraph(1, data_12_per_x, data_12_per);
    h10_7->SetMarkerColor(kMagenta);
    h10_7->SetMarkerStyle(20);
    h10_7->SetMarkerSize(1);
    h10_7->Write();
    
    TGraph *h10_8 = new TGraph(1, data_14_per_x, data_14_per);
    h10_8->SetMarkerColor(kMagenta);
    h10_8->SetMarkerStyle(20);
    h10_8->SetMarkerSize(1);
    h10_8->Write();
    
    TGraph *h11_3 = new TGraph(1, data_10_rand_x, trig_10_rand);
    h11_3->SetMarkerColor(kGreen);
    h11_3->SetMarkerStyle(24);
    h11_3->SetMarkerSize(1);
    h11_3->Write();
    
    TGraph *h11_4 = new TGraph(1, data_12_rand_x, trig_12_rand);
    h11_4->SetMarkerColor(kGreen);
    h11_4->SetMarkerStyle(24);
    h11_4->SetMarkerSize(1);
    h11_4->Write();
    
    TGraph *h11_5 = new TGraph(1, data_14_rand_x, trig_14_rand);
    h11_5->SetMarkerColor(kGreen);
    h11_5->SetMarkerStyle(24);
    h11_5->SetMarkerSize(1);
    h11_5->Write();
    
    TGraph *h11_6 = new TGraph(1, data_10_per_x,  trig_10_per);
    h11_6->SetMarkerColor(kMagenta);
    h11_6->SetMarkerStyle(24);
    h11_6->SetMarkerSize(1);
    h11_6->Write();
    
    TGraph *h11_7 = new TGraph(1, data_12_per_x,  trig_12_per);
    h11_7->SetMarkerColor(kMagenta);
    h11_7->SetMarkerStyle(24);
    h11_7->SetMarkerSize(1);
    h11_7->Write();
    
    TGraph *h11_8 = new TGraph(1, data_14_per_x,  trig_14_per);
    h11_8->SetMarkerColor(kMagenta);
    h11_8->SetMarkerStyle(24);
    h11_8->SetMarkerSize(1);
    h11_8->Write();
    
    //FED v15.1
    TGraph *h11 = new TGraph(8, Data_x_10G, Data_10G);
    h11->SetLineColor(kRed);
    h11->SetLineWidth(3);
    h11->SetLineStyle(1);
    h11->Write();
    
    TGraph *h12 = new TGraph(8, Data_x_10G, Data_SRAM);
    h12->SetLineColor(kBlue);
    h12->SetLineWidth(3);
    h12->SetLineStyle(1);
    h12->Write();
    
    TGraph *h13 = new TGraph(6, Data_x, Trigger_5G);
    h13->SetLineColor(kBlack);
    h13->SetLineWidth(3);
    h13->SetLineStyle(2);
    h13->Write();
   
    TGraph *h14 = new TGraph(8, Data_x_10G, Trigger_10G);
    h14->SetLineColor(kRed);
    h14->SetLineWidth(3);
    h14->SetLineStyle(2);
    h14->Write();
    
    TGraph *h15 = new TGraph(8, Data_x_10G, Trigger_SRAM);
    h15->SetLineColor(kBlue);
    h15->SetLineWidth(3);
    h15->SetLineStyle(2);
    h15->Write();
    
    //FED v15.5 periodic/fine scan
    TGraph *h20 = new TGraph(8, Data_x_v15_5, Data_periodic);
    h20->SetLineColor(kGreen);
    h20->SetLineWidth(3);
    h20->SetLineStyle(1);
    h20->Write();
    
    TGraph *h21 = new TGraph(27, fine_x, Data_fine);
    h21->SetLineColor(kMagenta);
    h21->SetLineWidth(3);
    h21->SetLineStyle(1);
    h21->Write();
    
    TGraph *h20_1 = new TGraph(8, Data_x_v15_5, Trigger_periodic);
    h20_1->SetLineColor(kGreen);
    h20_1->SetLineWidth(3);
    h20_1->SetLineStyle(2);
    h20_1->Write();
    
    TGraph *h21_1 = new TGraph(27, fine_x, Trigger_fine);
    h21_1->SetLineColor(kMagenta);
    h21_1->SetLineWidth(3);
    h21_1->SetLineStyle(2);
    h21_1->Write();
    
    //FED Theory Prediction
    TGraph *hTheory = new TGraph(14, Trig_Theory_x, Trig_Theory);
    hTheory->SetLineColor(kBlack);
    hTheory->SetLineWidth(3);
    hTheory->SetLineStyle(2);
    hTheory->Write();
   
    //FED v15.8 random trigger
    TGraph *h22 = new TGraph(14, Data_v15_8_x, Data_v15_8);
    h22->SetLineColor(kMagenta);
    h22->SetLineWidth(3);
    h22->SetLineStyle(1);
    h22->Write();

    TGraph *h22_1 = new TGraph(14, Data_v15_8_x, Trig_v15_8);
    h22_1->SetLineColor(kMagenta);
    h22_1->SetLineWidth(3);
    h22_1->SetLineStyle(2);
    h22_1->Write();

    //FED v15.9 random trigger
    TGraph *h23 = new TGraph(14, Data_v15_8_x, Data_v15_9);
    h23->SetLineColor(kAzure);
    h23->SetLineWidth(3);
    h23->SetLineStyle(1);
    h23->Write();

    TGraph *h23_1 = new TGraph(14, Data_v15_8_x, Trig_v15_9);
    h23_1->SetLineColor(kAzure);
    h23_1->SetLineWidth(3);
    h23_1->SetLineStyle(2);
    h23_1->Write();

    //FED v15.10 random trigger
    TGraph *h24 = new TGraph(14, Data_v15_8_x, Data_v15_10);
    h24->SetLineColor(kGreen);
    h24->SetLineWidth(3);
    h24->SetLineStyle(1);
    h24->Write();

    TGraph *h24_1 = new TGraph(14, Data_v15_8_x, Trig_v15_10);
    h24_1->SetLineColor(kGreen);
    h24_1->SetLineWidth(3);
    h24_1->SetLineStyle(2);
    h24_1->Write();

    TMultiGraph * mgdata = new TMultiGraph();
    //mgdata->Add(h10);
    mgdata->Add(h11);
    //mgdata->Add(h12);
    mgdata->Add(h22);
    mgdata->Add(h23);
    mgdata->Add(h24);
    mgdata->Add(h10_1, "P");
    mgdata->Add(h10_2, "P");
    mgdata->Add(h10_3, "P");
    mgdata->Add(h10_4, "P");
    mgdata->Add(h10_5, "P");
    mgdata->Add(h10_6, "P");
    mgdata->Add(h10_7, "P");
    mgdata->Add(h10_8, "P");
    mgdata->Add(h11_3, "P");
    mgdata->Add(h11_4, "P");
    mgdata->Add(h11_5, "P");
    mgdata->Add(h11_6, "P");
    mgdata->Add(h11_7, "P");
    mgdata->Add(h11_8, "P");
    mgdata->Draw();
    
    Total->cd();
    //FED v15.1
    TPad *overlay = new TPad("overlay","",0,0,1,1);
    overlay->SetFillStyle(4000);
    overlay->SetFillColor(0);
    overlay->SetFrameFillStyle(4000);
    overlay->Draw();
    overlay->cd();
    Double_t xmin = pad->GetUxmin();
    Double_t ymin = 0;
    Double_t xmax = pad->GetUxmax();
    Double_t ymax = 120;
    TH1F *hframe = overlay->DrawFrame(xmin,ymin,xmax,ymax);
    hframe->GetXaxis()->SetLabelOffset(99);
    hframe->GetYaxis()->SetLabelOffset(99);
    hframe->GetYaxis()->SetNdivisions(612);
    h14->Draw();

    //TPad *overlay3 = new TPad("overlay3","",0,0,1,1);
    //overlay3->SetFillStyle(4000);
    //overlay3->SetFillColor(0);
    //overlay3->SetFrameFillStyle(4000);
    //overlay3->Draw();
    //overlay3->cd();
    //Double_t xmin3 = pad->GetUxmin();
    //Double_t ymin3 = 0;
    //Double_t xmax3 = pad->GetUxmax();
    //Double_t ymax3 = 120;
    //TH1F *hframe3 = overlay3->DrawFrame(xmin3,ymin3,xmax3,ymax3);
    //hframe3->GetXaxis()->SetLabelOffset(99);
    //hframe3->GetYaxis()->SetLabelOffset(99);
    //hframe3->GetYaxis()->SetNdivisions(612);
    //h15->Draw();
    
    //FED v15.8
    TPad *overlay2 = new TPad("overlay2","",0,0,1,1);
    overlay2->SetFillStyle(4000);
    overlay2->SetFillColor(0);
    overlay2->SetFrameFillStyle(4000);
    overlay2->Draw();
    overlay2->cd();
    Double_t xmin2 = pad->GetUxmin();
    Double_t ymin2 = 0;
    Double_t xmax2 = pad->GetUxmax();
    Double_t ymax2 = 120;
    TH1F *hframe2 = overlay2->DrawFrame(xmin2,ymin2,xmax2,ymax2);
    hframe2->GetXaxis()->SetLabelOffset(99);
    hframe2->GetYaxis()->SetLabelOffset(99);
    hframe2->GetYaxis()->SetNdivisions(612);
    h22_1->Draw();
    
    //FED v15.9
    TPad *overlay6 = new TPad("overlay6","",0,0,1,1);
    overlay6->SetFillStyle(4000);
    overlay6->SetFillColor(0);
    overlay6->SetFrameFillStyle(4000);
    overlay6->Draw();
    overlay6->cd();
    Double_t xmin6 = pad->GetUxmin();
    Double_t ymin6 = 0;
    Double_t xmax6 = pad->GetUxmax();
    Double_t ymax6 = 120;
    TH1F *hframe6 = overlay6->DrawFrame(xmin6,ymin6,xmax6,ymax6);
    hframe6->GetXaxis()->SetLabelOffset(99);
    hframe6->GetYaxis()->SetLabelOffset(99);
    hframe6->GetYaxis()->SetNdivisions(612);
    h23_1->Draw();
    
    //FED v15.10
    TPad *overlay7 = new TPad("overlay7","",0,0,1,1);
    overlay7->SetFillStyle(4000);
    overlay7->SetFillColor(0);
    overlay7->SetFrameFillStyle(4000);
    overlay7->Draw();
    overlay7->cd();
    Double_t xmin7 = pad->GetUxmin();
    Double_t ymin7 = 0;
    Double_t xmax7 = pad->GetUxmax();
    Double_t ymax7 = 120;
    TH1F *hframe7 = overlay7->DrawFrame(xmin7,ymin7,xmax7,ymax7);
    hframe7->GetXaxis()->SetLabelOffset(99);
    hframe7->GetYaxis()->SetLabelOffset(99);
    hframe7->GetYaxis()->SetNdivisions(612);
    h24_1->Draw();
    
    //Theory
    TPad *overlay5 = new TPad("overlay5","",0,0,1,1);
    overlay5->SetFillStyle(5000);
    overlay5->SetFillColor(0);
    overlay5->SetFrameFillStyle(5000);
    overlay5->Draw();
    overlay5->cd();
    Double_t xmin5 = pad->GetUxmin();
    Double_t ymin5 = 0;
    Double_t xmax5 = pad->GetUxmax();
    Double_t ymax5 = 120;
    TH1F *hframe5 = overlay5->DrawFrame(xmin5,ymin5,xmax5,ymax5);
    hframe5->GetXaxis()->SetLabelOffset(99);
    hframe5->GetYaxis()->SetLabelOffset(99);
    hframe5->GetYaxis()->SetNdivisions(612);
    hTheory->Draw();
    
    TGaxis *axis = new TGaxis(xmax,ymin,xmax, ymax,ymin,ymax,612,"+L");
    axis->SetTitle("Trigger Rate (kHz)");
    axis->SetTitleSize(0.035);
    axis->SetLabelSize(0.03);
    axis->SetTitleFont(42);
    axis->SetLabelFont(42);
    axis->Draw();

    auto legend = new TLegend(0.45,0.11,0.7,0.35);
    //legend->AddEntry(h10, "Data 5G", "l");
    legend->AddEntry(hTheory, "Trig Theory", "l");
    legend->AddEntry(h11, "Data 10G", "l");
    //legend->AddEntry(h12, "SRAM 10G", "l");
    //legend->AddEntry(h13, "Trigger 5G", "l");
    legend->AddEntry(h14, "Trigger 10G", "l");
    //legend->AddEntry(h15, "SRAM Trigger 10G", "l");
    legend->AddEntry(h22, "Data v15.8", "l");
    legend->AddEntry(h22_1, "Trigger v15.8", "l");
    legend->AddEntry(h23, "Data v15.9", "l");
    legend->AddEntry(h23_1, "Trigger v15.9", "l");
    legend->AddEntry(h24, "Data v15.10", "l");
    legend->AddEntry(h24_1, "Trigger v15.10", "l");
    legend->AddEntry(h10_1, "PU=70 expected (PYTHIA)", "p");
    legend->AddEntry(h10_2, "PU=130", "p");
    legend->AddEntry(h10_3, "Random", "p");
    legend->AddEntry(h10_6, "Periodic", "p");
    legend->Draw();

    Total->SaveAs(outputDir + "/" + "FEROL_Data_Throughput.pdf");
}

void Slink_Plots_PKAM(vector<vector<unsigned int>>& SLINK_RDY, vector<vector<unsigned int>>& SLINK_BSY, int fw_major, int fw_minor, int SRAM){

    TFile * fout;
    string fw_major_str = to_string(fw_major);
    string fw_minor_str = to_string(fw_minor);
    string PKAM_str = to_string(PKAMRead());
    TString outputDir = "v" + fw_major_str + "_" + fw_minor_str + "_log";
    gSystem->mkdir(outputDir, true);
    TString SlinkName = "Slink_v" + fw_major_str + "_" + fw_minor_str;
    if(SRAM) SlinkName += "_SRAM_" + PKAM_str + "clks.root";
    else     SlinkName += "_" + PKAM_str + "clks.root";
    fout = new TFile(outputDir + "/" + SlinkName, "Recreate");

    int glibVersion[3] = {};
    for(int i = 1; i < 4; i++) glibVersion[i - 1] = GLIBVersion(i);

    string glibVersion_str;
    if(glibVersion[0] == 0x32 || glibVersion[1] == 0x32 || glibVersion[2] == 0x32) glibVersion_str = "TBM09_10";
    else glibVersion_str = "TBM08";

    TH1F *h6[2];
    TH1F *h7[2];  
    TCanvas *Total[2];
    TCanvas *AllDDR[2];
    TString SlinkRocs = "Slink_v" + fw_major_str + "_" + fw_minor_str;

    TString PKAMName = "Slink Size PKAM " + PKAM_str + "clks";
    TString PKAMName_curr = "Slink Size Current PKAM " + PKAM_str + "clks";
    TString PKAMName_prev = "Slink Size " + glibVersion_str + " PKAM " + PKAM_str + "clks";


    h6[1] = new TH1F("h6[1]",PKAMName_prev,1101,-0.5,1100.5);
    h6[1]->SetDirectory(fout);
    h6[1]->GetXaxis()->SetTitle("Slink Size");

    h7[1] = new TH1F("h7[1]",PKAMName_prev,1101,-0.5,1100.5);
    h7[1]->SetDirectory(fout);
    h7[1]->GetXaxis()->SetTitle("Slink Size");

    for(int i = 1; i < 2; i++){
        for(uint32_t j = 0; j < SLINK_RDY[i].size(); j++) h6[i]->Fill(SLINK_RDY[ i ][ j ]);
        for(uint32_t j = 0; j < SLINK_BSY[i].size(); j++) h7[i]->Fill(SLINK_BSY[ i ][ j ]);
    }

    for(int i = 1; i < 2; i++){
        AllDDR[i] = new TCanvas("Slink Total",PKAMName,200,10,700,500);
        AllDDR[i]->cd();
        gPad->SetLogy();
        h6[i]->Draw();
        h6[i]->SetLineColor(kBlue);
        h6[i]->Write();

        h7[i]->Draw("same");
        h7[i]->SetLineColor(kRed);
        h7[i]->Write();
    
        h6[i]->Draw("same");
        h6[i]->SetLineColor(kBlue);
        h6[i]->Write();

        gStyle->SetOptStat(1110);
        auto legend = new TLegend(0.80,0.65,0.95,0.80);
        legend->AddEntry("h6[1]", "Slink RDY", "l");
        legend->AddEntry("h7[1]", "Slink BSY", "l");
        legend->Draw();
        
        if(i == 0){
            if(SRAM) SlinkRocs += "_curr_SRAM_" + PKAM_str + "clks.pdf";
            else     SlinkRocs += "_curr_" + PKAM_str + "clks.pdf";
        }
        else{
            if(SRAM) SlinkRocs += "_" + glibVersion_str + "_SRAM_" + PKAM_str + "clks.pdf";
            else     SlinkRocs += "_" + glibVersion_str + "_" + PKAM_str + "clks.pdf";
        }
        AllDDR[i]->SaveAs(outputDir + "/" + SlinkRocs);   
    }
}

void Slink_Plots_PKAM(vector<vector<uint64_t>>& SLINK_SIZE, int fw_major, int fw_minor){

    TFile * fout;
    string fw_major_str = to_string(fw_major);
    string fw_minor_str = to_string(fw_minor);
    TString outputDir = "v" + fw_major_str + "_" + fw_minor_str + "_log";
    gSystem->mkdir(outputDir, true);
    TString SlinkName = "Slink_v" + fw_major_str + "_" + fw_minor_str;
    
    SlinkName += "_Size_Total.root";
    fout = new TFile(outputDir + "/" + SlinkName, "Recreate");

    int glibVersion[3] = {};
    for(int i = 1; i < 4; i++) glibVersion[i - 1] = GLIBVersion(i);

    string glibVersion_str;
    if(glibVersion[0] == 0x32 || glibVersion[1] == 0x32 || glibVersion[2] == 0x32) glibVersion_str = "_TBM09_10";
    else glibVersion_str = "_TBM08";

    int NumPlots = 64;

    TH1F *h6[NumPlots];
    TCanvas *AllPKAM;
    TCanvas *PKAMplots;
    TString SlinkRocs = "Slink_v" + fw_major_str + "_" + fw_minor_str + glibVersion_str;
    TString Slink =     "Slink_v" + fw_major_str + "_" + fw_minor_str + glibVersion_str;
    char *PKAMName = new char[NumPlots];
    char *h6name = new char[NumPlots];
    char *AllPKAMname = new char;
    char *AllPKAMtitle = new char;

    for(int i = 0; i < NumPlots; i++){
        sprintf(PKAMName, "Event Hit Size %i", i);
        sprintf(h6name, "h6_%i", i);

        h6[i] = new TH1F(h6name,PKAMName,1201,-0.5,1200.5);
        h6[i]->SetDirectory(fout);
        h6[i]->GetXaxis()->SetTitle("Slink Size");
    }

    for(int i = 0; i < NumPlots; i++){
        for(uint32_t j = 256; j < (SLINK_SIZE[i].size()/2); j+=2) h6[i]->Fill((SLINK_SIZE[ i ][ j + 1 ] << 32) | SLINK_SIZE[ i ][ j ]);
    }


    AllPKAM = new TCanvas( "AllPKAM", "Slink Total",200,10,700,500);
    AllPKAM->Divide(4,4);
    SlinkRocs += "_SIZE.pdf";
    AllPKAM->Print(outputDir + "/" + SlinkRocs + TString("["));   
    
    for(int i = 0; i < NumPlots; i++){
        AllPKAM->cd((i%16) + 1);
        (AllPKAM->GetPad((i%16) + 1))->SetLogy();
        int Maxh6 = h6[i]->GetMaximum();

        h6[i]->SetLineColor(kAzure + 10);
        h6[i]->Draw();
        h6[i]->SetMaximum(1.1*Maxh6);
        h6[i]->Write();
        
        gStyle->SetOptStat(1110);
        auto legend = new TLegend(0.80,0.65,0.95,0.80);
        legend->AddEntry(h6name, "Slink SIZE", "l");
        legend->Draw();
        if((i%16) == 15) AllPKAM->Print(outputDir + "/" + SlinkRocs);
    }
    AllPKAM->Print(outputDir + "/" + SlinkRocs + TString("]"));
    
    PKAMplots = new TCanvas( "SIZEplots", "Slink Total",200,10,700,500);
    Slink += "_PKAM_all.pdf";
    PKAMplots->Print(outputDir + "/" + Slink + TString("["));   
    
    for(int i = 0; i < NumPlots; i++){
        int Maxh6 = h6[i]->GetMaximum();

        h6[i]->SetLineColor(kGreen);
        gPad->SetLogy();
        h6[i]->Draw();
        h6[i]->SetMaximum(1.1*Maxh6);
        
        gStyle->SetOptStat(1110);
        auto legend = new TLegend(0.80,0.65,0.95,0.80);
        legend->AddEntry(h6name, "Slink SIZE", "l");
        legend->Draw();
        PKAMplots->Print(outputDir + "/" + Slink);
    }
    PKAMplots->Print(outputDir + "/" + Slink + TString("]"));
}

void Slink_Plots_PKAM(vector<vector<unsigned int>>& SLINK_RDY, vector<vector<unsigned int>>& SLINK_BSY, vector<unsigned int>& PKAM, vector<unsigned int>& TTS, int fw_major, int fw_minor, int SRAM, bool TBM_Error, int Marker_Type){

    TFile * fout;
    string fw_major_str = to_string(fw_major);
    string fw_minor_str = to_string(fw_minor);
    TString outputDir = "v" + fw_major_str + "_" + fw_minor_str + "_log";
    gSystem->mkdir(outputDir, true);
    TString SlinkName = "Slink_v" + fw_major_str + "_" + fw_minor_str;
    
    if(SRAM) SlinkName += "_SRAM";
    if(TBM_Error){
        if(Marker_Type == 1) SlinkName += "_TBM_Header_Error";
        if(Marker_Type == 2) SlinkName += "_TBM_Trailer_Error";
    }
    SlinkName += "_PKAM_Total.root";
    fout = new TFile(outputDir + "/" + SlinkName, "Recreate");

    int glibVersion[3] = {};
    for(int i = 1; i < 4; i++) glibVersion[i - 1] = GLIBVersion(i);

    string glibVersion_str;
    if(glibVersion[0] == 0x32 || glibVersion[1] == 0x32 || glibVersion[2] == 0x32) glibVersion_str = "_TBM09_10";
    else glibVersion_str = "_TBM08";

    int NumPlots = 48;

    TH1F *h6[NumPlots];
    TH1F *h7[NumPlots];  
    TCanvas *AllPKAM;
    TCanvas *PKAMplots;
    TString SlinkRocs = "Slink_v" + fw_major_str + "_" + fw_minor_str + glibVersion_str;
    TString Slink =     "Slink_v" + fw_major_str + "_" + fw_minor_str + glibVersion_str;
    char *PKAMName = new char[NumPlots];
    char *h6name = new char[NumPlots];
    char *h7name = new char[NumPlots];
    char *AllPKAMname = new char;
    char *AllPKAMtitle = new char;

    for(int i = 0; i < NumPlots; i++){
        sprintf(PKAMName, "Slink Size PKAM %i clks", PKAM[i]);
        sprintf(h6name, "h6_%i", i);
        sprintf(h7name, "h7_%i", i);

        h6[i] = new TH1F(h6name,PKAMName,1201,-0.5,1200.5);
        h6[i]->SetDirectory(fout);
        h6[i]->GetXaxis()->SetTitle("Slink Size");

        h7[i] = new TH1F(h7name,PKAMName,1201,-0.5,1200.5);
        h7[i]->SetDirectory(fout);
        h7[i]->GetXaxis()->SetTitle("Slink Size");
    }

    for(int i = 0; i < NumPlots; i++){
        for(uint32_t j = 256; j < SLINK_RDY[i].size(); j++) h6[i]->Fill(SLINK_RDY[ i ][ j ]);
        for(uint32_t j = 256; j < SLINK_BSY[i].size(); j++) h7[i]->Fill(SLINK_BSY[ i ][ j ]);
    }


    AllPKAM = new TCanvas( "AllPKAM", "Slink Total",200,10,700,500);
    AllPKAM->Divide(4,4);
    if(SRAM) SlinkRocs += "_SRAM";
    if(TBM_Error){
        if(Marker_Type == 1) SlinkRocs += "_TBM_Header_Error";
        if(Marker_Type == 2) SlinkRocs += "_TBM_Trailer_Error";
    }
    SlinkRocs += "_PKAM.pdf";
    AllPKAM->Print(outputDir + "/" + SlinkRocs + TString("["));   
    
    for(int i = 0; i < NumPlots; i++){
        AllPKAM->cd((i%16) + 1);
        (AllPKAM->GetPad((i%16) + 1))->SetLogy();
        int Maxh6 = h6[i]->GetMaximum();
        int Maxh7 = h7[i]->GetMaximum();
        int Max = 0;
        if(Maxh7 >= Maxh6) Max = Maxh7;
        else Max = Maxh6;

        if(TTS[i] != 2) h6[i]->SetLineColor(kBlue);
        else h6[i]->SetLineColor(kAzure + 10);
        h6[i]->Draw();
        h6[i]->SetMaximum(1.1*Max);
        h6[i]->Write();
        
        h7[i]->SetLineColor(kRed);
        h7[i]->Draw("same");
        h7[i]->SetMaximum(1.1*Max);
        h7[i]->Write();
   
        if(TTS[i] != 2) h6[i]->SetLineColor(kBlue);
        else h6[i]->SetLineColor(kAzure + 10);
        h6[i]->Draw("same");
        h6[i]->SetMaximum(1.1*Max);
        h6[i]->Write();
    
        gStyle->SetOptStat(1110);
        auto legend = new TLegend(0.80,0.65,0.95,0.80);
        legend->AddEntry(h6name, "Slink RDY", "l");
        legend->AddEntry(h7name, "Slink BSY", "l");
        legend->Draw();
        if((i%16) == 15) AllPKAM->Print(outputDir + "/" + SlinkRocs);
    }
    AllPKAM->Print(outputDir + "/" + SlinkRocs + TString("]"));
    
    PKAMplots = new TCanvas( "PKAMplots", "Slink Total",200,10,700,500);
    if(SRAM) Slink += "_SRAM";
    if(TBM_Error){
        if(Marker_Type == 1) Slink +="_TBM_Header_Error";
        if(Marker_Type == 2) Slink +="_TBM_Trailer_Error";
    }
    Slink += "_PKAM_all.pdf";
    PKAMplots->Print(outputDir + "/" + Slink + TString("["));   
    
    for(int i = 0; i < NumPlots; i++){
        int Maxh6 = h6[i]->GetMaximum();
        int Maxh7 = h7[i]->GetMaximum();
        int Max = 0;
        if(Maxh7 >= Maxh6) Max = Maxh7;
        else Max = Maxh6;

        if(TTS[i] != 2) h6[i]->SetLineColor(kBlue);
        else h6[i]->SetLineColor(kGreen);
        gPad->SetLogy();
        h6[i]->Draw();
        h6[i]->SetMaximum(1.1*Max);
        
        h7[i]->SetLineColor(kRed);
        gPad->SetLogy();
        h7[i]->Draw("same");
        h7[i]->SetMaximum(1.1*Max);
   
        if(TTS[i] != 2) h6[i]->SetLineColor(kBlue);
        else h6[i]->SetLineColor(kAzure + 10);
        h6[i]->Draw("same");
        h6[i]->SetMaximum(1.1*Max);
    
        gStyle->SetOptStat(1110);
        auto legend = new TLegend(0.80,0.65,0.95,0.80);
        legend->AddEntry(h6name, "Slink RDY", "l");
        legend->AddEntry(h7name, "Slink BSY", "l");
        legend->Draw();
        PKAMplots->Print(outputDir + "/" + Slink);
    }
    PKAMplots->Print(outputDir + "/" + Slink + TString("]"));
}

void Slink_Plots(vector<vector<unsigned int>>& SLINK_RDY, vector<vector<unsigned int>>& SLINK_BSY, int fw_major, int fw_minor, int SRAM){

    TFile * fout;
    string fw_major_str = to_string(fw_major);
    string fw_minor_str = to_string(fw_minor);
    TString outputDir = "v" + fw_major_str + "_" + fw_minor_str + "_log";
    gSystem->mkdir(outputDir, true);
    string SlinkName_Str;
    if(SRAM) SlinkName_Str = "Slink_v" + fw_major_str + "_" + fw_minor_str + "_fedtestfed_SRAM.root";
    else     SlinkName_Str = "Slink_v" + fw_major_str + "_" + fw_minor_str + "_fedtestfed.root";
    const char* SlinkName = SlinkName_Str.c_str();
    fout = new TFile(outputDir + "/" + SlinkName, "Recreate");

    TH1F *h6[2];
    TH1F *h7[2];  
    TCanvas *Total[2];
    TCanvas *AllDDR[2];
    string SlinkRocs_str;



    h6[0] = new TH1F("h6[0]","Slink Size Current",2001,-0.5,2000.5);
    h6[0]->SetDirectory(fout);
    h6[0]->GetXaxis()->SetTitle("Slink Size");

    h6[1] = new TH1F("h6[1]","Slink Size Prev",2001,-0.5,2000.5);
    h6[1]->SetDirectory(fout);
    h6[1]->GetXaxis()->SetTitle("Slink Size");

    h7[0] = new TH1F("h7[0]","Slink Size Current",2001,-0.5,2000.5);
    h7[0]->SetDirectory(fout);
    h7[0]->GetXaxis()->SetTitle("Slink Size");

    h7[1] = new TH1F("h7[1]","Slink Size Prev",2001,-0.5,2000.5);
    h7[1]->SetDirectory(fout);
    h7[1]->GetXaxis()->SetTitle("Slink Size");

    for(int i = 0; i < 2; i++){
        for(uint32_t j = 0; j < SLINK_RDY[i].size(); j++) h6[i]->Fill(SLINK_RDY[ i ][ j ]);
        for(uint32_t j = 0; j < SLINK_BSY[i].size(); j++) h7[i]->Fill(SLINK_BSY[ i ][ j ]);
    }

    for(int i = 1; i < 2; i++){
        AllDDR[i] = new TCanvas("Slink Total","Slink all ROCs",200,10,700,500);
        AllDDR[i]->cd();
        gPad->SetLogy();
        h6[i]->Draw();
        h6[i]->SetLineColor(kBlue);
        h6[i]->Write();

        h7[i]->Draw("same");
        h7[i]->SetLineColor(kRed);
        h7[i]->Write();
    
        if(i == 0){
            if(SRAM) SlinkRocs_str = "Slink_v" + fw_major_str + "_" + fw_minor_str + "_curr_SRAM.pdf";
            else     SlinkRocs_str = "Slink_v" + fw_major_str + "_" + fw_minor_str + "_curr.pdf";
        }
        else{
            if(SRAM) SlinkRocs_str = "Slink_v" + fw_major_str + "_" + fw_minor_str + "_prev_SRAM.pdf";
            else     SlinkRocs_str = "Slink_v" + fw_major_str + "_" + fw_minor_str + "_prev.pdf";
        }
        const char* SlinkRocs = SlinkRocs_str.c_str();
        AllDDR[i]->SaveAs(outputDir + "/" + SlinkRocs);   
    }
}

void ferolTest ()
{

    const char* cHWFile;
    std::string filename ("settings/HWDescription_ferol.xml");
    cHWFile = filename.c_str();
    std::cout << "HW Description File: " << cHWFile << std::endl;

    uhal::setLogLevelTo (uhal::Debug() );

    // instantiate System Controller
    SystemController cSystemController;
    Amc13Controller  cAmc13Controller;

    // initialize map of settings so I can know the proper number of acquisitions and TBMs
    cSystemController.InitializeSettings (cHWFile, std::cout);

    // initialize HWdescription from XML, beware, settings have to be read first
    cAmc13Controller.InitializeAmc13 ( cHWFile, std::cout );
    cSystemController.InitializeHw (cHWFile, std::cout);

    auto cSetting = cSystemController.fSettingsMap.find ("NAcq");
    int cNAcq = (cSetting != std::end (cSystemController.fSettingsMap) ) ? cSetting->second : 10;
    cSetting = cSystemController.fSettingsMap.find ("BlockSize");
    int cBlockSize = (cSetting != std::end (cSystemController.fSettingsMap) ) ? cSetting->second : 2;

    cSetting = cSystemController.fSettingsMap.find ("ROCOfInterest");
    int cROCOfInterest = (cSetting != std::end (cSystemController.fSettingsMap) ) ? cSetting->second : 0;

    // configure the AMC13
    cAmc13Controller.ConfigureAmc13 ( std::cout );

    //configure FED & FITELS & SFP+
    cSystemController.ConfigureHw ( std::cout );

    auto& cFED = cSystemController.fPixFEDVector[0];
    cSystemController.fFEDInterface->getBoardInfo(cFED);
    cSystemController.fFEDInterface->findPhases (cFED);
    cSystemController.fFEDInterface->Start (cFED);

    uint32_t iAcq = 0;
    //bool running = true;
    std::cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << std::endl;
    std::cout << "Pressing Enter again will stop the application" << std::endl;
    mypause();

    cAmc13Controller.fAmc13Interface->SendEC0();
    cAmc13Controller.fAmc13Interface->EnableBGO(0);
    cAmc13Controller.fAmc13Interface->DisableBGO(0);

    mypause();
    cAmc13Controller.fAmc13Interface->StartL1A();


    while ( !kbhit() )
    {
        //std::cout << cNAcq << " ##########################" << std::endl;
        //for (auto& cFED : cSystemController.fPixFEDVector)
        //{
	  //cSystemController.fFEDInterface->readSpyFIFO(cFED);
	  cSystemController.fFEDInterface->ReadData(cFED, 0 );
	     //cSystemController.fFEDInterface->readFIFO1 (cFED);
            cSystemController.fFEDInterface->PrintSlinkStatus (cFED);
            usleep (1000000);
        //}

        iAcq++;

        if (iAcq < cNAcq && cNAcq > 0 ) continue;
        else if (cNAcq == 0 ) continue;
        else break;
    }

    //    cAmc13Controller.fAmc13Interface->StopL1A();

    //for (auto& cFED : cSystemController.fPixFEDVector)
        cSystemController.fFEDInterface->Stop (cFED);
    cAmc13Controller.fAmc13Interface->DisableMaskFEDSlot();

    //    cSystemController.HaltHw();
    //    cAmc13Controller.HaltAmc13();
    exit (0);
}

void Ferol_TBM_Header( int Choice,  int SRAM ){
	using namespace std;
	using namespace uhal;
	const char* cHWFile;
	std::string filename ("settings/HWDescription_ferol.xml");
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
	auto& cFED1 = cSystemController.fPixFEDVector[1];
	auto& cFED2 = cSystemController.fPixFEDVector[2];
	auto& cFED3 = cSystemController.fPixFEDVector[3];

    cSystemController.fFEDInterface->getBoardInfo(cFED);
    cSystemController.fFEDInterface->findPhases(cFED);
    cSystemController.fFEDInterface->Start (cFED);

	int TTSState = 0;
	std::vector<uint32_t> OOSTimeout_vec;
    std::vector<std::vector<uint32_t> > SLINK_RDY (2, vector<unsigned int>(2));
    std::vector<std::vector<uint32_t> > SLINK_BSY (2, vector<unsigned int>(2));
	int readState = 0;
	int readTBMFIFO = 0;
	int channel = 0;
	int L1A_Empty = 0;
	int Main_Empty = 0;
	int TBM_FIFO_31to0 = 0;
	int TBM_FIFO_47to32 = 0;
    int fw_major = 0;
    int fw_minor = 0;
	std::vector<std::pair<std::string, uint32_t> > Counter_reg;
	typedef std::vector<std::pair<std::string, uint32_t> > vector_type;
    int MaxRead = 200000;
    int MaxLoop = 30;

    if(Choice == 1 || Choice == 2){
        int Which_chan = rand() % 16;
        int Enable_chan = 1 << (2 * Which_chan);
        cout << std::hex << Enable_chan << endl;
        int Marker_value = rand() % 0xfff;
        int Marker_type = rand() % 3 + 1;
        int Marker_rate = rand() % 20000000 + 5000000;
        if(Choice == 1) Marker_Error(Enable_chan, Marker_type, Marker_value, Marker_rate);
        if(Choice == 2) {
            Marker_Error(0x55550000, 2, 0x7e8, 333333); // TBM Trailer Error
            //Marker_Error(0x00004444, 2, 0x7e8, 200000); // TBM Trailer Error
            //Marker_Error(0x11110000, 2, 0x7e8, 150000); // TBM Trailer Error
            Marker_Error(0x00005555, 2, 0x7e8, 1000000); // TBM Trailer Error
            //Marker_Error(0x55555555, 2, 0x7e8, 100000); // TBM Trailer Error
        }
    }

	cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
    cAmc13Controller.fAmc13Interface->SendEC0();
	cAmc13Controller.fAmc13Interface->EnableBGO(0);
    cAmc13Controller.fAmc13Interface->DisableBGO(0);

    cAmc13Controller.fAmc13Interface->DisableFakeData();
	mypause();
	mypause();
	sleep(1);

	cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
    sleep(1);

    for(int i = 0; (i < MaxLoop && readState != (MaxRead - 1)); i++){
        if(i != 0) cSystemController.fFEDInterface->Start (cFED);
        cout << "Loop Number: " << i + 1 << endl;
        cAmc13Controller.fAmc13Interface->SendEC0();
	    cAmc13Controller.fAmc13Interface->EnableBGO(0);
        sleep(0.001);
        cAmc13Controller.fAmc13Interface->DisableBGO(0);
        sleep(1);
        cAmc13Controller.fAmc13Interface->StartL1A();
        usleep(10000);
        readState = 0;
        readTBMFIFO = 0;

	    do{
	    	TTSState = cSystemController.fFEDInterface->readTTSStateNoPrint(cFED);
	    	//cSystemController.fFEDInterface->readErrorFIFO(cFED, true);
	    	readState++;
            if(TTSState == 0x8){
                SLINK_RDY[0].push_back(cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.slink64_evnt_size_curr"));
                SLINK_RDY[1].push_back(cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.slink64_evnt_size_prev"));
            }
            if(TTSState == 0x4){ 
                SLINK_BSY[0].push_back(cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.slink64_evnt_size_curr"));
                SLINK_BSY[1].push_back(cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.slink64_evnt_size_prev"));
            }
            if(readState % 10000 == 0) cout << std::dec << readState << endl;
	    	if((TTSState == 4) || (TTSState == 2)){
	    		readTBMFIFO++;
	    	}
	    	else {readTBMFIFO = 0;}
            
            if((TTSState == 2) && (readTBMFIFO == 1500)){
                for(int i = 0; i < 20; i++){
                    cAmc13Controller.fAmc13Interface->SendEC0();
                    cAmc13Controller.fAmc13Interface->EnableBGO(0);
                    sleep(0.001);
                    cAmc13Controller.fAmc13Interface->DisableBGO(0);
                }
            }
	    
            if(readState == (MaxRead - 1) || (i == (MaxLoop - 1) && readTBMFIFO >= 3000)){
	    		cSystemController.fFEDInterface->readTTSState(cFED);
                GLIBL1ACounter();
	    		Counter_reg = GLIBHeaderCountDump();
	    		for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
	    			cout << pos->first << " " << pos->second << endl;
	    		
	    		Counter_reg = GLIBTrailerCountDump();
	    		for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
	    			cout << pos->first << " " << pos->second << endl;
	    		
	    		Counter_reg = cSystemController.fFEDInterface->PacketCountDump(cFED, 0);
	    		for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
	    			cout << pos->first << " " << pos->second << endl;
	    		
	    		Counter_reg = cSystemController.fFEDInterface->TrailerCountDump(cFED, 0);
	    		for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
	    			cout << pos->first << " " << pos->second << endl;

	    		Counter_reg = cSystemController.fFEDInterface->TBMHeaderIDErrorDump(cFED, 0);
	    		for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
	    			cout << pos->first << " " << pos->second << endl;

	    		Counter_reg = cSystemController.fFEDInterface->TBMTrailerIDErrorDump(cFED, 0);
	    		for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
	    			cout << pos->first << " " << pos->second << endl;

	    		Counter_reg = cSystemController.fFEDInterface->EventResyncAheadDump(cFED, 0);
	    		for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
	    			cout << pos->first << " " << pos->second << endl;

	    		Counter_reg = cSystemController.fFEDInterface->ENECountDump(cFED, 0);
	    		for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
	    			cout << pos->first << " " << pos->second << endl;

	    		Counter_reg = cSystemController.fFEDInterface->TOCountDump(cFED, 0);
	    		for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
	    			cout << pos->first << " " << pos->second << endl;

	    		Counter_reg = cSystemController.fFEDInterface->PKAMCountDump(cFED, 0);
	    		for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
	    			cout << pos->first << " " << pos->second << endl;

	    		Counter_reg = cSystemController.fFEDInterface->NTPCountDump(cFED, 0);
	    		for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
	    			cout << pos->first << " " << pos->second << endl;

	    		Counter_reg = cSystemController.fFEDInterface->ROC_NBR_ERRCountDump(cFED, 0);
	    		for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
	    			cout << pos->first << " " << pos->second << endl;

	    		//Counter_reg = cSystemController.fFEDInterface->OVFCountDump(cFED, 0);
	    		//for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
	    		//	cout << pos->first << " " << pos->second << endl;

	    		//Counter_reg = cSystemController.fFEDInterface->AUTORESETCountDump(cFED, 0);
	    		//for(vector_type::const_iterator pos = Counter_reg.begin(); pos != Counter_reg.end(); ++pos)
	    		//	cout << pos->first << " " << pos->second << endl;

	    		break;
	    	}
	        if(readTBMFIFO >= 3000) break;
	    }while(readState < MaxRead);
        
        cAmc13Controller.fAmc13Interface->StopL1A();
        cSystemController.fFEDInterface->Stop (cFED);
        sleep(1);
    }
    cAmc13Controller.fAmc13Interface->StopL1A();
    fw_major = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.user_iphc_fw_id.fw_ver_nb");
    fw_minor = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.user_iphc_fw_id.archi_ver_nb");
    Slink_Plots_PKAM(SLINK_RDY, SLINK_BSY, fw_major, fw_minor, SRAM);
}	

void Ferol_TBM_Header(bool TBM_Error, bool SlinkScan, int SRAM ){
	using namespace std;
	using namespace uhal;
	const char* cHWFile;
	std::string filename ("settings/HWDescription_ferol.xml");
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
	auto& cFED1 = cSystemController.fPixFEDVector[1];
	auto& cFED2 = cSystemController.fPixFEDVector[2];
	auto& cFED3 = cSystemController.fPixFEDVector[3];

    cSystemController.fFEDInterface->getBoardInfo(cFED);
    cSystemController.fFEDInterface->findPhases(cFED);
    cSystemController.fFEDInterface->Start (cFED);

	int TTSState = 0;
	std::vector<uint32_t> OOSTimeout_vec;
    std::vector<std::vector<uint32_t> > SLINK_RDY  (256, vector<unsigned int>(256));
    std::vector<std::vector<uint32_t> > SLINK_BSY  (256, vector<unsigned int>(256));
    std::vector<std::vector<uint64_t> > SLINK_SIZE (256, vector<uint64_t>(256));
    std::vector<unsigned int> PKAM;
    std::vector<unsigned int> TTS;
	int readState = 0;
	int readTBMFIFO = 0;
	int channel = 0;
	int L1A_Empty = 0;
	int Main_Empty = 0;
	int TBM_FIFO_31to0 = 0;
	int TBM_FIFO_47to32 = 0;
    int fw_major = 0;
    int fw_minor = 0;
    int MaxRead = 20000;
    int Marker_Type = 1;
    uint32_t NumHits1 = 0;
    uint32_t NumHits2 = 0;
    int NumHits[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    if(TBM_Error){
        Marker_Error(0x44440000, Marker_Type, 0x7e8, 333333); // TBM Trailer Error
        Marker_Error(0x00004444, Marker_Type, 0x7e8, 200000); // TBM Trailer Error
        Marker_Error(0x11110000, Marker_Type, 0x7e8, 150000); // TBM Trailer Error
        Marker_Error(0x00004444, Marker_Type, 0x7e8, 1000000); // TBM Trailer Error
        //Marker_Error(0x55555555, 2, 0x7e8, 100000); // TBM Trailer Error
    }

	cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
    cAmc13Controller.fAmc13Interface->SendEC0();
	cAmc13Controller.fAmc13Interface->EnableBGO(0);
    cAmc13Controller.fAmc13Interface->DisableBGO(0);

    cAmc13Controller.fAmc13Interface->DisableFakeData();
	mypause();
	mypause();
	sleep(1);

	cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
    sleep(1);
    for(int i = 0; i < 64; i++){
        if(i != 0) cSystemController.fFEDInterface->Start (cFED);
        if(TBM_Error){
            PKAM_Enable( 1, i);
            usleep(1);
            PKAM.push_back(PKAMRead());
            cout << "PKAM: " << PKAM[i] << endl;
        }
        if(SlinkScan){
            NumHits[i % 8]++;
            NumHits1 = NumHits[0] << 24 | NumHits[1] << 16 | NumHits[2] << 8 | NumHits[3];
            NumHits2 = NumHits[4] << 24 | NumHits[5] << 16 | NumHits[6] << 8 | NumHits[7];
            std::cout << "NumHits1: " << std::hex << NumHits1 << " NumHits2: " << NumHits2 << std::endl;
            Set_Num_Hits( NumHits1, NumHits2, NumHits1, NumHits2, NumHits1, NumHits2, NumHits1, NumHits2, NumHits1, NumHits2, NumHits1, NumHits2, 0x55555555);
        }
        cAmc13Controller.fAmc13Interface->SendEC0();
	    cAmc13Controller.fAmc13Interface->EnableBGO(0);
        sleep(0.001);
        cAmc13Controller.fAmc13Interface->DisableBGO(0);
        sleep(1);
        cAmc13Controller.fAmc13Interface->StartL1A();
        usleep(10000);
        readState = 0;
        readTBMFIFO = 0;

        do{
	    	TTSState = cSystemController.fFEDInterface->readTTSStateNoPrint(cFED);
	    	//cSystemController.fFEDInterface->readErrorFIFO(cFED, true);
	    	readState++;
            if(SlinkScan) SLINK_SIZE[i].push_back(cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.slink_core_status.data_31to0"));
            if(SlinkScan) SLINK_SIZE[i].push_back(cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.slink_core_status.data_63to32"));
            if(TTSState == 0x8){
                SLINK_RDY[i].push_back(cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.slink64_evnt_size_prev"));
            }
            if(TTSState == 0x4){ 
                SLINK_BSY[i].push_back(cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.slink64_evnt_size_prev"));
            }
            if(readState % 10000 == 0) cout << std::dec << readState << endl;
	    	if((TTSState == 4) || (TTSState == 2)){
	    		readTBMFIFO++;
	    	}
	    	else {readTBMFIFO = 0;}
            
            if((TTSState == 2) && (readTBMFIFO % 300 == 0)){
                cAmc13Controller.fAmc13Interface->SendEC0();
                cAmc13Controller.fAmc13Interface->EnableBGO(0);
                sleep(0.001);
                cAmc13Controller.fAmc13Interface->DisableBGO(0);
            }
	    
	    	if(readTBMFIFO >= 3000) break;

	    }while(readState < MaxRead && readTBMFIFO < 3000);
        TTS.push_back(TTSState);
        cAmc13Controller.fAmc13Interface->StopL1A();
        cSystemController.fFEDInterface->Stop (cFED);
        sleep(1);
    }
    fw_major = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.user_iphc_fw_id.fw_ver_nb");
    fw_minor = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_stat_regs.user_iphc_fw_id.archi_ver_nb");
    if(TBM_Error) Slink_Plots_PKAM(SLINK_RDY, SLINK_BSY, PKAM, TTS, fw_major, fw_minor, SRAM, TBM_Error, Marker_Type);
    if(SlinkScan) Slink_Plots_PKAM(SLINK_SIZE, fw_major, fw_minor);
}

