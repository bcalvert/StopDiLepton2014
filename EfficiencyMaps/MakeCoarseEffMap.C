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
    
    SUSYT2LimitParams SUSYT2LPs;
    SUSYT2LPs.DefaultVals();
    SUSYT2LPs.SetParamsFromCommandLine(argc, argv);
    SUSYT2LPs.SetStrings();
    SUSYT2LPs.PrintStrings();
    
    float paramSMST2bw = 0;
    
    bool doVerb = 0;
    
    for (int k = 0; k < argc; ++k) {
        if (strncmp (argv[k],"doVerb", 6) == 0) {
            doVerb = 1;
        }
    }
    CMM.SetParamsFromCommandLine(argc, argv);
    
    
    LFSC.DefaultVals(SUSYT2LPs.typeT2);
    LFSC.SetHistAndOutFile(&SUSYT2LPs, true);
    
    CMM.DefaultVals(&LFSC);
    CMM.InitializeVecs();
//    CMM.InitializeHistsandOutfile(SUSYT2LPs.typeT2, paramSMS, false);
    CMM.InitializeHistsandOutfile(&SUSYT2LPs, false);
    
    massMap mapMassT2bw;
    massMap mapMassT2tt;
    
    massMap * massMapToUse;
    
    SetMassMapT2tt(mapMassT2tt, 1);
    
    SetMassMapT2bw(mapMassT2bw, SUSYT2LPs.charFrac);
    
    massCombo currCombo;
    
    labelMap mapT2bw, mapT2bwLSP0;
    labelMap mapT2tt, mapT2ttLSP0;
    labelMap mapT2tt_v2, mapT2ttLSP0_v2, mapT2ttStop350_v2, mapT2ttStop350LSP0_v2;
    
    SetLeptFiltSFMapT2bw(mapT2bw, false);
    SetLeptFiltSFMapT2bw(mapT2bwLSP0, true);
    
    SetLeptFiltSFMapT2tt_ver2(mapT2tt_v2, false, false);
    SetLeptFiltSFMapT2tt_ver2(mapT2ttLSP0_v2, true, false);
    SetLeptFiltSFMapT2tt_ver2(mapT2ttStop350_v2, false, true);
    SetLeptFiltSFMapT2tt_ver2(mapT2ttStop350LSP0_v2, true, true);
    
    SetLeptFiltSFMapT2tt(mapT2tt, false);
    SetLeptFiltSFMapT2tt(mapT2ttLSP0, true);
    
    labelMap * mapToUse;
    
    
    int stopLB = SUSYT2LPs.typeT2 ? 100 : 100;
    int stopUB = SUSYT2LPs.typeT2 ? 1000 : 800;
    //int stopLB = SUSYT2LPs.typeT2 ? 550 : 550;
    //int stopUB = SUSYT2LPs.typeT2 ? 550 : 550;
//    int stopUB = 100;
    int LSPLB = SUSYT2LPs.typeT2 ? 0 : 0;
    //int LSPUB = SUSYT2LPs.typeT2 ? 0 : 0;
    int LSPUB = SUSYT2LPs.typeT2 ? 900 : 700;
    
    int stepSize = 25;
    
    //CMM.PrintBasic(SUSYT2LPs.typeT2, paramSMS);
    CMM.LoadXSec();
    
    for (int massStop = stopLB; massStop <= stopUB; massStop += stepSize) {
        for (int massLSP = LSPLB; massLSP <= TMath::Min(LSPUB, massStop - 100); massLSP += stepSize) {
            cout << endl;
            cout << "massStop:massLSP " << massStop << ":" << massLSP << endl;
            if (SUSYT2LPs.typeT2 == 0) {
                mapToUse = massLSP == 0 ? &mapT2bwLSP0 : &mapT2bw;
                massMapToUse = &mapMassT2bw;
            }
            else if (SUSYT2LPs.typeT2 == 1) {
                mapToUse = massLSP == 0 ? &mapT2ttLSP0 : &mapT2tt;
                massMapToUse = &mapMassT2tt;
            }
            else {
                if (massStop == 350) {
                    mapToUse = massLSP == 0 ? &mapT2ttStop350LSP0_v2 : &mapT2ttStop350_v2;
                }
                else {
                    mapToUse = massLSP == 0 ? &mapT2ttLSP0_v2 : &mapT2tt_v2;
                }
            }
            
             LFSC.SetFiles(massStop, massLSP, &SUSYT2LPs, mapToUse, doVerb);
            
            
            if (!SUSYT2LPs.typeT2) {
                paramSMST2bw = SUSYT2LPs.charFrac  * (1.0 / 100);
            }
            else {
                paramSMST2bw = -1;
            }
	    //cout << "about to set files " << endl;
            CMM.SetFiles(massStop, massLSP, &SUSYT2LPs, massMapToUse, doVerb);
            CMM.SetEff(&LFSC, massStop, massLSP, paramSMST2bw, SUSYT2LPs.typeT2, doVerb);
            CMM.SetBin(massStop, massLSP);
            
	    //            LFSC.CloseFiles();
            CMM.CloseFiles();
            cout << "CMM eff " << CMM.effCV << endl;
        }
    }
    
    CMM.DoOverflow();
    
    CMM.WriteFile();
    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
