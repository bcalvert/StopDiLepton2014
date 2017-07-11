#include "TString.h"
#include "TH2F.h"
#include "TFile.h"

typedef pair<float, float> valPlusErr;

float GetError(vector<float> * inVecVarTerms, vector<float> * inVecCovarTerms) {
    //function to calculate a total approximate error for a function of random variables, assuming you have an input vector, inVecVarTerms,
    //containing the variance terms and another vector, inVecCovarTerms, containing the covariance terms
    //the input inVecVarTerms is assumed to have the un-squared variance terms, so they get squared
    //while the input inVecCovarTerms is assumed to have full terms, which are not squared so that relative signs can come into the picture
    float outErr = 0.0;
    for (unsigned int iVarVal = 0; iVarVal < inVecVarTerms->size(); ++iVarVal) {
        outErr += pow(inVecVarTerms->at(iVarVal), 2);
    }
    for (unsigned int iCoVarVal = 0; iCoVarVal < inVecCovarTerms->size(); ++iCoVarVal) {
        outErr += inVecCovarTerms->at(iCoVarVal);
    }
    return TMath::Sqrt(outErr);
}

typedef struct FakeLeptonCalculator {
    vector<valPlusErr> vecLepEpsilonVPE;
    vector<valPlusErr> vecLepEtaVPE;
    valPlusErr weightFakeVPE;
    bool isSameEta, isSameEpsilon;
    void InitializeVecs(int numLeps = 2) {
        vecLepEpsilonVPE.resize(numLeps);
        vecLepEtaVPE.resize(numLeps);
    }
    void PrintVals() {
        for (unsigned int iLep = 0; iLep < vecLepEpsilonVPE.size(); ++iLep) {
            cout << "for lepton " << iLep << " epsilon:eta is " << vecLepEpsilonVPE[iLep].first << ":" << vecLepEtaVPE[iLep].first << endl;
            cout << "for lepton " << iLep << " epsilon_err:eta_err is " << vecLepEpsilonVPE[iLep].second << ":" << vecLepEtaVPE[iLep].second << endl;
        }
        cout << "weightFake: " << weightFakeVPE.first << " +- " << weightFakeVPE.second << endl;
    }
    pair<int, int> binCombo(EventLepInfo * inELI, int whichLep, TH2F * histToUse, int levelVerbosity = 0) {
        float lepPt = inELI->vecEventLeps[whichLep].BVC.Vec_Pt;
        float lepEta = inELI->vecEventLeps[whichLep].isElec() ? inELI->vecEventLeps[whichLep].TempEtaFixElectron() : inELI->vecEventLeps[whichLep].BVC.Vec_Eta;
        int xBin = histToUse->GetXaxis()->FindBin(abs(lepEta));
        int yBin = histToUse->GetYaxis()->FindBin(lepPt);
        
        if (xBin < 1) {
            cout << "something weird " << lepEta << endl;
        }
        else if (xBin > histToUse->GetNbinsX()) {
            xBin = histToUse->GetNbinsX();
        }
        if (yBin < 1) {
            cout << "something weird " << lepPt << endl;
        }
        else if (yBin > histToUse->GetNbinsY()) {
            yBin = histToUse->GetNbinsY();
        }
        if (levelVerbosity) {
            cout << "whichLep " << whichLep << endl;
            cout << "lepPt " << lepPt << endl;
            cout << "lepEta " << lepEta << endl;
            cout << "xBin " << xBin << endl;
            cout << "yBin " << yBin << endl;
        }
        pair<int, int> binPair(xBin, yBin);
        return binPair;
    }
    void SetEta(EventLepInfo * inELI, TH2F * histElecPromptRate, TH2F * histMuonPromptRate, int levelVerbosity = 0) {
        //TH2F * currLepHistToUse; to use?
        pair<int, int> currLepBinPair;
        pair<int, int> firstLepBinPair(-1,-1);
        pair<int, int> secondLepBinPair(-1,-1);
        
        valPlusErr currPromptRateVPE;
        valPlusErr currLepEtaVPE;
        

        
        for (unsigned int iLep = 0; iLep < vecLepEtaVPE.size(); ++iLep) {
            if (levelVerbosity) {
                cout << "PDGID for Lepton " << iLep << ": " << inELI->vecEventLeps[iLep].PDGID << endl;
            }
            TH2F * currLepHistToUse = abs(inELI->vecEventLeps[iLep].PDGID) == 11 ? histElecPromptRate : histMuonPromptRate;
            currLepBinPair = binCombo(inELI, iLep, currLepHistToUse, levelVerbosity);
            
            if (iLep == 0) {
                firstLepBinPair = currLepBinPair;
            }
            else if (iLep == 1) {
                secondLepBinPair = currLepBinPair;
            }
            
            currPromptRateVPE.first = currLepHistToUse->GetBinContent(currLepBinPair.first, currLepBinPair.second);
            if (currPromptRateVPE.first < 1E-3) {
                cout << "something wrong with currPromptRate for iLep = " << iLep << " it is very small -- " << currPromptRateVPE.first << endl;
            }
            currPromptRateVPE.second = currLepHistToUse->GetBinError(currLepBinPair.first, currLepBinPair.second);
            if (currPromptRateVPE.second < 1E-5) {
                cout << "something wrong with currPromptRateErr for iLep = " << iLep << " it is very small -- " << currPromptRateVPE.second << endl;
            }
            
            currLepEtaVPE.first = (1 - currPromptRateVPE.first) / currPromptRateVPE.first;
            currLepEtaVPE.second = currPromptRateVPE.second / (currPromptRateVPE.first * currPromptRateVPE.first);
            
            if (levelVerbosity) {
                cout << "iLep: " << iLep << endl;
                cout << "currPromptRate: " << currPromptRateVPE.first << endl;
                cout << "currPromptRateErr: " << currPromptRateVPE.second << endl;
            }
            
            vecLepEtaVPE[iLep].first = currLepEtaVPE.first;
            vecLepEtaVPE[iLep].second = currLepEtaVPE.second;
        }
        
        isSameEta = false;
        if (abs(inELI->vecEventLeps[0].PDGID) == abs(inELI->vecEventLeps[1].PDGID)) {
            isSameEta = (firstLepBinPair.first == secondLepBinPair.first && firstLepBinPair.second == secondLepBinPair.second);
            //FIXME: Temporarily shorting the above and instead going to require consistency of the central value and error of the prompt rate
            //as the histograms have "finer" binning than the actual information that I have..."
            if (fabs(vecLepEtaVPE[0].first - vecLepEtaVPE[1].first) < 1E-6 && fabs(vecLepEtaVPE[0].second - vecLepEtaVPE[1].second) < 1E-6) {
                isSameEta = true;
            }
            else {
                isSameEta = false;
            }
        }
        
        if (levelVerbosity) {
            for (unsigned int iLep = 0; iLep < vecLepEtaVPE.size(); ++iLep) {
                cout << "Eta for iLep " << iLep << " is " << vecLepEtaVPE[iLep].first << endl;
                cout << "Eta Err for iLep " << iLep << " is " << vecLepEtaVPE[iLep].second << endl;
            }
        }
    }
    void SetEpsilon(EventLepInfo * inELI, TH2F * histElecFakeRate, TH2F * histMuonFakeRate, int levelVerbosity = 0) {
        //TH2F * currLepHistToUse; to use?
        pair<int, int> currLepBinPair;
        
        valPlusErr currFakeRateVPE;
        valPlusErr currLepEpsilonVPE;
        pair<int, int> firstLepBinPair(-1,-1);
        pair<int, int> secondLepBinPair(-1,-1);
        
        for (unsigned int iLep = 0; iLep < vecLepEpsilonVPE.size(); ++iLep) {
            if (levelVerbosity) {
                cout << "PDGID for Lepton " << iLep << ":" << inELI->vecEventLeps[iLep].PDGID << endl;
            }
            TH2F * currLepHistToUse = abs(inELI->vecEventLeps[iLep].PDGID) == 11 ? histElecFakeRate : histMuonFakeRate;
            currLepBinPair = binCombo(inELI, iLep, currLepHistToUse, levelVerbosity);
            
            if (iLep == 0) {
                firstLepBinPair = currLepBinPair;
            }
            else if (iLep == 1) {
                secondLepBinPair = currLepBinPair;
            }
            
            currFakeRateVPE.first = currLepHistToUse->GetBinContent(currLepBinPair.first, currLepBinPair.second);
            if (currFakeRateVPE.first < 1E-3) {
                cout << "something wrong with currFakeRate for iLep = " << iLep << " it is very small -- " << currFakeRateVPE.first << endl;
            }
            currFakeRateVPE.second = currLepHistToUse->GetBinError(currLepBinPair.first, currLepBinPair.second);
            if (currFakeRateVPE.second < 1E-5) {
                cout << "something wrong with currPromptRateErr for iLep = " << iLep << " it is very small -- " << currFakeRateVPE.second << endl;
            }
            
            currLepEpsilonVPE.first = currFakeRateVPE.first / (1 - currFakeRateVPE.first);
            currLepEpsilonVPE.second = currFakeRateVPE.second / ((1 - currFakeRateVPE.first) * (1 - currFakeRateVPE.first));
            
            if (levelVerbosity) {
                cout << "iLep: " << iLep << endl;
                cout << "currFakeRate: " << currFakeRateVPE.first << endl;
                cout << "currFakeRateErr: " << currFakeRateVPE.second << endl;
            }
            
            vecLepEpsilonVPE[iLep].first = currLepEpsilonVPE.first;
            vecLepEpsilonVPE[iLep].second = currLepEpsilonVPE.second;
        }
        
        isSameEpsilon = false;
        if (abs(inELI->vecEventLeps[0].PDGID) == abs(inELI->vecEventLeps[1].PDGID)) {
            isSameEpsilon = (firstLepBinPair.first == secondLepBinPair.first && firstLepBinPair.second == secondLepBinPair.second);
            //FIXME: Temporarily shorting the above and instead going to require consistency of the central value and error of the fake rate
            //as the histograms have "finer" binning than the actual information that I have..."
            if (fabs(vecLepEpsilonVPE[0].first - vecLepEpsilonVPE[1].first) < 1E-6 && fabs(vecLepEpsilonVPE[0].second - vecLepEpsilonVPE[1].second) < 1E-6) {
                isSameEpsilon = true;
            }
            else {
                isSameEpsilon = false;
            }
        }
        if (levelVerbosity) {
            for (unsigned int iLep = 0; iLep < vecLepEpsilonVPE.size(); ++iLep) {
                cout << "Epsilon for iLep " << iLep << " is " << vecLepEpsilonVPE[iLep].first << endl;
                cout << "Epsilon Err for iLep " << iLep << " is " << vecLepEpsilonVPE[iLep].second << endl;
            }
        }
    }
    void SetEtaAndEpsilon(EventLepInfo * inELI, TH2F * histElecFakeRate, TH2F * histElecPromptRate, TH2F * histMuonFakeRate, TH2F * histMuonPromptRate, int levelVerbosity = 0) {
        if (levelVerbosity) {
            cout << "about to set eta " << endl;
        }
        SetEta(inELI, histElecPromptRate, histMuonPromptRate, levelVerbosity);
        if (levelVerbosity) {
            cout << "about to set epsilon " << endl;
        }
        SetEpsilon(inELI, histElecFakeRate, histMuonFakeRate, levelVerbosity);
    }
    void SetWeightPP(int levelVerbosity = 0) {
        if (levelVerbosity) {
            cout << "terms comprising PP numerator" << endl;
            cout << "term 1 " << vecLepEtaVPE[0].first * vecLepEpsilonVPE[0].first << endl;
            cout << "term 2 " << vecLepEtaVPE[1].first * vecLepEpsilonVPE[1].first << endl;
            cout << "term 3 " << vecLepEtaVPE[0].first * vecLepEpsilonVPE[0].first * vecLepEtaVPE[1].first * vecLepEpsilonVPE[1].first << endl;
        }
        float numerPP = vecLepEtaVPE[0].first * vecLepEpsilonVPE[0].first;
        numerPP += vecLepEtaVPE[1].first * vecLepEpsilonVPE[1].first;
        numerPP -= vecLepEtaVPE[0].first * vecLepEpsilonVPE[0].first * vecLepEtaVPE[1].first * vecLepEpsilonVPE[1].first;
        numerPP *= -1;
        float denomTerm0 = 1- vecLepEtaVPE[0].first * vecLepEpsilonVPE[0].first;
        float denomTerm1 = 1- vecLepEtaVPE[1].first * vecLepEpsilonVPE[1].first;
        float denom = denomTerm0 * denomTerm1;
        
        weightFakeVPE.first = numerPP / denom;
        
        float errTermEps0 = -1 * vecLepEtaVPE[0].first * vecLepEpsilonVPE[0].second / (TMath::Power(denomTerm0, 2) * denomTerm1);
        float errTermEta0 = -1 * vecLepEpsilonVPE[0].first * vecLepEtaVPE[0].second / (TMath::Power(denomTerm0, 2) * denomTerm1);
        float errTermEps1 = -1 * vecLepEtaVPE[1].first * vecLepEpsilonVPE[1].second / (denomTerm0 * TMath::Power(denomTerm1, 2));
        float errTermEta1 = -1 * vecLepEpsilonVPE[1].first * vecLepEtaVPE[1].second / (denomTerm0 * TMath::Power(denomTerm1, 2));
        
        float covarTermEps = isSameEpsilon ? 2 * errTermEps0 * errTermEps1 : 0;
        float covarTermEta = isSameEta ? 2 * errTermEta0 * errTermEta1 : 0;

        vector<float> vecVarTerms(0);
        vector<float> vecCovarTerms(0);
        vecVarTerms.push_back(errTermEps0);
        vecVarTerms.push_back(errTermEps1);
        vecVarTerms.push_back(errTermEta0);
        vecVarTerms.push_back(errTermEta1);
        vecCovarTerms.push_back(covarTermEps);
        vecCovarTerms.push_back(covarTermEta);

        weightFakeVPE.second = GetError(&vecVarTerms, &vecCovarTerms);
        if (levelVerbosity) {
            cout << "numerPP " << numerPP << endl;
            cout << "denom " << denom << endl;
            cout << "weightFake " << weightFakeVPE.first << endl;
         
            cout << "errTermEps0 " << errTermEps0 << endl;
            cout << "errTermEps1 " << errTermEps1 << endl;
            cout << "covarTermEps " << covarTermEps << endl;
            cout << "errTermEta0 " << errTermEta0 << endl;
            cout << "errTermEta1 " << errTermEta1 << endl;
            cout << "covarTermEta " << covarTermEta << endl;
            
            cout << "weightFakeErr " << weightFakeVPE.second << endl;
        }
    }
    
    void SetWeightPF(int levelVerbosity = 0) {
        float numerPF = vecLepEpsilonVPE[1].first;
        float denomTerm0 = 1 - vecLepEtaVPE[0].first * vecLepEpsilonVPE[0].first;
        float denomTerm1 = 1 - vecLepEtaVPE[1].first * vecLepEpsilonVPE[1].first;
        float denom = denomTerm0 * denomTerm1;
        
        weightFakeVPE.first = numerPF / denom;
        
        float errTermEps0 = vecLepEpsilonVPE[1].first * vecLepEtaVPE[0].first * vecLepEpsilonVPE[0].second / (TMath::Power(denomTerm0, 2) * denomTerm1);
        float errTermEta0 = vecLepEpsilonVPE[1].first * vecLepEpsilonVPE[0].first * vecLepEtaVPE[0].second / (TMath::Power(denomTerm0, 2) * denomTerm1);
        
        float errTermEps1 = vecLepEpsilonVPE[1].second / (denomTerm0 * TMath::Power(denomTerm1, 2));
        float errTermEta1 = vecLepEpsilonVPE[1].first * vecLepEpsilonVPE[1].first * vecLepEtaVPE[1].second / (denomTerm0 * TMath::Power(denomTerm1, 2));
        
        float covarTermEps = isSameEpsilon ? 2 * errTermEps0 * errTermEps1  : 0;
        float covarTermEta = isSameEta ? 2 * errTermEta0 * errTermEta1  : 0;
        
        vector<float> vecVarTerms(0);
        vector<float> vecCovarTerms(0);
        vecVarTerms.push_back(errTermEps0);
        vecVarTerms.push_back(errTermEps1);
        vecCovarTerms.push_back(covarTermEps);
        vecVarTerms.push_back(errTermEta0);
        vecVarTerms.push_back(errTermEta1);
        vecCovarTerms.push_back(covarTermEta);
        
        weightFakeVPE.second = GetError(&vecVarTerms, &vecCovarTerms);
        if (levelVerbosity) {
            cout << "numerPF " << numerPF << endl;
            cout << "denom " << denom << endl;
            cout << "weightFake " << weightFakeVPE.first << endl;
            
            cout << "errTermEps0 " << errTermEps0 << endl;
            cout << "errTermEps1 " << errTermEps1 << endl;
            cout << "covarTermEps " << covarTermEps << endl;
            cout << "errTermEta0 " << errTermEta0 << endl;
            cout << "errTermEta1 " << errTermEta1 << endl;
            cout << "covarTermEta " << covarTermEta << endl;
            
            cout << "weightFakeErr " << weightFakeVPE.second << endl;
        }
    }
    
    void SetWeightFP(int levelVerbosity = 0) {
        float numerFP = vecLepEpsilonVPE[0].first;
        float denomTerm0 = 1 - vecLepEtaVPE[0].first * vecLepEpsilonVPE[0].first;
        float denomTerm1 = 1 - vecLepEtaVPE[1].first * vecLepEpsilonVPE[1].first;
        float denom = denomTerm0 * denomTerm1;
        
        weightFakeVPE.first = numerFP / denom;
        
        float errTermEps0 = vecLepEpsilonVPE[0].second / (TMath::Power(denomTerm0, 2) * denomTerm1);
        float errTermEta0 = vecLepEpsilonVPE[0].first * vecLepEpsilonVPE[0].first * vecLepEtaVPE[0].second / (TMath::Power(denomTerm0, 2) * denomTerm1);
        
        float errTermEps1 = vecLepEpsilonVPE[0].first * vecLepEtaVPE[1].first * vecLepEpsilonVPE[1].second / (denomTerm0 * TMath::Power(denomTerm1, 2));
        float errTermEta1 = vecLepEpsilonVPE[0].first * vecLepEpsilonVPE[1].first * vecLepEtaVPE[1].second / (denomTerm0 * TMath::Power(denomTerm1, 2));
        
        float covarTermEps = isSameEpsilon ? TMath::Sqrt(2 * errTermEps0 * errTermEps1)  : 0;
        float covarTermEta = isSameEta ? TMath::Sqrt(2 * errTermEta0 * errTermEta1)  : 0;
        
        vector<float> vecVarTerms(0);
        vector<float> vecCovarTerms(0);
        vecVarTerms.push_back(errTermEps0);
        vecVarTerms.push_back(errTermEps1);
        vecVarTerms.push_back(errTermEta0);
        vecVarTerms.push_back(errTermEta1);
        vecCovarTerms.push_back(covarTermEps);
        vecCovarTerms.push_back(covarTermEta);
        
        weightFakeVPE.second = GetError(&vecVarTerms, &vecCovarTerms);
        if (levelVerbosity) {
            cout << "numerFP " << numerFP << endl;
            cout << "denom " << denom << endl;
            cout << "weightFake " << weightFakeVPE.first << endl;
            
            cout << "errTermEps0 " << errTermEps0 << endl;
            cout << "errTermEps1 " << errTermEps1 << endl;
            cout << "covarTermEps " << covarTermEps << endl;
            cout << "errTermEta0 " << errTermEta0 << endl;
            cout << "errTermEta1 " << errTermEta1 << endl;
            cout << "covarTermEta " << covarTermEta << endl;
            
            cout << "weightFakeErr " << weightFakeVPE.second << endl;
        }
    }
    
    
    
    void SetWeightFF(int levelVerbosity = 0) {
        float numerFF = -1 * vecLepEpsilonVPE[0].first * vecLepEpsilonVPE[1].first;
        float denomTerm0 = 1 - vecLepEtaVPE[0].first * vecLepEpsilonVPE[0].first;
        float denomTerm1 = 1 - vecLepEtaVPE[1].first * vecLepEpsilonVPE[1].first;
        float denom = denomTerm0 * denomTerm1;
        
        weightFakeVPE.first = numerFF / denom;
        
        float errTermEps0 = -1 * vecLepEpsilonVPE[0].second * vecLepEpsilonVPE[1].first / (TMath::Power(denomTerm0, 2) * denomTerm1);
        float errTermEta0 = -1 * vecLepEtaVPE[0].second * vecLepEpsilonVPE[0].first * vecLepEpsilonVPE[0].first * vecLepEpsilonVPE[1].first / (TMath::Power(denomTerm0, 2) * denomTerm1);
        
        float errTermEps1 = -1 * vecLepEpsilonVPE[1].second * vecLepEpsilonVPE[0].first / (denomTerm0 * TMath::Power(denomTerm1, 2));
        float errTermEta1 = -1 * vecLepEtaVPE[1].second * vecLepEpsilonVPE[1].first * vecLepEpsilonVPE[1].first * vecLepEpsilonVPE[0].first / (denomTerm0 * TMath::Power(denomTerm1, 2));
        
        float covarTermEps = isSameEpsilon ? 2 * errTermEps0 * errTermEps1  : 0;
        float covarTermEta = isSameEta ? 2 * errTermEta0 * errTermEta1  : 0;
        
        vector<float> vecVarTerms(0);
        vector<float> vecCovarTerms(0);
        vecVarTerms.push_back(errTermEps0);
        vecVarTerms.push_back(errTermEps1);
        vecVarTerms.push_back(errTermEta0);
        vecVarTerms.push_back(errTermEta1);
        vecCovarTerms.push_back(covarTermEps);
        vecCovarTerms.push_back(covarTermEta);
        
        weightFakeVPE.second = GetError(&vecVarTerms, &vecCovarTerms);
        if (levelVerbosity) {
            cout << "numerFF " << numerFF << endl;
            cout << "denom " << denom << endl;
            cout << "weightFake " << weightFakeVPE.first << endl;
            
            cout << "errTermEps0 " << errTermEps0 << endl;
            cout << "errTermEps1 " << errTermEps1 << endl;
            cout << "covarTermEps " << covarTermEps << endl;
            cout << "errTermEta0 " << errTermEta0 << endl;
            cout << "errTermEta1 " << errTermEta1 << endl;
            cout << "covarTermEta " << covarTermEta << endl;
            
            cout << "weightFakeErr " << weightFakeVPE.second << endl;
        }
    }
    void SetWeight(EventLepInfo * inELI, int levelVerbosity = 0) {
        if (inELI->vecEventLeps[0].lepQuality == 0) {
            if (inELI->vecEventLeps[1].lepQuality == 0) {
                SetWeightFF(levelVerbosity);
            }
            else if (inELI->vecEventLeps[1].lepQuality == 1) {
                SetWeightFP(levelVerbosity);
            }
            else {
                cout << "something weird: inELI->vecEventLeps[1].lepQuality " << inELI->vecEventLeps[1].lepQuality << endl;
            }
        }
        else if (inELI->vecEventLeps[0].lepQuality == 1) {
            if (inELI->vecEventLeps[1].lepQuality == 0) {
                SetWeightPF(levelVerbosity);
            }
            else if (inELI->vecEventLeps[1].lepQuality == 1) {
                SetWeightPP(levelVerbosity);
            }
            else {
                cout << "something weird: inELI->vecEventLeps[1].lepQuality " << inELI->vecEventLeps[1].lepQuality << endl;
            }
        }
        else {
            cout << "something weird: inELI->vecEventLeps[0].lepQuality " << inELI->vecEventLeps[0].lepQuality << endl;
        }
        if (levelVerbosity) {
            PrintVals();
        }
    }
    void GetWeightFullChain(EventLepInfo * inELI, TH2F * histElecFakeRate, TH2F * histElecPromptRate, TH2F * histMuonFakeRate, TH2F * histMuonPromptRate, int levelVerbosity = 0) {
        SetEtaAndEpsilon(inELI, histElecFakeRate, histElecPromptRate, histMuonFakeRate, histMuonPromptRate, levelVerbosity);
        SetWeight(inELI, levelVerbosity);
    }
} FakeLeptonCalculator;

/*
inline float EventWeight(EventLepInfo * inELI, TH2F * histElecFakeRate, TH2F * histElecPromptRate, TH2F * histMuonFakeRate, TH2F * histMuonPromptRate) {
    
}
*/
