// Original Header file used for definition of functions that related to plots -- slowly being migrated into various other header files

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
//#include <Math/GenVector/PtEtaPhiM4D.h>
#include <vector>
//#include <Math/GenVector/LorentzVector.h>
//BTag shenanigans
//#include "./BTagSFFiles/BTagSFUtil.C"
#include "./BTagSFUtil.C"


#include <iostream>
#include <fstream>
//#include <vector>
#include <cmath>
#include <sstream>

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

//typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > LV;
//typedef std::vector<LV> VLV;

inline void AddPatsyName(vector<HistogramT> * vecHistT) {
    // function used to take on the "PATSY" into hist names so that I can find and replace it later with the sub-channel name
    for (unsigned int iHistT = 0; iHistT < vecHistT->size(); ++iHistT) {
        vecHistT->at(iHistT).name += "PATSY";
    }
}

inline vector<TH1F *> * OneDProjectionReturnVec(TH1 * inputHist, int numDims, int whichAxisToProjTo, int whichAxisForDist, int axisProjLB, int axisProjUB, TString nameBase) {
    TString name;
    vector<TH1F *> * outHistVector = new vector<TH1F *>;
    TH1F * projHist;
    TH2F * projPatsy2DHist;
    TH3F * projPatsy3DHist;
    TString patsyNamebase = "patsy";
    TString patsyName;
    int NBins;
    int whichAxisToMix = 6 - (whichAxisForDist + whichAxisToProjTo);
    int axis1LB, axis1UB, axis2LB, axis2UB;
    switch (whichAxisForDist) {
        case 1:
            NBins = inputHist->GetNbinsX();
            break;
        case 2:
            NBins = inputHist->GetNbinsY();
            break;
        case 3:
            NBins = inputHist->GetNbinsZ();
            break;
        default:
            break;
    }    
    for (int ib = 1; ib <= NBins; ++ib) {
        axis1LB = (whichAxisToMix > whichAxisForDist) ? ib : axisProjLB;
        axis1UB = (whichAxisToMix > whichAxisForDist) ? ib : axisProjUB;
        axis2LB = (whichAxisToMix > whichAxisForDist) ? axisProjLB : ib;
        axis2UB = (whichAxisToMix > whichAxisForDist) ? axisProjUB : ib;
        name = nameBase;
        name += ib;
        patsyName = patsyNamebase;
        patsyName += name;
        switch (whichAxisToProjTo) {
            case 1:
                if (numDims > 2) {
                    projPatsy3DHist = (TH3F *) inputHist->Clone(patsyName);
                    projHist = (TH1F *) projPatsy3DHist->ProjectionX(name, axis1LB, axis1UB, axis2LB, axis2UB);   
                }
                else {
                    projPatsy2DHist = (TH2F *) inputHist->Clone(patsyName);
                    
                    projHist = (TH1F *) projPatsy2DHist->ProjectionX(name, axis1LB, axis1UB);   
                }
                break;
            case 2:
                if (numDims > 2) {
                    projPatsy3DHist = (TH3F *) inputHist->Clone(patsyName);
                    projHist = (TH1F *) projPatsy3DHist->ProjectionY(name, axis1LB, axis1UB, axis2LB, axis2UB);   
                }
                else {
                    projPatsy2DHist = (TH2F *) inputHist->Clone(patsyName);
                    
                    projHist = (TH1F *) projPatsy2DHist->ProjectionY(name, axis1LB, axis1UB);   
                }
                break;
            case 3:
                if (numDims > 2) {
                    projPatsy3DHist = (TH3F *) inputHist->Clone(patsyName);
                    projHist = (TH1F *) projPatsy3DHist->ProjectionZ(name, axis1LB, axis1UB, axis2LB, axis2UB);   
                }
                break;
            default:
                break;
        }
        outHistVector->push_back(projHist);
    }
    return outHistVector;
}

#include "./StopHistTDefinitions_1D.h"
#include "./StopHistTDefinitions_2D.h"
#include "./StopHistTDefinitions_3D.h"
/*
 #include "./StopHistTDefinitions/StopHistTDefinitions_1D.h"
 #include "./StopHistTDefinitions/StopHistTDefinitions_2D.h"
 #include "./StopHistTDefinitions/StopHistTDefinitions_3D.h"
 */

