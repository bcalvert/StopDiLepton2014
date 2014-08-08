#include <vector>
#include "TLegend.h"
//#include <boost/format.hpp>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include "TRegexp.h"
using namespace std;

typedef pair<int, int> intBounds;

vector<TH1F *> * sortedVector(vector<TH1F *> * inputVector, vector<int> * whichOrder) {
    int currCounter = 0;
    vector<TH1F *> * outVec = new vector<TH1F *>;
    for (unsigned int iBaseCount = 0; iBaseCount < inputVector->size(); ++iBaseCount) {
        currCounter = iBaseCount;
        for (unsigned int iCount = 0; iCount < inputVector->size(); ++iCount) {
            if (whichOrder->at(iCount) == currCounter) outVec->push_back(inputVector->at(iCount));
        }
    }
    return outVec;
}

vector<float> * ScaleBackVecCalc(vector<TFile *> * inputFiles) {
    vector<float> * outVec = new vector<float>;
    TString mcplot = "h_nVtx_inclusive";
    TString mcplot_preRW = "h_nVtx_preRW_inclusive";
    TString fileName;
    TH1F * nVtxOrigHist;
    TH1F * nVtxNewHist;
    int NBinsX;
    float scaleBack;
    for (unsigned int i = 0; i < inputFiles->size(); ++i) {
        fileName = inputFiles->at(i)->GetName();
        //        if (fileName.Contains("Data")) continue;
        nVtxOrigHist = (TH1F*) inputFiles->at(i)->Get(mcplot_preRW);
        nVtxNewHist = (TH1F*) inputFiles->at(i)->Get(mcplot);
        NBinsX = nVtxOrigHist->GetNbinsX();
        scaleBack = (float) nVtxOrigHist->Integral(1, NBinsX + 1) / nVtxNewHist->Integral(1, NBinsX + 1);
        std::cout << "scaleBack " << scaleBack << std::endl;
        outVec->push_back(scaleBack);
    }
    return outVec;
}

vector<float> * ScaleBackVecCalc_Recoil(vector<TFile *> * inputFiles, bool doDoubleCorr) {
    vector<float> * outVec = new vector<float>;
    TString mcplot = "h_SmearMT2ll_inclusive";
    if (doDoubleCorr) {
        mcplot = "h_SmearMT2ll_genRecoilRWShiftDown_inclusive";
    }
    TString mcplot_preRW = "h_SmearMT2ll_genRecoilRWShiftUp_inclusive";
    TString fileName;
    TH1F * RecoilOrigHist;
    TH1F * RecoilNewHist;
    int NBinsX;
    float scaleBack;
    for (unsigned int i = 0; i < inputFiles->size(); ++i) {
        fileName = inputFiles->at(i)->GetName();
        if (fileName.Contains("Data")) {
            scaleBack = 1.0;
        }
        else {
            RecoilOrigHist = (TH1F*) inputFiles->at(i)->Get(mcplot_preRW);
            RecoilNewHist = (TH1F*) inputFiles->at(i)->Get(mcplot);
            NBinsX = RecoilOrigHist->GetNbinsX();
            scaleBack = (float) RecoilOrigHist->Integral(1, NBinsX + 1) / RecoilNewHist->Integral(1, NBinsX + 1);
            std::cout << "scaleBack " << scaleBack << std::endl;
        }
        outVec->push_back(scaleBack);
    }
    return outVec;
}

