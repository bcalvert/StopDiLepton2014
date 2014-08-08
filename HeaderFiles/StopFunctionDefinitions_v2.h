// very general set of functions used in the stop analysis
// this is the very first header file I wrote, so it's ugly and obfuscated
// gradually I am moving functions out of here into more specific areas

#ifndef HistSampFunc_h_
#define HistSampFunc_h_

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
#include <Math/GenVector/PtEtaPhiM4D.h>
#include <vector>
#include <Math/GenVector/LorentzVector.h>
//BTag shenanigans
#include "./BTagSFUtil.C"


#include <iostream>
#include <fstream>
//#include <vector>
#include <cmath>
#include <sstream>
#include <map>
using namespace std;



/*
 currently not working with this error:
 error: passing 'const TString' as 'this' argument of 'TString& TString::operator=(const TString&)' discards qualifiers [-fpermissive]
 void operator+=(const SampleT &a, const SampleT &b)
 {
 a.histNameSuffix = a.histNameSuffix + b.histNameSuffix;
 a.histXaxisSuffix += a.histXaxisSuffix + b.histXaxisSuffix;
 a.histYaxisSuffix += a.histYaxisSuffix + b.histYaxisSuffix;
 a.histZaxisSuffix += a.histZaxisSuffix + b.histZaxisSuffix;
 }
 */

typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > LV;
typedef std::vector<LV> VLV;

inline void METComponents(float &MET, float &METPhi, float &METX, float &METY, int METCompDirection) {
    // Bust MET Pt/Phi into MET X/Y or reverse
    
    // METCompDirection = 1 means take MET/METPhi to METX/METY, = 0 means opposite
    if (METCompDirection > 0) {
        METX = MET * TMath::Cos(METPhi);
        METY = MET * TMath::Sin(METPhi);
    }
    else {
        MET = TMath::Sqrt(METX * METX + METY * METY);
        METPhi = TMath::ATan2(METY, METX);
    }
}
inline vector<float> * SystWeights(TH1 * origHist, TH1 * rwHist) {
    // origHist is histogram for initial distribution, //rwHist is histogram for distribution
    // you wish to RW to -- both should be 1D (functionality for now) and have same nBins
    // reweighting is just bin by bin for now -- another consideration is to do some sort of shape fit
    // Another consideration is to return a histogram as that facilitates "finding" the weight to use a bit more
    vector<float> * weightVec = new vector<float>;
    float currWeight;
    float rwHistBC, origHistBC;
    int nBins = origHist->GetNbinsX();
    if (nBins != rwHist->GetNbinsX()) {
        cout << "ruh roh!!! nBins between two hists aren't the same" << endl;
        return weightVec;
    }
    for (int i = 0; i < nBins+2; ++i) {
        rwHistBC = rwHist->GetBinContent(i);
        origHistBC = origHist->GetBinContent(i);
        currWeight = origHistBC > 0 ? rwHistBC/origHistBC : 0;
        weightVec->push_back(currWeight);
    }
    return weightVec;
}

inline bool FilterMET(vector<bool> * regFilterVec, vector<bool> * oppFilterVec) {
    // boolean for checking if event passes met filters
    
    for (unsigned int iReg = 0; iReg < regFilterVec->size(); ++iReg) {
        if (!regFilterVec->at(iReg)) return false;
    }
    for (unsigned int iOpp = 0; iOpp < oppFilterVec->size(); ++iOpp) {
        if (oppFilterVec->at(iOpp)) return false;
    }
    return true;
}
inline bool BadTrigRunCheck(unsigned int RunNumber) {
    // returns boolean based on whether event was one of the bad trigger runs
    // !!!should cite the twiki here!!!
    
    bool PassesCheck = true;
    unsigned int BadRunList[5] = {190949, 191090, 191367, 193112, 193116};
    for (unsigned int i = 0; i < 5; ++i) {
        if (RunNumber == BadRunList[i]) PassesCheck = false;
    }
    return PassesCheck;
}
inline bool ElectronPassCutStage1(bool isEB, bool isEE, float SCEta, float deltaPhiIn, float deltaEtaIn, float sigIetaIeta, float HtoERatio) {
    // function that says if electron passes cuts we put on it
    // nominally would like to move this function into the ElectronEventPointers struct
    
    
    float EBDeltaPhiInCut = 0.06, EEDeltaPhiInCut = 0.03, deltaPhiInCut;
    float EBDeltaEtaInCut = 0.004, EEDeltaEtaInCut = 0.007, deltaEtaInCut;
    float EBSigIetaIetaCut = 0.01, EESigIetaIetaCut = 0.03, sigIetaIetaCut;
    float EBHtoERatio = 0.12, EEHtoERatio = 0.10, HtoERatioCut;
    float SCEtaMax = 2.5, SCEtaBarrelCut = 1.4442, SCEtaEndcapCut = 1.566;
    bool passCut = false;
    if (fabs(SCEta) >= SCEtaMax || ((fabs(SCEta) >= SCEtaBarrelCut ) && (fabs(SCEta) <= SCEtaEndcapCut))) {
        passCut = false;
        return passCut;
    }
    if (isEB) {
        deltaPhiInCut  = EBDeltaPhiInCut;
        deltaEtaInCut  = EBDeltaEtaInCut;
        sigIetaIetaCut = EBSigIetaIetaCut;
        HtoERatioCut   = EBHtoERatio;
    }
    else if (isEE) {
        deltaPhiInCut  = EEDeltaPhiInCut;
        deltaEtaInCut  = EEDeltaEtaInCut;
        sigIetaIetaCut = EESigIetaIetaCut;
        HtoERatioCut   = EEHtoERatio;
    }
    else {
        passCut = false;
        return passCut;
    }
    passCut = ((fabs(deltaPhiIn) < deltaPhiInCut) && (fabs(deltaEtaIn) < deltaEtaInCut) && (sigIetaIeta < sigIetaIetaCut) && (HtoERatio < HtoERatioCut));
    return passCut;
}
inline bool ElectronPassCutStage2(bool isPFElectron, bool passConvVeto, float relPFElecIso, float elecIP, float elecDZ, float elecE, float elecEoverP, float elecMissHits) {
    // function that says if electron passes cuts we put on it
    // nominally would like to move this function into the ElectronEventPointers struct

    bool passCut;
    float elecIsoRatioCut = 0.15;
    float elecEPCut = 0.05, elecDZCut = 0.1, elecIPCut = 0.02;
    int elecNumMissHitsCut = 1;
    if (!isPFElectron || !passConvVeto || relPFElecIso >= elecIsoRatioCut) {
        passCut = false;
        return passCut;
    }
    passCut = ((fabs(elecIP) < elecIPCut) && (fabs(elecDZ) < elecDZCut) && (fabs((1 - elecEoverP)/elecE) < elecEPCut) && (elecMissHits <= elecNumMissHitsCut));
    return passCut;
    //https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification    
}
inline bool ElectronPassCutDESY(float relPFElecIso, float elecEta) {
    // function that says if electron passes cuts we put on it -- was only relevant when we had DESY nTuples
    
    bool passCut;        
    float elecIsoRatioCut = 0.15; float elecEtaUB = 2.5;
    float barrelEtaEnd = 1.4442; float endcapEtaStart = 1.566;
    if (relPFElecIso > elecIsoRatioCut || fabs(elecEta) > elecEtaUB || (fabs(elecEta) > barrelEtaEnd && fabs(elecEta) < endcapEtaStart)) {
        passCut = false;
    }
    else {
        passCut = true;
    }
    return passCut;
}
inline void ElectronPickOvi(ElectronEventPointers inEEPs, float eventRhoIso, vector<Lepton> * vecIsoLeptons, int levelVerbosity) {
    // runs through the vectors of electron information (contained in the input ElectronEventPointers struct) and selects good electrons
    
    
    bool currElecPassCut;
    float currElecPt;
    float currRelPFElecIso;
    float currElecECalE, currElecSCEOverP, currElecSCEta;
    float currElecIP, currElecDZ;
    float currElecDeltaPhiIn, currElecDeltaEtaIn, currElecSigIetaIeta, currElecHtoERatio;
    int currElecNumMissHits;
    Lepton patsyLep;
    TLorentzVector patsyVec;
//    for (unsigned int i = 0; i < ElecPx->size(); ++i) {
    for (unsigned int i = 0; i < inEEPs.numElectrons; ++i) {
        patsyVec.SetPxPyPzE(inEEPs.ElecPx->at(i), inEEPs.ElecPy->at(i), inEEPs.ElecPz->at(i), inEEPs.ElecEn->at(i));        
        if (levelVerbosity > 0) {
            cout << "electron " << i << " has:" << endl;
            cout << "pT is " << patsyVec.Pt() << endl;
            cout << "Eta is " << patsyVec.Eta() << endl;
            cout << "Phi is " << patsyVec.Phi() << endl;
            cout << "PF pT is " << inEEPs.PFElecPt->at(i) << endl;
        }
        currElecPt = patsyVec.Pt();
        if (fabs(currElecPt - inEEPs.PFElecPt->at(i)) >= 10) continue;
        currElecECalE = inEEPs.ElecECalE->at(i); currElecSCEOverP = inEEPs.ElecSCEOverP->at(i); currElecSCEta = inEEPs.ElecSCEta->at(i);
        currRelPFElecIso = (inEEPs.ElecPFCharHadIso->at(i) + TMath::Max(0., inEEPs.ElecPFNeutHadIso->at(i) +  inEEPs.ElecPFPhotIso->at(i) - (eventRhoIso * ElectronEffectiveArea(currElecSCEta))))/currElecPt;
        currElecDZ = inEEPs.ElecDZ->at(i); currElecIP = inEEPs.ElecIP->at(i);
        currElecDeltaPhiIn = inEEPs.ElecDeltaPhiIn->at(i); currElecDeltaEtaIn = inEEPs.ElecDeltaEtaIn->at(i);
        currElecSigIetaIeta = inEEPs.ElecSigIetaIeta->at(i); currElecHtoERatio = inEEPs.ElecHtoERatio->at(i);
        currElecNumMissHits = inEEPs.ElecNumMissHits->at(i);        
        if (levelVerbosity > 0) {
            cout << "continuing for electron " << i << endl;
            cout << "currElecECalE " << currElecECalE << endl;
            cout << "currElecSCEOverP " << currElecSCEOverP << endl;
            cout << "currElecSCEta " << currElecSCEta << endl;
            cout << "currRelPFElecIso " << currRelPFElecIso << endl;
            cout << "currElecDZ " << currElecDZ << endl;
            cout << "currElecDeltaPhiIn " << currElecDeltaPhiIn << endl;
            cout << "currElecDeltaEtaIn " << currElecDeltaEtaIn << endl;
            cout << "currElecSigIetaIeta " << currElecSigIetaIeta << endl;
            cout << "currElecHtoERatio " << currElecHtoERatio << endl;
            cout << "currElecNumMissHits " << currElecNumMissHits << endl;
            cout << "ElecisEB->at(i) " << inEEPs.ElecisEB->at(i) << endl;
            cout << "ElecisEE->at(i) " << inEEPs.ElecisEE->at(i) << endl;
            cout << "isPFElectron->at(i) " << inEEPs.isPFElectron->at(i) << endl;
            cout << "passConvVeto->at(i) " << inEEPs.passConvVeto->at(i) << endl;
        }
        currElecPassCut = ElectronPassCutStage1(inEEPs.ElecisEB->at(i), inEEPs.ElecisEE->at(i), currElecSCEta, currElecDeltaPhiIn, currElecDeltaEtaIn, currElecSigIetaIeta, currElecHtoERatio);    
        if (levelVerbosity > 0) {
            cout << "continuing for electron " << i << endl;
            cout << "pass stage 1? " << currElecPassCut << endl;
        }
        currElecPassCut &= ElectronPassCutStage2(inEEPs.isPFElectron->at(i), inEEPs.passConvVeto->at(i), currRelPFElecIso, currElecIP, currElecDZ, currElecECalE, currElecSCEOverP, currElecNumMissHits);
        if (levelVerbosity > 0) {
            cout << "continuing for electron " << i << endl;
            cout << "pass stage 1 & 2? " << currElecPassCut << endl;
        }        
        if (currElecPassCut) {
            patsyLep.P4 = patsyVec;
            patsyLep.PDGID = inEEPs.ElecCharge->at(i) > 0 ? -11 : 11;
            patsyLep.relPFLepIso = currRelPFElecIso;
            patsyLep.BVC.ExtractParams(&patsyLep.P4);
//            cout << "patsyElecEn " << patsyLep.BVC.Vec_En << endl;
            vecIsoLeptons->push_back(patsyLep);
        }
        else {
            continue;
        }
    }
}
inline void ElectronPickOviSyst(ElectronEventPointers inEEPs, float eventRhoIso, int whichSystCase, vector<Lepton> * vecIsoLeptons, int levelVerbosity, vector<Lepton> * vecIsoLeptons_CentVal) {
    
    // see above but adds systematic scale shifts -- note also keeps central value versions of the shifted leptons for MET shifting purposes
    
    bool currElecPassCut;
    float currElecPt;
    float currRelPFElecIso;
    float currElecECalE, currElecSCEOverP, currElecSCEta;
    float currElecIP, currElecDZ;
    float currElecDeltaPhiIn, currElecDeltaEtaIn, currElecSigIetaIeta, currElecHtoERatio;
    int currElecNumMissHits;
    Lepton patsyLep; 
    TLorentzVector patsyVec, patsyVec2;
    for (unsigned int i = 0; i < inEEPs.numElectrons; ++i) {
        patsyVec.SetPxPyPzE(inEEPs.ElecPx->at(i), inEEPs.ElecPy->at(i), inEEPs.ElecPz->at(i), inEEPs.ElecEn->at(i));
        currElecPt = patsyVec.Pt();
        if (fabs(currElecPt - inEEPs.PFElecPt->at(i)) >= 10) continue;
        patsyVec2.SetPxPyPzE(inEEPs.ElecPx->at(i), inEEPs.ElecPy->at(i), inEEPs.ElecPz->at(i), inEEPs.ElecEn->at(i));
        if (whichSystCase != 0) {
            patsyVec = LeptonScaleSystShift(patsyVec, 11, whichSystCase);
            currElecPt = patsyVec.Pt();
        }
        currElecECalE = inEEPs.ElecECalE->at(i); currElecSCEOverP = inEEPs.ElecSCEOverP->at(i); currElecSCEta = inEEPs.ElecSCEta->at(i);
        currRelPFElecIso = (inEEPs.ElecPFCharHadIso->at(i) + TMath::Max(0., inEEPs.ElecPFNeutHadIso->at(i) +  inEEPs.ElecPFPhotIso->at(i) - (eventRhoIso * ElectronEffectiveArea(currElecSCEta))))/currElecPt;
        currElecDZ = inEEPs.ElecDZ->at(i); currElecIP = inEEPs.ElecIP->at(i);
        currElecDeltaPhiIn = inEEPs.ElecDeltaPhiIn->at(i); currElecDeltaEtaIn = inEEPs.ElecDeltaEtaIn->at(i); 
        currElecSigIetaIeta = inEEPs.ElecSigIetaIeta->at(i); currElecHtoERatio = inEEPs.ElecHtoERatio->at(i);
        currElecNumMissHits = inEEPs.ElecNumMissHits->at(i);
        currElecPassCut = ElectronPassCutStage1(inEEPs.ElecisEB->at(i), inEEPs.ElecisEE->at(i), currElecSCEta, currElecDeltaPhiIn, currElecDeltaEtaIn, currElecSigIetaIeta, currElecHtoERatio);
        currElecPassCut &= ElectronPassCutStage2(inEEPs.isPFElectron->at(i), inEEPs.passConvVeto->at(i), currRelPFElecIso, currElecIP, currElecDZ, currElecECalE, currElecSCEOverP, currElecNumMissHits);
        
        if (currElecPassCut) {
            patsyLep.P4 = patsyVec;
            patsyLep.PDGID = inEEPs.ElecCharge->at(i) > 0 ? -11 : 11;
            patsyLep.relPFLepIso = currRelPFElecIso;
            patsyLep.BVC.ExtractParams(&patsyLep.P4);
//            cout << "patsyElecEn " << patsyLep.BVC.Vec_En << endl;
            vecIsoLeptons->push_back(patsyLep);
            patsyLep.P4 = patsyVec2;
            patsyLep.BVC.ExtractParams(&patsyLep.P4);
//            cout << "patsyElecEnCentVal " << patsyLep.BVC.Vec_En << endl;
            vecIsoLeptons_CentVal->push_back(patsyLep);
        }
        else {
            continue;
        }
    }
}
inline void ElectronPickOviNew(ElectronEventPointers inEEPs, float eventRhoIso, vector<Lepton> * vecIsoLeptons, ElecCutInfo * inECITight, bool keepLoose = false, ElecCutInfo * inECILoose = 0, int levelVerbosity = 0, int whichSystCase = 0, vector<Lepton> * vecIsoLeptons_CentVal = 0) {    
    // see above but adds systematic scale shifts -- note also keeps central value versions of the shifted muons for MET shifting purposes
    int levelElQualToKeep = keepLoose ? 0 : 1;
    Lepton patsyLep, patsyLep_CentVal;    
    for (unsigned int iEl = 0; iEl < inEEPs.numElectrons; ++iEl) {
        inEEPs.SetElec(iEl, &patsyLep, eventRhoIso, inECITight, keepLoose, inECILoose, levelVerbosity, whichSystCase);
        if (patsyLep.lepQuality >= levelElQualToKeep) {
            vecIsoLeptons->push_back(patsyLep);            
            if (whichSystCase != 0) {
                patsyLep_CentVal = patsyLep;
                inEEPs.SetElecP4(iEl, &patsyLep_CentVal.P4);
                vecIsoLeptons_CentVal->push_back(patsyLep_CentVal);
            }
        }
    }
}


