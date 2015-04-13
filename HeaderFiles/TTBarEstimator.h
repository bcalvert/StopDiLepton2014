#ifndef TTBarEstimator_h
#define TTBarEstimator_h

#include <vector>
#include <string>
#include <iostream>

#include "TH1F.h"
#include "TString.h"

typedef pair<float, float> valPlusErr; //VPE -- first component represents a quantity, second component represents the statistical uncertainty

void SubtractVPE(valPlusErr * inVPEtoSubtractFrom, valPlusErr * inVPEtoSubtract, bool doVerb = 0) {
    //Used for subtracting one VPE from another, using linear subtraction for the first component and quadrature subtraction for the second
    if (doVerb) {
        cout << "pre subtraction: " << endl;
        cout << "inVPEtoSubtractFrom->first: " << inVPEtoSubtractFrom->first << endl;
        cout << "inVPEtoSubtractFrom->second: " << inVPEtoSubtractFrom->second << endl;
        cout << "inVPEtoSubtract->first: " << inVPEtoSubtract->first << endl;
        cout << "inVPEtoSubtract->second: " << inVPEtoSubtract->second << endl;
    }
    float errSum = pow(inVPEtoSubtractFrom->second, 2);
    errSum -= pow(inVPEtoSubtract->second, 2);
    inVPEtoSubtractFrom->first = inVPEtoSubtractFrom->first - inVPEtoSubtract->first;
    inVPEtoSubtractFrom->second = TMath::Sqrt(errSum);
    if (doVerb) {
        cout << "post subtraction: " << endl;
        cout << "inVPEtoSubtractFrom->first: " << inVPEtoSubtractFrom->first << endl;
        cout << "inVPEtoSubtractFrom->second: " << inVPEtoSubtractFrom->second << endl;
    }
}
void AddVPE(valPlusErr * inVPEtoAddTo, valPlusErr * inVPEtoAdd) {
        //Used for adding one VPE to another, using linear addition for the first component and quadrature addition for the second
    float errSum = pow(inVPEtoAddTo->second, 2);
    errSum += pow(inVPEtoAdd->second, 2);
    inVPEtoAddTo->first = inVPEtoAddTo->first + inVPEtoAdd->first;
    inVPEtoAddTo->second = TMath::Sqrt(errSum);
}

float GetError(vector<float> * inVecVals) {
    //returns the quadrature sum of a bunch of input floats
    float outErr = 0.0;
    for (unsigned int iVal = 0; iVal < inVecVals->size(); ++iVal) {
        outErr += pow(inVecVals->at(iVal), 2);
    }
    return TMath::Sqrt(outErr);
}

void SetSystVec(vector<TString> * inVecSystNames) {
    //Used for setting up the systematic names
    const int numSysts = 11;
    TString nameSaveSysts[numSysts] = {"_LepES", "_JetES", "_BTagEffSF", "_BMisTagSF", "_JetSmear", "_UncES", "_LepEffSF", "_genRecoilRW", "_FakeLepStat", "_FakeLepFakeRateSyst", "_FakeLepPromptRateSyst"};
    cout << "make sure this matches the syst vec definition for the PlotMakingStructs!!!! see StopPlotStructs.h" << endl;
    for (int iSyst = 0; iSyst < numSysts; ++iSyst) {
        inVecSystNames->push_back(nameSaveSysts[iSyst]);
    }
}

struct TTBarEstimator {
    //Structure used as part of the TTBar data-driven estimation, specifically, the normalization to a MT2 control region
    
    
    //Stuff for the output
    ValError SF_CV;
    ValError DDYield_CV;
    
    vector<ValError> vecSF_SystVarUp;
    vector<ValError> vecSF_SystVarDown;
    vector<ValError> vecYield_SystVarUp;
    vector<ValError> vecYield_SystVarDown;
    
    //vector for the systematics
    vector<TString> vecSystNames;
    
    TString nameChan;
    
    TH1F * compHistData;
    TH1F * compHistMC;
    vector<TH1F *> vecIndHistMC_TTBar;
    vector<TH1F *> vecIndHistMC_NonTTBar;
    
    int BinMax;
    
    void PrintSF(bool doVerb = 0) {
        TString currName;
        currName = "TTBar SF for " + nameChan;
        SF_CV.PrintOut(currName, true, "", true);
        for (int iSyst = 0; iSyst < (int) vecSystNames.size(); ++iSyst) {
            vecSF_SystVarUp[iSyst].PrintOut(currName, false, vecSystNames[iSyst] + TString("Up"), true);
            vecSF_SystVarDown[iSyst].PrintOut(currName, false, vecSystNames[iSyst] + TString("Down"), true);
        }
        ValError FullSyst = MakeFullSyst(&SF_CV, &vecSF_SystVarUp, &vecSF_SystVarDown, "FullSyst", doVerb);
        FullSyst.PrintOut(currName, false, FullSyst.Name, true);
    }
    