void SignalSkimEfficiencyCalc(vector<float> * scaleBackVec, SampLoadSettings * inSLS, float intLumi, bool doVerb = false) {
    TFile * inStopNormFile;
    TH1 * inNormHist;
    int numDims;
    TAxis * xAxis, * yAxis, * zAxis;
    int xBin, yBin, zBin;
    TString histBase = inSLS->customPathSignalNorm;
    if (doVerb) {
        cout << "histBase " << histBase << endl;
    }
    TString fileGrabString;
    TString histGrabString;
    float scaleFactor = inSLS->prefixT2tt.Contains("LeptonFilter") ? 0.53 : 1.0;
    if (doVerb) {
        cout << "scaleFactor " << scaleFactor << endl;
    }
    float estOrigNum;
    for (unsigned int iSkimEff = 0; iSkimEff < inSLS->vecStopMassGrab.size(); ++iSkimEff) {
        xBin = -1;
        yBin = -1;
        zBin = -1;
        
        numDims = inSLS->prefixT2tt.Contains("T2tt") ? 2 : 3;
        fileGrabString = histBase;
        fileGrabString += inSLS->prefixT2tt;
        fileGrabString += ".root";
        inStopNormFile =  TFile::Open(fileGrabString);
        histGrabString = "h_SUSY";
        histGrabString += numDims;
        histGrabString += "DMassEventCount";
        inNormHist = (TH1 *) inStopNormFile->Get(histGrabString);
        xAxis = inNormHist->GetXaxis();
        xBin = xAxis->FindBin(inSLS->vecStopMassGrab[iSkimEff]);
        yAxis = inNormHist->GetYaxis();
        yBin = yAxis->FindBin(inSLS->vecChi0MassGrab[iSkimEff]);
        
        if (numDims > 2) {
            zAxis = inNormHist->GetZaxis();
            zBin = zAxis->FindBin(inSLS->vecCharginoMassGrab[iSkimEff]);
        }
        else {
            zBin = 1;
        }
        if (xBin < 1 || xBin > xAxis->GetNbins() || yBin < 1 || yBin > yAxis->GetNbins()) {            
            if (numDims > 2) {                
                if (zBin < 1 || zBin > zAxis->GetNbins()) {
                    estOrigNum = 0;
                    cout << "issue with bins " << endl;
                    cout << "xAxis->GetNbins() " << xAxis->GetNbins() << endl;
                    cout << "yAxis->GetNbins() " << yAxis->GetNbins() << endl;
                    cout << "zAxis->GetNbins() " << zAxis->GetNbins() << endl;
                    cout << "vecStopMassGrab->at(iSkimEff) " << inSLS->vecStopMassGrab[iSkimEff] << " => xBin = " << xBin << endl;
                    cout << "vecChi0MassGrab->at(iSkimEff) " << inSLS->vecChi0MassGrab[iSkimEff] << " => yBin = " << yBin << endl;
                    cout << "vecCharginoMassGrab->at(iSkimEff) " << inSLS->vecCharginoMassGrab[iSkimEff] << " => zBin = " << zBin << endl;
                }
            }
        }
        else {
            estOrigNum = inNormHist->GetBinContent(xBin, yBin, zBin);        
        }
        if (doVerb) {
            cout << "intLumi " << intLumi << endl;
            cout << "estOrigNum " << estOrigNum << endl;
        }
        cout << "SF = intLumi / estOrigNum " << (intLumi / estOrigNum) << endl;            
        scaleBackVec->push_back(intLumi / estOrigNum);
    }

}
vector<TH1 *> * IndividualHistSysts(TFile * inputFile, TH1 * centValTH1Hist, TString histPlotName, vector<SampleT> * inputSubSampVec, vector<int> * subSampChanIDs, vector<SystT> * systVec, int whichNTuple) {
    TString fileName, systName, mcGrabName, currHistGrabName;
    TH1 * currHist, * currHistPatsy;
    TH1 * systCompHist;
    TString mcSystPlotName, systCompHistName;
    bool grabSyst;
    
    vector<TH1 *> * outVec = new vector<TH1 *>;
    fileName = inputFile->GetName();
    for (unsigned int iSyst = 0; iSyst < systVec->size(); ++iSyst) {
        grabSyst = true;
        systCompHist = NULL;
        cout << "iSyst " << iSyst << endl;
        systName = systVec->at(iSyst).name;
        cout << "systName " << systName << endl;
        cout << "mcGrabName " << mcGrabName << endl;
        if (!mcGrabName.Contains("MT2ll") && (systName.Contains("MT2ll") || systName.Contains("MT2UncES"))) continue;
        if (systName.Contains("genStop")) grabSyst = false;
        if (systName.Contains("genTop")) {
            if (!fileName.Contains("TTBar")) grabSyst = false;
            if (whichNTuple == 1 && !fileName.Contains("Sig")) grabSyst = false;
        }
        if (systName.Contains("MT2ll")) {
            continue;
            /*
             if (!allMT2llSystematic && !fileName.Contains("TTBar")) {
             grabSyst = false;
             }
             else {
             grabSyst = true;
             }
             */
        }
        if (!grabSyst) {
            currHist = centValTH1Hist;
        }            
        else {  
            for (unsigned int iSamp = 0; iSamp < subSampChanIDs->size(); ++iSamp) {
                mcGrabName = "";
                mcGrabName += histPlotName;
                mcSystPlotName = mcGrabName;
                mcSystPlotName += systName;
                mcGrabName += inputSubSampVec->at(subSampChanIDs->at(iSamp)).histNameSuffix;
                mcSystPlotName += inputSubSampVec->at(subSampChanIDs->at(iSamp)).histNameSuffix;
                cout << "mcSystPlotName " << mcSystPlotName << endl;                    
                currHistPatsy = (TH1 *) inputFile->Get(mcSystPlotName);   
                cout << "currHist integral " << currHistPatsy->Integral() << endl;
                if (currHist == NULL) {
                    currHist = currHistPatsy;
                }
                else {
                    currHist->Add(currHistPatsy);
                }
            }
        }
        outVec->push_back(currHist);
    }
    return outVec;    
}
vector<TH1 *> * IndividualHistSysts(TFile * inputFile, TH1 * centValTH1Hist, TString histPlotName, TString subSampName, vector<SystT> * systVec, int whichNTuple) {    
    TString fileName, systName, mcGrabName, currHistGrabName;
    TH1 * currHist;
    TH1 * systCompHist;
    TString mcSystPlotName, systCompHistName;
    bool grabSyst;
    vector<TH1 *> * outVec = new vector<TH1 *>;
    fileName = inputFile->GetName();
    for (unsigned int iSyst = 0; iSyst < systVec->size(); ++iSyst) {
        grabSyst = true;
        systCompHist = NULL;
        mcGrabName = "";
        mcGrabName += histPlotName;
        cout << "iSyst " << iSyst << endl;
        systName = systVec->at(iSyst).name;
        cout << "systName " << systName << endl;
        cout << "mcGrabName " << mcGrabName << endl;
        if (!mcGrabName.Contains("MT2ll") && (systName.Contains("MT2ll") || systName.Contains("MT2UncES"))) continue;
        if (systName.Contains("genStop")) grabSyst = false;
        mcSystPlotName = mcGrabName;
        mcSystPlotName += systName;
        mcGrabName += subSampName;
        mcSystPlotName += subSampName;
        cout << "mcSystPlotName " << mcSystPlotName << endl;
        if (systName.Contains("genTop")) {
            if (!fileName.Contains("TTBar")) grabSyst = false;
            if (whichNTuple == 1 && !fileName.Contains("Sig")) grabSyst = false;
        }
        if (systName.Contains("MT2ll")) {
            continue;
            /*
            if (!allMT2llSystematic && !fileName.Contains("TTBar")) {
                grabSyst = false;
            }
            else {
                grabSyst = true;
            }
            */
        }
        if (!grabSyst) {
            currHist = centValTH1Hist;
        }
        else {
            currHist = (TH1 *) inputFile->Get(mcSystPlotName);
        }
        cout << "centValTH1Hist Bin Content 1 " << centValTH1Hist->GetBinContent(1) << endl;
        cout << "centValTH1Hist Bin Content 2 " << centValTH1Hist->GetBinContent(2) << endl;
        cout << "currHist Bin Content 1 " << currHist->GetBinContent(1) << endl;
        cout << "currHist Bin Content 2 " << currHist->GetBinContent(2) << endl;
        outVec->push_back(currHist);
    }
    return outVec;
}
void CombineSystInfo(vector<TH1F *> * MCAddHists, vector<int> * sampleStartPositions, vector<SampleSystematicsInfo> * inVecSSI, vector<SampleSystematicsInfo> * outVecSSI) {
    for (unsigned int iSamp = 0; iSamp < sampleStartPositions->size() - 1; ++iSamp) {        
        SampleSystematicsInfo currSSI;
        currSSI.Name = MCAddHists->at(iSamp)->GetName();
        currSSI.StatError.SetStatError(0, 0);
        currSSI.SetVecSizes(inVecSSI->at(0).SystError.size());
        cout << "name " << currSSI.Name << endl;
        cout << "iSamp: " << iSamp << endl;
        for (int iMCSpec = sampleStartPositions->at(iSamp); iMCSpec < sampleStartPositions->at(iSamp + 1); ++iMCSpec) {
            cout << "iMCSpec " << iMCSpec << endl;
            cout << "centVal " << inVecSSI->at(iMCSpec).StatError.centVal << endl;
            cout << "statError " << inVecSSI->at(iMCSpec).StatError.upError << endl;
            currSSI.StatError.centVal += inVecSSI->at(iMCSpec).StatError.centVal;
            currSSI.StatError.upError += inVecSSI->at(iMCSpec).StatError.upError * inVecSSI->at(iMCSpec).StatError.upError;
            for (int iSyst = 0; iSyst < currSSI.SystError.size(); ++iSyst) {
                if (iSyst == 0) {
                    currSSI.SystError[iSyst].centVal = 0;
                    currSSI.SystError[iSyst].upError = 0;
                    currSSI.SystError[iSyst].downError = 0;
                }                
                cout << "Syst Name " << inVecSSI->at(iMCSpec).SystName[iSyst] << endl;
                cout << "curr centval " << inVecSSI->at(iMCSpec).SystError[iSyst].centVal << endl;
                cout << "curr upError " << inVecSSI->at(iMCSpec).SystError[iSyst].upError << endl;
                cout << "curr downError " << inVecSSI->at(iMCSpec).SystError[iSyst].downError << endl;
                currSSI.SystError[iSyst].centVal += inVecSSI->at(iMCSpec).SystError[iSyst].centVal;
                currSSI.SystError[iSyst].upError += inVecSSI->at(iMCSpec).SystError[iSyst].upError;
                currSSI.SystError[iSyst].downError += inVecSSI->at(iMCSpec).SystError[iSyst].downError;
                currSSI.SystName[iSyst] = inVecSSI->at(iMCSpec).SystName[iSyst];
            }
        }
        currSSI.StatError.SetStatError(currSSI.StatError.centVal, TMath::Sqrt(currSSI.StatError.upError));
        outVecSSI->push_back(currSSI);
    }
}