inline void SetHistTVec_Inclusive(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, int numDims, int cutPlotLevel) {
    bool noSmear = false;
    
    switch (numDims) {
        case 1:
            OneDeeHistTVec_Inclusive(inHistTVec, inSHBB, mapVartoLabel, cutPlotLevel);
            break;
        case 2:
            TwoDeeHistTVec_Inclusive(inHistTVec, inSHBB, mapVartoLabel, cutPlotLevel);            
            break;
        case 3:
            ThreeDeeHistTVec_Inclusive(inHistTVec, inSHBB, mapVartoLabel, cutPlotLevel);
            break;
        default:
            cout << "num of Dims should be 1, 2, or 3 -- it is " << numDims << endl;
            break;
    }
}
inline void SetHistTVec_Inclusive_Smeared(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, int numDims, int cutPlotLevel) {
    bool noSmear = false;
    
    switch (numDims) {
        case 1:
            OneDeeHistTVec_Inclusive_Smeared(inHistTVec, inSHBB, mapVartoLabel, cutPlotLevel);
            break;
        case 2:
            TwoDeeHistTVec_Inclusive_Smeared(inHistTVec, inSHBB, mapVartoLabel, cutPlotLevel);            
            break;
        case 3:
            ThreeDeeHistTVec_Inclusive_Smeared(inHistTVec, inSHBB, mapVartoLabel, cutPlotLevel);
            break;
        default:
            cout << "num of Dims should be 1, 2, or 3 -- it is " << numDims << endl;
            break;
    }
}
inline void AddSystHists(vector<HistogramT> * outputHistTVec, vector<HistogramT> * inputHistTVec, vector<SystT> * inputSystTVec, bool isData, bool isSignal, bool doVerbosity = false) {
    TString currSystName, currHistName;
    HistogramT currHistT;
    for (unsigned int iHist = 0; iHist < inputHistTVec->size(); ++iHist) {        
        if (inputHistTVec->at(iHist).xAxis.doAxisSyst || inputHistTVec->at(iHist).yAxis.doAxisSyst || inputHistTVec->at(iHist).zAxis.doAxisSyst) {
            currHistName = inputHistTVec->at(iHist).name;
            for (unsigned int iSyst = 0; iSyst < inputSystTVec->size(); ++iSyst) {
                currSystName = inputSystTVec->at(iSyst).name;
                //11/6/14 -- commented this so that we can include smearing systematics on any MET cuts we do
                //11/10/14 -- decommented this because it doesn't work
                if ((currSystName.Contains("Smear") || currSystName.Contains("UncES")) && !(currHistName.Contains("Smear"))) continue;
                if (currSystName.Contains("genStop") && !isSignal) continue;
                if (currSystName.Contains("Fake") && !isData) {
                    continue;
                }
                else if (!currSystName.Contains("Fake") && isData) {
                    continue;
                }
                currHistT = inputHistTVec->at(iHist);
                if (doVerbosity) {
                    cout << "in adding systematics, currHistT name is " << currHistT.name << endl;
                }
                currHistT.name += currSystName;
                if (doVerbosity) {
                    cout << "in adding systematics, currHistT name after adding systematics is " << currHistT.name << endl;
                }
                /*
                if (inputHistTVec->at(iHist).xAxis.doAxisSyst) currHistT.xAxis.axisVarKey += inputSystTVec->at(iSyst).systVarKey;
                if (inputHistTVec->at(iHist).yAxis.doAxisSyst) currHistT.yAxis.axisVarKey += inputSystTVec->at(iSyst).systVarKey;
                if (inputHistTVec->at(iHist).zAxis.doAxisSyst) currHistT.zAxis.axisVarKey += inputSystTVec->at(iSyst).systVarKey; 
                */
                outputHistTVec->push_back(currHistT);               
            }
        }
    }
}

