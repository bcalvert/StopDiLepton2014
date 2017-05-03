/*
 #include "HeaderFiles/Hasher.h"
 #include "HeaderFiles/Typedefs.h"
 */
/*
#include "../HeaderFiles/BasicFunctions.h"
#include "../HeaderFiles/BasicFunctions_Math.h"
#include "../HeaderFiles/BasicFunctions_HistogramMaking.h"
#include "../HeaderFiles/StopSignalYieldFunctions_Maps.h"
#include "../HeaderFiles/StopSignalYieldFunctions_Efficiency.h"
#include "../HeaderFiles/LimitScriptHeaderFiles/LimitScripts_ParametersStruct.h"
#include "../HeaderFiles/StopSignalYieldStructs.h"
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
#include "TColor.h"
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
    
    SignalYieldMaker SYM, SYM_Control;
    SYM.DefaultVals();
    


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
    
    int whichPlotType = 0;
    bool doVerb = 0;
    
    for (int k = 0; k < argc; ++k) {
        if (strncmp (argv[k],"doVerb", 6) == 0) {
            doVerb = 1;
        }
        else if (strncmp (argv[k],"-wpt", 4) == 0) {
            whichPlotType = strtol(argv[k+1], NULL, 10);
        }
    }
    
    cout << "whichPlotType " << whichPlotType << endl;
    
    SignalYieldPlotter SYP;
    //SYP.DefaultVals(whichPlotType, &SUSYT2LPs);
    SYP.DefaultVals(whichPlotType);
    SYP.InitializeVecs(SUSYT2LPs.typeT2);
    
    CMM.SetParamsFromCommandLine(argc, argv);
    SMM.SetParamsFromCommandLine(argc, argv);
    SYM.SetParamsFromCommandLine(argc, argv);
    SYP.SetParamsFromCommandLine(argc, argv);
    
    LFSC.DefaultVals(SUSYT2LPs.typeT2, &BasicLPs);
    LFSC.SetHistAndOutFile(&SUSYT2LPs, true);
    
    CMM.DefaultVals(&LFSC, &BasicLPs);
    CMM.InitializeVecs();
    CMM.InitializeHistsandOutfile(&SUSYT2LPs, true, SUSYT2LPs.typeT2 != 2);
    
    if (SUSYT2LPs.typeT2 != 2) {
        SMM.InitializeHistsandOutfile(&SUSYT2LPs, &CMM, true);
        SMM.SetFile(&CMM);
    }
    SYM_Control = SYM;
    if (SUSYT2LPs.typeT2 != 2) {
        if (doVerb) {
            cout << "going to initialize Hists for SYM Signal " << endl;
        }
        SYM.InitializeHistsandOutfile(&SUSYT2LPs, &CMM, true, false, doVerb);
        if (doVerb) {
            cout << "going to initialize Hists for SYM Control " << endl;
        }
        SYM_Control.InitializeHistsandOutfile(&SUSYT2LPs, &CMM, true, true, doVerb);
    }
    if (SYM.doSigContam) {
        TString nameTempFile = "tempSigContam_T2";
        nameTempFile += SUSYT2LPs.typeT2 ? "tt" : "bw_CharFrac0p";
        if (!SUSYT2LPs.typeT2) {
            nameTempFile += SUSYT2LPs.charFrac;
        }
        nameTempFile += "_IndexPol";
        nameTempFile += SUSYT2LPs.indexPol;
        nameTempFile += "_MT2ll";
        nameTempFile += CMM.MT2llCut;
        nameTempFile += "_MT2lblb";
        nameTempFile += CMM.MT2lblbCut;
        nameTempFile += "_MT2bb_ZMET";
        nameTempFile += CMM.MT2bb_ZMETCut;
        nameTempFile += ".txt";
        
        SYM.nameFileTTBarYieldControl = "../LimitSetting/DileptonYields/Backgrounds/NormalFullSelection/ControlRegion/ANDCUT/TrimOutBackgrounds_ANDCut_MT2ll";
        SYM.nameFileTTBarYieldControl += CMM.cutMT2ll_Control;
        SYM.nameFileTTBarYieldControl += "_MT2lblb";
        SYM.nameFileTTBarYieldControl += CMM.cutMT2lblb_Control;
        SYM.nameFileTTBarYieldControl += ".txt";

        SYM.nameFileTTBarYieldSignal = "../LimitSetting/DileptonYields/Backgrounds/NormalFullSelection/SignalRegion/ANDCUT/TrimOutBackgrounds_ANDCut_MT2ll";
        SYM.nameFileTTBarYieldSignal += CMM.MT2llCut;
        SYM.nameFileTTBarYieldSignal += "_MT2lblb";
        SYM.nameFileTTBarYieldSignal += CMM.MT2lblbCut;
        SYM.nameFileTTBarYieldSignal += ".txt";
        
        if (BasicLPs.typeDilepAna > 0) {
            TString arrWhichFullSel[4] = {"_BaseFullSel_LowBLepMass", "_2BJetsFullSel_LowBLepMass", "_METSig1DFullSel_LowBLepMass", "_METSigTrueFullSel_LowBLepMass"};
            SYM.nameFileTTBarYieldControl = TString("./TTBarShapeControl") + arrWhichFullSel[BasicLPs.typeFullSel] + TString(".txt");
        }
        
        if (doVerb) {
            cout << "about to set Sig Contam Yields";
        }
        SYM.SetSigContamYields(nameTempFile, doVerb);
    }
    
    if (SYP.doDraw) {
//        if (!SYM.doSigContam) {
        if (true) {
            int NRGBs = 5;
            int NCont = 255;
            
            double arrayStops[5] = {0.00, 0.34, 0.61, 0.84, 1.00};
            double arrayReds[5] = {0.50, 0.50, 1.00, 1.00, 1.00};
            double arrayGreens[5] = {0.50, 1.00, 1.00, 0.60, 0.50};
            double arrayBlues[5] = {1.00, 1.00, 0.50, 0.40, 0.50};
            
            //int colorTable =
            TColor::CreateGradientColorTable(NRGBs, arrayStops, arrayReds, arrayGreens, arrayBlues, NCont);
            gStyle->SetNumberContours(NCont);
        }
        gStyle->SetOptTitle(1);
        gROOT->ForceStyle();
        if (doVerb) {
            cout << "about to draw! " << endl;
        }
        SYP.Draw(&LFSC, &CMM, &SMM, &SYM, &SUSYT2LPs, doVerb); //Need to do this!
        gStyle->SetOptTitle(1);
    }
    else if (SYP.callLatexSedScript) {
        CoarseMapMaker * inCMM = SYP.whichPlotType > 0 ? &CMM : NULL;
        SYP.SetCanvasAddName(&SUSYT2LPs, inCMM);
        SYP.SetStringsOutLatex(&CMM, &SUSYT2LPs);
        SYP.CallLatexSedScript(doVerb);
    }
    else if (SYP.showYields) {
        SYM.PrintYields(SYP.doAverage, doVerb, &SYM_Control);
    }
    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
