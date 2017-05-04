#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TGraph.h"


#include <iostream>
using namespace std;

TString HistogramBinString(unsigned int numBins, float binLB, float binUB) {
    TString outString = "";
    outString += numBins;
    outString += ", ";
    outString += binLB;
    outString += ", ";
    outString += binUB;
    return outString;
}


TH1F * GetTotalHistogram(TTree * inTree, ROCCurveParams * inROCCP, vector<TString> * vecAddtlCutStrings, TString weightString) {
    TString histName = "h1d";

    TString strDraw = inROCCP->strVarCut + TString(">>") + histName + TString("(");
    unsigned int numBins = inROCCP->vecCutVals.size();
    TString binString = HistogramBinString(numBins, inROCCP->vecCutVals[0] - 0.1, inROCCP->vecCutVals[numBins - 1] + 0.1);
    strDraw += binString;
    strDraw += ")";
    
    TString strCut = inROCCP->strCutCommand;
    unsigned int numCutsAddtl = vecAddtlCutStrings->size();
    for (unsigned int iCut = 0; iCut < numCutsAddtl; ++iCut) {
        /*if (iCut != numCutsAddtl - 1) {
            strCut += " && ";
        }
         */
        strCut += " && ";
        strCut += vecAddtlCutStrings->at(iCut);

    }

    cout << "going to draw " << strDraw << endl;
    cout << "with the cuts " << weightString + EncapsulateString(strCut) << endl;
    
    inTree->Draw(strDraw, weightString + EncapsulateString(strCut), "goff");
    TH1F * outTH1F = (TH1F *) gDirectory->Get(histName);
    return outTH1F;
}

TH3F * GetTotalHistogram_TH3(TTree * inTree, TString drawString, ROCCurveParams * inROCCP, vector<TString> * vecAddtlCutStrings, TString weightString) {
    TString histName = "h3d";
    
    TString strCut = inROCCP->strCutCommand;
    unsigned int numCutsAddtl = vecAddtlCutStrings->size();
    for (unsigned int iCut = 0; iCut < numCutsAddtl; ++iCut) {
        /*if (iCut != numCutsAddtl - 1) {
         strCut += " && ";
         }
         */
        strCut += " && ";
        strCut += vecAddtlCutStrings->at(iCut);
        
    }
    
    cout << "going to draw " << drawString << endl;
    cout << "with the cuts " << weightString + EncapsulateString(strCut) << endl;
    
    inTree->Draw(drawString, weightString + EncapsulateString(strCut), "goff");
    TH3F * outTH3F = (TH3F *) gDirectory->Get(histName);
    return outTH3F;
}

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

TGraph * PunziCurve(vector<float> * inVecPunzi, vector<int> * inVecCut, TString nameGraph, TString titleXAxis, float factor = 1) {
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
    
    TString titleYAxis = "Punzi Parameter";
    if (factor != 1) {
        titleYAxis += " x ";
        titleYAxis += factor;
        titleYAxis += "        ";
    }
    
    outGraph->GetXaxis()->SetTitle(titleXAxis);
    outGraph->GetYaxis()->SetTitle("Punzi Parameter");
    return outGraph;
}