inline void SetSubSampVec(vector<SampleT> * subSampVec, int whichSubSampType = -1) {
    SampCutInt NJetsGeq2; NJetsGeq2.SetParamsBound("_NJetsGeq2", 2, 1);
    SampCutInt NBJetsGeq1; NBJetsGeq1.SetParamsBound("_NBJetsGeq2", 1, 1);
    
    SampCut METGeq20; METGeq20.SetParamsBound("_METGeq20", 20, 1);
    SampCut METGeq40; METGeq40.SetParamsBound("_METGeq40", 40, 1);
    SampCut METGeq60; METGeq60.SetParamsBound("_METGeq60", 60, 1);
    
    SampCutInt DiLepInZMass; DiLepInZMass.SetParamsBound("_InZMass", 1, 1);
    /*######Define Subchannels###########*/
    /*
    TString lepNameSuffix[3] = {"_mumu", "_ee", "_emu"};
    TString lepHistXAxisSuffix[3] = {"_{#mu#mu}", "_{ee}", "_{e#mu}"};
    */
    TString ZVetoString = "_ZVeto";
    TString METCutString = "_METGeq40";
    TString JetCutString = "_Jet2";
    TString BJetCutString = "_BJet1";
    
    TString stringZVeto[2] = {"_inZ", "_outZ"};
    TString stringJetCut[5] = {"", "_Jet2", "_Jet2_METGeq40", "_Jet2_BJet1", "_Jet2_BJet1_METGeq40"};
    
    int cutJet[5] = {-1, 2, 2, 2, 2};
    int cutBJet[5] = {-1, -1, -1, 1, 1};
    int cutMETVals[5] = {0, 0, 40, 0, 40};
    
    vector< vector< SampleT > > vecSampT_ZSF; vecSampT_ZSF.resize(2);
    
    for (unsigned int iDiLepMass = 0; iDiLepMass < 2; ++iDiLepMass) {
        vecSampT_ZSF[iDiLepMass].resize(5);
        for (unsigned int iJetMET = 0; iJetMET < 5; ++iJetMET) {
            vecSampT_ZSF[iDiLepMass][iJetMET].DefaultVarVals();
            
            vecSampT_ZSF[iDiLepMass][iJetMET].doZVeto = iDiLepMass;
            vecSampT_ZSF[iDiLepMass][iJetMET].cutNJets = cutJet[iJetMET];
            vecSampT_ZSF[iDiLepMass][iJetMET].cutNBJets = cutBJet[iJetMET];
            vecSampT_ZSF[iDiLepMass][iJetMET].cutMET = cutMETVals[iJetMET];
            
            vecSampT_ZSF[iDiLepMass][iJetMET].histNameSuffix = stringZVeto[iDiLepMass];
            vecSampT_ZSF[iDiLepMass][iJetMET].histNameSuffix += stringJetCut[iJetMET];
            
            if (whichSubSampType < 2) {
                subSampVec->push_back(vecSampT_ZSF[iDiLepMass][iJetMET]);
            }
        }
    }            
    SampleT events_outZ_ZCR; events_outZ_ZCR.histNameSuffix = "_outZ_ZCR"; events_outZ_ZCR.histXaxisSuffix = "";
    events_outZ_ZCR.histYaxisSuffix = ""; events_outZ_ZCR.histZaxisSuffix = "";
    events_outZ_ZCR.whichdiLepType = -1; events_outZ_ZCR.doZVeto = 1; events_outZ_ZCR.cutNJets = 2; events_outZ_ZCR.cutNBJets = -1; events_outZ_ZCR.cutMET = 40.;
    events_outZ_ZCR.blindDataChannel = 1;
    
    SampleT events_inZ_ZCR; events_inZ_ZCR.histNameSuffix = "_inZ_ZCR"; events_inZ_ZCR.histXaxisSuffix = "";
    events_inZ_ZCR.histYaxisSuffix = ""; events_inZ_ZCR.histZaxisSuffix = "";
    events_inZ_ZCR.whichdiLepType = -1; events_inZ_ZCR.doZVeto = 0; events_inZ_ZCR.cutNJets = 2; events_inZ_ZCR.cutNBJets = -1; events_inZ_ZCR.cutMET = 40.;
    events_inZ_ZCR.blindDataChannel = 1;
    
    SampleT events_inZ_ZCR_LowBLepMass; events_inZ_ZCR_LowBLepMass.histNameSuffix = "_inZ_ZCR_LowBLepMass"; events_inZ_ZCR_LowBLepMass.histXaxisSuffix = "";
    events_inZ_ZCR_LowBLepMass.histYaxisSuffix = ""; events_inZ_ZCR_LowBLepMass.histZaxisSuffix = "";
    events_inZ_ZCR_LowBLepMass.whichdiLepType = -1; events_inZ_ZCR_LowBLepMass.doZVeto = 0; events_inZ_ZCR_LowBLepMass.cutNJets = 2; events_inZ_ZCR_LowBLepMass.cutNBJets = -1; events_inZ_ZCR_LowBLepMass.cutMET = 40.;
    events_inZ_ZCR_LowBLepMass.blindDataChannel = 1;
    
    SampleT events_FullCut; events_FullCut.histNameSuffix = "_FullCut"; events_FullCut.histXaxisSuffix = ""; 
    events_FullCut.histYaxisSuffix = ""; events_FullCut.histZaxisSuffix = "";
    events_FullCut.whichdiLepType = -1; events_FullCut.doZVeto = 1; events_FullCut.cutNJets = 2; events_FullCut.cutNBJets = 1; events_FullCut.cutMET = 40.;
    events_FullCut.blindDataChannel = 1;
    
    SampleT events_FullCut_LowBLepMass; events_FullCut_LowBLepMass.histNameSuffix = "_FullCut_LowBLepMass"; events_FullCut_LowBLepMass.histXaxisSuffix = "";
    events_FullCut_LowBLepMass.histYaxisSuffix = ""; events_FullCut_LowBLepMass.histZaxisSuffix = "";
    events_FullCut_LowBLepMass.whichdiLepType = -1; events_FullCut_LowBLepMass.doZVeto = 1; events_FullCut_LowBLepMass.cutNJets = 2; events_FullCut_LowBLepMass.cutNBJets = 1; events_FullCut_LowBLepMass.cutMET = 40.;
    events_FullCut_LowBLepMass.blindDataChannel = 1;
    
    SampleT events_FullCutTTBarControl; events_FullCutTTBarControl.histNameSuffix = "_FullCutTTBarControl"; events_FullCutTTBarControl.histXaxisSuffix = "";
    events_FullCutTTBarControl.histYaxisSuffix = ""; events_FullCutTTBarControl.histZaxisSuffix = "";
    events_FullCutTTBarControl.whichdiLepType = -1; events_FullCutTTBarControl.doZVeto = 1; events_FullCutTTBarControl.cutNJets = 2; events_FullCutTTBarControl.cutNBJets = 1; events_FullCutTTBarControl.cutMET = 40.;
    events_FullCutTTBarControl.blindDataChannel = 1;
    
    SampleT events_FullCutSignal; events_FullCutSignal.histNameSuffix = "_FullCutSignal"; events_FullCutSignal.histXaxisSuffix = "";
    events_FullCutSignal.histYaxisSuffix = ""; events_FullCutSignal.histZaxisSuffix = "";
    events_FullCutSignal.whichdiLepType = -1; events_FullCutSignal.doZVeto = 1; events_FullCutSignal.cutNJets = 2; events_FullCutSignal.cutNBJets = 1; events_FullCutSignal.cutMET = 40.;
    events_FullCutSignal.blindDataChannel = 1;
    
    SampleT events_FullCutMETSig1D; events_FullCutMETSig1D.histNameSuffix = "_FullCutMETSig1D"; events_FullCutMETSig1D.histXaxisSuffix = "";
    events_FullCutMETSig1D.histYaxisSuffix = ""; events_FullCutMETSig1D.histZaxisSuffix = "";
    events_FullCutMETSig1D.whichdiLepType = -1; events_FullCutMETSig1D.doZVeto = 1; events_FullCutMETSig1D.cutNJets = 2; events_FullCutMETSig1D.cutNBJets = 1; events_FullCutMETSig1D.cutMET = 0.;
    events_FullCutMETSig1D.blindDataChannel = 1;
    
    SampleT events_FullCutMETSig2D; events_FullCutMETSig2D.histNameSuffix = "_FullCutMETSig2D"; events_FullCutMETSig2D.histXaxisSuffix = ""; 
    events_FullCutMETSig2D.histYaxisSuffix = ""; events_FullCutMETSig2D.histZaxisSuffix = "";
    events_FullCutMETSig2D.whichdiLepType = -1; events_FullCutMETSig2D.doZVeto = 1; events_FullCutMETSig2D.cutNJets = 2; events_FullCutMETSig2D.cutNBJets = 1; events_FullCutMETSig2D.cutMET = 0.;
    events_FullCutMETSig2D.blindDataChannel = 1;
    
    SampleT events_FullCutMETSigTrue; events_FullCutMETSigTrue.histNameSuffix = "_FullCutMETSigTrue"; events_FullCutMETSigTrue.histXaxisSuffix = ""; 
    events_FullCutMETSigTrue.histYaxisSuffix = ""; events_FullCutMETSigTrue.histZaxisSuffix = "";
    events_FullCutMETSigTrue.whichdiLepType = -1; events_FullCutMETSigTrue.doZVeto = 1; events_FullCutMETSigTrue.cutNJets = 2; events_FullCutMETSigTrue.cutNBJets = 1; events_FullCutMETSigTrue.cutMET = 0.;
    events_FullCutMETSigTrue.blindDataChannel = 1;
    
    SampleT events_FullCutOnly2Leps; events_FullCutOnly2Leps.histNameSuffix = "_FullCutOnly2Leps"; events_FullCutOnly2Leps.histXaxisSuffix = ""; 
    events_FullCutOnly2Leps.histYaxisSuffix = ""; events_FullCutOnly2Leps.histZaxisSuffix = "";
    events_FullCutOnly2Leps.whichdiLepType = -1; events_FullCutOnly2Leps.doZVeto = 1; events_FullCutOnly2Leps.cutNJets = 2; events_FullCutOnly2Leps.cutNBJets = 1; events_FullCutOnly2Leps.cutMET = 40.;
    events_FullCutOnly2Leps.blindDataChannel = 1;
    
    SampleT events_FullCutISR_100; events_FullCutISR_100.histNameSuffix = "_FullCutISR_100"; events_FullCutISR_100.histXaxisSuffix = ""; 
    events_FullCutISR_100.histYaxisSuffix = ""; events_FullCutISR_100.histZaxisSuffix = "";
    events_FullCutISR_100.whichdiLepType = -1; events_FullCutISR_100.doZVeto = 1; events_FullCutISR_100.cutNJets = 3; events_FullCutISR_100.cutNBJets = 1; events_FullCutISR_100.cutMET = 40.;
    events_FullCutISR_100.blindDataChannel = 1;
    
    SampleT events_FullCutISR_200; events_FullCutISR_200.histNameSuffix = "_FullCutISR_200"; events_FullCutISR_200.histXaxisSuffix = ""; 
    events_FullCutISR_200.histYaxisSuffix = ""; events_FullCutISR_200.histZaxisSuffix = "";
    events_FullCutISR_200.whichdiLepType = -1; events_FullCutISR_200.doZVeto = 1; events_FullCutISR_200.cutNJets = 3; events_FullCutISR_200.cutNBJets = 1; events_FullCutISR_200.cutMET = 40.;
    events_FullCutISR_200.blindDataChannel = 1;
    
    //"Inclusive subsample"//
    SampleT allEvents; allEvents.DefaultVarVals();
    allEvents.histNameSuffix = "_inclusive"; 
   
    SampleT events_FullCutISR_100Only2Leps; events_FullCutISR_100Only2Leps.histNameSuffix = "_FullCutISR_100Only2Leps"; events_FullCutISR_100Only2Leps.histXaxisSuffix = ""; 
    events_FullCutISR_100Only2Leps.histYaxisSuffix = ""; events_FullCutISR_100Only2Leps.histZaxisSuffix = "";
    events_FullCutISR_100Only2Leps.whichdiLepType = -1; events_FullCutISR_100Only2Leps.doZVeto = 1; events_FullCutISR_100Only2Leps.cutNJets = 3; events_FullCutISR_100Only2Leps.cutNBJets = 1; events_FullCutISR_100Only2Leps.cutMET = 40.;
    events_FullCutISR_100Only2Leps.blindDataChannel = 1;
    
    SampleT events_FullCutISR_200Only2Leps; events_FullCutISR_200Only2Leps.histNameSuffix = "_FullCutISR_200Only2Leps"; events_FullCutISR_200Only2Leps.histXaxisSuffix = ""; 
    events_FullCutISR_200Only2Leps.histYaxisSuffix = ""; events_FullCutISR_200Only2Leps.histZaxisSuffix = "";
    events_FullCutISR_200Only2Leps.whichdiLepType = -1; events_FullCutISR_200Only2Leps.doZVeto = 1; events_FullCutISR_200Only2Leps.cutNJets = 3; events_FullCutISR_200Only2Leps.cutNBJets = 1; events_FullCutISR_200Only2Leps.cutMET = 40.;
    events_FullCutISR_200Only2Leps.blindDataChannel = 1;
    
    if (whichSubSampType < 2) {
        subSampVec->push_back(events_outZ_ZCR);
        subSampVec->push_back(events_inZ_ZCR);
        
        subSampVec->push_back(events_inZ_ZCR_LowBLepMass);
    }
    subSampVec->push_back(events_FullCut);
    subSampVec->push_back(events_FullCut_LowBLepMass);
    subSampVec->push_back(events_FullCutTTBarControl);
    subSampVec->push_back(events_FullCutSignal);
    if (whichSubSampType < 2) {
        subSampVec->push_back(events_FullCutMETSig1D);
        subSampVec->push_back(events_FullCutMETSig2D);
        subSampVec->push_back(events_FullCutMETSigTrue);
        subSampVec->push_back(events_FullCutOnly2Leps);
        subSampVec->push_back(events_FullCutISR_100); subSampVec->push_back(events_FullCutISR_200);
        subSampVec->push_back(events_FullCutISR_100Only2Leps); subSampVec->push_back(events_FullCutISR_200Only2Leps);
    }
    subSampVec->push_back(allEvents);
}