inline float goodVertexSelection(vector<float> * VertZ, vector<float> * VertRho, vector<float> * VertNDOF, vector<bool> * VertIsFake, int &nGoodVertices) {
    // name says it all -- select good vertices
    
    float firstGoodVertZ = -99;
    nGoodVertices = 0;
    for (unsigned int iVert = 0; iVert < VertZ->size(); ++iVert) {
        /*
        cout << "VertZ->at(iVert) " << VertZ->at(iVert) << endl;
        cout << "fabs(VertZ->at(iVert)) < 24 " << (fabs(VertZ->at(iVert)) < 24) << endl;
        cout << "VertRho   ->at(iVert) " << VertRho   ->at(iVert) << endl;
        cout << "VertRho   ->at(iVert)  <  2 " << (VertRho   ->at(iVert)  <  2) << endl;
        cout << "VertIsFake ->at(iVert) " << VertIsFake ->at(iVert) << endl;
        cout << "!VertIsFake ->at(iVert) " << (!VertIsFake ->at(iVert)) << endl;
        cout << " VertNDOF  ->at(iVert) " << VertNDOF  ->at(iVert) << endl;
        cout << " VertNDOF  ->at(iVert)  >  4 " << (VertNDOF  ->at(iVert)  >  4) << endl;
         */
        if (fabs(VertZ->at(iVert)) < 24 &&
            VertRho   ->at(iVert)  <  2 &&
            VertNDOF  ->at(iVert)  >  4 &&
            !VertIsFake ->at(iVert)) {
            //cout << " passed vertex cut! " << endl;
            ++nGoodVertices;
            if (nGoodVertices == 1) firstGoodVertZ = VertZ->at(iVert);
        }
    }
    //cout << "number of good vertices " << nGoodVertices << endl;
    return firstGoodVertZ;
}
inline bool MuonPassCutStage1(bool isGMPTMuon, bool isPFMuon, bool isGlobMuon, float relPFMuonIso, float muonEta) {    
    // function that says if muon passes cuts we put on it
    // nominally would like to move this function into the MuonEventPointers struct

    
    bool muonPassCut;
    float muonIsoRatioCut = 0.15; float muonEtaCut = 2.4;
//    if (!isGMPTMuon || !isPFMuon || (!isGlobMuon && !isTrackArbitMuon) || !(relPFMuonIso < muonIsoRatioCut) || !(fabs(muonEta) < muonEtaCut)) {
    if (!isGMPTMuon || !isPFMuon || !isGlobMuon || !(relPFMuonIso < muonIsoRatioCut) || !(fabs(muonEta) < muonEtaCut)) {
        muonPassCut = false;
    }
    else {
        muonPassCut = true;
    }
    return muonPassCut;
}
inline bool MuonPassCutStage2(float muonD0, float muonDZ, int muonNumMatchStations, int muonNumValidPixHitsinTrack, int muonNumLayers) {
    // function that says if muon passes cuts we put on it
    // nominally would like to move this function into the MuonEventPointers struct
    float muonD0Cut = 0.2; float muonDZCut = 0.5;
    int muonNumMatchStationsCut = 1, muonNumValidPixHitsinTrackCut = 0, muonNumLayersCut = 5;
    bool muonPassCut;
    if (!(muonD0 < muonD0Cut) || !(muonDZ < muonDZCut) || !(muonNumMatchStations > muonNumMatchStationsCut) || !(muonNumLayers > muonNumLayersCut) || !(muonNumValidPixHitsinTrack > muonNumValidPixHitsinTrackCut)) {
        muonPassCut = false;
    }
    else {
        muonPassCut = true;
    }
    return muonPassCut;
}
inline bool MuonPassCutDESY(float relPFMuonIso, float muonEta) {
    // function that says if muon passes cuts we put on it -- deprecated as was only used with DESY nTuples
    bool muonPassCut;
    float muonIsoRatioCut = 0.15; float muonEtaCut = 2.4;
    if (!(relPFMuonIso < muonIsoRatioCut) || !(fabs(muonEta) < muonEtaCut)) {
        muonPassCut = false;
    }
    else {
        muonPassCut = true;
    }
    return muonPassCut;
}
inline void MuonPickOvi(MuonEventPointers inMEPs, float firstGoodVertZ, vector<Lepton> * vecIsoLeptons, int levelVerbosity) {
    // runs through the vectors of muon information (contained in the input MuonEventPointers struct) and selects good muons
    
    int currMuonNumMatchStations, currMuonNumValidPixHitsinTrack, currMuonNumLayers;
    float currMuonPt, currMuonEta;
    float currMuonRelPFIso;
    float currMuonDZ, currMuonD0;
    bool  currMuonPassCut;
    Lepton patsyLep;
    
    TLorentzVector patsyVec;
    for (unsigned int i = 0; i < inMEPs.numMuons; ++i) {
        //          https://twiki.cern.ch/twiki/bin/viewauth/CMS/SingleLepton2012#Muon_Selection
        //          https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId
        patsyVec.SetPxPyPzE(inMEPs.MuonPx->at(i), inMEPs.MuonPy->at(i), inMEPs.MuonPz->at(i), inMEPs.MuonEn->at(i));
        currMuonPt = patsyVec.Pt();
        if (levelVerbosity > 0) {
            cout << "muon " << i << " has:" << endl;
            cout << "pT is " << currMuonPt << endl;
            cout << "Eta is " << patsyVec.Eta() << endl;
            cout << "Phi is " << patsyVec.Phi() << endl;
            cout << "PF pT is " << inMEPs.PFMuonPt->at(i) << endl;
        }
        //        if (fabs(currMuonPt - PFMuonPt->at(i)) >= 5.) continue;
        if (fabs(inMEPs.MuonPt->at(i) - inMEPs.PFMuonPt->at(i)) >= 5.) continue;
        currMuonEta = patsyVec.Eta();
        currMuonRelPFIso = (inMEPs.MuonPFCharHadIso->at(i) + TMath::Max(0., inMEPs.MuonPFPhotIso->at(i) + inMEPs.MuonPFNeutHadIso->at(i) - 0.5 * inMEPs.MuonSumPUPt->at(i)))/currMuonPt;
//        currMuonRelPFIso /= currMuonPt;
        currMuonDZ = fabs(firstGoodVertZ - inMEPs.MuonVertZ->at(i));        
        currMuonD0 = inMEPs.MuonD0->at(i);
        currMuonNumMatchStations = inMEPs.MuonNumMatchStations->at(i);
        currMuonNumLayers = inMEPs.MuonNumLayers->at(i);
        currMuonNumValidPixHitsinTrack = inMEPs.MuonNumValidPixHitsinTrack->at(i);
        
        if (levelVerbosity > 0) {
            cout << "continuing for muon " << i << endl;
            cout << "currMuonRelPFIso " << currMuonRelPFIso << endl;
            cout << "currMuonDZ " << currMuonDZ << endl;
            cout << "^^ coming from firstGoodVertZ = " << firstGoodVertZ << " and MuonVertZ->at(i) = " << inMEPs.MuonVertZ->at(i) << endl;
            cout << "currMuonD0 " << currMuonD0 << endl;
            cout << "currMuonNumMatchStations " << currMuonNumMatchStations << endl;
            cout << "currMuonNumLayers " << currMuonNumLayers << endl;
            cout << "currMuonNumValidPixHitsinTrack " << currMuonNumValidPixHitsinTrack << endl;
            cout << "isGMPTMuon->at(i) " << inMEPs.isGMPTMuon->at(i) << endl;
            cout << "isPFMuon->at(i) " << inMEPs.isPFMuon->at(i) << endl;
            cout << "isGlobMuon->at(i) " << inMEPs.isGlobMuon->at(i) << endl;
        }
        
        currMuonPassCut = MuonPassCutStage1(inMEPs.isGMPTMuon->at(i), inMEPs.isPFMuon->at(i), inMEPs.isGlobMuon->at(i), currMuonRelPFIso, currMuonEta);
        if (levelVerbosity > 0) {
            cout << "continuing for muon " << i << endl;
            cout << "pass stage 1? " << currMuonPassCut << endl;
        }
        currMuonPassCut &= MuonPassCutStage2(currMuonD0, currMuonDZ, currMuonNumMatchStations, currMuonNumValidPixHitsinTrack, currMuonNumLayers);
        if (levelVerbosity > 0) {
            cout << "continuing for muon " << i << endl;
            cout << "pass stage 1 & 2? " << currMuonPassCut << endl;
        }
        if (currMuonPassCut){
            patsyLep.P4 = patsyVec;
            patsyLep.PDGID = inMEPs.MuonCharge->at(i) > 0 ? -13 : 13;
            patsyLep.relPFLepIso = currMuonRelPFIso;
            patsyLep.BVC.ExtractParams(&patsyLep.P4);
//            cout << "patsyLepEn " << patsyLep.BVC.Vec_En << endl;
            vecIsoLeptons->push_back(patsyLep);
        }
        else {
            continue;
        }
    }
}   
inline void MuonPickOviSyst(MuonEventPointers inMEPs, float firstGoodVertZ, int whichSystCase, vector<Lepton> * vecIsoLeptons, int levelVerbosity, vector<Lepton> * vecIsoLeptons_CentVal) {    
    // see above but adds systematic scale shifts -- note also keeps central value versions of the shifted muons for MET shifting purposes
    
    
    int currMuonNumMatchStations, currMuonNumValidPixHitsinTrack, currMuonNumLayers;
    float currMuonPt, currMuonEta;
    float currMuonRelPFIso;
    float currMuonDZ, currMuonD0;
    bool  currMuonPassCut;  
    Lepton patsyLep;

    TLorentzVector patsyVec, patsyVec2;        
    for (unsigned int i = 0; i < inMEPs.numMuons; ++i) {
        patsyVec.SetPxPyPzE(inMEPs.MuonPx->at(i), inMEPs.MuonPy->at(i), inMEPs.MuonPz->at(i), inMEPs.MuonEn->at(i));
        patsyVec2.SetPxPyPzE(inMEPs.MuonPx->at(i), inMEPs.MuonPy->at(i), inMEPs.MuonPz->at(i), inMEPs.MuonEn->at(i));
        //        if (fabs(patsyVec.Pt() - PFMuonPt->at(i)) >= 5.) continue;
        
        if (levelVerbosity > 0) {
            cout << "muon " << i << " has:" << endl;
            cout << "pT is " << patsyVec.Pt() << endl;
            cout << "Eta is " << patsyVec.Eta() << endl;
            cout << "Phi is " << patsyVec.Phi() << endl;
            cout << "PF pT is " << inMEPs.PFMuonPt->at(i) << endl;
        }
        
        if (fabs(inMEPs.MuonPt->at(i) - inMEPs.PFMuonPt->at(i)) >= 5.) continue; // want to check before shifting energy scale around
        if (whichSystCase != 0) {
            patsyVec = LeptonScaleSystShift(patsyVec, 13, whichSystCase);
        }    
        currMuonPt = patsyVec.Pt();
        currMuonEta = patsyVec.Eta();
        currMuonRelPFIso = (inMEPs.MuonPFCharHadIso->at(i) + TMath::Max(0., inMEPs.MuonPFPhotIso->at(i) + inMEPs.MuonPFNeutHadIso->at(i) - 0.5 * inMEPs.MuonSumPUPt->at(i)))/currMuonPt;
//        currMuonRelPFIso /= currMuonPt;
        currMuonDZ = fabs(firstGoodVertZ - inMEPs.MuonVertZ->at(i));
        currMuonD0 = inMEPs.MuonD0->at(i);
        currMuonNumMatchStations = inMEPs.MuonNumMatchStations->at(i);
        currMuonNumLayers = inMEPs.MuonNumLayers->at(i);
        currMuonNumValidPixHitsinTrack = inMEPs.MuonNumValidPixHitsinTrack->at(i);
        if (levelVerbosity > 0) {
            cout << "continuing for muon " << i << endl;
            cout << "currMuonRelPFIso " << currMuonRelPFIso << endl;
            cout << "currMuonDZ " << currMuonDZ << endl;
            cout << "^^ coming from firstGoodVertZ = " << firstGoodVertZ << " and MuonVertZ->at(i) = " << inMEPs.MuonVertZ->at(i) << endl;
            cout << "currMuonD0 " << currMuonD0 << endl;
            cout << "currMuonNumMatchStations " << currMuonNumMatchStations << endl;
            cout << "currMuonNumLayers " << currMuonNumLayers << endl;
            cout << "currMuonNumValidPixHitsinTrack " << currMuonNumValidPixHitsinTrack << endl;
            cout << "isGMPTMuon->at(i) " << inMEPs.isGMPTMuon->at(i) << endl;
            cout << "isPFMuon->at(i) " << inMEPs.isPFMuon->at(i) << endl;
            cout << "isGlobMuon->at(i) " << inMEPs.isGlobMuon->at(i) << endl;
        }        
        currMuonPassCut = MuonPassCutStage1(inMEPs.isGMPTMuon->at(i), inMEPs.isPFMuon->at(i), inMEPs.isGlobMuon->at(i), currMuonRelPFIso, currMuonEta);
        if (levelVerbosity > 0) {
            cout << "continuing for muon " << i << endl;
            cout << "pass stage 1? " << currMuonPassCut << endl;
        }
        currMuonPassCut &= MuonPassCutStage2(currMuonD0, currMuonDZ, currMuonNumMatchStations, currMuonNumValidPixHitsinTrack, currMuonNumLayers);
        if (levelVerbosity > 0) {
            cout << "continuing for muon " << i << endl;
            cout << "pass stage 1 & 2? " << currMuonPassCut << endl;
        }
        if (currMuonPassCut){
            patsyLep.P4 = patsyVec;
            patsyLep.PDGID = inMEPs.MuonCharge->at(i) > 0 ? -13 : 13;
            patsyLep.relPFLepIso = currMuonRelPFIso;
            patsyLep.BVC.ExtractParams(&patsyLep.P4);
//            cout << "patsyLepEn " << patsyLep.BVC.Vec_En << endl;
            vecIsoLeptons->push_back(patsyLep);
            //            vecIsoLeptons->push_back(patsyVec);
            patsyLep.P4 = patsyVec2;
            patsyLep.BVC.ExtractParams(&patsyLep.P4);
//            cout << "patsyLepCentValEn " << patsyLep.BVC.Vec_En << endl;
            vecIsoLeptons_CentVal->push_back(patsyLep);
        }
        else {
            continue;
        }
    }
}