    void PrintYield(bool doVerb = 0) {
        TString currName;
        currName = "TTBar Yield for " + nameChan;
        DDYield_CV.PrintOut(currName, true, "", true);
        for (int iSyst = 0; iSyst < (int) vecSystNames.size(); ++iSyst) {
            vecYield_SystVarUp[iSyst].PrintOut(currName, false, vecSystNames[iSyst] + TString("Up"), true);
            vecYield_SystVarDown[iSyst].PrintOut(currName, false, vecSystNames[iSyst] + TString("Down"), true);
        }
        ValError FullSyst = MakeFullSyst(&DDYield_CV, &vecYield_SystVarUp, &vecYield_SystVarDown, "FullSyst", doVerb);
        FullSyst.PrintOut(currName, false, FullSyst.Name, true);
    }
    void PrintVals(bool printSF = true, bool printYield = false, bool doVerb = 0) {
        if (printSF) {
            PrintSF(doVerb);
        }
        if (printYield) {
            PrintYield(doVerb);
        }
    }
    
    void InitializeVecs() {
        SetSystVec(&vecSystNames);
        vecSF_SystVarUp.resize(vecSystNames.size());
        vecSF_SystVarDown.resize(vecSystNames.size());
        
        vecYield_SystVarUp.resize(vecSystNames.size());
        vecYield_SystVarDown.resize(vecSystNames.size());
    }
    
    void DefaultVals(TString inNameChan, bool doVerb = 0) {
        nameChan = inNameChan;
        InitializeVecs();
    }
    

    void SetDataHist(HistogramDisplayStructs * inHDS_Data, int binStatEnd = -1) {
        compHistData = inHDS_Data->compSamp.first.grabbedHist_TH1F;
        BinMax = binStatEnd < 1 ? compHistData->GetNbinsX() : binStatEnd;
    }
    void SetMCHists(HistogramDisplayStructs * inHDS_MC, vector<TString> * vecStringsToContain, vector<TString> * vecStringsToNotContain, int iSyst = 0, bool doVerb = 0) {
        if (iSyst == 0) {
            compHistMC = inHDS_MC->compSamp.first.grabbedHist_TH1F;
        }
        else if (iSyst > 0) {
            compHistMC = inHDS_MC->compSamp.first.vecGrabbedHist_SystVarUp_TH1F[abs(iSyst) - 1];
        }
        else if (iSyst < 0) {
            compHistMC = inHDS_MC->compSamp.first.vecGrabbedHist_SystVarDown_TH1F[abs(iSyst) - 1];
        }
        
        vecIndHistMC_TTBar.resize(0);
        vecIndHistMC_NonTTBar.resize(0);
        
        TString mcName;
        
        TH1F * histToAdd;
        
        for (unsigned int iMC = 0; iMC < inHDS_MC->vecSampDisplay_IndMC.size(); ++iMC) {
            bool indMCContainsBadString = false;
            bool indMCContainsGoodString = false;
            
            mcName = inHDS_MC->vecSampDisplay_IndMC[iMC].first.grabbedHist_TH1F->GetName();
            if (doVerb) {
                cout << "mcName " << mcName << endl;
            }
            for (unsigned int iBadString = 0; iBadString < vecStringsToNotContain->size(); ++iBadString) {
                if (doVerb) {
                    cout << "checking against bad string iBadString " << iBadString << " : " << vecStringsToNotContain->at(iBadString) << endl;
                }
                if (mcName.Contains(vecStringsToNotContain->at(iBadString))) indMCContainsBadString = true;
            }
            if (!indMCContainsBadString) {
                for (unsigned int iGoodString = 0; iGoodString < vecStringsToContain->size(); ++iGoodString) {
                    if (doVerb) {
                        cout << "checking against good string iGoodString " << iGoodString << " : " << vecStringsToContain->at(iGoodString) << endl;
                    }
                    if (mcName.Contains(vecStringsToContain->at(iGoodString))) indMCContainsGoodString = true;
                }
            }
            if (iSyst == 0) {
                histToAdd = inHDS_MC->vecSampDisplay_IndMC[iMC].first.grabbedHist_TH1F;
            }
            else if (iSyst > 0) {
                histToAdd = inHDS_MC->vecSampDisplay_IndMC[iMC].first.vecGrabbedHist_SystVarUp_TH1F[abs(iSyst) - 1];
            }
            else if (iSyst < 0) {
                histToAdd = inHDS_MC->vecSampDisplay_IndMC[iMC].first.vecGrabbedHist_SystVarDown_TH1F[abs(iSyst) - 1];
            }
            
            if (indMCContainsGoodString) {
                vecIndHistMC_TTBar.push_back(histToAdd);
            }
            else {
                vecIndHistMC_NonTTBar.push_back(histToAdd);
            }
            /*
            else {
                cout << "the MC name isn't satisfying either good or bad " << mcName << endl;
            }
             */
        }
    }
    
