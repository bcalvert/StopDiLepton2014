#include <iostream>
#include <fstream>
//#include <vector>                                                                                   
#include <cmath>
#include <sstream>
#include <vector>
using namespace std;

#include "TH2F.h"
#include "TString.h"
#include "TMath.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TPaveStats.h"
#include "TCanvas.h"

//Functions used when making the limit plots -- need to work on this

int RatioExclusionStatus(int inBinNew, int inBinOld) {
    int onlyExcNew = -1;
    
    if (inBinNew <= 1) {
        if (inBinOld <= 1) {
            onlyExcNew = 0;
        }
        else {
            onlyExcNew = 1;
        }
    }
    else {
        if (inBinOld <= 1) {
            onlyExcNew = 2;
        }
        else {
            onlyExcNew = -1;
        }
    }
    
    return onlyExcNew;
}


float BinRatioLimit(float inBinNew, float inBinOld, int &onlyExcNew) {
    float outRatio = -1;
    float thresh = 9998.;
    float smallThresh = 1E-5;
    if (inBinOld > thresh && inBinNew < thresh) {
//        outRatio = 1E3;
//        outRatio = -1;
//        onlyExcNew = 2;
        outRatio = 500;
    }
    else if (inBinOld < smallThresh || inBinNew < smallThresh) {
        outRatio = -1;
        onlyExcNew = 3;
        return outRatio;
    }
    else {
        outRatio = inBinOld / inBinNew;
    }
    onlyExcNew = RatioExclusionStatus(inBinNew, inBinOld);
    return outRatio;
}
void ZeroNonExclPoints(TH2F * inputSigStrengthHist) {
    int nBinsX = inputSigStrengthHist->GetNbinsX();
    int nBinsY = inputSigStrengthHist->GetNbinsY();
    float currBinContent = 0.0, currBinErr = 0.0;
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            if (iBinY > iBinX) continue;
            currBinContent = inputSigStrengthHist->GetBinContent(iBinX, iBinY);
            currBinErr = inputSigStrengthHist->GetBinError(iBinX, iBinY);
            if (currBinContent > 1) {
                currBinContent = -9999;
                inputSigStrengthHist->SetBinContent(iBinX, iBinY, currBinContent);
            }
            else if (currBinContent < 1E-6) {
                currBinContent = 0.99999999;
                currBinErr = currBinContent;
                inputSigStrengthHist->SetBinContent(iBinX, iBinY, currBinContent);
                inputSigStrengthHist->SetBinError(iBinX, iBinY, currBinContent);
            }
        }
    }
}
TH2F * ExclConsistencyHist(TH2F * inputSigStrengthHist, int howManySig = 0) {
    TString cloneName = inputSigStrengthHist->GetName() + TString("_ExclConsistencyShift");
    cloneName += TMath::Abs(howManySig);
    cloneName += howManySig > 0 ? "Up" : "Down";
    TH2F * outHist = (TH2F *) inputSigStrengthHist->Clone(cloneName);
    
    int nBinsX = outHist->GetNbinsX();
    int nBinsY = outHist->GetNbinsY();
    
    float currBinContent = 0.0, currBinErr = 0.0;
    float currBinExclConsist = 0.0;
    float globalMin = 10.0;
    
    float threshTruncate = -2;
    
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            if (iBinY > iBinX) continue;
            currBinContent = inputSigStrengthHist->GetBinContent(iBinX, iBinY);
            currBinErr = inputSigStrengthHist->GetBinError(iBinX, iBinY);
            if (currBinContent > 9E3 || currBinContent < 1E-6) {
                currBinExclConsist = -1E3;
            }
            else {
                currBinExclConsist = (currBinContent - 1)/currBinErr;
                if (currBinExclConsist < threshTruncate) currBinExclConsist = threshTruncate;
                if (currBinExclConsist > 1.0) currBinExclConsist = -1E2;
            }
            globalMin = TMath::Min(globalMin, currBinExclConsist);
            /*
            cout << "cloneName " << cloneName << endl;
            cout << "iBinX " << iBinX << endl;
            cout << "iBinY " << iBinY << endl;
            cout << "currBinExclConsist " << currBinExclConsist << endl;
            cout << "globalMin " << globalMin << endl;
            */
            outHist->SetBinContent(iBinX, iBinY, currBinExclConsist);
        }
    }
    outHist->GetZaxis()->SetRangeUser(TMath::Max(globalMin, threshTruncate), howManySig);
