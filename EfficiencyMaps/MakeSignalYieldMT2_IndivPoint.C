/*
#include "HeaderFiles/Hasher.h"
#include "HeaderFiles/Typedefs.h"
*/
#include "../HeaderFiles/StopEfficiencyMapHeaderFiles.h"
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
    
    TString nameDilep = "";
    if (BasicLP.typeDilepChannel > -1) {
        nameDilep += BasicLP.strDilepChannel;
    }
    TString arrWhichFullSel[9] = {"_BaseFullSel_LowBLepMass", "_2BJetsFullSel_LowBLepMass", "_METSig1DFullSel_LowBLepMass", "_METSigTrueFullSel_LowBLepMass", "_ZCR_LowBLepMass",  "_0BJetsFullSel_LowBLepMass", "", "", "_2BJetsFullSel"};
    TString nameTTBarFile = "../LimitSetting/ShapeAnalysis/Backgrounds/TTBar";
    TString nameTTBarDims[3] = {"_One", "_Two", "_Three"};
    
    bool doVerb = 0;
    
    for (int k = 0; k < argc; ++k) {
        if (strncmp (argv[k],"doVerb", 6) == 0) {
            doVerb = 1;
        }
    }
    
    SUSYMLP.SetMassStrings();

    
    nameTTBarFile += arrWhichFullSel[BasicLP.typeFullSel];
    nameTTBarFile += nameDilep;
    nameTTBarFile += nameTTBarDims[BasicLP.numDims - 1];
    nameTTBarFile += "Dee.root";
    TFile * fileTTBar;
    if (BasicLP.useSigContam) {
        cout << "going to open " << nameTTBarFile << " for use in signal contamination calculations" << endl;
        fileTTBar = TFile::Open(nameTTBarFile);
    }
    
    SignalYieldMaker_IndivPoint SYM_IP;
    SYM_IP.DefaultVals(BasicLP.numDims);
    SYM_IP.InitializeVecs();
    
    for (SUSYMLP.massStop = SUSYMLP.minStopMass; SUSYMLP.massStop <= SUSYMLP.maxStopMass; SUSYMLP.massStop += SUSYMLP.binSize) {
        for (SUSYMLP.massLSP = SUSYMLP.minLSPMass; SUSYMLP.massLSP <= TMath::Min(SUSYMLP.maxLSPMass, SUSYMLP.massStop - 100); SUSYMLP.massLSP += SUSYMLP.binSize) {
            SUSYMLP.PrintCurrMass();
            SUSYMLP.SetMassStrings();
            SYM_IP.SetStringSignal(&SUSYT2LP, &SUSYMLP, &BasicLP, BasicLP.useSigContam);
            SYM_IP.InitializeHistsandOutfile(doVerb);
            massCombo stop(SUSYMLP.massStop, SUSYMLP.massLSP);
            SYM_IP.IterateOverCuts(&stop, doVerb);
            if (BasicLP.useSigContam) {
                if (doVerb) {
                    cout << "about to do signal contamination " << endl;
                }
                SYM_IP.DoSignalContamination(fileTTBar, doVerb);
            }
            SYM_IP.WriteFile(BasicLP.useSigContam);
        }
    }        
    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