inline void MuonPickOviNew(MuonEventPointers inMEPs, float firstGoodVertZ, vector<Lepton> * vecIsoLeptons, bool keepLoose = false, int levelVerbosity = 0, int whichSystCase = 0, vector<Lepton> * vecIsoLeptons_CentVal = 0) {    
    // see above but adds systematic scale shifts -- note also keeps central value versions of the shifted muons for MET shifting purposes
    int levelMuQualToKeep = keepLoose ? 0 : 1;
    Lepton patsyLep, patsyLep_CentVal;    
    for (unsigned int iMu = 0; iMu < inMEPs.numMuons; ++iMu) {
        inMEPs.SetMuon(iMu, &patsyLep, firstGoodVertZ, keepLoose, levelVerbosity, whichSystCase);
        if (patsyLep.lepQuality >= levelMuQualToKeep) {
            vecIsoLeptons->push_back(patsyLep);            
            if (whichSystCase != 0) {
                patsyLep_CentVal = patsyLep;
                inMEPs.SetMuonP4(iMu, &patsyLep_CentVal.P4);
                vecIsoLeptons_CentVal->push_back(patsyLep_CentVal);
            }
        }
    }
}

inline void IsoLeptonsPickDESY(VLV * Leptons, vector<int> *lepPdgId, vector<double> *lepPFIso, int whichSystCase, vector<Lepton> * vecIsoLeptons, vector<Lepton> * vecIsoLeptons_CentVal) {    
    // deprecated -- relevant for DESY
    
    unsigned int vecSize = Leptons->size();
    if (vecSize < 2) {
        return;
    }
    TLorentzVector patsyVec, patsyVec2;
    float currLepEta, currLepRelPFIso;
    int   currLepPDGID;
    Lepton patsyLep;
    bool  currLepPassCut = false;    
    for (unsigned int iLep = 0; iLep < vecSize; ++iLep) {
        patsyVec.SetPxPyPzE(Leptons->at(iLep).Px(), Leptons->at(iLep).Py(), Leptons->at(iLep).Pz(), Leptons->at(iLep).E());
        patsyVec2.SetPxPyPzE(Leptons->at(iLep).Px(), Leptons->at(iLep).Py(), Leptons->at(iLep).Pz(), Leptons->at(iLep).E());
        currLepEta = patsyVec.Eta();
        currLepRelPFIso = lepPFIso->at(iLep);
        currLepPDGID = lepPdgId->at(iLep);        
        if (whichSystCase != 0) {
            patsyVec = LeptonScaleSystShift(patsyVec, abs(currLepPDGID), whichSystCase);
        }
        if (abs(currLepPDGID) == 13) {
            currLepPassCut = MuonPassCutDESY(currLepRelPFIso, currLepEta);
        }
        else if (abs(currLepPDGID) == 11) {
            currLepPassCut = ElectronPassCutDESY(currLepRelPFIso, currLepEta);
        }
        if (currLepPassCut) {            
            patsyLep.P4 = patsyVec;
            patsyLep.PDGID = currLepPDGID;
            patsyLep.relPFLepIso = currLepRelPFIso;
            vecIsoLeptons->push_back(patsyLep);
            patsyLep.P4 = patsyVec2;
            vecIsoLeptons_CentVal->push_back(patsyLep);
        }
        else {
            continue;
        }
    }
}

inline void IsoLeptonsPickDESY(VLV * Leptons, vector<int> *lepPdgId, vector<double> *lepPFIso, vector<Lepton> * vecIsoLeptons) {    
    // deprecated -- relevant for DESY
    unsigned int vecSize = Leptons->size();
    if (vecSize < 2) {
        return;
    }
    TLorentzVector patsyVec;
    float currLepEta, currLepRelPFIso;
    int   currLepPDGID;
    bool  currLepPassCut = false;  
    Lepton patsyLep;
    for (unsigned int iLep = 0; iLep < vecSize; ++iLep) {
        patsyVec.SetPxPyPzE(Leptons->at(iLep).Px(), Leptons->at(iLep).Py(), Leptons->at(iLep).Pz(), Leptons->at(iLep).E());
        currLepEta = patsyVec.Eta();
        currLepRelPFIso = lepPFIso->at(iLep);
        currLepPDGID = lepPdgId->at(iLep);
        if (abs(currLepPDGID) == 13) {
            currLepPassCut = MuonPassCutDESY(currLepRelPFIso, currLepEta);
        }
        else if (abs(currLepPDGID) == 11) {
            currLepPassCut = ElectronPassCutDESY(currLepRelPFIso, currLepEta);
        }
        if (currLepPassCut) {
            patsyLep.P4 = patsyVec;
            patsyLep.PDGID = currLepPDGID;
            patsyLep.relPFLepIso = currLepRelPFIso;
            vecIsoLeptons->push_back(patsyLep);
        }
        else {
            continue;
        }
    }
}
inline EventLepInfo LeptonPair(vector<Lepton> * vecIsoLeptons, int levelVerbosity) {
    // tries to find a suitable pairing of opposite sign leptons that matches our selection
    
    EventLepInfo outELI; outELI.ELIDefaultVarVals();
    int lep0Index = -1; int lep1Index = -1;
    int vecSize = vecIsoLeptons->size();
    if (levelVerbosity > 0) {
        cout << endl;
        cout << "starting on trying to pair up leptons " << endl;
        cout << "vecSize " << vecSize << endl;   
    }
    if (vecSize < 2) {
        outELI.EventFails();
        return outELI;
    }
    float leadLepPtCut = 20., subLepPtCut = 10.;
    float currLeadLepPt, currSubLepPt;
    float currLeadLepEta, currSubLepEta;
    float currLeadLepCharge, currSubLepCharge;
    int   currLeadLepPDGID, currSubLepPDGID;
    int   productPdgId;
    float currDiLepPt;
    float leadDiLepPt = 0;
    float diLepmassCut = 20.;
    for (int iLep = 0; iLep < vecSize; ++iLep) {
        currLeadLepPt = vecIsoLeptons->at(iLep).P4.Pt();
        currLeadLepEta = vecIsoLeptons->at(iLep).P4.Eta();
        currLeadLepPDGID = vecIsoLeptons->at(iLep).PDGID;
        currLeadLepCharge = TMath::Sign(1, currLeadLepPDGID);
        if (currLeadLepPDGID == -13) {
            if (currLeadLepPt > leadLepPtCut) {
                outELI.ELI_LC.EventNIsoMubars_pT20++;
            }
            else if (currLeadLepPt > subLepPtCut) {
                outELI.ELI_LC.EventNIsoMubars_pT10to20++;
            }
        }
        else if (currLeadLepPDGID == 13) {
            if (currLeadLepPt > leadLepPtCut) {
                outELI.ELI_LC.EventNIsoMuons_pT20++;
            }
            else if (currLeadLepPt > subLepPtCut) {
                outELI.ELI_LC.EventNIsoMuons_pT10to20++;
            }
        }
        else if (currLeadLepPDGID == 11) {
            if (currLeadLepPt > leadLepPtCut) {
                outELI.ELI_LC.EventNIsoElecs_pT20++;
            }
            else if (currLeadLepPt > subLepPtCut) {
                outELI.ELI_LC.EventNIsoElecs_pT10to20++;
            }
        }
        else if (currLeadLepPDGID == -11) {
            if (currLeadLepPt > leadLepPtCut) {
                outELI.ELI_LC.EventNIsoPosits_pT20++;
            }
            else if (currLeadLepPt > subLepPtCut) {
                outELI.ELI_LC.EventNIsoPosits_pT10to20++;
            }
        }
        if (levelVerbosity > 0) {
            cout << "For attempted leading lepton, vector position " << iLep << endl;
            cout << "currLeadLepPt = " << currLeadLepPt << endl;
            cout << "currLeadLepEta = " << currLeadLepEta << endl;
            cout << "currLeadLepPhi = " << vecIsoLeptons->at(iLep).P4.Phi() << endl;
            cout << "currLeadLepPDGID = " << currLeadLepPDGID << endl;
            cout << "currLeadLepCharge = " << currLeadLepCharge << endl;
        }
        if (currLeadLepPt < leadLepPtCut) continue;
        for (int iLep2 = iLep+1; iLep2 < vecSize; ++iLep2) {
            currSubLepPt = vecIsoLeptons->at(iLep2).P4.Pt();
            currSubLepEta = vecIsoLeptons->at(iLep2).P4.Eta();
            currSubLepPDGID = vecIsoLeptons->at(iLep2).PDGID;
            currSubLepCharge = TMath::Sign(1, currSubLepPDGID);
            if (levelVerbosity > 0) {
                cout << "For attempted leading lepton, vector position " << iLep2 << endl;
                cout << "currSubLepPt = " << currSubLepPt << endl;
                cout << "currSubLepEta = " << currSubLepEta << endl;
                cout << "currSubLepPhi = " << vecIsoLeptons->at(iLep2).P4.Phi() << endl;
                cout << "currSubLepPDGID = " << currSubLepPDGID << endl;
                cout << "currSubLepCharge = " << currSubLepCharge << endl;
            }
            if (currSubLepPt < subLepPtCut) continue;
            if (currLeadLepCharge * currSubLepCharge > 0) continue;//i.e. same sign event
            outELI.ELI_LC.EventNViableLepPairsPreMassCut++;
            if (levelVerbosity > 0) {
                productPdgId = currLeadLepPDGID * currSubLepPDGID;
                float currPairMass = (vecIsoLeptons->at(iLep).P4 + vecIsoLeptons->at(iLep2).P4).M();
                currDiLepPt = currLeadLepPt + currSubLepPt;
                cout << "for iLep = " << iLep << " and iLep2 = " << iLep2 << endl;
                cout << "productPdgId = " << productPdgId << endl;
                cout << "currPairMass = " << currPairMass << endl;
                cout << "currDiLepPt = " << currDiLepPt << endl;
            }
            if ((vecIsoLeptons->at(iLep).P4 + vecIsoLeptons->at(iLep2).P4).M() < diLepmassCut) continue;
            outELI.ELI_LC.EventNViableLepPairsPostMassCut++;
            currDiLepPt = currLeadLepPt + currSubLepPt;            
            if (currDiLepPt > leadDiLepPt) {
                leadDiLepPt = currDiLepPt;
                lep0Index = iLep;
                lep1Index = iLep2;
            }
        }
    }
    if (levelVerbosity) {
        cout << "lep0index in isolated leptons vector " << lep0Index << endl;
        cout << "lep1index in isolated leptons vector " << lep1Index << endl;
    }
    if (lep0Index < 0 || lep1Index < 0) {
        outELI.EventFails();
    }
    else {
        vector<int> indices; 
        indices.push_back(lep0Index);
        indices.push_back(lep1Index);
        outELI.EventPasses(&indices, vecIsoLeptons);
    }
    if (levelVerbosity) {
        cout << "Lep0PdgId " << outELI.vecEventLeps[0].PDGID << endl;
        cout << "Lep1PdgId " << outELI.vecEventLeps[1].PDGID << endl;
        cout << "Dilepton Event Type " << outELI.EventDiLepType << endl;
    }
    return outELI;
}







inline void LeptonPair(EventLepInfo * inELI, vector<Lepton> * vecIsoLeptons, int levelVerbosity) {
    //tries to find a suitable pairing of opposite sign leptons that matches our selection
    
    
    int lep0Index = -1; int lep1Index = -1;
    int vecSize = vecIsoLeptons->size();
    if (levelVerbosity > 0) {
        cout << endl;
        cout << "starting on trying to pair up leptons " << endl;
        cout << "vecSize " << vecSize << endl;   
    }    
    if (vecSize < 2) {
        inELI->EventFails();
    }
    float leadLepPtCut = 20., subLepPtCut = 10.;
    float currLeadLepPt, currSubLepPt;
    float currLeadLepEta, currSubLepEta;
    float currLeadLepCharge, currSubLepCharge;
    int   currLeadLepPDGID, currSubLepPDGID;
    int   productPdgId;
    float currDiLepPt;
    float leadDiLepPt = 0;
    float diLepmassCut = 20.;
    for (int iLep = 0; iLep < vecSize; ++iLep) {
        currLeadLepPt = vecIsoLeptons->at(iLep).P4.Pt();
        currLeadLepEta = vecIsoLeptons->at(iLep).P4.Eta();
        currLeadLepPDGID = vecIsoLeptons->at(iLep).PDGID;
        currLeadLepCharge = TMath::Sign(1, currLeadLepPDGID);
        if (currLeadLepPDGID == -13) {
            if (currLeadLepPt > leadLepPtCut) {
                inELI->ELI_LC.EventNIsoMubars_pT20++;
            }
            else if (currLeadLepPt > subLepPtCut) {
                inELI->ELI_LC.EventNIsoMubars_pT10to20++;
            }
        }
        else if (currLeadLepPDGID == 13) {
            if (currLeadLepPt > leadLepPtCut) {
                inELI->ELI_LC.EventNIsoMuons_pT20++;
            }
            else if (currLeadLepPt > subLepPtCut) {
                inELI->ELI_LC.EventNIsoMuons_pT10to20++;
            }
        }
        else if (currLeadLepPDGID == 11) {
            if (currLeadLepPt > leadLepPtCut) {
                inELI->ELI_LC.EventNIsoElecs_pT20++;
            }
            else if (currLeadLepPt > subLepPtCut) {
                inELI->ELI_LC.EventNIsoElecs_pT10to20++;
            }
        }
        else if (currLeadLepPDGID == -11) {
            if (currLeadLepPt > leadLepPtCut) {
                inELI->ELI_LC.EventNIsoPosits_pT20++;
            }
            else if (currLeadLepPt > subLepPtCut) {
                inELI->ELI_LC.EventNIsoPosits_pT10to20++;
            }
        }
        if (levelVerbosity > 0) {
            cout << "For attempted leading lepton, vector position " << iLep << endl;
            cout << "currLeadLepPt = " << currLeadLepPt << endl;
            cout << "currLeadLepEta = " << currLeadLepEta << endl;
            cout << "currLeadLepPhi = " << vecIsoLeptons->at(iLep).P4.Phi() << endl;
            cout << "currLeadLepPDGID = " << currLeadLepPDGID << endl;
            cout << "currLeadLepCharge = " << currLeadLepCharge << endl;
        }
        if (currLeadLepPt < leadLepPtCut) continue;
        for (int iLep2 = iLep+1; iLep2 < vecSize; ++iLep2) {
            currSubLepPt = vecIsoLeptons->at(iLep2).P4.Pt();
            currSubLepEta = vecIsoLeptons->at(iLep2).P4.Eta();
            currSubLepPDGID = vecIsoLeptons->at(iLep2).PDGID;
            currSubLepCharge = TMath::Sign(1, currSubLepPDGID);
            if (levelVerbosity > 0) {
                cout << "For attempted leading lepton, vector position " << iLep2 << endl;
                cout << "currSubLepPt = " << currSubLepPt << endl;
                cout << "currSubLepEta = " << currSubLepEta << endl;
                cout << "currSubLepPhi = " << vecIsoLeptons->at(iLep2).P4.Phi() << endl;
                cout << "currSubLepPDGID = " << currSubLepPDGID << endl;
                cout << "currSubLepCharge = " << currSubLepCharge << endl;
            }
            if (currSubLepPt < subLepPtCut) continue;
            if (currLeadLepCharge * currSubLepCharge > 0) continue;//i.e. same sign event
            inELI->ELI_LC.EventNViableLepPairsPreMassCut++;
            if (levelVerbosity > 0) {
                productPdgId = currLeadLepPDGID * currSubLepPDGID;
                float currPairMass = (vecIsoLeptons->at(iLep).P4 + vecIsoLeptons->at(iLep2).P4).M();
                currDiLepPt = currLeadLepPt + currSubLepPt;
                cout << "for iLep = " << iLep << " and iLep2 = " << iLep2 << endl;
                cout << "productPdgId = " << productPdgId << endl;
                cout << "currPairMass = " << currPairMass << endl;
                cout << "currDiLepPt = " << currDiLepPt << endl;
            }
            if ((vecIsoLeptons->at(iLep).P4 + vecIsoLeptons->at(iLep2).P4).M() < diLepmassCut) continue;
            inELI->ELI_LC.EventNViableLepPairsPostMassCut++;
            currDiLepPt = currLeadLepPt + currSubLepPt;            
            if (currDiLepPt > leadDiLepPt) {
                leadDiLepPt = currDiLepPt;
                lep0Index = iLep;
                lep1Index = iLep2;
            }
        }
    }
    if (levelVerbosity) {
        cout << "lep0index in isolated leptons vector " << lep0Index << endl;
        cout << "lep1index in isolated leptons vector " << lep1Index << endl;
    }
    if (lep0Index < 0 || lep1Index < 0) {
        inELI->EventFails();
    }
    else {
        vector<int> indices; 
        indices.push_back(lep0Index);
        indices.push_back(lep1Index);
        inELI->EventPasses(&indices, vecIsoLeptons);
    }
    if (levelVerbosity) {
        cout << "Lep0PdgId " << inELI->vecEventLeps[0].PDGID << endl;
        cout << "Lep1PdgId " << inELI->vecEventLeps[1].PDGID << endl;
        cout << "Dilepton Event Type " << inELI->EventDiLepType << endl;
    }
}


