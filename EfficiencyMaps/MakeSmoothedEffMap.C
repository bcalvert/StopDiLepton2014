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
    
    SmoothMapMaker SMM_TightBin;
    CoarseMapMaker CMM_TightBin;
    SUSYT2LimitParams SUSYT2LPs_TightBin;
    
    SUSYT2LimitParams SUSYT2LPs;
    SUSYT2LPs.DefaultVals();
    SUSYT2LPs.SetParamsFromCommandLine(argc, argv);
    SUSYT2LPs.SetStrings();
    SUSYT2LPs.PrintStrings();

    LimitParametersContainer BasicLPs;
    BasicLPs.DefaultVals();
    BasicLPs.SetParamsFromCommandLine(argc, argv);
    BasicLPs.SetStrings();
    BasicLPs.PrintStrings();
    
    bool doVerb = 0;
    bool doDraw = 0;
    for (int k = 0; k < argc; ++k) {
        if (strncmp (argv[k],"doVerb", 6) == 0) {
            doVerb = 1;
        }
        if (strncmp (argv[k],"doDraw", 6) == 0) {
            doDraw = 1;
        }
    }
    
    CMM.SetParamsFromCommandLine(argc, argv);
    SMM.SetParamsFromCommandLine(argc, argv);
    
    LFSC.DefaultVals(SUSYT2LPs.typeT2, &BasicLPs);
    LFSC.SetHistAndOutFile(&SUSYT2LPs, true);
    
    CMM.DefaultVals(&LFSC, &BasicLPs);
    CMM.InitializeVecs();
    CMM.InitializeHistsandOutfile(&SUSYT2LPs, true, SUSYT2LPs.typeT2 != 2);



    
//    if (doVerb) {
//    }
    if (SUSYT2LPs.typeT2 == 2) {
        SMM.numRebins--;
    }
    SMM.SetFile(&CMM);
    SMM.PrintBasic();
    SMM.DoSmoothing(&CMM, doVerb);
    SMM.DoInterpolation(doVerb);
    
    bool doCombo_T2SMS = true;
    if (SUSYT2LPs.typeT2 == 1 && doCombo_T2SMS) {
        cout << "now adding in Tight Bin guys' information " << endl;
        //Hack to add in Tight bin guys into the information
        SUSYT2LPs_TightBin = SUSYT2LPs;
        SUSYT2LPs_TightBin.typeT2 = 2;
        
//        CMM_TightBin.SetParamsFromCommandLine(argc, argv);
        //        CMM_TightBin.DefaultVals(&LFSC, &BasicLPs);
        //        CMM_TightBin.InitializeVecs();

        CMM_TightBin = CMM;
        CMM_TightBin.InitializeHistsandOutfile(&SUSYT2LPs_TightBin, true, false);
        
        //SMM_TightBin.InitializeHistsandOutfile(&SUSYT2LPs_TightBin, &CMM_TightBin, &BasicLPs, false);
        
        SMM_TightBin.DefaultVals();
        SMM_TightBin.SetParamsFromCommandLine(argc, argv);
        SMM_TightBin.numRebins = SMM.numRebins - 1;
        SMM_TightBin.SetFile(&CMM_TightBin);
        SMM_TightBin.PrintBasic();
        cout << "about to do smoothing of tight bin stuff " << endl;
        //doVerb = true;
        SMM_TightBin.DoSmoothing(&CMM_TightBin, doVerb);
        cout << "about to do interpolation of tight bin stuff " << endl;
        //doVerb = false;
        SMM_TightBin.DoInterpolation(doVerb);
        //SMM_TightBin.WriteFile(false);
        cout << "about to combine tight bin stuff with main stuff " << endl;
        SMM.CombineSmoothEffs(&SMM_TightBin, doVerb);
    }
    /*
    for (int iVec = 0; iVec < SMM.vecInterpHist.size(); ++iVec) {
        cout << "iVec: " << iVec << " is " << SMM.vecInterpHist[iVec]->GetName() << endl;
    }
     */
    SMM.InitializeHistsandOutfile(&SUSYT2LPs, &CMM, false);
    SMM.WriteFile();
    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
