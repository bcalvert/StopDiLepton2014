#include "../../HeaderFiles/NewOviStopPlotSkimmer_Legacy.h"

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

using namespace std;
int main( int argc, char* argv[] ) {
    gROOT->ProcessLine("#include <vector>");
    TString baseDirNonDataRootFile = "../../NonDataRootFiles/";
    TString baseDirOfficialSmearing = baseDirNonDataRootFile + "OfficialSmearing/";
    TString baseDirSmearingTemplate = baseDirNonDataRootFile + "SmearingTemplates/";
    /////////Variable initializations/////////////////
    /////Organization Variables//////////////
    
    TFile * JetESFile = new TFile("../../PlotShowingCode/Summer13JESUncert.root");
    TH2F * h_JetESUp = (TH2F *) JetESFile->Get("h_JESSystUncertUp");
    TH2F * h_JetESDown = (TH2F *) JetESFile->Get("h_JESSystUncertDown");
    TFile * GenJetSmearFile = new TFile("pfJetResolutionMCtoDataCorrLUT.root");
    TH2F * h_GenJetSmearHist = (TH2F*) GenJetSmearFile->Get("pfJetResolutionMCtoDataCorrLUT");
    TH2F * h_RecoJetLowPtSmearHist = ResolutionHistMaker(TString("JetResolution.txt"));
    
    TFile * FullSimPUWeights = new TFile("WeightStop_FullSim.root");
    TH3F * FullSimWeightHist = (TH3F *) FullSimPUWeights->Get("WHist");
    
    TFile * FastSimPUWeights = new TFile("WeightStop_FastSim.root");
    TH3F * FastSimWeightHist = (TH3F *) FastSimPUWeights->Get("WHist");
    
    TFile * FullSimTruePUWeight = new TFile("FullSimWeight1D.root");
    TH1F * FullSimTrueWeightHist = (TH1F *) FullSimTruePUWeight->Get("TrueNumIntWeights");
    TFile * FastSimTruePUWeight = new TFile("FastSimWeight1D.root");
    TH1F * FastSimTrueWeightHist = (TH1F *) FastSimTruePUWeight->Get("TrueNumIntWeights");
    
    TH3F * WeightHistToUse3D;
    TH1F * TrueWeightHistToUse1D;
    
    TFile * JetSmearSFFile = new TFile("pfJetResolutionMCtoDataCorrLUT.root");
    TH2F * h_JetSmearSFs = (TH2F*) JetSmearSFFile->Get("pfJetResolutionMCtoDataCorrLUT");
    /*
    TFile * JetESFile = new TFile("../../PlotShowingCode/Summer13JESUncert.root");
    TH2F * h_JetESUp = (TH2F *) JetESFile->Get("h_JESSystUncertUp");
    TH2F * h_JetESDown = (TH2F *) JetESFile->Get("h_JESSystUncertDown");
    
    TFile * GenJetSmearFile = new TFile(baseDirOfficialSmearing + TString("pfJetResolutionMCtoDataCorrLUT.root"));
    TH2F * h_GenJetSmearHist = (TH2F*) GenJetSmearFile->Get("pfJetResolutionMCtoDataCorrLUT");
    TH2F * h_RecoJetLowPtSmearHist = ResolutionHistMaker(baseDirOfficialSmearing + TString("JetResolution.txt"));
    
    
    TString baseDirPUReweighting = baseDirNonDataRootFile + TString("PUReweightingRootFiles/");
    
    TFile * FullSimPUWeights = new TFile(baseDirPUReweighting + TString("WeightStop_FullSim.root"));
    TH3F * FullSimWeightHist = (TH3F *) FullSimPUWeights->Get("WHist");
    
    TFile * FastSimPUWeights = new TFile(baseDirPUReweighting + TString("WeightStop_FastSim.root"));
    TH3F * FastSimWeightHist = (TH3F *) FastSimPUWeights->Get("WHist");
    
    TFile * FullSimTruePUWeight = new TFile(baseDirPUReweighting + TString("FullSimWeight1D.root"));
    TH1F * FullSimTrueWeightHist = (TH1F *) FullSimTruePUWeight->Get("TrueNumIntWeights");
    TFile * FastSimTruePUWeight = new TFile(baseDirPUReweighting + TString("FastSimWeight1D.root"));
    TH1F * FastSimTrueWeightHist = (TH1F *) FastSimTruePUWeight->Get("TrueNumIntWeights");
    
    TH3F * WeightHistToUse3D;
    TH1F * TrueWeightHistToUse1D;
    
    TFile * JetSmearSFFile = new TFile(baseDirOfficialSmearing + TString("pfJetResolutionMCtoDataCorrLUT.root"));
    TH2F * h_JetSmearSFs = (TH2F*) JetSmearSFFile->Get("pfJetResolutionMCtoDataCorrLUT");
    
    */
    
    TH2F * h_DeltaPtSmearUnsmear_GenMatched = new TH2F("h_DeltaPtSmearUnsmear_GenMatched", ";Unsmeared Jet p_{T}; #Delta p_{T} (Smear, Unsmear)",  250, 0, 250, 150, -30, 30);
    TH2F * h_DeltaPtSmearUnsmear_NotGenMatched = new TH2F("h_DeltaPtSmearUnsmear_NotGenMatched", ";Unsmeared Jet p_{T}; #Delta p_{T} (Smear, Unsmear)",  250, 0, 250, 150, -30, 30);
    
//    TH2F * h_DeltaEnergyUnsmearJetMinGenMatchJet = new TH2F("h_DeltaEnergyUnsmearJetMinGenMatchJet", ";Unsmeared Jet Energy; #Delta En (Unsmear, GenMatch)", 250, 0, 250, 150, -30, 30);
    
    vector<float> * vecDeltaEnUnsmearJetGenMatchJet = new vector<float>;
    vector<float> * vecGoodUnsmearJetEn = new vector<float>;
    
    vector<float> * vecDeltaEnUnsmearJetGenMatchJet_SmearMatch = new vector<float>;
    vector<float> * vecGoodUnsmearJetEn_SmearMatch = new vector<float>;
    vector<float> * vecDeltaEnUnsmearJetSmearJet_GenMatch = new vector<float>;
    vector<float> * vecDeltaEnUnsmearJetSmearJet_NotGenMatch = new vector<float>;
    
    
    std::map<string, int> mapGenRW;
    int genWeightType;
    SetGenRWMap(mapGenRW);
    
    bool doSmearHack;
    
    // List of files for the "by-hand smearing"
    // "High jet" refers to the smearing as calculated for events with 2+ jets passing basic selection (30 GeV pT and some other cuts)
    // "Low jet" refers to the smearing as calculated for events with 0 - 1 jets passing above basic selection (30 GeV pT and some other cuts)
    
    
    TString highJet = "_NJetsGrEq2";
    TString lowJet = "_NJetsLess2";
    TString outputAppendMM = "_Output.root";
    TString outputAppendBC = "Haddplots.root";
    
    TFile * TTBarFile_HighJet = new TFile("TTbarJets_NJetsGrEq2_Output.root");
    TFile * DYFile_HighJet = new TFile("ZJets_2l_NJetsGrEq2_Output.root");
    TFile * WWFile_HighJet = new TFile("WW_NJetsGrEq2_Output.root");
    TFile * WZFile_HighJet = new TFile("WZ_NJetsGrEq2_Output.root");
    TFile * ZZFile_HighJet = new TFile("ZZ_NJetsGrEq2_Output.root");
    TFile * WJFile_HighJet = new TFile("WJHaddplots.root");
    TFile * ZGFile_HighJet = new TFile("ZGHaddplots.root");
    TFile * WGFile_HighJet = new TFile("WGHaddplots.root");
    
    TFile * TTBarFile_LowJet = new TFile("TTbarJets_NJetsLess2_Output.root");
    TFile * DYFile_LowJet = new TFile("ZJets_2l_NJetsLess2_Output.root");
    TFile * WWFile_LowJet = new TFile("WW_NJetsLess2_Output.root");
    TFile * WZFile_LowJet = new TFile("WZ_NJetsLess2_Output.root");
    TFile * ZZFile_LowJet = new TFile("ZZ_NJetsLess2_Output.root");
    TFile * WJFile_LowJet = new TFile("WJHaddplots.root");
    TFile * ZGFile_LowJet = new TFile("ZGHaddplots.root");
    TFile * WGFile_LowJet = new TFile("WGHaddplots.root");
    /*
    TFile * TTBarFile_HighJet = new TFile(baseDirSmearingTemplate + TString("TTbarJets") + highJet + outputAppendMM);
    TFile * DYFile_HighJet = new TFile(baseDirSmearingTemplate + TString("ZJets_2l") + highJet + outputAppendMM);
    TFile * WWFile_HighJet = new TFile(baseDirSmearingTemplate + TString("WW") + highJet + outputAppendMM);
    TFile * WZFile_HighJet = new TFile(baseDirSmearingTemplate + TString("WZ") + highJet + outputAppendMM);
    TFile * ZZFile_HighJet = new TFile(baseDirSmearingTemplate + TString("ZZ") + highJet + outputAppendMM);
    TFile * WJFile_HighJet = new TFile(baseDirSmearingTemplate + TString("WJ") + outputAppendBC);
    TFile * ZGFile_HighJet = new TFile(baseDirSmearingTemplate + TString("ZG") + outputAppendBC);
    TFile * WGFile_HighJet = new TFile(baseDirSmearingTemplate + TString("WG") + outputAppendBC);
    
    TFile * TTBarFile_LowJet = new TFile(baseDirSmearingTemplate + TString("TTbarJets") + lowJet + outputAppendMM);
    TFile * DYFile_LowJet = new TFile(baseDirSmearingTemplate + TString("ZJets_2l") + lowJet + outputAppendMM);
    TFile * WWFile_LowJet = new TFile(baseDirSmearingTemplate + TString("WW") + lowJet + outputAppendMM);
    TFile * WZFile_LowJet = new TFile(baseDirSmearingTemplate + TString("WZ") + lowJet + outputAppendMM);
    TFile * ZZFile_LowJet = new TFile(baseDirSmearingTemplate + TString("ZZ") + lowJet + outputAppendMM);
    TFile * WJFile_LowJet = new TFile(baseDirSmearingTemplate + TString("WJ") + outputAppendBC);
    TFile * ZGFile_LowJet = new TFile(baseDirSmearingTemplate + TString("ZG") + outputAppendBC);
    TFile * WGFile_LowJet = new TFile(baseDirSmearingTemplate + TString("WG") + outputAppendBC);
    
    */
    
    // push the files from above into two specific vector<TFile *>
    vector<TFile *> * METSmearFiles_LowJet = new vector<TFile *>;
    vector<TFile *> * METSmearFiles_HighJet = new vector<TFile *>;
    METSmearFiles_LowJet->push_back(TTBarFile_LowJet);
    METSmearFiles_LowJet->push_back(DYFile_LowJet);
    METSmearFiles_LowJet->push_back(WWFile_LowJet);
    METSmearFiles_LowJet->push_back(WZFile_LowJet);
    METSmearFiles_LowJet->push_back(ZZFile_LowJet);
    METSmearFiles_LowJet->push_back(WJFile_LowJet);
    METSmearFiles_LowJet->push_back(ZGFile_LowJet);
    METSmearFiles_LowJet->push_back(WGFile_LowJet);
    
    METSmearFiles_HighJet->push_back(TTBarFile_HighJet);
    METSmearFiles_HighJet->push_back(DYFile_HighJet);
    METSmearFiles_HighJet->push_back(WWFile_HighJet);
    METSmearFiles_HighJet->push_back(WZFile_HighJet);
    METSmearFiles_HighJet->push_back(ZZFile_HighJet);
    METSmearFiles_HighJet->push_back(WJFile_HighJet);
    METSmearFiles_HighJet->push_back(ZGFile_HighJet);
    METSmearFiles_HighJet->push_back(WGFile_HighJet);
    
    /*
     vector<TH2F * > * METXSmearHistVec          = SetMETXYSmearHistVec(1, METSmearFiles);
     vector<TH2F * > * METXSmearJERUpHistVec     = SetMETXYSmearHistVec(1, METSmearFiles, 1, 1);
     vector<TH2F * > * METXSmearJERDownHistVec   = SetMETXYSmearHistVec(1, METSmearFiles, 1, -1);
     vector<TH2F * > * METXSmearUncESUpHistVec   = SetMETXYSmearHistVec(1, METSmearFiles, 2, 2);
     vector<TH2F * > * METXSmearUncESDownHistVec = SetMETXYSmearHistVec(1, METSmearFiles, 2, -2);
     
     vector<TH2F * > * METYSmearHistVec          = SetMETXYSmearHistVec(0, METSmearFiles);
     vector<TH2F * > * METYSmearJERUpHistVec     = SetMETXYSmearHistVec(0, METSmearFiles, 1, 1);
     vector<TH2F * > * METYSmearJERDownHistVec   = SetMETXYSmearHistVec(0, METSmearFiles, 1, -1);
     vector<TH2F * > * METYSmearUncESUpHistVec   = SetMETXYSmearHistVec(0, METSmearFiles, 2, 2);
     vector<TH2F * > * METYSmearUncESDownHistVec = SetMETXYSmearHistVec(0, METSmearFiles, 2, -2);    
     
     std::map<string, int> METXSmearMap;
     std::map<string, int> METYSmearMap;
     
     SetMETSmearHistMap(METXSmearMap);
     SetMETSmearHistMap(METYSmearMap);
     
     TH2F * METXSmearHist2D, * METYSmearHist2D;
     
     TH2F * METXSmearJERUpHist2D, * METXSmearJERDownHist2D;
     TH2F * METYSmearJERUpHist2D, * METYSmearJERDownHist2D;
     
     TH2F * METXSmearUncESUpHist2D, * METXSmearUncESDownHist2D;
     TH2F * METYSmearUncESUpHist2D, * METYSmearUncESDownHist2D;
     
     vector<TH1F *> * vecOneDeeMETXSmearHist;
     vector<TH1F *> * vecOneDeeMETYSmearHist;
     
     vector<TH1F *> * vecOneDeeMETXSmearJERUpHist, * vecOneDeeMETXSmearJERDownHist;
     vector<TH1F *> * vecOneDeeMETXSmearUncESUpHist, * vecOneDeeMETXSmearUncESDownHist;
     
     vector<TH1F *> * vecOneDeeMETYSmearJERUpHist, * vecOneDeeMETYSmearJERDownHist;
     vector<TH1F *> * vecOneDeeMETYSmearUncESUpHist, * vecOneDeeMETYSmearUncESDownHist;
     */
    
    
    // Make vectors of TH2Fs taken from the vector<TFile *>s above -- These TH2Fs will be chopped into TH1Fs for MET dependent smearing
    vector<TH2F * > * METSmearHistVec_HighJet          = SetMETSmearHistVec(1, METSmearFiles_HighJet);
    vector<TH2F * > * METSmearJERUpHistVec_HighJet     = SetMETSmearHistVec(1, METSmearFiles_HighJet, 1, 1);
    vector<TH2F * > * METSmearJERDownHistVec_HighJet   = SetMETSmearHistVec(1, METSmearFiles_HighJet, 1, -1);
    vector<TH2F * > * METSmearUncESUpHistVec_HighJet   = SetMETSmearHistVec(1, METSmearFiles_HighJet, 2, 2);
    vector<TH2F * > * METSmearUncESDownHistVec_HighJet = SetMETSmearHistVec(1, METSmearFiles_HighJet, 2, -2);
    
    vector<TH2F * > * METSmearHistVec_LowJet          = SetMETSmearHistVec(1, METSmearFiles_LowJet);
    vector<TH2F * > * METSmearJERUpHistVec_LowJet     = SetMETSmearHistVec(1, METSmearFiles_LowJet, 1, 1);
    vector<TH2F * > * METSmearJERDownHistVec_LowJet   = SetMETSmearHistVec(1, METSmearFiles_LowJet, 1, -1);
    vector<TH2F * > * METSmearUncESUpHistVec_LowJet   = SetMETSmearHistVec(1, METSmearFiles_LowJet, 2, 2);
    vector<TH2F * > * METSmearUncESDownHistVec_LowJet = SetMETSmearHistVec(1, METSmearFiles_LowJet, 2, -2);
    
    vector<TH2F * > * METPhiSmearHistVec_HighJet          = SetMETSmearHistVec(0, METSmearFiles_HighJet);        
    vector<TH2F * > * METPhiSmearJERUpHistVec_HighJet     = SetMETSmearHistVec(0, METSmearFiles_HighJet, 1, 1);
    vector<TH2F * > * METPhiSmearJERDownHistVec_HighJet   = SetMETSmearHistVec(0, METSmearFiles_HighJet, 1, -1);
    vector<TH2F * > * METPhiSmearUncESUpHistVec_HighJet   = SetMETSmearHistVec(0, METSmearFiles_HighJet, 2, 2);
    vector<TH2F * > * METPhiSmearUncESDownHistVec_HighJet = SetMETSmearHistVec(0, METSmearFiles_HighJet, 2, -2);
    
    vector<TH2F * > * METPhiSmearHistVec_LowJet          = SetMETSmearHistVec(0, METSmearFiles_LowJet);        
    vector<TH2F * > * METPhiSmearJERUpHistVec_LowJet     = SetMETSmearHistVec(0, METSmearFiles_LowJet, 1, 1);
    vector<TH2F * > * METPhiSmearJERDownHistVec_LowJet   = SetMETSmearHistVec(0, METSmearFiles_LowJet, 1, -1);
    vector<TH2F * > * METPhiSmearUncESUpHistVec_LowJet   = SetMETSmearHistVec(0, METSmearFiles_LowJet, 2, 2);
    vector<TH2F * > * METPhiSmearUncESDownHistVec_LowJet = SetMETSmearHistVec(0, METSmearFiles_LowJet, 2, -2);
    
    // maps between file names and the appropriate smearing topology to apply -- so "TTBar" datasets get smearing from the TTBar template
    // "DY" and "Z+Jets" get smearing from the Z->2l template, etc.
    std::map<string, int> METSmearMap;
    std::map<string, int> METPhiSmearMap;
    SetMETSmearHistMap(METSmearMap);
    SetMETSmearHistMap(METPhiSmearMap);
    
    // The given TH2F to use based on what file -- including specifics ones for the JER and UncES systematics
    TH2F * METSmearHist2D_HighJet = 0, * METPhiSmearHist2D_HighJet = 0;
    TH2F * METSmearHist2D_LowJet = 0, * METPhiSmearHist2D_LowJet = 0;
    TH2F * METSmearJERUpHist2D_HighJet = 0, * METSmearJERDownHist2D_HighJet = 0;
    TH2F * METSmearJERUpHist2D_LowJet = 0, * METSmearJERDownHist2D_LowJet = 0;
    
    TH2F * METPhiSmearJERUpHist2D_HighJet = 0, * METPhiSmearJERDownHist2D_HighJet = 0;
    TH2F * METPhiSmearJERUpHist2D_LowJet = 0, * METPhiSmearJERDownHist2D_LowJet = 0;
    TH2F * METSmearUncESUpHist2D_HighJet = 0, * METSmearUncESDownHist2D_HighJet = 0;
    TH2F * METSmearUncESUpHist2D_LowJet = 0, * METSmearUncESDownHist2D_LowJet = 0;
    TH2F * METPhiSmearUncESUpHist2D_HighJet = 0, * METPhiSmearUncESDownHist2D_HighJet = 0;
    TH2F * METPhiSmearUncESUpHist2D_LowJet = 0, * METPhiSmearUncESDownHist2D_LowJet = 0;
    
    // vectors of "TH1F *"s to contain the 1D smearing templates
    vector<TH1F *> * vecOneDeeMETSmearHist_HighJet = 0;
    vector<TH1F *> * vecOneDeeMETSmearHist_LowJet = 0;
    
    vector<TH1F *> * vecOneDeeMETPhiSmearHist_HighJet = 0;
    vector<TH1F *> * vecOneDeeMETPhiSmearHist_LowJet = 0;
    
    vector<TH1F *> * vecOneDeeMETSmearJERUpHist_HighJet = 0, * vecOneDeeMETSmearJERDownHist_HighJet = 0;
    vector<TH1F *> * vecOneDeeMETSmearJERUpHist_LowJet = 0, * vecOneDeeMETSmearJERDownHist_LowJet = 0;
    vector<TH1F *> * vecOneDeeMETSmearUncESUpHist_HighJet = 0, * vecOneDeeMETSmearUncESDownHist_HighJet = 0;
    vector<TH1F *> * vecOneDeeMETSmearUncESUpHist_LowJet = 0, * vecOneDeeMETSmearUncESDownHist_LowJet = 0;
    
    vector<TH1F *> * vecOneDeeMETPhiSmearJERUpHist_HighJet = 0, * vecOneDeeMETPhiSmearJERDownHist_HighJet = 0;
    vector<TH1F *> * vecOneDeeMETPhiSmearJERUpHist_LowJet = 0, * vecOneDeeMETPhiSmearJERDownHist_LowJet = 0;
    vector<TH1F *> * vecOneDeeMETPhiSmearUncESUpHist_HighJet = 0, * vecOneDeeMETPhiSmearUncESDownHist_HighJet = 0;
    vector<TH1F *> * vecOneDeeMETPhiSmearUncESUpHist_LowJet = 0, * vecOneDeeMETPhiSmearUncESDownHist_LowJet = 0;
    
    
    // To minimize amount of code to write, utilize a "ToUse" set of pointers that will change depending upon the event conditions and/or systematic
    // in consideration
    TH2F * METSmearHist2D_ToUse = 0, * METPhiSmearHist2D_ToUse = 0;
    vector<TH1F *> * vecOneDeeMETSmearHist_ToUse = 0, * vecOneDeeMETPhiSmearHist_ToUse = 0;
    
    TH2F * METSmearHist2D_ToUse_SystVarUp = 0, * METPhiSmearHist2D_ToUse_SystVarUp = 0;
    vector<TH1F *> * vecOneDeeMETSmearHist_ToUse_SystVarUp, * vecOneDeeMETPhiSmearHist_ToUse_SystVarUp = 0;
    
    TH2F * METSmearHist2D_ToUse_SystVarDown = 0, * METPhiSmearHist2D_ToUse_SystVarDown = 0;
    vector<TH1F *> * vecOneDeeMETSmearHist_ToUse_SystVarDown = 0, * vecOneDeeMETPhiSmearHist_ToUse_SystVarDown = 0;
    
    // Stuff related to "official" smearing method done directly on the jets
    vector<TF1> * JetResolutionTF1Vec = new vector<TF1>; VecJetHighPtResolutionTF1(JetResolutionTF1Vec);
    
    TH1F * h_DeltaEnergySmearMinUnSmear = new TH1F("h_dEnSmearUnSmear", ";SmearJetEnergy - Unsmear Jet Energy", 160, -40, 40);
    TH1F * h_DeltaEnergySmearMinUnSmear_JESUp = new TH1F("h_dEnSmearUnSmear_JESUp", ";SmearJetEnergy - Unsmear Jet Energy", 160, -40, 40);
    TH1F * h_DeltaEnergySmearMinUnSmear_JESDown = new TH1F("h_dEnSmearUnSmear_JESDown", ";SmearJetEnergy - Unsmear Jet Energy", 160, -40, 40);
    TH1F * h_DeltaEnergySmearMinUnSmear_JERUp = new TH1F("h_dEnSmearUnSmear_JERUp", ";SmearJetEnergy - Unsmear Jet Energy", 160, -40, 40);
    TH1F * h_DeltaEnergySmearMinUnSmear_JERDown = new TH1F("h_dEnSmearUnSmear_JERDown", ";SmearJetEnergy - Unsmear Jet Energy", 160, -40, 40);
    
    TString fileInTreeName, fileOutTreeName;
    TString fOutName;
    TString outputSavePathString = "outputSavePath";
    //    Double_t relIsoBins[] = {-1.01, 0., 0.005, 0.01, 0.015, 0.02, 0.025, 0.03, 0.035, 0.04, 0.045, 0.05, 0.055, 0.06, 0.065, 0.07, 0.075, 0.08, 0.085, 0.09, 0.095, 0.1, 0.105, 0.11, 0.115, 0.12, 0.125, 0.13, 0.135, 0.14, 0.145, 0.15, 0.155, 0.16};
    TH1F * h_eventCount = new TH1F("h_eventCount", "; numEvents (# of Entries);", 2, -0.5, 1.5);
    TH1F * h_CutFlow = new TH1F("h_CutFlow", "; Cut Stage; N_{evts} passing Cut Stage", 3, 0.5, 3.5);
    TH1F * h_CutFlow_LepESUp = 0, * h_CutFlow_LepESDown = 0;
    
    //    TH1F * h_ElecRelIso = new TH1F("h_ElecRelIso", "; Electron Relative Iso.; N_{evts} / bin", 32, 0., 0.16);
    /////Event Variables/////////////////////
    
    //////containers for pointers to vectors
    //Muons
    MuonEventPointers MEPs;
    MEPs.InitializeVecs();
    
    //Electrons
    ElectronEventPointers EEPs;
    EEPs.InitializeVecs();
    
    //Jets
    PFJetEventPointers PFJEPs;
    PFJEPs.InitializeVecs();
    
    PFJetEventPointers SmearPFJEPs;
    SmearPFJEPs.InitializeVecs();
    
    FilterTriggerInfo FTI;
    
    ElecCutInfo        ECI_Main; ECI_Main.DefaultCutVarVals();
    ElecCutInfo        ECI_Loose; ECI_Loose.SetAsFakeLepECI();
    JetCutInfo         JCI; JCI.DefaultCutVarVals(); JCI.PrintVals();
    
    BTagSFUtil * BTagSFUtilBase, * BTagSFUtilSignal, * BTagSFUtilToUse;
    TString BTagSFAlgorithm = "CSVM", BTagSFDataPeriod = "ABCD", BTagSFSignalString;
    
    //Vertex info (both for nVtx info and also muon dZ)
    vector<float> * VertZ, * VertNDOF, * VertRho; 
    vector<bool> * VertIsFake;
    float firstGoodVertZ = 0.0;
    
    VertZ = new vector<float>; VertNDOF = new vector<float>;
    VertRho = new vector<float>; 
    VertIsFake = new vector<bool>;
    
    BasicEventInfo BEI;
    
    EventGenWeight EGW; EGW.SetGenParticleWeightDefault();
    
    EventPileUpInfo EPI;
    
    vector<int> systLB_Lepton, systUB_Lepton;
    vector<int> systLB_Jet, systUB_Jet;
    vector<int> systLB_SmearJet, systUB_SmearJet;
    
    vector<int> systLB_MET, systUB_MET;
    vector<int> systLB_SmearMET, systUB_SmearMET;
    
    vector<int> systLB_EDSI, systUB_EDSI;
    vector<int> systLB_SmearEDSI, systUB_SmearEDSI;
    
    const int numSysts = 6;
    // Systs -- 1: LepES, 2: JetES, 3/4: BTag SF stuff, 5: JetSmear, 6: UncES
    int systLepES = 1;
    int systJetES = 2;
    int systBTagEffSF = 3;
    int systBMisTagSF = 4;
    int systJetSmear = 5;
    int systUncES = 6;
    
    int systLepESShiftUp, systLepESShiftDown;
    int systJetESShiftUp, systJetESShiftDown;
    int systBTagEffSFShiftUp, systBTagEffSFShiftDown;
    int systBMisTagSFShiftUp, systBMisTagSFShiftDown;
    int systJetSmearShiftUp, systJetSmearShiftDown;
    int systUncESShiftUp, systUncESShiftDown;
    
    systLB_Lepton.push_back(systLepES);  systUB_Lepton.push_back(systLepES);
    systLB_Jet.push_back(systJetES);     systUB_Jet.push_back(systBMisTagSF);
    systLB_SmearJet.push_back(systJetES); systUB_SmearJet.push_back(systJetSmear);
    
    systLB_MET.push_back(systLepES); systUB_MET.push_back(systJetES);
    systLB_SmearMET.push_back(1); systUB_SmearMET.push_back(numSysts);
    
    systLB_EDSI.push_back(systLepES); systUB_EDSI.push_back(systBMisTagSF);
    systLB_SmearEDSI.push_back(1); systUB_SmearEDSI.push_back(numSysts);
    
    
    EventLepInfo * ELIToUse = 0;
    EventJetInfo * EJIToUse = 0;
    EventMETInfo * EMIToUse = 0;
    
    EventJetInfo * SmearEJIToUse = 0;
    EventMETInfo * SmearEMIToUse = 0;
    
    int numSavedLeps  = 2;
    int numSavedJets  = 3;
    int numSavedBJets = 2;
    
    EventLepInfo ELI; ELI.ELIDefaultVarVals(numSavedLeps);  
    EventJetInfo EJI; EJI.EJIDefaultVarVals(numSavedJets, numSavedBJets);
    EventMETInfo EMI_PF_noPhiCorr, EMI_PF_noPhiCorr_Old, EMI_PF_Old, EMI_PF, SmearEMI_PF, SmearEMI_PF_noPhiCorr, SmearHackEMI_PF, SmearHackEMI_PF_Old;
    EventMETInfo EMI_Calo_noPhiCorr, EMI_Calo;
    
    EMI_PF_noPhiCorr.METType = 0;
    EMI_PF_noPhiCorr_Old.METType = 0;
    EMI_PF.METType = 0;
    EMI_PF_Old.METType = 0;
    SmearEMI_PF.METType = 0;
    SmearEMI_PF_noPhiCorr.METType = 0;
    SmearHackEMI_PF.METType = 0;
    SmearHackEMI_PF_Old.METType = 0;
    EMI_Calo_noPhiCorr.METType = 4;
    EMI_Calo.METType = 4;
    
    
    bool passesFullSelection;
    vector<bool> vecPFS_SystVarUp; vecPFS_SystVarUp.resize(numSysts + 1);
    vector<bool> vecPFS_SystVarDown; vecPFS_SystVarDown.resize(numSysts + 1);
    
    EventDiStructureInfo EDSI; EDSI.EDSIDefaultVarVals();
    EventDiStructureInfo EDSI_noPhiCorr; EDSI_noPhiCorr.EDSIDefaultVarVals();
    EventDiStructureInfo SmearEDSI; SmearEDSI.EDSIDefaultVarVals();
    
    EventJetInfo SmearEJI; SmearEJI.EJIDefaultVarVals(numSavedJets, numSavedBJets);
    
    vector<EventLepInfo> ELI_SystVarUp;   ELI_SystVarUp.resize(numSysts + 1);
    vector<EventLepInfo> ELI_SystVarDown; ELI_SystVarDown.resize(numSysts + 1);
    
    vector<EventJetInfo> EJI_SystVarUp;   EJI_SystVarUp.resize(numSysts + 1);
    vector<EventJetInfo> EJI_SystVarDown; EJI_SystVarDown.resize(numSysts + 1);
    
    vector<EventMETInfo> EMI_PF_SystVarUp;   EMI_PF_SystVarUp.resize(numSysts + 1);
    vector<EventMETInfo> EMI_PF_SystVarDown; EMI_PF_SystVarDown.resize(numSysts + 1);
    
    vector<EventDiStructureInfo> EDSI_SystVarUp; EDSI_SystVarUp.resize(numSysts + 1);                                                                       
    vector<EventDiStructureInfo> EDSI_SystVarDown; EDSI_SystVarDown.resize(numSysts + 1);                                                                       
    
    vector<EventJetInfo> SmearEJI_SystVarUp;   SmearEJI_SystVarUp.resize(numSysts + 1);
    vector<EventJetInfo> SmearEJI_SystVarDown; SmearEJI_SystVarDown.resize(numSysts + 1);
    
    vector<EventMETInfo> SmearEMI_PF_SystVarUp;   SmearEMI_PF_SystVarUp.resize(numSysts + 1);
    vector<EventMETInfo> SmearEMI_PF_SystVarDown; SmearEMI_PF_SystVarDown.resize(numSysts + 1);
    
    vector<EventMETInfo> SmearHackEMI_PF_SystVarUp;   SmearHackEMI_PF_SystVarUp.resize(numSysts + 1);
    vector<EventMETInfo> SmearHackEMI_PF_SystVarDown; SmearHackEMI_PF_SystVarDown.resize(numSysts + 1);
    
    vector<EventMETInfo> SmearHackEMI_PF_Old_SystVarUp;   SmearHackEMI_PF_Old_SystVarUp.resize(numSysts + 1);
    vector<EventMETInfo> SmearHackEMI_PF_Old_SystVarDown; SmearHackEMI_PF_Old_SystVarDown.resize(numSysts + 1);
    
    vector<EventDiStructureInfo> SmearEDSI_SystVarUp; SmearEDSI_SystVarUp.resize(numSysts + 1);                                                                       
    vector<EventDiStructureInfo> SmearEDSI_SystVarDown; SmearEDSI_SystVarDown.resize(numSysts + 1);
    
    for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {
        ELI_SystVarUp[iSyst].ELIDefaultVarVals(numSavedLeps);
        ELI_SystVarDown[iSyst].ELIDefaultVarVals(numSavedLeps);
        
        EJI_SystVarUp[iSyst].EJIDefaultVarVals(numSavedJets, numSavedBJets);
        EJI_SystVarDown[iSyst].EJIDefaultVarVals(numSavedJets, numSavedBJets);
        
        SmearEJI_SystVarUp[iSyst].EJIDefaultVarVals(numSavedJets, numSavedBJets);
        SmearEJI_SystVarDown[iSyst].EJIDefaultVarVals(numSavedJets, numSavedBJets);
        
        EDSI_SystVarUp[iSyst].EDSIDefaultVarVals();
        EDSI_SystVarDown[iSyst].EDSIDefaultVarVals();
        
        SmearEDSI_SystVarUp[iSyst].EDSIDefaultVarVals();
        SmearEDSI_SystVarDown[iSyst].EDSIDefaultVarVals();
        
        
        SmearEMI_PF_SystVarUp[iSyst].METType = 0;
        SmearEMI_PF_SystVarDown[iSyst].METType = 0;
        
        SmearHackEMI_PF_SystVarUp[iSyst].METType = 0;
        SmearHackEMI_PF_SystVarDown[iSyst].METType = 0;
        
        SmearHackEMI_PF_Old_SystVarUp[iSyst].METType = 0;
        SmearHackEMI_PF_Old_SystVarDown[iSyst].METType = 0;
    }
    float METSig_Raw;
    float AddSmearMETX, AddSmearMETY;
    
    float METX_Corr, METY_Corr, MET_Corr, MET_Phi_Corr;
    float METX_Corr_Old, METY_Corr_Old, MET_Corr_Old, MET_Phi_Corr_Old;
    
    //SUSY particle Gen Mass stuff    
    float genStopMassCut = 0., genChi0MassCut = 0.;
    float genCharginoMassCut = 0.;
    
    bool doBugCheckSignal = false;
    
    GenJetEventPointers GJEPs;
    GJEPs.InitializeVecs();
    
    EventGenLeptonParticleInfo EGLPI;
    EGLPI.InitializeVecs();
    EGLPI.EGLPIDefaultVarVals();
    
    EventGenSUSYParticleInfo EGSPI;
    EGSPI.InitializeVecs();
    EGSPI.EGSPIDefaultVarVals();
    
    EventGenQuarkParticleInfo EGQPI;
    EGQPI.InitializeVecs();
    
    EventGenEWKParticleInfo EGEWKPI;
    EGEWKPI.DefaultVarVals();
    EGEWKPI.InitializeVecs();
    
    
    float genMET, genMETPhi;
    
    bool doEvent, doEvent_LepESDown, doEvent_LepESUp;    
    
    float weight;
    
    PlotMakingRunParams PMRP;
    PMRP.DefaultVarVals();
    PMRP.SetVals(argc, argv);
    PMRP.SetStrings(1);
    
    char Buffer[500];
    ifstream * outDirFile;
    TRegexp fCutSlash("[^/]+$");
    fOutName = "";
    if (PMRP.PSIV.grabOutDir) {
        outDirFile = new ifstream(outputSavePathString + TString(".txt"));
        if (!(outDirFile->eof())) {
            outDirFile->getline(Buffer,500);
            fOutName += TString(string(Buffer));
            fOutName += "/"; //in case user forgot a slash
        }
    }
    fOutName += PMRP.PSIV.fInName(fCutSlash);
    if (!PMRP.SMV.doData) {
        h_CutFlow_LepESUp = new TH1F("h_CutFlow_LepESUp", "; Cut Stage; N_{evts} passing Cut Stage", 3, 0.5, 3.5);
        h_CutFlow_LepESDown = new TH1F("h_CutFlow_LepESDown", "; Cut Stage; N_{evts} passing Cut Stage", 3, 0.5, 3.5);
    }
    
    fOutName += PMRP.OutStringAdd();
    fOutName += "_SkimOutput.root";
    cout << "saving to " << fOutName << endl;
    TFile * outputFile;
    outputFile = new TFile(fOutName, "RECREATE");
    fileOutTreeName = "Ovi";
    fileOutTreeName += "SkimTree";
    TTree * outTree = new TTree(fileOutTreeName, fileOutTreeName);
    
    fileInTreeName = "Tree";
    TChain fileTree(fileInTreeName);
    TFile inputFile(PMRP.PSIV.fInName + TString(".root"));
    
    if (!PMRP.SMV.doData) {
        /*
         METXSmearHist2D          = SmearHist(PMRP.PSIV.fInName, &METXSmearMap, METXSmearHistVec);
         METXSmearJERUpHist2D     = SmearHist(PMRP.PSIV.fInName, &METXSmearMap, METXSmearJERUpHistVec);
         METXSmearJERDownHist2D   = SmearHist(PMRP.PSIV.fInName, &METXSmearMap, METXSmearJERDownHistVec);
         METXSmearUncESUpHist2D   = SmearHist(PMRP.PSIV.fInName, &METXSmearMap, METXSmearUncESUpHistVec);
         METXSmearUncESDownHist2D = SmearHist(PMRP.PSIV.fInName, &METXSmearMap, METXSmearUncESDownHistVec);
         
         METYSmearHist2D          = SmearHist(PMRP.PSIV.fInName, &METYSmearMap, METYSmearHistVec);
         METYSmearJERUpHist2D     = SmearHist(PMRP.PSIV.fInName, &METYSmearMap, METYSmearJERUpHistVec);
         METYSmearJERDownHist2D   = SmearHist(PMRP.PSIV.fInName, &METYSmearMap, METYSmearJERDownHistVec);
         METYSmearUncESUpHist2D   = SmearHist(PMRP.PSIV.fInName, &METYSmearMap, METYSmearUncESUpHistVec);
         METYSmearUncESDownHist2D = SmearHist(PMRP.PSIV.fInName, &METYSmearMap, METYSmearUncESDownHistVec);
         
         cout << "METXSmearHist2D->Integral() > 0? " << METXSmearHist2D->Integral() << endl;
         doSmearHack = (METXSmearHist2D->Integral() > 0);
         vecOneDeeMETXSmearHist          = OneDProjectionReturnVec(METXSmearHist2D,          2, 2, 1, 1, 2, "METXSmearHist");
         vecOneDeeMETXSmearJERUpHist     = OneDProjectionReturnVec(METXSmearJERUpHist2D,     2, 2, 1, 1, 2, "METXSmearJERUpHist");
         vecOneDeeMETXSmearJERDownHist   = OneDProjectionReturnVec(METXSmearJERDownHist2D,   2, 2, 1, 1, 2, "METXSmearJERDownHist");        
         vecOneDeeMETXSmearUncESUpHist   = OneDProjectionReturnVec(METXSmearUncESUpHist2D,   2, 2, 1, 1, 2, "METXSmearUncESUpHist");
         vecOneDeeMETXSmearUncESDownHist = OneDProjectionReturnVec(METXSmearUncESDownHist2D, 2, 2, 1, 1, 2, "METXSmearUncESDownHist");
         
         doSmearHack = (METYSmearHist2D->Integral() > 0);
         vecOneDeeMETYSmearHist          = OneDProjectionReturnVec(METYSmearHist2D,          2, 2, 1, 1, 2, "METYSmearHist");
         vecOneDeeMETYSmearJERUpHist     = OneDProjectionReturnVec(METYSmearJERUpHist2D,     2, 2, 1, 1, 2, "METYSmearJERUpHist");
         vecOneDeeMETYSmearJERDownHist   = OneDProjectionReturnVec(METYSmearJERDownHist2D,   2, 2, 1, 1, 2, "METYSmearJERDownHist");        
         vecOneDeeMETYSmearUncESUpHist   = OneDProjectionReturnVec(METYSmearUncESUpHist2D,   2, 2, 1, 1, 2, "METYSmearUncESUpHist");
         vecOneDeeMETYSmearUncESDownHist = OneDProjectionReturnVec(METYSmearUncESDownHist2D, 2, 2, 1, 1, 2, "METYSmearUncESDownHist");
         */
        
        // Set up the TH2Fs to use based on the input file
        METSmearHist2D_HighJet          = SmearHist(PMRP.PSIV.fInName, &METSmearMap, METSmearHistVec_HighJet);
        METSmearHist2D_LowJet          = SmearHist(PMRP.PSIV.fInName, &METSmearMap, METSmearHistVec_LowJet);
        METSmearJERUpHist2D_HighJet     = SmearHist(PMRP.PSIV.fInName, &METSmearMap, METSmearJERUpHistVec_HighJet);
        METSmearJERUpHist2D_LowJet     = SmearHist(PMRP.PSIV.fInName, &METSmearMap, METSmearJERUpHistVec_LowJet);
        METSmearJERDownHist2D_HighJet   = SmearHist(PMRP.PSIV.fInName, &METSmearMap, METSmearJERDownHistVec_HighJet);
        METSmearJERDownHist2D_LowJet   = SmearHist(PMRP.PSIV.fInName, &METSmearMap, METSmearJERDownHistVec_LowJet);
        METSmearUncESUpHist2D_HighJet   = SmearHist(PMRP.PSIV.fInName, &METSmearMap, METSmearUncESUpHistVec_HighJet);
        METSmearUncESUpHist2D_LowJet   = SmearHist(PMRP.PSIV.fInName, &METSmearMap, METSmearUncESUpHistVec_LowJet);
        METSmearUncESDownHist2D_HighJet = SmearHist(PMRP.PSIV.fInName, &METSmearMap, METSmearUncESDownHistVec_HighJet);
        METSmearUncESDownHist2D_LowJet = SmearHist(PMRP.PSIV.fInName, &METSmearMap, METSmearUncESDownHistVec_LowJet);
        
        METPhiSmearHist2D_HighJet          = SmearHist(PMRP.PSIV.fInName, &METPhiSmearMap, METPhiSmearHistVec_HighJet);
        METPhiSmearHist2D_LowJet          = SmearHist(PMRP.PSIV.fInName, &METPhiSmearMap, METPhiSmearHistVec_LowJet);
        METPhiSmearJERUpHist2D_HighJet     = SmearHist(PMRP.PSIV.fInName, &METPhiSmearMap, METPhiSmearJERUpHistVec_HighJet);
        METPhiSmearJERUpHist2D_LowJet     = SmearHist(PMRP.PSIV.fInName, &METPhiSmearMap, METPhiSmearJERUpHistVec_LowJet);
        METPhiSmearJERDownHist2D_HighJet   = SmearHist(PMRP.PSIV.fInName, &METPhiSmearMap, METPhiSmearJERDownHistVec_HighJet);
        METPhiSmearJERDownHist2D_LowJet   = SmearHist(PMRP.PSIV.fInName, &METPhiSmearMap, METPhiSmearJERDownHistVec_LowJet);
        METPhiSmearUncESUpHist2D_HighJet   = SmearHist(PMRP.PSIV.fInName, &METPhiSmearMap, METPhiSmearUncESUpHistVec_HighJet);
        METPhiSmearUncESUpHist2D_LowJet   = SmearHist(PMRP.PSIV.fInName, &METPhiSmearMap, METPhiSmearUncESUpHistVec_LowJet);
        METPhiSmearUncESDownHist2D_HighJet = SmearHist(PMRP.PSIV.fInName, &METPhiSmearMap, METPhiSmearUncESDownHistVec_HighJet);
        METPhiSmearUncESDownHist2D_LowJet = SmearHist(PMRP.PSIV.fInName, &METPhiSmearMap, METPhiSmearUncESDownHistVec_LowJet);
        
        // sanity check to make sure that you loaded in a valid TH2F
        cout << "METSmearHist2D->Integral() > 0? " << METSmearHist2D_HighJet->Integral() << endl;
        cout << "METSmearHist2D->Integral() > 0? " << METSmearHist2D_LowJet->Integral() << endl;
        doSmearHack = (METSmearHist2D_HighJet->Integral() > 0);
        
        // chop the TH2Fs into TH1Fs and place into the vectors
        vecOneDeeMETSmearHist_HighJet          = OneDProjectionReturnVec(METSmearHist2D_HighJet,          2, 2, 1, 1, 2, "METSmearHist_HighJet");
        vecOneDeeMETSmearJERUpHist_HighJet     = OneDProjectionReturnVec(METSmearJERUpHist2D_HighJet,     2, 2, 1, 1, 2, "METSmearJERUpHist_HighJet");
        vecOneDeeMETSmearJERDownHist_HighJet   = OneDProjectionReturnVec(METSmearJERDownHist2D_HighJet,   2, 2, 1, 1, 2, "METSmearJERDownHist_HighJet");        
        vecOneDeeMETSmearUncESUpHist_HighJet   = OneDProjectionReturnVec(METSmearUncESUpHist2D_HighJet,   2, 2, 1, 1, 2, "METSmearUncESUpHist_HighJet");
        vecOneDeeMETSmearUncESDownHist_HighJet = OneDProjectionReturnVec(METSmearUncESDownHist2D_HighJet, 2, 2, 1, 1, 2, "METSmearUncESDownHist_HighJet");
        
        vecOneDeeMETSmearHist_LowJet          = OneDProjectionReturnVec(METSmearHist2D_LowJet,          2, 2, 1, 1, 2, "METSmearHist_LowJet");
        vecOneDeeMETSmearJERUpHist_LowJet     = OneDProjectionReturnVec(METSmearJERUpHist2D_LowJet,     2, 2, 1, 1, 2, "METSmearJERUpHist_LowJet");
        vecOneDeeMETSmearJERDownHist_LowJet   = OneDProjectionReturnVec(METSmearJERDownHist2D_LowJet,   2, 2, 1, 1, 2, "METSmearJERDownHist_LowJet");        
        vecOneDeeMETSmearUncESUpHist_LowJet   = OneDProjectionReturnVec(METSmearUncESUpHist2D_LowJet,   2, 2, 1, 1, 2, "METSmearUncESUpHist_LowJet");
        vecOneDeeMETSmearUncESDownHist_LowJet = OneDProjectionReturnVec(METSmearUncESDownHist2D_LowJet, 2, 2, 1, 1, 2, "METSmearUncESDownHist_LowJet");
        
        vecOneDeeMETPhiSmearHist_HighJet          = OneDProjectionReturnVec(METPhiSmearHist2D_HighJet,          2, 2, 1, 1, 2, "METPhiSmearHist_HighJet");
        vecOneDeeMETPhiSmearJERUpHist_HighJet     = OneDProjectionReturnVec(METPhiSmearJERUpHist2D_HighJet,     2, 2, 1, 1, 2, "METPhiSmearJERUpHist_HighJet");
        vecOneDeeMETPhiSmearJERDownHist_HighJet   = OneDProjectionReturnVec(METPhiSmearJERDownHist2D_HighJet,   2, 2, 1, 1, 2, "METPhiSmearJERDownHist_HighJet");        
        vecOneDeeMETPhiSmearUncESUpHist_HighJet   = OneDProjectionReturnVec(METPhiSmearUncESUpHist2D_HighJet,   2, 2, 1, 1, 2, "METPhiSmearUncESUpHist_HighJet");
        vecOneDeeMETPhiSmearUncESDownHist_HighJet = OneDProjectionReturnVec(METPhiSmearUncESDownHist2D_HighJet, 2, 2, 1, 1, 2, "METPhiSmearUncESDownHist_HighJet");
        
        vecOneDeeMETPhiSmearHist_LowJet          = OneDProjectionReturnVec(METPhiSmearHist2D_LowJet,          2, 2, 1, 1, 2, "METPhiSmearHist_LowJet");
        vecOneDeeMETPhiSmearJERUpHist_LowJet     = OneDProjectionReturnVec(METPhiSmearJERUpHist2D_LowJet,     2, 2, 1, 1, 2, "METPhiSmearJERUpHist_LowJet");
        vecOneDeeMETPhiSmearJERDownHist_LowJet   = OneDProjectionReturnVec(METPhiSmearJERDownHist2D_LowJet,   2, 2, 1, 1, 2, "METPhiSmearJERDownHist_LowJet");        
        vecOneDeeMETPhiSmearUncESUpHist_LowJet   = OneDProjectionReturnVec(METPhiSmearUncESUpHist2D_LowJet,   2, 2, 1, 1, 2, "METPhiSmearUncESUpHist_LowJet");
        vecOneDeeMETPhiSmearUncESDownHist_LowJet = OneDProjectionReturnVec(METPhiSmearUncESDownHist2D_LowJet, 2, 2, 1, 1, 2, "METPhiSmearUncESDownHist_LowJet");
    }
    
    if (PMRP.PSIV.useOldNTuple) {
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
        cout << "HACKING OUTPUT TO OLD " << endl;
    }
    
    
    
    //////////////////////////
    fileTree.Add(PMRP.PSIV.fInName + TString(".root"));
    BTagSFUtilBase = new BTagSFUtil("CSVM");
    
    SetInTreeBranchRecoElec(&fileTree, &EEPs);
    
    SetInTreeBranchRecoMuon(&fileTree, &MEPs);
    SetInTreeBranchRecoJet(&fileTree, &PFJEPs);
    if (!PMRP.SMV.doData && !PMRP.PSIV.useOldNTuple) {
        SetInTreeBranchRecoJet(&fileTree, &SmearPFJEPs, true);
    }
    //Vertex information
    fileTree.SetBranchAddress("T_Vertex_z",      &VertZ);
    fileTree.SetBranchAddress("T_Vertex_ndof",   &VertNDOF);
    fileTree.SetBranchAddress("T_Vertex_rho",    &VertRho);
    fileTree.SetBranchAddress("T_Vertex_isFake", &VertIsFake);
    
    //MET information
    if (PMRP.PSIV.useOldNTuple) {
        SetInTreeBranchMETOld(&fileTree, &EMI_PF_noPhiCorr);
    }
    else {
        SetInTreeBranchMET(&fileTree, &EMI_PF_noPhiCorr);
        SetInTreeBranchMETOld(&fileTree, &EMI_PF_noPhiCorr_Old);
    }
    if (!PMRP.PSIV.useOldNTuple) {
        SetInTreeBranchMET(&fileTree, &EMI_Calo_noPhiCorr);
    }
    if (!PMRP.SMV.doData && !PMRP.PSIV.useOldNTuple) {
        //            SetInTreeBranchMETSmeared(&fileTree, &SmearEMI_PF, 0);
        SetInTreeBranchMETSmeared(&fileTree, &SmearEMI_PF_noPhiCorr, 0);
        /*
         SetInTreeBranchMETSmeared(&fileTree, &SmearEMI_PF_SystVarUp[systLepES],    1 * systLepES);
         SetInTreeBranchMETSmeared(&fileTree, &SmearEMI_PF_SystVarDown[systLepES], -1 * systLepES);
         */
        SetInTreeBranchMETSmeared(&fileTree, &SmearEMI_PF_SystVarUp[systJetES],    1 * systJetES);
        SetInTreeBranchMETSmeared(&fileTree, &SmearEMI_PF_SystVarDown[systJetES], -1 * systJetES);
        SetInTreeBranchMETSmeared(&fileTree, &SmearEMI_PF_SystVarUp[systJetSmear],    1 * systJetSmear);
        SetInTreeBranchMETSmeared(&fileTree, &SmearEMI_PF_SystVarDown[systJetSmear], -1 * systJetSmear);
        SetInTreeBranchMETSmeared(&fileTree, &SmearEMI_PF_SystVarUp[systUncES],    1 * systUncES);
        SetInTreeBranchMETSmeared(&fileTree, &SmearEMI_PF_SystVarDown[systUncES], -1 * systUncES);
    }
    //run information
    fileTree.SetBranchAddress( "T_Event_RunNumber",       &BEI.runNumber );
    fileTree.SetBranchAddress( "T_Event_EventNumber",     &BEI.eventNumber );
    fileTree.SetBranchAddress( "T_Event_LuminosityBlock", &BEI.lumiBlock );
    
    fileTree.SetBranchAddress( "T_Event_nTruePU",   &EPI.nPUTrue );
    fileTree.SetBranchAddress( "T_Event_nPU",       &EPI.nPUBX0 );
    fileTree.SetBranchAddress( "T_Event_nPUp",      &EPI.nPUBXP );
    fileTree.SetBranchAddress( "T_Event_nPUm",      &EPI.nPUBXM );  
    
    fileTree.SetBranchAddress( "T_METPF_Sig",       &METSig_Raw);
    //event Rho variable (used for electron Isolation)        
    fileTree.SetBranchAddress( "T_Event_RhoIso", &EPI.eventRhoIso );
    
    SetInTreeBranchFilterTrigger(&fileTree, &FTI);
    if (!PMRP.SMV.doData) {
        //// Generator information
        /// Gen MET info
        fileTree.SetBranchAddress( "T_METgen_ET",                   &genMET );
        fileTree.SetBranchAddress( "T_METgen_Phi",                  &genMETPhi );
        
        SetInTreeBranchGenJet(&fileTree, &GJEPs);
        SetInTreeBranchGenQuark(&fileTree, &EGQPI.GQEPs);
        SetInTreeBranchGenLep(&fileTree, &EGLPI.GLEPs);
        SetInTreeBranchSUSY(&fileTree, &EGSPI.GSEPs);
        SetInTreeBranchGenEWK(&fileTree, &EGEWKPI.GEWKEPs);
    }
    ///Out tree information
    outTree->Branch( "T_METPF_Sig",       &METSig_Raw);
    if (PMRP.PSIV.makeGenJetCheck) {
        outTree->Branch("T_VecDeltaEnUnsmearedGenMatch", &vecDeltaEnUnsmearJetGenMatchJet);
        outTree->Branch("T_VecUnsmearEn", &vecGoodUnsmearJetEn);
        
        outTree->Branch("T_VecDeltaEnUnsmearedGenMatch_SmearMatch", &vecDeltaEnUnsmearJetGenMatchJet_SmearMatch);
        outTree->Branch("T_VecUnsmearEn_SmearMatch", &vecGoodUnsmearJetEn_SmearMatch);
        outTree->Branch("T_VecDeltaEnUnsmearJetSmearJet_GenMatch", &vecDeltaEnUnsmearJetSmearJet_GenMatch);
        outTree->Branch("T_VecDeltaEnUnsmearJetSmearJet_NotGenMatch", &vecDeltaEnUnsmearJetSmearJet_NotGenMatch);
    }
    SetOutTreeBranchBasicInfo(outTree, &FTI, &EPI, &BEI, &EGW);
    SetOutTreeBranchMETInfo(outTree, &EMI_PF_noPhiCorr, 0, false, false);
    SetOutTreeBranchMETInfo(outTree, &EMI_PF_noPhiCorr_Old, 0, false, false, "_noType0");
    SetOutTreeBranchMETInfo(outTree, &EMI_PF, 0, false, true);
    SetOutTreeBranchMETInfo(outTree, &EMI_PF_Old, 0, false, true, "_noType0");
    SetOutTreeBranchMETInfo(outTree, &EMI_Calo_noPhiCorr, 0, false, false);
    SetOutTreeBranchMETInfo(outTree, &EMI_Calo, 0, false, true);
    SetOutTreeBranchLeptonInfo(outTree, &ELI, 0, PMRP.SMV.doData, PMRP.SMV.keepLooseLeps);
    SetOutTreeBranchJetInfo(outTree, &EJI, 0, false);
    //    SetOutTreeBranchDiStructureInfo(outTree, &EDSI_noPhiCorr, 0, false, false);
    SetOutTreeBranchDiStructureInfo(outTree, &EDSI, 0, false, true);
    
    /*
     not done yet
     SetOutTreeBranchPassesCutInfo(outTree, passesFullSelection, 0);    
     */
    if (PMRP.SRS.isSignal) {
        if (PMRP.PSIV.fInName.Contains("T2tt")) {
            BTagSFSignalString = "T2tt";
        }
        else if (PMRP.PSIV.fInName.Contains("T2bw")) {
            BTagSFSignalString = "T2bw";
        }
        else {
            BTagSFSignalString = "";
        }
        doBugCheckSignal = (PMRP.PSIV.fInName.Contains("FineBin") || PMRP.PSIV.fInName.Contains("LSP0"));        
        BTagSFUtilSignal = new BTagSFUtil(BTagSFAlgorithm, BTagSFDataPeriod, BTagSFSignalString);
        BTagSFUtilToUse = BTagSFUtilSignal;
        
        SetOutTreeBranchSUSYInfo(outTree, &EGSPI);
        
        WeightHistToUse3D = FastSimWeightHist;
        TrueWeightHistToUse1D = FastSimTrueWeightHist;
    }
    else {
        BTagSFUtilToUse = BTagSFUtilBase;
        WeightHistToUse3D = FullSimWeightHist;
        TrueWeightHistToUse1D = FullSimTrueWeightHist;
    }
    if (!PMRP.SMV.doData) {
        SetOutTreeBranchMETInfo(outTree, &SmearEMI_PF_noPhiCorr, 0, true, false);
        SetOutTreeBranchMETInfo(outTree, &SmearEMI_PF, 0, true, true);
        SetOutTreeBranchSpecialMETInfo(outTree, &SmearHackEMI_PF, 0, true, true);
        SetOutTreeBranchSpecialMETInfo(outTree, &SmearHackEMI_PF_Old, 0, true, true, "_noType0");
        SetOutTreeBranchJetInfo(outTree, &SmearEJI, 0, true);
        SetOutTreeBranchDiStructureInfo(outTree, &SmearEDSI, 0, true, true);
        for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {
            if (InSystBound(iSyst, &systLB_Lepton, &systUB_Lepton)) {
                SetOutTreeBranchLeptonInfo(outTree, &ELI_SystVarUp[iSyst],    1 * iSyst, PMRP.SMV.doData);
                SetOutTreeBranchLeptonInfo(outTree, &ELI_SystVarDown[iSyst], -1 * iSyst, PMRP.SMV.doData);
            }            
            if (InSystBound(iSyst, &systLB_Jet, &systUB_Jet)) {
                SetOutTreeBranchJetInfo(outTree, &EJI_SystVarUp[iSyst],    1 * iSyst, false);
                SetOutTreeBranchJetInfo(outTree, &EJI_SystVarDown[iSyst], -1 * iSyst, false);
            }
            //            cout << "iSyst b" << iSyst << endl;
            if (InSystBound(iSyst, &systLB_MET, &systUB_MET)) {
                SetOutTreeBranchMETInfo(outTree, &EMI_PF_SystVarUp[iSyst],    1 * iSyst, false, true);
                SetOutTreeBranchMETInfo(outTree, &EMI_PF_SystVarDown[iSyst], -1 * iSyst, false, true);
            }
            //            cout << "iSyst c" << iSyst << endl;
            if (InSystBound(iSyst, &systLB_SmearMET, &systUB_SmearMET)) {
                SetOutTreeBranchMETInfo(outTree, &SmearEMI_PF_SystVarUp[iSyst],    1 * iSyst, true, true);
                SetOutTreeBranchMETInfo(outTree, &SmearEMI_PF_SystVarDown[iSyst], -1 * iSyst, true, true);
            }
            
            if (InSystBound(iSyst, &systLB_SmearMET, &systUB_SmearMET)) {
                SetOutTreeBranchSpecialMETInfo(outTree, &SmearHackEMI_PF_SystVarUp[iSyst],    1 * iSyst, true, true);
                SetOutTreeBranchSpecialMETInfo(outTree, &SmearHackEMI_PF_SystVarDown[iSyst], -1 * iSyst, true, true);
            }
            
            if (InSystBound(iSyst, &systLB_SmearMET, &systUB_SmearMET)) {
                SetOutTreeBranchSpecialMETInfo(outTree, &SmearHackEMI_PF_Old_SystVarUp[iSyst],    1 * iSyst, true, true, "_noType0");
                SetOutTreeBranchSpecialMETInfo(outTree, &SmearHackEMI_PF_Old_SystVarDown[iSyst], -1 * iSyst, true, true, "_noType0");
            }
            //            cout << "iSyst d" << iSyst << endl;
            if (InSystBound(iSyst, &systLB_SmearJet, &systUB_SmearJet)) {
                SetOutTreeBranchJetInfo(outTree, &SmearEJI_SystVarUp[iSyst],    1 * iSyst, true);
                SetOutTreeBranchJetInfo(outTree, &SmearEJI_SystVarDown[iSyst], -1 * iSyst, true);
            }
            
            if (InSystBound(iSyst, &systLB_EDSI, &systUB_EDSI)) {                
                SetOutTreeBranchDiStructureInfo(outTree, &EDSI_SystVarUp[iSyst],    1 * iSyst, false, true);
                SetOutTreeBranchDiStructureInfo(outTree, &EDSI_SystVarDown[iSyst], -1 * iSyst, false, true);
            }
            
            if (InSystBound(iSyst, &systLB_SmearEDSI, &systUB_SmearEDSI)) {                
                SetOutTreeBranchDiStructureInfo(outTree, &SmearEDSI_SystVarUp[iSyst],    1 * iSyst, true, true);
                SetOutTreeBranchDiStructureInfo(outTree, &SmearEDSI_SystVarDown[iSyst], -1 * iSyst, true, true);
            }
            /*
             not done yet
             if (InSystBound(iSyst, &systLB_SmearEDSI, &systUB_SmearEDSI)) {                
             SetOutTreeBranchPassesCutInfo(outTree, &vecPFS_SystVarUp[iSyst],    1 * iSyst);
             SetOutTreeBranchPassesCutInfo(outTree, &vecPFS_SystVarDown[iSyst], -1 * iSyst);
             }
             */
        }
        
        
        
        //        cout << "test5c " << endl;
        ///###GENMET Branches###/////
        outTree->Branch("TGenMET",                      &genMET);
        outTree->Branch("TGenMETPhi",                   &genMETPhi);                
        
        SetOutTreeBranchGenLepton(outTree, &EGLPI);
        SetOutTreeBranchGenQuark(outTree, &EGQPI);
    }
    cout << "--- Processing: " << fileTree.GetEntries() << " events" << endl;
    h_eventCount->Fill(1);
    h_eventCount->SetEntries(fileTree.GetEntries());
    outputFile->cd();    
    
    vector<PFJet> Jets;     // Will contain all jets not overlapping with isolated electrons or muons
    vector<Lepton> * vecIsoLeptons;
    TLorentzVector patsyVec;
    
    
    vector< vector<Lepton> *> vecVecIsoLeptons_SystVarUp, vecVecIsoLeptons_SystVarDown, vecVecIsoLeptonsCentValMETPatsy_SystVarUp, vecVecIsoLeptonsCentValMETPatsy_SystVarDown;
    
    vecVecIsoLeptons_SystVarUp.resize(numSysts + 1); 
    vecVecIsoLeptons_SystVarDown.resize(numSysts + 1);
    vecVecIsoLeptonsCentValMETPatsy_SystVarUp.resize(numSysts + 1);
    vecVecIsoLeptonsCentValMETPatsy_SystVarDown.resize(numSysts + 1);
    
    
    vector< vector<PFJet> > vecVecGoodJets_SystVarUp, vecVecGoodJets_SystVarDown;
    vecVecGoodJets_SystVarUp.resize(numSysts + 1);
    vecVecGoodJets_SystVarDown.resize(numSysts + 1);
    
    
    
    //float SmearJetSF = 1.00;
    //    float SmearJetSF = 1.05;
    //    float SmearJetSF = TMath::Sqrt(2);
    //    float SmearJetSF = 1.3;
    float SmearJetSF = 1.15;
    vector<SmearPFJet> SmearJets; vector<SmearPFJet> SmearJets_Scaled;
    
    vector< vector<SmearPFJet>> vecVecGoodSmearJets_SystVarUp, vecVecGoodSmearJets_SystVarDown;
    vecVecGoodSmearJets_SystVarUp.resize(numSysts + 1);
    vecVecGoodSmearJets_SystVarDown.resize(numSysts + 1);    
    
    vector<GenJet> * vecGoodGenJets;
    
    float roundNum = 25.0;
    int roundMult = 25;
    if (PMRP.SRS.isSignal) {
        if (PMRP.PSIV.fInName.Contains("FineBin")) {
            roundNum = 10.0;
            roundMult = 10;
        }
    }
    /////Iterate over events  
    //    for (Long64_t ievt=0; ievt < 10;ievt++) {
    int modFactor = 1E4;
    //    int modFactor = 1;
    if (PMRP.RRL.nEvents < 0) {
        cout << "running over all events " << endl;  
        PMRP.RRL.nEvents = fileTree.GetEntries();
    }
    else {
        if (PMRP.RRL.nEvents > fileTree.GetEntries()) PMRP.RRL.nEvents = fileTree.GetEntries();        
        cout << "running on just " << PMRP.RRL.nEvents << " events " << endl;
    }
    if (PMRP.RRL.startPoint >= PMRP.RRL.nEvents) {
        PMRP.RRL.startPoint = PMRP.RRL.nEvents - 1;
    }
    if (PMRP.RRL.startPoint != 0) {
        cout << "starting on event " << PMRP.RRL.startPoint << endl;
    }
    if (PMRP.PSIV.levelJetVerbosity > 0 || PMRP.PSIV.levelLepVerbosity > 0) modFactor = 1;
    
    
    genWeightType = setGenRWType(PMRP.PSIV.fInName, &mapGenRW);
    cout << "genWeightType " << genWeightType << endl;
    
    
    for (Long64_t ievt = PMRP.RRL.startPoint; ievt < PMRP.RRL.nEvents; ievt++) {
        if (ievt%modFactor == 0) cout << ievt << endl;
        //    for (Long64_t ievt=0; ievt<100;ievt++) {
        vecIsoLeptons = new vector<Lepton>;
        //        Jets = new vector<PFJet>;
        doEvent = true;        
        if (!PMRP.SMV.doData) {
            
            
            for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {
                if (InSystBound(iSyst, &systLB_Lepton, &systUB_Lepton)) {            
                    vecVecIsoLeptonsCentValMETPatsy_SystVarUp[iSyst] = new vector<Lepton>;                    
                    vecVecIsoLeptonsCentValMETPatsy_SystVarDown[iSyst] = new vector<Lepton>;                    
                    vecVecIsoLeptons_SystVarUp[iSyst] = new vector<Lepton>;
                    vecVecIsoLeptons_SystVarDown[iSyst] = new vector<Lepton>;
                }
            }
            
            doEvent_LepESUp = true;
            doEvent_LepESDown = true;
        }
        
        
        //        map<string, float> stringKeyToVar;
        fileTree.GetEntry(ievt);
        
        MEPs.SetSize();
        EEPs.SetSize();
        PFJEPs.SetSize();
        if (!PMRP.SMV.doData) {
            SmearPFJEPs.SetSize();
            EGQPI.SetSize();
            EGLPI.SetSize();
            EGEWKPI.SetSize();
            if (PMRP.SRS.isSignal) {
                EGSPI.SetSize();
            }
        }
        
        if (PMRP.SRS.isSignal) {
            if (doBugCheckSignal) {
                if (!EGSPI.PassesStopBugCheck()) continue;
            }
            EGSPI.SetMasses(roundNum, roundMult);
            if (PMRP.SRS.doMassCutSkim) {
                if (!EGSPI.PassMassCut(genStopMassCut, genChi0MassCut, roundNum)) continue;
            }
            EGSPI.SetSUSYParticles(PMRP.PSIV.fInName.Contains("T2bw"));
        }
        if (PMRP.PSIV.printEventNum && PMRP.SRS.isSignal) {
            EGSPI.PrintMassInfo(BEI.eventNumber, PMRP.PSIV.fInName.Contains("T2tt"));
        }
        else if (PMRP.PSIV.printEventNum) {
            cout << "Event Prior to Selections: RunNum:EventNum:LumiBlock " << BEI.runNumber  << ":" << BEI.eventNumber << ":" << BEI.lumiBlock << endl;
        }
        firstGoodVertZ = goodVertexSelection(VertZ, VertRho, VertNDOF,VertIsFake, EPI.nVtx);
        if (EPI.nVtx < 1) {
            cout << "failed vertex cut " << endl; 
            continue;
        }
        weight = 1.;
        //            cout << "test a " << endl;
        if (!PMRP.SMV.doData) {
            EGLPI.SetParticleInfo();
            //                cout << "test b " << endl;
            EGQPI.SetParticleInfo();
            //                cout << "test c " << endl;
            EGEWKPI.SetParticleInfo();
            //                cout << "test d " << endl;
        }
        /*
         for (unsigned int i = 0; i < genStopSt3En->size(); ++i) {
         float E = genStopSt3En->at(i);
         float pt = genStopSt3Pt->at(i);
         float eta = genStopSt3Eta->at(i);
         float phi = genStopSt3Phi->at(i);
         float M;
         if (i == 0) {
         M = genStopMass0;
         }
         else if (i == 1) {
         M = genStopMass1;
         }
         else {
         M = 0;
         }
         cout << "i " << i << endl;
         cout << "curr energy " << E << endl;
         cout << "curr pt " << pt << endl;
         cout << "curr phi " << phi << endl;
         cout << "curr eta " << eta << endl;
         cout << "curr mass " << M << endl;
         float ESq = E * E;                
         float MSq = M * M;
         float PZ = pt * TMath::SinH(eta);
         float PSq = PZ * PZ + pt * pt;
         cout << "Esq " << ESq << endl;
         cout << "MSq " << MSq << endl;
         cout << "PSq " << PSq << endl;
         float diff = ESq - MSq - PSq;
         cout << "test: " << diff <<endl;
         }
         */
        if (PMRP.SMV.doData) {
            FTI.SetVecs();
            doEvent = FilterMET(&FTI.regularMETFilterVec, &FTI.oppositeMETFilterVec);
            //                cout << "For RunNum " << RunNum << ", doEvent before? " << doEvent << endl;
            doEvent &= BadTrigRunCheck(BEI.runNumber);
            //                cout << "doEvent after? " << doEvent << endl;
        }        
        h_CutFlow->Fill(1.);
        if (!PMRP.SMV.doData) {
            h_CutFlow_LepESUp->Fill(1);
            h_CutFlow_LepESDown->Fill(1);
        }
        if (!doEvent) continue;
        if (PMRP.RRL.doSpecRun) {
            if (BEI.runNumber == (unsigned int) PMRP.RRL.whichRun) {
                cout << "Pre LepCut Run:Event:Lumi " << BEI.runNumber << ":" << BEI.eventNumber << ":" << BEI.lumiBlock << endl;
            }
            else {
                continue;
            }
        }
        else if (PMRP.RRL.doSpecRunEvent) {
            if (!(BEI.runNumber == (unsigned int) PMRP.RRL.whichRun && BEI.eventNumber == (unsigned int) PMRP.RRL.whichEvent)) {
                continue;
            }
            else {
                cout << "Pre LepCut Run:Event:Lumi " << BEI.runNumber << ":" << BEI.eventNumber << ":" << BEI.lumiBlock << endl;
            }
        }
        h_CutFlow->Fill(2.);
        if (!PMRP.SMV.doData) {
            h_CutFlow_LepESUp->Fill(2.);
            h_CutFlow_LepESDown->Fill(2.);
        }
        EMI_Calo.SetVars(&EMI_Calo_noPhiCorr);
        EMI_Calo.PhiCorrection(PMRP.SMV.doData, EPI.nVtx, false, false);
        EMI_PF.SetVars(&EMI_PF_noPhiCorr);
        EMI_PF.PhiCorrection(PMRP.SMV.doData, EPI.nVtx, false, PMRP.PSIV.useOldNTuple);
        EMI_PF_Old.SetVars(&EMI_PF_noPhiCorr_Old);
        EMI_PF_Old.PhiCorrection(PMRP.SMV.doData, EPI.nVtx, false, true);
        if (!PMRP.SMV.doData) {
            for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {
                EMI_PF_SystVarUp[iSyst].SetVars(&EMI_PF);
                EMI_PF_SystVarDown[iSyst].SetVars(&EMI_PF);
            }
        }
//        ElectronPickOvi(EEPs, EPI.eventRhoIso, vecIsoLeptons, PMRP.PSIV.levelLepVerbosity);
        ElectronPickOviNew(EEPs, EPI.eventRhoIso, vecIsoLeptons, &ECI_Main, PMRP.SMV.keepLooseLeps, &ECI_Loose, PMRP.PSIV.levelLepVerbosity);
        MuonPickOviNew(MEPs, firstGoodVertZ, vecIsoLeptons, PMRP.SMV.keepLooseLeps, PMRP.PSIV.levelLepVerbosity);
        sort(vecIsoLeptons->begin(), vecIsoLeptons->end(), greater<Lepton>());
        LeptonPair(&ELI, vecIsoLeptons, PMRP.PSIV.levelLepVerbosity);
        doEvent = ELI.doEvent;
        if (!PMRP.SMV.doData) {              
            for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {
                if (InSystBound(iSyst, &systLB_Lepton, &systUB_Lepton)) {
                    systLepESShiftUp =  iSyst == systLepES;
                    systLepESShiftDown = -1 * systLepESShiftUp;                      
//                    ElectronPickOviSyst(EEPs, EPI.eventRhoIso,  systLepESShiftUp, vecVecIsoLeptons_SystVarUp[iSyst], PMRP.PSIV.levelLepVerbosity, vecVecIsoLeptonsCentValMETPatsy_SystVarUp[iSyst]); 
//                    MuonPickOviSyst(MEPs, firstGoodVertZ, systLepESShiftUp, vecVecIsoLeptons_SystVarUp[iSyst], PMRP.PSIV.levelLepVerbosity, vecVecIsoLeptonsCentValMETPatsy_SystVarUp[iSyst]);
                    ElectronPickOviNew(EEPs, EPI.eventRhoIso, vecVecIsoLeptons_SystVarUp[iSyst], &ECI_Main, PMRP.SMV.keepLooseLeps, &ECI_Loose, PMRP.PSIV.levelLepVerbosity, systLepESShiftUp, vecVecIsoLeptonsCentValMETPatsy_SystVarUp[iSyst]); 
                    MuonPickOviNew(MEPs, firstGoodVertZ, vecVecIsoLeptons_SystVarUp[iSyst], PMRP.SMV.keepLooseLeps, PMRP.PSIV.levelLepVerbosity, systLepESShiftUp, vecVecIsoLeptonsCentValMETPatsy_SystVarUp[iSyst]);                                                                        
                    sort(vecVecIsoLeptons_SystVarUp[iSyst]->begin(), vecVecIsoLeptons_SystVarUp[iSyst]->end(), greater<Lepton>());
                    LeptonPair(&ELI_SystVarUp[iSyst], vecVecIsoLeptons_SystVarUp[iSyst], PMRP.PSIV.levelLepVerbosity);
                    //                        EMI_PF_SystVarUp[iSyst].SetVars(&EMI_PF);
                    EMI_PF_SystVarUp[iSyst].SystShiftMET(vecVecIsoLeptonsCentValMETPatsy_SystVarUp[iSyst], vecVecIsoLeptons_SystVarUp[iSyst], &EMI_PF);                        
                    
                    ElectronPickOviNew(EEPs, EPI.eventRhoIso, vecVecIsoLeptons_SystVarDown[iSyst], &ECI_Main, PMRP.SMV.keepLooseLeps, &ECI_Loose, PMRP.PSIV.levelLepVerbosity, systLepESShiftDown, vecVecIsoLeptonsCentValMETPatsy_SystVarDown[iSyst]); 
                    MuonPickOviNew(MEPs, firstGoodVertZ, vecVecIsoLeptons_SystVarDown[iSyst], PMRP.SMV.keepLooseLeps, PMRP.PSIV.levelLepVerbosity, systLepESShiftDown, vecVecIsoLeptonsCentValMETPatsy_SystVarDown[iSyst]);
                    //ElectronPickOviSyst(EEPs, EPI.eventRhoIso, systLepESShiftDown, vecVecIsoLeptons_SystVarDown[iSyst], PMRP.PSIV.levelLepVerbosity, vecVecIsoLeptonsCentValMETPatsy_SystVarDown[iSyst]);                    
//                    MuonPickOviSyst(MEPs, firstGoodVertZ, systLepESShiftDown, vecVecIsoLeptons_SystVarDown[iSyst], PMRP.PSIV.levelLepVerbosity, vecVecIsoLeptonsCentValMETPatsy_SystVarDown[iSyst]);                          
                    sort(vecVecIsoLeptons_SystVarDown[iSyst]->begin(), vecVecIsoLeptons_SystVarDown[iSyst]->end(), greater<Lepton>());
                    LeptonPair(&ELI_SystVarDown[iSyst], vecVecIsoLeptons_SystVarDown[iSyst], PMRP.PSIV.levelLepVerbosity);
                    //                        EMI_PF_SystVarDown[iSyst].SetVars(&EMI_PF);
                    EMI_PF_SystVarDown[iSyst].SystShiftMET(vecVecIsoLeptonsCentValMETPatsy_SystVarDown[iSyst], vecVecIsoLeptons_SystVarDown[iSyst], &EMI_PF);
                }
            }
        }
        doEvent_LepESUp = ELI_SystVarUp[systLepES].doEvent;
        doEvent_LepESDown = ELI_SystVarDown[systLepES].doEvent;
        if (!doEvent) {
            if (PMRP.SMV.doData) { 
                continue;
            }
            else if (!doEvent_LepESUp && !doEvent_LepESDown) {
                continue;
            }
        }
        if (!PMRP.SMV.doData) {
            ELI.FindMatchingGenLeps(&EGLPI.GLEPs);
            for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {
                if (InSystBound(iSyst, &systLB_Lepton, &systUB_Lepton)) {
                    ELI_SystVarUp[iSyst].FindMatchingGenLeps(&EGLPI.GLEPs);
                    ELI_SystVarDown[iSyst].FindMatchingGenLeps(&EGLPI.GLEPs);
                }
            }
        }
        Jets.resize(0);
        JetInfo(&Jets, vecIsoLeptons, PFJEPs, &JCI,  0., h_JetESUp);            
        sort(Jets.begin(), Jets.end(), greater<PFJet>());
        JetKinematicsCut(&EJI, &Jets, BTagSFUtilToUse, PMRP.SMV.doData);
        if (!PMRP.SMV.doData) {
            if (!PMRP.PSIV.useOldNTuple) {
                ///            cout << "messing around with jets " << endl;
                SmearJets.resize(0);
                SmearJets_Scaled.resize(0);
                
                ///            cout << "about to try matching gen jets " << endl;
                
                GJEPs.SetSize();
                vecGoodGenJets = GenJetsNonZero(GJEPs);
                
                if (PMRP.PSIV.makeGenJetCheck) {
                    vecDeltaEnUnsmearJetSmearJet_GenMatch->resize(0);
                    vecDeltaEnUnsmearJetSmearJet_NotGenMatch->resize(0);
                    vecDeltaEnUnsmearJetGenMatchJet_SmearMatch->resize(0);
                    vecGoodUnsmearJetEn_SmearMatch->resize(0);
                    JetSmearMatchUnSmear(&SmearJets, SmearPFJEPs, &Jets, vecGoodGenJets, vecDeltaEnUnsmearJetSmearJet_GenMatch, vecDeltaEnUnsmearJetSmearJet_NotGenMatch, vecDeltaEnUnsmearJetGenMatchJet_SmearMatch, vecGoodUnsmearJetEn_SmearMatch);
                }
                else {
                    JetSmearMatchUnSmear(&SmearJets, SmearPFJEPs, &Jets, vecGoodGenJets, h_DeltaPtSmearUnsmear_GenMatched, h_DeltaPtSmearUnsmear_NotGenMatched);                    
                }
                Jet_ScaleUpSmearing_SF(&SmearJets, &SmearJets_Scaled, SmearJetSF);
                
                
                ///            cout << "test " << endl;
                JetKinematicsCutSmearedJet(&SmearEJI, &SmearJets_Scaled, BTagSFUtilToUse);
                //            cout << " about to do JetSmear Up " << endl;
                vecVecGoodSmearJets_SystVarUp[systJetSmear].resize(0);
                Jet_ScaleUpSmearing_SFHist(&SmearJets_Scaled, &vecVecGoodSmearJets_SystVarUp[systJetSmear], h_JetSmearSFs, 1);
                JetKinematicsCutSmearedJet(&SmearEJI_SystVarUp[systJetSmear], &vecVecGoodSmearJets_SystVarUp[systJetSmear], BTagSFUtilToUse);
                //            cout << " about to do JetSmear Down " << endl;
                vecVecGoodSmearJets_SystVarDown[systJetSmear].resize(0);
                Jet_ScaleUpSmearing_SFHist(&SmearJets_Scaled, &vecVecGoodSmearJets_SystVarDown[systJetSmear], h_JetSmearSFs, -1);
                JetKinematicsCutSmearedJet(&SmearEJI_SystVarDown[systJetSmear], &vecVecGoodSmearJets_SystVarDown[systJetSmear], BTagSFUtilToUse);
            }
            
            if (PMRP.PSIV.makeGenJetCheck) {
                vecDeltaEnUnsmearJetGenMatchJet->resize(0);
                vecGoodUnsmearJetEn->resize(0);
                JetGenMatchUnSmear(&Jets, vecGoodGenJets, vecDeltaEnUnsmearJetGenMatchJet, vecGoodUnsmearJetEn);
            }
            
            ///            cout << "done messing with jets " << endl;
            /*
             cout << "SmearEJI Jet 0 Pt " << SmearEJI.vecEventJets[0].BVC.Vec_Pt << endl;
             cout << "SmearEJI NJets " << SmearEJI.EventNJets << endl;
             cout << "SmearEJI_SystVarUp[systJetSmear] Jet 0 Pt " << SmearEJI_SystVarUp[systJetSmear].vecEventJets[0].BVC.Vec_Pt << endl;
             cout << "SmearEJI_SystVarUp[systJetSmear] NJets " << SmearEJI_SystVarUp[systJetSmear].EventNJets << endl;
             cout << "SmearEJI_SystVarDown[systJetSmear] Jet 0 Pt " << SmearEJI_SystVarDown[systJetSmear].vecEventJets[0].BVC.Vec_Pt << endl;
             cout << "SmearEJI_SystVarDown[systJetSmear] NJets " << SmearEJI_SystVarDown[systJetSmear].EventNJets << endl;
             */                   
            SmearHackEMI_PF.SetVars(&EMI_PF);
            SmearHackEMI_PF_Old.SetVars(&EMI_PF_Old);
            SmearEMI_PF.SetVars(&SmearEMI_PF_noPhiCorr);
            SmearEMI_PF.PhiCorrection(PMRP.SMV.doData, EPI.nVtx, true, PMRP.PSIV.useOldNTuple);
            
            MET_CalcAddMET(&EMI_PF, &SmearEMI_PF, AddSmearMETX, AddSmearMETY, SmearJetSF);
            
            
            MET_ScaleUpSmearing_SF(&EMI_PF, &SmearEMI_PF, SmearJetSF);
            
            MET_AddMET(&SmearEMI_PF_SystVarUp[systUncES], AddSmearMETX, AddSmearMETY);
            MET_AddMET(&SmearEMI_PF_SystVarDown[systUncES], AddSmearMETX, AddSmearMETY);
            
            SmearEMI_PF_SystVarUp[systLepES].SetVars(&SmearEMI_PF);
            SmearEMI_PF_SystVarDown[systLepES].SetVars(&SmearEMI_PF);
            
            
            
            for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {          
                if (InSystBound(iSyst, &systLB_Jet, &systUB_Jet)) {
                    systJetESShiftUp =  iSyst == systJetES;
                    systJetESShiftDown = -1 * systJetESShiftUp;
                    systBTagEffSFShiftUp = iSyst == systBTagEffSF;
                    systBTagEffSFShiftDown = -1 * systBTagEffSFShiftUp;
                    systBMisTagSFShiftUp = iSyst == systBMisTagSF;
                    systBMisTagSFShiftDown = -1 * systBMisTagSFShiftUp;
                    
                    systJetSmearShiftUp = iSyst == systJetSmear;
                    systJetSmearShiftDown = -1 * systJetSmearShiftUp;
                    
                    if (systBMisTagSFShiftUp || systBTagEffSFShiftUp) {
                        JetKinematicsCutBTagSyst(&EJI_SystVarUp[iSyst], &Jets, BTagSFUtilToUse, systBTagEffSFShiftUp, systBMisTagSFShiftUp, PMRP.SRS.isSignal);
                        JetKinematicsCutBTagSyst(&EJI_SystVarDown[iSyst], &Jets, BTagSFUtilToUse, systBTagEffSFShiftDown, systBMisTagSFShiftDown, PMRP.SRS.isSignal);
                        
                        
                        //smeared jet guys
                        JetKinematicsCutSmearedJetBTagSyst(&SmearEJI_SystVarUp[iSyst], &SmearJets_Scaled, BTagSFUtilToUse, systBTagEffSFShiftUp, systBMisTagSFShiftUp, PMRP.SRS.isSignal);
                        JetKinematicsCutSmearedJetBTagSyst(&SmearEJI_SystVarDown[iSyst], &SmearJets_Scaled, BTagSFUtilToUse, systBTagEffSFShiftDown, systBMisTagSFShiftDown, PMRP.SRS.isSignal);
                    }
                    if (systJetESShiftUp) {   
                        vecVecGoodJets_SystVarUp[iSyst].resize(0);
                        JetInfo(&vecVecGoodJets_SystVarUp[iSyst], vecIsoLeptons, PFJEPs, &JCI, systJetESShiftUp, h_JetESUp);
                        sort(vecVecGoodJets_SystVarUp[iSyst].begin(), vecVecGoodJets_SystVarUp[iSyst].end(), greater<PFJet>());
                        JetKinematicsCut(&EJI_SystVarUp[iSyst], &vecVecGoodJets_SystVarUp[iSyst], BTagSFUtilToUse, PMRP.SMV.doData);
                        EMI_PF_SystVarUp[iSyst].SystShiftMET(&Jets, &vecVecGoodJets_SystVarUp[iSyst], &EMI_PF, PMRP.PSIV.levelJetVerbosity);
                        
                        vecVecGoodJets_SystVarDown[iSyst].resize(0);
                        JetInfo(&vecVecGoodJets_SystVarDown[iSyst], vecIsoLeptons, PFJEPs, &JCI, systJetESShiftDown, h_JetESDown);
                        sort(vecVecGoodJets_SystVarDown[iSyst].begin(), vecVecGoodJets_SystVarDown[iSyst].end(), greater<PFJet>());
                        JetKinematicsCut(&EJI_SystVarDown[iSyst], &vecVecGoodJets_SystVarDown[iSyst], BTagSFUtilToUse, PMRP.SMV.doData);
                        EMI_PF_SystVarDown[iSyst].SystShiftMET(&Jets, &vecVecGoodJets_SystVarDown[iSyst], &EMI_PF, PMRP.PSIV.levelJetVerbosity);                            
                        
                        
                        // smeared jets
                        JetKinematicsCutSmearedJetESSyst(&SmearEJI_SystVarUp[iSyst], &SmearJets, BTagSFUtilToUse, systJetESShiftUp, h_JetESUp);
                        JetKinematicsCutSmearedJetESSyst(&SmearEJI_SystVarDown[iSyst], &SmearJets, BTagSFUtilToUse, systJetESShiftDown, h_JetESDown);
                    }
                }
                if (InSystBound(iSyst, &systLB_SmearMET, &systUB_SmearMET)) {
                    SmearHackEMI_PF_SystVarUp[iSyst].SetVars(&EMI_PF);
                    SmearHackEMI_PF_SystVarDown[iSyst].SetVars(&EMI_PF);
                }
                
                if (InSystBound(iSyst, &systLB_SmearMET, &systUB_SmearMET)) {
                    SmearHackEMI_PF_Old_SystVarUp[iSyst].SetVars(&EMI_PF_Old);
                    SmearHackEMI_PF_Old_SystVarDown[iSyst].SetVars(&EMI_PF_Old);
                }
            }
            if (PMRP.PSIV.useOldNTuple) {                                        
                SmearEJI = EJI;
                for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {          
                    if (InSystBound(iSyst, &systLB_Jet, &systUB_Jet)) {
                        SmearEJI_SystVarUp[iSyst] = EJI_SystVarUp[iSyst];
                        SmearEJI_SystVarDown[iSyst] = EJI_SystVarDown[iSyst];
                    }
                    else {
                        SmearEJI_SystVarUp[iSyst] = EJI;
                        SmearEJI_SystVarDown[iSyst] = EJI;
                    }
                }
            }
            if (doSmearHack) {
                //Perform MET Phi corrections prior to doing by-hand smearing
                METComponents(EMI_PF_noPhiCorr.EventMET, EMI_PF_noPhiCorr.EventMETPhi, METX_Corr, METY_Corr, 1);
                MetPhiCorrect(false, METX_Corr, METY_Corr, EPI.nVtx, EMI_PF_noPhiCorr.METType);
                METComponents(MET_Corr, MET_Phi_Corr, METX_Corr, METY_Corr, 0);
                
                METComponents(EMI_PF_noPhiCorr_Old.EventMET, EMI_PF_noPhiCorr_Old.EventMETPhi, METX_Corr_Old, METY_Corr_Old, 1);
                MetPhiCorrect_noType0(false, METX_Corr_Old, METY_Corr_Old, EPI.nVtx);
                METComponents(MET_Corr_Old, MET_Phi_Corr_Old, METX_Corr_Old, METY_Corr_Old, 0);
                
                
                
                /*
                 METComponents(SmearMET, SmearMET_Phi, SmearMETX, SmearMETY, 1);
                 METComponents(SmearMET_JetSmearUp, SmearMET_Phi_JetSmearUp, SmearMETX_JetSmearUp, SmearMETY_JetSmearUp, 1);
                 METComponents(SmearMET_JetSmearDown, SmearMET_Phi_JetSmearDown, SmearMETX_JetSmearDown, SmearMETY_JetSmearDown, 1);
                 METComponents(SmearMET_UncESUp, SmearMET_Phi_UncESUp, SmearMETX_UncESUp, SmearMETY_UncESUp, 1);
                 METComponents(SmearMET_UncESDown, SmearMET_Phi_UncESDown, SmearMETX_UncESDown, SmearMETY_UncESDown, 1);            
                 
                 SmearMETX              += DeltaMETXY(vecOneDeeMETXSmearHist,          METXSmearHist2D,          MET_Corr);
                 SmearMETX_JetSmearUp   += DeltaMETXY(vecOneDeeMETXSmearJERUpHist,     METXSmearJERUpHist2D,     MET_Corr);
                 SmearMETX_JetSmearDown += DeltaMETXY(vecOneDeeMETXSmearJERDownHist,   METXSmearJERDownHist2D,   MET_Corr);                        
                 SmearMETX_UncESUp      += DeltaMETXY(vecOneDeeMETXSmearUncESUpHist,   METXSmearUncESUpHist2D,   MET_Corr);
                 SmearMETX_UncESDown    += DeltaMETXY(vecOneDeeMETXSmearUncESDownHist, METXSmearUncESDownHist2D, MET_Corr);
                 
                 SmearMETY              += DeltaMETXY(vecOneDeeMETYSmearHist,          METYSmearHist2D,          MET_Corr);
                 SmearMETY_JetSmearUp   += DeltaMETXY(vecOneDeeMETYSmearJERUpHist,     METYSmearJERUpHist2D,     MET_Corr);
                 SmearMETY_JetSmearDown += DeltaMETXY(vecOneDeeMETYSmearJERDownHist,   METYSmearJERDownHist2D,   MET_Corr);                        
                 SmearMETY_UncESUp      += DeltaMETXY(vecOneDeeMETYSmearUncESUpHist,   METYSmearUncESUpHist2D,   MET_Corr);
                 SmearMETY_UncESDown    += DeltaMETXY(vecOneDeeMETYSmearUncESDownHist, METYSmearUncESDownHist2D, MET_Corr);                        
                 
                 METComponents(SmearMET, SmearMET_Phi, SmearMETX, SmearMETY, 0);
                 METComponents(SmearMET_JetSmearUp, SmearMET_Phi_JetSmearUp, SmearMETX_JetSmearUp, SmearMETY_JetSmearUp, 0);
                 METComponents(SmearMET_JetSmearDown, SmearMET_Phi_JetSmearDown, SmearMETX_JetSmearDown, SmearMETY_JetSmearDown, 0);
                 METComponents(SmearMET_UncESUp, SmearMET_Phi_UncESUp, SmearMETX_UncESUp, SmearMETY_UncESUp, 0);
                 METComponents(SmearMET_UncESDown, SmearMET_Phi_UncESDown, SmearMETX_UncESDown, SmearMETY_UncESDown, 0);
                 */
                
                
                // new change from original by-hand smearing method -- condition on which vector of smearing templates to used
                // based on the number of jets in the event
                if (EJI.EventNJets > 1) {
                    METSmearHist2D_ToUse = METSmearHist2D_HighJet;
                    METPhiSmearHist2D_ToUse = METPhiSmearHist2D_HighJet;
                    
                    vecOneDeeMETSmearHist_ToUse = vecOneDeeMETSmearHist_HighJet;
                    vecOneDeeMETPhiSmearHist_ToUse = vecOneDeeMETPhiSmearHist_HighJet;                        
                }
                else {                        
                    METSmearHist2D_ToUse = METSmearHist2D_LowJet;
                    METPhiSmearHist2D_ToUse = METPhiSmearHist2D_LowJet;
                    
                    vecOneDeeMETSmearHist_ToUse = vecOneDeeMETSmearHist_LowJet;
                    vecOneDeeMETPhiSmearHist_ToUse = vecOneDeeMETPhiSmearHist_LowJet; 
                }
                // do the by-hand smearing
                SmearHackEMI_PF.EventMET += DeltaMET(vecOneDeeMETSmearHist_ToUse,        METSmearHist2D_ToUse,    MET_Corr, 0, 1.3);
                SmearHackEMI_PF.EventMETPhi  += DeltaMET(vecOneDeeMETPhiSmearHist_ToUse, METPhiSmearHist2D_ToUse, MET_Corr, 1, 1.3);  
                CorrectMETPhi(SmearHackEMI_PF.EventMETPhi);
                
                SmearHackEMI_PF_Old.EventMET += DeltaMET(vecOneDeeMETSmearHist_ToUse,        METSmearHist2D_ToUse,    MET_Corr_Old, 0, 1.3);
                SmearHackEMI_PF_Old.EventMETPhi  += DeltaMET(vecOneDeeMETPhiSmearHist_ToUse, METPhiSmearHist2D_ToUse, MET_Corr_Old, 1, 1.3); 
                CorrectMETPhi(SmearHackEMI_PF_Old.EventMETPhi);
                for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {
                    if (InSystBound(iSyst, &systLB_SmearMET, &systUB_SmearMET)) {
                        systUncESShiftUp = iSyst == systUncES;
                        systUncESShiftDown = -1 * systUncESShiftUp;
                        systJetSmearShiftUp = iSyst == systJetSmear;
                        systJetSmearShiftDown = -1 * systJetSmearShiftUp;
                        
                        if (systJetSmearShiftUp || systUncESShiftUp) {
                            // same thing as above -- condition on number of jets, but choose instead from the appropriate 
                            // systematic varied smearing template
                            if (systJetSmearShiftUp) {
                                if (EJI.EventNJets > 1) {
                                    METSmearHist2D_ToUse_SystVarUp = METSmearJERUpHist2D_HighJet;
                                    METPhiSmearHist2D_ToUse_SystVarUp = METPhiSmearJERUpHist2D_HighJet;
                                    
                                    vecOneDeeMETSmearHist_ToUse_SystVarUp = vecOneDeeMETSmearJERUpHist_HighJet;
                                    vecOneDeeMETPhiSmearHist_ToUse_SystVarUp = vecOneDeeMETPhiSmearJERUpHist_HighJet;
                                    
                                    METSmearHist2D_ToUse_SystVarDown = METSmearJERDownHist2D_HighJet;
                                    METPhiSmearHist2D_ToUse_SystVarDown = METPhiSmearJERDownHist2D_HighJet;
                                    
                                    vecOneDeeMETSmearHist_ToUse_SystVarDown = vecOneDeeMETSmearJERDownHist_HighJet;
                                    vecOneDeeMETPhiSmearHist_ToUse_SystVarDown = vecOneDeeMETPhiSmearJERDownHist_HighJet;                                         
                                }
                                else {
                                    METSmearHist2D_ToUse_SystVarUp = METSmearJERUpHist2D_LowJet;
                                    METPhiSmearHist2D_ToUse_SystVarUp = METPhiSmearJERUpHist2D_LowJet;
                                    
                                    vecOneDeeMETSmearHist_ToUse_SystVarUp = vecOneDeeMETSmearJERUpHist_LowJet;
                                    vecOneDeeMETPhiSmearHist_ToUse_SystVarUp = vecOneDeeMETPhiSmearJERUpHist_LowJet;
                                    
                                    METSmearHist2D_ToUse_SystVarDown = METSmearJERDownHist2D_LowJet;
                                    METPhiSmearHist2D_ToUse_SystVarDown = METPhiSmearJERDownHist2D_LowJet;
                                    
                                    vecOneDeeMETSmearHist_ToUse_SystVarDown = vecOneDeeMETSmearJERDownHist_LowJet;
                                    vecOneDeeMETPhiSmearHist_ToUse_SystVarDown = vecOneDeeMETPhiSmearJERDownHist_LowJet;
                                }
                            }
                            else if (systUncESShiftUp) {
                                if (EJI.EventNJets > 1) {
                                    METSmearHist2D_ToUse_SystVarUp = METSmearUncESUpHist2D_HighJet;
                                    METPhiSmearHist2D_ToUse_SystVarUp = METPhiSmearUncESUpHist2D_HighJet;
                                    
                                    vecOneDeeMETSmearHist_ToUse_SystVarUp = vecOneDeeMETSmearUncESUpHist_HighJet;
                                    vecOneDeeMETPhiSmearHist_ToUse_SystVarUp = vecOneDeeMETPhiSmearUncESUpHist_HighJet;
                                    
                                    METSmearHist2D_ToUse_SystVarDown = METSmearUncESDownHist2D_HighJet;
                                    METPhiSmearHist2D_ToUse_SystVarDown = METPhiSmearUncESDownHist2D_HighJet;
                                    
                                    vecOneDeeMETSmearHist_ToUse_SystVarDown = vecOneDeeMETSmearUncESDownHist_HighJet;
                                    vecOneDeeMETPhiSmearHist_ToUse_SystVarDown = vecOneDeeMETPhiSmearUncESDownHist_HighJet;                                         
                                }
                                else {
                                    METSmearHist2D_ToUse_SystVarUp = METSmearUncESUpHist2D_LowJet;
                                    METPhiSmearHist2D_ToUse_SystVarUp = METPhiSmearUncESUpHist2D_LowJet;
                                    
                                    vecOneDeeMETSmearHist_ToUse_SystVarUp = vecOneDeeMETSmearUncESUpHist_LowJet;
                                    vecOneDeeMETPhiSmearHist_ToUse_SystVarUp = vecOneDeeMETPhiSmearUncESUpHist_LowJet;
                                    
                                    METSmearHist2D_ToUse_SystVarDown = METSmearUncESDownHist2D_LowJet;
                                    METPhiSmearHist2D_ToUse_SystVarDown = METPhiSmearUncESDownHist2D_LowJet;
                                    
                                    vecOneDeeMETSmearHist_ToUse_SystVarDown = vecOneDeeMETSmearUncESDownHist_LowJet;
                                    vecOneDeeMETPhiSmearHist_ToUse_SystVarDown = vecOneDeeMETPhiSmearUncESDownHist_LowJet;
                                }
                            }     
                            // do by-hand smearing with said systematic varied template
                            SmearHackEMI_PF_SystVarUp[iSyst].EventMET   += DeltaMET(vecOneDeeMETSmearHist_ToUse_SystVarUp,     METSmearHist2D_ToUse_SystVarUp,   MET_Corr, 0, 1.3);
                            SmearHackEMI_PF_SystVarUp[iSyst].EventMETPhi  += DeltaMET(vecOneDeeMETPhiSmearHist_ToUse_SystVarUp,     METPhiSmearHist2D_ToUse_SystVarUp,   MET_Corr, 1, 1.3);
                            CorrectMETPhi(SmearHackEMI_PF_SystVarUp[iSyst].EventMETPhi);
                            
                            SmearHackEMI_PF_SystVarDown[iSyst].EventMET   += DeltaMET(vecOneDeeMETSmearHist_ToUse_SystVarDown,     METSmearHist2D_ToUse_SystVarDown,   MET_Corr, 0, 1.3);
                            SmearHackEMI_PF_SystVarDown[iSyst].EventMETPhi  += DeltaMET(vecOneDeeMETPhiSmearHist_ToUse_SystVarDown,     METPhiSmearHist2D_ToUse_SystVarDown,   MET_Corr, 1, 1.3); 
                            CorrectMETPhi(SmearHackEMI_PF_SystVarDown[iSyst].EventMETPhi);
                        }
                        else {
                            // no unique template for given systematic variation -- instead set the variables as is
                            // and then calculate the sytematic variations in the standard vector sum fashion
                            SmearHackEMI_PF_SystVarUp[iSyst].SetVars(&SmearHackEMI_PF);
                            SmearHackEMI_PF_SystVarDown[iSyst].SetVars(&SmearHackEMI_PF);      
                            
                            SmearHackEMI_PF_Old_SystVarUp[iSyst].SetVars(&SmearHackEMI_PF_Old);
                            SmearHackEMI_PF_Old_SystVarDown[iSyst].SetVars(&SmearHackEMI_PF_Old); 
                        }
                    }                        
                }
                
                if ( SmearHackEMI_PF.EventMET < 0)  SmearHackEMI_PF.EventMET = 0.;
                if ( SmearHackEMI_PF_Old.EventMET < 0)  SmearHackEMI_PF_Old.EventMET = 0.;
            }                    
            //                SmearEJI = EJI;
            for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {
                if (InSystBound(iSyst, &systLB_SmearJet, &systUB_SmearJet)) {
                    systJetSmearShiftUp =  iSyst == systJetSmear;
                    systJetSmearShiftDown = -1 * systJetSmearShiftUp;
                    /*
                     if (!systJetSmearShiftUp) {
                     SmearEJI_SystVarUp[iSyst] = EJI_SystVarUp[iSyst];
                     SmearEJI_SystVarDown[iSyst] = EJI_SystVarDown[iSyst];
                     }
                     else {
                     SmearEJI_SystVarUp[iSyst] = EJI;
                     SmearEJI_SystVarDown[iSyst] = EJI;
                     }
                     */
                }
                
                if (InSystBound(iSyst, &systLB_SmearMET, &systUB_SmearMET)) {
                    systLepESShiftUp =  iSyst ==systLepES;
                    systLepESShiftDown = -1 * systLepESShiftUp;
                    systJetESShiftUp =  iSyst == systJetES;
                    systJetESShiftDown = -1 * systJetESShiftUp;
                    if (systJetESShiftUp) {                                      
                        SmearHackEMI_PF_SystVarUp[iSyst].SystShiftMET(&Jets, &vecVecGoodJets_SystVarUp[iSyst], &SmearHackEMI_PF, PMRP.PSIV.levelJetVerbosity);
                        SmearHackEMI_PF_SystVarDown[iSyst].SystShiftMET(&Jets, &vecVecGoodJets_SystVarDown[iSyst], &SmearHackEMI_PF, PMRP.PSIV.levelJetVerbosity);
                        
                        SmearHackEMI_PF_Old_SystVarUp[iSyst].SystShiftMET(&Jets, &vecVecGoodJets_SystVarUp[iSyst], &SmearHackEMI_PF_Old, PMRP.PSIV.levelJetVerbosity);
                        SmearHackEMI_PF_Old_SystVarDown[iSyst].SystShiftMET(&Jets, &vecVecGoodJets_SystVarDown[iSyst], &SmearHackEMI_PF_Old, PMRP.PSIV.levelJetVerbosity);
                    }
                    if (systLepESShiftUp) {
                        
                        SmearEMI_PF_SystVarUp[iSyst].SystShiftMET(vecVecIsoLeptonsCentValMETPatsy_SystVarUp[iSyst], vecVecIsoLeptons_SystVarUp[iSyst], &SmearEMI_PF);
                        SmearEMI_PF_SystVarDown[iSyst].SystShiftMET(vecVecIsoLeptonsCentValMETPatsy_SystVarDown[iSyst],vecVecIsoLeptons_SystVarDown[iSyst], &SmearEMI_PF);
                        
                        SmearHackEMI_PF_SystVarUp[iSyst].SystShiftMET(vecVecIsoLeptonsCentValMETPatsy_SystVarUp[iSyst], vecVecIsoLeptons_SystVarUp[iSyst], &SmearHackEMI_PF);
                        SmearHackEMI_PF_SystVarDown[iSyst].SystShiftMET(vecVecIsoLeptonsCentValMETPatsy_SystVarDown[iSyst], vecVecIsoLeptons_SystVarDown[iSyst], &SmearHackEMI_PF);
                        
                        SmearHackEMI_PF_Old_SystVarUp[iSyst].SystShiftMET(vecVecIsoLeptonsCentValMETPatsy_SystVarUp[iSyst], vecVecIsoLeptons_SystVarUp[iSyst], &SmearHackEMI_PF_Old);
                        SmearHackEMI_PF_Old_SystVarDown[iSyst].SystShiftMET(vecVecIsoLeptonsCentValMETPatsy_SystVarDown[iSyst], vecVecIsoLeptons_SystVarDown[iSyst], &SmearHackEMI_PF_Old);
                    }
                }
            }
        }
        EPI.nVtxTrue = EPI.nVtx;
        if (PMRP.SMV.doData) {
            EPI.DefaultWeights();
        }
        else {
            EPI.Set1DWeightTrue(TrueWeightHistToUse1D);
            EPI.Set1DWeight(TrueWeightHistToUse1D);
            EPI.Set3DWeight(WeightHistToUse3D);   
        }
        if (ELI.EventDiLepType == -2) ELI.EventDiLepType = 2;
        if (ELI_SystVarUp[systLepES].EventDiLepType == -2) ELI_SystVarUp[systLepES].EventDiLepType = 2;
        if (ELI_SystVarDown[systLepES].EventDiLepType == -2) ELI_SystVarDown[systLepES].EventDiLepType = 2;        
        
        if (PMRP.PSIV.printEventNum && !PMRP.SRS.isSignal) {
            if (ELI.EventDiLepType == 0 && FTI.passTrigDoubleMu) {
                cout << "MuMu -- Event Passed: RunNum:EventNum:LumiBlock " << BEI.runNumber  << ":" << BEI.eventNumber << ":" << BEI.lumiBlock << endl;                
            }
            else if (ELI.EventDiLepType == 1 && FTI.passTrigDoubleEl) {
                cout << "EE -- Event Passed: RunNum:EventNum:LumiBlock " << BEI.runNumber  << ":" << BEI.eventNumber << ":" << BEI.lumiBlock << endl;                
            }
            else if (ELI.EventDiLepType == 2 && FTI.passTrigElMu) {
                cout << "EMu -- Event Passed: RunNum:EventNum:LumiBlock " << BEI.runNumber  << ":" << BEI.eventNumber << ":" << BEI.lumiBlock << endl;                
            }
        }        
        
        if (PMRP.SMV.doData) {
            if (ELI.EventDiLepType == 0) {
                if (!(PMRP.PSIV.fInName.Contains("DoubleMu") || PMRP.PSIV.fInName.Contains("mumu_run2012"))) continue;
            }
            else if (ELI.EventDiLepType == 1) {
                if (!(PMRP.PSIV.fInName.Contains("DoubleEl") || PMRP.PSIV.fInName.Contains("ee_run2012"))) continue;
            }
            else if (ELI.EventDiLepType == 2) {
                if (!(PMRP.PSIV.fInName.Contains("MuEG") || PMRP.PSIV.fInName.Contains("emu_run2012"))) continue;
            }
        }
        if (doEvent) {
            h_CutFlow->Fill(3.);    
        }
        if (!PMRP.SMV.doData) {
            if (doEvent_LepESUp) {
                h_CutFlow_LepESUp->Fill(3.);
            }
            if (doEvent_LepESDown) {
                h_CutFlow_LepESDown->Fill(3.);
            }
        }
        ////Grab Generator Info
        // grab generator MET info if there       
        if (PMRP.RRL.doSpecRun) {
            if (BEI.runNumber == (unsigned int) PMRP.RRL.whichRun) {
                cout << "Post LepCut Run:Event:Lumi" << BEI.runNumber << ":" << BEI.eventNumber << ":" << BEI.lumiBlock << endl;
            }
            else {
                continue;
            }
        }
        
        if (!PMRP.SMV.doData) {
            for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {
                if (InSystBound(iSyst, &systLB_SmearMET, &systUB_SmearMET)) {
                    SmearEMI_PF_SystVarUp[iSyst].PhiCorrection(PMRP.SMV.doData, EPI.nVtx, true, PMRP.PSIV.useOldNTuple);
                    SmearEMI_PF_SystVarDown[iSyst].PhiCorrection(PMRP.SMV.doData, EPI.nVtx, true, PMRP.PSIV.useOldNTuple);
                    if ((iSyst != systLepES && iSyst != systUncES)) {
                        MET_ScaleUpSmearing_SF(&EMI_PF, &SmearEMI_PF_SystVarUp[iSyst], SmearJetSF);
                        MET_ScaleUpSmearing_SF(&EMI_PF, &SmearEMI_PF_SystVarDown[iSyst], SmearJetSF);
                    }
                }
            }
        }
        
        //try calculating MT2 and MT
        EMI_PF_noPhiCorr_Old.CalcMETVariations(&ELI, &EJI);
        EMI_PF_noPhiCorr.CalcMETVariations(&ELI, &EJI);
        EMI_PF.CalcMETVariations(&ELI, &EJI);
        EMI_PF_Old.CalcMETVariations(&ELI, &EJI);
        if (!PMRP.SMV.doData) {
            EMI_PF_SystVarUp[systLepES].CalcMETVariations(&ELI_SystVarUp[systLepES], &EJI);
            EMI_PF_SystVarDown[systLepES].CalcMETVariations(&ELI_SystVarDown[systLepES], &EJI); 
            
            for (int iJetSyst = 2; iJetSyst < 5; ++iJetSyst) {
                EMI_PF_SystVarUp[iJetSyst].CalcMETVariations(&ELI, &EJI_SystVarUp[iJetSyst]);
                EMI_PF_SystVarDown[iJetSyst].CalcMETVariations(&ELI, &EJI_SystVarDown[iJetSyst]);
            }
            
            
            SmearEMI_PF.CalcMETVariations(&ELI, &SmearEJI);
            
            SmearEMI_PF_SystVarUp[systLepES].CalcMETVariations(&ELI_SystVarUp[systLepES], &SmearEJI);
            SmearEMI_PF_SystVarDown[systLepES].CalcMETVariations(&ELI_SystVarDown[systLepES], &SmearEJI);
            for (int iJetSyst = 2; iJetSyst < 6; ++iJetSyst) {
                SmearEMI_PF_SystVarUp[iJetSyst].CalcMETVariations(&ELI, &SmearEJI_SystVarUp[iJetSyst]);
                SmearEMI_PF_SystVarDown[iJetSyst].CalcMETVariations(&ELI, &SmearEJI_SystVarDown[iJetSyst]);
            }
            SmearEMI_PF_SystVarUp[systUncES].CalcMETVariations(&ELI, &SmearEJI);
            SmearEMI_PF_SystVarDown[systUncES].CalcMETVariations(&ELI, &SmearEJI);
            
            SmearHackEMI_PF.CalcMETVariations(&ELI, &EJI);
            SmearHackEMI_PF_Old.CalcMETVariations(&ELI, &EJI);
            
            SmearHackEMI_PF_SystVarUp[systLepES].CalcMETVariations(&ELI_SystVarUp[systLepES], &EJI);
            SmearHackEMI_PF_SystVarDown[systLepES].CalcMETVariations(&ELI_SystVarDown[systLepES], &EJI);
            
            SmearHackEMI_PF_Old_SystVarUp[systLepES].CalcMETVariations(&ELI_SystVarUp[systLepES], &EJI);
            SmearHackEMI_PF_Old_SystVarDown[systLepES].CalcMETVariations(&ELI_SystVarDown[systLepES], &EJI);
            
            for (int iJetSyst = 2; iJetSyst < 6; ++iJetSyst) {
                SmearHackEMI_PF_SystVarUp[iJetSyst].CalcMETVariations(&ELI, &EJI_SystVarUp[iJetSyst]);
                SmearHackEMI_PF_SystVarDown[iJetSyst].CalcMETVariations(&ELI, &EJI_SystVarDown[iJetSyst]);
                
                SmearHackEMI_PF_Old_SystVarUp[iJetSyst].CalcMETVariations(&ELI, &EJI_SystVarUp[iJetSyst]);
                SmearHackEMI_PF_Old_SystVarDown[iJetSyst].CalcMETVariations(&ELI, &EJI_SystVarDown[iJetSyst]);
            }
            SmearHackEMI_PF_SystVarUp[systUncES].CalcMETVariations(&ELI, &EJI);
            SmearHackEMI_PF_SystVarDown[systUncES].CalcMETVariations(&ELI, &EJI);
            
            SmearHackEMI_PF_Old_SystVarUp[systUncES].CalcMETVariations(&ELI, &EJI);
            SmearHackEMI_PF_Old_SystVarDown[systUncES].CalcMETVariations(&ELI, &EJI);
        }
        //Distructure info
        EDSI.SetVars(&ELI, &EJI, &EMI_PF);
        //        EDSI_noPhiCorr.SetVars(&ELI, &EJI, &EMI_PF_noPhiCorr);
        
        if (!PMRP.SMV.doData) {
            EDSI_SystVarUp[systLepES].SetVars(&ELI_SystVarUp[systLepES], &EJI, &EMI_PF_SystVarUp[systLepES]);
            EDSI_SystVarDown[systLepES].SetVars(&ELI_SystVarDown[systLepES], &EJI, &EMI_PF_SystVarDown[systLepES]);
            
            EDSI_SystVarUp[systJetES].SetVars(&ELI, &EJI_SystVarUp[systJetES], &EMI_PF_SystVarUp[systJetES]);
            EDSI_SystVarDown[systJetES].SetVars(&ELI, &EJI_SystVarDown[systJetES], &EMI_PF_SystVarDown[systJetES]);
            
            EDSI_SystVarUp[systBTagEffSF].SetVars(&ELI, &EJI_SystVarUp[systBTagEffSF], &EMI_PF_SystVarUp[systBTagEffSF]);
            EDSI_SystVarDown[systBTagEffSF].SetVars(&ELI, &EJI_SystVarDown[systBTagEffSF], &EMI_PF_SystVarDown[systBTagEffSF]);
            
            EDSI_SystVarUp[systBMisTagSF].SetVars(&ELI, &EJI_SystVarUp[systBMisTagSF], &EMI_PF_SystVarUp[systBMisTagSF]);
            EDSI_SystVarDown[systBMisTagSF].SetVars(&ELI, &EJI_SystVarDown[systBMisTagSF], &EMI_PF_SystVarDown[systBMisTagSF]);
            
            
            SmearEDSI.SetVars(&ELI, &SmearEJI, &SmearEMI_PF);
            
            SmearEDSI_SystVarUp[systLepES].SetVars(&ELI_SystVarUp[systLepES], &SmearEJI, &SmearEMI_PF_SystVarUp[systLepES]);
            SmearEDSI_SystVarDown[systLepES].SetVars(&ELI_SystVarDown[systLepES], &SmearEJI, &SmearEMI_PF_SystVarDown[systLepES]);
            
            SmearEDSI_SystVarUp[systJetES].SetVars(&ELI, &SmearEJI_SystVarUp[systJetES], &SmearEMI_PF_SystVarUp[systJetES]);
            SmearEDSI_SystVarDown[systJetES].SetVars(&ELI, &SmearEJI_SystVarDown[systJetES], &SmearEMI_PF_SystVarDown[systJetES]);
            
            SmearEDSI_SystVarUp[systBTagEffSF].SetVars(&ELI, &SmearEJI_SystVarUp[systBTagEffSF], &SmearEMI_PF_SystVarUp[systBTagEffSF]);
            SmearEDSI_SystVarDown[systBTagEffSF].SetVars(&ELI, &SmearEJI_SystVarDown[systBTagEffSF], &SmearEMI_PF_SystVarDown[systBTagEffSF]);
            
            SmearEDSI_SystVarUp[systBMisTagSF].SetVars(&ELI, &SmearEJI_SystVarUp[systBMisTagSF], &SmearEMI_PF_SystVarUp[systBMisTagSF]);
            SmearEDSI_SystVarDown[systBMisTagSF].SetVars(&ELI, &SmearEJI_SystVarDown[systBMisTagSF], &SmearEMI_PF_SystVarDown[systBMisTagSF]);
            
            SmearEDSI_SystVarUp[systJetSmear].SetVars(&ELI, &SmearEJI_SystVarUp[systJetSmear], &SmearEMI_PF_SystVarUp[systJetSmear]);
            SmearEDSI_SystVarDown[systJetSmear].SetVars(&ELI, &SmearEJI_SystVarUp[systJetSmear], &SmearEMI_PF_SystVarDown[systJetSmear]);
            
            SmearEDSI_SystVarUp[systUncES].SetVars(&ELI, &SmearEJI, &SmearEMI_PF_SystVarUp[systUncES]);
            SmearEDSI_SystVarDown[systUncES].SetVars(&ELI, &SmearEJI, &SmearEMI_PF_SystVarDown[systUncES]);
            
        }
        
        /*
         not done yet
         passesFullSelection = PassesFullSelection(&EMI_PF, &EJI, &ELI);
         if (!PMRP.SMV.doData) {
         vecPFS_SystVarUp[systLepES] = PassesFullSelection(&SmearEMI_PF_SystVarUp[systLepES], &EJI, &ELI_SystVarUp[systLepES]);
         vecPFS_SystVarDown[systLepES] = PassesFullSelection(&SmearEMI_PF_SystVarDown[systLepES], &EJI, &ELI_SystVarDown[systLepES]);
         vecPFS_SystVarUp[systLepES] = PassesFullSelection(&SmearEMI_PF_SystVarUp[systJetES], &EJI_, &ELI_SystVarUp[systLepES]);
         vecPFS_SystVarDown[systLepES] = PassesFullSelection(&SmearEMI_PF_SystVarDown[systJetES], &EJI, &ELI_SystVarDown[systLepES]);
         }
         
         */
        
        if (!PMRP.SMV.doData) {
            EGW.SetGenWeight(genWeightType, &EGSPI, &EGQPI, &EGEWKPI);
            if (genWeightType == 2) {
                EGW.SetWeightGenISR(&ELI.vecEventLeps[0].GP, &ELI.vecEventLeps[1].GP);
            }
        }
        
        outTree->Fill();
        
        delete vecIsoLeptons;
        //        delete Jets;
        if (!PMRP.SMV.doData) {
            for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {
                if (InSystBound(iSyst, &systLB_Lepton, &systUB_Lepton)) {            
                    delete vecVecIsoLeptonsCentValMETPatsy_SystVarUp[iSyst];
                    delete vecVecIsoLeptonsCentValMETPatsy_SystVarDown[iSyst];
                    delete vecVecIsoLeptons_SystVarUp[iSyst];
                    delete vecVecIsoLeptons_SystVarDown[iSyst];
                }   
                /*
                 if (InSystBound(iSyst, &systLB_Jet, &systUB_Jet)) {
                 delete vecVecGoodJets_SystVarUp[iSyst];
                 delete vecVecGoodJets_SystVarDown[iSyst];
                 }
                 if (InSystBound(iSyst, &systLB_SmearJet, &systUB_SmearJet)) {
                 delete vecVecGoodSmearJets_SystVarUp[iSyst];
                 delete vecVecGoodSmearJets_SystVarDown[iSyst];
                 }
                 */
            }
        }
    }
    cout << "All events done" << endl;
    outputFile->cd();
    if (PMRP.SMV.typeJetSmear != 1) {
        h_DeltaEnergySmearMinUnSmear->Write();
        h_DeltaEnergySmearMinUnSmear_JESUp->Write();
        h_DeltaEnergySmearMinUnSmear_JESDown->Write();
        h_DeltaEnergySmearMinUnSmear_JERUp->Write();
        h_DeltaEnergySmearMinUnSmear_JERDown->Write();
    }
    cout << "cd-ing to output directory" << endl;
    outputFile->Write();
    h_eventCount->Write();
    h_CutFlow->Write();
    if (!PMRP.SMV.doData) {
        h_CutFlow_LepESUp->Write();
        h_CutFlow_LepESDown->Write();
    }
    h_DeltaPtSmearUnsmear_GenMatched->Write();
    h_DeltaPtSmearUnsmear_NotGenMatched->Write();
    cout << "Writing of output file done" << endl;
    outputFile->Close();
    cout << "end of code" << endl;
    
    
    
    
    
    
    
    
    
    cout << "going to delete things now " << endl;
    /*
     delete JetESFile;
     delete h_JetESUp;
     delete h_JetESDown;
     delete GenJetSmearFile;
     delete h_GenJetSmearHist;
     delete h_RecoJetLowPtSmearHist;
     
     delete FullSimPUWeights;
     delete FullSimWeightHist;
     
     delete FastSimPUWeights;
     delete FastSimWeightHist;
     
     delete FullSimTruePUWeight;
     delete FullSimTrueWeightHist;
     delete FastSimTruePUWeight;
     delete FastSimTrueWeightHist;
     
     delete WeightHistToUse3D;
     delete TrueWeightHistToUse1D;
     
     
     delete TTBarFile;
     delete DYFile;
     delete WWFile;
     delete WZFile;
     delete ZZFile;
     delete WJFile;
     delete ZGFile;
     delete WGFile;
     */
    /*
     if (!PMRP.SMV.doData) {
     delete METSmearFiles;
     
     delete METSmearHistVec;
     delete METSmearJERUpHistVec;
     delete METSmearJERDownHistVec;
     delete METSmearUncESUpHistVec;
     delete METSmearUncESDownHistVec;
     delete METPhiSmearHistVec;
     delete METPhiSmearJERUpHistVec;
     delete METPhiSmearJERDownHistVec;
     delete METPhiSmearUncESUpHistVec;
     delete METPhiSmearUncESDownHistVec;
     
     METSmearHist2D_HighJet->Delete();
     METSmearHist2D_LowJet->Delete();
     delete METPhiSmearHist2D_HighJet;
     delete METPhiSmearHist2D_LowJet;
     delete METSmearJERUpHist2D_HighJet;
     delete METSmearJERUpHist2D_HighJet;
     delete METSmearJERDownHist2D_HighJet;
     delete METSmearJERDownHist2D_HighJet;
     
     
     delete METPhiSmearJERUpHist2D_HighJet;
     delete METPhiSmearJERUpHist2D_HighJet;
     delete METPhiSmearJERDownHist2D_HighJet;
     delete METPhiSmearJERDownHist2D_HighJet;
     delete METSmearUncESUpHist2D_HighJet;
     delete METSmearUncESUpHist2D_HighJet;
     delete METSmearUncESDownHist2D_HighJet;
     delete METSmearUncESDownHist2D_HighJet;
     delete METPhiSmearUncESUpHist2D_HighJet;
     delete METPhiSmearUncESUpHist2D_HighJet;
     delete METPhiSmearUncESDownHist2D_HighJet;
     delete METPhiSmearUncESDownHist2D_HighJet;
     delete vecOneDeeMETSmearHist_HighJet;
     delete vecOneDeeMETSmearHist_HighJet;
     delete vecOneDeeMETPhiSmearHist_HighJet;
     delete vecOneDeeMETPhiSmearHist_HighJet;
     
     delete vecOneDeeMETSmearJERUpHist_HighJet;
     delete vecOneDeeMETSmearJERUpHist_HighJet;
     delete vecOneDeeMETSmearJERDownHist_HighJet;
     delete vecOneDeeMETSmearJERDownHist_HighJet;
     delete vecOneDeeMETSmearUncESUpHist_HighJet;
     delete vecOneDeeMETSmearUncESUpHist_HighJet;
     yet        
     delete vecOneDeeMETSmearUncESDownHist_HighJet;
     
     delete vecOneDeeMETPhiSmearJERUpHist;
     delete vecOneDeeMETPhiSmearJERDownHist;
     delete vecOneDeeMETPhiSmearUncESUpHist;
     delete vecOneDeeMETPhiSmearUncESDownHist;
     
     delete JetResolutionTF1Vec;
     
     delete h_DeltaEnergySmearMinUnSmear;
     delete h_DeltaEnergySmearMinUnSmear_JESUp;
     delete h_DeltaEnergySmearMinUnSmear_JESDown;
     delete h_DeltaEnergySmearMinUnSmear_JERUp;
     delete h_DeltaEnergySmearMinUnSmear_JERDown;
     }
     */
    delete h_eventCount;
    delete h_CutFlow; 
    if (!PMRP.SMV.doData) {
        delete h_CutFlow_LepESUp; delete h_CutFlow_LepESDown;
    }
    
    MEPs.DeleteVecs();
    EEPs.DeleteVecs();
    PFJEPs.DeleteVecs();
    SmearPFJEPs.DeleteVecs();
    GJEPs.DeleteVecs();    
    EGLPI.DeleteVecs();    
    EGSPI.DeleteVecs();
    EGQPI.DeleteVecs();
    
    /*
     delete BTagSFUtilBase;
     delete BTagSFUtilSignal;
     delete BTagSFUtilToUse;
     */
    
    delete VertZ;
    delete VertNDOF;
    delete VertRho;
}