inline void SetSystVec(vector<SystT> * inVecSystT, bool includeJetSmear, bool doFakeLep = false) {
    // int whichSystType is used to demarcate between different systematics, with positive values designating a "shift up" systematic and negative values designating a "shift down" systematic -- note that systematics with no "shift" to them, i.e. GenTopRW which is unidirectional, are marked with a positive shift type by default
    // whichSystType number meanings:
    // MC based systematics
    // 1: Lepton Energy Scale
    // 2: Jet Energy Scale
    // 3: B RealTag Eff SF Shift
    // 4: B MisTag Eff SF Shift
    // 5: Jet Resolution (Smearing SF) Shift
    // 6: Unclustered Energy Scale
    // 7: Lepton Efficiency SF Shift
    // 8: Gen Recoil RW
    // 9: Stop XSec Shift
    
    // DD based systematics
    // 1: FakeEst Shift
    vector<TString> vecSystNames;
    vector<int> vecSystType;
    
    vecSystNames.push_back("");
    vecSystType.push_back(0);
    if (!doFakeLep) {
        vecSystNames.push_back("LepES");
        vecSystType.push_back(1);
        vecSystNames.push_back("JetES");
        vecSystType.push_back(2);
        vecSystNames.push_back("BTagEffSF");
        vecSystType.push_back(3);
        vecSystNames.push_back("BMisTagSF");
        vecSystType.push_back(4);
        if (includeJetSmear) {
            vecSystNames.push_back("JetSmear");
            vecSystType.push_back(5);
            vecSystNames.push_back("UncES");
            vecSystType.push_back(6);
        }
        vecSystNames.push_back("LepEffSF");
        vecSystType.push_back(7);
        vecSystNames.push_back("genRecoilRW");
        vecSystType.push_back(8);
    }
    else {
        vecSystNames.push_back("FakeLepStat");
        vecSystType.push_back(1);
        vecSystNames.push_back("FakeLepFakeRateSyst");
        vecSystType.push_back(2);
        vecSystNames.push_back("FakeLepPromptRateSyst");
        vecSystType.push_back(3);
    }
    /*
    vecSystNames.push_back("genStopXSec");
    vecSystType.push_back(9);
    */
    //cout << "test " << endl;
    inVecSystT->resize(0);
    //cout << "test 2" << endl;
    SystT currSystT;
    for (unsigned int iSyst = 0; iSyst < vecSystNames.size() - 1; ++iSyst) {
        //cout << "iSyst " << iSyst << endl;
        currSystT.SetParams(&vecSystNames, &vecSystType, iSyst + 1);
        inVecSystT->push_back(currSystT);
        currSystT.SetParams(&vecSystNames, &vecSystType, -1 * (iSyst + 1));
        inVecSystT->push_back(currSystT);
    }
    //cout << "done " << endl;
}

