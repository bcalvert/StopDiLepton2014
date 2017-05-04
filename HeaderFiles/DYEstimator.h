#ifndef DYEstimator_h
#define DYEstimator_h

#include <vector>
#include <string>
#include <iostream>

#include "TH1F.h"
#include "TString.h"

typedef pair<float, float> valPlusErr;

float GetError(vector<float> * inVecVals) {
    float outErr = 0.0;
    for (unsigned int iVal = 0; iVal < inVecVals->size(); ++iVal) {
        outErr += pow(inVecVals->at(iVal), 2);
    }
    return TMath::Sqrt(outErr);
}

void SetSystVec(vector<TString> * inVecSystNames) {
    const int numSysts = 11;
    TString nameSaveSysts[numSysts] = {"_LepES", "_JetES", "_BTagEffSF", "_BMisTagSF", "_JetSmear", "_UncES", "_LepEffSF", "_genRecoilRW", "_FakeLepStat", "_FakeLepFakeRateSyst", "_FakeLepPromptRateSyst"};
    cout << "make sure this matches the syst vec definition for the PlotMakingStructs!!!! see StopPlotStructs.h" << endl;
    for (int iSyst = 0; iSyst < numSysts; ++iSyst) {
        inVecSystNames->push_back(nameSaveSysts[iSyst]);
    }
}

struct DYEstimator {
    int cutMET_MT2ll; //used for determining if to cut on MT2ll or MET -- 1 is MT2ll, 2 is MET
    int cutValSF;
    
    
    //names for the histograms
    TString nameHistInZMass, nameHistOutZMass;
    
    int numDilepChannels;
    
    //vector for the systematics
    vector<TString> vecSystNames;
    
    //vector for the names of the channels
    vector<TString> vecChanNames;
    
    //vector to contain histograms in DY MC, including systematic variations
    vector<TH1F *> vecHistDYMC_InZMass_CV;
    vector<TH1F *> vecHistDYMC_OutZMass_CV;
    
    vector< vector<TH1F *> > vecVecHistDYMC_InZMass_SystVarUp;
    vector< vector<TH1F *> > vecVecHistDYMC_InZMass_SystVarDown;
    
    vector< vector<TH1F *> > vecVecHistDYMC_OutZMass_SystVarUp;
    vector< vector<TH1F *> > vecVecHistDYMC_OutZMass_SystVarDown;
    
    ///hack SFs to correct the SF for the ZCR,
    //derived from running this command
    //./StopPlotsSpectraShowNewDESY_wSyst_Test ReleaseTheKraken useTTBarDDEst 0 useDYDDEst 5 wTTbarGen 2 doReReco noFakes whichSS 1 nameWChan _inZ_ZCR noGenRecRW doSpecBound specHist h_Smear JsSm 0 whichDL ${DL}
    //where ${DL} is 0, 1, or 2
    vector<double> vecZCRHackSF;
    
    //vector to contain MuMu, EE, and EMu histograms for Data;
    vector<TH1F *> vecHistData_InZMass;
    vector<TH1F *> vecHistData_OutZMass;
    
    //Stuff for the output
    vector<ValError> vecSF_CV;
    vector<ValError> vecYield_CV;
    vector<ValError> vecROutIn_CV;
    
    vector< vector<ValError> > vecVecSF_SystVarUp;
    vector< vector<ValError> > vecVecSF_SystVarDown;
    vector< vector<ValError> > vecVecYield_SystVarUp;
    vector< vector<ValError> > vecVecYield_SystVarDown;
    
    vector< vector<ValError> > vecVecROutIn_SystVarUp;
    vector< vector<ValError> > vecVecROutIn_SystVarDown;

    
    