void PrintSystInfo(vector<float> * UpErrVec, vector<float> * DownErrVec, vector<TH1F *> * MCAddHists, vector<int> * sampleStartPositions) {
    float TotUpErr, TotDownErr;
    cout << "UpErrVec->size() " << UpErrVec->size() << endl;
    cout << "DownErrVec->size() " << DownErrVec->size() << endl;
    cout << "sampleStartPositions size " << sampleStartPositions->size() << endl;
    cout << "MCAddHists size " << MCAddHists->size() << endl;
    for (unsigned int iSamp = 0; iSamp < sampleStartPositions->size() - 1; ++iSamp) {
        TotUpErr = 0.;
        TotDownErr = 0.;
        for (int iMCSpec = sampleStartPositions->at(iSamp); iMCSpec < sampleStartPositions->at(iSamp + 1); ++iMCSpec) {
            TotUpErr += UpErrVec->at(iMCSpec);
            TotDownErr += DownErrVec->at(iMCSpec);
        }
        cout << "FullSyst UpError on Signal Bin for histogram " << MCAddHists->at(iSamp)->GetName() << " is " << TotUpErr << endl;
        cout << "FullSyst DownError on Signal Bin for histogram " << MCAddHists->at(iSamp)->GetName() << " is " << TotDownErr << endl;
    }
}

void VectorDist(vector<TH1F *> * inputVec, bool doMean, bool doGauss, TH1F * &outHist, float NumRMSFromMean) {
    TString outName = inputVec->at(0)->GetName();
    if (doMean) {
        outName += "Mean";   
    }
    else {
        outName += "RMS";   
    }
    TH1F * vectorHist;
    int VecSize = inputVec->size();
    for (int i = 0; i < VecSize; ++i) {
        vectorHist = inputVec->at(i);
        if (doMean) { 
            outHist->SetBinContent(i+1, vectorHist->GetMean());
            outHist->SetBinError(i+1, vectorHist->GetMeanError());            
        }
        else { 
            outHist->SetBinContent(i+1, vectorHist->GetRMS());
            outHist->SetBinError(i+1, vectorHist->GetRMSError());                        
        }
    }
}
vector<TH1F *> * OneDProjectionReturnVec(TH2F * inputHist, TString nameBase) {
    TString name = nameBase;
    vector<TH1F *> * outHistVector = new vector<TH1F *>;
    TH1F * projHist;
    int NBinsX = inputHist->GetNbinsX();
    for (int ib = 1; ib <= NBinsX; ++ib) {
        name += ib;
        projHist = (TH1F *) inputHist->ProjectionY(name,ib,ib);
        outHistVector->push_back(projHist);
    }
    return outHistVector;
}

void VectorDistMakerMean(TH1F * &outHistMean, vector<TH1F *> * &outVec, TH2F * inputHist, TString histMeanName, TString histVecName) {
    outHistMean = (TH1F *) inputHist->ProjectionX(histMeanName);
    outVec = OneDProjectionReturnVec(inputHist, histVecName);
    VectorDist(outVec, 1, 0, outHistMean, 2);
}
TH1F * HistogramAdderMCOneDee(vector<TH1 *> * mcIndHistCentValVec) {
    TString cloneName = mcIndHistCentValVec->at(0)->GetName() + TString("_1DMCComp");;
    TH1F * outHist = (TH1F *) mcIndHistCentValVec->at(0)->Clone(cloneName);
    for (unsigned int iMC = 1; iMC < mcIndHistCentValVec->size(); ++iMC) {
        outHist->Add((TH1F*) mcIndHistCentValVec->at(iMC));
    }  
    return outHist;
}
TH2F * HistogramAdderMCTwoDee(vector<TH1 *> * mcIndHistCentValVec) {
    TString cloneName = mcIndHistCentValVec->at(0)->GetName() + TString("_2DMCComp");;
    TH2F * outHist = (TH2F *) mcIndHistCentValVec->at(0)->Clone(cloneName);
    for (unsigned int iMC = 1; iMC < mcIndHistCentValVec->size(); ++iMC) {
        outHist->Add((TH2F*) mcIndHistCentValVec->at(iMC));
    }  
    return outHist;
}

TH2F * HistogramAdderDataTwoDee(vector<TH1 *> * DataHistVec) {
    TString cloneName = DataHistVec->at(0)->GetName() + TString("_DataComp");;
    TH2F * outHist = (TH2F *) DataHistVec->at(0)->Clone(cloneName);
    for (unsigned int iData = 1; iData < DataHistVec->size(); ++iData) {
        outHist->Add((TH2F*) DataHistVec->at(iData));
    }  
    return outHist;
}
TH2F * SystHistFinderTwoDee(vector<TH1 *> * vecSystHists, TString searchString) {
    TString name;
    int whichIndex;
    for (unsigned int iSyst = 0; iSyst < vecSystHists->size(); ++iSyst) {
        name = vecSystHists->at(iSyst)->GetName();
        if (name.Contains(searchString)) whichIndex = iSyst;
    }
    return (TH2F *) vecSystHists->at(whichIndex);
}





