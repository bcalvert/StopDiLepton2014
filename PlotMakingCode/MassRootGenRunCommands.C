#include "TH2F.h"
#include "TH3F.h"
#include "TFile.h"
#include "TROOT.h"
#include "TMath.h"
#include <vector>
#include <cmath>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

vector<float> valsNonZeroEntries(TH2F * inputHistogram, vector<int> * xCoords, vector<int> * yCoords, int thresh = 0) {
    vector<float> outVec;
    float binCont;
    if (xCoords->size() > 0) xCoords->resize(0);
    if (yCoords->size() > 0) xCoords->resize(0);
    for (int i = 1; i < inputHistogram->GetNbinsX() + 1; ++i) {
        for (int j = 1; j < inputHistogram->GetNbinsX() + 1; ++j) {
            binCont = inputHistogram->GetBinContent(i, j);
            if (binCont > thresh) {
                xCoords->push_back(i);
                yCoords->push_back(j);
                outVec.push_back(binCont);
            }
        }
    }
    return outVec;
}

vector<float> valsNonZeroEntries(TH3F * inputHistogram, vector<int> * xCoords, vector<int> * yCoords, vector<int> * zCoords, int thresh = 0) {
    vector<float> outVec;
    float binCont;
//    cout << "inputHistogram->GetNbinsX() " << inputHistogram->GetNbinsX() << endl;
//    cout << "inputHistogram->GetNbinsY() " << inputHistogram->GetNbinsY() << endl;
//    cout << "inputHistogram->GetNbinsZ() " << inputHistogram->GetNbinsZ() << endl;
    if (xCoords->size() > 0) xCoords->resize(0);
    if (yCoords->size() > 0) yCoords->resize(0);
    if (zCoords->size() > 0) zCoords->resize(0);
    for (int i = 1; i < inputHistogram->GetNbinsX() + 1; ++i) {
        for (int j = 1; j < inputHistogram->GetNbinsY() + 1; ++j) {
            for (int k = 1; k < inputHistogram->GetNbinsZ() + 1; ++k) {
                binCont = inputHistogram->GetBinContent(i, j, k);
//		cout << "{x, y, z} " << i << ", " << j << ", " << k << " binCont" << binCont << endl;
                if (binCont > thresh) {
                    xCoords->push_back(i);
                    yCoords->push_back(j);
                    zCoords->push_back(k);
                    outVec.push_back(binCont);
                }
            }
        }
    }
    return outVec;
}

int nearestIntMultiple(int inInt, int relMult) {
    int x = TMath::Nint(inInt / (float) relMult);
    x *= relMult;
    return x;
}
float nearestFloatMultiple(float inFloat, float relMult) {
    float x = (float) TMath::Nint(inFloat / relMult);
    x *= relMult;
    return x;
}
int main( int argc, char* argv[]) {
    TAxis * xAxis, * yAxis, * zAxis;
    TString fInName;
    TFile * inFile;
    TH1 * inHist;
    
    vector<int> xCoords, yCoords, zCoords;
    vector<float> numEntries;
    bool doTen = 0;
    bool Maria = 0;
    for (int k = 0; k < argc; ++k) {
        if (strncmp (argv[k],"-i",2) == 0) {
            fInName = TString(argv[k+1]);   
        }
        if (strncmp (argv[k],"Maria",5) == 0) {
            Maria = 1;
        }
    }
    bool isT2tt = fInName.Contains("T2tt");
    if (isT2tt && fInName.Contains("FineBin")) {
        doTen = 1;
    }

    int nearMult = doTen ? 10 : 25;
    float plus = Maria ? 0 : doTen ? 5 : 12.5;
    inFile = new TFile(fInName + TString(".root"));
    TString getString = Maria ? "masses" : isT2tt ? "h_SUSY2DMassEventCount" : "h_SUSY3DMassEventCount";

    inHist = (TH1 *) inFile->Get(getString);
    if (isT2tt) {
        numEntries = valsNonZeroEntries((TH2F*)inHist, &xCoords, &yCoords, 500);
    }
    else {
        numEntries = valsNonZeroEntries((TH3F*)inHist, &xCoords, &yCoords, &zCoords, 500);
    }
    xAxis = inHist->GetXaxis();
    yAxis = inHist->GetYaxis();
    zAxis = inHist->GetZaxis();
    int MStop, MChi0;
    float charginoX;
    if (isT2tt) {
        cout << "for inFile " << fInName << ".root" << endl;
        cout << "in format NumEntries:MStop:MChi0 -- " << endl;
        for (unsigned int i = 0; i < numEntries.size(); ++i) {
            MStop = nearestIntMultiple(plus + xAxis->GetBinLowEdge(xCoords[i]), nearMult);
            MChi0 = nearestIntMultiple(plus + yAxis->GetBinLowEdge(yCoords[i]), nearMult);
            //        cout << "for x = " << MStop << " and y = " << MChi0 << ", number of entries is " << numEntries[i] << ", or in format NumEntries:MStop:MChi0 -- " << endl;
            cout << numEntries[i] << ":" << MStop << ":" << MChi0 << endl;
        }
    }
    else {
        cout << "for inFile " << fInName << ".root" << endl;
        cout << "in format NumEntries:MStop:MChi0:CharginoMassFrac -- " << endl;
        for (unsigned int i = 0; i < numEntries.size(); ++i) {
            MStop = nearestIntMultiple(plus + xAxis->GetBinLowEdge(xCoords[i]), nearMult);
            MChi0 = nearestIntMultiple(plus + yAxis->GetBinLowEdge(yCoords[i]), nearMult);
            charginoX = nearestFloatMultiple(0.125 + zAxis->GetBinLowEdge(zCoords[i]), 0.25);
            //        cout << "for x = " << MStop << " and y = " << MChi0 << ", number of entries is " << numEntries[i] << ", or in format NumEntries:MStop:MChi0 -- " << endl;
            cout << numEntries[i] << ":" << MStop << ":" << MChi0 << ":" << charginoX << endl;
        }
    }
}