inline float JESUncertFactor(TH2F * histJES, float shiftDirection, float JetPt, float JetEta) {
    // calculate JESUncertainty factor for a given input Jet Pt and Eta
    
    int NXbins = histJES->GetNbinsX();
    TAxis * XAxis = histJES->GetXaxis();
    TAxis * YAxis = histJES->GetYaxis();
    int binX = XAxis->FindBin(JetPt);
    int binY = YAxis->FindBin(JetEta);
    float uncertFactor;
    float linYPointOne, linYPointTwo, linXPointOne, linXPointTwo, rise, run, slope;
    if (binX >= NXbins) uncertFactor = histJES->GetBinContent(NXbins, binY);
    else if (binX < 1) uncertFactor = histJES->GetBinContent(1, binY);
    else {
        linYPointOne = histJES->GetBinContent(binX, binY);
        linYPointTwo = histJES->GetBinContent(binX + 1, binY);
        linXPointOne = XAxis->GetBinLowEdge(binX);
        linXPointTwo = XAxis->GetBinUpEdge(binX);
        rise = linYPointTwo - linYPointOne;
        run = linXPointTwo - linXPointOne;
        slope = rise / run;
        uncertFactor = linYPointOne + slope * (JetPt - linXPointOne);
    }
    return (1 + shiftDirection * uncertFactor);
}
inline bool isLeptonJet(vector<Lepton> * isoLeptons, TLorentzVector * inputJet) {
    // determines if the input jet is consistent (deltaR sense) with a lepton
    
    float dRcut = 0.4;
    float lepPtCut;// = 10.0;
    float jetPtCutForCutLep = 20.0;
    for (unsigned int iLep = 0; iLep < isoLeptons->size(); ++iLep) {
        lepPtCut = (inputJet->Pt() > jetPtCutForCutLep) ? 10.0: 0.0; // for jets > 20 GeV, check that isoLepton's pT is > 10 GeV, otherwise don't care about pT
        if (isoLeptons->at(iLep).P4.Pt() > lepPtCut) {
            if (deltaR(inputJet->Eta(), inputJet->Phi(), isoLeptons->at(iLep).P4.Eta(), isoLeptons->at(iLep).P4.Phi()) < dRcut) return true;
        }
    }
    return false;
}

inline void JetInfoDESY(vector<PFJet> * inVecPFJet, vector<Lepton> * isoLeptons, VLV * Jets, vector<double> * JetBTag, float whichSystCase, TH2F * shiftHist) {
    //deprecated as it was for DESY nTuples
    
    
    TLorentzVector patsyJetLV;
    PFJet currPFJet;
    bool leptonJet;
    float dRcut = 0.4;
    float JetFactor;
    Double_t currJetPt, currJetEta;
    for (unsigned int iJet = 0; iJet < Jets->size(); ++iJet) {
        leptonJet = 0;        
        patsyJetLV.SetPxPyPzE(Jets->at(iJet).Px(), Jets->at(iJet).Py(), Jets->at(iJet).Pz(), Jets->at(iJet).E());
        currJetPt = patsyJetLV.Pt();
        currJetEta = patsyJetLV.Eta();
        if (isLeptonJet(isoLeptons, &patsyJetLV)) continue;
        if (currJetPt < 9.0) continue;
        if (whichSystCase != 0) {
            JetFactor = JESUncertFactor(shiftHist, whichSystCase, (float) currJetPt, (float) currJetEta);
            patsyJetLV *= JetFactor;
        }
        currPFJet.P4 = patsyJetLV;
        currPFJet.valBTagDisc = (float) JetBTag->at(iJet);
        currPFJet.partonFlavor = -999999;
        currPFJet.BVC.ExtractParams(&currPFJet.P4);
        inVecPFJet->push_back(currPFJet);
    }
}
inline void JetInfo(vector<PFJet> * inVecPFJet, vector<Lepton> * isoLeptons, PFJetEventPointers inPFJEPs, JetCutInfo * inJCI, float whichSystCase, TH2F * shiftHist) {
    // loops over the jets and picks out the good ones which don't overlap with leptons -- can also perform systematic shifts as needed
    
    TLorentzVector patsyJetLV;
    PFJet currPFJet;
    bool leptonJet;
    float dRcut = 0.4;
    float JetFactor;
    Double_t currJetPt, currJetEta;
    for (unsigned int iJet = 0; iJet < inPFJEPs.numPFJets; ++iJet) {
        leptonJet = 0;
        inPFJEPs.SetPFJet(iJet, &currPFJet);
//        currPFJet.SetJetVars(inPFJEPs, iJet);
        currJetPt = currPFJet.P4.Pt();
        
        if (currJetPt < 9.0) continue;
        currJetEta = currPFJet.P4.Eta();
        if (isLeptonJet(isoLeptons, &currPFJet.P4)) continue;
/*
        currPFJet.PrintVals();
        cout <<endl;
        */
        if (currPFJet.PassesID(inJCI)) {
            /*
            cout <<endl;
            cout << "passedID " << endl;
            cout <<endl;
            cout <<endl;
            */
            if (whichSystCase != 0) {
                JetFactor = JESUncertFactor(shiftHist, whichSystCase, (float) currJetPt, (float) currJetEta);
                currPFJet.P4 *= JetFactor;
            }
            currPFJet.BVC.ExtractParams(&currPFJet.P4);
            inVecPFJet->push_back(currPFJet);
        }
        else {
            continue;
        }
    }
}


inline bool GenJetMatch(TLorentzVector * inputJet, TLorentzVector * genJet, float &dRGenReco) {
    // function used to match PF jets to generator level jets
    
    bool isGenJetMatched;
    float dRmaxGenJetMatch = TMath::Min(0.5, 0.1 + 0.3 * TMath::Exp(-0.05 * (genJet->Pt() - 10)));
    dRGenReco = deltaR(inputJet->Eta(), inputJet->Phi(), genJet->Eta(), genJet->Phi());
    isGenJetMatched = (dRGenReco < dRmaxGenJetMatch);
    return isGenJetMatched;
}
inline void MatchGenJet(PFJet * inPFJet, vector<GenJet> * vecGoodGenJets) {
    float dRGenReco, bestdRGenReco = 1.0;
    bool isGenMatched = false;
    float dEnRecoGen;
    inPFJet->ResetGenMatchInfo();
    int bestGenMatchIndex = -1;
    for (unsigned int iGenJet = 0; iGenJet < vecGoodGenJets->size(); ++iGenJet) {
        if (GenJetMatch(&inPFJet->P4, &vecGoodGenJets->at(iGenJet).P4, dRGenReco)) {
            if (bestdRGenReco > dRGenReco) {
                if (!isGenMatched) isGenMatched = true;
                bestGenMatchIndex = iGenJet;
                bestdRGenReco = dRGenReco;
            }
        }
        else {
            continue;
        }
    }            
    if (isGenMatched) {
        dEnRecoGen = inPFJet->P4.E() - vecGoodGenJets->at(bestGenMatchIndex).P4.E();
        inPFJet->SetGenMatchInfo(dEnRecoGen, true);
    }
}
inline vector<GenJet> * GenJetsNonZero(GenJetEventPointers inGJEPs) {
    GenJet currGenJet;
    vector<GenJet> * outGenJetVec = new vector<GenJet>;
    for (unsigned int iGenJet = 0; iGenJet < inGJEPs.numGenJets; ++iGenJet) {
        if (inGJEPs.genJetIsGenJet->at(iGenJet)) {
            currGenJet.P4.SetPxPyPzE(inGJEPs.genJetPx->at(iGenJet), inGJEPs.genJetPy->at(iGenJet), inGJEPs.genJetPz->at(iGenJet), inGJEPs.genJetEn->at(iGenJet));
            currGenJet.invisE = inGJEPs.genJetInvisE->at(iGenJet);
            currGenJet.seedPDGID = 0;
            currGenJet.isBJet = false;
            outGenJetVec->push_back(currGenJet);
        }
    }
    return outGenJetVec;
}
bool InSystBound(int iSyst, vector<int> * systLB, vector<int> * systUB) {
    bool outBool = false;
    for (unsigned int iBound = 0; iBound < systLB->size(); ++iBound) {
        //        cout << "iBound " << iBound << endl;
        if (!outBool) {
            if (iSyst >= systLB->at(iBound) && iSyst <= systUB->at(iBound)) outBool = true;
        }
        else {
            return outBool;
        }
    }
    return outBool;
}