void HistogramVecGrabber(vector<TFile *> * inputFiles, vector<TH2F *> * dataHistVec, vector<TH2F *> * mcIndHistCentValVec, vector<TH2F *> * mcCompHistSystVec, vector<float> * nVtxBackScaleVec, vector<SystT> * systVec, TString dataPlotName, TString mcPlotName, TString subSampName, int RBNX, int RBNY, int RBNZ, bool doOverflow, bool doUnderflow, bool doSyst, bool useDDEstimate, float scaleFacTTBar, float scaleLumi, bool allMT2llSystematic, int whichNTuple) {
    int NBinsX, NBinsY, NBinsZ;
    TString fileName, systName, mcGrabName;
    TH2F * currHist;
    TH2F * systCompHist = NULL;
    //    cout << "inputFiles size " << inputFiles->size() << endl;
    cout << "nVtxBackScale size " << nVtxBackScaleVec->size() << endl;
    TString mcSystPlotName, systCompHistName;
    for (unsigned int k = 0; k < inputFiles->size(); ++k) {
        mcGrabName = "";
        fileName = inputFiles->at(k)->GetName();
        cout << "fileName" << " is " << fileName << endl;
        if (fileName.Contains("Data")) {
            currHist = (TH2F *) inputFiles->at(k)->Get(dataPlotName);
            NBinsX = currHist->GetNbinsX();
            NBinsY = currHist->GetNbinsY();
            NBinsZ = currHist->GetNbinsZ();
            currHist->RebinX(RBNX);
            if (NBinsY > 1) {
                if (NBinsZ > 1) {
                }
                else {
                }
            }
            else {
            }
            dataHistVec->push_back(currHist);
        }
        else {
            mcGrabName += mcPlotName;
            mcGrabName += subSampName;
            currHist = (TH2F *) inputFiles->at(k)->Get(mcGrabName);
            fileName = inputFiles->at(k)->GetName();
            currHist->Scale(scaleLumi);
            currHist->Scale(nVtxBackScaleVec->at(k)); // correct for PURW changes to integral
            if (useDDEstimate && fileName.Contains("TTBar")&& !fileName.Contains("VecBoson")) currHist->Scale(scaleFacTTBar);
            NBinsX = currHist->GetNbinsX();
            NBinsY = currHist->GetNbinsY();
            NBinsZ = currHist->GetNbinsZ();
            currHist->RebinX(RBNX);
            if (NBinsY > 1) {
                if (NBinsZ > 1) {
                }
                else {
                }
            }
            else {
            }
            mcIndHistCentValVec->push_back(currHist);
        }
    }
    bool grabSyst;
    if (doSyst) {
        for (unsigned int j = 0; j < systVec->size(); ++j) {
            systCompHist = NULL;
            mcGrabName = "";
            mcGrabName += mcPlotName;
            //            cout << "j " << j << endl;
            //        if (!systVec->at(j).doXSyst) continue;
            systName = systVec->at(j).name;
            cout << "systName " << systName << endl;
            cout << "mcGrabName " << mcGrabName << endl;
            if (!mcGrabName.Contains("MT2ll") && (systName.Contains("MT2ll") || systName.Contains("MT2UncES"))) continue;
            mcSystPlotName = mcGrabName;
            mcSystPlotName += systName;
            mcGrabName += subSampName;
            mcSystPlotName += subSampName;
            if (systName.Contains("genStop")) continue;
            for (unsigned int k = 0; k < inputFiles->size(); ++k) {
                grabSyst = true;
                fileName = inputFiles->at(k)->GetName();
                cout << "fileName " << fileName << endl;
                cout << "mcSystPlotName " << mcSystPlotName << endl;
                if (fileName.Contains("Data")) continue;
                if (systName.Contains("genTop")) {
                    if (!fileName.Contains("TTBar")) grabSyst = false;
                    if (whichNTuple == 1 && !fileName.Contains("Sig")) grabSyst = false;
                }
                if (!allMT2llSystematic && systName.Contains("MT2ll") && !fileName.Contains("TTBar")) {
                    grabSyst = false;
                }
                else {
                    grabSyst = true;
                }    
                if (grabSyst) {
                    currHist = (TH2F *) inputFiles->at(k)->Get(mcSystPlotName);                    
                    currHist->Scale(scaleLumi);
                    currHist->Scale(nVtxBackScaleVec->at(k)); // correct for PURW changes to integral
                    if (useDDEstimate && fileName.Contains("TTBar")) currHist->Scale(scaleFacTTBar);
                }
                else {
                    currHist = (TH2F *) inputFiles->at(k)->Get(mcGrabName);
                }
                NBinsX = currHist->GetNbinsX();
                NBinsY = currHist->GetNbinsY();
                NBinsZ = currHist->GetNbinsZ();
                currHist->RebinX(RBNX);
                if (NBinsY > 1) {
                    if (NBinsZ > 1) {
                    }
                    else {
                    }
                }
                else {
                }
                if (systCompHist == NULL) {
                    systCompHistName = currHist->GetName();
                    systCompHistName += "_Comp";
                    systCompHist = (TH2F *) currHist->Clone(systCompHistName);
                }
                else {
                    //                    cout << "adding syst " << endl;
                    //                    cout << "systCompHist nBins " << systCompHist->GetNbinsX() << endl;
                    //                    cout << "currHist nBins " << currHist->GetNbinsX() << endl;
                    systCompHist->Add(currHist);
                }
            }
            mcCompHistSystVec->push_back(systCompHist);
        }
    }
}


