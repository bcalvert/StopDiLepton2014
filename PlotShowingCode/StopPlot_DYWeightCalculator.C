#include <iostream>
#include "TMath.h"
#include "../HeaderFiles/StopPlottingHeaderFiles.h"
/*
 #include "../HeaderFiles/BasicFunctions.h"
 #include "../HeaderFiles/HistogramSystematics2.h"
 #include "../HeaderFiles/HistogramSystematicsStructs.h"
 #include "../HeaderFiles/StopStructDefinitions.h"
 #include "../HeaderFiles/StopFunctionDefinitions_Plots.h"
 #include "../HeaderFiles/GeneralPlotDrawingStructs.h"
 #include "../HeaderFiles/GeneralPlotDrawingFunctions.h"
 #include "../HeaderFiles/PlotDisplayStructs.h"
 #include "../HeaderFiles/GeneralPlotDrawingFunctions_Set2.h"
 #include "../HeaderFiles/StopPlotStructs_PlottingMacro.h"
 #include "../HeaderFiles/StopPlotSetup_FileSetup.h"
 #include "../HeaderFiles/StopPlotSetup_CustomPlots.h"
 #include "../HeaderFiles/StopPlotStructs.h"
 #include "../HeaderFiles/StopPlotSetup_ISPIFunctions_Set2.h"
 #include "../HeaderFiles/StopPlotSetup.h"
 */

#include "TH1F.h"
#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TRint.h"
#include <vector>
#include <cmath>
using namespace std;

typedef pair<int, int> intBounds;
typedef pair<TString, intBounds> indMCParams;
using namespace std;

//using namespace std;
int main( int argc, char* argv[]) {
    gROOT->ProcessLine("#include <vector>");
    // Style things
    gStyle->SetErrorX(0.5); // set X uncertainty to 1/2 of a bin
    gStyle->SetTitleFont(42);        
    //    gStyle->SetLegendFont(42);
    
    //./StopPlot_TTBarWeightCalculator doPURW wTTbarGen 2 wNTuple 0 doReReco JsSm 0
    
    
    TRint theApp("App", &argc, argv);
    Bool_t retVal = kTRUE;
    
    RunParams RP;
    WeightCalculators WC;
    PlotMakingStructs PMS;
    AncillaryDrawingVariables ADV;    
    GlobalHistPlotInfo GHPI;
    HistDisplayParams HDP;
    
    SetupPlotRunning_Part1(argc, argv, &RP, &WC, &PMS, &ADV, &GHPI, &HDP);
    bool doVerb = RP.HPM.doVerbosity;
    HistogramDisplayStructs HDS_Data;
    HistogramDisplayStructs HDS_MC;
    SetupPlotRunning_DataMC(&RP, &WC, &PMS, &HDS_Data, &HDS_MC, doVerb);
    
    TString multiChannelCompOutputName;    
    RP.SLS.SetupMultiHist_MultiChanLoading(&PMS.multiHistListIDs, PMS.vecVecHistTtoUse[0], &PMS.multiChannelIDs, PMS.subSampVec, multiChannelCompOutputName);
    
    TString nameTTBarNormHist_Data, nameTTBarNormHist_MC;
    TString appendString = RP.SLS.noType0 ? "_noType0" : "";
//    appendString += "_FullCut";
    
    nameTTBarNormHist_Data = "h_MT2ll";
    nameTTBarNormHist_Data += appendString;
    nameTTBarNormHist_MC = "h_SmearMT2ll";
    nameTTBarNormHist_MC += appendString;
    
    vector<TString> vecGrabNames_Data, vecGrabNames_MC;
    TString nameGrabData, nameGrabMC;
    for (unsigned int iSamp = 0; iSamp < PMS.multiChannelIDs.size(); ++iSamp) {
        nameGrabData = nameTTBarNormHist_Data;
        nameGrabData += PMS.subSampVec->at(PMS.multiChannelIDs[iSamp]).histNameSuffix;
        nameGrabMC = nameTTBarNormHist_MC;
        nameGrabMC += PMS.subSampVec->at(PMS.multiChannelIDs[iSamp]).histNameSuffix;
        vecGrabNames_Data.push_back(nameGrabData);
        vecGrabNames_MC.push_back(nameGrabMC);
    }
    
    HDS_Data.GrabCentralValues(&vecGrabNames_Data, doVerb);
    HDS_MC.GrabCentralValues(&vecGrabNames_MC, doVerb);
    HDS_MC.GrabSystValues(&vecGrabNames_MC, &PMS.vecSystNames, doVerb);
    
    HDS_Data.DoProjection(&PMS.vecIndMCParams, &RP.API, &HDP, "DataComp", false, doVerb);
    HDS_MC.DoProjection(&PMS.vecIndMCParams, &RP.API, &HDP, "MCComp", false, doVerb);
    
    TString weightName = "_DY";
    weightName += "_ZMassBVetoCR";
    SpecificWeight SW_TTBarNorm;
    SW_TTBarNorm.SetHistDefault(weightName, &PMS.vecSystNames);
    TString outFileName = "ScaleFactors";
    outFileName += weightName;
    outFileName += RP.SLS.TTBarString();
    if (!RP.SLS.doExcSamps) outFileName += "_noExcSamps";
    outFileName += ".root";
    TFile * outFile = new TFile(outFileName, "RECREATE");
    
    //    cout << "running with TTBar Generator " << RP.SLS.TTBarString() << endl;
    float SF;
    vector<TString> vecNamesToHave; 
    vecNamesToHave.push_back("ZDY");
    
    
    vector<TString> vecNamesNotToHave;
    
    int endBin = HDS_Data.compSamp.first.grabbedHist_TH1F->GetXaxis()->FindBin(79.9);
    cout << "endBin " << endBin << endl;
    SF = WC.CalcTTBarNormWeight(&vecNamesToHave, &vecNamesNotToHave, &HDS_Data, &HDS_MC, 0, endBin);
    SW_TTBarNorm.SetSF(SF, 0);
    //    cout << "SF " << SF << endl;
    //    cout << endl;
    for (int iSyst = 1; iSyst <= (int) PMS.vecSystNames.size(); ++iSyst) {
        SF = WC.CalcTTBarNormWeight(&vecNamesToHave, &vecNamesNotToHave, &HDS_Data, &HDS_MC, iSyst, endBin);
        SW_TTBarNorm.SetSF(SF, iSyst);
        //        cout << "Up SF for syst = " << vecSystNames[iSyst - 1] << " is " << SF << endl;
        SF = WC.CalcTTBarNormWeight(&vecNamesToHave, &vecNamesNotToHave, &HDS_Data, &HDS_MC, -1 * iSyst, endBin);
        SW_TTBarNorm.SetSF(SF, -1 * iSyst);
        //        cout << "Down SF for syst = " << vecSystNames[iSyst - 1] << " is " << SF << endl;
    }
    SW_TTBarNorm.WriteToFile(outFile);
    outFile->Write();
    outFile->Close();
    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
