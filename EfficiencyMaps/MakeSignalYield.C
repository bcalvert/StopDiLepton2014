/*
 #include "HeaderFiles/Hasher.h"
 #include "HeaderFiles/Typedefs.h"
 */
#include "../HeaderFiles/BasicFunctions.h"
#include "../HeaderFiles/StopSignalYieldFunctions_Maps.h"
#include "../HeaderFiles/StopSignalYieldFunctions_Efficiency.h"
#include "../HeaderFiles/StopSignalYieldStructs.h"
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
#include <vector>
#include <cmath>

using namespace std;

int main( int argc, char* argv[]) {
    
    gROOT->ProcessLine("#include <vector>");
    // Style things
    gStyle->SetErrorX(0.5); // set X uncertainty to 1/2 of a bin
    gStyle->SetTitleFont(42);
    
    TRint theApp("App", &argc, argv);
    Bool_t retVal = kTRUE;
    
    
    LeptFiltSignalAssociator LFSC;
    CoarseMapMaker CMM;
    CMM.DefaultCuts();
    SmoothMapMaker SMM;
    SMM.DefaultVals();
    
    SignalYieldMaker SYM;
    SYM.DefaultVals();
    
    SUSYT2LimitParams SUSYT2LPs;
    SUSYT2LPs.DefaultVals();
    SUSYT2LPs.SetParamsFromCommandLine(argc, argv);
    SUSYT2LPs.SetStrings();
    SUSYT2LPs.PrintStrings();
    
    bool doVerb = 0;
    
    CMM.SetParamsFromCommandLine(argc, argv);
    SMM.SetParamsFromCommandLine(argc, argv);
    SYM.SetParamsFromCommandLine(argc, argv);
    
    LFSC.DefaultVals(SUSYT2LPs.typeT2);
    LFSC.SetHistAndOutFile(&SUSYT2LPs, true);
    
    CMM.DefaultVals(&LFSC);
    CMM.InitializeVecs();
    CMM.InitializeHistsandOutfile(&SUSYT2LPs, true);
    
    SMM.InitializeHistsandOutfile(&SUSYT2LPs, &CMM, true);
    SMM.SetFile(&CMM);
    
    for (int k = 0; k < argc; ++k) {
        if (strncmp (argv[k],"doVerb", 6) == 0) {
            doVerb = 1;
        }
    }
    
    if (doVerb) {
        cout << "going to initialize hists and out file " << endl;
    }
    SYM.InitializeHistsandOutfile(&SUSYT2LPs, &CMM, false, false, doVerb);
    if (doVerb) {
        cout << "going to read smooth maps " << endl;
    }
    SYM.ReadSmoothMaps(&SMM);
    if (doVerb) {
        cout << "going to load XSec " << endl;
    }
    SYM.LoadXSec();
    if (doVerb) {
        cout << "going to make yield hists " << endl;
    }
    SYM.MakeYieldHists(doVerb);
    if (doVerb) {
        cout << "going to write file " << endl;
    }
    SYM.WriteFile();
    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
