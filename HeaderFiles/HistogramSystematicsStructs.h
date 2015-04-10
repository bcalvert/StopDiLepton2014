// Structs used as part of dealing with histogram systematics

#include "TString.h"
#include "TGraphAsymmErrors.h"
using namespace std;

typedef pair<float, float> valPlusErr;


typedef struct ValError {
    float upError;
    float downError;
    float centVal;
    TString Name;
    void DefaultVals() {
        upError = 0;
        downError = 0;
        centVal = 0;
        Name = "";
    }
    float relErr(int whichCase = 1) {
        float outVal = -1.;
        if (centVal == 0 || whichCase == 0) return outVal;
        else {
            if (whichCase > 0) {
                outVal = upError/centVal;
            }
            else {
                outVal = downError/centVal;
            }
            return outVal;
        }
    }
    void Scale(float scaleFactor) {
        centVal *= scaleFactor;
        upError *= scaleFactor;
        downError *= scaleFactor;
    }
    void SetStatError(valPlusErr * inVPE) {
        centVal = inVPE->first;
        upError = inVPE->second;
        downError = inVPE->second;
    }
    void SetStatError(float inCV, float inStatError) {
        centVal = inCV;
        upError = inStatError;
        downError = inStatError;
    }
    void SetSystError(float inCV, float upShift, float downShift, bool doSymError = false, bool doVerb = false) {
        if (doVerb) {
            cout << "inCV " << inCV << endl;
            cout << "upShift " << upShift << endl;
            cout << "downShift " << downShift << endl;
        }
        centVal = inCV;
        float diffShiftUp, diffShiftDown;
        float errSum;
        
        int whichErrCase = 0;
        // +(-) 3 for if shift up is greater (smaller) than cent Val; likewise for shift down but with +(-) 1 instead;
        // +4 (-4): both more (less) than cent val
        // +2 (-2): upErr more, downErr less (upErr less, downErr more)
        whichErrCase += (upShift > inCV) ?  3 : 0;
        whichErrCase += (upShift < inCV) ? -3 : 0;
        
        whichErrCase += (downShift > inCV) ?  1 : 0;
        whichErrCase += (downShift < inCV) ? -1 : 0;
        
        diffShiftUp = inCV - upShift;
        diffShiftDown = inCV - downShift;
        
        if (doVerb) {
            cout << "whichErrCase " << whichErrCase << endl;
            cout << "diffShiftUp " << diffShiftUp << endl;
            cout << "diffShiftDown " << diffShiftDown << endl;
        }
        
        errSum = TMath::Sqrt(0.5*(diffShiftDown*diffShiftDown + diffShiftUp*diffShiftUp));
        
        if (doSymError) {
            upError = errSum;
            downError = errSum;
        }
        else {
            switch (whichErrCase) {
                case 0:
                    upError = 0;
                    downError = 0;
                    break;
                case 4:
                    upError = errSum;
                    downError = 0;
                    break;
                case -4:
                    upError = 0;
                    downError = errSum;
                    break;
                case 2:
                    upError = abs(diffShiftUp);
                    downError = abs(diffShiftDown);
                    break;
                case -2:
                    upError = abs(diffShiftDown);
                    downError = abs(diffShiftUp);
                    break;
                case 3:
                    upError = abs(diffShiftUp);
                    downError = upError;
                    break;
                case -3:
                    upError = abs(diffShiftUp);
                    downError = upError;
                    break;
                case 1:
                    downError = abs(diffShiftDown);
                    upError = downError;
                    break;
                case -1:
                    downError = abs(diffShiftDown);
                    upError = downError;
                    break;
                    
                default:
                    break;
            }
        }
        if (doVerb) {
            cout << "upError " << upError << endl;
            cout << "downError " << downError << endl;
        }
    }
    void SetSystErrorHard(float inCV, float inUE, float inDE) {
        centVal = inCV;
        upError = inUE;
        downError = inDE;
        /*
         cout << "hard error " << endl;
         cout << "upError " << upError << endl;
         cout << "downError " << downError << endl;
         */
    }
    void SetSystError(ValError * inVEToCalcDiffUp, ValError * inVEToCalcDiffDown, bool doVerb = 0) {
        float upShift = inVEToCalcDiffUp->centVal;
        float downShift = inVEToCalcDiffDown->centVal;
        this->SetSystError(this->centVal, upShift, downShift, false, doVerb);
    }
    void QuadSum(ValError * inErrToSumWithCare) {
        float upErrSum = 0.0, downErrSum = 0.0;
        upErrSum += upError*upError;
        upErrSum += inErrToSumWithCare->upError*inErrToSumWithCare->upError;
        
        downErrSum += downError*downError;
        downErrSum += inErrToSumWithCare->downError*inErrToSumWithCare->downError;
        upError = TMath::Sqrt(upErrSum);
        downError = TMath::Sqrt(downErrSum);
    }
    void QuadSub(ValError * inErrToSubtractWithCare) {
        float upErrSum = 0.0, downErrSum = 0.0;
        upErrSum += upError*upError;
        upErrSum -= inErrToSubtractWithCare->upError*inErrToSubtractWithCare->upError;
        
        downErrSum += downError*downError;
        downErrSum -= inErrToSubtractWithCare->downError*inErrToSubtractWithCare->downError;
        if (upErrSum < 0 || downErrSum < 0) {
            cout << "the errors for the inErr you're subtracting are larger than the original!" << endl;
            cout << "upErrSum " << upErrSum << endl;
            cout << "downErrSum " << downErrSum << endl;
        }
        upError = TMath::Sqrt(upErrSum);
        downError = TMath::Sqrt(downErrSum);
    }
    float SymError(int SymType = 0) {
        float outError;
        //SymType: 0: True average, 1: Geometric Average, 2: Quadrature average
        switch (SymType) {
            case 0:
                outError = 0.5 * (upError + downError);
                break;
            case 1:
                outError = TMath::Sqrt(upError * downError);
                break;
            case 2:
                outError = TMath::Sqrt(0.5 * ((upError*upError) + (downError*downError)));
                break;
        }
        return outError;
    }
    int CompareErr(ValError * inErrToCompare, int whichCompareStyle = 0, int symErrCompare = 0) {
        int outVal = 0;
        //whichCompareStyle: -1, compare down errors, 0: compare average of errors, 1: compare up errors
        switch (whichCompareStyle) {
            case -1:
                if (downError > inErrToCompare->downError) outVal = 1;
                else if (downError < inErrToCompare->downError) outVal = -1;
                break;
            case 0:
                if (SymError(symErrCompare) > inErrToCompare->SymError(symErrCompare)) outVal = 1;
                else if (SymError(symErrCompare) < inErrToCompare->SymError(symErrCompare)) outVal = 1;
                break;
            case 1:
                if (upError > inErrToCompare->upError) outVal = 1;
                else if (upError < inErrToCompare->upError) outVal = -1;
                break;
        }
        return outVal;
    }
    
    
    void PrintOut(TString inName, bool isStat = true, TString systName = "", bool printRelErr= false, bool printSymSyst = false) {
        char buf[300];
        if (isStat) {
            if (printRelErr) {
                //sprintf(buf,"CentVal: %0.4f, StatErr: %0.4f", centVal, 1 + (upError/centVal));
                
                if (centVal > 0) {
                    sprintf(buf,"CentVal: %0.4f, StatErr: %0.4f", centVal, 1 + (upError/centVal));
                }
                else {
                    sprintf(buf,"CentVal: %0.4f, StatErr: %0.4f", 0.00, 1.00);
                }
                
                cout << "Name: " << inName << ", " << buf << endl;
            }
            else {
                sprintf(buf,"CentVal: %0.4f, StatErr: %0.4f", centVal, upError);
                cout << "Name: " << inName << ", " << buf << endl;
            }
        }
        else {
            if (printSymSyst) {
                if (printRelErr) {
                    //                    sprintf(buf,"CentVal: %0.4f, AverageErr: %0.4f", centVal, 1 + ((0.5/centVal) * (upError + downError)));
                    
                    if (centVal > 0) {
                        sprintf(buf,"CentVal: %0.4f, AverageErr: %0.4f", centVal, 1 + ((0.5/centVal) * (upError + downError)));
                    }
                    else {
                        sprintf(buf,"CentVal: %0.4f, AverageErr: %0.4f", 0.00, 1.00);
                    }
                    
                    cout << "Name: " << inName << ", Syst: " << systName << ", " << buf << endl;
                }
                else {
                    sprintf(buf,"CentVal: %0.4f, AverageErr: %0.4f", centVal, 0.5 * (upError + downError));
                    cout << "Name: " << inName << ", Syst: " << systName << ", " << buf << endl;
                }
            }
            else {
                if (printRelErr) {
                    //sprintf(buf,"CentVal: %0.4f, UpErr: %0.4f, DownErr: %0.4f", centVal, 1 + (upError/centVal), 1 + (downError/centVal));
                    
                    if  (centVal > 0) {
                        sprintf(buf,"CentVal: %0.4f, UpErr: %0.4f, DownErr: %0.4f", centVal, 1 + (upError/centVal), 1 - (downError/centVal));
                    }
                    else {
                        sprintf(buf,"CentVal: %0.4f, UpErr: %0.4f, DownErr: %0.4f", 0.00, 1.00, 1.00);
                    }
                    
                    cout << "Name: " << inName << ", Syst: " << systName << ", " << buf << endl;
                }
                else {
                    sprintf(buf,"CentVal: %0.4f, UpErr: %0.4f, DownErr: %0.4f", centVal, upError, downError);
                    cout << "Name: " << inName << ", Syst: " << systName << ", " << buf << endl;
                }
            }
        }
    }
    void SubtractVE(ValError * inVEToSubtract, int typeSub) {
        //typeSub = 0 for straight subtraction or 1 for quadrature subtraction
        centVal -= inVEToSubtract->centVal;
        if (typeSub == 0) {
            upError -= inVEToSubtract->upError;
            downError -= inVEToSubtract->downError;
        }
        else if (typeSub == 1) {
            QuadSub(inVEToSubtract);
        }
        else {
            cout << "typeSub needs to be 0 or 1; it is " << typeSub << endl;
        }
    }
} ValError;
bool operator<(ValError a, ValError b)
{
    return (a.CompareErr(&b) < 0);
}
bool operator>(ValError a, ValError b)
{
    return (a.CompareErr(&b) > 0);
}
bool operator==(ValError a, ValError b)
{
    return (a.CompareErr(&b) == 0);
}
ValError operator+(ValError a, ValError b)
{
    ValError outErr = a;
    outErr.QuadSum(&b);
    return outErr;
}