inline void SetSystVec_PDF(vector<SystT> * inVecSystT) {
    // Sets the input hist vector to have the associated PDF uncertainties
    // for the three PDF sets
    // CT10 -- 25 respective shift up and shift down weights
    // MSTW -- 23 respective shift up and shift down weights
    // NNPDF -- 50 respective shift up and shift down weights
    // Note all CT10 systs will have SystType 101##
    // Note all systs will have SystType 102##
    // Note all systs will have SystType 103##
    
    int baseSystTypeCT10 = 10100;
    int baseSystTypeMSTW = 10200;
    int baseSystTypeNNPDF = 10300;
    
    
    int numCT10 = 25;
    int numMSTW = 23;
    int numNNPDF = 50;
    
    vector<TString> vecSystNames_CT10;
    vector<TString> vecSystNames_MSTW;
    vector<TString> vecSystNames_NNPDF;
    
    vector<int> vecSystType_CT10;
    vector<int> vecSystType_MSTW;
    vector<int> vecSystType_NNPDF;
    
    TString baseNameCT10 = "CT10EV";
    TString baseNameMSTW = "MSTWEV";
    TString baseNameNNPDF = "NNPDFEV";
    
    vecSystNames_CT10.push_back("");
    vecSystType_CT10.push_back(0);
    
    vecSystNames_MSTW.push_back("");
    vecSystType_MSTW.push_back(0);

    vecSystNames_NNPDF.push_back("");
    vecSystType_NNPDF.push_back(0);

    
    TString currSystName;
    
    for (int iSystPDF_CT10 = 1; iSystPDF_CT10 <= numCT10; ++iSystPDF_CT10) {
        currSystName = baseNameCT10;
        currSystName += iSystPDF_CT10;
        vecSystNames_CT10.push_back(currSystName);
        vecSystType_CT10.push_back(baseSystTypeCT10 + iSystPDF_CT10);
    }
    
    for (int iSystPDF_MSTW = 1; iSystPDF_MSTW <= numMSTW; ++iSystPDF_MSTW) {
        currSystName = baseNameMSTW;
        currSystName += iSystPDF_MSTW;
        vecSystNames_MSTW.push_back(currSystName);
        vecSystType_MSTW.push_back(baseSystTypeMSTW + iSystPDF_MSTW);
    }
    
    for (int iSystPDF_NNPDF = 1; iSystPDF_NNPDF <= numNNPDF; ++iSystPDF_NNPDF) {
        currSystName = baseNameNNPDF;
        currSystName += iSystPDF_NNPDF;
        vecSystNames_NNPDF.push_back(currSystName);
        vecSystType_NNPDF.push_back(baseSystTypeNNPDF + iSystPDF_NNPDF);
    }
    
    
    inVecSystT->resize(0);
    SystT currSystT;
    
    
    vector<TString> vecSystNames_MainSysts;
    vector<int> vecSystType_MainSysts;
    
    vecSystNames_MainSysts.push_back("");
    vecSystType_MainSysts.push_back(0);
    vecSystNames_MainSysts.push_back("genRecoilRW");
    vecSystType_MainSysts.push_back(8);
    
    for (unsigned int iSystMain = 0; iSystMain < vecSystNames_MainSysts.size() - 1; ++iSystMain) {
        //cout << "iSyst " << iSyst << endl;
        currSystT.SetParams(&vecSystNames_MainSysts, &vecSystType_MainSysts, iSystMain + 1);
        inVecSystT->push_back(currSystT);
        currSystT.SetParams(&vecSystNames_MainSysts, &vecSystType_MainSysts, -1 * (iSystMain + 1));
        inVecSystT->push_back(currSystT);
    }
    
    
    for (unsigned int iSystPDF_CT10 = 0; iSystPDF_CT10 < vecSystNames_CT10.size() - 1; ++iSystPDF_CT10) {
        //cout << "iSyst " << iSyst << endl;
        currSystT.SetParams(&vecSystNames_CT10, &vecSystType_CT10, iSystPDF_CT10 + 1);
        inVecSystT->push_back(currSystT);
        currSystT.SetParams(&vecSystNames_CT10, &vecSystType_CT10, -1 * (iSystPDF_CT10 + 1));
        inVecSystT->push_back(currSystT);
    }
    
    for (unsigned int iSystPDF_MSTW = 0; iSystPDF_MSTW < vecSystNames_MSTW.size() - 1; ++iSystPDF_MSTW) {
        //cout << "iSyst " << iSyst << endl;
        currSystT.SetParams(&vecSystNames_MSTW, &vecSystType_MSTW, iSystPDF_MSTW + 1);
        inVecSystT->push_back(currSystT);
        currSystT.SetParams(&vecSystNames_MSTW, &vecSystType_MSTW, -1 * (iSystPDF_MSTW + 1));
        inVecSystT->push_back(currSystT);
    }
    
    for (unsigned int iSystPDF_NNPDF = 0; iSystPDF_NNPDF < vecSystNames_NNPDF.size() - 1; ++iSystPDF_NNPDF) {
        //cout << "iSyst " << iSyst << endl;
        currSystT.SetParams(&vecSystNames_NNPDF, &vecSystType_NNPDF, iSystPDF_NNPDF + 1);
        inVecSystT->push_back(currSystT);
        currSystT.SetParams(&vecSystNames_NNPDF, &vecSystType_NNPDF, -1 * (iSystPDF_NNPDF + 1));
        inVecSystT->push_back(currSystT);
    }
}


