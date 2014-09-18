#include <iostream>
#include "TFile.h"
#include "TF1.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TRint.h"
#include "TPaveStats.h"


#include "../HeaderFiles/BasicFunctions.h"
void BaseCanvasSetup(TCanvas * InputCanvas, bool logYPad1) {
    InputCanvas->Divide(1, 2);
    TPad * Pad1 = (TPad *) InputCanvas->cd(1);
    FixPad(Pad1, 1, InputCanvas);
    Pad1->SetLogy(logYPad1);
    TPad * botPad = (TPad *) InputCanvas->cd(2);
    FixPad(botPad, 2, InputCanvas);
    botPad->SetGridy(1);
}

#include "../HeaderFiles/StopRootHistoGenFunctions.h"

/*
 
#include "../HeaderFiles/GeneralPlotDrawingStructs.h"
#include "../HeaderFiles/GeneralPlotDrawingFunctions.h"

*/

#include <vector>
#include <cmath>
#include <stdlib.h>
using namespace std;

int main( int argc, char* argv[]) {
  //  gStyle->SetOptStat(0);
  //    gStyle->SetOptFit(kFALSE);
  //    gROOT->ForceStyle();
    gROOT->ProcessLine("#include <vector>");
    gStyle->SetErrorX(0.5); // set X uncertainty to 1/2 of a bin
    gStyle->SetTitleFont(42);
    TRint theApp("App", &argc, argv);
    Bool_t retVal = kTRUE;

    int funcTypeMain = 0;
    int funcTypeErr = 0;
    
    int nPoints = 1801;
    float xLB = 99.75, xUB = 1000.25;
    
    for (int k = 0; k < argc; ++k) {
        cout << "argv[k] for k = " << k << " is: " << argv[k] << endl;

        if (strncmp (argv[k],"wFunc", 5) == 0) {
            funcTypeMain = strtol(argv[k+1], NULL, 10 );
            funcTypeErr = strtol(argv[k+2], NULL, 10 );
        }
    }

    TString fileLoc = "../NonDataRootFiles/SignalXSection/";
    TString fileName = "referenceXSecs.root";
    
    TString nameHistStop = "stop8TeV_NLONLL";
    TString nameHistStopErr = "stop8TeV_NLONLL_unc";

    TFile * stopHistFile = TFile::Open(fileLoc + fileName);
    
    TH1F * baseXSecHist = (TH1F *) stopHistFile->Get(nameHistStop);
    TH1F * baseXSecHistErr = (TH1F *) stopHistFile->Get(nameHistStopErr);

    TH1F * FiveBinHist = new TH1F("StopXSec_5GeV", ";M_{#tilde{t}} [GeV]; #sigma(pp#rightarrow#tilde{t}#tilde{t}) [pb]", baseXSecHist->GetNbinsX(), 97.5, 1002.5);
    
    TH1F * fineBinHist = new TH1F("StopXSec", ";M_{#tilde{t}} [GeV]; #sigma(pp#rightarrow#tilde{t}#tilde{t}) [pb]", nPoints, xLB, xUB);
    TH1F * fineBinHistErr = new TH1F("StopXSecErr", ";M_{#tilde{t}} [GeV]; uncert #sigma(pp#rightarrow#tilde{t}#tilde{t}) [pb]", nPoints, xLB, xUB);
    
    TString canvPrepend = "c_";
    TString canvPostpend = ".pdf";
    TString nameMain = "XSec";
    TString nameErr = nameMain + "Err";
    nameMain += funcTypeMain;
    nameErr += funcTypeErr;    
    
    TString logNameMain = canvPrepend + "log" + nameMain + canvPostpend;
    TString logNameErr = canvPrepend + "log" + nameErr + canvPostpend;
    
    TString unLogNameMain = canvPrepend + nameMain + canvPostpend;
    TString unLogNameErr = canvPrepend + nameErr + canvPostpend;
    
    StopXSecFitter SXF_Main, SXF_Err;
    
    SXF_Main.FullChainLogLog((int) xLB, (int) xUB, true, true, funcTypeMain, baseXSecHist, fineBinHist);
    TCanvas * LogXSec = new TCanvas("LogXSec","LogXSec");
    TCanvas * c_SXSecMain = new TCanvas("c_StopXSec", "c_StopXSec");
    SXF_Main.DrawHists(LogXSec, logNameMain, c_SXSecMain, unLogNameMain, baseXSecHist);
    
    //    cout << "logNameMain " << logNameMain << endl;
    //    cout << "logNameErr " << logNameErr << endl;

    SXF_Err.FullChainLogLog((int) xLB, (int) xUB, false, true, funcTypeErr, baseXSecHistErr, fineBinHistErr);
    TCanvas * LogXSecErr = new TCanvas("LogXSecErr","LogXSecErr");
    TCanvas * c_SXSecErr = new TCanvas("c_StopXSecErr", "c_StopXSecErr");
    SXF_Err.DrawHists(LogXSecErr, logNameErr , c_SXSecErr, unLogNameErr, baseXSecHistErr);
    
    CloneVals(baseXSecHist, FiveBinHist);
    
    TFile * outputFile = new TFile("Stop8TeVXSecs.root", "RECREATE");
    outputFile->cd();
    TH1F * outHist5GeV = CombineHists(FiveBinHist, baseXSecHistErr, "");
    TH1F * outHist0p5GeV = CombineHists(SXF_Main.histFineBin, SXF_Err.histFineBin, "_Interpolate");
    outputFile->Write();
    outputFile->Close();

    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