//    outHist->GetZaxis()->SetRangeUser(-5, howManySig);
    return outHist;
}

TH2F * OutputHistConstDeltM(TH2F * inputLimHist, int whichVers) {
    TAxis * xAxis = inputLimHist->GetXaxis();
    int nBinsX = inputLimHist->GetNbinsX();
    
    TAxis * yAxis = inputLimHist->GetYaxis();
    int nBinsY = inputLimHist->GetNbinsY();
    
    TString axisString = inputLimHist->GetTitle();
    if (whichVers == 0) {
        axisString += ";";
        axisString += xAxis->GetTitle();
        axisString += ";";
        axisString += "#Delta M";
    }
    else {
        axisString += ";";
        axisString += "#Delta M";
        axisString += ";";
        axisString += yAxis->GetTitle();
    }
    
    TString newName = inputLimHist->GetName();
    newName += "_ConstDeltaM_vers";
    newName += whichVers;
    
    int minDeltM = xAxis->GetBinCenter(1) - yAxis->GetBinCenter(1);
    int maxDeltM = xAxis->GetBinCenter(nBinsX) - yAxis->GetBinCenter(1);
    float binWidth = xAxis->GetBinWidth(1);
    
    TH2F * outHist;
    if (whichVers == 0) {
        outHist = new TH2F(newName, axisString, nBinsX, xAxis->GetBinLowEdge(1), xAxis->GetBinUpEdge(nBinsX), nBinsX, minDeltM - binWidth, maxDeltM + binWidth);
    }
    else {
        outHist = new TH2F(newName, axisString, nBinsX, minDeltM - binWidth, maxDeltM + binWidth, nBinsY, yAxis->GetBinLowEdge(1), yAxis->GetBinUpEdge(nBinsY));
    }
    float currBinContent, currBinErr;
    float currDeltaM, currStopMass, currLSPMass;
    int currDeltaMBin;
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        currStopMass = xAxis->GetBinCenter(iBinX);
        for (int iBinY = 1; iBinY <= TMath::Min(iBinX, nBinsX); ++iBinY) {
            currLSPMass = yAxis->GetBinCenter(iBinY);
            currDeltaM = currStopMass - currLSPMass;
            currBinContent = inputLimHist->GetBinContent(iBinX, iBinY);
            currBinErr = inputLimHist->GetBinError(iBinX, iBinY);
            if (whichVers == 0) {
                currDeltaMBin = outHist->GetYaxis()->FindBin(currDeltaM);
                outHist->SetBinContent(iBinX, currDeltaMBin, currBinContent);
                outHist->SetBinError(iBinX, currDeltaMBin, currBinErr);
            }
            else {
                currDeltaMBin = outHist->GetXaxis()->FindBin(currDeltaM);
                outHist->SetBinContent(currDeltaMBin, iBinY, currBinContent);
                outHist->SetBinError(currDeltaMBin, iBinY, currBinErr);
            }
        }
    }
    return outHist;
}

TH2F * LimitRatioHist(TH2F * inHistNew, TH2F * inHistOld, TString outHistName, int whichKind = -1, bool doVerb = 0) {
    TH2F * outHist = (TH2F *) inHistNew->Clone(outHistName);

    int nBinsX = inHistNew->GetNbinsX();
    int nBinsY = inHistNew->GetNbinsY();
    
    float currBinNew, currBinOld;
    float currRatio;
    int onlyExcNew;
    
    if (doVerb) {
        cout << "name inHistNew " << inHistNew->GetName() << endl;
        cout << "name inHistOld " << inHistOld->GetName() << endl;
        cout << "outHistName " << outHistName << endl;
    }
    float currMassStop, currMassLSP;
    float maxMassDiff = 99;
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        currMassStop = inHistNew->GetXaxis()->GetBinCenter(iBinX);
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            currMassLSP = inHistNew->GetYaxis()->GetBinCenter(iBinY);
            if (currMassStop - currMassLSP > maxMassDiff) {
                
                currBinNew = inHistNew->GetBinContent(iBinX, iBinY);
                currBinOld = inHistOld->GetBinContent(iBinX, iBinY);
                
                if (doVerb) {
                    cout << "New iBinX:iBinY " << iBinX << ":" << iBinY << " is " << currBinNew << endl;
                    cout << "Old iBinX:iBinY " << iBinX << ":" << iBinY << " is " << currBinOld << endl;
                }
                currRatio = BinRatioLimit(currBinNew, currBinOld, onlyExcNew);
                if (whichKind == -1 || whichKind == onlyExcNew) {
                    outHist->SetBinContent(iBinX, iBinY, currRatio);
                }
                else {
                    outHist->SetBinContent(iBinX, iBinY, -1);
                }
            }
            else {
                outHist->SetBinContent(iBinX, iBinY, -1);
            }
        }
    }
    return outHist;
}

