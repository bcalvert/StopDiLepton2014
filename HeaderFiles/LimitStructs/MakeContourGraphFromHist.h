#include "TH2F.h"
#include "TGraph.h"
#include "TString.h"
#include "TObjArray.h"
#include "TList.h"
#include <vector>

#include "TCanvas.h"
#include "TROOT.h"
#include <iostream>
using namespace std;

void ReturnTGraphVector(vector<TGraph *> * inVecTGraphs, TH2F * inputHist, int minNumPoints, float desiredExclusion, bool doComplement = false, bool addToExistingGraphVec = false, bool doVerb = false) {
    //properly make sure vector is cleared first;
    
    unsigned int currNumGraphs = inVecTGraphs->size();
    if (!addToExistingGraphVec) {
        for (unsigned int iGraph = 0; iGraph < currNumGraphs; ++iGraph) {
            inVecTGraphs->at(iGraph)->Delete();
        }
        inVecTGraphs->clear();
    }
    
    
    TCanvas* c = new TCanvas("c","Contour List",0,0,600,600);
    
    
    TString tempName = inputHist->GetName();
    tempName += "_TempClone";
    
    int nBinsX = inputHist->GetNbinsX();
    int nBinsY = inputHist->GetNbinsY();
    float lowStop = inputHist->GetXaxis()->GetXmin();
    float highStop = inputHist->GetXaxis()->GetXmax();
    float lowLSP = inputHist->GetYaxis()->GetXmin();
    float highLSP = inputHist->GetYaxis()->GetXmax();
    
    float limThresh = 1E-9;
    TH2F * tempHist = new TH2F(tempName, "", nBinsX, lowStop, highStop, nBinsY, lowLSP, highLSP);
    
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        float currStopMass = inputHist->GetXaxis()->GetBinCenter(iBinX);
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            float currLSPMass = inputHist->GetYaxis()->GetBinCenter(iBinY);
            float limit = inputHist->GetBinContent(iBinX, iBinY);
            if (limit < limThresh) continue;
            if (doVerb) {
                cout << "currStopMass:currLSPMass " << currStopMass << ":" << currLSPMass << endl;
                cout << "limit " << limit << endl;
                if (doComplement) {
                    cout << "limit/desiredExclusion " << limit/desiredExclusion << endl;
                }
                else {
                    cout << "desiredExclusion/limit " << desiredExclusion/limit << endl;
                }
            }
            if (doComplement) {
                tempHist->Fill(currStopMass, currLSPMass, limit/desiredExclusion);
            }
            else {
                tempHist->Fill(currStopMass, currLSPMass, desiredExclusion/limit);
            }
        }
    }
    
    
    /*
     
     for (int iBinX = 1; iBinX <= inputHist->GetNbinsX(); ++iBinX) {
     for (int iBinY = 1; iBinY <= inputHist->GetNbinsY(); ++iBinY) {
     if (inputHist->GetBinContent(iBinX, iBinY) < 1E-9) {
     inputHist->SetBinContent(iBinX, iBinY, 1E12);
     }
     }
     }
     */
    
    
    
    tempHist->SetContour(3);
    tempHist->SetContourLevel(0, 0); //value for your first level
    tempHist->SetContourLevel(1, 1.); //non-existing high level
    tempHist->SetContourLevel(2, 1e9); //non-existing high level
    tempHist->Draw("CONT Z LIST");
    c->Update();
    
    if (gROOT->GetListOfSpecials()->FindObject("contours")) {
        TList * list = (TList*) ((TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours"))->At(0);
        
        for (int iCont = 0; iCont < list->GetSize(); ++iCont) {
            TGraph * currGraph = (TGraph *) list->At(iCont);
            int currNumPoints = currGraph->GetN();
            if (currNumPoints <= minNumPoints) continue;
            
            float* currArray_X = new float[currNumPoints];
            float* currArray_Y = new float[currNumPoints];
            
            double* currGraphArray_X = currGraph->GetX();
            double* currGraphArray_Y = currGraph->GetY();
            
            for (int iPoint = 0; iPoint < currNumPoints; iPoint++) {
                /*
                 cout << "iPoint " << iPoint << endl;
                 cout << "currArray_X[iPoint] " << currArray_X[iPoint] << endl;
                 cout << "currArray_Y[iPoint] " << currArray_Y[iPoint] << endl;
                 */
                currArray_X[iPoint] = currGraphArray_X[iPoint];
                currArray_Y[iPoint] = currGraphArray_Y[iPoint];
                if (doVerb) {
                    cout << "currArray_X[iPoint]:currArray_Y[iPoint] " << currArray_X[iPoint] << ":" << currArray_Y[iPoint] << endl;
                }
            }
            inVecTGraphs->push_back(new TGraph(currNumPoints, currArray_X, currArray_Y));
        }
    }
    cout << "number of contours found: " << inVecTGraphs->size() << endl;
    c->Close();
}