    void PrintROutIn(bool doVerb = 0) {
        TString currName;
        for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
            currName = "DY ROutIn for " + vecChanNames[iChan];
            vecROutIn_CV[iChan].PrintOut(currName, true, "", true);
            for (int iSyst = 0; iSyst < (int) vecSystNames.size(); ++iSyst) {
                vecVecROutIn_SystVarUp[iChan][iSyst].PrintOut(currName, false, vecSystNames[iSyst] + TString("Up"), true);
                vecVecROutIn_SystVarDown[iChan][iSyst].PrintOut(currName, false, vecSystNames[iSyst] + TString("Down"), true);
            }
            ValError FullSyst = MakeFullSyst(&vecROutIn_CV[iChan], &vecVecROutIn_SystVarUp[iChan], &vecVecROutIn_SystVarDown[iChan], "FullSyst", doVerb);
            FullSyst.PrintOut(currName, false, FullSyst.Name, true);
        }
    }
    void PrintSF(bool doVerb = 0) {
        TString currName;
        for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
            currName = "DY SF for " + vecChanNames[iChan];
            vecSF_CV[iChan].PrintOut(currName, true, "", true);
            for (int iSyst = 0; iSyst < (int) vecSystNames.size(); ++iSyst) {
                vecVecSF_SystVarUp[iChan][iSyst].PrintOut(currName, false, vecSystNames[iSyst] + TString("Up"), true);
                vecVecSF_SystVarDown[iChan][iSyst].PrintOut(currName, false, vecSystNames[iSyst] + TString("Down"), true);
            }
            ValError FullSyst = MakeFullSyst(&vecSF_CV[iChan], &vecVecSF_SystVarUp[iChan], &vecVecSF_SystVarDown[iChan], "FullSyst", doVerb);
            FullSyst.PrintOut(currName, false, FullSyst.Name, true);
        }
    }
    void PrintYield(bool doVerb = 0) {
        TString currName;
        for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
            currName = "DY Yield for " + vecChanNames[iChan];
            vecYield_CV[iChan].PrintOut(currName, true, "", true);
            for (int iSyst = 0; iSyst < (int) vecSystNames.size(); ++iSyst) {
                vecVecYield_SystVarUp[iChan][iSyst].PrintOut(currName, false, vecSystNames[iSyst] + TString("Up"), true);
                vecVecYield_SystVarDown[iChan][iSyst].PrintOut(currName, false, vecSystNames[iSyst] + TString("Down"), true);
            }
            ValError FullSyst = MakeFullSyst(&vecYield_CV[iChan], &vecVecYield_SystVarUp[iChan], &vecVecYield_SystVarDown[iChan], "FullSyst", doVerb);
            FullSyst.PrintOut(currName, false, FullSyst.Name, true);
        }
    }
    void PrintVals(bool printSF = true, bool printYield = false, bool printROutIn = false, bool doVerb = 0) {
        if (printSF) {
            PrintSF(doVerb);
        }
        if (printYield) {
            PrintYield(doVerb);
        }
        if (printROutIn) {
            PrintROutIn(doVerb);
        }
    }
    
    void InitializeVecs() {
        vecChanNames.resize(0);
        vecChanNames.push_back("MuMu");
        vecChanNames.push_back("EE");
        vecChanNames.push_back("EMu");
        
        numDilepChannels = (int) vecChanNames.size();
        
        SetSystVec(&vecSystNames);
        
        //resize the vector of scale factors and of yields
        vecSF_CV.resize(numDilepChannels);
        vecYield_CV.resize(numDilepChannels);
        vecROutIn_CV.resize(numDilepChannels);
        
        vecVecSF_SystVarUp.resize(numDilepChannels);
        vecVecSF_SystVarDown.resize(numDilepChannels);

        vecVecYield_SystVarUp.resize(numDilepChannels);
        vecVecYield_SystVarDown.resize(numDilepChannels);
        
        vecVecROutIn_SystVarUp.resize(numDilepChannels);
        vecVecROutIn_SystVarDown.resize(numDilepChannels);
        for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
            vecVecSF_SystVarUp[iChan].resize(vecSystNames.size());
            vecVecSF_SystVarDown[iChan].resize(vecSystNames.size());

            vecVecYield_SystVarUp[iChan].resize(vecSystNames.size());
            vecVecYield_SystVarDown[iChan].resize(vecSystNames.size());
            
            vecVecROutIn_SystVarUp[iChan].resize(vecSystNames.size());
            vecVecROutIn_SystVarDown[iChan].resize(vecSystNames.size());
        }
        
        //resize the vectors of histograms in Data
        vecHistData_InZMass.resize(numDilepChannels);
        vecHistData_OutZMass.resize(numDilepChannels);
        
        //resize the vectors of histograms in MC
        vecHistDYMC_InZMass_CV.resize(numDilepChannels);
        vecHistDYMC_OutZMass_CV.resize(numDilepChannels);
        
        vecVecHistDYMC_InZMass_SystVarUp.resize(numDilepChannels);
        vecVecHistDYMC_InZMass_SystVarDown.resize(numDilepChannels);
        vecVecHistDYMC_OutZMass_SystVarUp.resize(numDilepChannels);
        vecVecHistDYMC_OutZMass_SystVarDown.resize(numDilepChannels);
        
        for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
            vecVecHistDYMC_InZMass_SystVarUp[iChan].resize(vecSystNames.size());
            vecVecHistDYMC_InZMass_SystVarDown[iChan].resize(vecSystNames.size());
            
            vecVecHistDYMC_OutZMass_SystVarUp[iChan].resize(vecSystNames.size());
            vecVecHistDYMC_OutZMass_SystVarDown[iChan].resize(vecSystNames.size());
        }
        
        vecZCRHackSF.resize(numDilepChannels);
        //vecZCRHackSF[0] = 1; // + (22547 - 20092.8)/17902.4; //1.; //22547./18994.9;
        //vecZCRHackSF[1] = 1; //+ (14517 - 13065.9)/11542.8; //1.; //14517./15565.9;
        //vecZCRHackSF[2] = 1;
        
        vecZCRHackSF[0] = 1 + (22547 - 24010.5)/21661; //1.; //22547./18994.9;
        vecZCRHackSF[1] = 1 + (14517 - 15552.4)/13953.9; //1.; //14517./15565.9;
        vecZCRHackSF[2] = 1;
        
        /*
         MuMu
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_Higgs is 1.15145
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_Rare is 26.52
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_SingTop is 45.5966
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_VV is 361.12
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_Fake is 502.83
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_VG is 683.645
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_TTBar is 728.587
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_ZDY is 21661
         Integral Data 22547
         Integral MC 24010.5
         
         EE
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_Higgs is 0.603614
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_Rare is 14.7046
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_SingTop is 26.8551
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_VV is 223.444
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_VG is 371.465
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_Fake is 469.212
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_TTBar is 492.178
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_ZDY is 13953.9
         Integral Data 14517
         Integral MC 15552.4
         
         EMu
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_Higgs is 0.173701
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_VG is 1.29704
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_Rare is 20.7999
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_ZDY is 20.9778
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_SingTop is 68.7956
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_Fake is 108.138
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_VV is 111.608
         Integral for sample: h_SmearMT2llZoom_inZ_ZCR_TTBar is 1158.57
         Integral Data 1389
         Integral MC 1490.36
         */
        
        
    }
    void SetInputFromCommandLine(int argc, char* argv[]) {
        cutMET_MT2ll = 1;
        cutValSF = -1;
        for (int k = 0; k < argc; ++k) {
            if (strncmp (argv[k],"cutVal", 6) == 0) {
                cutValSF = strtol(argv[k+1], NULL, 10);
            }
            else if (strncmp (argv[k],"cutMET", 6) == 0) {
                cutMET_MT2ll = strtol(argv[k+1], NULL, 10);
            }
        }

    }
    void DefaultVals(TString histBaseName, TString histEndName, bool doVerb = 0) {
        TString strInZ = "_inZ";
        TString strOutZ = "_outZ";
        
        nameHistInZMass = histBaseName + strInZ + histEndName;
        nameHistOutZMass = histBaseName + strOutZ + histEndName;
        
        if (doVerb) {
            cout << "nameHistInZMass " << nameHistInZMass << endl;
            cout << "nameHistOutZMass " << nameHistOutZMass << endl;
        }
        
        InitializeVecs();
    }
    
    void LoadDataHistograms(vector<TString> * inVecDataNames, bool doVerb = 0) {
        TFile * currFileData;
        TString grabStringInZMass = nameHistInZMass;
        grabStringInZMass.Replace(grabStringInZMass.Index("Smear"), 5, "");
        TString grabStringOutZMass = nameHistOutZMass;
        grabStringOutZMass.Replace(grabStringOutZMass.Index("Smear"), 5, "");
        for (unsigned int iChan = 0; iChan < inVecDataNames->size(); ++iChan) {
            if (doVerb) {
                cout << "grabbing Data from file " << inVecDataNames->at(iChan) << endl;
                cout << "going to try and grab " << grabStringInZMass << " and " << grabStringOutZMass << endl;
            }
            currFileData = TFile::Open(inVecDataNames->at(iChan));
            vecHistData_InZMass[iChan] = (TH1F *) currFileData->Get(grabStringInZMass);
            vecHistData_OutZMass[iChan] = (TH1F *) currFileData->Get(grabStringOutZMass);
            if (doVerb) {
                cout << "iChan " << iChan << endl;
                cout << "currFileData name " << currFileData->GetName() << endl;
                cout << "vecHistData_InZMass name " << vecHistData_InZMass[iChan]->GetName() << endl;
                cout << "vecHistData_OutZMass name " << vecHistData_OutZMass[iChan]->GetName() << endl;
            }
        }
    }
    void LoadMCHistograms(vector<TString> * inVecMCNames, bool doVerb = 0) {
        TFile * currFileMC;
        
        float currSF;
        for (unsigned int iChan = 0; iChan < inVecMCNames->size(); ++iChan) {
            if (doVerb) {
                cout << "grabbing MC from file " << inVecMCNames->at(iChan) << endl;
            }
            currFileMC = TFile::Open(inVecMCNames->at(iChan));
            if (doVerb) {
                cout << "iChan " << iChan << endl;
                cout << "currFileMC Name " << currFileMC->GetName() << endl;
            }
            //grab central value hist, which has no extra name
            
            if (doVerb) {
                cout << "trying to grab " << nameHistInZMass << " and " << nameHistOutZMass << endl;
            }
            
            currSF = ScaleBackCalcBasic(currFileMC, doVerb, 0);
            
            vecHistDYMC_InZMass_CV[iChan] = (TH1F *) currFileMC->Get(nameHistInZMass);
            vecHistDYMC_InZMass_CV[iChan]->Scale(currSF);
            vecHistDYMC_OutZMass_CV[iChan] = (TH1F *) currFileMC->Get(nameHistOutZMass);
            vecHistDYMC_OutZMass_CV[iChan]->Scale(currSF);
            if (doVerb) {
                cout << "iChan " << iChan << endl;
                cout << "vecHistDYMC_InZMass_CV[iChan] Name " << vecHistDYMC_InZMass_CV[iChan]->GetName() << endl;
                cout << "vecHistDYMC_OutZMass_CV[iChan] Name " << vecHistDYMC_OutZMass_CV[iChan]->GetName() << endl;
            }
            
            //grab syst shift hists
            for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                if (doVerb) {
                    cout << "trying to grab systematic " << vecSystNames[iSyst] << endl;
                }
                if (vecSystNames[iSyst].Contains("Fake")) {
                    vecVecHistDYMC_InZMass_SystVarUp[iChan][iSyst] = (TH1F *) vecHistDYMC_InZMass_CV[iChan]->Clone(nameHistInZMass + vecSystNames[iSyst] + TString("ShiftUp"));
                    vecVecHistDYMC_OutZMass_SystVarUp[iChan][iSyst] = (TH1F *) vecHistDYMC_OutZMass_CV[iChan]->Clone(nameHistOutZMass + vecSystNames[iSyst] + TString("ShiftUp"));
                    
                    vecVecHistDYMC_InZMass_SystVarDown[iChan][iSyst] = (TH1F *) vecHistDYMC_InZMass_CV[iChan]->Clone(nameHistInZMass + vecSystNames[iSyst] + TString("ShiftDown"));
                    vecVecHistDYMC_OutZMass_SystVarDown[iChan][iSyst] = (TH1F *) vecHistDYMC_OutZMass_CV[iChan]->Clone(nameHistOutZMass + vecSystNames[iSyst] + TString("ShiftDown"));
                }
                else {
                    vecVecHistDYMC_InZMass_SystVarUp[iChan][iSyst] = (TH1F *) currFileMC->Get(nameHistInZMass + vecSystNames[iSyst] + TString("ShiftUp"));
                    vecVecHistDYMC_OutZMass_SystVarUp[iChan][iSyst] = (TH1F *) currFileMC->Get(nameHistOutZMass + vecSystNames[iSyst] + TString("ShiftUp"));
                    currSF = ScaleBackCalcBasic(currFileMC, doVerb, iSyst + 1, &vecSystNames);
                    vecVecHistDYMC_InZMass_SystVarUp[iChan][iSyst]->Scale(currSF);
                    vecVecHistDYMC_OutZMass_SystVarUp[iChan][iSyst]->Scale(currSF);
                    
                    vecVecHistDYMC_InZMass_SystVarDown[iChan][iSyst] = (TH1F *) currFileMC->Get(nameHistInZMass + vecSystNames[iSyst] + TString("ShiftDown"));
                    vecVecHistDYMC_OutZMass_SystVarDown[iChan][iSyst] = (TH1F *) currFileMC->Get(nameHistOutZMass + vecSystNames[iSyst] + TString("ShiftDown"));
                    currSF = ScaleBackCalcBasic(currFileMC, doVerb, -1 * (iSyst + 1), &vecSystNames);
                    vecVecHistDYMC_InZMass_SystVarDown[iChan][iSyst]->Scale(currSF);
                    vecVecHistDYMC_OutZMass_SystVarDown[iChan][iSyst]->Scale(currSF);
                }
            }
        }
    }
    
    void GrabIntegral(vector<float> * inVecIntegral, vector<float> * inVecIntegralErr, vector<TH1F *> * inVecCV, bool doVerb = 0, vector< vector<TH1F *> > * inVecVecMC_SystVarUp = 0, vector< vector<TH1F *> > * inVecVecMC_SystVarDown = 0, int iSyst = 0) {
        inVecIntegral->resize(numDilepChannels);
        inVecIntegralErr->resize(numDilepChannels);
        TH1F * histToUse;
        
        Double_t currIntError;
        
        for (unsigned int iChan = 0; iChan < numDilepChannels; ++iChan) {
            if (iSyst == 0) {
                histToUse = inVecCV->at(iChan);
            }
            else if (iSyst > 0) {
                if (inVecVecMC_SystVarUp == NULL) {
                    cout << "you're trying to grab from a Null histogram when iSyst = " << iSyst << endl;
                }
                else {
                    histToUse = inVecVecMC_SystVarUp->at(iChan)[abs(iSyst) - 1];
                }
            }
            else if (iSyst < 0) {
                if (inVecVecMC_SystVarDown == NULL) {
                    cout << "you're trying to grab from a Null histogram when iSyst = " << iSyst << endl;
                }
                else {
                    histToUse = inVecVecMC_SystVarDown->at(iChan)[abs(iSyst) - 1];
                }
            }
            else {
                cout << "something weird in DYestimator.h" << endl;
            }
            int whichBin = 0;
            if (cutValSF > -1) {
                whichBin = histToUse->GetXaxis()->FindBin(cutValSF);
                if (doVerb) {
                    cout << "histToUse name " << histToUse->GetName() << endl;
                    cout << "whichBin " << whichBin << endl;
                }
            }
            inVecIntegral->at(iChan) = histToUse->IntegralAndError(whichBin, histToUse->GetNbinsX() + 1, currIntError);
            inVecIntegralErr->at(iChan) = (float) currIntError;
            if (doVerb) {
                cout << "for iChan: " << iChan << " and iSyst = " << iSyst << ", for histogram " << histToUse->GetName() << endl;
                cout << "Integral +- error: " << inVecIntegral->at(iChan) << " +- " << inVecIntegralErr->at(iChan) << endl;
                cout << "Number of Entries: " << histToUse->GetEntries() << endl;
            }
        }
    }
    valPlusErr GetYieldPlusError(valPlusErr * pairYieldSFMainChannel, valPlusErr * pairYieldSFOtherChannel, valPlusErr * pairYieldOF, bool doVerb = 0) {
        valPlusErr outYield;
        
        float yieldSFMainChannel = pairYieldSFMainChannel->first;
        float yieldErrSFMainChannel = pairYieldSFMainChannel->second;
        
        float yieldSFOtherChannel = pairYieldSFOtherChannel->first;
        float yieldErrSFOtherChannel = pairYieldSFOtherChannel->second;

        float yieldOF = pairYieldOF->first;
        float yieldErrOF = pairYieldOF->second;

        
        float k_Dilep = sqrt(yieldSFMainChannel/yieldSFOtherChannel);
        
        float errTerm1 = (1 - 0.25 * k_Dilep * yieldOF / yieldSFMainChannel) * yieldErrSFMainChannel;
        float errTerm2 = (0.25 * yieldOF * k_Dilep / yieldSFOtherChannel) * yieldErrSFOtherChannel;
        float errTerm3 = (0.5 * k_Dilep) * yieldErrOF;
        vector<float> vecErrTerms(0);
        vecErrTerms.push_back(errTerm1);
        vecErrTerms.push_back(errTerm2);
        vecErrTerms.push_back(errTerm3);
        
        outYield.first = yieldSFMainChannel - 0.5 * k_Dilep * yieldOF;
        outYield.second = GetError(&vecErrTerms);
        
        if (doVerb) {
            cout << "Verbosity Info on Calculating the Yield " << endl;
            cout << "yieldSFMainChannel " << yieldSFMainChannel << endl;
            cout << "yieldErrSFMainChannel " << yieldErrSFMainChannel << endl;
            cout << "yieldSFOtherChannel " << yieldSFOtherChannel << endl;
            cout << "yieldErrSFOtherChannel " << yieldErrSFOtherChannel << endl;
            cout << "yieldOF " << yieldOF << endl;
            cout << "yieldErrOF " << yieldErrOF << endl;
            cout << "k_Dilep " << k_Dilep << endl;
            cout << "errTerm1 " << errTerm1 << endl;
            cout << "errTerm2 " << errTerm2 << endl;
            cout << "errTerm3 " << errTerm3 << endl;
            cout << "outYield.first " << outYield.first << endl;
            cout << "outYield.second " << outYield.second << endl;
        }
        
        return outYield;
    }
    valPlusErr GetROutIn(valPlusErr * pairYieldDY_InZ_MC, valPlusErr * pairYieldDY_OutZ_MC, bool doVerb = 0) {
        valPlusErr outR;
        outR.first = pairYieldDY_OutZ_MC->first / pairYieldDY_InZ_MC->first;
        
        float errTerm1 = pairYieldDY_OutZ_MC->second / pairYieldDY_InZ_MC->first;
        float errTerm2 = pairYieldDY_OutZ_MC->first * pairYieldDY_InZ_MC->second / (pow(pairYieldDY_InZ_MC->first, 2));
        
        vector<float> vecErrTerms(0);
        vecErrTerms.push_back(errTerm1);
        vecErrTerms.push_back(errTerm2);
        
        outR.second = GetError(&vecErrTerms);
        
        if (doVerb) {
            cout << "Verbosity Info on Calculating the ROutIn " << endl;
            cout << "MC yield in ZMass " << pairYieldDY_InZ_MC->first << endl;
            cout << "MC yield out ZMass " << pairYieldDY_OutZ_MC->first << endl;
            cout << "ROutIn: " << outR.first << endl;
            
            cout << "errTerm1 " << errTerm1 << endl;
            cout << "errTerm2 " << errTerm2 << endl;
            cout << "ROutIn Err: " << outR.second << endl;
        }
        
        return outR;
    }
    valPlusErr GetScaleFactor_SameFlavor(valPlusErr * pairYieldData, valPlusErr * pairYieldDY_InZ_MC, bool doVerb = 0) {
        valPlusErr outSF;
        outSF.first = pairYieldData->first / pairYieldDY_InZ_MC->first;
        
        float errTerm1 = pairYieldData->second / pairYieldDY_InZ_MC->first;
        float errTerm2 = pairYieldData->first * pairYieldDY_InZ_MC->second / (pow(pairYieldDY_InZ_MC->first, 2));
        
        vector<float> vecErrTerms(0);
        vecErrTerms.push_back(errTerm1);
        vecErrTerms.push_back(errTerm2);
        
        outSF.second = GetError(&vecErrTerms);
        
        if (doVerb) {
            cout << "Verbosity Info on Calculating the SF in same flavor " << endl;
            cout << "inputData yield " << pairYieldData->first << endl;
            cout << "MC yield " << pairYieldDY_InZ_MC->first << endl;
            cout << "SF: " << outSF.first << endl;
            
            cout << "errTerm1 " << errTerm1 << endl;
            cout << "errTerm2 " << errTerm2 << endl;
            cout << "SF Err: " << outSF.second << endl;
        }
        
        return outSF;
    }
    
    valPlusErr GetScaleFactor_EMu(valPlusErr * pairScaleFactor_EE, valPlusErr * pairScaleFactor_MuMu, vector<valPlusErr> * vecYieldVPEMC, vector<valPlusErr> * vecYieldVPEData, bool doVerb = 0) {
        //Take the geometric mean of the two quantities to be the EMu scale factor, techincally the two are correlated
        //but for now, assume they're not correlated when calculating the error...
        valPlusErr outSF_EMu;
        outSF_EMu.first = TMath::Sqrt(pairScaleFactor_EE->first * pairScaleFactor_MuMu->first);
        
        int indexMuMu = 0;
        int indexEE = 1;
        int indexEMu = 2;
        
        float sqrtF = vecYieldVPEData->at(indexMuMu).first * vecYieldVPEData->at(indexEE).first - vecYieldVPEData->at(indexEMu).first * TMath::Sqrt(vecYieldVPEData->at(indexMuMu).first * vecYieldVPEData->at(indexEE).first) + 0.25 * vecYieldVPEData->at(indexEMu).first * vecYieldVPEData->at(indexEMu).first;  //remaining terms inside the square root when you expand out SF_MuMu and SF_EE
        
        float errTerm1 = 0.5 * vecYieldVPEMC->at(indexMuMu).second * outSF_EMu.first / vecYieldVPEMC->at(indexMuMu).first;
        float errTerm2 = 0.5 * vecYieldVPEMC->at(indexEE).second * outSF_EMu.first / vecYieldVPEMC->at(indexEE).first;
        float errTerm3 = 0.5 * vecYieldVPEData->at(indexMuMu).second * outSF_EMu.first * (vecYieldVPEData->at(indexEE).first - 0.5 * vecYieldVPEData->at(indexEMu).first * TMath::Sqrt(vecYieldVPEData->at(indexEE).first/vecYieldVPEData->at(indexMuMu).first))  / sqrtF;
        float errTerm4 = 0.5 * vecYieldVPEData->at(indexEE).second * outSF_EMu.first * (vecYieldVPEData->at(indexMuMu).first - 0.5 * vecYieldVPEData->at(indexEMu).first * TMath::Sqrt(vecYieldVPEData->at(indexMuMu).first/vecYieldVPEData->at(indexEE).first))  / sqrtF;
        float errTerm5 = 0.5 * vecYieldVPEData->at(indexEMu).second * outSF_EMu.first * (TMath::Sqrt(vecYieldVPEData->at(indexMuMu).first * vecYieldVPEData->at(indexEE).first) - 0.5 * vecYieldVPEData->at(indexEMu).first)  / sqrtF;
        
        float covarTerm = 0.0; //no covariance the way I expanded it out
        vector<float> vecErrTerms(0);
        vecErrTerms.push_back(errTerm1);
        vecErrTerms.push_back(errTerm2);
        vecErrTerms.push_back(errTerm3);
        vecErrTerms.push_back(errTerm4);
        vecErrTerms.push_back(errTerm5);
        vecErrTerms.push_back(covarTerm);
        /*
        float errTerm1 = (0.5 * pairScaleFactor_EE->second) * TMath::Sqrt(pairScaleFactor_MuMu->first/pairScaleFactor_EE->first);
        float errTerm2 = (0.5 * pairScaleFactor_MuMu->second) * TMath::Sqrt(pairScaleFactor_EE->first/pairScaleFactor_MuMu->first);
        float covarTerm = TMath::Sqrt((1./8) * (1./(yieldMC_EE * yieldMC_MuMu)) * yieldErr_EMu * yieldErr_EMu); //Derivation up to first order of covariance term
         vector<float> vecErrTerms(0);
         vecErrTerms.push_back(errTerm1);
         vecErrTerms.push_back(errTerm2);
         vecErrTerms.push_back(covarTerm);
        */
        
        outSF_EMu.second = GetError(&vecErrTerms);
        
        if (doVerb) {
            cout << "Verbosity Info on Calculating the SF in opposite flavor " << endl;
            cout << "inputEE SF: " << pairScaleFactor_EE->first << endl;
            cout << "inputMuMu SF: " << pairScaleFactor_MuMu->first << endl;
            cout << "EMu SF: " << outSF_EMu.first << endl;
            
            cout << "errTerm1 " << errTerm1 << endl;
            cout << "errTerm2 " << errTerm2 << endl;
            cout << "errTerm3 " << errTerm3 << endl;
            cout << "errTerm4 " << errTerm4 << endl;
            cout << "errTerm5 " << errTerm5 << endl;
            cout << "covarTerm " << covarTerm << endl;
            cout << "EMu SF Err: " << outSF_EMu.second << endl;
        }
        
        return outSF_EMu;
    }
    
    valPlusErr GetDDYield(valPlusErr * pairScaleFactDY_InZ, valPlusErr * pairYieldDY_OutZ_MC, bool doVerb = 0) {
        valPlusErr outYield_DD;
        
        outYield_DD.first = pairScaleFactDY_InZ->first * pairYieldDY_OutZ_MC->first;
        
        float errTerm1 = pairYieldDY_OutZ_MC->second * pairScaleFactDY_InZ->first;
        float errTerm2 = pairYieldDY_OutZ_MC->first * pairScaleFactDY_InZ->second;
        vector<float> vecErrTerms(0);
        vecErrTerms.push_back(errTerm1);
        vecErrTerms.push_back(errTerm2);
        
        outYield_DD.second = GetError(&vecErrTerms);
        
        
        if (doVerb) {
            cout << "Verbosity Info on Calculating the DD Yield " << endl;
            cout << "Scale Factor in Z Mass: " << pairScaleFactDY_InZ->first << endl;
            cout << "DY Yield out of Z Mass: " << pairYieldDY_OutZ_MC->first << endl;
            cout << "Yield: " << outYield_DD.first << endl;
            
            cout << "errTerm1 " << errTerm1 << endl;
            cout << "errTerm2 " << errTerm2 << endl;
            cout << "Yield Err: " << outYield_DD.second << endl;
        }
        
        return outYield_DD;
    }
    
    void CalculateSFs(int iSyst = 0, bool doVerb = 0) {
        int indexMuMu = 0;
        int indexEE = 1;
        int indexEMu = 2;
        
        vector<float> vecIntegral_InZ_DYMC;
        vector<float> vecIntegralErr_InZ_DYMC;
        vector<float> vecIntegral_OutZ_DYMC;
        vector<float> vecIntegralErr_OutZ_DYMC;
        
        if (doVerb) {
            cout << "##############################" << endl;
            cout << "Grabbing Integral In Z Mass in MC" << endl;
            cout << "##############################" << endl;
        }
        GrabIntegral(&vecIntegral_InZ_DYMC, &vecIntegralErr_InZ_DYMC, &vecHistDYMC_InZMass_CV, doVerb, &vecVecHistDYMC_InZMass_SystVarUp, &vecVecHistDYMC_InZMass_SystVarDown, iSyst);
        if (doVerb) {
            cout << "##############################" << endl;
            cout << "Grabbing Integral Out Z Mass in MC" << endl;
            cout << "##############################" << endl;
        }
        GrabIntegral(&vecIntegral_OutZ_DYMC, &vecIntegralErr_OutZ_DYMC, &vecHistDYMC_OutZMass_CV, doVerb, &vecVecHistDYMC_OutZMass_SystVarUp, &vecVecHistDYMC_OutZMass_SystVarDown, iSyst);
        
        vector<float> vecIntegral_InZ_Data;
        vector<float> vecIntegralErr_InZ_Data;
        vector<float> vecIntegral_OutZ_Data;
        vector<float> vecIntegralErr_OutZ_Data;
        
        if (doVerb) {
            cout << "##############################" << endl;
            cout << "Grabbing Integral In Z Mass in Data" << endl;
            cout << "##############################" << endl;
        }
        GrabIntegral(&vecIntegral_InZ_Data, &vecIntegralErr_InZ_Data, &vecHistData_InZMass, doVerb);
        if (doVerb) {
            cout << "##############################" << endl;
            cout << "Grabbing Integral Out Z Mass in Data" << endl;
            cout << "##############################" << endl;
        }
        GrabIntegral(&vecIntegral_OutZ_Data, &vecIntegralErr_OutZ_Data, &vecHistData_OutZMass, doVerb);
        
        /////Calculate the Data yield expected from DY inside the Z mass window///////
        
        
        
        //valPlusErrs built for input into the GetYieldPlusError function
        /////Calculate the Data yield expected from DY inside the Z mass window///////
        vector<valPlusErr> vecPairYield_InZ_Data(numDilepChannels);
        
        //valPlusErrs built for MC input into the SF grabber and also into the total yield calculator
        vector<valPlusErr> vecPairYield_InZ_DYMC(numDilepChannels);
        vector<valPlusErr> vecPairYield_OutZ_DYMC(numDilepChannels);

        for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
            vecPairYield_InZ_Data[iChan] = make_pair(vecIntegral_InZ_Data[iChan], vecIntegralErr_InZ_Data[iChan]);
            
            vecPairYield_InZ_DYMC[iChan] = make_pair(vecIntegral_InZ_DYMC[iChan], vecIntegralErr_InZ_DYMC[iChan]);
            vecPairYield_OutZ_DYMC[iChan] = make_pair(vecIntegral_OutZ_DYMC[iChan], vecIntegralErr_OutZ_DYMC[iChan]);
        }
        
        //outputs for the yield and error on yield for DY inside the Z mass window for the two SF channels
        vector<valPlusErr> vecPairYieldDY_DD_InZ(2);
        if (doVerb) {
            cout << "##############################" << endl;
            cout << "Grabbing DD Yield In Z Mass in MuMu Data" << endl;
            cout << "##############################" << endl;
        }
        vecPairYieldDY_DD_InZ[indexMuMu] = GetYieldPlusError(&vecPairYield_InZ_Data[indexMuMu], &vecPairYield_InZ_Data[indexEE], &vecPairYield_InZ_Data[indexEMu], doVerb);
        if (doVerb) {
            cout << "##############################" << endl;
            cout << "Grabbing DD Yield In Z Mass in EE Data" << endl;
            cout << "##############################" << endl;
        }
        vecPairYieldDY_DD_InZ[indexEE] = GetYieldPlusError(&vecPairYield_InZ_Data[indexEE], &vecPairYield_InZ_Data[indexMuMu], &vecPairYield_InZ_Data[indexEMu], doVerb);
        
        /////Calculate the Scale Factor for Data/MC for DY inside the Z mass window///////
        
        //outputs for the Scale Factor and error on Scale Factor for DY inside the Z mass window for the two SF channels
        vector<valPlusErr> vecPairScaleFactDY_InZ(numDilepChannels);
        for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
            if (doVerb) {
                cout << "##############################" << endl;
                cout << "Grabbing SF In Z Mass in Data for channel: " << vecChanNames[iChan] << endl;
                cout << "##############################" << endl;
            }
            if (iChan != indexEMu) {
                vecPairScaleFactDY_InZ[iChan] = GetScaleFactor_SameFlavor(&vecPairYieldDY_DD_InZ[iChan], &vecPairYield_InZ_DYMC[iChan], doVerb);
            }
            else {
                vecPairScaleFactDY_InZ[iChan] = GetScaleFactor_EMu(&vecPairScaleFactDY_InZ[indexEE], &vecPairScaleFactDY_InZ[indexMuMu], &vecPairYield_InZ_DYMC, &vecPairYield_InZ_Data, doVerb);
            }
        }
        

        
        
        /////Calculate the Data-Driven yield expected from DY outside the Z mass window///////
        //Also calculate the ROutIn for MC//
        //vector of outputs to contain the Data-Driven yield and associated errors
        vector<valPlusErr> vecPairDDYieldDY(numDilepChannels);
        //vector of outputs to contain the MC based ROutIn and associated errors
        vector<valPlusErr> vecDYMCROutIn(numDilepChannels);
        for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
            if (doVerb) {
                cout << "##############################" << endl;
                cout << "Grabbing DD Yield Out Z Mass in Data for channel: " << vecChanNames[iChan] << endl;
                cout << "And calculating ROutIn in MC for channel: " << vecChanNames[iChan] << endl;
                cout << "##############################" << endl;
            }
            vecPairDDYieldDY[iChan] = GetDDYield(&vecPairScaleFactDY_InZ[iChan], &vecPairYield_OutZ_DYMC[iChan], doVerb);
            vecDYMCROutIn[iChan] = GetROutIn(&vecPairYield_InZ_DYMC[iChan], &vecPairYield_OutZ_DYMC[iChan], doVerb);
        }
        for (unsigned int iChan = 0; iChan < vecSF_CV.size(); ++iChan) {
            if (iSyst == 0) {
                vecSF_CV[iChan].SetStatError(&vecPairScaleFactDY_InZ[iChan]);
                vecYield_CV[iChan].SetStatError(&vecPairDDYieldDY[iChan]);
                vecROutIn_CV[iChan].SetStatError(&vecDYMCROutIn[iChan]);
            }
            else if (iSyst > 0) {
                vecVecSF_SystVarUp[iChan][abs(iSyst) - 1].SetStatError(&vecPairScaleFactDY_InZ[iChan]);
                vecVecYield_SystVarUp[iChan][abs(iSyst) - 1].SetStatError(&vecPairDDYieldDY[iChan]);
                vecVecROutIn_SystVarUp[iChan][abs(iSyst) - 1].SetStatError(&vecDYMCROutIn[iChan]);
            }
            else if (iSyst < 0) {
                vecVecSF_SystVarDown[iChan][abs(iSyst) - 1].SetStatError(&vecPairScaleFactDY_InZ[iChan]);
                vecVecYield_SystVarDown[iChan][abs(iSyst) - 1].SetStatError(&vecPairDDYieldDY[iChan]);
                vecVecROutIn_SystVarDown[iChan][abs(iSyst) - 1].SetStatError(&vecDYMCROutIn[iChan]);
            }
        }
    }
    void RunSFCalc_FullSet(vector<TString> * inVecDataNames, vector<TString> * inVecMCNames, bool doVerb = 0) {
        
        if (doVerb) {
            cout << "##############################" << endl;
            cout << "####Loading Data Histogram####" << endl;
            cout << "##############################" << endl;
        }
        LoadDataHistograms(inVecDataNames, doVerb);
        if (doVerb) {
            cout << "##############################" << endl;
            cout << "#####Loading MC Histogram#####" << endl;
            cout << "##############################" << endl;
        }
        LoadMCHistograms(inVecMCNames, doVerb);
        if (doVerb) {
            cout << "##############################" << endl;
            cout << "######Calculating CV SFs######" << endl;
            cout << "##############################" << endl;
        }
        CalculateSFs(0, doVerb);
        for (int iSyst = 0; iSyst < (int) vecSystNames.size(); ++iSyst) {
            if (doVerb) {
                cout << "################################" << endl;
                cout << "##Calculating SFs for iSyst " << iSyst + 1 << "##" << endl;
                cout << "################################" << endl;
            }
            CalculateSFs(iSyst + 1, doVerb);
            if (doVerb) {
                cout << "################################" << endl;
                cout << "##Calculating SFs for iSyst " << -1 * (iSyst + 1) << "##" << endl;
                cout << "################################" << endl;
            }
            CalculateSFs(-1 * (iSyst + 1), doVerb);
        }
    }
    void AddZCRHack() {
        for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
            cout << "vecZCRHackSF[ " << iChan << "] " << vecZCRHackSF[iChan] << endl;
            vecSF_CV[iChan].Scale(vecZCRHackSF[iChan]);
            for (unsigned int iSyst = 0; iSyst < vecVecSF_SystVarUp[iChan].size(); ++iSyst) {
                vecVecSF_SystVarUp[iChan][iSyst].Scale(vecZCRHackSF[iChan]);
                vecVecSF_SystVarDown[iChan][iSyst].Scale(vecZCRHackSF[iChan]);
            }
        }
    }
    void SetInputSW(SpecificWeight * inSW, int iChan, int iSyst = 0) {
        ValError * VEToUse;
        
        if (iChan < 0 || iChan >= numDilepChannels) {
            cout << "iChan needs to be between 0 and numDilepChannels (" << numDilepChannels << ") it is " << iChan << endl;
        }
        if (iSyst == 0) {
            VEToUse = &vecSF_CV[iChan];
        }
        else if (iSyst > 0) {
            VEToUse = &vecVecSF_SystVarUp[iChan][abs(iSyst) - 1];
        }
        else if (iSyst < 0) {
            VEToUse = &vecVecSF_SystVarDown[iChan][abs(iSyst) - 1];
        }
        inSW->SetSF(VEToUse, iSyst);
    }
    void SetAllInputSWs(vector<SpecificWeight> * inVecSWs) {
        for (unsigned int iChan = 0; iChan < inVecSWs->size(); ++iChan) {
            SetInputSW(&inVecSWs->at(iChan), iChan, 0);
            for (int iSyst = 1; iSyst < (int) vecSystNames.size() + 1; ++iSyst) {
                SetInputSW(&inVecSWs->at(iChan), iChan, iSyst);
                SetInputSW(&inVecSWs->at(iChan), iChan, -1 * iSyst);
            }
        }
    }
};
#endif