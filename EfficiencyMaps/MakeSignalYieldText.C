/*
 #include "HeaderFiles/Hasher.h"
 #include "HeaderFiles/Typedefs.h"
 */
#include "../HeaderFiles/StopEfficiencyMapHeaderFiles.h"
#include "../HeaderFiles/LimitScriptHeaderFiles/LimitFunctions_Saving.h"
#include "../HeaderFiles/LimitScriptHeaderFiles/YieldCalculationFunctions.h"

#include <iostream>
#include "TTree.h"
#include "TMath.h"
#include "TFile.h"
#include "TH3F.h"
#include "TH2F.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TString.h"
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
    
    LimitParametersContainer BasicLPs;
    BasicLPs.DefaultVals();
    BasicLPs.SetParamsFromCommandLine(argc, argv);
    BasicLPs.SetStrings();


    SUSYT2LimitParams SUSYT2LPs;
    SUSYT2LPs.DefaultVals();
    SUSYT2LPs.SetParamsFromCommandLine(argc, argv);
    SUSYT2LPs.SetStrings();
    SUSYT2LPs.PrintStrings();
    
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
    
    SignalYieldPlotter SYP;
    SYP.DefaultVals(whichPlotType);
    SYP.InitializeVecs();
    
    CMM.SetParamsFromCommandLine(argc, argv);
    SMM.SetParamsFromCommandLine(argc, argv);
    SYM.SetParamsFromCommandLine(argc, argv);
    SYP.SetParamsFromCommandLine(argc, argv);
    
    LFSC.DefaultVals(SUSYT2LPs.typeT2);
    LFSC.SetHistAndOutFile(&SUSYT2LPs, true);
    
    CMM.DefaultVals(&LFSC);
    CMM.InitializeVecs();
    CMM.InitializeHistsandOutfile(&SUSYT2LPs, &BasicLPs, true);
    
    SMM.InitializeHistsandOutfile(&SUSYT2LPs, &CMM, &BasicLPs, true);
    SMM.SetFile(&CMM);
    if (doVerb) {
        cout << "going to initialize Hists for SYM Signal " << endl;
    }
    SYM.InitializeHistsandOutfile(&SUSYT2LPs, &CMM, &BasicLPs, true, false, doVerb);
    
    MT2LimitParams MT2LPs;
    MT2LPs.DefaultVals();
    
//    TString pathYieldText = "EfficiencyMaps/" + SUSYT2LPs.SavingName(true) + SetSMSStringAppend(4);
    TString pathYieldText = SUSYT2LPs.SavingName(true) + SetSMSStringAppend(4);
    
    System_MakeDir(pathYieldText);
    
    TString nameYieldFile;
    
    TString basicCommand = "./DrawSignalStuff";
    vector<TString> vecComArgs(0);
    vecComArgs.push_back(" T2SMS ");
    vecComArgs[0] += SUSYT2LPs.typeT2;
    vecComArgs.push_back(" indexPol ");
    vecComArgs[1] += SUSYT2LPs.indexPol;
    vecComArgs.push_back(" charFrac ");
    vecComArgs[2] += SUSYT2LPs.charFrac;
    vecComArgs.push_back("");
    vecComArgs.push_back(" controlCuts ");
    vecComArgs[4] += MT2LPs.contMT2ll;
    vecComArgs[4] += " ";
    vecComArgs[4] += MT2LPs.contMT2lblb;
    vecComArgs[4] += " ";
    vecComArgs[4] += -1;
    vecComArgs.push_back(" noDraw ");
    vecComArgs.push_back(" -b -q");
    if (CMM.doControl) {
        vecComArgs.push_back(" doControl");
    }
    if (BasicLPs.useSigContam) {
        vecComArgs.push_back(" doSC");
    }
    vecComArgs.push_back(" >> ");
    vecComArgs.push_back("");

    /*
    ${strMacroCommand} T2SMS ${isT2tt} indexPol ${indexPol} charFrac ${charFrac} cuts ${MT2llCut} ${MT2lblbCut} ${MT2bb_ZMETCut} -b -q
    
    ./DrawSignalStuff typeSMS ${inTypeSMS} paramSMS ${inParamSMS} -b -q cuts ${inMT2llCut} ${inMT2lblbCut} -1 noDraw ${strSC} controlCuts 80 0 -1 >> ${outName}
    */
    
    for (int currLBMT2ll = MT2LPs.minMT2ll; currLBMT2ll <= MT2LPs.maxMT2ll; currLBMT2ll += MT2LPs.bigStepSizeMT2ll) {
        int currUBMT2ll = TMath::Min(MT2LPs.maxMT2ll, currLBMT2ll + (MT2LPs.bigStepSizeMT2ll - MT2LPs.smallStepSizeMT2ll));
        for (int currLBMT2lblb = MT2LPs.minMT2lblb; currLBMT2lblb <= MT2LPs.maxMT2lblb; currLBMT2lblb += MT2LPs.bigStepSizeMT2lblb) {
            int currUBMT2lblb = TMath::Min(MT2LPs.maxMT2lblb, currLBMT2lblb + (MT2LPs.bigStepSizeMT2lblb - MT2LPs.smallStepSizeMT2lblb));
            for (MT2LPs.cutMT2ll = currLBMT2ll; MT2LPs.cutMT2ll <= currUBMT2ll; MT2LPs.cutMT2ll += MT2LPs.smallStepSizeMT2ll) {
                for (MT2LPs.cutMT2lblb = currLBMT2lblb; MT2LPs.cutMT2lblb <= currUBMT2lblb; MT2LPs.cutMT2lblb += MT2LPs.smallStepSizeMT2lblb) {
                    TString cutString = " cuts ";
                    cutString += MT2LPs.cutMT2ll;
                    cutString += " ";
                    cutString += MT2LPs.cutMT2lblb;
                    cutString += " ";
                    cutString += -1;
                    vecComArgs[3] = cutString;
                    

                    MT2LPs.SetMT2AppendString(false);
                    
                    nameYieldFile = NameYield_Signal(BasicLPs.useSigContam, CMM.doControl, &BasicLPs, &MT2LPs, &SUSYT2LPs);
                    vecComArgs[vecComArgs.size() - 1] = pathYieldText + nameYieldFile;
                    System_RemoveFile(pathYieldText + nameYieldFile);
                    //cout << "going to save to " << pathYieldText + nameYieldFile << endl;
                    System_GeneralCommand(basicCommand, &vecComArgs, true);
                    System_GeneralCommand(basicCommand, &vecComArgs, false);
                }
            }
        }
    }
    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