    void SetVPE(valPlusErr * inVPEToSet, TH1F * histToIntegrate, bool doVerb = 0) {
        Double_t currErr;
        if (doVerb) {
            cout << "going to integrate histToIntegrate " << histToIntegrate->GetName() << endl;
            cout << "from bin 1 to " << BinMax << endl;
        }
        inVPEToSet->first = histToIntegrate->IntegralAndError(1, BinMax, currErr);
        inVPEToSet->second = (float) currErr;
        if (doVerb) {
            cout << "integral results " << endl;
            cout << "inVPEToSet->first: " << inVPEToSet->first << endl;
            cout << "inVPEToSet->second: " << inVPEToSet->second << endl;
        }
    }
    
    valPlusErr GetSF(valPlusErr * inVPEData, valPlusErr * inVPEMC, bool doVerb = 0) {
        valPlusErr outSF;
        outSF.first = inVPEData->first / inVPEMC->first;
        
        float errTerm1 = inVPEData->second / inVPEMC->first;
        float errTerm2 = inVPEData->first * inVPEMC->second / (pow(inVPEMC->first, 2));
        
        vector<float> vecErrTerms(0);
        vecErrTerms.push_back(errTerm1);
        vecErrTerms.push_back(errTerm2);
        
        outSF.second = GetError(&vecErrTerms);
        if (doVerb) {
            cout << "Verbosity Info on Calculating the TTBar Scale Factor " << endl;
            cout << "inputData Min NonTTBar yield " << inVPEData->first << endl;
            cout << "MC yield " << inVPEMC->first << endl;
            cout << "SF: " << outSF.first << endl;
            
            cout << "errTerm1 " << errTerm1 << endl;
            cout << "errTerm2 " << errTerm2 << endl;
            cout << "SF Err: " << outSF.second << endl;
        }
        return outSF;
    }
    