void SystGraphMakersIndivSamp(TH1F * inputBaseMCHist, vector<TH1F *> * inputBaseMCSystHists, vector<TGraphAsymmErrors *> * errCompSpecSource, vector<TGraphAsymmErrors *> * errCompSpecSource_pStat, bool doSymErr, bool doSmear, bool isEWK = false) {
    
    TString stringLepEffSF       = "LepEffSF";
    TString stringLepEnSc        = "LepES";
    TString stringJetEnSc        = "JetES";
    TString stringUncEnSc        = "UncES";
    TString stringSpecialUncEnSc = "UncESSpec";
    TString stringBTagSF         = "BTagSF";
    TString stringMT2ll          = "MT2ll";  
    TString stringGenTopRW       = "genRecoilRW";
    TString stringStopXSecUncert = "genStopXSec";
    TString stringJetSmear       = "JetSmear";
    TString stringEWKXSec        = "EWKXSec";
    
    TGraphAsymmErrors * errCompStatCentVal, * errSystQuadSum, * errSystQuadSum_pStat;
    TGraphAsymmErrors * errLepEnSc, * errLepEffSF, * errJetEnSc, * errUncEnSc, * errBTagSF, * errGenRecoilRW, * errStopXSecUncert;
    TGraphAsymmErrors * errJetSmear;
    TGraphAsymmErrors * errEWKXSec;
    
    float normFactor = 0.0;
    
    //Grab btag stuff to then combine
    TGraphAsymmErrors * errBTagEffSF, * errBMisTagSF;
    vector<TGraphAsymmErrors *> errBTagComp;
    TString stringBTagEffSF = "BTagEffSF";
    TString stringBMisTagSF = "BMisTagSF";

    
    TGraphAsymmErrors * errLepEnSc_pStat, * errLepEffSF_pStat, * errJetEnSc_pStat, * errUncEnSc_pStat, * errBTagSF_pStat, * errGenRecoilRW_pStat, * errStopXSecUncert_pStat;
    TGraphAsymmErrors * errJetSmear_pStat;
    TGraphAsymmErrors * errEWKXSec_pStat;        
    
    errCompStatCentVal = ClonePoints(inputBaseMCHist);
    errLepEnSc = GraphSystErrorSet_SingleSource(inputBaseMCHist, inputBaseMCSystHists, stringLepEnSc + TString("Shift"), doSymErr, 0);
    errLepEnSc_pStat = GraphSystErrorSumErrors(errCompStatCentVal, stringLepEnSc, errLepEnSc, inputBaseMCHist);
    errCompSpecSource->push_back(errLepEnSc);
    errCompSpecSource_pStat->push_back(errLepEnSc_pStat);
    
    errJetEnSc = GraphSystErrorSet_SingleSource(inputBaseMCHist, inputBaseMCSystHists, stringJetEnSc + TString("Shift"), doSymErr, 0);
    errJetEnSc_pStat = GraphSystErrorSumErrors(errCompStatCentVal, stringJetEnSc, errJetEnSc, inputBaseMCHist);
    errCompSpecSource->push_back(errJetEnSc);
    errCompSpecSource_pStat->push_back(errJetEnSc_pStat);
    
    errUncEnSc = GraphSystErrorSet_SingleSource(inputBaseMCHist, inputBaseMCSystHists, stringUncEnSc + TString("Shift"), doSymErr, 0);
    errUncEnSc_pStat = GraphSystErrorSumErrors(errCompStatCentVal, stringUncEnSc, errUncEnSc, inputBaseMCHist);
    errCompSpecSource->push_back(errUncEnSc);
    errCompSpecSource_pStat->push_back(errUncEnSc_pStat);
    
    if (doSmear) {
        errJetSmear = GraphSystErrorSet_SingleSource(inputBaseMCHist, inputBaseMCSystHists, stringJetSmear + TString("Shift"), doSymErr, 0);
        errJetSmear_pStat = GraphSystErrorSumErrors(errCompStatCentVal, stringJetSmear, errJetSmear, inputBaseMCHist);
        errCompSpecSource->push_back(errJetSmear);
        errCompSpecSource_pStat->push_back(errJetSmear_pStat);
    }
    
    if (isEWK) {
        normFactor = 0.5;
    }
    errEWKXSec = NormalizationSystGraph(inputBaseMCHist, normFactor);
    errEWKXSec_pStat = GraphSystErrorSumErrors(errCompStatCentVal, stringEWKXSec, errEWKXSec, inputBaseMCHist);
    errCompSpecSource->push_back(errEWKXSec);
    errCompSpecSource_pStat->push_back(errEWKXSec_pStat);
    
    //Grab btag stuff to then combine
    errBTagEffSF = GraphSystErrorSet_SingleSource(inputBaseMCHist, inputBaseMCSystHists, stringBTagEffSF + TString("Shift"), doSymErr, 0);
    errBMisTagSF = GraphSystErrorSet_SingleSource(inputBaseMCHist, inputBaseMCSystHists, stringBMisTagSF + TString("Shift"), doSymErr, 0);
    
    errBTagComp.push_back(errBTagEffSF);
    errBTagComp.push_back(errBMisTagSF);
    
    errBTagSF = GraphSystErrorSumErrors(errCompStatCentVal, stringBTagSF, &errBTagComp, false, inputBaseMCHist);
//    errBTagSF = GraphSystErrorSet_SingleSource(inputBaseMCHist, inputBaseMCSystHists, stringBTagSF + TString("Shift"), doSymErr, 0);
    errBTagSF_pStat = GraphSystErrorSumErrors(errCompStatCentVal, stringBTagSF, errBTagSF, inputBaseMCHist);
    errCompSpecSource->push_back(errBTagSF);
    errCompSpecSource_pStat->push_back(errBTagSF_pStat);
    
    errLepEffSF = GraphSystErrorSet_SingleSource(inputBaseMCHist, inputBaseMCSystHists, stringLepEffSF + TString("Shift"), doSymErr, 0);
    errLepEffSF_pStat = GraphSystErrorSumErrors(errCompStatCentVal, stringLepEffSF, errLepEffSF, inputBaseMCHist);
    errCompSpecSource->push_back(errLepEffSF);
    errCompSpecSource_pStat->push_back(errLepEffSF_pStat);
    
    errGenRecoilRW = GraphSystErrorSet_SingleSource(inputBaseMCHist, inputBaseMCSystHists, stringGenTopRW, doSymErr, 0);
    errGenRecoilRW_pStat = GraphSystErrorSumErrors(errCompStatCentVal, stringGenTopRW, errGenRecoilRW, inputBaseMCHist);
    errCompSpecSource->push_back(errGenRecoilRW);
    errCompSpecSource_pStat->push_back(errGenRecoilRW_pStat);
    
    errStopXSecUncert = GraphSystErrorSet_SingleSource(inputBaseMCHist, inputBaseMCSystHists, stringStopXSecUncert + TString("Shift"), doSymErr, 0);
    errStopXSecUncert_pStat = GraphSystErrorSumErrors(errCompStatCentVal, stringStopXSecUncert, errStopXSecUncert, inputBaseMCHist);
    errCompSpecSource->push_back(errStopXSecUncert);
    errCompSpecSource_pStat->push_back(errStopXSecUncert_pStat);
    
    errSystQuadSum = GraphSystErrorSumErrors(errCompStatCentVal, TString("FullSyst"), errCompSpecSource, false, inputBaseMCHist);
    errSystQuadSum_pStat = GraphSystErrorSumErrors(errCompStatCentVal, TString("FullSyst"), errCompSpecSource, true, inputBaseMCHist);
    errCompSpecSource->push_back(errSystQuadSum);
    errCompSpecSource_pStat->push_back(errSystQuadSum_pStat);
}