void SetLimRatio(TH2F * inHistRatio, TH2F * inHistNew, TH2F * inHistOld, int whichKind = -1, bool doVerb = 0) {
    int nBinsX = inHistRatio->GetNbinsX();
    int nBinsY = inHistRatio->GetNbinsY();
    
    float currBinNew, currBinOld;
    float currMassStop, currMassLSP;
    float maxMassDiff = 99;
    
    int onlyExcNew;
    
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        currMassStop = inHistRatio->GetXaxis()->GetBinCenter(iBinX);
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            currMassLSP = inHistRatio->GetYaxis()->GetBinCenter(iBinY);
            if (currMassStop - currMassLSP > maxMassDiff) {
                currBinNew = inHistNew->GetBinContent(iBinX, iBinY);
                currBinOld = inHistOld->GetBinContent(iBinX, iBinY);
                
                onlyExcNew = RatioExclusionStatus(currBinNew, currBinOld);
                
                if (doVerb) {
                    cout << "New iBinX:iBinY " << iBinX << ":" << iBinY << " is " << currBinNew << endl;
                    cout << "Old iBinX:iBinY " << iBinX << ":" << iBinY << " is " << currBinOld << endl;
                }
                
                if (whichKind != -1 && whichKind != onlyExcNew) {
                    inHistRatio->SetBinContent(iBinX, iBinY, -1E3);
                }
            }
            else {
                inHistRatio->SetBinContent(iBinX, iBinY, -1E3);
            }
        }
    }
}
TH2F * LimitRatioHist_vers2(TH2F * inHistNew, TH2F * inHistOld, TString addHistName = "", bool doConsistency = false, int whichKind = -1, bool doVerb = 0) {

    TH2F * outLimRatioHist = TwoDeeRatio(inHistOld, inHistNew, addHistName, doConsistency);
    SetLimRatio(outLimRatioHist, inHistNew, inHistOld, whichKind, doVerb);
    
    return outLimRatioHist;
}

TGraphErrors * GrabDeltaMLimit(TH2F * inputLimHist, int deltaM, bool doLog) {
    int nBinsX = inputLimHist->GetNbinsX();
    int nBinsY = inputLimHist->GetNbinsY();
    
    int startBinX = inputLimHist->GetXaxis()->FindBin(deltaM); //assuming 0 LSP mass
    int nPoints = nBinsX + 1 - startBinX;
    
    TGraphErrors * outGraph = new TGraphErrors(nPoints);
    TString nameOutGraph = inputLimHist->GetName() + TString("_1DLimGraph_DeltM");
    nameOutGraph += deltaM;
    outGraph->SetName(nameOutGraph);
    
    float currBinContent, currBinErr;
    float currMassStop;
    for (int iX = 0; iX < nPoints; ++iX) {
        currMassStop = inputLimHist->GetXaxis()->GetBinCenter(iX + startBinX);
        currBinContent = inputLimHist->GetBinContent(iX + startBinX, iX + 1);
        currBinErr = inputLimHist->GetBinError(iX + startBinX, iX + 1);
        if (doLog) {
            currBinContent = TMath::Log(currBinContent);
            currBinErr = TMath::Log(currBinErr);
        }
        outGraph->SetPoint(iX, currMassStop, currBinContent);
        outGraph->SetPointError(iX, 0, currBinErr);
    }
    return outGraph;
}