/***********************************************************************************************/
/***********************************************************************************************/
// And now begin some ill-fated attempts at performing jet smearing by myself....ugh so annoying
/***********************************************************************************************/
/***********************************************************************************************/
inline float JetResolutionHighPt(TLorentzVector * inputJet, vector<TF1> * inputTF1Vec) {
    float currJetAbsEta = fabs(inputJet->Eta());
    float EtaBinEnds[10] = {0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 9.9}; // in reality 9 TF1 vecs
    int   currJetEtaIndex = -1;
    bool  cornerCaseBinEdge = false;
    float outResolution;
    for (unsigned int iEta = 0; iEta < 9; ++iEta) {
        if (currJetAbsEta > EtaBinEnds[iEta] && currJetAbsEta < EtaBinEnds[iEta + 1]) {
            currJetEtaIndex = iEta;
            break;
        }
        else if (currJetAbsEta == EtaBinEnds[iEta] && currJetAbsEta > 0) {
            currJetEtaIndex = iEta;
            cornerCaseBinEdge = true;
            cout << "corner case came up, jet eta is " << inputJet->Eta() << endl;
            // you've somehow managed to fall right on the bin edge -- floating point precision -- so take average of two different bins -- currently rolling with straight up mean
        }
    }
    if (currJetEtaIndex < 0) {          
        cout << "can't do this jet!!...technically jet eta is: " << inputJet->Eta() << endl; 
        currJetEtaIndex = 8;        
    }
    cout << "currJetEtaIndex " << currJetEtaIndex << endl;;
    cout << "currJetPt " << inputJet->Pt() << endl;
    outResolution = inputTF1Vec->at(currJetEtaIndex).Eval(inputJet->Pt());
    outResolution *= inputJet->Pt(); // assuming the outResolution is a fractional resolution
    /*
    if (cornerCaseBinEdge) {
        outResolution += inputTF1Vec->at(currJetEtaIndex - 1).Eval(inputJet->Pt());
        outResolution /= 2;
    }
    */
    return outResolution;
}
inline TLorentzVector SmearJet(TLorentzVector * inputJet, vector<GenJet> * vecGoodGenJets, bool &isGenMatched, float &dEnRecoGen, TH2F * GenJetSmearHist, TH2F * RecoJetLowPtSmearHist, vector<TF1> * inputTF1Vec, float smearBy = 1.0, int levelVerbosity = 0) {
    /// Function to smear the jets according to C.V.'s prescription. Look at the produce method for the SmearedJet EDProducer shown here:
    /// http://cmslxr.fnal.gov/lxr/source/PhysicsTools/PatUtils/interface/SmearedJetProducerT.h#207
    /// As of 10/10/13 I am missing the Reco vs. Corr Jet Max function, but otherwise things are in order with what he does
    /// This function basically comes about from tracing down associated configurations and EDProducers from running the runMEtUncertainties.py tool defined here:
    /// http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/PhysicsTools/PatUtils/python/tools/runType1PFMEtUncertainties.py?view=log
    TRandom3 rnd_;
    float x = fabs(inputJet->Eta()), y = inputJet->Pt(); // set up the "x" and "y" to look for in the resolution histograms
    float inEn = inputJet->E();
    TLorentzVector smearJet = *inputJet;    // initializing incase I don't smear jet for some reasons
    if (levelVerbosity > 0) {
        cout << endl;
        cout << "TRYING TO SMEAR JET " << endl;
        cout << "smear jet initial px " << smearJet.Px() << endl;
        cout << "smear jet initial py " << smearJet.Py() << endl;
        cout << "smear jet initial pz " << smearJet.Pz() << endl;
        cout << "smear jet initial E " << smearJet.E() << endl;
        cout << "smear jet initial Pt " << smearJet.Pt() << endl;
    }
    float smearJetEnergy = inEn;            // initializing incase I don't smear jet for some reasons
    
    TLorentzVector currGenJetLV;
    float dRGenReco;
    float inputJetBaseRes, inputJetScaleRes;
    float smearFactor = 1.0, smearFactorErr = 0.0;
    float pTResolThreshold = 10.0, sigmaMaxGenJetMatch = 3.0; 
    float skipRawJetPtThreshold = 10.0, skipCorrJetPtThreshold = 0.01, minSmearJetEn = 0.01;
    int bestGenMatchIndex = -1;
    float bestdRGenReco = 1.0;
    isGenMatched = false;
    
    if (x > GenJetSmearHist->GetXaxis()->GetXmin() && x < GenJetSmearHist->GetXaxis()->GetXmax() && 
        y > GenJetSmearHist->GetYaxis()->GetXmin() && y < GenJetSmearHist->GetYaxis()->GetXmax() ) {
        int binIndexGenJetMatch = GenJetSmearHist->FindBin(x, y);
        smearFactor += smearBy * (GenJetSmearHist->GetBinContent(binIndexGenJetMatch) - 1);
        smearFactorErr = GenJetSmearHist->GetBinError(binIndexGenJetMatch);   
    }
    int binIndexNoGenJetMatch = RecoJetLowPtSmearHist->FindBin(x, y);
    if (levelVerbosity > 0) {
        cout << " smear factor pre shift " << smearFactor << endl;
        cout << " smearFactorErr " << smearFactorErr << endl; 
    }

    if (smearBy != 0) smearFactor += smearBy * smearFactorErr;
    if (smearFactor < 1.) smearFactor = 1.;
    if (levelVerbosity > 0) {
        cout << " smear factor post shift " << smearFactor << endl;        
        cout << "multiply factor " << TMath::Sqrt(smearFactor * smearFactor - 1.) << endl;
    }
    if (y < pTResolThreshold && y < 20.) {
        inputJetBaseRes = RecoJetLowPtSmearHist->GetBinContent(binIndexNoGenJetMatch);
    }
    else {
        inputJetBaseRes = JetResolutionHighPt(inputJet, inputTF1Vec);
    }
    if (levelVerbosity > 0) {
        cout << "inputJetBaseRes pre multiply " << inputJetBaseRes << endl;   
    }
    inputJetScaleRes = inputJetBaseRes * TMath::Sqrt(smearFactor * smearFactor - 1.);
    if (levelVerbosity > 0) {
        cout << "inputJetBaseRes post multiply (i.e. inputJetScaleRes) " << inputJetScaleRes << endl;   
    }
    for (unsigned int iGenJet = 0; iGenJet < vecGoodGenJets->size(); ++iGenJet) {
//        cout << " test " << endl;        
        currGenJetLV = vecGoodGenJets->at(iGenJet).P4;
//        cout << " test 2 " << currGenJetLV.Pt() <<        endl;
        if (GenJetMatch(inputJet, &currGenJetLV, dRGenReco)) {
            if (bestdRGenReco > dRGenReco) {
                if (!isGenMatched) isGenMatched = true;
                bestGenMatchIndex = iGenJet;
                bestdRGenReco = dRGenReco;
            }
        }
        else {
            continue;
        }
    }
    if (levelVerbosity > 0) {
        cout << "isGenMatched? " << isGenMatched << endl;   
    }
    if (isGenMatched) {
        dEnRecoGen = inEn - vecGoodGenJets->at(bestGenMatchIndex).P4.E();
        if (levelVerbosity > 0) {
            cout << " dEnRecoGen " << dEnRecoGen << endl;
            cout << " sigmaMaxGenJetMatch * inputJetBaseRes " << sigmaMaxGenJetMatch * inputJetBaseRes << endl;
            cout << "(smearFactor - 1.) * dEnRecoGen / inEn " << (smearFactor - 1.) * dEnRecoGen / inEn << endl;
        }
        if (fabs(dEnRecoGen) < (sigmaMaxGenJetMatch * inputJetBaseRes)) {
            smearJetEnergy = inEn * (1. + (smearFactor - 1.) * dEnRecoGen / inEn); 
            //^^ technically this isn't matching the analogous line from the smeared jet producer (line 271) but I don't have access to raw jet info as of right now
        }
        else {
            isGenMatched = false;
        }
    }
    if (!isGenMatched) {
        // gaussian smearing when no gen jet matches
        if (smearFactor > 1. ) {
            smearJetEnergy = inEn * (1. + rnd_.Gaus(0., inputJetScaleRes)/inEn);
            cout << "relative difference: smearJetEnergy - normal E " << (smearJetEnergy - inEn)/inEn << endl;
            //^^ technically this isn't matching the analogous line from the smeared jet producer (line 291) but I don't have access to raw jet info as of right now
        }
    }
    if (smearJetEnergy < minSmearJetEn) smearJetEnergy = minSmearJetEn;
    if (!(y < skipCorrJetPtThreshold )) {
        smearJet *= (smearJetEnergy/inEn);
    }
    if (levelVerbosity > 0) {
        if (isGenMatched ) {
            cout << " jet was Gen Jet matched! " << endl;
        }
        else {
            cout << " jet was NOT Gen Jet matched! " << endl;
        }
        cout << "smearJetEnergy (i.e. post smearing energy) " << smearJetEnergy << endl;
        cout << "smear jet post px " << smearJet.Px() << endl;
        cout << "smear jet post py " << smearJet.Py() << endl;
        cout << "smear jet post pz " << smearJet.Pz() << endl;
        cout << "smear jet post E " << smearJet.E() << endl;
        cout << "smear jet post Pt " << smearJet.Pt() << endl;
        cout << endl;
    }
    return smearJet;
    // 
    // Line 254 of the EDProducer shown above has the following
    // -- double sigmaEn = jetResolutionExtractor_(jet)*TMath::Sqrt(smearFactor*smearFactor - 1.);
    // Looking at line 341 of this file
    // -- SmearedJetProducer_namespace::JetResolutionExtractorT<T> jetResolutionExtractor_;
    // Look here for the definition of what JetResolutionExtractorT is:
    // http://cmslxr.fnal.gov/lxr/source/PhysicsTools/PatUtils/plugins/SmearedJetProducer.cc#015
    // You'll see this salient line (line 26):
    // -- metsig::SigInputObj pfJetResolution = jetResolutions_.evalPFJet(&jet);
    // Looking at a combination of 
    // http://cmslxr.fnal.gov/lxr/source/DataFormats/METReco/interface/SigInputObj.h#047
    // and 
    // http://cmslxr.fnal.gov/lxr/source/RecoMET/METAlgorithms/src/SignAlgoResolutions.cc#109
    // you can recognize that line 134 of the second of the aforementioned two files
    // -- metsig::SigInputObj obj_jet(inputtype,jpt,jphi,jdeltapt,jdeltapphi);
    // shows that jdeltapt is the sigmaEn used in line 254 of the SmearedJetEDProducer
    // This jdeltapt is different for low and high pT jets
    // low pT jets are based off of the jdpt vectors (numbering for different eta bins)
    // http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/RecoMET/METProducers/python/METSigParams_cfi.py?revision=1.3&view=markup
    // Binning for |eta| is steps of 0.5 from 0.0 to 5.0
    // while binning for pT is steps of 2 GeV from 1 to 21 GeV
    // high pT jets are based off of a double sided crystal ball function, specifically the sigma of the gaussian core
    // as can basically be surmised from a combination of 
    // http://cmslxr.fnal.gov/lxr/source/RecoMET/METAlgorithms/src/SignAlgoResolutions.cc#129
    // http://cmslxr.fnal.gov/lxr/source/CondFormats/JetMETObjects/src/JetResolution.cc#082
    // and 
    // http://cmslxr.fnal.gov/lxr/source/CondFormats/JetMETObjects/data/Spring10_PtResolution_AK5PF.txt
    // pT/Eta dependent functions were fit for each of the parameters of the DSCB function, and you can see more discussion for the 
    // "sigma" parameter in the function NoGenJetMatchHighPtResolutionTF1 defined in this header file
}
inline TH2F * ResolutionHistMaker(TString inputFile) {
    // Following Discussion in SmearJet Function this function tries to load in the resolutions defined here:
    // http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/RecoMET/METProducers/python/METSigParams_cfi.py?revision=1.3&view=markup
    // (look for jdpt# (where # runs from 0 to 9)
    // the jdpt# refers to different Eta bins, as can be ascertained from 
    // http://cmslxr.fnal.gov/lxr/source/RecoMET/METAlgorithms/src/SignAlgoResolutions.cc#121
    // and 
    // http://cmslxr.fnal.gov/lxr/source/RecoMET/METAlgorithms/src/SignAlgoResolutions.cc#151
    // The function reads in a text file where these parameters have been put in, 
    // with the first two lines contain pT eta information (number of eta/Pt bins and bin ranges)
    
    TH2F * outHist;
    ifstream inputFileStream(inputFile);
    vector<unsigned int> pTParams, EtaParams;
    //    int nPtBins, PtMin, PtMax, nEtaBins, EtaMin, EtaMax;
    string line, field;
    int pTCounter;
    if (!(inputFileStream.eof())) {
        getline(inputFileStream, line);
        stringstream ss( line );
        while (std::getline(ss, field, ',' )) {
            stringstream fs ( field );
            int currField = 0;
            fs >> currField;
            pTParams.push_back(currField);
        }
    }
    else {
        cout << "issue with inputFile: prematurely reached end of file " << endl;
    }
    if (!(inputFileStream.eof())) {
        getline(inputFileStream, line);
        stringstream ss( (line) );
        while (std::getline(ss, field, ',' )) {
            stringstream fs ( field );
            int currField = 0;
            fs >> currField;
            EtaParams.push_back(currField);
        }
    }
    else {
        cout << "issue with inputFile: prematurely reached end of file " << endl;
    }
    outHist = new TH2F("h_MCJetResol", ";Jet #eta;Jet p_{T}", EtaParams[0], EtaParams[1], EtaParams[2], pTParams[0], pTParams[1], pTParams[2]);
    for (unsigned int iEta = 0; iEta < EtaParams[0]; ++iEta) {
        pTCounter = 0;
        if (!(inputFileStream.eof())) {
            getline(inputFileStream, line);
            stringstream ss( (line) );
            while (std::getline(ss, field, ',' )) {
                stringstream fs ( field );
                float currField = 0.0;
                fs >> currField;
                if (pTCounter == 0) outHist->SetBinContent(iEta + 1, pTCounter, currField);
                    outHist->SetBinContent(iEta + 1, pTCounter + 1, currField);
                ++pTCounter;
            }
        }
        else {
            cout << "issue with inputFile: prematurely reached end of file " << endl;
        }
    }
    for (int i = 1; i < outHist->GetNbinsX() + 1; ++i) {
        for (int j = 1; j < outHist->GetNbinsY() + 1; ++j) {
            cout << "bin (" << i << ", " << j << ") is (eta = " << outHist->GetXaxis()->GetBinLowEdge(i) << ", pT = " << outHist->GetYaxis()->GetBinLowEdge(j) << endl;
            cout << "bin content in this bin: " << outHist->GetBinContent(i, j) << endl; 
        }        
    }
    return outHist;
}


/*
inline TH2F * FakeLeptonHistMaker(TString inputFile, int whichHistType, int whichLepType) {
    // Reads in input from .txt files that contain the lepton fake and/or prompt rate
    
    ￼pT (GeV)
    10 < pT ≤ 15 15 < pT ≤ 20 20 < pT ≤ 25 25 < pT ≤ 50 50 < pT
    pT (GeV)
    electron prompt rate
    0 < η ≤ 1.4442 1.566<η≤2.5
    0.833 ± 0.015 0.75 ± 0.02 0.872 ± 0.007 0.785 ± 0.013 0.902 ± 0.004 0.828 ± 0.008 0.9592 ± 0.0005 0.904 ± 0.001 0.9783 ± 0.0010 0.942 ± 0.003
    muon prompt rate
    ￼￼￼￼￼￼￼￼￼￼￼￼￼0<η≤1.479
    1.479 < η ≤ 2.4
    0.844 ± 0.009 0.895 ± 0.005 0.935 ± 0.003 0.9761 ± 0.0005 0.9921 ± 0.0008
    ￼10 < pT ≤ 15 ￼ 0.837 ± 0.009 15 < pT ≤ 20 ￼ 0.881 ± 0.005 20 < pT ≤ 25 ￼ 0.915 ± 0.003 25 < pT ≤ 50 ￼ 0.9754 ± 0.0003
    ￼50 < pT
    0.9918 ± 0.0005
    ￼
    TString lepString = whichLepType == 11 ? "Elec" : "Muon";
    TString typeString = whichHistType == 1 ? "Prompt" : "Fake";
    TString histName = "h_";
    histName += lepString;
    histName += typeString;
    histName += "Rate";
    TString axisString = ";Lepton #eta;Lepton p_{T} [GeV]";
    TH2F * outHist;
    ifstream inputFileStream(inputFile);
    vector<unsigned int> pTParams;
    float etaBounds[5] = {};
    //    int nPtBins, PtMin, PtMax, nEtaBins, EtaMin, EtaMax;
    string line, field;
    int pTCounter;
    if (!(inputFileStream.eof())) {
        getline(inputFileStream, line);
        stringstream ss( line );
        while (std::getline(ss, field, ',' )) {
            stringstream fs ( field );
            int currField = 0;
            fs >> currField;
            pTParams.push_back(currField);
        }
    }
    else {
        cout << "issue with inputFile: prematurely reached end of file " << endl;
    }
    if (!(inputFileStream.eof())) {
        getline(inputFileStream, line);
        stringstream ss( (line) );
        while (std::getline(ss, field, ',' )) {
            stringstream fs ( field );
            int currField = 0;
            fs >> currField;
            EtaParams.push_back(currField);
        }
    }
    else {
        cout << "issue with inputFile: prematurely reached end of file " << endl;
    }
    outHist = new TH2F("h_MCJetResol", ";Jet #eta;Jet p_{T}", EtaParams[0], EtaParams[1], EtaParams[2], pTParams[0], pTParams[1], pTParams[2]);
    for (unsigned int iEta = 0; iEta < EtaParams[0]; ++iEta) {
        pTCounter = 0;
        if (!(inputFileStream.eof())) {
            getline(inputFileStream, line);
            stringstream ss( (line) );
            while (std::getline(ss, field, ',' )) {
                stringstream fs ( field );
                float currField = 0.0;
                fs >> currField;
                if (pTCounter == 0) outHist->SetBinContent(iEta + 1, pTCounter, currField);
                outHist->SetBinContent(iEta + 1, pTCounter + 1, currField);
                ++pTCounter;
            }
        }
        else {
            cout << "issue with inputFile: prematurely reached end of file " << endl;
        }
    }
    for (int i = 1; i < outHist->GetNbinsX() + 1; ++i) {
        for (int j = 1; j < outHist->GetNbinsY() + 1; ++j) {
            cout << "bin (" << i << ", " << j << ") is (eta = " << outHist->GetXaxis()->GetBinLowEdge(i) << ", pT = " << outHist->GetYaxis()->GetBinLowEdge(j) << endl;
            cout << "bin content in this bin: " << outHist->GetBinContent(i, j) << endl; 
        }        
    }
    return outHist;
}
*/



inline vector<PFJet> * JetSmear(vector<PFJet> * InputJets, vector<GenJet> * vecGoodGenJets, float systJetES, float systJetSmear, TH2F * shiftHistJetES, TH2F * shiftHistLowPt, TH2F * shiftHistSmearFactor, vector<TF1> * inputTF1Vec, int levelVerbosity, TH1F * DeltaEnergyHist) {
    
    cout << "BROKEN!!!!" << endl;
    cout << "BROKEN!!!!" << endl;
    cout << "BROKEN!!!!" << endl;
    cout << "BROKEN!!!!" << endl;
    cout << "" << endl;
    vector<PFJet> * outSmearJets = new vector<PFJet>;
    TLorentzVector patsyJetLV, patsyJetLVSmear;
    PFJet currPFJet;
    float JetFactor;
    float currJetPt, currJetEta;
    for (unsigned int iJet = 0; iJet < InputJets->size(); ++iJet) {
        currPFJet = InputJets->at(iJet);
        if (levelVerbosity > 0) {
            cout << "currently on Jet " << iJet << endl;
            cout << " which has: " << endl;
            cout << "Pt: " << currPFJet.P4.Pt() << endl;
            cout << "Eta: " << currPFJet.P4.Eta() << endl;
            cout << "Phi: " << currPFJet.P4.Phi() << endl;
            cout << "Energy: " << currPFJet.P4.E() << endl;
        }
        patsyJetLVSmear = SmearJet(&currPFJet.P4, vecGoodGenJets, currPFJet.isGenJetMatched, currPFJet.dEnRecoGen, shiftHistSmearFactor, shiftHistLowPt, inputTF1Vec, systJetSmear, levelVerbosity);
        /*
        cout << "patsyJetLVSmear px " << patsyJetLVSmear.Px() << endl;
        cout << "patsyJetLVSmear py " << patsyJetLVSmear.Py() << endl;
        cout << "patsyJetLVSmear pz " << patsyJetLVSmear.Pz() << endl;
        cout << "patsyJetLVSmear E " << patsyJetLVSmear.E() << endl;    
         */
        currJetPt = patsyJetLVSmear.Pt();
        currJetEta = patsyJetLVSmear.Eta();
        if (systJetES != 0 && currJetPt > 9.0) {
            JetFactor = JESUncertFactor(shiftHistJetES, systJetES, currJetPt, currJetEta);
            patsyJetLVSmear *= JetFactor;
        }
        DeltaEnergyHist->Fill(patsyJetLVSmear.E() - currPFJet.P4.E());
        currPFJet.P4 = patsyJetLVSmear;
        outSmearJets->push_back(currPFJet);
    }
    return outSmearJets;
}

/***********************************************************************************************/
/***********************************************************************************************/
// And now begin some ill-fated attempts at performing jet smearing by myself....ugh so annoying
/***********************************************************************************************/
/***********************************************************************************************/


inline void JetSmearMatchUnSmear(vector<SmearPFJet> * vecGoodSmearPFJets, PFJetEventPointers inSmearPFJEPs, vector<PFJet> * vecGoodPFJets, vector<GenJet> * vecGoodGenJets, TH2F * h_DeltPtSmearUnsmear_GenMatch, TH2F * h_DeltPtSmearUnsmear_NotGenMatch) {
    SmearPFJet currSmearPFJet;
    bool foundMatchGoodUnsmearJet;
    for (unsigned int iJet = 0; iJet < inSmearPFJEPs.numPFJets; ++iJet) {
        foundMatchGoodUnsmearJet = 0;
        inSmearPFJEPs.SetPFJet(iJet, &currSmearPFJet.intPFJet);
        currSmearPFJet.ExtractParams();
        foundMatchGoodUnsmearJet = currSmearPFJet.SetNonKinematicValues(vecGoodPFJets);
        if (!foundMatchGoodUnsmearJet) continue;
        else {
            MatchGenJet(&currSmearPFJet.intPFJet, vecGoodGenJets);
            if (currSmearPFJet.intPFJet.isGenJetMatched) {
                h_DeltPtSmearUnsmear_GenMatch->Fill(currSmearPFJet.NonSmearP4.Pt(), currSmearPFJet.intPFJet.BVC.Vec_Pt - currSmearPFJet.NonSmearP4.Pt());
//                cout << "currSmearPFJet.intPFJet dEn " << currSmearPFJet.intPFJet.dEnRecoGen<< endl;
            }
            else {
                h_DeltPtSmearUnsmear_NotGenMatch->Fill(currSmearPFJet.NonSmearP4.Pt(), currSmearPFJet.intPFJet.BVC.Vec_Pt - currSmearPFJet.NonSmearP4.Pt());
            }            
            vecGoodSmearPFJets->push_back(currSmearPFJet);
        }
    }
}


