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
//    CMM.InitializeVecs(SUSYT2LPs.typeT2 != 3 || (BasicLPs.typeFullSel > 3 && BasicLPs.typeFullSel < 6));
//    CMM.InitializeVecs(SUSYT2LPs.typeT2 != 3);
    CMM.InitializeVecs(true);
    
    CoarseMapMaker CMM_NonLeptFilter = CMM;
//    CMM.InitializeHistsandOutfile(SUSYT2LPs.typeT2, paramSMS, false);
//    CMM.InitializeHistsandOutfile(&SUSYT2LPs, false, SUSYT2LPs.typeT2 != 3 || (BasicLPs.typeFullSel > 3 && BasicLPs.typeFullSel < 6));
//    CMM.InitializeHistsandOutfile(&SUSYT2LPs, false, SUSYT2LPs.typeT2 != 3);
    CMM.InitializeHistsandOutfile(&SUSYT2LPs, false, true);

    bool doLeptFiltAverage = 1;
    
    if (SUSYT2LPs.typeT2 == 3) doLeptFiltAverage = 0;
    if (doLeptFiltAverage) {
//        CMM_NonLeptFilter.InitializeHistsandOutfile(&SUSYT2LPs, -1, SUSYT2LPs.typeT2 != 3 || (BasicLPs.typeFullSel > 3 && BasicLPs.typeFullSel < 6));
//        CMM_NonLeptFilter.InitializeHistsandOutfile(&SUSYT2LPs, -1, SUSYT2LPs.typeT2 != 3);
        CMM_NonLeptFilter.InitializeHistsandOutfile(&SUSYT2LPs, -1, true);
    }
    
    massMap mapMassT2tt, mapMassT2ttNoLeptFilter;
    massMap mapMassT2bw, mapMassT2bwNoLeptFilter;
    massMap mapMassT2tb;
    
    massMap * massMapToUse, * massMapToUseNoLeptFilter;
    
    if (SUSYT2LPs.typeT2 > 0) {
        if (SUSYT2LPs.typeT2 == 1) {
            SetMassMapT2tt(mapMassT2tt, true);
            SetMassMapT2tt(mapMassT2ttNoLeptFilter, false);
        }
        else if (SUSYT2LPs.typeT2 == 3) {
            SetMassMapT2tb(mapMassT2tb);
        }
    }
    else {
        SetMassMapT2bw(mapMassT2bw, SUSYT2LPs.charFrac, true);
        SetMassMapT2bw(mapMassT2bwNoLeptFilter, SUSYT2LPs.charFrac, false);
    }
    
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
    CMM_NonLeptFilter.LoadXSec();
    
    for (int massStop = stopLB; massStop <= stopUB; massStop += stepSize) {
        for (int massLSP = LSPLB; massLSP <= TMath::Min(LSPUB, massStop - 100); massLSP += stepSize) {
            cout << endl;
            cout << "massStop:massLSP " << massStop << ":" << massLSP << endl;
            if (SUSYT2LPs.typeT2 == 0) {
                mapToUse = massLSP == 0 ? &mapT2bwLSP0 : &mapT2bw;
                massMapToUse = &mapMassT2bw;
                massMapToUseNoLeptFilter = &mapMassT2bwNoLeptFilter;
            }
            else if (SUSYT2LPs.typeT2 == 1) {
                mapToUse = massLSP == 0 ? &mapT2ttLSP0 : &mapT2tt;
                massMapToUse = &mapMassT2tt;
                massMapToUseNoLeptFilter = &mapMassT2ttNoLeptFilter;
            }
            else if (SUSYT2LPs.typeT2 == 2) {
                if (massStop == 350) {
                    mapToUse = massLSP == 0 ? &mapT2ttStop350LSP0_v2 : &mapT2ttStop350_v2;
                }
                else {
                    mapToUse = massLSP == 0 ? &mapT2ttLSP0_v2 : &mapT2tt_v2;
                }
                massMapToUse = &mapMassT2tt;
            }
            else if (SUSYT2LPs.typeT2 == 3) {
                massMapToUse = &mapMassT2tb;
            }
            
            if (SUSYT2LPs.typeT2 != 3) {
                LFSC.SetFiles(massStop, massLSP, &SUSYT2LPs, mapToUse, doVerb);
            }
            
            
            if (!SUSYT2LPs.typeT2) {
                paramSMST2bw = SUSYT2LPs.charFrac  * (1.0 / 100);
            }
            else {
                paramSMST2bw = -1;
            }
	    //cout << "about to set files " << endl;
            if (doVerb) {
                cout << "about to set efficiency for (possibly) lept-filter" << endl;
            }
            CMM.SetFiles(massStop, massLSP, &SUSYT2LPs, massMapToUse, doVerb);
            if (doVerb) {
                cout << "about to set efficiency for non lept-filter" << endl;
            }
            CMM.SetEff(&LFSC, massStop, massLSP, paramSMST2bw, SUSYT2LPs.typeT2, SUSYT2LPs.typeT2 == 3 || (BasicLPs.typeFullSel > 3 && BasicLPs.typeFullSel < 6), doVerb);
            CMM.SetBin(massStop, massLSP);
            //            LFSC.CloseFiles();
            CMM.CloseFiles();
            cout << "Pre-addition of non-lept Filter, CMM eff " << CMM.effCV << " +- " << CMM.errEffCV << endl;
            if (doLeptFiltAverage) {
                if (CMM.isLeptFilt && SUSYT2LPs.typeT2 != 2) {
                    if (SUSYT2LPs.typeT2 == 1 && massStop < 150) continue; //c.f. similar thing in header file
                    CMM_NonLeptFilter.SetFiles(massStop, massLSP, &SUSYT2LPs, massMapToUseNoLeptFilter, doVerb);
                    CMM_NonLeptFilter.SetEff(&LFSC, massStop, massLSP, paramSMST2bw, SUSYT2LPs.typeT2, SUSYT2LPs.typeT2 == 3 || (BasicLPs.typeFullSel > 3 && BasicLPs.typeFullSel < 6), doVerb);
                    CMM_NonLeptFilter.SetBin(massStop, massLSP);
                    CMM_NonLeptFilter.CloseFiles();
                    CMM.CombineCoarseEffs(massStop, massLSP, &CMM_NonLeptFilter, doVerb);
                    cout << "Post-addition of non-lept Filter, CMM eff " << CMM.effCV << " +- " << CMM.errEffCV << endl;
                }
            }
            if (SUSYT2LPs.typeT2 != 3) {
                LFSC.CloseFiles();
            }
            //Deal with efficiency calculation when CV is 0 but Syst Var is not (specifically error of calculations)
        }
    }
    CMM.SetMCStatEff(doVerb > 1);
    CMM.DoOverflow();
    
    if (doLeptFiltAverage) {
        CMM_NonLeptFilter.DeleteHists();
    }
    CMM.WriteFile();
    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