void LimitHist(TH2F * &inputSigStrengthHist, vector<TH2F *> * inputVecMT2CutHists, TString inputFile, TString inputNameSigStrengthHist, vector<TString> * vecHistNames_MT2Cut, int binSize, bool doVerb = 0) {
    //Function to read the text file containing limits
    //Assumption is that text files are in format StopMass:LSPMass:CVLimit:OneSigUpLimit:OneSigDownLimit:MT2llCut:MT2lblbCut
    //The "MT2ll Cut" and "MT2lblbCut" only make sense in the context of the dilepton Cut and Count limits
    //As well, for frequentist limits the OneSigUp/OneSigDown limits are the *uncertainty on the calculation*
    //while for Asymptotic they are the *uncertainty on the limit*
    //So for Frequentist limits you need access as well to files containing OneSigUp/OneSigDown limits
    int numMT2CutHists = 2;
    inputVecMT2CutHists->resize(numMT2CutHists);
    
    float thresh = 9998;
    float cvLimit;
    
    if (doVerb) cout << "going to try to open inputFile " << inputFile << endl;
    
    vector<float> vecStopMass, vecLSPMass;
    vector<float> vecLimit, vecLimitOneSigUp, vecLimitOneSigDown;
    vector<float> vecMT2llCut, vecMT2lblbCut;
    
    ifstream inputFileStream(inputFile);
    if (!inputFileStream) {
        cout << "file: " << inputFile << " doesn't exist apparently!" << endl;
        return;
    }
    string line, field;
    int lineCounter = 0;
    int fieldCounter;
    while (!(inputFileStream.eof())) {
        lineCounter++;
        getline(inputFileStream, line);
        stringstream ss( line );
        fieldCounter = 0;
        while (std::getline(ss, field, ':' )) {
            stringstream fs ( field );
            float currField = 0.0;
            fs >> currField;
            if (doVerb) {
                cout << "For line: " << lineCounter << " and Field # " << fieldCounter << ", currField is " << currField << endl;
            }
            switch (fieldCounter) {
                case 0:
                    vecStopMass.push_back(currField);
                    break;
                case 1:
                    vecLSPMass.push_back(currField);
                    break;
                case 2:
                    vecLimit.push_back(currField);
                    cvLimit = currField;
                    break;
                case 3:
                    if (cvLimit > thresh) {
                        currField = cvLimit;
                    }
                    vecLimitOneSigUp.push_back(currField);
                    break;
                case 4:
                    if (cvLimit > thresh) {
                        currField = cvLimit;
                    }
                    vecLimitOneSigDown.push_back(currField);
                    break;
                case 5:
                    vecMT2llCut.push_back(currField);
                    break;
                case 6:
                    vecMT2lblbCut.push_back(currField);
                    break;
                default:
                    cout << "way too many fields " << fieldCounter << endl;
                    break;
            }
            fieldCounter++;
        }
    }
    int nStopMass = 1 + (800 - 100) / binSize;
    int nLSPMass = 1 + (700 - 0) / binSize;
    float stopMassLB = 100 - (binSize * 0.5);
    float stopMassUB = 800 + (binSize * 0.5);
    float LSPMassLB = 0 - (binSize * 0.5);
    float LSPMassUB = 700 + (binSize * 0.5);
    
    inputSigStrengthHist = new TH2F(inputNameSigStrengthHist, "; M_{stop} [GeV]; M_{LSP} [GeV]", nStopMass, stopMassLB, stopMassUB, nLSPMass, LSPMassLB, LSPMassUB);
    for (unsigned int iHist = 0; iHist < vecHistNames_MT2Cut->size(); ++iHist) {
        inputVecMT2CutHists->at(iHist) = new TH2F(vecHistNames_MT2Cut->at(iHist), "; M_{stop} [GeV]; M_{LSP} [GeV]", nStopMass, stopMassLB, stopMassUB, nLSPMass, LSPMassLB, LSPMassUB);
    }
    
    int binX, binY;
    for (unsigned int iPoint = 0; iPoint < vecStopMass.size(); ++iPoint) {
        binX = inputSigStrengthHist->GetXaxis()->FindBin(vecStopMass[iPoint]);
        binY = inputSigStrengthHist->GetYaxis()->FindBin(vecLSPMass[iPoint]);
        inputSigStrengthHist->SetBinContent(binX, binY, vecLimit[iPoint]);
        inputSigStrengthHist->SetBinError(binX, binY, TMath::Abs(vecLimitOneSigUp[iPoint] - vecLimit[iPoint]));
        if (vecMT2llCut.size() > iPoint + 1) {
            // temp solution to deal with vectors not being filled
            inputVecMT2CutHists->at(0)->SetBinContent(binX, binY, vecMT2llCut[iPoint]);
            inputVecMT2CutHists->at(1)->SetBinContent(binX, binY, vecMT2lblbCut[iPoint]);
        }
    }
}

