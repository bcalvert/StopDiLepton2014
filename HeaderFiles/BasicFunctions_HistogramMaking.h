#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TString.h"
#include "TGraphAsymmErrors.h"

using namespace std;

typedef pair<float, float> valPlusError;

//List of functions for creating histograms/TGraphs
//that are used in multiple different aspects of the analysis

inline TH2F * TwoDeeRatio(TH2F * inputHistNum, TH2F * inputHistDenom, TString addHistName = "", bool doConsistency = false, TH2F * inputHist_UncertUp = 0, TH2F * inputHist_UncertDown = 0) {
    //Histogram that generates a 2D plot showing the ratio of inputHistNum/inputHistDenom
    //If the "doConsistency" boolean is turned on, then
    //the function assumes one is asking how consistent the ratio is with 1.0 and will do
    //(Ratio - 1.0)/SigmaRatio
    //If two additional histograms are included -- inputHist_UncertUp/inputHist_UncertDown --
    //then the function treats those as the uncertainty on the ratio, as opposed to the
    //standard uncertainties on the actual input histograms. Including these two histograms is
    //for the purpose of doing the consistency check;
    //if the ratio is < 1.0, the sigma ratio used will be the
    //up shift (as that would bring the ratio closer to one);
    //if the ratio is > 1.0, the sigma ratio used will be the
    //down shift (for similar reasons as down shift)
    
    bool useSyst = inputHist_UncertUp && inputHist_UncertDown && doConsistency;
    
    int nBinsXNum = inputHistNum->GetNbinsX();
    int nBinsYNum = inputHistNum->GetNbinsY();
    
    int nBinsXDenom = inputHistDenom->GetNbinsX();
    int nBinsYDenom = inputHistDenom->GetNbinsY();
    
    int nBinsXUncertUp, nBinsXUncertDown;
    int nBinsYUncertUp, nBinsYUncertDown;
    
    if (nBinsXNum != nBinsXDenom) {
        cout << "Something wrong with input histograms, nBinsX don't match" << endl;
        cout << "histNum Name: " << inputHistNum->GetName() << ", nBinsXNum " << nBinsXNum << endl;
        cout << "histDenom Name: " << inputHistDenom->GetName() << ", nBinsXDenom " << nBinsXDenom << endl;
        return NULL;
    }
    if (nBinsYNum != nBinsYDenom) {
        cout << "Something wrong with input histograms, nBinsY don't match" << endl;
        cout << "histNum Name: " << inputHistNum->GetName() << ", nBinsYNum " << nBinsYNum << endl;
        cout << "histDenom Name: " << inputHistDenom->GetName() << ", nBinsYDenom " << nBinsYDenom << endl;
        return NULL;
    }
    
    if (useSyst) {
        nBinsXUncertUp = inputHist_UncertUp->GetNbinsX();
        nBinsXUncertDown = inputHist_UncertDown->GetNbinsX();
        
        nBinsYUncertUp = inputHist_UncertUp->GetNbinsY();
        nBinsYUncertDown = inputHist_UncertDown->GetNbinsY();
        if (nBinsXNum != nBinsXUncertUp || nBinsXNum != nBinsXUncertDown) {
            cout << "Something wrong with input histograms, nBinsX don't match" << endl;
            cout << "histNum Name: " << inputHistNum->GetName() << ", nBinsXNum " << nBinsXNum << endl;
            cout << "histUncertUp Name: " << inputHist_UncertUp->GetName() << ", nBinsXUncertUp " << nBinsXUncertUp << endl;
            cout << "histUncertDown Name: " << inputHist_UncertDown->GetName() << ", nBinsXUncertDown " << nBinsXUncertDown << endl;
            return NULL;
        }
        
        if (nBinsYNum != nBinsYUncertUp || nBinsYNum != nBinsYUncertDown) {
            cout << "Something wrong with input histograms, nBinsY don't match" << endl;
            cout << "histNum Name: " << inputHistNum->GetName() << ", nBinsYNum " << nBinsYNum << endl;
            cout << "histUncertUp Name: " << inputHist_UncertUp->GetName() << ", nBinsYUncertUp " << nBinsYUncertUp << endl;
            cout << "histUncertDown Name: " << inputHist_UncertDown->GetName() << ", nBinsYUncertDown " << nBinsYUncertDown << endl;
            return NULL;
        }
    }
    
    TString outHistName = inputHistNum->GetName() + TString("Div") + inputHistDenom->GetName() + addHistName;
    if (doConsistency) {
        outHistName += "_Consistency";
    }
    TH2F * outRatioHist = (TH2F *) inputHistNum->Clone(outHistName);
    
    valPlusError currVPENum, currVPEDenom;
    valPlusError currVPEUncertUp, currVPEUncertDown;
    float currBinContentNum, currBinContentDenom;
    float currBinErrNum, currBinErrDenom;
    
    valPlusError currBinRatioVPE(0,0);
    for (int iBinX = 1; iBinX <= nBinsXNum; ++iBinX) {
        for (int iBinY = 1; iBinY <= nBinsYNum; ++iBinY) {
            currBinContentNum = inputHistNum->GetBinContent(iBinX, iBinY);
            currBinErrNum = inputHistNum->GetBinError(iBinX, iBinY);
            
            currBinContentDenom = inputHistDenom->GetBinContent(iBinX, iBinY);
            currBinErrDenom = inputHistDenom->GetBinError(iBinX, iBinY);
            
            currVPENum = valPlusError(currBinContentNum, currBinErrNum);
            currVPEDenom = valPlusError(currBinContentDenom, currBinErrDenom);
            
            if (useSyst) {
                currVPEUncertUp = valPlusError(inputHist_UncertUp->GetBinContent(iBinX, iBinY), inputHist_UncertUp->GetBinError(iBinX, iBinY));
                currVPEUncertDown = valPlusError(inputHist_UncertDown->GetBinContent(iBinX, iBinY), inputHist_UncertDown->GetBinError(iBinX, iBinY));
                currBinRatioVPE = GetRatio(&currVPENum, &currVPEDenom, doConsistency, &currVPEUncertUp, &currVPEUncertDown);
            }
            else {
                currBinRatioVPE = GetRatio(&currVPENum, &currVPEDenom, doConsistency);
            }
            outRatioHist->SetBinContent(iBinX, iBinY, currBinRatioVPE.first);
            outRatioHist->SetBinError(iBinX, iBinY, currBinRatioVPE.second);
        }
    }
    return outRatioHist;
}

