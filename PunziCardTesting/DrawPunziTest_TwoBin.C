#include "TRint.h"
#include <iostream>     // std::cout
#include <algorithm>    // std::next_permutation, std::sort
#include <map>
#include "TMath.h"
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
    
    TString baseFilePunzi = "combinedPunzi_TwoBinCheck";
    TString baseFilePunzi2 = "combinedPunzi2_TwoBinCheck";
    TString baseFilePunzi3 = "combinedPunzi3_TwoBinCheck";
    TString baseFilePunzi4 = "combinedPunzi4_TwoBinCheck";
    TString baseFileLimits = "combinedLimits_TwoBinCheck";
    
    TString namePunziHist = "h_Punzi";
    TString namePunziHist2 = "h_Punzi2";
    TString namePunziHist3 = "h_Punzi3";
    TString namePunziHist4 = "h_Punzi4";
    TString nameLimitsHist = "h_Limits";
    
    TH1F * histPunzi, * histPunzi2, * histPunzi3, * histPunzi4;
    TH1F * histLimits;
    
    bool doVerb = false;
    
    
    int whichSigBin3 = 0;
    int whichBkgBin3 = 0;
    
    int whichVersPunzi = 1;
    
    for (int k = 0; k < argc; ++k) {
        if (strncmp (argv[k],"doVerb", 6) == 0) {
            doVerb = 1;
        }
        else if (strncmp (argv[k],"wSig", 4) == 0) {
            whichSigBin3 = strtol(argv[k + 1], NULL, 10 );
        }
        else if (strncmp (argv[k],"wBkg", 4) == 0) {
            whichBkgBin3 = strtol(argv[k + 1], NULL, 10 );
        }
        else if (strncmp (argv[k],"wPunzi", 6) == 0) {
            whichVersPunzi = strtol(argv[k + 1], NULL, 10 );
        }
    }
    
    TString addName = "_Bin3Sig";
    addName += whichSigBin3;
    addName += "Bkg";
    addName += whichBkgBin3;
    addName += ".txt";
    
    TString filePunzi = baseFilePunzi + addName;
    TString filePunzi2 = baseFilePunzi2 + addName;
    TString filePunzi3 = baseFilePunzi3 + addName;
    TString filePunzi4 = baseFilePunzi4 + addName;
    TString fileLimits = baseFileLimits + addName;
    
    
    //    LimitHist(histLimits, fileLimits, nameLimitsHist, 1, doVerb);
    PunziHist(histPunzi, filePunzi, namePunziHist, true, doVerb);
    PunziHist(histPunzi2, filePunzi2, namePunziHist2, true, doVerb);
    PunziHist(histPunzi3, filePunzi3, namePunziHist3, true, doVerb);
    PunziHist(histPunzi4, filePunzi4, namePunziHist4, true, doVerb);
    
    TGraphErrors * graph_TestLimits = new TGraphErrors();
    graph_TestLimits->SetName("graph_TestLimits");
    SetTGraphErrors(graph_TestLimits, histPunzi, fileLimits, true, doVerb);
    
    TString nameCanv_PunziComboVal = "c_PunziComboVal" + addName;
    TCanvas * c_HistPunzi_ComboVal = new TCanvas(nameCanv_PunziComboVal, nameCanv_PunziComboVal);

    TString nameCanv_PunziNonSquareComboVal = "c_PunziNonSquareComboVal" + addName;
    TCanvas * c_HistPunzi_NonSquareComboVal = new TCanvas(nameCanv_PunziNonSquareComboVal, nameCanv_PunziNonSquareComboVal);
    
    TString nameCanv_PunziEllipseVal = "c_PunziEllipse" + addName;
    TCanvas * c_HistPunzi_EllipseVal = new TCanvas(nameCanv_PunziEllipseVal, nameCanv_PunziEllipseVal);
    
    TString nameCanv_PunziEllipseVal2 = "c_PunziEllipse2" + addName;
    TCanvas * c_HistPunzi_EllipseVal2 = new TCanvas(nameCanv_PunziEllipseVal2, nameCanv_PunziEllipseVal2);
    
    c_HistPunzi_ComboVal->cd();
    histPunzi->Draw("e1");
    
    c_HistPunzi_NonSquareComboVal->cd();
    histPunzi2->Draw("e1");
    
    c_HistPunzi_EllipseVal->cd();
    histPunzi3->Draw("e1");
    
    c_HistPunzi_EllipseVal2->cd();
    histPunzi4->Draw("e1");
    
    TString nameCanv_Limits = "c_Limits" + addName;
    TCanvas * c_Limits = new TCanvas(nameCanv_Limits, nameCanv_Limits);
    c_Limits->cd();
    graph_TestLimits->Draw("AP");
    
    vector<TString> vecAddName{"1", "2", "3", "4"};
    vector<TH1F *> vecWeightedPunziLimitCompare;
    vecWeightedPunziLimitCompare.push_back(WeightedPunziLimitCompare(histPunzi, graph_TestLimits, 1, vecAddName[0]));
    vecWeightedPunziLimitCompare.push_back(WeightedPunziLimitCompare(histPunzi2, graph_TestLimits, 1, vecAddName[1]));
    vecWeightedPunziLimitCompare.push_back(WeightedPunziLimitCompare(histPunzi3, graph_TestLimits, -1, vecAddName[2]));
    vecWeightedPunziLimitCompare.push_back(WeightedPunziLimitCompare(histPunzi4, graph_TestLimits, -1, vecAddName[3]));
    vector<TString> vecCanvNames_PunziLimitCompare{"c_WeightedCompare_ComboVal", "c_WeightedCompare_NonSquareComboVal", "c_WeightedCompare_EllipseVal", "c_WeightedCompare_EllipseVal2"};
    vector<TCanvas *> vecCanvases_WeightedPunziLimitCompare(4);
    
    for (int iCanv = 0; iCanv < 4; ++iCanv) {
        vecCanvases_WeightedPunziLimitCompare[iCanv] = new TCanvas(vecCanvNames_PunziLimitCompare[iCanv] + addName, vecCanvNames_PunziLimitCompare[iCanv] + addName);
        vecWeightedPunziLimitCompare[iCanv]->Draw("hist");
        int currNBins = vecWeightedPunziLimitCompare[iCanv]->GetNbinsX() + 1;
        int ZeroBin = vecWeightedPunziLimitCompare[iCanv]->GetXaxis()->FindBin(0.);
        int OneBin = vecWeightedPunziLimitCompare[iCanv]->GetXaxis()->FindBin(1.);
        int NegOneBin = vecWeightedPunziLimitCompare[iCanv]->GetXaxis()->FindBin(-1.);

        float totalEvents = vecWeightedPunziLimitCompare[iCanv]->Integral(0, currNBins);
        float totalEventsLeftOfZero = vecWeightedPunziLimitCompare[iCanv]->Integral(0, ZeroBin - 1);
        float totalEventsRightOfZero = vecWeightedPunziLimitCompare[iCanv]->Integral(ZeroBin, currNBins);
        float totalEventsLeftOfNegOne = vecWeightedPunziLimitCompare[iCanv]->Integral(0, NegOneBin - 1);
        float totalEventsRightOfOne = vecWeightedPunziLimitCompare[iCanv]->Integral(OneBin, currNBins);
        
        cout << "for iCanv " << iCanv << endl;
        cout << "totalEvents " << totalEvents << endl;
        cout << "total events right of zero " << totalEventsRightOfZero << endl;
        cout << "fraction of correct events: " << totalEventsRightOfZero/totalEvents << endl;
        cout << "total events left of -1 " << totalEventsLeftOfNegOne << endl;
        cout << "fraction of events unambiguously wrong " << totalEventsLeftOfNegOne/totalEvents << endl;
        /*
        cout << "totalEventsLeftOfZero " << totalEventsLeftOfZero << endl;
        cout << "fracOfEventsLeftOfZero " << totalEventsLeftOfZero/totalEvents << endl;
        cout << "totalEventsRightOfZero " << totalEventsRightOfZero << endl;
        cout << "fracOfEventsRightOfZero " << totalEventsRightOfZero/totalEvents << endl;
        cout << "totalEventsLeftOfNegOne " << totalEventsLeftOfNegOne << endl;
        cout << "fracOfEventsLeftOfNegOne " << totalEventsLeftOfNegOne/totalEvents << endl;
        cout << "totalEventsRightOfOne " << totalEventsRightOfOne << endl;
        cout << "fracOfEventsRightOfOne " << totalEventsRightOfOne/totalEvents << endl;
         */
    }
    theApp.Run(retVal);
}