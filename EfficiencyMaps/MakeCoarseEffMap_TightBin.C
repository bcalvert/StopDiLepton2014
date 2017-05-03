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
    
    SUSYT2LimitParams SUSYT2LPs;
    SUSYT2LPs.DefaultVals();
    SUSYT2LPs.SetParamsFromCommandLine(argc, argv);
    SUSYT2LPs.SetStrings();
    SUSYT2LPs.typeT2 = 2;
    SUSYT2LPs.PrintStrings();
    
    LimitParametersContainer BasicLPs;
    BasicLPs.DefaultVals();
    BasicLPs.SetParamsFromCommandLine(argc, argv);
    BasicLPs.SetStrings();
    BasicLPs.PrintStrings();
    
    float paramSMST2bw = 0;
    
    int doVerb = 0;
    
    for (int k = 0; k < argc; ++k) {
        if (strncmp (argv[k],"doVerb", 6) == 0) {
            doVerb = 1;
        }
        else if (strncmp (argv[k],"levelVerb", 9) == 0) {
            doVerb = strtol(argv[k + 1], NULL, 10);
        }
    }
    CMM.SetParamsFromCommandLine(argc, argv);
    
    
    LFSC.DefaultVals(SUSYT2LPs.typeT2, &BasicLPs);
    LFSC.SetHistAndOutFile(&SUSYT2LPs, true);

    CMM.DefaultVals(&LFSC, &BasicLPs);
//    CMM.InitializeHistsandOutfile(SUSYT2LPs.typeT2, paramSMS, false);
    CMM.InitializeHistsandOutfile(&SUSYT2LPs, false, false);
    
    massMap mapMassT2tt;
    
    SetMassMapT2ttTightBin(mapMassT2tt);
    
    massCombo currCombo;
    
    labelMap mapT2tt_v2, mapT2ttLSP0_v2, mapT2ttStop350_v2, mapT2ttStop350LSP0_v2;
    
    SetLeptFiltSFMapT2tt_ver2(mapT2tt_v2, false, false);
    SetLeptFiltSFMapT2tt_ver2(mapT2ttLSP0_v2, true, false);
    SetLeptFiltSFMapT2tt_ver2(mapT2ttStop350_v2, false, true);
    SetLeptFiltSFMapT2tt_ver2(mapT2ttStop350LSP0_v2, true, true);
    
    labelMap * mapToUse;
    
    
    int stopLB = 1500;
    //int stopUB = 4500;
    int stopUB = 4000;
    
    int LSPLB = 0;
//    int LSPUB = 3000;
    int LSPUB = 2500;
    
    int stepSize = 125;
    
    //CMM.PrintBasic(SUSYT2LPs.typeT2, paramSMS);
    CMM.LoadXSec();
    
    int minMassDiff = 1500, maxMassDiff = 2000;
    
    for (int massStop = stopLB; massStop <= stopUB; massStop += stepSize) {
        for (int massLSP = TMath::Max(LSPLB, massStop - maxMassDiff); massLSP <= TMath::Min(LSPUB, TMath::Max(0, massStop - minMassDiff)); massLSP += stepSize) {
            cout << endl;
            cout << "massStop:massLSP " << massStop/10 << ":" << massLSP/10 << endl;

            if (massStop == 3500) {
                mapToUse = massLSP == 0 ? &mapT2ttStop350LSP0_v2 : &mapT2ttStop350_v2;
            }
            else {
                mapToUse = massLSP == 0 ? &mapT2ttLSP0_v2 : &mapT2tt_v2;
            }
            
            LFSC.SetFiles(massStop/10, massLSP/10, &SUSYT2LPs, mapToUse, doVerb);

            paramSMST2bw = -1;
            if (doVerb) {
                cout << "about to set efficiency for (possibly) lept-filter" << endl;
            }
            CMM.SetFiles(massStop/10, massLSP/10, &SUSYT2LPs, &mapMassT2tt, doVerb);
            CMM.SetEff(&LFSC, massStop/10, massLSP/10, paramSMST2bw, SUSYT2LPs.typeT2, (BasicLPs.typeFullSel > 3 && BasicLPs.typeFullSel < 6), doVerb);
            CMM.SetBin(massStop/10, massLSP/10);
            //            LFSC.CloseFiles();
            CMM.CloseFiles();
            LFSC.CloseFiles();
            cout << "Pre-addition of non-lept Filter, CMM eff " << CMM.effCV << " +- " << CMM.errEffCV << endl;
        }
    }
    CMM.SetMCStatEff(doVerb > 1);
    CMM.DoOverflow();
    
    CMM.WriteFile();
    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