inline void JetSmearMatchUnSmear(vector<SmearPFJet> * vecGoodSmearPFJets, PFJetEventPointers inSmearPFJEPs, vector<PFJet> * vecGoodPFJets, vector<GenJet> * vecGoodGenJets, vector<float> * vecDeltaEnUnsmearJetSmearJetGenMatched, vector<float> * vecDeltaEnUnsmearJetSmearJetNotGenMatched, vector<float> * vecDeltaEnUnsmearJetGenMatchJet, vector<float> * vecGoodUnsmearJetEn) {
  SmearPFJet currSmearPFJet;
  bool foundMatchGoodUnsmearJet;
  for (unsigned int iJet = 0; iJet < inSmearPFJEPs.numPFJets; ++iJet) {
    foundMatchGoodUnsmearJet = 0;
    inSmearPFJEPs.SetPFJet(iJet, &currSmearPFJet.intPFJet);
    currSmearPFJet.ExtractParams();
    foundMatchGoodUnsmearJet = currSmearPFJet.SetNonKinematicValues(vecGoodPFJets);
    if (!foundMatchGoodUnsmearJet) continue;
    else {
      MatchGenJet(&currSmearPFJet.intPFJet, vecGoodGenJets);
      if (currSmearPFJet.intPFJet.isGenJetMatched) {
	vecDeltaEnUnsmearJetGenMatchJet->push_back(currSmearPFJet.intPFJet.dEnRecoGen);
	vecDeltaEnUnsmearJetSmearJetGenMatched->push_back(currSmearPFJet.intPFJet.BVC.Vec_En - currSmearPFJet.NonSmearP4.E());
	//                cout << "currSmearPFJet.intPFJet dEn " << currSmearPFJet.intPFJet.dEnRecoGen<< endl;
      }
      else {
	vecDeltaEnUnsmearJetSmearJetNotGenMatched->push_back(currSmearPFJet.intPFJet.BVC.Vec_En - currSmearPFJet.NonSmearP4.E());
      }
      vecGoodUnsmearJetEn->push_back(currSmearPFJet.NonSmearP4.E());
      vecGoodSmearPFJets->push_back(currSmearPFJet);
    }
  }
}









inline void JetGenMatchUnSmear(vector<PFJet> * vecGoodPFJets, vector<GenJet> * vecGoodGenJets, vector<float> * vecDeltaEnUnsmearJetGenMatchJet, vector<float> * vecGoodUnsmearJetEn) {
  bool foundGenMatch = 0;
  for (unsigned int iJet = 0; iJet < vecGoodPFJets->size(); ++iJet) {
    foundGenMatch = 0;
    MatchGenJet(&vecGoodPFJets->at(iJet), vecGoodGenJets);
    if (vecGoodPFJets->at(iJet).isGenJetMatched) {
      //            cout << "iJet " << iJet << endl;
      //            cout << "vecGoodPFJets->at(iJet).BVC.Vec_En " << vecGoodPFJets->at(iJet).BVC.Vec_En << endl;
      //            cout << "vecGoodPFJets->at(iJet).dEnRecoGen " << vecGoodPFJets->at(iJet).dEnRecoGen << endl;
      vecDeltaEnUnsmearJetGenMatchJet->push_back(vecGoodPFJets->at(iJet).dEnRecoGen);
      vecGoodUnsmearJetEn->push_back(vecGoodPFJets->at(iJet).BVC.Vec_En);
    }
  }
}

inline void Jet_ScaleUpSmearing_SF(vector<SmearPFJet> * vecGoodSmearPFJets, vector<SmearPFJet> * vecGoodSmearPFJets_Scaled, float DeltaEnSF = 1.0) {
    TLorentzVector currDiffVec;
    for (unsigned int iSmearJet = 0; iSmearJet < vecGoodSmearPFJets->size(); ++iSmearJet) {
        vecGoodSmearPFJets_Scaled->push_back(vecGoodSmearPFJets->at(iSmearJet));
        currDiffVec = vecGoodSmearPFJets_Scaled->at(iSmearJet).intPFJet.P4 - vecGoodSmearPFJets_Scaled->at(iSmearJet).NonSmearP4;
        currDiffVec *= DeltaEnSF;
        vecGoodSmearPFJets_Scaled->at(iSmearJet).intPFJet.P4 = currDiffVec + vecGoodSmearPFJets_Scaled->at(iSmearJet).NonSmearP4;
    }
}

inline void Jet_ScaleUpSmearing_SFHist(vector<SmearPFJet> * vecGoodSmearPFJets, vector<SmearPFJet> * vecGoodSmearPFJets_Scaled, TH2F * histSF, int whichDir = 0) {
    TLorentzVector currDiffVec;
    float DeltaEnSF;
    float centValSF, systShiftSF;
    int XBin, YBin;
    for (unsigned int iSmearJet = 0; iSmearJet < vecGoodSmearPFJets->size(); ++iSmearJet) {
        vecGoodSmearPFJets_Scaled->push_back(vecGoodSmearPFJets->at(iSmearJet));
        XBin = histSF->GetXaxis()->FindBin(fabs(vecGoodSmearPFJets_Scaled->at(iSmearJet).intPFJet.P4.Eta()));
        YBin = histSF->GetYaxis()->FindBin(fabs(vecGoodSmearPFJets_Scaled->at(iSmearJet).intPFJet.P4.Pt()));
        centValSF = histSF->GetBinContent(XBin, YBin);
        systShiftSF = centValSF + whichDir * histSF->GetBinError(XBin, YBin);
//        cout << "centVal SF " << centValSF << endl;
//        cout << "systShiftSF " << systShiftSF << endl;
        if (systShiftSF <= 1.0) {
            vecGoodSmearPFJets_Scaled->at(iSmearJet).intPFJet.P4 = vecGoodSmearPFJets_Scaled->at(iSmearJet).NonSmearP4;
        }
        else {
            DeltaEnSF = systShiftSF/centValSF;
            currDiffVec = vecGoodSmearPFJets_Scaled->at(iSmearJet).intPFJet.P4 - vecGoodSmearPFJets_Scaled->at(iSmearJet).NonSmearP4;
//            cout << "vecGoodSmearPFJets_Scaled->at(iSmearJet).intPFJet.P4.Pt() " << vecGoodSmearPFJets_Scaled->at(iSmearJet).intPFJet.P4.Pt() << endl;
//            cout << "DeltaEnSF " << DeltaEnSF << endl;
            currDiffVec *= DeltaEnSF;
            vecGoodSmearPFJets_Scaled->at(iSmearJet).intPFJet.P4 = currDiffVec + vecGoodSmearPFJets_Scaled->at(iSmearJet).NonSmearP4;
            vecGoodSmearPFJets_Scaled->at(iSmearJet).ExtractParams();
//            cout << "vecGoodSmearPFJets_Scaled->at(iSmearJet).intPFJet.P4.Pt() " << vecGoodSmearPFJets_Scaled->at(iSmearJet).intPFJet.P4.Pt() << endl;
        }
    }
}

inline void MET_AddMET(EventMETInfo * inEMI, float addMETX, float addMETY) {
    float initMETX, initMETY;
    float newMETX, newMETY;
    METComponents(inEMI->EventMET, inEMI->EventMETPhi, initMETX, initMETY, 1);   
    newMETX = initMETX + addMETX;
    newMETY = initMETY + addMETY;
    METComponents(inEMI->EventMET, inEMI->EventMETPhi, newMETX, newMETY, 0);
}

inline void MET_CalcAddMET(EventMETInfo * inEMICV, EventMETInfo * inEMISmeared, float &addMETX, float &addMETY, float DeltaEnSF = 1.0) {
    float METX_CV, METY_CV;
    float METX_Smear, METY_Smear;
    METComponents(inEMICV->EventMET, inEMICV->EventMETPhi, METX_CV, METY_CV, 1);
    METComponents(inEMISmeared->EventMET, inEMISmeared->EventMETPhi, METX_Smear, METY_Smear, 1);
    addMETX =  (DeltaEnSF - 1.0) * (METX_Smear - METX_CV);
    addMETY =  (DeltaEnSF - 1.0) * (METY_Smear - METY_CV);
}
inline void MET_ScaleUpSmearing_SF(EventMETInfo * inEMICV, EventMETInfo * inEMISmeared, float DeltaEnSF = 1.0) {
    float METX_CV, METY_CV;
    float METX_Smear, METY_Smear;
    METComponents(inEMICV->EventMET, inEMICV->EventMETPhi, METX_CV, METY_CV, 1);
    METComponents(inEMISmeared->EventMET, inEMISmeared->EventMETPhi, METX_Smear, METY_Smear, 1);
    METX_Smear = METX_CV + DeltaEnSF * (METX_Smear - METX_CV);
    METY_Smear = METY_CV + DeltaEnSF * (METY_Smear - METY_CV);
    METComponents(inEMISmeared->EventMET, inEMISmeared->EventMETPhi, METX_Smear, METY_Smear, 0);
}

inline void JetKinematicsCut(EventJetInfo * inEJI, vector<PFJet> * vecGoodPFJets) {
    int NJets = 0;
    int NBJets = 0;
    float HT = 0;
    TLorentzVector currJetSumVec; DefaultLorentzVector(&currJetSumVec);
    TLorentzVector currBJetSumVec; DefaultLorentzVector(&currBJetSumVec);
    vector<int> vecJetIndex;
    vector<int> vecBJetIndex;
    vector<int> vecBJetSubIndex;
    
    for (int iJet = 0; iJet < inEJI->numSavedJets; ++iJet) {
        vecJetIndex.push_back(-1);
    }
    for (int iBJet = 0; iBJet < inEJI->numSavedBJets; ++iBJet) {
        vecBJetIndex.push_back(-1);
        vecBJetSubIndex.push_back(-1);
    }
    
    
    float currJetPt, currJetEta;
    
    float BTagWP = 0.679;  //CSV Middle working point, see (remove underscore in address): h_ttps://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP
    float JetPtCut = 30, JetEtaCut = 2.4;
    
    unsigned int numGoodJets = vecGoodPFJets->size();
    for (unsigned int iJet = 0; iJet < numGoodJets; ++iJet) {
        currJetPt = vecGoodPFJets->at(iJet).BVC.Vec_Pt;
        currJetEta = vecGoodPFJets->at(iJet).BVC.Vec_Eta;
        if (currJetPt < JetPtCut) continue;
        if (fabs(currJetEta) > JetEtaCut) continue;
        if (NJets == 0) currJetSumVec = vecGoodPFJets->at(iJet).P4;
        else currJetSumVec += vecGoodPFJets->at(iJet).P4;

        ++NJets;
        HT += currJetPt;
        
        if (NJets <= inEJI->numSavedJets) {
            vecJetIndex[NJets - 1] = iJet;
        }
        if (vecGoodPFJets->at(iJet).valBTagDisc > BTagWP) {
            vecGoodPFJets->at(iJet).isBJet = true;
            
            if (NBJets == 0) currBJetSumVec = vecGoodPFJets->at(iJet).P4;
            else currBJetSumVec += vecGoodPFJets->at(iJet).P4;
            
            ++NBJets;
            if (NBJets <= inEJI->numSavedBJets) {
                vecBJetIndex[NBJets - 1] = iJet;
                vecBJetSubIndex[NBJets - 1] = NJets - 1;
            }
        }
        else {
            vecGoodPFJets->at(iJet).isBJet = false;
        }
    }
    inEJI->EJISetValsInput(HT, &currJetSumVec, &currBJetSumVec, NJets, &vecJetIndex, NBJets, &vecBJetIndex, &vecBJetSubIndex, vecGoodPFJets);
}
inline void JetKinematicsCut(EventJetInfo * inEJI, vector<PFJet> * vecGoodPFJets, BTagSFUtil * inputBTagSFUtil, bool isData) {
    int NJets = 0;
    int NBJets = 0;
    float HT = 0;
    
    TLorentzVector currJetSumVec; DefaultLorentzVector(&currJetSumVec);
    TLorentzVector currBJetSumVec; DefaultLorentzVector(&currBJetSumVec);
    vector<int> vecJetIndex;
    vector<int> vecBJetIndex;
    vector<int> vecBJetSubIndex;
    for (int iJet = 0; iJet < inEJI->numSavedJets; ++iJet) {
        vecJetIndex.push_back(-1);
    }
    for (int iBJet = 0; iBJet < inEJI->numSavedBJets; ++iBJet) {
        vecBJetIndex.push_back(-1);
        vecBJetSubIndex.push_back(-1);
    }
    float currJetPt, currJetEta, currJetValBTagDisc;
    int currJetPartFlav;
    float JetPtCut = 30, JetEtaCut = 2.4;
    unsigned int numGoodJets = vecGoodPFJets->size();
    for (unsigned int iJet = 0; iJet < numGoodJets; ++iJet) {
        currJetPt = vecGoodPFJets->at(iJet).BVC.Vec_Pt;
        currJetEta = vecGoodPFJets->at(iJet).BVC.Vec_Eta;
        if (currJetPt < JetPtCut) continue;
        if (fabs(currJetEta) > JetEtaCut) continue;
        if (NJets == 0) currJetSumVec = vecGoodPFJets->at(iJet).P4;
        else currJetSumVec += vecGoodPFJets->at(iJet).P4;

        ++NJets;
        HT += currJetPt;     
        
        if (NJets <= inEJI->numSavedJets) {
            vecJetIndex[NJets - 1] = iJet;
        }
        
        currJetPartFlav = (isData) ? -999999 : vecGoodPFJets->at(iJet).partonFlavor;
        currJetValBTagDisc = vecGoodPFJets->at(iJet).valBTagDisc;
        if (inputBTagSFUtil->IsTagged(currJetValBTagDisc, currJetPartFlav, currJetPt, currJetEta, 0, 0, false)) {
            vecGoodPFJets->at(iJet).isBJet = true;
            
            if (NBJets == 0) currBJetSumVec = vecGoodPFJets->at(iJet).P4;
            else currBJetSumVec += vecGoodPFJets->at(iJet).P4;
            ++NBJets;
            
            if (NBJets <= inEJI->numSavedBJets) {
                vecBJetIndex[NBJets - 1] = iJet;
                vecBJetSubIndex[NBJets - 1] = NJets - 1; 
            }
            
        }
        else {
            vecGoodPFJets->at(iJet).isBJet = false;
        }
    }
    inEJI->EJISetValsInput(HT, &currJetSumVec, &currBJetSumVec, NJets, &vecJetIndex, NBJets, &vecBJetIndex, &vecBJetSubIndex, vecGoodPFJets);
}

