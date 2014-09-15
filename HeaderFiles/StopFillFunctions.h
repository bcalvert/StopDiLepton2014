#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TH3F.h"
#include "TH3D.h"
#include "TLorentzVector.h"
#include "TVector3.h"
//#include "TFile.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLatex.h"
#include "TProfile.h"
#include "TMath.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TF1.h"
// Header file for the classes stored in the TTree if any.                                                                            
//#include <Math/GenVector/PtEtaPhiM4D.h>
#include <vector>
#include <vector>
//#include <Math/GenVector/LorentzVector.h>

#include <iostream>
//#include <vector>
#include <cmath>
#include <sstream>
using namespace std;

inline float PileupRW(TH1 * nVtxSFHist, int nVtx) {
    int nVtxBin = nVtxSFHist->GetXaxis()->FindBin(nVtx);
    if (nVtxBin > nVtxSFHist->GetNbinsX()) nVtxBin =  nVtxSFHist->GetNbinsX();
    return (float) nVtxSFHist->GetBinContent(nVtxBin);
}
inline bool PassesStopBugCheck(float genStopMass0, float genStopMass1, float genChi0Mass0, float genChi0Mass1) {
//    return !(fabs(genStopMass0 -genStopMass1) > 0.1 || fabs(genChi0Mass0 - genChi0Mass1) > 0.1); // || (genChi0Mass0 < 0) || (genChi0Mass1< 0));
    return !(fabs(genStopMass0 -genStopMass1) > 0.1 || fabs(genChi0Mass0 - genChi0Mass1) > 0.1 || (genChi0Mass0 < 0) || (genChi0Mass1< 0));
}

inline bool PassesFullSelection(EventMETInfo * inEMI, EventJetInfo * inEJI, EventLepInfo * inELI) {
    if (inEJI->EventNJets < 2) return false;
    if (inEJI->EventNBtagJets < 1) return false;
    if (inELI->EventDiLepType < 2) {
        if (inELI->EventDiLepinZMass) return false;
        if (inEMI->EventMET < 40) return false;
    }
    return true;
}

inline void PrintEvent(BasicEventInfo * inBEI, EventMETInfo * inEMI, EventJetInfo * inEJI, EventLepInfo * inELI, EventDiStructureInfo * inEDSI, float MT2llThresh) {
    if (inEMI->MET_EMT2I.EventMT2ll > MT2llThresh && PassesFullSelection(inEMI, inEJI, inELI)) {
        cout << "EMI.EventMT2ll " << inEMI->MET_EMT2I.EventMT2ll << endl;
        cout << "EMI.EventMT2lb " << inEMI->MET_EMT2I.EventMT2lb << endl;
        inEMI->PrintVals();
        cout << "DiLepMass " << inEDSI->DP_DiLepton.BVC.Vec_Mass << endl;
        cout << "DiLepPt " << inEDSI->DP_DiLepton.BVC.Vec_Pt << endl;
        cout << "DPhiZMET " << inEDSI->DP_ZMET.DPhiInputObjects << endl;
        cout << "{run, event, lumiBlock} = {" << inBEI->runNumber << ", " << inBEI->eventNumber << ", " << inBEI->lumiBlock << "}" << endl;
    }
}

inline float ScaleFactorMC(int Type, int Syst) {
    
    // Type: 0 MuMu, 1 EE 2 EMu
    // Syst: 0 CentVal, 1 SystShiftUp, 2 SystShiftDown
    //    float SFTrig[3] = {0.994, 0.955, 0.978};
    //    float SFIDIso[3] = {0.993, 0.979, 0.986};    
    //return SFTrig[Type] * SFIDIso[Type];
    // Note! As of 8/10/13, trying an additional SF just for funsies, ok not really just for funsies, basically we find that the DD TTBar normalization is different for the three separate channels, which is bad news bears because it is indicative of different lepton reconstruction efficiency scale factors for data/MC for the different leptons
    /// 
    /*
     //30-10-2012
     float SF_trigger_mumu=0.965;// +/- 0.0102;
     float SF_trigger_ee  =0.962;// +/- 0.0130;
     float SF_trigger_mue =0.943;// +/- 0.0120;
     //02-11-2012
     float SF_IDISO_mumu=0.997;// +/- 0.00085;
     float SF_IDISO_ee  =0.975;// +/- 0.0006;
     float SF_IDISO_mue =0.986;// +/- 0.0007;
     *////
    float SFTotal[3], SFSystUp[3], SFSystDown[3];
    /*
    float SFTotal[3] = {0.962105, 0.9379500, 0.9297980};
    float SFSystUp[3] = {0.0102024, 0.0126881, 0.0185040};
    float SFSystDown[3] = {0.0102024, 0.0126881, 0.0185040};
     */
    /*
     float SFTotal[3] = {0.987, 0.957, 0.935};
     float SFSystUp[3] = {0.011, 0.015, 0.013};
     float SFSystDown[3] = {0.011, 0.015, 0.013};
     float MuMuAdditionalSF = 1.02169370264668;
     float EEAddtionalSF    = 0.977225138882017;
     float EMuAdditionalSF  = 0.999212074818846;
     
     float SFAdditional[3] = {MuMuAdditionalSF, EEAddtionalSF, EMuAdditionalSF};
     switch (Syst) {
     case 0:
     return SFTotal[Type] * SFAdditional[Type];
     break;
     case 1:
     return (SFTotal[Type] * SFAdditional[Type]) + SFSystUp[Type];
     break;      
     case -1:
     return (SFTotal[Type] * SFAdditional[Type]) - SFSystDown[Type];
     break;  
     default:
     return 1;
     break;
     }
     */
    
    switch (Syst) {
        case 0:
            return SFTotal[Type];
            break;            
        case 1:
            return SFTotal[Type] + SFSystUp[Type];
            break;            
        case -1:
            return SFTotal[Type] - SFSystDown[Type];
            break;
        default:
            return 1;
            break;            
    }
}