ValError MakeFullSyst(ValError * inValCentVal, vector<ValError> * inVecSystUp, vector<ValError> * inVecSystDown, TString inName, bool doVerb = 0) {
    ValError outVal;
    outVal.DefaultVals();
    outVal.Name = inName;
    outVal.centVal = inValCentVal->centVal;
    for (unsigned int iVE = 0; iVE < inVecSystUp->size(); ++iVE) {
        ValError currVE;
        currVE.DefaultVals();
        currVE.centVal = outVal.centVal;
        currVE.SetSystError(&inVecSystUp->at(iVE), &inVecSystDown->at(iVE), doVerb);
        if (doVerb) {
            cout << "iVE " << iVE << endl;
            cout << "outVal upError pre-Add " << outVal.upError << endl;
            cout << "outVal downError pre-Add " << outVal.downError << endl;
            cout << "currVE upError " << currVE.upError << endl;
            cout << "currVE downError " << currVE.downError << endl;
        }
        outVal = outVal + currVE;
        if (doVerb) {
            cout << "outVal upError post-Add " << outVal.upError << endl;
            cout << "outVal downError post-Add " << outVal.downError << endl;
        }
    }
    return outVal;
}

typedef struct SampleSystematicsInfo {
    ValError StatError;
    vector<ValError> SystError;
    vector<ValError> SystPlusStatError;
    vector<TString> SystName;
    vector<TGraphAsymmErrors * > SystErrorGraph;
    vector<TGraphAsymmErrors * > SystPlusStatErrorGraph;
    vector<TGraphAsymmErrors *> fracRatioSystVec;
    TString Name;
    void SetVecSizes(int size) {
        SystError.resize(size);
        SystPlusStatError.resize(size);
        SystName.resize(size);
        SystErrorGraph.resize(size);
        SystPlusStatErrorGraph.resize(size);
        fracRatioSystVec.resize(size);
    }
    void SetVecSizes_SystGraphInfo(int size) {
        SystName.resize(size);
        SystErrorGraph.resize(size);
        SystPlusStatErrorGraph.resize(size);
        fracRatioSystVec.resize(size);
    }
    void QuadSumSystPlusStat() {
        
    }
    void PrintVec(int iSyst = 0, bool printFracRatioInfo = true) {
        cout << "SystName size " << SystName.size() << endl;
        cout << "SystName " << SystName[iSyst] << endl;
        for (int iPoint = 0; iPoint < SystPlusStatErrorGraph[iSyst]->GetN() + 2; ++iPoint) {
            cout << "iPoint: " << iPoint << endl;
            cout << "SystErrorGraph up Error: " << SystErrorGraph[iSyst]->GetErrorYhigh(iPoint) << endl;
            cout << "SystErrorGraph down Error: " << SystErrorGraph[iSyst]->GetErrorYlow(iPoint) << endl;
            if (printFracRatioInfo) {
                cout << "fracRatioSystVec up Error: " << fracRatioSystVec[iSyst]->GetErrorYhigh(iPoint) << endl;
                cout << "fracRatioSystVec down Error: " << fracRatioSystVec[iSyst]->GetErrorYlow(iPoint) << endl;
            }
        }
    }
    void PrintOut(bool justStat = true, bool noSystPlusStat = true, bool printRelErr = false, bool printSymSyst = false) {
        StatError.PrintOut(Name, true, "", printRelErr);
        if (!justStat) {
            for (unsigned int iSyst = 0; iSyst < SystError.size(); ++iSyst) {
                if (noSystPlusStat) {
                    SystError[iSyst].PrintOut(Name, false, SystName[iSyst], printRelErr, printSymSyst);
                }
                else {
                    SystPlusStatError[iSyst].PrintOut(Name, false, SystName[iSyst], printRelErr, printSymSyst);
                }
            }
        }
    }
    /*
     void SetSystGraphsFromVec(TH1F * inputBaseMCHist, vector<TGraphAsymmErrors> * inVecIndSampleErrJustSyst, bool doAbsRatio, float fracRatioYAxisLB, float fracRatioYAxisUB, bool doSymErr, bool doFracRatio, bool doVerbosity = false, TString systNameToSet = "") {
     broken
     
     
     const int numSysts = 9;
     TString namesSysts[numSysts] = {"LepEffSF", "LepES", "JetES", "BTagSF", "UncES", "JetSmear", "genRecoilRW", "genStopXSec", "EWKXSec"};
     //     cout << "issues/biases come up when both errors are on one side current with this " << endl;
     SetVecSizes_SystGraphInfo(0);
     TGraphAsymmErrors * currFracRatioGraph;
     
     TGraphAsymmErrors * errCompStatCentVal = ClonePoints(inputBaseMCHist);
     
     TGraphAsymmErrors * errSystQuadSum, * errSystQuadSum_pStat;
     TGraphAsymmErrors * errCurrSyst, * errCurrSyst_pStat;
     
     vector<TGraphAsymmErrors *> errJustSystComp;
     
     bool careAboutName;
     careAboutName = systNameToSet.Length() != 0;
     
     TString nameToUse;
     
     for (unsigned int iSyst = 0; iSyst < inVecIndSampleErrJustSyst->at(0).SystErrorGraph.size(); ++iSyst) {
     errJustSystComp.resize(0);
     if (careAboutName && !inVecIndSampleErrJustSyst->at(0).SystName[iSyst].Contains(systNameToSet)) continue;
     cout << "iSyst " << iSyst << endl;
     for (unsigned int iIndSamp = 0; iIndSamp < inVecIndSampleErrJustSyst->size(); ++iIndSamp) {
     errJustSystComp.push_back(inVecIndSampleErrJustSyst->at(iIndSamp).SystErrorGraph[iSyst]);
     }
     nameToUse = careAboutName ? systNameToSet : namesSysts[iSyst];
     errCurrSyst = GraphSystErrorSumErrors(errCompStatCentVal, nameToUse, &errJustSystComp, false, inputBaseMCHist, doVerbosity);
     errCurrSyst_pStat = GraphSystErrorSumErrors(errCompStatCentVal, nameToUse, errCurrSyst, inputBaseMCHist);
     GraphMainAttSet(errCurrSyst, colorErrGraph, 3001, colorErrGraph, 2, kWhite, 0, 0);
     GraphMainAttSet(errCurrSyst_pStat, colorErrGraph, 3001, colorErrGraph, 2, kWhite, 0, 0);
     SystErrorGraph[iSyst] = errCurrSyst;
     SystPlusStatErrorGraph[iSyst] = errCurrSyst_pStat;
     if (doFracRatio) {
     currFracRatioGraph = FracGraph(inputBaseMCHist, errCurrSyst, doAbsRatio, fracRatioYAxisLB, fracRatioYAxisUB);
     fracRatioSystVec.push_back(currFracRatioGraph);
     }
     }
     }
     */
    void SetSystGraphs(TH1F * inputBaseMCHist, vector<TH1F *> * vecInputMC_SystVarUp, vector<TH1F *> * vecInputMC_SystVarDown, Color_t colorErrGraph, bool doAbsRatio, float fracRatioYAxisLB, float fracRatioYAxisUB, bool doSymErr, bool doFracRatio, bool doSmear, bool isEWK = false, bool doStopXSec = true, bool doVerbosity = false) {
        SetVecSizes_SystGraphInfo(0);
        
        float normFactor = isEWK ? 0.5 : 0.0;
        
        if (doVerbosity) {
            cout << "normFactor " << normFactor << endl;
        }
        
        int indexSystUp, indexSystDown;
        TH1F * histSystUp, * histSystDown;
        TString baseSearchString;
        
//        const int numSysts = 8;
        const int numSysts = 11;
//        bool isSmearSyst[numSysts] = {false, false, false, false, true, true, false, false};
        bool isSmearSyst[numSysts] = {false, false, false, false, true, true, false, false, false, false, false};
//        TString namesSysts[numSysts] = {"LepEffSF", "LepES", "JetES", "BTagSF", "UncES", "JetSmear", "genRecoilRW", "EWKXSec"};
        TString namesSysts[numSysts] = {"LepEffSF", "LepES", "JetES", "BTagSF", "UncES", "JetSmear", "genRecoilRW", "TTBarNorm", "DYNorm", "EWKNorm", "FakeLepSyst"};
        
        TGraphAsymmErrors * currFracRatioGraph;
        TGraphAsymmErrors * errCompStatCentVal = ClonePoints(inputBaseMCHist);
        
        TGraphAsymmErrors * errSystQuadSum, * errSystQuadSum_pStat;
        TGraphAsymmErrors * errCurrSyst, * errCurrSyst_pStat;
        
        const int numFakeSysts = 3;
        TString nameFakeSysts[numFakeSysts] = {"FakeLepStat", "FakeLepFakeRateSyst", "FakeLepPromptRateSyst"};
        vector<TGraphAsymmErrors *> errFakeComp;
        TGraphAsymmErrors * errCurrFakeSyst;
        
        
        const int numBTagSysts = 2;
        TString nameBTagSysts[numBTagSysts] = {"BTagEffSF", "BMisTagSF"};
        vector<TGraphAsymmErrors *> errBTagComp;
        TGraphAsymmErrors * errCurrBTagSyst;
        
        for (int iSyst = 0; iSyst < numSysts; ++iSyst) {
            if (!doSmear && isSmearSyst[iSyst]) continue;
            if (!doStopXSec && namesSysts[iSyst].Contains("genStopXSec")) continue;
            if (namesSysts[iSyst].Contains("BTagSF")) {
                for (int iBTagSysts = 0; iBTagSysts < numBTagSysts; ++iBTagSysts) {
                    baseSearchString = nameBTagSysts[iBTagSysts] + TString("Shift");
                    indexSystUp = SystHistFinderOneDee(vecInputMC_SystVarUp, baseSearchString + TString("Up"), doVerbosity);
                    indexSystDown = SystHistFinderOneDee(vecInputMC_SystVarDown, baseSearchString + TString("Down"), doVerbosity);
                    if (indexSystUp == -1 || indexSystDown == -1) {
                        cout << "something wiggedy with syst indices " << endl;
                    }
                    histSystUp = vecInputMC_SystVarUp->at(indexSystUp);
                    histSystDown = vecInputMC_SystVarDown->at(indexSystDown);
                    errCurrBTagSyst = GraphSystErrorSet_SingleSource(inputBaseMCHist, histSystUp, histSystDown, nameBTagSysts[iBTagSysts], doSymErr, doVerbosity);
                    //                    cout << "curr Btag up err at point 1 " << errCurrBTagSyst->GetErrorYhigh(1) << endl;
                    errBTagComp.push_back(errCurrBTagSyst);
                }
                if (doVerbosity) {
                    cout << "making the combo BTag syst " << endl;
                }
                errCurrSyst = GraphSystErrorSumErrors(errCompStatCentVal, namesSysts[iSyst], &errBTagComp, false, inputBaseMCHist, doVerbosity);
                if (doVerbosity) {
                    cout << "name of btag combo " << errCurrSyst->GetName() << endl;
                }
            }
            else if (namesSysts[iSyst].Contains("FakeLepSyst")) {
                for (int iFakeSyst = 0; iFakeSyst < numFakeSysts; ++iFakeSyst) {
                    baseSearchString = nameFakeSysts[iFakeSyst] + TString("Shift");
                    indexSystUp = SystHistFinderOneDee(vecInputMC_SystVarUp, baseSearchString + TString("Up"), doVerbosity);
                    indexSystDown = SystHistFinderOneDee(vecInputMC_SystVarDown, baseSearchString + TString("Down"), doVerbosity);
                    if (indexSystUp == -1 || indexSystDown == -1) {
                        cout << "something wiggedy with syst indices " << endl;
                    }
                    histSystUp = vecInputMC_SystVarUp->at(indexSystUp);
                    histSystDown = vecInputMC_SystVarDown->at(indexSystDown);
                    errCurrFakeSyst = GraphSystErrorSet_SingleSource(inputBaseMCHist, histSystUp, histSystDown, nameFakeSysts[iFakeSyst], doSymErr, doVerbosity);
                    errFakeComp.push_back(errCurrFakeSyst);
                }
                if (doVerbosity) {
                    cout << "making the combo Fake syst " << endl;
                }
                errCurrSyst = GraphSystErrorSumErrors(errCompStatCentVal, namesSysts[iSyst], &errFakeComp, false, inputBaseMCHist, doVerbosity);
                if (doVerbosity) {
                    cout << "name of Fake combo " << errCurrSyst->GetName() << endl;
                }
            }
            else if (namesSysts[iSyst].Contains("EWKXSec")) {
                errCurrSyst = NormalizationSystGraph(inputBaseMCHist, normFactor);
            }
            else {
                baseSearchString = namesSysts[iSyst] + TString("Shift");
                indexSystUp = SystHistFinderOneDee(vecInputMC_SystVarUp, baseSearchString + TString("Up"), doVerbosity);
                indexSystDown = SystHistFinderOneDee(vecInputMC_SystVarDown, baseSearchString + TString("Down"), doVerbosity);
                if (indexSystUp == -1 || indexSystDown == -1) {
                    cout << "something wiggedy with syst indices " << endl;
                }
                histSystUp = vecInputMC_SystVarUp->at(indexSystUp);
                histSystDown = vecInputMC_SystVarDown->at(indexSystDown);
                errCurrSyst = GraphSystErrorSet_SingleSource(inputBaseMCHist, histSystUp, histSystDown, namesSysts[iSyst], doSymErr, doVerbosity);
                if (doVerbosity) {
                    cout << "name for iSyst = " << iSyst << endl;
                    cout << errCurrSyst->GetName() << endl;
                }
            }
            
            errCurrSyst_pStat = GraphSystErrorSumErrors(errCompStatCentVal, namesSysts[iSyst], errCurrSyst, inputBaseMCHist);
            GraphMainAttSet(errCurrSyst, colorErrGraph, 3001, colorErrGraph, 2, kWhite, 0, 0);
            GraphMainAttSet(errCurrSyst_pStat, colorErrGraph, 3001, colorErrGraph, 2, kWhite, 0, 0);
            SystErrorGraph.push_back(errCurrSyst);
            SystPlusStatErrorGraph.push_back(errCurrSyst_pStat);
            SystName.push_back(namesSysts[iSyst]);
            if (doFracRatio) {
                currFracRatioGraph = FracGraph(inputBaseMCHist, errCurrSyst, doAbsRatio, fracRatioYAxisLB, fracRatioYAxisUB);
                fracRatioSystVec.push_back(currFracRatioGraph);
            }
            
            if (doVerbosity) {
                cout << "not so quick sanity check " << endl;
                PrintVec(iSyst, doFracRatio);
            }
        }
        /*
         if (doVerbosity) {
         for (unsigned int iSyst = 0; iSyst < SystErrorGraph.size(); ++iSyst) {
         cout << "name?" << endl;
         cout << SystErrorGraph[iSyst].GetName() << endl;
         }
         }
         */
        errSystQuadSum = GraphSystErrorSumErrors(errCompStatCentVal, TString("FullSyst"), &SystErrorGraph, false, inputBaseMCHist, doVerbosity);
        errSystQuadSum_pStat = GraphSystErrorSumErrors(errCompStatCentVal, TString("FullSyst"), &SystErrorGraph, true, inputBaseMCHist, doVerbosity);
        GraphMainAttSet(errSystQuadSum, colorErrGraph, 3001, colorErrGraph, 2, kWhite, 0, 0);
        GraphMainAttSet(errSystQuadSum_pStat, colorErrGraph, 3001, colorErrGraph, 2, kWhite, 0, 0);
        SystErrorGraph.push_back(errSystQuadSum);
        SystPlusStatErrorGraph.push_back(errSystQuadSum_pStat);
        SystName.push_back("FullSyst");
        if (doFracRatio) {
            currFracRatioGraph = FracGraph(inputBaseMCHist, errSystQuadSum, doAbsRatio, fracRatioYAxisLB, fracRatioYAxisUB);
            fracRatioSystVec.push_back(currFracRatioGraph);
        }
        
    }
    
    
    
    
    void SetSystGraphsShapes(TH1F * inputBaseMCHist, vector<TH1F *> * vecInputMC_SystVarUp, vector<TH1F *> * vecInputMC_SystVarDown, Color_t colorErrGraph, bool doAbsRatio, float fracRatioYAxisLB, float fracRatioYAxisUB, bool doSymErr, bool doFracRatio, bool doSmear, bool isEWK = false, bool doStopXSec = true, bool doVerbosity = false) {
        
        //would like to do this as a proper setting within the other guy
        SetVecSizes_SystGraphInfo(0);
        
        float normFactor = isEWK ? 0.5 : 0.0;
        
        if (doVerbosity) {
            cout << "normFactor " << normFactor << endl;
        }
        
        int indexSystUp, indexSystDown;
        TH1F * histSystUp, * histSystDown;
        TString baseSearchString;
        
        //        const int numSysts = 8;
        const int numSysts = 10;
        //        bool isSmearSyst[numSysts] = {false, false, false, false, true, true, false, false};
        bool isSmearSyst[numSysts] = {false, false, false, false, true, true, false, false, false, false};
        //        TString namesSysts[numSysts] = {"LepEffSF", "LepES", "JetES", "BTagSF", "UncES", "JetSmear", "genRecoilRW", "EWKXSec"};
        TString namesSysts[numSysts] = {"LepEffSF", "LepES", "JetES", "BTagSF", "UncES", "JetSmear", "genRecoilRW", "TTBarNorm", "DYNorm", "EWKNorm"};
        
        TGraphAsymmErrors * currFracRatioGraph;
        TGraphAsymmErrors * errCompStatCentVal = ClonePoints(inputBaseMCHist);
        
        TGraphAsymmErrors * errSystQuadSum, * errSystQuadSum_pStat;
        TGraphAsymmErrors * errCurrSyst, * errCurrSyst_pStat;
        
        const int numBTagSysts = 2;
        TString nameBTagSysts[numBTagSysts] = {"BTagEffSF", "BMisTagSF"};
        vector<TGraphAsymmErrors *> errBTagComp;
        TGraphAsymmErrors * errCurrBTagSyst;
        
        int currIndex = -1;
        
        for (int iSyst = 0; iSyst < numSysts; ++iSyst) {
            if (!doSmear && isSmearSyst[iSyst]) continue;
            if (!doStopXSec && namesSysts[iSyst].Contains("genStopXSec")) continue;

            if (namesSysts[iSyst].Contains("BTagSF")) {
                for (int iBTagSysts = 0; iBTagSysts < numBTagSysts; ++iBTagSysts) {
                    baseSearchString = nameBTagSysts[iBTagSysts];
                    indexSystUp = SystHistFinderOneDee(vecInputMC_SystVarUp, baseSearchString + TString("Up"), doVerbosity);
                    indexSystDown = SystHistFinderOneDee(vecInputMC_SystVarDown, baseSearchString + TString("Down"), doVerbosity);
                    if (indexSystUp == -1 || indexSystDown == -1) {
                        cout << "something wiggedy with syst indices " << endl;
                    }
                    histSystUp = vecInputMC_SystVarUp->at(indexSystUp);
                    histSystDown = vecInputMC_SystVarDown->at(indexSystDown);
                    errCurrBTagSyst = GraphSystErrorSet_SingleSource(inputBaseMCHist, histSystUp, histSystDown, nameBTagSysts[iBTagSysts], doSymErr, doVerbosity);
                    //                    cout << "curr Btag up err at point 1 " << errCurrBTagSyst->GetErrorYhigh(1) << endl;
                    errBTagComp.push_back(errCurrBTagSyst);
                }
                if (doVerbosity) {
                    cout << "making the combo BTag syst " << endl;
                }
                errCurrSyst = GraphSystErrorSumErrors(errCompStatCentVal, namesSysts[iSyst], &errBTagComp, false, inputBaseMCHist, doVerbosity);
                if (doVerbosity) {
                    cout << "name of btag combo " << errCurrSyst->GetName() << endl;
                }
            }
            else if (namesSysts[iSyst].Contains("EWKXSec")) {
                errCurrSyst = NormalizationSystGraph(inputBaseMCHist, normFactor);
            }
            else {
                baseSearchString = namesSysts[iSyst];
                indexSystUp = SystHistFinderOneDee(vecInputMC_SystVarUp, baseSearchString + TString("Up"), doVerbosity);
                indexSystDown = SystHistFinderOneDee(vecInputMC_SystVarDown, baseSearchString + TString("Down"), doVerbosity);
                if (indexSystUp == -1 || indexSystDown == -1) {
                    cout << "something wiggedy with syst indices " << endl;
                }
                histSystUp = vecInputMC_SystVarUp->at(indexSystUp);
                histSystDown = vecInputMC_SystVarDown->at(indexSystDown);
                errCurrSyst = GraphSystErrorSet_SingleSource(inputBaseMCHist, histSystUp, histSystDown, namesSysts[iSyst], doSymErr, doVerbosity);
                if (doVerbosity) {
                    cout << "name for iSyst = " << iSyst << endl;
                    cout << errCurrSyst->GetName() << endl;
                }
            }
            
            errCurrSyst_pStat = GraphSystErrorSumErrors(errCompStatCentVal, namesSysts[iSyst], errCurrSyst, inputBaseMCHist);
            GraphMainAttSet(errCurrSyst, colorErrGraph, 3001, colorErrGraph, 2, kWhite, 0, 0);
            GraphMainAttSet(errCurrSyst_pStat, colorErrGraph, 3001, colorErrGraph, 2, kWhite, 0, 0);
            SystErrorGraph.push_back(errCurrSyst);
            SystPlusStatErrorGraph.push_back(errCurrSyst_pStat);
            SystName.push_back(namesSysts[iSyst]);
            if (doFracRatio) {
                currFracRatioGraph = FracGraph(inputBaseMCHist, errCurrSyst, doAbsRatio, fracRatioYAxisLB, fracRatioYAxisUB);
                fracRatioSystVec.push_back(currFracRatioGraph);
            }
            
            currIndex++;
            
            if (doVerbosity) {
                cout << "not so quick sanity check " << endl;
                PrintVec(currIndex, doFracRatio);
            }
        }
        /*
         if (doVerbosity) {
         for (unsigned int iSyst = 0; iSyst < SystErrorGraph.size(); ++iSyst) {
         cout << "name?" << endl;
         cout << SystErrorGraph[iSyst].GetName() << endl;
         }
         }
         */
        errSystQuadSum = GraphSystErrorSumErrors(errCompStatCentVal, TString("FullSyst"), &SystErrorGraph, false, inputBaseMCHist, doVerbosity);
        errSystQuadSum_pStat = GraphSystErrorSumErrors(errCompStatCentVal, TString("FullSyst"), &SystErrorGraph, true, inputBaseMCHist, doVerbosity);
        GraphMainAttSet(errSystQuadSum, colorErrGraph, 3001, colorErrGraph, 2, kWhite, 0, 0);
        GraphMainAttSet(errSystQuadSum_pStat, colorErrGraph, 3001, colorErrGraph, 2, kWhite, 0, 0);
        SystErrorGraph.push_back(errSystQuadSum);
        SystPlusStatErrorGraph.push_back(errSystQuadSum_pStat);
        SystName.push_back("FullSyst");
        if (doFracRatio) {
            currFracRatioGraph = FracGraph(inputBaseMCHist, errSystQuadSum, doAbsRatio, fracRatioYAxisLB, fracRatioYAxisUB);
            fracRatioSystVec.push_back(currFracRatioGraph);
        }
        
    }
    
    void PrepValErrVecSizes() {
        SystError.resize(SystErrorGraph.size());
        SystPlusStatError.resize(SystPlusStatErrorGraph.size());
    }
    void SetStatErrorOnYield(TH1F * histYield, int whichBin = 2, TString nameSSI = "") {
        if (nameSSI.Length() != 0) Name = nameSSI;
        StatError.SetStatError(histYield->GetBinContent(whichBin), histYield->GetBinError(whichBin));
    }
    void SetSystErrorOnYield(int whichSyst, int whichBin = 2, bool noSystPlusStat = true, bool doSymErr = false) {
        TGraphAsymmErrors * errGraphToUse;
        errGraphToUse = noSystPlusStat ? SystErrorGraph[whichSyst] : SystPlusStatErrorGraph[whichSyst];
        SystError[whichSyst].SetSystErrorHard(StatError.centVal, errGraphToUse->GetErrorYhigh(whichBin), errGraphToUse->GetErrorYlow(whichBin));
    }
    
    void SetStatPlusSystErrorYields(TH1F * histYieldCV, int whichBin = 2, TString nameSSI = "", bool justStat = true, bool noSystPlusStat = true, bool doSymErr = false) {
        SetStatErrorOnYield(histYieldCV, whichBin, nameSSI);
        if (!justStat) {
            PrepValErrVecSizes();
            for (unsigned int iSyst = 0; iSyst < SystError.size(); ++iSyst) {
                SetSystErrorOnYield(iSyst, whichBin, noSystPlusStat, doSymErr);
            }
        }
    }
    

} SampleSystematicsInfo;


