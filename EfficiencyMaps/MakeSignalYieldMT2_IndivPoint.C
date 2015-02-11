/*
#include "HeaderFiles/Hasher.h"
#include "HeaderFiles/Typedefs.h"
*/
#include "../HeaderFiles/BasicFunctions.h"
#include "../HeaderFiles/StopSignalYieldFunctions_Maps.h"
#include "../HeaderFiles/StopSignalYieldFunctions_Efficiency.h"
#include "../HeaderFiles/StopSignalYieldStructs.h"
#include "../HeaderFiles/LimitScriptHeaderFiles/LimitScripts_ParametersStruct.h"
#include <iostream>
#include "TTree.h"
#include "TMath.h"
#include "TFile.h"
#include "TH3F.h"
#include "TH2F.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TRint.h"
#include "TString.h"
#include <vector>
#include <cmath>

using namespace std;

typedef pair<int, int> massCombo;

int main( int argc, char* argv[]) {        
    
    gROOT->ProcessLine("#include <vector>");
    // Style things
    gStyle->SetErrorX(0.5); // set X uncertainty to 1/2 of a bin
    gStyle->SetTitleFont(42);
    
    TRint theApp("App", &argc, argv);
    Bool_t retVal = kTRUE;
    
    TString nameTTBarFile = "LimitSetting/ShapeAnalysis/Backgrounds/TTBar";
    TString nameTTBarDims[3] = {"One", "Two", "Three"};
    
    SUSYT2LimitParams SUSYT2LP;
    SUSYT2LP.DefaultVals();
    SUSYT2LP.SetParamsFromCommandLine(argc, argv);
    SUSYT2LP.SetStrings();
    SUSYT2LP.PrintStrings();
    
    SUSYMassLimitParams SUSYMLP;
    SUSYMLP.DefaultVals();
    SUSYMLP.PrintVals();
    SUSYMLP.SetParamsFromCommandLine(argc, argv);
    
    LimitParametersContainer BasicLP;
    BasicLP.DefaultVals();
    BasicLP.numDims = 1;
    BasicLP.SetParamsFromCommandLine(argc, argv);
    BasicLP.SetStrings();
    BasicLP.PrintStrings();
    
    bool doVerb = 0;
    
    for (int k = 0; k < argc; ++k) {
        if (strncmp (argv[k],"doVerb", 6) == 0) {
            doVerb = 1;
        }
    }
    
    SUSYMLP.SetMassStrings();
    
    nameTTBarFile += nameTTBarDims[BasicLP.numDims - 1];
    nameTTBarFile += "Dee.root";
    TFile * fileTTBar;
    if (BasicLP.useSigContam) {
        fileTTBar = TFile::Open(nameTTBarFile);
    }
    massCombo stop(SUSYMLP.massStop, SUSYMLP.massLSP);
    
    SignalYieldMaker_IndivPoint SYM_IP;
    SYM_IP.DefaultVals(BasicLP.numDims);
    SYM_IP.InitializeVecs();
    SYM_IP.SetStringSignal(&SUSYT2LP, &SUSYMLP, BasicLP.useSigContam);
    SYM_IP.InitializeHistsandOutfile(doVerb);
    SYM_IP.IterateOverCuts(&stop, doVerb);
    if (BasicLP.useSigContam) {
        if (doVerb) {
            cout << "about to do signal contamination " << endl;
        }
        SYM_IP.DoSignalContamination(fileTTBar, doVerb);
    }
    SYM_IP.WriteFile();
    
    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