inline void JetKinematicsCutBTagSyst(EventJetInfo * inEJI, vector<PFJet> * vecGoodPFJets, BTagSFUtil * inputBTagSFUtil, int BTagEffSystVar, int BMisTagSFSystVar, bool isSignal) {
    int NJets = 0;
    int NBJets = 0;
    float HT = 0;
    TLorentzVector currJetSumVec; DefaultLorentzVector(&currJetSumVec);
    TLorentzVector currBJetSumVec; DefaultLorentzVector(&currBJetSumVec);
    vector<int> vecJetIndex;
    vector<int> vecBJetIndex;
    vector<int> vecBJetSubIndex;
    
    for (int iJet = 0; iJet < inEJI->numSavedJets; ++iJet) {
        vecJetIndex.push_back(-1);
    }
    for (int iBJet = 0; iBJet < inEJI->numSavedBJets; ++iBJet) {
        vecBJetIndex.push_back(-1);
        vecBJetSubIndex.push_back(-1);
    }
    
    float currJetPt, currJetEta, currJetValBTagDisc;
    int currJetPartFlav;
    float JetPtCut = 30, JetEtaCut = 2.4;
    bool currJetIsBJet;
    int FastSimSystVar;
    int SystVar;
    unsigned int numGoodJets = vecGoodPFJets->size();
    for (unsigned int iJet = 0; iJet < numGoodJets; ++iJet) {
        currJetPt = vecGoodPFJets->at(iJet).P4.Pt();
        currJetEta = vecGoodPFJets->at(iJet).P4.Eta();
        if (currJetPt < JetPtCut) continue;
        if (fabs(currJetEta) > JetEtaCut) continue;
        if (NJets == 0) currJetSumVec = vecGoodPFJets->at(iJet).P4;
        else currJetSumVec += vecGoodPFJets->at(iJet).P4;
       
        ++NJets;
        HT += currJetPt;
        
        if (NJets <= inEJI->numSavedJets) {
            vecJetIndex[NJets - 1] = iJet;
        }

        
        currJetPartFlav = vecGoodPFJets->at(iJet).partonFlavor;
        currJetIsBJet = (abs(currJetPartFlav) == 5 || abs(currJetPartFlav == 4)); 
        currJetValBTagDisc = vecGoodPFJets->at(iJet).valBTagDisc;
        if (currJetIsBJet) {
            SystVar = BTagEffSystVar;
        }
        else {
            SystVar = BMisTagSFSystVar;
        }
        FastSimSystVar = (isSignal) ? SystVar : 0;
        if (inputBTagSFUtil->IsTagged(currJetValBTagDisc, currJetPartFlav, currJetPt, currJetEta, SystVar, FastSimSystVar, false)) {
            vecGoodPFJets->at(iJet).isBJet = true;

            if (NBJets == 0) currBJetSumVec = vecGoodPFJets->at(iJet).P4;
            else currBJetSumVec += vecGoodPFJets->at(iJet).P4;
            ++NBJets;
            
            if (NBJets <= inEJI->numSavedBJets) {
                vecBJetIndex[NBJets - 1] = iJet;
                vecBJetSubIndex[NBJets - 1] = NJets - 1; 
            }
            
        }
        else {
            vecGoodPFJets->at(iJet).isBJet = false;
        }
    }
    inEJI->EJISetValsInput(HT, &currJetSumVec, &currBJetSumVec, NJets, &vecJetIndex, NBJets, &vecBJetIndex, &vecBJetSubIndex, vecGoodPFJets);
}



inline void JetKinematicsCutSmearedJet(EventJetInfo * inEJI, vector<SmearPFJet> * vecGoodSmearPFJets, BTagSFUtil * inputBTagSFUtil) {
    int NJets = 0;
    int NBJets = 0;
    float HT = 0;
    TLorentzVector currJetSumVec; DefaultLorentzVector(&currJetSumVec);
    TLorentzVector currBJetSumVec; DefaultLorentzVector(&currBJetSumVec);
    vector<int> vecJetIndex;
    vector<int> vecBJetIndex;
    vector<int> vecBJetSubIndex;
    for (int iJet = 0; iJet < inEJI->numSavedJets; ++iJet) {
        vecJetIndex.push_back(-1);
    }
    for (int iBJet = 0; iBJet < inEJI->numSavedBJets; ++iBJet) {
        vecBJetIndex.push_back(-1);
        vecBJetSubIndex.push_back(-1);
    }
    float currJetPt, currJetEta, currJetValBTagDisc;
    int currJetPartFlav;
    float JetPtCut = 30, JetEtaCut = 2.4;
    unsigned int numGoodJets = vecGoodSmearPFJets->size();
    for (unsigned int iJet = 0; iJet < numGoodJets; ++iJet) {
        currJetPt = vecGoodSmearPFJets->at(iJet).intPFJet.BVC.Vec_Pt;
        currJetEta = vecGoodSmearPFJets->at(iJet).intPFJet.BVC.Vec_Eta;
        if (currJetPt < JetPtCut) continue;
        if (fabs(currJetEta) > JetEtaCut) continue;
        if (NJets == 0) currJetSumVec = vecGoodSmearPFJets->at(iJet).intPFJet.P4;
        else currJetSumVec += vecGoodSmearPFJets->at(iJet).intPFJet.P4;
        
        ++NJets;
        HT += currJetPt;     
        
        if (NJets <= inEJI->numSavedJets) {
            vecJetIndex[NJets - 1] = iJet;
        }
        
        currJetPartFlav = vecGoodSmearPFJets->at(iJet).intPFJet.partonFlavor;
        currJetValBTagDisc = vecGoodSmearPFJets->at(iJet).intPFJet.valBTagDisc;
        if (inputBTagSFUtil->IsTagged(currJetValBTagDisc, currJetPartFlav, currJetPt, currJetEta, 0, 0, false)) {
            vecGoodSmearPFJets->at(iJet).intPFJet.isBJet = true;
            
            if (NBJets == 0) currBJetSumVec = vecGoodSmearPFJets->at(iJet).intPFJet.P4;
            else currBJetSumVec += vecGoodSmearPFJets->at(iJet).intPFJet.P4;
            ++NBJets;
            
            if (NBJets <= inEJI->numSavedBJets) {
                vecBJetIndex[NBJets - 1] = iJet;
                vecBJetSubIndex[NBJets - 1] = NJets - 1; 
            }
            
        }
        else {
            vecGoodSmearPFJets->at(iJet).intPFJet.isBJet = false;
        }
    }
    inEJI->EJISetValsInput(HT, &currJetSumVec, &currBJetSumVec, NJets, &vecJetIndex, NBJets, &vecBJetIndex, &vecBJetSubIndex, vecGoodSmearPFJets);
}


inline void JetKinematicsCutSmearedJetESSyst(EventJetInfo * inEJI, vector<SmearPFJet> * vecGoodSmearPFJets, BTagSFUtil * inputBTagSFUtil, float whichSystCase, TH2F * shiftHist) {
    int NJets = 0;
    int NBJets = 0;
    float HT = 0;
    TLorentzVector currJetSumVec; DefaultLorentzVector(&currJetSumVec);
    TLorentzVector currBJetSumVec; DefaultLorentzVector(&currBJetSumVec);
    vector<int> vecJetIndex;
    vector<int> vecBJetIndex;
    vector<int> vecBJetSubIndex;
    for (int iJet = 0; iJet < inEJI->numSavedJets; ++iJet) {
        vecJetIndex.push_back(-1);
    }
    for (int iBJet = 0; iBJet < inEJI->numSavedBJets; ++iBJet) {
        vecBJetIndex.push_back(-1);
        vecBJetSubIndex.push_back(-1);
    }
    float JetFactor;
    float currJetPt, currJetEta, currJetValBTagDisc;
    int currJetPartFlav;
    float JetPtCut = 30, JetEtaCut = 2.4;
    unsigned int numGoodJets = vecGoodSmearPFJets->size();
    for (unsigned int iJet = 0; iJet < numGoodJets; ++iJet) {
        currJetPt = vecGoodSmearPFJets->at(iJet).intPFJet.BVC.Vec_Pt;
        currJetEta = vecGoodSmearPFJets->at(iJet).intPFJet.BVC.Vec_Eta;
        if (whichSystCase != 0) {
            JetFactor = JESUncertFactor(shiftHist, whichSystCase, (float) currJetPt, (float) currJetEta);
            vecGoodSmearPFJets->at(iJet).intPFJet.P4 *= JetFactor;
            vecGoodSmearPFJets->at(iJet).intPFJet.BVC.ExtractParams(&vecGoodSmearPFJets->at(iJet).intPFJet.P4);
        }    
        currJetPt = vecGoodSmearPFJets->at(iJet).intPFJet.BVC.Vec_Pt;
        currJetEta = vecGoodSmearPFJets->at(iJet).intPFJet.BVC.Vec_Eta;
        if (currJetPt < JetPtCut) continue;
        if (fabs(currJetEta) > JetEtaCut) continue;
        if (NJets == 0) currJetSumVec = vecGoodSmearPFJets->at(iJet).intPFJet.P4;
        else currJetSumVec += vecGoodSmearPFJets->at(iJet).intPFJet.P4;
        
        ++NJets;
        HT += currJetPt;     
        
        if (NJets <= inEJI->numSavedJets) {
            vecJetIndex[NJets - 1] = iJet;
        }
        
        currJetPartFlav = vecGoodSmearPFJets->at(iJet).intPFJet.partonFlavor;
        currJetValBTagDisc = vecGoodSmearPFJets->at(iJet).intPFJet.valBTagDisc;
        if (inputBTagSFUtil->IsTagged(currJetValBTagDisc, currJetPartFlav, currJetPt, currJetEta, 0, 0, false)) {
            vecGoodSmearPFJets->at(iJet).intPFJet.isBJet = true;
            
            if (NBJets == 0) currBJetSumVec = vecGoodSmearPFJets->at(iJet).intPFJet.P4;
            else currBJetSumVec += vecGoodSmearPFJets->at(iJet).intPFJet.P4;
            ++NBJets;
            
            if (NBJets <= inEJI->numSavedBJets) {
                vecBJetIndex[NBJets - 1] = iJet;
                vecBJetSubIndex[NBJets - 1] = NJets - 1; 
            }
            
        }
        else {
            vecGoodSmearPFJets->at(iJet).intPFJet.isBJet = false;
        }
    }
    inEJI->EJISetValsInput(HT, &currJetSumVec, &currBJetSumVec, NJets, &vecJetIndex, NBJets, &vecBJetIndex, &vecBJetSubIndex, vecGoodSmearPFJets);
}


inline void JetKinematicsCutSmearedJetBTagSyst(EventJetInfo * inEJI, vector<SmearPFJet> * vecGoodSmearPFJets, BTagSFUtil * inputBTagSFUtil, int BTagEffSystVar, int BMisTagSFSystVar, bool isSignal) {
    int NJets = 0;
    int NBJets = 0;
    float HT = 0;
    TLorentzVector currJetSumVec; DefaultLorentzVector(&currJetSumVec);
    TLorentzVector currBJetSumVec; DefaultLorentzVector(&currBJetSumVec);
    vector<int> vecJetIndex;
    vector<int> vecBJetIndex;
    vector<int> vecBJetSubIndex;
    
    for (int iJet = 0; iJet < inEJI->numSavedJets; ++iJet) {
        vecJetIndex.push_back(-1);
    }
    for (int iBJet = 0; iBJet < inEJI->numSavedBJets; ++iBJet) {
        vecBJetIndex.push_back(-1);
        vecBJetSubIndex.push_back(-1);
    }
    
    float currJetPt, currJetEta, currJetValBTagDisc;
    int currJetPartFlav;
    float JetPtCut = 30, JetEtaCut = 2.4;
    bool currJetIsBJet;
    int FastSimSystVar;
    int SystVar;
    unsigned int numGoodJets = vecGoodSmearPFJets->size();
    for (unsigned int iJet = 0; iJet < numGoodJets; ++iJet) {
        currJetPt = vecGoodSmearPFJets->at(iJet).intPFJet.P4.Pt();
        currJetEta = vecGoodSmearPFJets->at(iJet).intPFJet.P4.Eta();
        if (currJetPt < JetPtCut) continue;
        if (fabs(currJetEta) > JetEtaCut) continue;
        if (NJets == 0) currJetSumVec = vecGoodSmearPFJets->at(iJet).intPFJet.P4;
        else currJetSumVec += vecGoodSmearPFJets->at(iJet).intPFJet.P4;
        
        ++NJets;
        HT += currJetPt;
        
        if (NJets <= inEJI->numSavedJets) {
            vecJetIndex[NJets - 1] = iJet;
        }
        
        
        currJetPartFlav = vecGoodSmearPFJets->at(iJet).intPFJet.partonFlavor;
        currJetIsBJet = (abs(currJetPartFlav) == 5 || abs(currJetPartFlav == 4)); 
        currJetValBTagDisc = vecGoodSmearPFJets->at(iJet).intPFJet.valBTagDisc;
        if (currJetIsBJet) {
            SystVar = BTagEffSystVar;
        }
        else {
            SystVar = BMisTagSFSystVar;
        }
        FastSimSystVar = (isSignal) ? SystVar : 0;
        if (inputBTagSFUtil->IsTagged(currJetValBTagDisc, currJetPartFlav, currJetPt, currJetEta, SystVar, FastSimSystVar, false)) {
            vecGoodSmearPFJets->at(iJet).intPFJet.isBJet = true;
            
            if (NBJets == 0) currBJetSumVec = vecGoodSmearPFJets->at(iJet).intPFJet.P4;
            else currBJetSumVec += vecGoodSmearPFJets->at(iJet).intPFJet.P4;
            ++NBJets;
            
            if (NBJets <= inEJI->numSavedBJets) {
                vecBJetIndex[NBJets - 1] = iJet;
                vecBJetSubIndex[NBJets - 1] = NJets - 1; 
            }
            
        }
        else {
            vecGoodSmearPFJets->at(iJet).intPFJet.isBJet = false;
        }
    }
    inEJI->EJISetValsInput(HT, &currJetSumVec, &currBJetSumVec, NJets, &vecJetIndex, NBJets, &vecBJetIndex, &vecBJetSubIndex, vecGoodSmearPFJets);
}

