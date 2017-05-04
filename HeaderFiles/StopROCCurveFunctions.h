#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TGraph.h"


#include <iostream>
using namespace std;

TH1F * GetNPassEventList(TTree * inTree, TString drawString, TString cutString) {
    TH1F * outTH1F;
    
    inTree->Draw(TString("TDiLepMass >>") + drawString + TString("(1, 0, 1E5)"), cutString, "goff");
    outTH1F = (TH1F *) gDirectory->Get(drawString);
    return outTH1F;
}

float GetNPass(TTree * inTree, TString drawString, TString cutString, bool doVerb = false) {
    cout << "trying to do NPass " << doVerb << endl;
    if (doVerb) {
        cout << "for cut " << cutString << endl;
    }
    TH1F * tempTH1F = GetNPassEventList(inTree, drawString, cutString);
    if (doVerb) {
        cout << " number passing cut " << tempTH1F->Integral() << endl;
    }
    return tempTH1F->Integral();
}

float CalcEff(TTree * inTree, TString drawString, TString cutString, float totalN, float &passCut, bool doVerb = false) {
    if (doVerb) {
        cout << "for cut " << cutString << endl;
    }
    passCut = GetNPass(inTree, drawString, cutString, doVerb);
    if (passCut < 0) {
        if (doVerb) {
            cout << "passCut < 0! " << passCut << endl;
        }
        passCut = 0;
    }
    if (doVerb) {
        cout << "number passing cut is " << passCut << endl;
        cout << "while total number is " << totalN << endl;
    }
    
    return (passCut * 1.0)/totalN;
}

TGraph * EffCurve(vector<float> * inVecBkg, vector<float> * inVecSig, TString nameGraph, bool doStandardEffCurve = true) {
    // reads in vectors representing the efficiencies for monotonically increasing cuts
    // makes a TGraph showing the ROC curve (with boolean for what kind) and returns it
    int sizeVecBkg = (int) inVecBkg->size();
    int sizeVecSig = (int) inVecSig->size();
    if (sizeVecBkg != sizeVecSig) {
        cout << "something weird with vector sizes: inVecBkg " << sizeVecBkg << " and inVecSig " << sizeVecSig << endl;
    }
    TGraph * outGraph = new TGraph(inVecBkg->size());
    outGraph->SetName(nameGraph);
    
    float x, y;
    
    for (int iPoint = 0; iPoint < sizeVecBkg; ++iPoint) {
        x = inVecBkg->at(iPoint);
        if (doStandardEffCurve) x = (1 - x);
        y = inVecSig->at(iPoint);
        outGraph->SetPoint(iPoint, x, y);
    }
    if (doStandardEffCurve) {
        outGraph->GetXaxis()->SetTitle("1-#epsilon_{bkg}");
    }
    else {
        outGraph->GetXaxis()->SetTitle("#epsilon_{bkg}");
    }
    outGraph->GetYaxis()->SetTitle("#epsilon_{sig}");
    
    return outGraph;
}


TGraph * PunziCurve(vector<float> * inVecPunzi, vector<int> * inVecCut, TString nameGraph, TString titleXAxis) {
    // reads in vectors representing the Punzi parameter for monotonically increasing cuts
    // makes a TGraph showing the Punzi curve (with boolean for what kind) and returns it
    int sizeVecPunzi = (int) inVecPunzi->size();
    int sizeVecCuts = (int) inVecCut->size();
    if (sizeVecPunzi != sizeVecCuts) {
        cout << "something weird with vector sizes: inVecPunzi " << sizeVecPunzi << " and inVecCut " << sizeVecCuts << endl;
    }
    TGraph * outGraph = new TGraph(sizeVecPunzi);
    outGraph->SetName(nameGraph);
    
    float x, y;
    
    for (int iPoint = 0; iPoint < sizeVecPunzi; ++iPoint) {
        x = inVecCut->at(iPoint);
        y = inVecPunzi->at(iPoint);
        outGraph->SetPoint(iPoint, x, y);
    }
    //outGraph->GetXaxis()->SetTitle("M_{T2}(lb)(lb) Cut [GeV]");
    outGraph->GetXaxis()->SetTitle(titleXAxis);
    outGraph->GetYaxis()->SetTitle("Punzi Parameter");
    return outGraph;
}

TString EncapsulateString(TString inputString) {
    return TString("(") + inputString + TString(")");
}