TString GetCanvasName(int index, int typeCanvas) {
    TString nameSigExcl[3] = {"c_SigExclusionCV", "c_SigExclusionOneSigUp", "c_SigExclusionOneSigDown"};
    TString nameSigStrength[3] = {"c_SigStrengthCV", "c_SigStrengthOneSigUp", "c_SigStrengthOneSigDown"};
    TString nameRatioLimit[3] = {"c_RatioLimitCV", "c_RatioLimitOneSigUp", "c_RatioLimitOneSigDown"};
    TString nameMT2Cut[3] = {"c_MT2llCut", "c_MT2lblbCut", "c_MT2bb_ZMETCut"};
    
    TString * arrToUse = 0;
    
    switch (typeCanvas) {
        case 0:
            arrToUse = nameSigExcl;
            break;
        case 1:
            arrToUse = nameSigStrength;
            break;
        case 2:
            arrToUse = nameRatioLimit;
            break;
        case 3:
            arrToUse = nameMT2Cut;
            break;
        default:
            arrToUse = 0;
            break;
    }
    if (arrToUse == NULL) {
        cout << "index needs to be between 0 and 3, it is " << index << endl;
    }
    return arrToUse[index];
}

void SetHistExclusion_2DSUSY(TH2F * inputHist, bool doVerb, float threshExclusion = 1.0, float maxMassDiff = 99) {
    int nBinsX = inputHist->GetNbinsX();
    int nBinsY = inputHist->GetNbinsY();
    float currBinContent;
    float currMassStop, currMassLSP;
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        currMassStop = inputHist->GetXaxis()->GetBinCenter(iBinX);
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            currMassLSP = inputHist->GetYaxis()->GetBinCenter(iBinY);
            if (currMassStop - currMassLSP < maxMassDiff) continue;
            currBinContent = inputHist->GetBinContent(iBinX, iBinY);
            if (currBinContent < threshExclusion) {
                if (doVerb) {
                    cout << "currMassStop " << currMassStop << endl;
                    cout << "currMassLSP " << currMassLSP << endl;
                    cout << "currLimit " << currBinContent << endl;
                }
                if (currBinContent < 1E-6) {
                    cout << "continuing because limit is too low, implying error in reading in file! " << endl;
                    continue;
                }
                inputHist->SetBinContent(iBinX, iBinY, 1);
                if (iBinY == 1) {
                    //you're on the bottom so also try setting the underflow bin to be 1
                    inputHist->SetBinContent(iBinX, 0, 1);
                }
            }
            else {
                inputHist->SetBinContent(iBinX, iBinY, 0);
            }
        }
    }
}

void SetHistExclusion(TH1 * inputHist, float threshExclusion = 1.0) {
    int nBinsX = inputHist->GetNbinsX();
    int nBinsY = inputHist->GetNbinsY();
    int nBinsZ = inputHist->GetNbinsZ();
    
    float currBinContent;
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            for (int iBinZ = 1; iBinZ <= nBinsZ; ++iBinZ) {
                currBinContent = inputHist->GetBinContent(iBinX, iBinY, iBinZ);
                if (currBinContent < threshExclusion) {
                    inputHist->SetBinContent(iBinX, iBinY, iBinZ, 1);
                }
                else {
                    inputHist->SetBinContent(iBinX, iBinY, iBinZ, 0);
                }
            }
        }
    }
}

void SetStyleHist(TH2F * inHist, int whichKind = -1) {
//    Color_t colorMarker[3] = {kWhite, kGray, kBlack};
//    Color_t colorMarker[3] = {kWhite, kOrange-5, kGreen - 7};
//    Color_t colorMarker[3] = {kGreen - 7, kOrange-5, kWhite};
//    Color_t colorMarker[4] = {kGray+2, kOrange-5, kWhite, kGreen - 4};
    Color_t colorMarker[4] = {kGray+2, kOrange-5, kAzure + 6, kGreen - 4};
    Size_t markerSize = 0.8;
    inHist->SetMarkerColor(colorMarker[whichKind + 1]);
    inHist->SetMarkerSize(markerSize);
    inHist->SetFillColor(colorMarker[whichKind + 1]);
}