inline TString DescriptorString(SampleT inputSubSamp) {
    //descriptor strings
    TString baseDescString[4] = {"All three flavors of lepton events, inclusive other than being/requiring ",  "All mumu events, inclusive other than being/requiring", "All ee events, inclusive other than being/requiring", "All emu events, inclusive other than being/requiring"};
    TString dsZVeto = ", diLepton system invariant mass outside of Z Mass window (76 GeV:106 GeV)";
    TString dsAntiZVeto = ", diLepton system invariant mass inside of Z Mass window (76 GeV:106 GeV) ";
    TString dsJetCutStringP1 = ", at least ";
    TString dsJetCutStringP2 = " jet(s)";
    TString dsBJetCutStringP2 = " b jet(s)";
    TString dsMETCutStringP1 = ", at least ";
    TString dsMETCutStringP2 = " GeV of MET";
    TString etaCutString[3] = {", both leptons in endcap eta range", ", one of the two leptons in barrel eta range", ", both leptons in barrel eta range"};
    //descriptor strings   
    TString outString;
    outString += baseDescString[inputSubSamp.whichdiLepType + 1];
    switch (inputSubSamp.doZVeto) {
        case 0:
            outString += dsAntiZVeto;
            break;
        case 1:
            outString += dsZVeto;
            break;            
        default:
            break;
    }
    if (inputSubSamp.cutNJets > 0) {
        outString += dsJetCutStringP1;
        outString += inputSubSamp.cutNJets;
        outString += dsJetCutStringP2;
    }
    if (inputSubSamp.cutNBJets > 0) {
        outString += dsJetCutStringP1;
        outString += inputSubSamp.cutNBJets;
        outString += dsBJetCutStringP2;
    }
    if (inputSubSamp.cutMET > 0) {
        outString += dsMETCutStringP1;
        outString += inputSubSamp.cutMET;
        outString += dsMETCutStringP2;
    }
    if (inputSubSamp.histNameSuffix.Contains("BothinEndcap")) outString += etaCutString[0];
    if (inputSubSamp.histNameSuffix.Contains("OneinBarrel")) outString += etaCutString[1];
    if (inputSubSamp.histNameSuffix.Contains("BothinBarrel")) outString += etaCutString[2];
    if (inputSubSamp.histNameSuffix.Contains("0BJets")) outString += TString(", with NO BJets");
    if (inputSubSamp.blindDataChannel) outString += TString(", note blinded channel");
    return outString;
}
inline vector<SpecHistBinT> * SpecHistBinVec() {
    SpecHistBinT ZMass; 
    ZMass.HistTXLabel = "M_{ll}";
    ZMass.SampTLabel = "inZ";
    vector<SpecHistBinT> * specHistBinVec = new vector<SpecHistBinT>;
    specHistBinVec->push_back(ZMass); 
    return specHistBinVec;
}


inline TString HistProjection1D(vector<TH2F *> * inputHistVec, vector<TH1F *> * outputHistVec, TString plotName, int whichCase) {
    const double PI = 3.14159265;
    TString outString = "Nothin";
    TH1F * currOutHist;
    TH2F * currInHist;
//    TAxis * XAxis;
    TAxis * YAxis;
//    int XAxisLB, XAxisUB, 
    int YAxisLB, YAxisUB;
    TString outHistName;
    TString outHistPlusName;
    double YAxisLBFind, YAxisUBFind;
    if (plotName.Contains("h_MT2ll_vs_DeltaPhiLep0Lep1")) {
        switch (whichCase) {
            case 0: // plot for DeltaPhiZMET > 2./3. Pi
                outHistPlusName = "_XAxis_DPhiGt2/3_";
                YAxisLBFind = 2./3. * PI;
                YAxisUBFind = PI;
                outString = "#Delta #phi_{ll} #in {2/3#pi:#pi}";
                break;
            case 1: // plot for 1./3. Pi < DeltaPhiZMET < 2/3 Pi
                outHistPlusName = "_XAxis_DPhiGt1/3Lt2/3_";
                YAxisLBFind = 1./3. * PI;
                YAxisUBFind = 2./3. * PI;
                outString = "#Delta #phi_{ll} #in {1/3#pi:2/3#pi}";
                break;   
            case 2: // plot for DeltaPhiZMET < 1/3 Pi
                outHistPlusName = "_XAxis_DPhiLt1/3_";
                YAxisLBFind = 0.;
                YAxisUBFind = 1./3. * PI;
                outString = "#Delta #phi_{ll} #in {0:1/3#pi}";
                break; 
            default:
                break;
        }
    }
    if (outString.Contains("Nothin")) return outString;
    for (unsigned int iHist = 0; iHist < inputHistVec->size(); ++iHist) {
        currInHist = (TH2F*) inputHistVec->at(iHist);
        outHistName = plotName;
        outHistName += outHistPlusName;
        outHistName += iHist;
        YAxis = currInHist->GetYaxis();
        YAxisLB = YAxis->FindBin(YAxisLBFind);
        YAxisUB = YAxis->FindBin(YAxisUBFind);
        cout << "case " << whichCase << endl;
        cout << "YAxisLBFind " << YAxisLBFind << endl;
        cout << "YAxisUBFind " << YAxisUBFind << endl;
        cout << "YAxisLB " << YAxisLB << endl;
        cout << "YAxisUB " << YAxisUB << endl;
        currOutHist = (TH1F*) currInHist->ProjectionX(outHistName, YAxisLB, YAxisUB, "e");
        outputHistVec->push_back(currOutHist);        
    }
    return outString;
}