void HistogramVecGrabber(vector<TFile *> * inputFiles, vector<TH3F *> * dataHistVec, vector<TH3F *> * mcIndHistCentValVec, vector<TH3F *> * mcCompHistSystVec, vector<float> * nVtxBackScaleVec, vector<SystT> * systVec, TString dataPlotName, TString mcPlotName, TString subSampName, int RBNX, int RBNY, int RBNZ, bool doOverflow, bool doUnderflow, bool doSyst, bool useDDEstimate, float scaleFacTTBar, float scaleLumi, bool allMT2llSystematic, int whichNTuple) {
    int NBinsX, NBinsY, NBinsZ;
    TString fileName, systName, mcGrabName;
    TH3F * currHist;
    TH3F * systCompHist = NULL;
    //    cout << "inputFiles size " << inputFiles->size() << endl;
    //    cout << "nVtxBackScale size " << nVtxBackScaleVec->size() << endl;
    TString mcSystPlotName, systCompHistName;
    for (unsigned int k = 0; k < inputFiles->size(); ++k) {
        mcGrabName = "";
        fileName = inputFiles->at(k)->GetName();
        cout << "fileName" << " is " << fileName << endl;
        if (fileName.Contains("Data")) {
            currHist = (TH3F *) inputFiles->at(k)->Get(dataPlotName);
            NBinsX = currHist->GetNbinsX();
            NBinsY = currHist->GetNbinsY();
            NBinsZ = currHist->GetNbinsZ();
            currHist->RebinX(RBNX);
            if (NBinsY > 1) {
                if (NBinsZ > 1) {
                }
                else {
                }
            }
            else {
            }
            dataHistVec->push_back(currHist);
        }
        else {
            mcGrabName += mcPlotName;
            mcGrabName += subSampName;
            currHist = (TH3F *) inputFiles->at(k)->Get(mcGrabName);
            fileName = inputFiles->at(k)->GetName();
            currHist->Scale(scaleLumi);
            currHist->Scale(nVtxBackScaleVec->at(k)); // correct for PURW changes to integral
            if (useDDEstimate && fileName.Contains("TTBar")) currHist->Scale(scaleFacTTBar);
            NBinsX = currHist->GetNbinsX();
            NBinsY = currHist->GetNbinsY();
            NBinsZ = currHist->GetNbinsZ();
            currHist->RebinX(RBNX);
            if (NBinsY > 1) {
                if (NBinsZ > 1) {
                }
                else {
                }
            }
            else {
            }
            mcIndHistCentValVec->push_back(currHist);
        }
    }
    if (doSyst) {
        for (unsigned int j = 0; j < systVec->size(); ++j) {
            systCompHist = NULL;
            mcGrabName = "";
            mcGrabName += mcPlotName;
            //        if (!systVec->at(j).doXSyst) continue;
            systName = systVec->at(j).name;
            cout << "systName " << systName << endl;
            cout << "mcGrabName " << mcGrabName << endl;
            if (!mcGrabName.Contains("MT2ll") && (systName.Contains("MT2ll") || systName.Contains("MT2UncES"))) continue;
            mcSystPlotName = mcGrabName;
            mcSystPlotName += systName;
            mcGrabName += subSampName;
            mcSystPlotName += subSampName;
            if (systName.Contains("genStop")) continue;
            for (unsigned int k = 0; k < inputFiles->size(); ++k) {
                fileName = inputFiles->at(k)->GetName();
                cout << "fileName " << fileName << endl;
                cout << "mcSystPlotName " << mcSystPlotName << endl;
                if (fileName.Contains("Data")) continue;
                if (systName.Contains("genTop")) {
                    if (!fileName.Contains("TTBar")) continue;
                    if (whichNTuple == 1 && !fileName.Contains("Sig")) continue;
                }                
                if (systName.Contains("genStop") && !fileName.Contains("Stop")) continue;            
                if (systName.Contains("MT2ll") && !fileName.Contains("TTBar")) {
                    currHist = (TH3F *) inputFiles->at(k)->Get(mcGrabName);   
                }
                else {
                    currHist = (TH3F *) inputFiles->at(k)->Get(mcSystPlotName);   
                }        
                //                currHist = (TH3F *) inputFiles->at(k)->Get(mcSystPlotName);   
                currHist->Scale(nVtxBackScaleVec->at(k)); // correct for PURW changes to integral
                if (useDDEstimate && fileName.Contains("TTBar")) currHist->Scale(scaleFacTTBar);
                NBinsX = currHist->GetNbinsX();
                NBinsY = currHist->GetNbinsY();
                NBinsZ = currHist->GetNbinsZ();
                currHist->RebinX(RBNX);
                if (NBinsY > 1) {
                    if (NBinsZ > 1) {
                    }
                    else {
                    }
                }
                else {
                }
                if (systCompHist == NULL) {
                    systCompHistName = currHist->GetName();
                    systCompHistName += "_Comp";
                    systCompHist = (TH3F *) currHist->Clone(systCompHistName);
                }
                else {
                    //                    cout << "adding syst " << endl;
                    //                    cout << "systCompHist nBins " << systCompHist->GetNbinsX() << endl;
                    //                    cout << "currHist nBins " << currHist->GetNbinsX() << endl;
                    systCompHist->Add(currHist);
                }
            }
            mcCompHistSystVec->push_back(systCompHist);
        }
    }
}
void HistogramAdderSyst(vector<TH1F *> * dataHistVec, vector<TH1F *> * mcIndHistCentValVec, vector<TH1F *> * mcCompHistCentValVec, TH1F * &DataComp, TH1F * &MCComp, TH1F * &FracComp, int whichNTuple, bool doAbsRatio, float yAxisRange) {
    /*
     bool cloneTTBar = false;
     bool cloneVV = false;
     bool cloneSingTop = false;
     bool cloneWJ = false;
     bool cloneZDY = false;
     bool cloneStop = false;
     bool cloneQCD = false;
     */
    TString MCName;
    TH1F * TTbarComp, * VVComp, * SingTopComp, * WJComp, * ZDYComp, * QCDComp;
    TH1F * VGComp, * HiggsComp, * RareComp;
    DataComp = (TH1F *) dataHistVec->at(0)->Clone(dataHistVec->at(0)->GetName() + TString("_DataComp"));
    MCComp = (TH1F *) mcIndHistCentValVec->at(0)->Clone(mcIndHistCentValVec->at(0)->GetName() + TString("_MCComp"));
    for (unsigned int i = 1; i < dataHistVec->size(); ++i) {
        DataComp->Add(dataHistVec->at(i));
    }
    TTbarComp = (TH1F *) mcIndHistCentValVec->at(0)->Clone(mcIndHistCentValVec->at(0)->GetName() + TString("_TTbar"));
    VVComp = (TH1F *) mcIndHistCentValVec->at(2)->Clone(mcIndHistCentValVec->at(2)->GetName() + TString("_VV"));
    VVComp->Add(mcIndHistCentValVec->at(3));
    VVComp->Add(mcIndHistCentValVec->at(4));
    SingTopComp = (TH1F *) mcIndHistCentValVec->at(5)->Clone(mcIndHistCentValVec->at(5)->GetName() + TString("_SingTop"));
    WJComp = (TH1F *) mcIndHistCentValVec->at(6)->Clone(mcIndHistCentValVec->at(6)->GetName() + TString("_WJ"));
    ZDYComp = (TH1F *) mcIndHistCentValVec->at(7)->Clone(mcIndHistCentValVec->at(7)->GetName() + TString("_ZDY"));
    if (whichNTuple == 1) {
        QCDComp = (TH1F *) mcIndHistCentValVec->at(8)->Clone(mcIndHistCentValVec->at(8)->GetName() + TString("_QCD"));
    }
    else {
        VGComp = (TH1F *) mcIndHistCentValVec->at(8)->Clone(mcIndHistCentValVec->at(8)->GetName() + TString("_VG"));
        VGComp->Add(mcIndHistCentValVec->at(9));
        HiggsComp = (TH1F *) mcIndHistCentValVec->at(10)->Clone(mcIndHistCentValVec->at(10)->GetName() + TString("_Higgs"));
        HiggsComp->Add(mcIndHistCentValVec->at(11));
        HiggsComp->Add(mcIndHistCentValVec->at(12));
        RareComp = (TH1F *) mcIndHistCentValVec->at(13)->Clone(mcIndHistCentValVec->at(10)->GetName() + TString("_Rare"));
        RareComp->Add(mcIndHistCentValVec->at(14));
    }
    for (unsigned int j = 0; j < mcIndHistCentValVec->size(); ++j) {
        //        MCName = mcIndHistCentValVec->at(j)->GetName();
        if (j != 0) {
            MCComp->Add(mcIndHistCentValVec->at(j));
        }
    }
    if (doAbsRatio) {
        FracComp = (TH1F*) DataComp->Clone("ratioComp");
        FracComp->Divide(FracComp, MCComp, 1, 1, "");
        HistAxisAttSet(FracComp->GetYaxis(), TString("Data/MC"), .15, .54, .14, .011, 1.0 - yAxisRange, 1.0 + yAxisRange); 
    }
    else {
        FracComp = (TH1F*) MCComp->Clone("ratioComp");
        FracComp->Add(DataComp, -1);
        FracComp->Divide(FracComp, DataComp, 1, 1, "");
        HistAxisAttSet(FracComp->GetYaxis(), TString("(MC-Data)/Data"), .15, .54, .14, .011, -1.0 * yAxisRange, 1.0 * yAxisRange);
    }     
    if (whichNTuple == 0) {
        mcCompHistCentValVec->push_back(RareComp);
        mcCompHistCentValVec->push_back(HiggsComp);
        mcCompHistCentValVec->push_back(WJComp);
        mcCompHistCentValVec->push_back(VGComp);
        mcCompHistCentValVec->push_back(VVComp);
        mcCompHistCentValVec->push_back(ZDYComp);
        mcCompHistCentValVec->push_back(SingTopComp);
        mcCompHistCentValVec->push_back(TTbarComp);
    }
    else {
        mcCompHistCentValVec->push_back(WJComp);
        mcCompHistCentValVec->push_back(VVComp);
        mcCompHistCentValVec->push_back(ZDYComp);
        mcCompHistCentValVec->push_back(SingTopComp);
        mcCompHistCentValVec->push_back(TTbarComp);  
        mcCompHistCentValVec->push_back(QCDComp);
    }
}