inline void VecJetHighPtResolutionTF1(vector<TF1> * inTF1Vec) {
    // Formula and parameter values for "sigma" taken from http://cmslxr.fnal.gov/lxr/source/CondFormats/JetMETObjects/data/Spring10_PtResolution_AK5PF.txt
    // sqrt(((TMath::Sign(1,[0])*sq([0]/x))+(sq([1])*(x^([3]-1))))+sq([2]))
    /*
     EtaMin EtaMax  NMinuitParams   FuncMin FuncMax Par0        Par1     Par2   Par3
     0      0.5     6               0.      99999.  -0.349206   0.297831 0      0.471121
     0.5    1       6               0.      99999.  -0.499735   0.336391 0      0.430689
     1      1.5     6               0.      99999.  -0.561649   0.420293 0      0.392398
     1.5    2       6               0.      99999.  -1.12329    0.657891 0      0.139595
     2      2.5     6               0.      99999.  1.04792     0.466763 0      0.193137
     2.5    3       6               0.      99999.  2.56933     0.305802 0      0.398929
     3      3.5     6               0.      99999.  2.81978     0.272373 0      0.579396
     3.5    4       6               0.      99999.  1.65966     0.223683 0      0.60873
     4      9.9     6               0.      99999.  1.41584     0.209477 0      0.588872
     */
    
    TString baseTF1Name = "SigmaResolTF1", TF1Name;
    TString function = "sqrt(((TMath::Sign(1,[0])*sq([0]/x))+(sq([1])*(x^([3]-1))))+sq([2]))";
    const int numTF1s = 9;
    float Par0[numTF1s] = {-0.349206, -0.499735, -0.561649, -1.12329, 1.04792, 2.56933, 2.81978, 1.65966, 1.41584};
    float Par1[numTF1s] = {0.297831, 0.336391, 0.420293, 0.657891, 0.466763, 0.305802, 0.272373, 0.223683, 0.209477};
    float Par2[numTF1s] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    float Par3[numTF1s] = {0.471121, 0.430689, 0.392398, 0.139595, 0.193137, 0.398929, 0.579396, 0.60873, 0.588872};
    for (int iFunc = 0; iFunc < numTF1s; ++iFunc) {
        TF1Name = baseTF1Name;
        TF1Name += "_EtaBin"; TF1Name += iFunc;
        TF1 currTF1(TF1Name, function, 0., 99999.);
        currTF1.SetParameter(0, Par0[iFunc]);
        currTF1.SetParameter(1, Par1[iFunc]);
        currTF1.SetParameter(2, Par2[iFunc]);
        currTF1.SetParameter(3, Par3[iFunc]);
        inTF1Vec->push_back(currTF1);
    }
}
inline TH2F * SmearHist(TString inputFileName, std::map<string, int> * inputHistSmearMap, vector<TH2F *> * inputVecMETSmearHists) {
    TH2F * outHist;
    for(map<string, int>::iterator xIter = inputHistSmearMap->begin(); xIter != inputHistSmearMap->end(); xIter++) {
//        cout << "inputFileName " << inputFileName << endl;
//        cout << "xIter->first " << xIter->first << endl;
        if (inputFileName.Contains(xIter->first)) {
            if ((unsigned int) xIter->second > inputVecMETSmearHists->size() - 1) {
                outHist = new TH2F;
                break;
            }
            cout << inputVecMETSmearHists->at(xIter->second)->GetName() << endl;
            outHist = inputVecMETSmearHists->at(xIter->second);
            break;
        }
        else {
            outHist = new TH2F;
        }
        // iterator->first = key
        // iterator->second = value
        // Repeat if you also want to iterate through the second map.
    }
    return outHist;
}
inline int setGenRWType(TString fInName, map<string, int> * inputGenRWMap) {
    int outType = -1;
    for(map<string, int>::iterator xIter = inputGenRWMap->begin(); xIter != inputGenRWMap->end(); xIter++) {
        if (fInName.Contains(xIter->first)) {
            outType = xIter->second;
        }
    }
    return outType;
}
inline void SetMETSmearHistMap(std::map<string, int> &inputHistSmearMap) {
    int TTBarIndex  = 0;
    int DYIndex     = 1;
    int WWIndex     = 2;
    int WZIndex     = 3;
    int ZZIndex     = 4;
    int WJIndex     = 5;
    int ZGIndex     = 6;
    int WGIndex     = 7;
    //Signal -- use TTBar for now...
    
    inputHistSmearMap["T2tt"] = TTBarIndex;
    inputHistSmearMap["T2bw"] = TTBarIndex;
    
    inputHistSmearMap["TTbar_Powheg"]           = TTBarIndex;
    inputHistSmearMap["TTbar_Madgraph"]         = TTBarIndex;
    inputHistSmearMap["TTbar_MCatNLO"]          = TTBarIndex;
    inputHistSmearMap["TTJetsFullHadrMG"]       = TTBarIndex;
    inputHistSmearMap["TTJets_MadSpin"]         = TTBarIndex;
    inputHistSmearMap["TTJetsSemiLeptMG"]       = TTBarIndex;
    inputHistSmearMap["TTJetsFullLeptMG"]       = TTBarIndex;
    inputHistSmearMap["TWDilep"]                = TTBarIndex;
    inputHistSmearMap["TbarWDilep"]             = TTBarIndex;
    inputHistSmearMap["TTWJets"]                = TTBarIndex;
    inputHistSmearMap["TTZJets"]                = TTBarIndex;
    inputHistSmearMap["TTWWJets"]               = TTBarIndex;
    inputHistSmearMap["TTGJets"]                = TTBarIndex;
    
    inputHistSmearMap["DYJets_Madgraph"]    = DYIndex;
    inputHistSmearMap["DY50toLLMad1jet"]    = DYIndex;
    inputHistSmearMap["DY50toLLMad2jet"]    = DYIndex;
    inputHistSmearMap["DY50toLLMad3jet"]    = DYIndex;
    inputHistSmearMap["DY50toLLMad4jet"]    = DYIndex;
    inputHistSmearMap["ZJets_Madgraph"]     = DYIndex;
    inputHistSmearMap["ZgammaToLLG"]        = DYIndex;
        
    inputHistSmearMap["SingleTop_Tchannel"]     = WJIndex;
    inputHistSmearMap["SingleTopBar_Tchannel"]  = WJIndex;
    inputHistSmearMap["SingleTop_Schannel"]     = WJIndex;
    inputHistSmearMap["SingleTopBar_Schannel"]  = WJIndex;
    inputHistSmearMap["WJets_Madgraph"]         = WJIndex;
    
    inputHistSmearMap["WGstarToElNuMad"]    = WGIndex;
    inputHistSmearMap["WGstarToMuNuMad"]    = WGIndex;
    inputHistSmearMap["WGstarToTauNuMad"]   = WGIndex;
    inputHistSmearMap["WgammaToLNuG"]       = WGIndex;        

    
    inputHistSmearMap["ZZ_"]                = ZZIndex;    
    inputHistSmearMap["ZZZJets"]            = ZZIndex;
    inputHistSmearMap["HZZ4L"]              = ZZIndex;    
    
    inputHistSmearMap["WZZJets"]            = WZIndex;
    inputHistSmearMap["WZ_"]                = WZIndex;

    inputHistSmearMap["WWincl"]             = WWIndex;
    inputHistSmearMap["ggWWto2L"]           = WWIndex;
    inputHistSmearMap["WWTo2L2Nu_"]         = WWIndex;
    inputHistSmearMap["HWW125"]             = WWIndex;
    inputHistSmearMap["VBF125"]             = WWIndex;
    inputHistSmearMap["WWGJets"]            = WWIndex;
    inputHistSmearMap["WWZJets"]            = WWIndex;
    inputHistSmearMap["WWWJets"]            = WWIndex;                                                                   
}



inline void SetGenRWMap(std::map<string, int> &inputGenRWMap) {
    int SUSYIndex   = 0;
    int TTBarIndex  = 1;
    int DYIndex     = 2;
    int WWIndex     = 3;
    int WZIndex     = 4;
    int ZZIndex     = 5;    
    //Signal -- use TTBar for now...
    
    inputGenRWMap["T2tt"] = SUSYIndex;
    inputGenRWMap["T2bw"] = SUSYIndex;
    
    inputGenRWMap["TTbar_Powheg"]           = TTBarIndex;
    inputGenRWMap["TTbar_Madgraph"]         = TTBarIndex;
    inputGenRWMap["TTbar_MCatNLO"]          = TTBarIndex;
    inputGenRWMap["TTJetsFullHadrMG"]       = TTBarIndex;
    inputGenRWMap["TTJets_MadSpin"]         = TTBarIndex;
    inputGenRWMap["TTJetsSemiLeptMG"]       = TTBarIndex;
    inputGenRWMap["TTJetsFullLeptMG"]       = TTBarIndex;
    inputGenRWMap["TTWJets"]                = TTBarIndex;
    inputGenRWMap["TTZJets"]                = TTBarIndex;
    inputGenRWMap["TTWWJets"]               = TTBarIndex;
    inputGenRWMap["TTGJets"]                = TTBarIndex;
    
    inputGenRWMap["DYJets_Madgraph"]    = DYIndex;
    inputGenRWMap["DY50toLLMad1jet"]    = DYIndex;
    inputGenRWMap["DY50toLLMad2jet"]    = DYIndex;
    inputGenRWMap["DY50toLLMad3jet"]    = DYIndex;
    inputGenRWMap["DY50toLLMad4jet"]    = DYIndex;
    inputGenRWMap["ZJets_Madgraph"]     = DYIndex;
       
    
    inputGenRWMap["ZZ_"]                = ZZIndex;    
    inputGenRWMap["ZZZJets"]            = ZZIndex;
    inputGenRWMap["HZZ4L"]              = ZZIndex;    
    
    inputGenRWMap["WZZJets"]            = WZIndex;
    inputGenRWMap["WZ_"]                = WZIndex;
    
    inputGenRWMap["WWincl"]             = WWIndex;
    inputGenRWMap["ggWWto2L"]           = WWIndex;
    inputGenRWMap["WWTo2L2Nu_"]         = WWIndex;
    inputGenRWMap["HWW125"]             = WWIndex;
    inputGenRWMap["VBF125"]             = WWIndex;
    inputGenRWMap["WWGJets"]            = WWIndex;
    inputGenRWMap["WWZJets"]            = WWIndex;
    inputGenRWMap["WWWJets"]            = WWIndex;                                                
}




inline vector<TH2F *> * SetMETSmearHistVec(int METorMETPhi, vector<TFile *> * inputVecMETSmearFiles, int whichSystCase = 0, int whichSystDirection = 0) {    
    TString METSmearString = "METSmearMinMET_vs_MET";
    TString METPhiSmearString = "METPhiSmearMinMETPhi_vs_MET";
    TString METSmearSystString[4] = {"METSmearJetERUpMinMET_vs_MET", "METSmearJetERDownMinMET_vs_MET", "METSmearUncESUpMinMET_vs_MET", "METSmearUncESDownMinMET_vs_MET"};
    TString METPhiSmearSystString[4] = {"METPhiSmearJetERUpMinMETPhi_vs_MET", "METPhiSmearJetERDownMinMETPhi_vs_MET", "METPhiSmearUncESUpMinMETPhi_vs_MET", "METPhiSmearUncESDownMinMETPhi_vs_MET"};
    TString TTBarHistString = "h_";
    int systGrabIndex = 0;
    if (whichSystCase > 0) {
        if (whichSystDirection == 0) cout << "error with Systs: they're turned on, but no direction has been specified " << whichSystDirection << endl;
        if (whichSystCase == 1) {
            systGrabIndex = whichSystDirection > 0 ? 0 : 1;
        }
        else if (whichSystCase == 2) {
            systGrabIndex = whichSystDirection > 0 ? 2 : 3;
        }
        TTBarHistString += METorMETPhi > 0 ? METSmearSystString[systGrabIndex] : METPhiSmearSystString[systGrabIndex];
    }
    else if (whichSystCase == 0) {
        TTBarHistString += METorMETPhi > 0 ? METSmearString : METPhiSmearString;
    }
    else {
        cout << "bug in whichSystCase! " << whichSystCase << endl;
    }
    TTBarHistString += "_inclusive";
    vector<TH2F *> * outVec = new vector<TH2F *>;
    TString OtherHistString = METorMETPhi > 0 ? "h_SmearMetMinUnsmearMet_vs_UnsmearMet" : "h_SmearMetPhiMinUnsmearMetPhi_vs_UnsmearMet";
    TString grabString;
    TH2F * currHist;
    for (unsigned int iFile = 0; iFile < inputVecMETSmearFiles->size(); ++iFile) {
        if (whichSystCase != 0 && iFile > 4) continue; // no JER or UncEnSmearing for nonTTBar for now
        grabString = iFile < 5 ? TTBarHistString : OtherHistString;
/*
        if (iFile == 1) {
            if (whichSystCase != 0) continue;
            grabString = OtherHistString;
        }
        */
        //        cout << "fileName " << inputVecMETSmearFiles->at(iFile)->GetName();
        currHist = (TH2F *) inputVecMETSmearFiles->at(iFile)->Get(grabString);
        outVec->push_back(currHist);

    }
    return outVec;
}

inline float DeltaMET(std::vector<TH1F *> * vecOneDeeHists, TH2F * TwoDeeHist, float inputMETValue, bool isMETPhi, float SF = 1.0) {
    //returns float which is a random number drawn from the distribution of MET (Phi) Smeared - MET (Phi) Unsmeared
    // Also works for the "Jet Smear systematics version"
    // the "isMETPhi" boolean turns on an additional random number that is +/- 1 (since dPhi for the METPhi p.d.f. is a magnitude thing)
    double sign = 1.0;
    
    if (isMETPhi) {
        TRandom3 rand(0);
        if (rand.Integer(2)) sign = -1;
//        cout << "sign " << sign << endl;
    }
    int whichOneDeeHist = TwoDeeHist->GetXaxis()->FindBin(inputMETValue) - 1;
    unsigned int numOneDeeHists = vecOneDeeHists->size();
    if (whichOneDeeHist < 0) {
        std::cout << inputMETValue << std::endl;
        std::cout << "ERROR with which one dee hist!" << std::endl;
        return 0.;
    }
    else if (whichOneDeeHist >= (int) numOneDeeHists) {
        whichOneDeeHist = (int) numOneDeeHists - 1;
    }
    if (vecOneDeeHists->at(whichOneDeeHist)->Integral() == 0) {
        return 0;
    }
    else {
        return SF * sign * vecOneDeeHists->at(whichOneDeeHist)->GetRandom();   
    }
}

inline void CorrectMETPhi(float &inputMETPhiValue) {
    // correct a MET Phi that's outside of -PI to PI to be within -PI to PI
    const float PI = 3.14159265;
    if (inputMETPhiValue < -1 * PI) {
        inputMETPhiValue += 2 * PI;
    }
    else if (inputMETPhiValue > 1 * PI) {
        inputMETPhiValue -= 2 * PI;
    }
}



inline vector<TH2F *> * SetMETXYSmearHistVec(int METXorMETY, vector<TFile *> * inputVecMETSmearFiles, int whichSystCase = 0, int whichSystDirection = 0) {
    TString METXSmearString = "METXSmearMinMETX_vs_MET";
    TString METYSmearString = "METYSmearMinMETY_vs_MET";
    TString METXSmearSystString[4] = {"METXSmearJetERUpMinMETX_vs_MET", "METXSmearJetERDownMinMETX_vs_MET", "METXSmearUncESUpMinMETX_vs_MET", "METXSmearUncESDownMinMETX_vs_MET"};
    TString METYSmearSystString[4] = {"METYSmearJetERUpMinMETY_vs_MET", "METYSmearJetERDownMinMETY_vs_MET", "METYSmearUncESUpMinMETY_vs_MET", "METYSmearUncESDownMinMETY_vs_MET"};
    TString TTBarHistString = "h_";
    int systGrabIndex = 0;
    if (whichSystCase > 0) {
        if (whichSystDirection == 0) cout << "error with Systs: they're turned on, but no direction has been specified " << whichSystDirection << endl;
        if (whichSystCase == 1) {
            systGrabIndex = whichSystDirection > 0 ? 0 : 1;
        }
        else if (whichSystCase == 2) {
            systGrabIndex = whichSystDirection > 0 ? 2 : 3;
        }
        TTBarHistString += METXorMETY > 0 ? METXSmearSystString[systGrabIndex] : METYSmearSystString[systGrabIndex];
    }
    else if (whichSystCase == 0) {
        TTBarHistString += METXorMETY > 0 ? METXSmearString : METYSmearString;
    }
    else {
        cout << "bug in whichSystCase! " << whichSystCase << endl;
    }
    TTBarHistString += "_inclusive";
    vector<TH2F *> * outVec = new vector<TH2F *>;
    TString OtherHistString = METXorMETY > 0 ? "h_SmearMetXMinUnsmearMetX_vs_UnsmearMet" : "h_SmearMetYMinUnsmearMetY_vs_UnsmearMet";
    TString grabString;
    TH2F * currHist;
    for (unsigned int iFile = 0; iFile < inputVecMETSmearFiles->size(); ++iFile) {
        if (whichSystCase != 0 && iFile > 4) continue; // no JER or UncEnSmearing for nonTTBar for now
        grabString = iFile < 5 ? TTBarHistString : OtherHistString;
        //        cout << "fileName " << inputVecMETSmearFiles->at(iFile)->GetName();
        currHist = (TH2F *) inputVecMETSmearFiles->at(iFile)->Get(grabString);
        outVec->push_back(currHist);        
    }
    return outVec;
}

inline float DeltaMETXY(std::vector<TH1F *> * vecOneDeeHists, TH2F * TwoDeeHist, float inputMETValue) {
    //returns float which is a random number drawn from the distribution of MET (Phi) Smeared - MET (Phi) Unsmeared
    // Also works for the "Jet Smear systematics version"
    // the "isMETPhi" boolean turns on an additional random number that is +/- 1 (since dPhi for the METPhi p.d.f. is a magnitude thing)
    int whichOneDeeHist = TwoDeeHist->GetXaxis()->FindBin(inputMETValue) - 1;
    unsigned int numOneDeeHists = vecOneDeeHists->size();
    if (whichOneDeeHist < 0) {
        std::cout << inputMETValue << std::endl;
        std::cout << "ERROR with which one dee hist!" << std::endl;
        return 0.;
    }
    else if (whichOneDeeHist >= (int) numOneDeeHists) {
        whichOneDeeHist = (int) numOneDeeHists - 1;
    }
    if (vecOneDeeHists->at(whichOneDeeHist)->Integral() == 0) {
        return 0;
    }
    else {
        return vecOneDeeHists->at(whichOneDeeHist)->GetRandom();   
    }
}




#endif //HistSampFunc_h_
