#include "TRint.h"
#include <iostream>     // std::cout
#include <algorithm>    // std::next_permutation, std::sort
#include <map>
#include "TMath.h"
#include "TLegend.h"
#include "TString.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include "TCanvas.h"

#include <sstream>
#include <fstream>
using namespace std;
typedef map<TString, float> mapStrToF;

#include "../HeaderFiles/BasicFunctions_SystemCommands.h"
#include "./PunziTestingFunctions.h"



int main( int argc, char* argv[]) {
    
    TRint theApp("App", &argc, argv);
    Bool_t retVal = kTRUE;
    
    
    bool doVerb = false;

    
    int whichVersPunzi = 1;

    
    TString addNamePunzi = whichVersPunzi ? "SigmaA2_SigmaB2_" : "SigmaA5_SigmaB2_";
    
    vector<TString> vecCheckAddName{"", "TwoThirds", "Sqrt", "CubeRoot", "TwoFifths"};
    TString baseStringLatex = "#epsilon_{S} = #epsilon_{S0} + #Delta #epsilon_{S} #frac{B^{C}}{#Delta B}";
//    vector<TString> vecLegNames{"C = 1",  "C = #frac{2}{3}", "C = #frac{1}{2}", "C = #frac{1}{3}", "C = #frac{2}{5}"};
    vector<TString> vecLegNames{"C = 1",  "C = 2/3", "C = 1/2", "C = 1/3", "C = 2/5"};
    vector<Color_t> vecColor{kBlack, kRed, kGreen+3, kBlue, kMagenta + 2};
    vector<Style_t> vecMarkerStyle{0, 20, 22, 33, 34};
    vector<TString> vecPunziFiles;
    vector<TString> vecLimitFiles;
    
    vector<TH1F *> vecPunziHists;
    vector<TGraphErrors *> vecLimitGraphs;
    
    TString baseFileNamePunzi = "combinedParams_Punzi";
    TString baseFileNameLimits = "combinedLimits_Punzi";
    TString endName = "_OneBin.txt";
    int numHists = 5;
    
    vecPunziFiles.resize(numHists);
    vecPunziHists.resize(numHists);
    vecLimitFiles.resize(numHists);
    vecLimitGraphs.resize(numHists);
    
    TString currPunziHistName, currLimitName;
    
    vector<TCanvas> vecCanv(numHists);
    
    TCanvas * commonCanv = new TCanvas("c_PunziToyTest", "c_PunziToyTest", 800, 800);
    commonCanv->SetTopMargin(0.032);
    commonCanv->SetRightMargin(0.03);
    TLegend * A = new TLegend(0.157, 0.67, 0.37, 0.91);
    TCanvas * commonCanv2 = new TCanvas("c_PunziToyTest2", "c_PunziToyTest2", 800, 800);
    commonCanv2->SetTopMargin(0.032);
    commonCanv2->SetBottomMargin(0.131);
    commonCanv2->SetLeftMargin(0.148);
    commonCanv2->SetRightMargin(0.024);
    int minHist = 1;
    for (int iHist = minHist; iHist < numHists; ++iHist) {
        vecPunziFiles[iHist] = baseFileNamePunzi + addNamePunzi + vecCheckAddName[iHist] + endName;
        vecLimitFiles[iHist] = baseFileNameLimits + addNamePunzi + vecCheckAddName[iHist] + endName;
        
        currPunziHistName = "h_Punzi" + addNamePunzi + vecCheckAddName[iHist];
        currLimitName = "h_Limit" + addNamePunzi + vecCheckAddName[iHist];
        
        PunziHist(vecPunziHists[iHist], vecPunziFiles[iHist], currPunziHistName, false, doVerb);
        vecLimitGraphs[iHist] = new TGraphErrors();
        SetTGraphErrors(vecLimitGraphs[iHist], vecPunziHists[iHist], vecLimitFiles[iHist], false, doVerb);
        vecLimitGraphs[iHist]->SetName(currLimitName);
        vecCanv[iHist].cd();
        
        vecPunziHists[iHist]->SetMarkerColor(vecColor[iHist]);
        vecPunziHists[iHist]->SetLineColor(vecColor[iHist]);
        vecPunziHists[iHist]->SetMarkerStyle(vecMarkerStyle[iHist]);
        vecLimitGraphs[iHist]->SetMarkerColor(vecColor[iHist]);
        vecLimitGraphs[iHist]->SetMarkerStyle(vecMarkerStyle[iHist]);
        vecLimitGraphs[iHist]->SetLineColor(vecColor[iHist]);
        
        vecPunziHists[iHist]->Draw("e1");
        vecLimitGraphs[iHist]->Draw("P same");
        
        commonCanv->cd();
        if (iHist == minHist) {
            vecPunziHists[iHist]->Draw("e1");
            vecPunziHists[iHist]->GetYaxis()->SetRangeUser(0,1);
            vecPunziHists[iHist]->GetXaxis()->SetRangeUser(0,200);
        }
        else {
            vecPunziHists[iHist]->Draw("e1 same");
        }
        vecLimitGraphs[iHist]->Draw("P same");
        A->AddEntry(vecPunziHists[iHist], vecLegNames[iHist], "lp");
        
        commonCanv2->cd();
        if (iHist == minHist) {
            TH1F * clone = (TH1F*) vecPunziHists[iHist]->Clone("tempClone");
            clone->Draw("e1");
            clone->GetYaxis()->SetRangeUser(0.15, 0.65);
            clone->GetYaxis()->SetTitleOffset(1.25);
            clone->GetXaxis()->SetRangeUser(0, 60);
        }
        else {
            vecPunziHists[iHist]->Draw("e1 same");
        }
        vecLimitGraphs[iHist]->Draw("P same");
    }
    
    commonCanv->cd();
    A->Draw("same");
    
    commonCanv->SaveAs("c_PunziToyTest.pdf");
    commonCanv2->SaveAs("c_PunziToyTest2.pdf");
    


    theApp.Run(retVal);
}