void SpectrumDrawSyst_AddSignal(TCanvas * InputCanvas, vector<TH1F *> * vecSignalHists, vector<vector<TGraphAsymmErrors *> *> * vecSigErrGraph, unsigned int whichSyst, vector<TString> * vecSignalLegends, TLegend * leg, bool showLegend) {
    TH1F * currSigHist;
    TGraphAsymmErrors * currSigErrGraph;
    
    TPad * Pad1 = (TPad *) InputCanvas->cd(1);
    for (unsigned int iSigPoints = 0; iSigPoints < vecSignalHists->size(); ++iSigPoints) {
        currSigHist = vecSignalHists->at(iSigPoints);
        currSigErrGraph = vecSigErrGraph->at(iSigPoints)->at(whichSyst);
        currSigErrGraph->Draw("2 same");
        currSigHist->Draw("hist same");
        leg->AddEntry(currSigHist, vecSignalLegends->at(iSigPoints), "l");
        if (showLegend) {
            leg->Draw("same");
        }
    }
    Pad1->Update();
    Pad1->Modified();
    /*
     TPad * Pad2 = (TPad *) InputCanvas->cd(2);    
     TGraphAsymmErrors * fracRatioDrawGraph = ClonePoints(fracRatioHist, false);
     HistToGraphCopyAttributes(fracRatioHist, fracRatioDrawGraph);
     TH1F * patsy = (TH1F*) fracRatioHist->Clone("frac_patsy");
     patsy->SetLineColor(kWhite);
     patsy->SetMarkerColor(kWhite);
     fracRatioHist->SetLineColor(kBlack);
     patsy->Draw("e1");    
     fracRatioDrawGraph->Draw("p0 same");
     //    fracRatioHist->Draw("e1");
     Pad2->Update();
     Pad2->Modified();
     */
}



