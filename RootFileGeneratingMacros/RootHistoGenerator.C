#include <iostream>
#include "../HeaderFiles/StopRootHistoGenFunctions.h"

#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TRint.h"
#include <vector>
#include <cmath>
#include <stdlib.h>
using namespace std;

int main( int argc, char* argv[]) {
    cout << "a " << endl;
    gROOT->ProcessLine("#include <vector>");
    cout << "b " << endl;
//    gROOT->SetBatch(false);
    // Style things
    gStyle->SetErrorX(0.5); // set X uncertainty to 1/2 of a bin
    cout << "c " << endl;
    gStyle->SetTitleFont(42);
    cout << "d " << endl;
    
    cout << "e " << endl;
    TRint theApp("App", &argc, argv);
    Bool_t retVal = kTRUE;
    cout << "f " << endl;
    
    int lepType = 11;
    int histType = 0;
    
    for (int k = 0; k < argc; ++k) {
        cout << "argv[k] for k = " << k << " is: " << argv[k] << endl;

        if (strncmp (argv[k],"wLep", 4) == 0) {
            lepType = strtol(argv[k+1], NULL, 10 );
        }
        else if (strncmp (argv[k],"wTHist", 6) == 0) {
            histType = strtol(argv[k+1], NULL, 10 );
        }
    }
    

    TString stringLep = lepType == 11 ? "Elec" : "Muon";
    TString stringHistType = histType == 1 ? "Prompt" : "Fake";
    stringHistType += "Rate";
    TString fileNameAppend = ".txt";
    
    TString fileLocation = "../FilesForRootHistGeneration/";
    TString outFileLocation = "../NonDataRootFiles/FakePromptRateHistos/";
    
    TString fileNameMain = fileLocation + stringLep + stringHistType + fileNameAppend;
    TString fileNameErr = fileLocation + stringLep + stringHistType + "Err" + fileNameAppend;
    

    TH2F * outHistMain = FakeLeptonHistMaker(fileNameMain, histType, lepType);
    outHistMain->SetMarkerColor(kWhite);
    outHistMain->SetMarkerSize(2.5);
    outHistMain->SetTitleSize(50);
    TH2F * outHistErr = FakeLeptonHistMaker(fileNameErr, histType, lepType, "Err");
    outHistErr->SetMarkerColor(kWhite);
    outHistErr->SetMarkerSize(2.5);
    outHistErr->SetTitleSize(50);
    TCanvas * mainRate = new TCanvas("main rate", "main rate", 800, 800);
    mainRate->cd();
    outHistMain->Draw("colz text");
    
    TCanvas * errRate = new TCanvas("err rate", "err rate", 800, 800);
    errRate->cd();
    outHistErr->Draw("colz text");
    
    TFile * outFile = new TFile(outFileLocation + stringLep + stringHistType + ".root", "RECREATE");
    TH2F * outHistComp = CombineHists(outHistMain, outHistErr);
    outFile->Write();
    outFile->Close();

    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