TH1F * ThreeDeeProject(TH3F * inputHist, TString name, int whichAxis, vector<int> * vecBounds) {
    //Function to project an input 3D histogram into a 1D histogram given an input vector of bounds for the axes
    
    TString optProj = name + "_";
    switch (whichAxis) {
        case 1:
            optProj += "x";
            inputHist->GetYaxis()->SetRange(vecBounds->at(0), vecBounds->at(1));
            inputHist->GetZaxis()->SetRange(vecBounds->at(2), vecBounds->at(3));
            break;
        case 2:
            optProj += "y";
            inputHist->GetXaxis()->SetRange(vecBounds->at(0), vecBounds->at(1));
            inputHist->GetZaxis()->SetRange(vecBounds->at(2), vecBounds->at(3));
            break;
        case 3:
            optProj += "z";
            inputHist->GetXaxis()->SetRange(vecBounds->at(0), vecBounds->at(1));
            inputHist->GetYaxis()->SetRange(vecBounds->at(2), vecBounds->at(3));
            break;
        default:
            break;
    }
    return (TH1F *) inputHist->Project3D(optProj);
}

inline TH2F * ThreeDeeToTwoDeeStatProjection(TH3F * inputHist, int whichAxis, int whichPercentile, bool doVerb = 0) {
    //Function that takes an input 3D histogram and, in the 2D plane of the 2 axes not projected with, projects the remaining axis
    //and calculates some statistical quantity with this projection, the idea being that you can use this to help visualize 3D distributions
    //still a little bit of a work in progress
    
    float currStatBinVal;
    TH1F * tempDist;
    
    /*
    int nBinsX = inputHist->GetNbinsX();
    int nBinsY = inputHist->GetNbinsY();
    int nBinsZ = inputHist->GetNbinsZ();
    
    int nBins_Loop1 = whichAxis > 1 ? nBinsX : nBinsY;
    int nBins_Loop2 = whichAxis != 3 ? nBinsZ : nBinsY;
    */
    
    TString optProjString = "p2_";
    switch (whichAxis) {
        case 1:
            optProjString += "zy";
            break;
        case 2:
            optProjString += "zx";
            break;
        case 3:
            optProjString += "yx";
            break;
        default:
            break;
    }
    if (doVerb) {
        cout << "inputHist name " << inputHist->GetName() << endl;
        cout << "optProjString " << optProjString << endl;
    }
    TH2F * outHist = (TH2F *) inputHist->Project3D(optProjString);
    int nBinsX = outHist->GetNbinsX();
    int nBinsY = outHist->GetNbinsY();
    if (doVerb) {
        cout << "outHist name " << outHist->GetName() << endl;
    }
    
    vector<int> vecBounds(4);
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        vecBounds[0] = iBinX;
        vecBounds[1] = iBinX;
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            vecBounds[2] = iBinY;
            vecBounds[3] = iBinY;
            if (doVerb) {
                cout << "iBinX " << iBinX << endl;
                cout << "iBinY " << iBinY << endl;
            }
            TString name = "p1_";
            name += iBinX;
            name += "_";
            name += iBinY;
            tempDist = ThreeDeeProject(inputHist, name, whichAxis, &vecBounds);
            if (doVerb) {
                cout << "tempDist name " << tempDist->GetName() << endl;
            }
            currStatBinVal = GetStatValue(tempDist, whichPercentile, doVerb);
            if (doVerb) {
                cout << "currStatBinVal " << currStatBinVal << endl;
            }
            outHist->SetBinContent(iBinX, iBinY, currStatBinVal);
        }
    }
    
    return outHist;
    /*
    vector<int> vecBounds(4);
    for (int iBinLoop1 = 1; iBinLoop1 <= nBins_Loop1; ++iBinLoop1) {
        vecBounds[0] = iBinLoop1;
        vecBounds[1] = iBinLoop1;
        for (int iBinLoop2 = 1; iBinLoop2 <= nBins_Loop2; ++iBinLoop2) {
            vecBounds[2] = iBinLoop2;
            vecBounds[3] = iBinLoop2;
            
            tempDist = ThreeDeeProject(inputHist, whichAxis, &vecBounds);
        }
    }
    */
}