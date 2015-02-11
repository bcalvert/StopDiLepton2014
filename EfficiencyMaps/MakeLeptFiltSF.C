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
    
    int paramSMS = 0;
    float paramSMST2bw = 0;
    LeptFiltSignalAssociator LFSC;
    
    bool doVerb = 0;
    
    SUSYT2LimitParams SUSYT2LPs;
    SUSYT2LPs.DefaultVals();
    SUSYT2LPs.SetParamsFromCommandLine(argc, argv);
    SUSYT2LPs.SetStrings();
    
    for (int k = 0; k < argc; ++k) {
        if (strncmp (argv[k],"doVerb", 6) == 0) {
            doVerb = 1;
        }
    }
    
    LFSC.DefaultVals(SUSYT2LPs.typeT2);
    //LFSC.SetHistAndOutFile(SUSYT2LPs.typeT2, paramSMS, false);
    LFSC.SetHistAndOutFile(&SUSYT2LPs, false);
    
    
    
    //LFSC.PrintBasic(SUSYT2LPs.typeT2, paramSMS);
    SUSYT2LPs.PrintSignalModel();
    
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
    
    
    
    int stopLB, stopUB;
    int LSPLB, LSPUB;
    
    int maxDiffLSP = SUSYT2LPs.typeT2 ? 200 : 350;
    
    switch (SUSYT2LPs.typeT2) {
        case 0:
            stopLB = 100;
            stopUB = 650;
            LSPLB = 0;
            LSPUB = 300;
            break;
        case 1:
            stopLB = 100;
//            stopLB = 150;
            stopUB = 350;
            LSPLB = 0;
            LSPUB = 250;
            break;
        case 2:
            stopLB = 150;
            stopUB = 450;
            LSPLB = 0;
            LSPUB = 300;
            break;
        default:
            break;
    }
    
    int stepSize = 25;
    
    
    
    for (int massStop = stopLB; massStop <= stopUB; massStop += stepSize) {
        for (int massLSP = TMath::Max(LSPLB, massStop - maxDiffLSP); massLSP <= TMath::Min(LSPUB, massStop - 100); massLSP += stepSize) {
            cout << endl;
            cout << "massStop:massLSP " << massStop << ":" << massLSP << endl;
            if (SUSYT2LPs.typeT2 == 0) {
                mapToUse = massLSP == 0 ? &mapT2bwLSP0 : &mapT2bw;
            }
            else if (SUSYT2LPs.typeT2 == 1) {
                mapToUse = massLSP == 0 ? &mapT2ttLSP0 : &mapT2tt;
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
                paramSMST2bw = SUSYT2LPs.charFrac * (1.0 / 100);
                LFSC.SetBin(massStop, massLSP, paramSMST2bw, SUSYT2LPs.typeT2, doVerb);
            }
            else {
                LFSC.SetBin(massStop, massLSP, SUSYT2LPs.indexPol, SUSYT2LPs.typeT2, doVerb);
            }
            LFSC.CloseFiles();
        }
    }
    
    LFSC.WriteFile();
    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
