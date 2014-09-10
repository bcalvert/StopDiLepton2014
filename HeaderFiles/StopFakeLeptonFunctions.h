#include "TString.h"
#include "TH2F.h"
#include "TFile.h"

/*
typedef struct FakeLeptonCalculator {
    TH2F * histElecFakeRate, * histElecPromptRate,
    TH2F * histMuonFakeRate, * histMuonPromptRate;
    
    vector<TF1 *>
    
    void Initialize(TString baseLocFiles) {
        TFile *
        histElecFakeRate
    }
} FakeLeptonCalculator;
*/

typedef struct FakeLeptonCalculator {
    float epsLep0, epsLep1;
    float etaLep0, etaLep1;
    void PrintVals() {
        cout << "eps0:eta0 = " << epsLep0 << ":" << etaLep0 << endl;
        cout << "eps1:eta1 = " << epsLep1 << ":" << etaLep1 << endl;
    }
    pair<int, int> binCombo(EventLepInfo * inELI, int whichLep, TH2F * histToUse, int levelVerbosity = 0) {
        float lepPt = inELI->vecEventLeps[whichLep].BVC.Vec_Pt;
        float lepEta = inELI->vecEventLeps[whichLep].isElec() ? inELI->vecEventLeps[whichLep].TempEtaFixElectron() : inELI->vecEventLeps[whichLep].BVC.Vec_Eta;
        int xBin = histToUse->GetXaxis()->FindBin(abs(lepEta));
        int yBin = histToUse->GetYaxis()->FindBin(lepPt);
        
        if (xBin < 1 || xBin > histToUse->GetNbinsX()) {
            cout << "something weird " << lepEta << endl;
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
        if (levelVerbosity) {
            cout << "Lep0 PDGID " << inELI->vecEventLeps[0].PDGID << endl;
        }
        TH2F * histToUseLep0 = abs(inELI->vecEventLeps[0].PDGID) == 11 ? histElecPromptRate : histMuonPromptRate;
        pair<int, int> binPairLep0 = binCombo(inELI, 0, histToUseLep0, levelVerbosity);
        float promptRateLep0 = histToUseLep0->GetBinContent(binPairLep0.first, binPairLep0.second);
        if (promptRateLep0 < 1E-3) {
            cout << "something wrong with promptRateLep0 " << promptRateLep0 << endl;
        }

        etaLep0 = (1 - promptRateLep0) / promptRateLep0;
        
        if (levelVerbosity) {
            cout << "Lep1 PDGID " << inELI->vecEventLeps[1].PDGID << endl;
        }
        TH2F * histToUseLep1 = abs(inELI->vecEventLeps[1].PDGID) == 11 ? histElecPromptRate : histMuonPromptRate;
        pair<int, int> binPairLep1 = binCombo(inELI, 1, histToUseLep1, levelVerbosity);
        float promptRateLep1 = histToUseLep1->GetBinContent(binPairLep1.first, binPairLep1.second);
        if (promptRateLep1 < 1E-3) {
            cout << "something wrong with promptRateLep1 " << promptRateLep1 << endl;
        }
        etaLep1 = (1 - promptRateLep1) / promptRateLep1;
        
        if (levelVerbosity) {
            cout << "etaLep0 " << etaLep0 << endl;
            cout << "etaLep1 " << etaLep1 << endl;
        }
    }
    void SetEpsilon(EventLepInfo * inELI, TH2F * histElecFakeRate, TH2F * histMuonFakeRate, int levelVerbosity = 0) {
        if (levelVerbosity) {
            cout << "Lep0 PDGID " << inELI->vecEventLeps[0].PDGID << endl;
        }
        TH2F * histToUseLep0 = abs(inELI->vecEventLeps[0].PDGID) == 11 ? histElecFakeRate : histMuonFakeRate;
        pair<int, int> binPairLep0 = binCombo(inELI, 0, histToUseLep0, levelVerbosity);
        float fakeRateLep0 = histToUseLep0->GetBinContent(binPairLep0.first, binPairLep0.second);
        if (fakeRateLep0 < 1E-3) {
            cout << "something wrong with fakeRateLep0 " << fakeRateLep0 << endl;
        }
        epsLep0 = fakeRateLep0 / (1 - fakeRateLep0);
        
        if (levelVerbosity) {
            cout << "Lep1 PDGID " << inELI->vecEventLeps[1].PDGID << endl;
        }
        TH2F * histToUseLep1 = abs(inELI->vecEventLeps[1].PDGID) == 11 ? histElecFakeRate : histMuonFakeRate;
        pair<int, int> binPairLep1 = binCombo(inELI, 1, histToUseLep1, levelVerbosity);
        float fakeRateLep1 = histToUseLep1->GetBinContent(binPairLep1.first, binPairLep1.second);
        if (fakeRateLep1 < 1E-3) {
            cout << "something wrong with fakeRateLep1 " << fakeRateLep1 << endl;
        }
        epsLep1 = fakeRateLep1 / (1 - fakeRateLep1);
        
        if (levelVerbosity) {
            cout << "epsLep0 " << epsLep0 << endl;
            cout << "epsLep1 " << epsLep1 << endl;
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
    float GetWeightPassPass(int levelVerbosity = 0) {
        if (levelVerbosity) {
            cout << "getting weight for Pass-Pass case " << endl;
        }
        float numerator = (epsLep0 * etaLep0 + epsLep1*etaLep1) - (etaLep0 * etaLep0 * epsLep0 * epsLep1);
        numerator *= -1;
        if (levelVerbosity) {
            cout << "numerator " << numerator << endl;
        }
        float denominator = (1 - etaLep0 * epsLep0) * (1 - etaLep1 * epsLep1);
        if (levelVerbosity) {
            cout << "denominator " << denominator << endl;
        }
        return numerator / denominator;
    }
    float GetWeightPassFail(int levelVerbosity = 0) {
        if (levelVerbosity) {
            cout << "getting weight for Pass-Fail case " << endl;
        }
        float numerator = epsLep1;
        if (levelVerbosity) {
            cout << "numerator " << numerator << endl;
        }
        float denominator = (1 - etaLep0 * epsLep0) * (1 - etaLep1 * epsLep1);
        if (levelVerbosity) {
            cout << "denominator " << denominator << endl;
        }
        return numerator / denominator;
    }
    float GetWeightFailPass(int levelVerbosity = 0) {
        if (levelVerbosity) {
            cout << "getting weight for Fail-Pass case " << endl;
        }
        float numerator = epsLep0;
        if (levelVerbosity) {
            cout << "numerator " << numerator << endl;
        }
        float denominator = (1 - etaLep0 * epsLep0) * (1 - etaLep1 * epsLep1);
        if (levelVerbosity) {
            cout << "denominator " << denominator << endl;
        }
        return numerator / denominator;
    }
    float GetWeightFailFail(int levelVerbosity = 0) {
        if (levelVerbosity) {
            cout << "getting weight for Fail-Fail case " << endl;
        }
        float numerator = epsLep0 * epsLep1;
        numerator *= -1;
        if (levelVerbosity) {
            cout << "numerator " << numerator << endl;
        }
        float denominator = (1 - etaLep0 * epsLep0) * (1 - etaLep1 * epsLep1);
        if (levelVerbosity) {
            cout << "denominator " << denominator << endl;
        }
        return numerator / denominator;
    }
    //Functions for calculating the actual weight
    float GetWeight(EventLepInfo * inELI, int levelVerbosity = 0) {
        float outWeight = 1.0;
        if (levelVerbosity) {
            cout << "inELI->vecEventLeps[0].lepQuality " << inELI->vecEventLeps[0].lepQuality << endl;
            cout << "inELI->vecEventLeps[1].lepQuality " << inELI->vecEventLeps[1].lepQuality << endl;
            PrintVals();
        }
        if (inELI->vecEventLeps[0].lepQuality == 0) {
            if (inELI->vecEventLeps[1].lepQuality == 0) {
                outWeight = GetWeightFailFail(levelVerbosity);
            }
            else if (inELI->vecEventLeps[1].lepQuality == 1) {
                outWeight = GetWeightFailPass(levelVerbosity);
            }
            else {
                cout << "something weird: inELI->vecEventLeps[1].lepQuality " << inELI->vecEventLeps[1].lepQuality << endl;
            }
        }
        else if (inELI->vecEventLeps[0].lepQuality == 1) {
            if (inELI->vecEventLeps[1].lepQuality == 0) {
                outWeight = GetWeightPassFail(levelVerbosity);
            }
            else if (inELI->vecEventLeps[1].lepQuality == 1) {
                outWeight = GetWeightPassPass(levelVerbosity);
            }
            else {
                cout << "something weird: inELI->vecEventLeps[1].lepQuality " << inELI->vecEventLeps[1].lepQuality << endl;
            }
        }
        else {
            cout << "something weird: inELI->vecEventLeps[0].lepQuality " << inELI->vecEventLeps[0].lepQuality << endl;
        }
        if (levelVerbosity) {
            cout << "returned weight " << outWeight << endl;
        }
        return outWeight;
    }
    float GetWeightFullChain(EventLepInfo * inELI, TH2F * histElecFakeRate, TH2F * histElecPromptRate, TH2F * histMuonFakeRate, TH2F * histMuonPromptRate, int levelVerbosity = 0) {
        SetEtaAndEpsilon(inELI, histElecFakeRate, histElecPromptRate, histMuonFakeRate, histMuonPromptRate, levelVerbosity);
        float outWeight = GetWeight(inELI, levelVerbosity);
        return outWeight;
    }
} FakeLeptonCalculator;

/*
inline float EventWeight(EventLepInfo * inELI, TH2F * histElecFakeRate, TH2F * histElecPromptRate, TH2F * histMuonFakeRate, TH2F * histMuonPromptRate) {
    
}
*/