inline bool EventPassTrigger(BasicEventInfo * inBEI, EventLepInfo * inELI) {
    bool stillDoEvent;
    if (inELI->doEvent) {
        if (inELI->EventDiLepType == 0) stillDoEvent = inBEI->passTrigDoubleMu;
        else if (inELI->EventDiLepType == 1) stillDoEvent = inBEI->passTrigDoubleEl;
        else if (inELI->EventDiLepType == 2) stillDoEvent = inBEI->passTrigElMu;
        else {
            std::cout << "oddity with EventDiLepType for input ELI" << std::endl;
            stillDoEvent = false;
        }
    }
    return stillDoEvent;
}
inline float nVtxWeight(BasicEventInfo * inBEI, TH2F * nVtxSFHistOviToDESY, TH2F * nVtxSFHist_v2, TH2F * h_S7toS10RWHist, TH2F * nVtxSFHist) {
    float outNVtxWeight = inBEI->weight;
    if (!inBEI->doData) {
        if (inBEI->doHackPURW) {
            if (inBEI->isSignal) {
                outNVtxWeight = PileupRW(nVtxSFHist_v2, inBEI->nVtx);
                outNVtxWeight *= PileupRW(h_S7toS10RWHist, inBEI->nVtx);
            }
            else {
                outNVtxWeight = PileupRW(nVtxSFHist, inBEI->nVtx);      
            }
        }
    }
    return outNVtxWeight;
}

inline float nVtxWeight(BasicEventInfo * inBEI, TH1F * nVtxSFHistOviToDESY, TH1F * nVtxSFHist_v2, TH1F * h_S7toS10RWHist, TH1F * nVtxSFHist) {
    float outNVtxWeight = inBEI->weight;
    if (!inBEI->doData) {
        if (inBEI->doHackPURW) {
            if (inBEI->isSignal) {
                outNVtxWeight = PileupRW(nVtxSFHist_v2, inBEI->nVtx);
                outNVtxWeight *= PileupRW(h_S7toS10RWHist, inBEI->nVtx);
            }
            else {
                outNVtxWeight = PileupRW(nVtxSFHist, inBEI->nVtx);      
            }
        }
    }
    return outNVtxWeight;
}



inline bool IndJetNotBPassPt(PFJet * inJet, float pTCut) {
    return (inJet->P4.Pt() > pTCut && !inJet->isBJet);
}
inline bool ISRJetCutFullSelection(EventJetInfo * inEJI, float pTCut) {
    if (inEJI->EventNJets < 3) return false;
    if (IndJetNotBPassPt(&inEJI->vecEventJets[0], pTCut) || IndJetNotBPassPt(&inEJI->vecEventJets[1], pTCut) || IndJetNotBPassPt(&inEJI->vecEventJets[2], pTCut)) {
        return true;
    }
    else {
        return false;
    }
}
inline bool LepInBarrel(Lepton * inLep) {
    float barrelEtaEnd = 1.4442;
    return (fabs(inLep->P4.Eta()) < barrelEtaEnd);
}
inline bool LepInEndcap(Lepton * inLep) {
    float endcapEtaStart = 1.566;
    return (fabs(inLep->P4.Eta()) > endcapEtaStart);
}
inline bool DiLeptonGeometry(EventLepInfo * inELI, int whichCase) {
    // whichCase -- which case for individual lepton eta location we care about
    // 0 for both leptons in Barrel
    // 1 for one lepton in Barrel, one lepton not in Barrel (bot not necessarily in the Endcap)
    // 2 for both leptons in Endcap
    bool  Lep0InBarrel = LepInBarrel(&inELI->vecEventLeps[0]);
    bool  Lep0InEndcap = LepInEndcap(&inELI->vecEventLeps[0]);
    bool  Lep1InBarrel = LepInBarrel(&inELI->vecEventLeps[1]);
    bool  Lep1InEndcap = LepInEndcap(&inELI->vecEventLeps[1]);
    bool PassesGeometryCut = true;
    if (whichCase == 0) {
        PassesGeometryCut = Lep0InBarrel && Lep1InBarrel;
    }
    else if (whichCase == 1) {
        PassesGeometryCut = Lep0InBarrel || Lep1InBarrel;
        PassesGeometryCut &= !(Lep0InBarrel && Lep1InBarrel);
    }
    else if (whichCase == 2) {
        PassesGeometryCut = Lep0InEndcap && Lep1InEndcap;
    }
    else {
        cout << "issue with whichCase in DiLeptonGeometry function: " << whichCase << endl;
    }
    return PassesGeometryCut;    
}