SampleSystematicsInfo SSISubtraction(SampleSystematicsInfo * inSSIToSubtractFrom, SampleSystematicsInfo * inSSIToSubtract, TString subName) {
    //Function take a "composite" SSI, inSSIToSubtractFrom, that contains a sub-sample SSI, "inSSIToSubtract"
    //and subtract the sub-sample SSI from the "composite" SSI for the Stat Error and Systematic Errors
    
    unsigned int numSystsInSSIToSubtractFrom = inSSIToSubtractFrom->SystError.size();
    unsigned int numSystsInSSIToSubtract = inSSIToSubtract->SystError.size();
    
    if (numSystsInSSIToSubtractFrom != numSystsInSSIToSubtract) {
        cout << "the two SSIs don't have same number of systematics...first one " << numSystsInSSIToSubtractFrom << " while second has " << numSystsInSSIToSubtract << endl;
    }
    
    SampleSystematicsInfo outSSI = *inSSIToSubtractFrom;
    outSSI.Name += "Minus";
    outSSI.Name += subName;
    
    outSSI.StatError.SubtractVE(&inSSIToSubtract->StatError, 1);
    for (unsigned int iSyst = 0; iSyst < numSystsInSSIToSubtract; ++iSyst) {
        outSSI.SystError[iSyst].SubtractVE(&inSSIToSubtract->SystError[iSyst], 0);
    }
    return outSSI;
}