void SetupPlotRunning_Part1(int argc, char * argv[], RunParams * inRP, WeightCalculators * inWC, PlotMakingStructs * inPMS, AncillaryDrawingVariables * inADV, GlobalHistPlotInfo * inGHPI, HistDisplayParams * inHDP) {
    inRP->DefaultVarVals();
    inRP->SetVals(argc, argv);
    inRP->SetGSISaveName();
    
    inWC->DefaultVarVals();
    //    inWC->SetUniWeights(&RP.SLS);
    inWC->SetIntLumi();    
    
    inPMS->SetStructs(&inRP->SLS);    
    
    if (inRP->HPM.useDDEstimate_TTBar) {
        inWC->LoadTTBarWeight(&inRP->SLS, &inPMS->vecSystNames);        
    }
    if (inRP->HPM.useDDEstimate_DY) {
        inWC->LoadDYWeight(&inRP->SLS, &inPMS->vecSystNames, inRP->HPM.versDDEst_DY);        
    }    
    inADV->SetDefaultAxisSizeParams();
    inADV->DefaultLegParams();
    
    
    int typeMET = 0;
    TString stringMETExtra = inRP->SLS.noType0 ? " w/o Type 0" : "";
    TString genCutUsed = "";
    inGHPI->DefaultVarVals();
    inGHPI->SetFracRatioADPNameRange(inRP->GHS.doAbsRatio);
    inGHPI->SetLatexStringVec(inWC->intLumi, typeMET, stringMETExtra, genCutUsed, true);
    
    
    int RBNX = 1;
    bool doOverflow = true;
    bool doUnderflow = true;
    TString addName = "";
    inHDP->SetVals(RBNX, doOverflow, doUnderflow, addName);
}






void SetupPlotRunning_DataMC(RunParams * inRP, WeightCalculators * inWC, PlotMakingStructs * inPMS, HistogramDisplayStructs * inHDS_Data, HistogramDisplayStructs * inHDS_MC, bool doVerb = false) {
    bool isNotSig = false;
    vector<TString> vecBaseFileNames_Data;
    StopBaseFileNames_Data(&vecBaseFileNames_Data, &inRP->SLS);
    
    inHDS_Data->DefaultVarVals();
    inHDS_Data->vecISPI_asLoaded.resize(vecBaseFileNames_Data.size());
    SetFileNames(&inHDS_Data->vecISPI_asLoaded, &vecBaseFileNames_Data, &inRP->SLS, isNotSig, doVerb);
    
    
    inHDS_Data->SetCompParams("Data", 0);
    inHDS_Data->SetSystSize(0);
    SetWeights(inHDS_Data, inWC, inRP->HPM.useDDEstimate_TTBar, inRP->HPM.useDDEstimate_DY);
    
    vector<TString> vecBaseFileNames_MC;    
    StopBaseFileNames_MC(&vecBaseFileNames_MC, &inRP->SLS);
    
    inHDS_MC->DefaultVarVals();
    inHDS_MC->vecISPI_asLoaded.resize(vecBaseFileNames_MC.size());
    SetFileNames(&inHDS_MC->vecISPI_asLoaded, &vecBaseFileNames_MC, &inRP->SLS, isNotSig, doVerb);
    inHDS_MC->SetCompParams("MC", 1);
    
    
    const int numIndMC_Available = 8;
    Color_t mcColors[numIndMC_Available] = {kRed, kOrange + 2, kRed - 10, kGreen + 2, kBlue, kOrange - 5, kCyan - 2, kGreen + 3};
    TString mcLegends[numIndMC_Available] = {"t#bar{t}", "VV", "Single Top", "W + Jets", "Z/#gamma* #rightarrow l^{+}l^{-}", "VG", "Higgs", "Rare"};
    int mcSortParams[numIndMC_Available] = {7, 4, 6, 2, 5, 3, 1, 0};
    vector<Color_t> vecMCColors;
    vector<TString> vecMCLegends;
    vector<int> vecMCSortParams;
    for (unsigned int iIndMC = 0; iIndMC < numIndMC_Available; ++iIndMC) {
        vecMCColors.push_back(mcColors[iIndMC]);
        vecMCLegends.push_back(mcLegends[iIndMC]);
        vecMCSortParams.push_back(mcSortParams[iIndMC]);        
    }
    inHDS_MC->SetIndMCParams(&vecMCLegends, &vecMCColors, &vecMCSortParams);
    inHDS_MC->SetSystSize(inPMS->numSysts, inRP->SLS.doSyst);
    SetWeights(inHDS_MC, inWC, inRP->HPM.useDDEstimate_TTBar, inRP->HPM.useDDEstimate_DY, doVerb, &inPMS->vecSystNames);
    SetSystBasics(inHDS_MC, &inPMS->vecSystNames, inRP->SLS.SmearedPlots, false);
}

void SetupPlotRunning_Signal(RunParams * inRP, WeightCalculators * inWC, PlotMakingStructs * inPMS, HistogramDisplayStructs * inHDS_Signal, bool doVerb = false) {
    bool isSig = true;
    
    vector<TString> vecBaseFileNames_Signal;
    StopBaseFileNames_Signal(&vecBaseFileNames_Signal, &inRP->SLS);
    
    vector<float> signalSkimScaleVec;
    SignalSkimEfficiencyCalc(&signalSkimScaleVec, &inRP->SLS, inWC->intLumi, doVerb);
    
    inHDS_Signal->DefaultVarVals();
    inHDS_Signal->vecISPI_asLoaded.resize(vecBaseFileNames_Signal.size());
    SetFileNames(&inHDS_Signal->vecISPI_asLoaded, &vecBaseFileNames_Signal, &inRP->SLS, isSig, doVerb);
    
    inHDS_Signal->SetCompParams(inRP->SLS.SignalLegendString(0), -1);
    inHDS_Signal->SetSystSize(inPMS->numSysts, inRP->SLS.doSyst);
    SetWeights(inHDS_Signal, inWC, inRP->HPM.useDDEstimate_TTBar, inRP->HPM.useDDEstimate_DY, doVerb, &inPMS->vecSystNames, &signalSkimScaleVec);
    SetSystBasics(inHDS_Signal, &inPMS->vecSystNames, inRP->SLS.SmearedPlots, true);    
}