inline TString HistProjection1D(vector<TH3F *> * inputHistVec, vector<TH1F *> * outputHistVec, TString plotName, int whichCase) {
    const double PI = 3.14159265;
    TString outString = "Nothin";
    TH1F * currOutHist;
    TH3F * currInHist;
    TString addName = "";
//    TAxis * XAxis;
    TAxis * YAxis, * ZAxis;
//    int XAxisLB, XAxisUB;
    int YAxisLB, YAxisUB, ZAxisLB, ZAxisUB;
    TString outHistName;
    TString outHistPlusName;
    float YAxisLBFind, YAxisUBFind, ZAxisLBFind, ZAxisUBFind;
    if (plotName.Contains("h_MT2ll_vs_DeltaPhiZMET_vs_nVtx")) {
        switch (whichCase) {
            case 0: // plot for DeltaPhiZMET > 2./3. Pi and nVtx = 1-10
                outHistPlusName = "_XAxis_DPhiZMETGt2/3_nVtx1to10_";
                outHistPlusName += addName; 
                YAxisLBFind = 2./3. * PI;
                YAxisUBFind = PI;
                ZAxisLBFind = 1.;
                ZAxisUBFind = 10.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {2/3#pi:#pi}, N_{vtx}^{reco} #in {1:10}";
                break;
            case 1: // plot for 1./3. Pi < DeltaPhiZMET < 2/3 Pi and nVtx = 1-10
                outHistPlusName = "_XAxis_DPhiZMETGt1/3Lt2/3_nVtx1to10_";
                outHistPlusName += addName; 
                YAxisLBFind = 1./3. * PI;
                YAxisUBFind = 2./3. * PI;
                ZAxisLBFind = 1.;
                ZAxisUBFind = 10.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {1/3#pi:2/3 #pi}, N_{vtx}^{reco} #in {1:10}";
                break;   
            case 2: // plot for DeltaPhiZMET < 1/3 Pi and nVtx = 1-10
                outHistPlusName = "_XAxis_DPhiZMETLt1/3_nVtx1to10_";
                outHistPlusName += addName; 
                YAxisLBFind = 0;
                YAxisUBFind = 1./3. * PI;
                ZAxisLBFind = 1.;
                ZAxisUBFind = 10.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {0:1/3#pi}, N_{vtx}^{reco} #in {1:10}";
                break; 
            case 3: // plot for DeltaPhiZMET > 2./3. Pi and nVtx = 11-20
                outHistPlusName = "_XAxis_DPhiZMETGt2/3_nVtx11to20_";
                outHistPlusName += addName; 
                YAxisLBFind = 2./3. * PI;
                YAxisUBFind = PI;
                ZAxisLBFind = 11.;
                ZAxisUBFind = 20.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {2/3#pi:#pi}, N_{vtx}^{reco} #in {11:20}";
                break;
            case 4: // plot for 1./3. Pi < DeltaPhiZMET < 2/3 Pi and nVtx = 11-20
                outHistPlusName = "_XAxis_DPhiZMETGt1/3Lt2/3_nVtx11to20_";
                outHistPlusName += addName; 
                YAxisLBFind = 1./3. * PI;
                YAxisUBFind = 2./3. * PI;
                ZAxisLBFind = 11.;
                ZAxisUBFind = 20.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {1/3#pi:2/3 #pi}, N_{vtx}^{reco} #in {11:20}";
                break;   
            case 5: // plot for DeltaPhiZMET < 1/3 Pi and nVtx = 11-20
                outHistPlusName = "_XAxis_DPhiZMETLt1/3_nVtx11to20_";
                outHistPlusName += addName; 
                YAxisLBFind = 0;
                YAxisUBFind = 1./3. * PI;
                ZAxisLBFind = 11.;
                ZAxisUBFind = 20.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {0:1/3#pi}, N_{vtx}^{reco} #in {11:20}";
                break; 
            case 6: // plot for DeltaPhiZMET > 2./3. Pi and nVtx = 21-30
                outHistPlusName = "_XAxis_DPhiZMETGt2/3_nVtx21to30_";
                outHistPlusName += addName; 
                YAxisLBFind = 2./3. * PI;
                YAxisUBFind = PI;
                ZAxisLBFind = 21.;
                ZAxisUBFind = 30.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {2/3#pi:#pi}, N_{vtx}^{reco} #in {21:30}";
                break;
            case 7: // plot for 1./3. Pi < DeltaPhiZMET < 2/3 Pi and nVtx = 21-30
                outHistPlusName = "_XAxis_DPhiZMETGt1/3Lt2/3_nVtx21to30_";
                outHistPlusName += addName; 
                YAxisLBFind = 1./3. * PI;
                YAxisUBFind = 2./3. * PI;
                ZAxisLBFind = 21.;
                ZAxisUBFind = 30.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {1/3#pi:2/3 #pi}, N_{vtx}^{reco} #in {21:30}";
                break;   
            case 8: // plot for DeltaPhiZMET < 1/3 Pi and nVtx = 21-30
                outHistPlusName = "_XAxis_DPhiZMETLt1/3_nVtx21to30_";
                outHistPlusName += addName; 
                YAxisLBFind = 0;
                YAxisUBFind = 1./3. * PI;
                ZAxisLBFind = 21.;
                ZAxisUBFind = 30.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {0:1/3#pi}, N_{vtx}^{reco} #in {21:30}";
                break;
            default:
                break;
        }
    }    
    else if (plotName.Contains("h_MT2ll_vs_DeltaPhiZMET_vs_NJets")) {
        if (plotName.Contains("nVtx1to10")) {
            addName = "nVtx1to10";
        }
        else if (plotName.Contains("nVtx11to20")) {
            addName = "nVtx11to20";
        }
        else if (plotName.Contains("nVtx21to30")) {
            addName = "nVtx21to30";
        }
        switch (whichCase) {
            case 0: // plot for DeltaPhiZMET > 2./3. Pi and N_{Jets} #in {0}
                outHistPlusName = "_XAxis_DPhiZMETGt2/3_nJets0_";
                outHistPlusName += addName; 
                YAxisLBFind = 2./3. * PI;
                YAxisUBFind = PI;
                ZAxisLBFind = 0.;
                ZAxisUBFind = 0.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {2/3#pi:#pi}, N_{Jets} #in {0}";
                break;
            case 1: // plot for 1./3. Pi < DeltaPhiZMET < 2/3 Pi and N_{Jets} #in {0}
                outHistPlusName = "_XAxis_DPhiZMETGt1/3Lt2/3_nJets0_";
                outHistPlusName += addName; 
                YAxisLBFind = 1./3. * PI;
                YAxisUBFind = 2./3. * PI;
                ZAxisLBFind = 0.;
                ZAxisUBFind = 0.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {1/3#pi:2/3 #pi}, N_{Jets} #in {0}";
                break;   
            case 2: // plot for DeltaPhiZMET < 1/3 Pi and N_{Jets} #in {0}
                outHistPlusName = "_XAxis_DPhiZMETLt1/3_nJets0_";
                outHistPlusName += addName; 
                YAxisLBFind = 0;
                YAxisUBFind = 1./3. * PI;
                ZAxisLBFind = 0.;
                ZAxisUBFind = 0.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {0:1/3#pi}, N_{Jets} #in {0}";
                break; 
            case 3: // plot for DeltaPhiZMET > 2./3. Pi and N_{Jets} #in {1}
                outHistPlusName = "_XAxis_DPhiZMETGt2/3_nJets1_";
                outHistPlusName += addName; 
                YAxisLBFind = 2./3. * PI;
                YAxisUBFind = PI;
                ZAxisLBFind = 1.;
                ZAxisUBFind = 1.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {2/3#pi:#pi}, N_{Jets} #in {1}";
                break;
            case 4: // plot for 1./3. Pi < DeltaPhiZMET < 2/3 Pi and N_{Jets} #in {1}
                outHistPlusName = "_XAxis_DPhiZMETGt1/3Lt2/3_nJets1_";
                outHistPlusName += addName; 
                YAxisLBFind = 1./3. * PI;
                YAxisUBFind = 2./3. * PI;
                ZAxisLBFind = 1.;
                ZAxisUBFind = 1.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {1/3#pi:2/3 #pi}, N_{Jets} #in {1}";
                break;   
            case 5: // plot for DeltaPhiZMET < 1/3 Pi and N_{Jets} #in {1}
                outHistPlusName = "_XAxis_DPhiZMETLt1/3_nJets1_";
                outHistPlusName += addName; 
                YAxisLBFind = 0;
                YAxisUBFind = 1./3. * PI;
                ZAxisLBFind = 1.;
                ZAxisUBFind = 1.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {0:1/3#pi}, N_{Jets} #in {1}";
                break; 
            case 6: // plot for DeltaPhiZMET > 2./3. Pi and N_{Jets} #in {2:#infty}
                outHistPlusName = "_XAxis_DPhiZMETGt2/3_nJetsGt1_";
                outHistPlusName += addName; 
                YAxisLBFind = 2./3. * PI;
                YAxisUBFind = PI;
                ZAxisLBFind = 2.;
                ZAxisUBFind = 20.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {2/3#pi:#pi}, N_{Jets} #in {2:#infty}";
                break;
            case 7: // plot for 1./3. Pi < DeltaPhiZMET < 2/3 Pi and N_{Jets} #in {2:#infty}
                outHistPlusName = "_XAxis_DPhiZMETGt1/3Lt2/3_nJetsGt1_";
                outHistPlusName += addName; 
                YAxisLBFind = 1./3. * PI;
                YAxisUBFind = 2./3. * PI;
                ZAxisLBFind = 2.;
                ZAxisUBFind = 20.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {1/3#pi:2/3 #pi}, N_{Jets} #in {2:#infty}";
                break;   
            case 8: // plot for DeltaPhiZMET < 1/3 Pi and N_{Jets} #in {2:#infty}
                outHistPlusName = "_XAxis_DPhiZMETLt1/3_nJetsGt1_";
                outHistPlusName += addName; 
                YAxisLBFind = 0;
                YAxisUBFind = 1./3. * PI;
                ZAxisLBFind = 2.;
                ZAxisUBFind = 20.;
                outString = "#Delta #phi_{Z, #slash{E}_{T}} #in {0:1/3#pi}, N_{Jets} #in {2:#infty}";
                break;
        }
    }
    if (outString.Contains("Nothin")) return outString;
    for (unsigned int iHist = 0; iHist < inputHistVec->size(); ++iHist) {
        currInHist = (TH3F*) inputHistVec->at(iHist);
        outHistName = plotName;
        outHistName += outHistPlusName;
        outHistName += iHist;
        YAxis = currInHist->GetYaxis();
        YAxisLB = YAxis->FindBin(YAxisLBFind);
        YAxisUB = YAxis->FindBin(YAxisUBFind);
        ZAxis = currInHist->GetZaxis();
        ZAxisLB = ZAxis->FindBin(ZAxisLBFind);
        ZAxisUB = ZAxis->FindBin(ZAxisUBFind);
        currOutHist = (TH1F*) currInHist->ProjectionX(outHistName, YAxisLB, YAxisUB, ZAxisLB, ZAxisUB, "e");
        outputHistVec->push_back(currOutHist);        
    }
    return outString;
}
#endif //HistSampFunc_h_