    valPlusErr GetDDYield(valPlusErr * inVPETTBarSF, valPlusErr * pairYieldTTBarMCInMT2Control, bool doVerb = 0) {
        valPlusErr outYield_DD;
        
        outYield_DD.first = inVPETTBarSF->first * pairYieldTTBarMCInMT2Control->first;
        
        float errTerm1 = pairYieldTTBarMCInMT2Control->second * inVPETTBarSF->first;
        float errTerm2 = pairYieldTTBarMCInMT2Control->first * inVPETTBarSF->second;
        vector<float> vecErrTerms(0);
        vecErrTerms.push_back(errTerm1);
        vecErrTerms.push_back(errTerm2);
        
        outYield_DD.second = GetError(&vecErrTerms);
        
        
        if (doVerb) {
            cout << "Verbosity Info on Calculating the DD TTBar Yield " << endl;
            cout << "Scale Factor in MT2 Control Region: " << inVPETTBarSF->first << endl;
            cout << "TTBar Yield in MT2 Control Region: " << pairYieldTTBarMCInMT2Control->first << endl;
            cout << "Scaled Yield: " << outYield_DD.first << endl;
            
            cout << "errTerm1 " << errTerm1 << endl;
            cout << "errTerm2 " << errTerm2 << endl;
            cout << "Yield Err: " << outYield_DD.second << endl;
        }
        
        return outYield_DD;
    }
    void CalculateSFs(int iSyst = 0, bool doVerb = 0) {
        valPlusErr dataVPE(0.0, 0.0);
        valPlusErr mcVPE_v1(0.0, 0.0), mcVPE_v2(0.0, 0.0);
        valPlusErr mcVPE_TTBar(0.0, 0.0), mcVPE_NonTTBar(0.0, 0.0);
        valPlusErr currVPE_v2;
        SetVPE(&dataVPE, compHistData, doVerb);
        SetVPE(&mcVPE_v1, compHistMC, doVerb);
        
        for (unsigned int iMC = 0; iMC < vecIndHistMC_TTBar.size(); ++iMC) {
            SetVPE(&currVPE_v2, vecIndHistMC_TTBar[iMC], doVerb);
            AddVPE(&mcVPE_TTBar, &currVPE_v2);
        }
        
        for (unsigned int iMC = 0; iMC < vecIndHistMC_NonTTBar.size(); ++iMC) {
            SetVPE(&currVPE_v2, vecIndHistMC_NonTTBar[iMC], doVerb);
            AddVPE(&mcVPE_NonTTBar, &currVPE_v2);
        }
        AddVPE(&mcVPE_v2, &mcVPE_TTBar);
        AddVPE(&mcVPE_v2, &mcVPE_NonTTBar);
        if (abs(mcVPE_v1.first - mcVPE_v2.first) > 10) {
            cout << "something funky...mc comp integral calculated two different but ostensibly equivalent ways isn't the same..." << endl;
            cout << "mcIntegral_v1 " << mcVPE_v1.first << endl;
            cout << "mcIntegral_v2 " << mcVPE_v2.first << endl;
        }
        
        SubtractVPE(&dataVPE, &mcVPE_NonTTBar, doVerb);
        
        valPlusErr vpeSF = GetSF(&dataVPE, &mcVPE_TTBar, doVerb);
        
        valPlusErr vpeTTBarYield = GetDDYield(&vpeSF, &mcVPE_TTBar, doVerb);

        if (iSyst == 0) {
            SF_CV.SetStatError(&vpeSF);
            DDYield_CV.SetStatError(&vpeTTBarYield);
        }
        else if (iSyst > 0) {
            vecSF_SystVarUp[abs(iSyst) - 1].SetStatError(&vpeSF);
            vecYield_SystVarUp[abs(iSyst) - 1].SetStatError(&vpeTTBarYield);
        }
        else if (iSyst < 0) {
            vecSF_SystVarDown[abs(iSyst) - 1].SetStatError(&vpeSF);
            vecYield_SystVarDown[abs(iSyst) - 1].SetStatError(&vpeTTBarYield);
        }
    }
    void RunSFCalc_FullSet(HistogramDisplayStructs * inHDS_Data, HistogramDisplayStructs * inHDS_MC, vector<TString> * vecStringsToContain, vector<TString> * vecStringsToNotContain, bool doVerb, int binStatEnd = -1) {
        if (doVerb) {
            cout << "##############################" << endl;
            cout << "####Loading Data Histogram####" << endl;
            cout << "##############################" << endl;
        }
        SetDataHist(inHDS_Data, binStatEnd);

        if (doVerb) {
            cout << "##############################" << endl;
            cout << "#####Loading MC Histogram for CV#####" << endl;
            cout << "##############################" << endl;
        }
        SetMCHists(inHDS_MC, vecStringsToContain, vecStringsToNotContain, 0);
        if (doVerb) {
            cout << "##############################" << endl;
            cout << "######Calculating CV SFs######" << endl;
            cout << "##############################" << endl;
        }
        CalculateSFs(0, doVerb);
        for (int iSyst = 0; iSyst < (int) vecSystNames.size(); ++iSyst) {
            if (doVerb) {
                cout << "################################" << endl;
                cout << "##Loading MC Histograms for iSyst " << iSyst + 1 << "##" << endl;
                cout << "################################" << endl;
            }
            SetMCHists(inHDS_MC, vecStringsToContain, vecStringsToNotContain, iSyst + 1);
            if (doVerb) {
                cout << "################################" << endl;
                cout << "##Calculating SFs for iSyst " << iSyst + 1 << "##" << endl;
                cout << "################################" << endl;
            }
            CalculateSFs(iSyst + 1, doVerb);
            
            
            if (doVerb) {
                cout << "################################" << endl;
                cout << "##Loading MC Histograms for iSyst " << -1 * (iSyst + 1) << "##" << endl;
                cout << "################################" << endl;
            }
            SetMCHists(inHDS_MC, vecStringsToContain, vecStringsToNotContain, -1 * (iSyst + 1));
            if (doVerb) {
                cout << "################################" << endl;
                cout << "##Calculating SFs for iSyst " << -1 * (iSyst + 1) << "##" << endl;
                cout << "################################" << endl;
            }
            CalculateSFs(-1 * (iSyst + 1), doVerb);
        }
    }
    void SetInputSW(SpecificWeight * inSW, int iSyst = 0) {
        ValError * VEToUse;
        
        if (iSyst == 0) {
            VEToUse = &SF_CV;
        }
        else if (iSyst > 0) {
            VEToUse = &vecSF_SystVarUp[abs(iSyst) - 1];
        }
        else if (iSyst < 0) {
            VEToUse = &vecSF_SystVarDown[abs(iSyst) - 1];
        }
        inSW->SetSF(VEToUse, iSyst);
    }
    void SetAllSystInputSW(SpecificWeight * inSW) {
        SetInputSW(inSW, 0);
        for (int iSyst = 1; iSyst < (int) vecSystNames.size() + 1; ++iSyst) {
            SetInputSW(inSW, iSyst);
            SetInputSW(inSW, -1 * iSyst);
        }
    }
};
#endif