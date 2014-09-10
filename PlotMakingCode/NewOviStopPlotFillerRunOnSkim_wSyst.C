#include "../HeaderFiles/NewOviStopPlotFillerRunOnSkim_wSyst.h"
#include "../HeaderFiles/hackFunction.h"


#include <vector>
#include <cmath>
#include <sstream>
#include <map>
//#ifdef __MAKECINT__
//#pragma link C++ class vector >float< ;
//#endif

using namespace std;
typedef map<TString, TString> labelMap;
typedef map<TString, float> StV_Map;
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

typedef struct {
    float stopProdXsec;
    float stopProdXsecUncert;
} StopXSec;
typedef pair<HistogramT, TString> histKeyString;
typedef map<histKeyString, TH1 *>      HMap_1D;
typedef map<histKeyString, TH2 *>      HMap_2D;
typedef map<histKeyString, TH3 *>      HMap_3D;
typedef map<TString, bool>       passCutMap;

typedef map<int, BasicEventInfo *> mapIntBEI;
typedef map<int, EventMETInfo *> mapIntEMI;
typedef map<int, EventLepInfo *> mapIntELI;
typedef map<int, EventJetInfo *> mapIntEJI;
typedef map<int, EventDiStructureInfo *> mapIntEDSI;
typedef map<int, bool> mIB;

StopXSec getCrossSectionStop(float);


//different nVtx regions for the plots
int main( int argc, char* argv[] ) {
    /////////Variable initializations/////////////////
    /////Organization Variables//////////////

    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    ///////////////////////  Begin: Deal with event variable initializations    
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    
    TString baseDirNonDataRootFile = "../NonDataRootFiles/";
    
    
    /******************************************************************************************************************************/
    ///////////////////////  Begin: Deal with loading in Lepton SFs
    /******************************************************************************************************************************/
    
    TString baseDirLeptonSF = baseDirNonDataRootFile + TString("LeptonScaleFactors/");
    
    TString baseNameTrigSF = "triggerSummary";
    TString appendNameTrigSF_SM = "_ttbar.root";
    TString appendNameTrigSF_Sig = "_stop.root";
    
    TString nameDiLepTrigSF[3] = {"_mumu", "_ee", "_emu"};
    TString nameLeptonTrigSFHist = "scalefactor_eta2d_with_syst";
    
    TFile * fileTriggerBkgMuMu  = new TFile(baseDirLeptonSF + baseNameTrigSF + nameDiLepTrigSF[0] + appendNameTrigSF_SM);
    TFile * fileTriggerBkgEE  = new TFile(baseDirLeptonSF + baseNameTrigSF + nameDiLepTrigSF[1] + appendNameTrigSF_SM);
    TFile * fileTriggerBkgEMu  = new TFile(baseDirLeptonSF + baseNameTrigSF + nameDiLepTrigSF[2] + appendNameTrigSF_SM);
    TH2F * h_TrigSFBkgMuMu      = (TH2F *) fileTriggerBkgMuMu->Get(nameLeptonTrigSFHist);
    TH2F * h_TrigSFBkgEE      = (TH2F *) fileTriggerBkgEE->Get(nameLeptonTrigSFHist);
    TH2F * h_TrigSFBkgEMu      = (TH2F *) fileTriggerBkgEMu->Get(nameLeptonTrigSFHist);

    TFile * fileTriggerSigMuMu  = new TFile(baseDirLeptonSF + baseNameTrigSF + nameDiLepTrigSF[0] + appendNameTrigSF_Sig);
    TFile * fileTriggerSigEE  = new TFile(baseDirLeptonSF + baseNameTrigSF + nameDiLepTrigSF[1] + appendNameTrigSF_Sig);
    TFile * fileTriggerSigEMu  = new TFile(baseDirLeptonSF + baseNameTrigSF + nameDiLepTrigSF[2] + appendNameTrigSF_Sig);
    
    TH2F * h_TrigSFSigMuMu      = (TH2F *) fileTriggerSigMuMu->Get(nameLeptonTrigSFHist);
    TH2F * h_TrigSFSigEE        = (TH2F *) fileTriggerSigEE->Get(nameLeptonTrigSFHist);
    TH2F * h_TrigSFSigEMu       = (TH2F *) fileTriggerSigEMu->Get(nameLeptonTrigSFHist);
    vector<TH2F *> h_TrigSFVec;
    
    
    TString nameElectronIDIsoSF = "ElecGlobalSF_susy";
    TString nameMuonIDIsoSF = "MuonGlobalSF_susy";
    
    TFile * fileFullSimLepIDIsoElec = new TFile(baseDirLeptonSF + nameElectronIDIsoSF + TString(".root"));
    TH2F * h_LepIDIsoSFBkgElec = (TH2F *) fileFullSimLepIDIsoElec->Get(nameElectronIDIsoSF)->Clone("h_LepIDIsoSF_Electron_SM");
    
    TFile * fileFullSimLepIDIsoMuon = new TFile(baseDirLeptonSF + nameMuonIDIsoSF + TString(".root"));
    TH2F * h_LepIDIsoSFBkgMuon = (TH2F *) fileFullSimLepIDIsoMuon->Get(nameMuonIDIsoSF)->Clone("h_LepIDIsoSF_Muon_SM");
        
    TFile * fileFastSimLepIDIsoElec = new TFile(baseDirLeptonSF + nameElectronIDIsoSF + TString("FastSim.root"));
    TH2F * h_LepIDIsoSFSigElec = (TH2F *) fileFastSimLepIDIsoElec->Get(nameElectronIDIsoSF);
    TFile * fileFastSimLepIDIsoMuon = new TFile(baseDirLeptonSF + nameMuonIDIsoSF + TString("FastSim.root"));
    TH2F * h_LepIDIsoSFSigMuon = (TH2F *) fileFastSimLepIDIsoMuon->Get(nameMuonIDIsoSF);
    
    
    TH2F * h_LepIDIsoElecSFToUse, * h_LepIDIsoMuonSFToUse;
    
     // stuff for additional scale factors from Pieter (decommisioned with the new SF measurements from Carmen/Jan (12/6/13)
    TFile * fileFastSimLepIDIsoMuMu = new TFile(baseDirLeptonSF + TString("muon_FastSim_EWKino.root"));
    TFile * fileFastSimLepIDIsoEE = new TFile(baseDirLeptonSF + TString("electron_FastSim_EWKino.root"));
    
    TH2F * h_LepIDIsoSFSigMuMu = (TH2F *) fileFastSimLepIDIsoMuMu->Get("SF");
    TH2F * h_LepIDIsoSFSigEE = (TH2F *) fileFastSimLepIDIsoEE->Get("SF");

    /******************************************************************************************************************************/
    ///////////////////////  End: Deal with loading in Lepton SFs
    /******************************************************************************************************************************/
    
    /******************************************************************************************************************************/
    ///////////////////////  Begin: Deal with loading in Fake/Prompt Lepton Histos + FakeLeptonCalculator Initalization
    /******************************************************************************************************************************/
    
    TString baseDirFakePromptRate = baseDirNonDataRootFile + "FakePromptRateHistos/";
    
    TString nameElec = "Elec";
    TString nameMuon = "Muon";
    TString nameFake = "Fake";
    TString namePrompt = "Prompt";
    
    TFile * fileElecFakeRate = TFile::Open(baseDirFakePromptRate + nameElec + nameFake + "Rate.root");
    TH2F * h_ElecFakeRate = (TH2F *) fileElecFakeRate->Get(TString("h_") + nameElec + nameFake + TString("Rate"));
    
    TFile * fileElecPromptRate = TFile::Open(baseDirFakePromptRate + nameElec + namePrompt + "Rate.root");
    TH2F * h_ElecPromptRate = (TH2F *) fileElecPromptRate->Get(TString("h_") + nameElec + namePrompt + TString("Rate"));
    
    TFile * fileMuonFakeRate = TFile::Open(baseDirFakePromptRate + nameMuon + nameFake + "Rate.root");
    TH2F * h_MuonFakeRate = (TH2F *) fileMuonFakeRate->Get(TString("h_") + nameMuon + nameFake + TString("Rate"));
    
    TFile * fileMuonPromptRate = TFile::Open(baseDirFakePromptRate + nameMuon + namePrompt + "Rate.root");
    TH2F * h_MuonPromptRate = (TH2F *) fileMuonPromptRate->Get(TString("h_") + nameMuon + namePrompt + TString("Rate"));
    
    FakeLeptonCalculator FLC; //Structure used for FakeLepton Weight Calculations
    
    /******************************************************************************************************************************/
    ///////////////////////  End: Deal with loading in Fake/Prompt Lepton Histos + FakeLeptonCalculator Initalization
    /******************************************************************************************************************************/
    
    
    TString fileTreeName;
    TString fOutName;    
    TH1F * h_eventCount, * h_CutFlow;    
    /////Event Variables/////////////////////
    bool  keepEvent;
    int numSavedLeps  = 2;
    int numSavedJets  = 3;
    int numSavedBJets = 2;
    
    BasicEventInfo BEI; // BEI.doData = 0;
    FilterTriggerInfo FTI;
    EventGenWeight EGW; EGW.SetGenParticleWeightDefault();
    
    EventGenLeptonParticleInfo EGLPI;
    EGLPI.EGLPIDefaultVarVals();
    
    EventGenSUSYParticleInfo EGSPI;
    EGSPI.InitializeVecs();
    EGSPI.EGSPIDefaultVarVals();
    
    EventGenQuarkParticleInfo EGQPI;
    
    EventGenEWKParticleInfo EGEWKPI;
    EGEWKPI.DefaultVarVals();
    
    
    EventPileUpInfo EPI;
    
    vector<int> systLB_Lepton, systUB_Lepton;
    vector<int> systLB_Jet, systUB_Jet;
    vector<int> systLB_SmearJet, systUB_SmearJet;
    
    vector<int> systLB_MET, systUB_MET;
    vector<int> systLB_SmearMET, systUB_SmearMET;
    
    vector<int> systLB_EDSI, systUB_EDSI;
    vector<int> systLB_SmearEDSI, systUB_SmearEDSI;
    
    const int numKinSysts = 6;
    const int numSystsTotal = 9;
    
    // Systs -- 1: LepES, 2: JetES, 3/4: BTag SF stuff, 5: JetSmear, 6: UncES
    int systLepES = 1;
    int systJetES = 2;
    int systBTagEffSF = 3;
    int systBMisTagSF = 4;
    int systJetSmear = 5;
    int systUncES = 6;
    int systLepEff = 7;
    int systGenRecoilRW = 8;
    int systStopXSec = 9;
    
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
    systLB_SmearMET.push_back(1); systUB_SmearMET.push_back(numKinSysts);
    
    systLB_EDSI.push_back(systLepES); systUB_EDSI.push_back(systBMisTagSF);
    systLB_SmearEDSI.push_back(1); systUB_SmearEDSI.push_back(numKinSysts);
    
    EventLepInfo ELI; ELI.ELIDefaultVarVals(numSavedLeps);  
    EventJetInfo EJI; EJI.EJIDefaultVarVals(numSavedJets, numSavedBJets);
    EventMETInfo EMI_PF, EMI_PF_noType0, SmearEMI_PF, SmearEMI_PF_noType0;
    EventMETInfo EMI_Calo;
    
    EMI_PF.METType = 0;
    EMI_PF_noType0.METType = 0;
    SmearEMI_PF.METType = 0;
    SmearEMI_PF_noType0.METType = 0;
    EMI_Calo.METType = 4;        
    
    EventStructPointerInfo ESPI, SmearESPI, ESPI_NoType0, SmearESPI_NoType0;
    ESPI.ZeroPointers();
    SmearESPI.ZeroPointers();
    ESPI_NoType0.ZeroPointers();
    SmearESPI_NoType0.ZeroPointers();
    
    vector<EventStructPointerInfo> ESPI_SystVarUp; ESPI_SystVarUp.resize(numSystsTotal + 1);
    vector<EventStructPointerInfo> ESPI_SystVarDown; ESPI_SystVarDown.resize(numSystsTotal + 1);
    
    for (int iSyst = 0; iSyst <= numSystsTotal; ++iSyst) {
        ESPI_SystVarUp[iSyst].ZeroPointers();
        ESPI_SystVarDown[iSyst].ZeroPointers();
    }
    
    vector<EventStructPointerInfo> SmearESPI_SystVarUp; SmearESPI_SystVarUp.resize(numSystsTotal + 1);
    vector<EventStructPointerInfo> SmearESPI_SystVarDown; SmearESPI_SystVarDown.resize(numSystsTotal + 1);
    
    for (int iSyst = 0; iSyst <= numSystsTotal; ++iSyst) {
        SmearESPI_SystVarUp[iSyst].ZeroPointers();
        SmearESPI_SystVarDown[iSyst].ZeroPointers();
    }
    
    vector<EventStructPointerInfo> SmearESPI_NoType0_SystVarUp; SmearESPI_NoType0_SystVarUp.resize(numSystsTotal + 1);
    vector<EventStructPointerInfo> SmearESPI_NoType0_SystVarDown; SmearESPI_NoType0_SystVarDown.resize(numSystsTotal + 1);

    for (int iSyst = 0; iSyst <= numSystsTotal; ++iSyst) {
        SmearESPI_NoType0_SystVarUp[iSyst].ZeroPointers();
        SmearESPI_NoType0_SystVarDown[iSyst].ZeroPointers();
    }

    
    EventDiStructureInfo EDSI; EDSI.EDSIDefaultVarVals();
    EventDiStructureInfo SmearEDSI; SmearEDSI.EDSIDefaultVarVals();
    EventJetInfo SmearEJI; SmearEJI.EJIDefaultVarVals(numSavedJets, numSavedBJets);
    
    vector<EventLepInfo> ELI_SystVarUp;   ELI_SystVarUp.resize(numKinSysts + 1);
    vector<EventLepInfo> ELI_SystVarDown; ELI_SystVarDown.resize(numKinSysts + 1);
    
    vector<EventJetInfo> EJI_SystVarUp;   EJI_SystVarUp.resize(numKinSysts + 1);
    vector<EventJetInfo> EJI_SystVarDown; EJI_SystVarDown.resize(numKinSysts + 1);
    
    vector<EventMETInfo> EMI_PF_SystVarUp;   EMI_PF_SystVarUp.resize(numKinSysts + 1);
    vector<EventMETInfo> EMI_PF_SystVarDown; EMI_PF_SystVarDown.resize(numKinSysts + 1);
    
    vector<EventDiStructureInfo> EDSI_SystVarUp; EDSI_SystVarUp.resize(numKinSysts + 1);                                                                       
    vector<EventDiStructureInfo> EDSI_SystVarDown; EDSI_SystVarDown.resize(numKinSysts + 1);                                                                       
    
    vector<EventJetInfo> SmearEJI_SystVarUp;   SmearEJI_SystVarUp.resize(numKinSysts + 1);
    vector<EventJetInfo> SmearEJI_SystVarDown; SmearEJI_SystVarDown.resize(numKinSysts + 1);
    
    vector<EventMETInfo> SmearEMI_PF_SystVarUp;   SmearEMI_PF_SystVarUp.resize(numKinSysts + 1);
    vector<EventMETInfo> SmearEMI_PF_SystVarDown; SmearEMI_PF_SystVarDown.resize(numKinSysts + 1);
    
    vector<EventMETInfo> SmearEMI_PF_noType0_SystVarUp;   SmearEMI_PF_noType0_SystVarUp.resize(numKinSysts + 1);
    vector<EventMETInfo> SmearEMI_PF_noType0_SystVarDown; SmearEMI_PF_noType0_SystVarDown.resize(numKinSysts + 1);
    
    vector<EventDiStructureInfo> SmearEDSI_SystVarUp; SmearEDSI_SystVarUp.resize(numKinSysts + 1);                                                                       
    vector<EventDiStructureInfo> SmearEDSI_SystVarDown; SmearEDSI_SystVarDown.resize(numKinSysts + 1);
    
    for (int iSyst = 1; iSyst <= numKinSysts; ++iSyst) {
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
        
        SmearEMI_PF_noType0_SystVarUp[iSyst].METType = 0;
        SmearEMI_PF_noType0_SystVarDown[iSyst].METType = 0;
    }
    ////Systematics Versions of the Event Info Structures////
    vector<BasicEventInfo> BEI_SystVarUp; BEI_SystVarUp.resize(numSystsTotal + 1);
    vector<BasicEventInfo> BEI_SystVarDown; BEI_SystVarDown.resize(numSystsTotal + 1);
    
    ////Systematics Versions of the Event Info Structures////

    float genMET_Pt = 0.0, genMET_Phi = 0.0;
    
    float METSig_Raw;
    
    
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    ///////////////////////  END: Deal with event variable initializations
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    
    /********************************************************************************************************/
    TString eventCountName;   
    /********************************************************************************************************/
    
    ////input cuts/commands
    int  startPoint, endPoint;
    
    ////input cuts/commands    
    /////loop over inputs
    
    PlotMakingRunParams PMRP;
    PMRP.DefaultVarVals();
    PMRP.SetVals(argc, argv);
    PMRP.SetStrings(2);
    
    BEI.blindData = PMRP.SMV.doBlindData;
    
    ////input cuts/commands    
    
    char Buffer[500];
    ifstream * outDirFile;
    TRegexp fCutSlash("[^/]+$");
    fOutName = "";    
//    TString outputPathName = (PMRP.SRS.isSignal) ? "signalOutputSavePath" : PMRP.PSIV.outputSavePathString;
    TString outputPathName = PMRP.PSIV.outputSavePathString;
    if (PMRP.PSIV.grabOutDir) {
        outDirFile = new ifstream(outputPathName + TString(".txt"));
        if (!(outDirFile->eof())) {
            outDirFile->getline(Buffer,500);
            fOutName += TString(string(Buffer));
            fOutName += "/"; //in case user forgot a slash
        }
    }
    fOutName += PMRP.PSIV.fInName(fCutSlash);
    fOutName += PMRP.OutStringAdd();
    fOutName += "_Output.root";
    cout << "saving to " << fOutName << endl;
    TFile * outputFile;
    outputFile = new TFile(fOutName,"RECREATE");
    fileTreeName = "OviSkimTree";    
    TChain fileTree(fileTreeName);
    TFile inputFile(PMRP.PSIV.fInName + TString(".root"));
    h_CutFlow = (TH1F *) inputFile.Get("h_CutFlow");
    
    /////Set up the tree////////
    fileTree.Add(PMRP.PSIV.fInName + TString(".root"));        
    
    ///Out tree information
    fileTree.SetBranchAddress( "T_METPF_Sig",       &METSig_Raw);
    SetInTreeBranch_PlotMaker_BasicInfo(&fileTree, &FTI, &EPI, &BEI, &EGW);
    SetInTreeBranch_PlotMaker_METInfo(&fileTree, &EMI_PF, 0, false, PMRP.SMV.doPhiCorr);
    if (!PMRP.PSIV.useOldNTuple) {
        SetInTreeBranch_PlotMaker_METInfo(&fileTree, &EMI_PF_noType0, 0, false, PMRP.SMV.doPhiCorr, "_noType0");
        SetInTreeBranch_PlotMaker_METInfo(&fileTree, &EMI_Calo, 0, false, PMRP.SMV.doPhiCorr);
    }
    SetInTreeBranch_PlotMaker_LeptonInfo(&fileTree, &ELI, 0, PMRP.SMV.doData, PMRP.SMV.keepLooseLeps);
    SetInTreeBranch_PlotMaker_JetInfo(&fileTree, &EJI, 0, false);
    SetInTreeBranch_PlotMaker_DiStructureInfo(&fileTree, &EDSI, 0, false, PMRP.SMV.doPhiCorr);
    if (!PMRP.SMV.doData) {
        if (PMRP.SMV.doOfficialSmear) {
            SetInTreeBranch_PlotMaker_METInfo(&fileTree, &SmearEMI_PF, 0, true, PMRP.SMV.doPhiCorr);
        }
        else {
            SetInTreeBranch_PlotMaker_SpecialMETInfo(&fileTree, &SmearEMI_PF, 0, true, PMRP.SMV.doPhiCorr);   
        }
        if (!PMRP.PSIV.useOldNTuple) {
            SetInTreeBranch_PlotMaker_SpecialMETInfo(&fileTree, &SmearEMI_PF_noType0, 0, true, PMRP.SMV.doPhiCorr, "_noType0");
        }
        SetInTreeBranch_PlotMaker_JetInfo(&fileTree, &SmearEJI, 0, true);
        SetInTreeBranch_PlotMaker_DiStructureInfo(&fileTree, &SmearEDSI, 0, true, PMRP.SMV.doPhiCorr);
        for (int iSyst = 1; iSyst <= numKinSysts; ++iSyst) {
            if (InSystBound(iSyst, &systLB_Lepton, &systUB_Lepton)) {
                SetInTreeBranch_PlotMaker_LeptonInfo(&fileTree, &ELI_SystVarUp[iSyst],    1 * iSyst, PMRP.SMV.doData);
                SetInTreeBranch_PlotMaker_LeptonInfo(&fileTree, &ELI_SystVarDown[iSyst], -1 * iSyst, PMRP.SMV.doData);
            }            
            if (InSystBound(iSyst, &systLB_Jet, &systUB_Jet)) {
                SetInTreeBranch_PlotMaker_JetInfo(&fileTree, &EJI_SystVarUp[iSyst],    1 * iSyst, false);
                SetInTreeBranch_PlotMaker_JetInfo(&fileTree, &EJI_SystVarDown[iSyst], -1 * iSyst, false);
            }
            if (InSystBound(iSyst, &systLB_MET, &systUB_MET)) {
                SetInTreeBranch_PlotMaker_METInfo(&fileTree, &EMI_PF_SystVarUp[iSyst],    1 * iSyst, false, true);
                SetInTreeBranch_PlotMaker_METInfo(&fileTree, &EMI_PF_SystVarDown[iSyst], -1 * iSyst, false, true);
            }
            if (InSystBound(iSyst, &systLB_SmearMET, &systUB_SmearMET)) {
                if (PMRP.SMV.doOfficialSmear) {
                    SetInTreeBranch_PlotMaker_METInfo(&fileTree, &SmearEMI_PF_SystVarUp[iSyst],    1 * iSyst, true, true);
                    SetInTreeBranch_PlotMaker_METInfo(&fileTree, &SmearEMI_PF_SystVarDown[iSyst], -1 * iSyst, true, true);
                }
                else {
                    SetInTreeBranch_PlotMaker_SpecialMETInfo(&fileTree, &SmearEMI_PF_SystVarUp[iSyst],    1 * iSyst, true, true);
                    SetInTreeBranch_PlotMaker_SpecialMETInfo(&fileTree, &SmearEMI_PF_SystVarDown[iSyst], -1 * iSyst, true, true);
                }
            }
            if (!PMRP.PSIV.useOldNTuple) {
                if (InSystBound(iSyst, &systLB_SmearMET, &systUB_SmearMET)) {
                    SetInTreeBranch_PlotMaker_SpecialMETInfo(&fileTree, &SmearEMI_PF_noType0_SystVarUp[iSyst],    1 * iSyst, true, true, "_noType0");
                    SetInTreeBranch_PlotMaker_SpecialMETInfo(&fileTree, &SmearEMI_PF_noType0_SystVarDown[iSyst], -1 * iSyst, true, true, "_noType0");
                }
            }
            if (InSystBound(iSyst, &systLB_SmearJet, &systUB_SmearJet)) {
                SetInTreeBranch_PlotMaker_JetInfo(&fileTree, &SmearEJI_SystVarUp[iSyst],    1 * iSyst, true);
                SetInTreeBranch_PlotMaker_JetInfo(&fileTree, &SmearEJI_SystVarDown[iSyst], -1 * iSyst, true);
            }
            if (InSystBound(iSyst, &systLB_EDSI, &systUB_EDSI)) {                
                SetInTreeBranch_PlotMaker_DiStructureInfo(&fileTree, &EDSI_SystVarUp[iSyst],    1 * iSyst, false, true);
                SetInTreeBranch_PlotMaker_DiStructureInfo(&fileTree, &EDSI_SystVarDown[iSyst], -1 * iSyst, false, true);
            }
            if (InSystBound(iSyst, &systLB_SmearEDSI, &systUB_SmearEDSI)) {                
                SetInTreeBranch_PlotMaker_DiStructureInfo(&fileTree, &SmearEDSI_SystVarUp[iSyst],    1 * iSyst, true, true);
                SetInTreeBranch_PlotMaker_DiStructureInfo(&fileTree, &SmearEDSI_SystVarDown[iSyst], -1 * iSyst, true, true);
            }
            
        }
        fileTree.SetBranchAddress("TGenMET",                      &genMET_Pt);
        fileTree.SetBranchAddress("TGenMETPhi",                   &genMET_Phi);                
        
        SetInTreeBranch_PlotMaker_GenLepton(&fileTree, &EGLPI);
        SetInTreeBranch_PlotMaker_GenQuark(&fileTree, &EGQPI);
        
    }
        
    
    if (fileTree.GetBranch("TGenStop_Mass_0")){
        BEI.hasStopInfo = true;
        SetInTreeBranch_PlotMaker_SUSYInfo(&fileTree, &EGSPI);
        PMRP.SRS.isT2tt = PMRP.PSIV.fInName.Contains("T2tt");
        PMRP.SRS.isT2ttFineBin = (PMRP.SRS.isT2tt && PMRP.PSIV.fInName.Contains("FineBin"));
        float stopMassToUseForXSec;
        float roundedStopMass;
        if (!PMRP.SRS.isT2ttFineBin) {
            roundedStopMass = floor((EGSPI.vecVecGenSUSYRoundMass[0][0]+12.5)/25.)*25.;
            stopMassToUseForXSec = roundedStopMass;
            PMRP.SRS.massDiffThresh = 12.5;
            //                cout << "roundedStopMass " << roundedStopMass << endl;                                                                                         
        }
        else {
            roundedStopMass = floor((EGSPI.vecVecGenSUSYRoundMass[0][0]+5)/10.)*10.;
            stopMassToUseForXSec = roundedStopMass;
            //      stopMassToUseForXSec = PMRP.SRS.grabStopMass ;                                                                                                                     
        }
        cout << "roundedStopMass " << roundedStopMass << endl;
        
        stopMassToUseForXSec = PMRP.SRS.grabStopMass ;
        StopXSec theStopXSec = getCrossSectionStop(stopMassToUseForXSec);
        EGSPI.StopXSec = theStopXSec.stopProdXsec;
        EGSPI.StopXSecErr = theStopXSec.stopProdXsecUncert * EGSPI.StopXSec;
        
        cout << "stopWeight " << EGSPI.StopXSec << endl;
        cout << "stopWeightErr " << EGSPI.StopXSecErr << endl;
    }
    if (PMRP.SRS.isSignal) {
        h_TrigSFVec.push_back(h_TrigSFSigMuMu);
        h_TrigSFVec.push_back(h_TrigSFSigEE);
        h_TrigSFVec.push_back(h_TrigSFSigEMu);        

        h_LepIDIsoElecSFToUse = h_LepIDIsoSFSigElec;
        h_LepIDIsoMuonSFToUse = h_LepIDIsoSFSigMuon;
 
 
//        h_LepIDIsoElecSFToUse = h_LepIDIsoSFSigEE;
//        h_LepIDIsoMuonSFToUse = h_LepIDIsoSFSigMuMu;
    }
    else {
        h_TrigSFVec.push_back(h_TrigSFBkgMuMu);
        h_TrigSFVec.push_back(h_TrigSFBkgEE);
        h_TrigSFVec.push_back(h_TrigSFBkgEMu);
        h_LepIDIsoElecSFToUse = h_LepIDIsoSFBkgElec;
        h_LepIDIsoMuonSFToUse = h_LepIDIsoSFBkgMuon;
    }
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    ///////////////////////  BEGIN: Deal with booking of histograms
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    ////Book histograms and histogram names
    outputFile->cd();
    
    /******************************************************************************************************************************/
    
    /////Set up preliminary things
    //Set up the Default Stop Hist Binnings and Bounds
    StopHistBinBounds basicSHBB; basicSHBB.DefaultVarVals();
    
    //Set up the mapping of input axis label variables to full out labels;
    labelMap mapVartoLabel;
    SetStringKey_StSMap_Composite(mapVartoLabel);
    
    // Set up the SubSampleVector (i.e. the different classes of events
    int whichSSType = PMRP.SRS.isSignal ? 2 : -1;
    vector<SampleT> * subSampVec = new vector<SampleT>;
    SetSubSampVec(subSampVec, whichSSType);
    
    // Set up the systematics that we'll book histograms for
    vector<SystT> * systVec = new vector<SystT>;
    SetSystVec(systVec, true);
    /////Set up preliminary things
    
    /******************************************************************************************************************************/
    
    /////initialize the vectors of HistogramTs and then create them
    int numSpaceDimensions = 3;    
    vector< vector<HistogramT> *> vecVecHistT_Inclusive; vecVecHistT_Inclusive.resize(numSpaceDimensions);
    vector< vector<HistogramT> *> vecVecHistT_MET_noType0; vecVecHistT_MET_noType0.resize(numSpaceDimensions);
    
    for (int iDim = 0; iDim < numSpaceDimensions; ++iDim) {
        vecVecHistT_Inclusive[iDim] = new vector<HistogramT>;
        SetHistTVec_Inclusive(vecVecHistT_Inclusive[iDim], &basicSHBB, &mapVartoLabel, iDim + 1, PMRP.SRS.isSignal);
        AddPatsyName(vecVecHistT_Inclusive[iDim]);
        vecVecHistT_MET_noType0[iDim] = new vector<HistogramT>;
        //SetHistTVec_MET_noType0(vecVecHistT_MET_noType0[iDim], &basicSHBB, &mapVartoLabel, iDim + 1, PMRP.SRS.isSignal);
        //AddPatsyName(vecVecHistT_MET_noType0[iDim]);
    }
    
    vector< vector<HistogramT> *> vecVecHistT_Inclusive_Smear;
    vector< vector<HistogramT> *> vecVecHistT_MET_noType0_Smear;
    
    vector< vector<HistogramT> *> vecVecHistT_Inclusive_Syst;
    vector< vector<HistogramT> *> vecVecHistT_MET_noType0_Syst;
    
    vector< vector<HistogramT> *> vecVecHistT_Inclusive_Smear_Syst;
    vector< vector<HistogramT> *> vecVecHistT_MET_noType0_Smear_Syst;
    
    int systStopBook = 2; //3/30/14: temporarily don't book systematics for 3D hists -- too much memory 
    
    if (!PMRP.SMV.doData) {
        vecVecHistT_Inclusive_Smear.resize(numSpaceDimensions);
        vecVecHistT_MET_noType0_Smear.resize(numSpaceDimensions);
        
        for (int iDim = 0; iDim < numSpaceDimensions; ++iDim) {
            vecVecHistT_Inclusive_Smear[iDim] = new vector<HistogramT>;
            SetHistTVec_Inclusive_Smeared(vecVecHistT_Inclusive_Smear[iDim], &basicSHBB, &mapVartoLabel, iDim + 1, PMRP.SRS.isSignal);
            AddPatsyName(vecVecHistT_Inclusive_Smear[iDim]);
            
            vecVecHistT_MET_noType0_Smear[iDim] = new vector<HistogramT>;
            //SetHistTVec_MET_noType0_Smeared(vecVecHistT_MET_noType0_Smear[iDim], &basicSHBB, &mapVartoLabel, iDim + 1, PMRP.SRS.isSignal); 
            //AddPatsyName(vecVecHistT_MET_noType0_Smear[iDim]);
        }
        
        if (PMRP.PSIV.fInName) {
            vecVecHistT_Inclusive_Syst.resize(numSpaceDimensions);
            vecVecHistT_MET_noType0_Syst.resize(numSpaceDimensions);
            
            vecVecHistT_Inclusive_Smear_Syst.resize(numSpaceDimensions);
            vecVecHistT_MET_noType0_Smear_Syst.resize(numSpaceDimensions);
            
            
            for (int iDim = 0; iDim < numSpaceDimensions; ++iDim) {
                if (iDim == systStopBook) continue;                
                vecVecHistT_Inclusive_Syst[iDim] = new vector<HistogramT>;
                AddSystHists(vecVecHistT_Inclusive_Syst[iDim], vecVecHistT_Inclusive[iDim], systVec, PMRP.SRS.isSignal);
                
                vecVecHistT_MET_noType0_Syst[iDim] = new vector<HistogramT>;
                AddSystHists(vecVecHistT_MET_noType0_Syst[iDim], vecVecHistT_MET_noType0[iDim], systVec, PMRP.SRS.isSignal);
                
                
                vecVecHistT_Inclusive_Smear_Syst[iDim] = new vector<HistogramT>;
                AddSystHists(vecVecHistT_Inclusive_Smear_Syst[iDim], vecVecHistT_Inclusive_Smear[iDim], systVec, PMRP.SRS.isSignal);
                
                vecVecHistT_MET_noType0_Smear_Syst[iDim] = new vector<HistogramT>;
                AddSystHists(vecVecHistT_MET_noType0_Smear_Syst[iDim], vecVecHistT_MET_noType0[iDim], systVec, PMRP.SRS.isSignal);
            }
        }
    }
    /////initialize the vectors of HistogramTs and then create them

    /******************************************************************************************************************************/
    
    /////Declare the Histogram Maps that will be used for determining how to fill the histograms
    ///Basic 1D HistMaps
    HMap_1D histMap_1D;
    HMap_1D histMap_1D_Syst;
    ///Smeared 1D HistMaps
    HMap_1D histMap_1D_Smear;
    HMap_1D histMap_1D_Smear_Syst;
    
    ///Basic 2D HistMaps
    HMap_2D histMap_2D;
    HMap_2D histMap_2D_Syst;
    ///Smeared 2D HistMaps
    HMap_2D histMap_2D_Smear;
    HMap_2D histMap_2D_Smear_Syst;

    ///Basic 3D HistMaps
    HMap_3D histMap_3D;
    HMap_3D histMap_3D_Syst;
    ///Smeared 3D HistMaps
    HMap_3D histMap_3D_Smear;
    HMap_3D histMap_3D_Smear_Syst;
        
    //Hist Maps for MET w/o Type 0 corrections
    HMap_1D histMap_1D_METnoType0;
    HMap_1D histMap_1D_METnoType0_Syst;
    HMap_1D histMap_1D_SmearMETnoType0;
    HMap_1D histMap_1D_SmearMETnoType0_Syst;
    HMap_2D histMap_2D_METnoType0;
    HMap_2D histMap_2D_METnoType0_Syst;
    HMap_2D histMap_2D_SmearMETnoType0;
    HMap_2D histMap_2D_SmearMETnoType0_Syst;
    HMap_3D histMap_3D_METnoType0;
    HMap_3D histMap_3D_METnoType0_Syst;
    HMap_3D histMap_3D_SmearMETnoType0;
    HMap_3D histMap_3D_SmearMETnoType0_Syst;
    /////Declare the Histogram Maps that will be used for determining how to fill the histograms    
    /******************************************************************************************************************************/
    /////Set up vectors for custom bins (patsies right now)
    vector<int> * vecNBins = new vector<int>;    
    vector<Double_t *> vecBinEdges;
    /////Set up vectors for custom bins (patsies right now)
    /******************************************************************************************************************************/
    /////Book the Histograms
    for (unsigned int iSubSamp = 0; iSubSamp < subSampVec->size(); ++iSubSamp) {
        if (PMRP.PSIV.doVerbosity_Plots) {
            cout << "currently on iSubSamp " << iSubSamp << " which is " << subSampVec->at(iSubSamp).histNameSuffix << endl;
        }
        for (int iDim = 0; iDim < numSpaceDimensions; ++iDim) {
            if (PMRP.PSIV.doVerbosity_Plots) {
                cout << "currently on iDim " << iDim << endl;
            }
            // book the normal hists
            if (PMRP.PSIV.doVerbosity_Plots) {
                cout << "going to try and book Normal " << endl;
            }
            BookHists(vecVecHistT_Inclusive[iDim], histMap_1D, histMap_2D, histMap_3D, &subSampVec->at(iSubSamp), vecNBins, vecBinEdges, iDim + 1);
            if (PMRP.PSIV.doVerbosity_Plots) {
                cout << "going to try and book Type 0 " << endl;
            }
            // book Type0 hists
            BookHists(vecVecHistT_MET_noType0[iDim], histMap_1D_METnoType0, histMap_2D_METnoType0, histMap_3D_METnoType0, &subSampVec->at(iSubSamp), vecNBins, vecBinEdges, iDim + 1);
            
            if (!PMRP.SMV.doData) {
                // book the smeared hists
                BookHists(vecVecHistT_Inclusive_Smear[iDim], histMap_1D_Smear, histMap_2D_Smear, histMap_3D_Smear, &subSampVec->at(iSubSamp), vecNBins, vecBinEdges, iDim + 1);
                
                // book the smeared hists w/o Type0
                BookHists(vecVecHistT_MET_noType0_Smear[iDim], histMap_1D_SmearMETnoType0, histMap_2D_SmearMETnoType0, histMap_3D_SmearMETnoType0, &subSampVec->at(iSubSamp), vecNBins, vecBinEdges, iDim + 1);
                
                if (PMRP.PSIV.fInName) {
                    if (iDim == systStopBook) continue;
                    // book the syst. hists
                    BookHists(vecVecHistT_Inclusive_Syst[iDim], histMap_1D_Syst, histMap_2D_Syst, histMap_3D_Syst, &subSampVec->at(iSubSamp), vecNBins, vecBinEdges, iDim + 1);
                    
                    // book the syst. hists w/o Type0
                    BookHists(vecVecHistT_MET_noType0_Syst[iDim], histMap_1D_METnoType0_Syst, histMap_2D_METnoType0_Syst, histMap_3D_METnoType0_Syst, &subSampVec->at(iSubSamp), vecNBins, vecBinEdges, iDim + 1);
                    
                    // book the syst. smeared hists
                    BookHists(vecVecHistT_Inclusive_Smear_Syst[iDim], histMap_1D_Smear_Syst, histMap_2D_Smear_Syst, histMap_3D_Smear_Syst, &subSampVec->at(iSubSamp), vecNBins, vecBinEdges, iDim + 1);
                    
                    // book the syst. smeared hists w/o Type0
                    BookHists(vecVecHistT_MET_noType0_Smear_Syst[iDim], histMap_1D_SmearMETnoType0_Syst, histMap_2D_SmearMETnoType0_Syst, histMap_3D_SmearMETnoType0_Syst, &subSampVec->at(iSubSamp), vecNBins, vecBinEdges, iDim + 1);
                }
                
            }
            
        }
    }
    /////Book the Histograms
    
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    ///////////////////////  END: Deal with booking of histograms
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    ///////////////////////  BEGIN: Map Declarations
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    
    /////declare the maps that will be used for determining whether a given event passes a set of cuts
    passCutMap subSampBool, subSampBool_Smear;
    
    passCutMap subSampBool_noType0, subSampBool_Smear_noType0;
    
    vector<passCutMap> subSampBool_SystVarUp, subSampBool_SystVarDown;
    subSampBool_SystVarUp.resize(numSystsTotal + 1);
    subSampBool_SystVarDown.resize(numSystsTotal + 1);
    
    vector<passCutMap> subSampBool_Smear_SystVarUp, subSampBool_Smear_SystVarDown;
    subSampBool_Smear_SystVarUp.resize(numSystsTotal + 1);
    subSampBool_Smear_SystVarDown.resize(numSystsTotal + 1);
    
    vector<passCutMap> subSampBool_Smear_noType0_SystVarUp, subSampBool_Smear_noType0_SystVarDown;
    subSampBool_Smear_noType0_SystVarUp.resize(numSystsTotal + 1);
    subSampBool_Smear_noType0_SystVarDown.resize(numSystsTotal + 1);
    /////declare the maps that will be used for determining whether a given event passes a set of cuts

    /******************************************************************************************************************************/
    
    //maps of ints to booleans to determine whether given systematics need to be cared about
    
    mIB mapIntBool_BEI, mapIntBool_ELI, mapIntBool_EMI, mapIntBool_EJI, mapIntBool_EDSI; //no smearing map{Int:Bool}s
    mIB mapIntBool_SmearEJI, mapIntBool_SmearEMI, mapIntBool_SmearEDSI; //"JER/MET" smearing map{Int:Bool}s
    SetMapIntBool(mapIntBool_BEI,  0, false);
    /*
     cout << "bout to run on map " << endl;
     for (mIB::iterator xx = mapIntBool_BEI.begin(); xx != mapIntBool_BEI.end(); ++xx) {
     cout << "first " << xx->first << endl;
     cout << "second " << xx->second << endl;
     }
     */
    SetMapIntBool(mapIntBool_ELI,  1, false);
    SetMapIntBool(mapIntBool_EJI,  2, false);
    SetMapIntBool(mapIntBool_EMI,  3, false);
    SetMapIntBool(mapIntBool_EDSI, 4, false);
    
    SetMapIntBool(mapIntBool_SmearEJI,  2, true);
    SetMapIntBool(mapIntBool_SmearEMI,  3, true);
    SetMapIntBool(mapIntBool_SmearEDSI, 4, true);
    
    //maps of ints to booleans to determine whether given systematics need to be cared about
    
    /******************************************************************************************************************************/

    /////declare the maps that will contain the values of event variables (using strings as keys)
    StV_Map StVM_Basic;
    vector<StV_Map> StVM_Basic_SystVarUp; StVM_Basic_SystVarUp.resize(numSystsTotal + 1);
    vector<StV_Map> StVM_Basic_SystVarDown; StVM_Basic_SystVarDown.resize(numSystsTotal + 1);
    
    StV_Map StVM_Smear;
    vector<StV_Map> StVM_Smear_SystVarUp; StVM_Smear_SystVarUp.resize(numSystsTotal + 1);
    vector<StV_Map> StVM_Smear_SystVarDown; StVM_Smear_SystVarDown.resize(numSystsTotal + 1);
    
    StV_Map StVM_MET_noType0;
    
    StV_Map StVM_Smear_MET_noType0;
    vector<StV_Map> StVM_Smear_MET_noType0_SystVarUp; StVM_Smear_MET_noType0_SystVarUp.resize(numSystsTotal + 1);
    vector<StV_Map> StVM_Smear_MET_noType0_SystVarDown; StVM_Smear_MET_noType0_SystVarDown.resize(numSystsTotal + 1);
    /////declare the maps that will contain the values of event variables (using strings as keys)
    
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    ///////////////////////  END: Map Declarations
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/        
    
    /////Iterate over events  
    cout << "--- Total Events in file: " << fileTree.GetEntries() << " events" << endl;
    if (PMRP.RRL.nEvents < 0) {
        cout << "running over all events " << endl;  
        PMRP.RRL.nEvents = fileTree.GetEntries();
    }
    else {
        if (PMRP.RRL.nEvents > fileTree.GetEntries()) PMRP.RRL.nEvents = fileTree.GetEntries();
        cout << "running on just " << PMRP.RRL.nEvents << " events " << endl;
    }
    vector<int> * breakPoints = new vector<int>;
    int currBreakPoint;
    int endBreakPoint = -1;
    breakPoints->push_back(0);
    /*
    if (numBreakPoints > 0) {
        endBreakPoint = (numBreakPoints + 1 > PMRP.RRL.nEvents) ? PMRP.RRL.nEvents : numBreakPoints + 1;
        for (int i = 1; i < endBreakPoint; ++i) {
            currBreakPoint = i * PMRP.RRL.nEvents / endBreakPoint;
            breakPoints->push_back(currBreakPoint);
        }
    }
    */
    int startPointNum = 1;
    startPoint = breakPoints->at(startPointNum - 1);
    cout << " startpoint (event number 'startpoint'): " << startPoint << endl;
    endPoint = ((int) breakPoints->size() <= startPointNum) ? PMRP.RRL.nEvents : breakPoints->at(startPointNum);
    cout << " endpoint (the 'endpoint + 1' th event won't be run over -- i.e. event number 'endpoint'): " << endPoint << endl;    
    eventCountName = "h_eventCount";
    h_eventCount = (TH1F *) inputFile.Get(eventCountName);
    
    
    int modFactor = 1E3;
//    cout << "START POINT is 930" << endl;
//    startPoint = 930;
    for (Long64_t ievt = startPoint; ievt < PMRP.RRL.nEvents;ievt++) {
        //    for (Long64_t ievt=0; ievt<1000;ievt++)
        if (startPoint > 0 ) {
            if ((ievt / startPoint == 1) && (ievt % startPoint == 0)) cout << "ievt at start point: " << ievt << endl;
        }
        if (ievt%modFactor == 0) cout << ievt << endl;
        if (ievt == endPoint) {
            cout << "ievt at end point (note, not running on this event!):" << ievt << endl;
            break;
        }
                        
        fileTree.GetEntry(ievt);
        
        if (PMRP.RRL.doSpecRun) {
            if (BEI.runNumber == (unsigned int) PMRP.RRL.whichRun) {
                cout << "Pre anything Run:Event:Lumi" << BEI.runNumber << ":" << BEI.eventNumber << ":" << BEI.lumiBlock << endl;
                if (PMRP.PSIV.doVerbosity) {
                    cout << "PU Weight " << EPI.nPUWeight1DTrue << endl;
                }
            }
            else {
                continue;
            }
        }
        else if (PMRP.RRL.doSpecRunEvent) {
            /*
            cout << "PMRP.RRL.whichRun " << PMRP.RRL.whichRun << endl;
            cout << "BEI.runNumber " << BEI.runNumber << endl;
            cout << "BEI.eventNumber " << BEI.eventNumber << endl;
            cout << "PMRP.RRL.whichEvent " << PMRP.RRL.whichEvent << endl;
            */
            if (!(BEI.runNumber == (unsigned int) PMRP.RRL.whichRun && BEI.eventNumber == (unsigned int) PMRP.RRL.whichEvent)) {
                continue;
            }
            else {
                cout << "Pre anything Run:Event:Lumi" << BEI.runNumber << ":" << BEI.eventNumber << ":" << BEI.lumiBlock << endl;
                if (PMRP.PSIV.doVerbosity) {
                    cout << "PU Weight " << EPI.nPUWeight1DTrue << endl;
                }
            }
        }
        /*
        if (!PMRP.SMV.doOfficialSmear) {
            SmearEJI = EJI;
            SmearEDSI = EDSI;
            for (int iSyst = 1; iSyst <= numKinSysts; ++iSyst) {
                SmearEJI_SystVarUp[iSyst] = EJI_SystVarUp[iSyst];
                SmearEJI_SystVarDown[iSyst] = EJI_SystVarDown[iSyst];
                
                SmearEDSI_SystVarUp[iSyst] = EDSI_SystVarUp[iSyst];
                SmearEDSI_SystVarDown[iSyst] = EDSI_SystVarDown[iSyst];
            }
        }
        */
        
        if (PMRP.SMV.whichDiLepType > -1) {
            if (PMRP.PSIV.doVerbosity_Plots) {
                cout << "checking this event!!!! " << endl;
            }
            if (ELI.EventDiLepType != PMRP.SMV.whichDiLepType && ELI_SystVarUp[systLepES].EventDiLepType != PMRP.SMV.whichDiLepType && ELI_SystVarDown[systLepES].EventDiLepType != PMRP.SMV.whichDiLepType) continue;
        }
        
        ELI.EventDiLepMass = EDSI.DP_DiLepton.BVC.Vec_Mass;
        ELI.EventDiLepinZMass = ELI.EventDiLepMass > 76 && ELI.EventDiLepMass < 106;
        if (PMRP.PSIV.fInName) {
            ELI_SystVarUp[systLepES].EventDiLepMass = EDSI_SystVarUp[systLepES].DP_DiLepton.BVC.Vec_Mass;
            ELI_SystVarUp[systLepES].EventDiLepinZMass = ELI_SystVarUp[systLepES].EventDiLepMass > 76 && ELI_SystVarUp[systLepES].EventDiLepMass < 106;
            ELI_SystVarDown[systLepES].EventDiLepMass = EDSI_SystVarDown[systLepES].DP_DiLepton.BVC.Vec_Mass;
            ELI_SystVarDown[systLepES].EventDiLepinZMass = ELI_SystVarDown[systLepES].EventDiLepMass > 76 && ELI_SystVarDown[systLepES].EventDiLepMass < 106;
        }
        
        if (!PMRP.SMV.doData) {
            //cout << "EDSI DPhiZMET before " << SmearEDSI.DP_ZMET.DPhiInputObjects << endl;
            SmearEDSI.SetDiLepVals(&EDSI);
            SmearEDSI.SetMETVars(&ELI, &SmearEMI_PF);
            //cout << "EDSI DPhiZMET after " << SmearEDSI.DP_ZMET.DPhiInputObjects << endl;
            
            SmearEDSI_SystVarUp[systLepES].SetDiLepVals(&EDSI_SystVarUp[systLepES]);
            SmearEDSI_SystVarUp[systLepES].SetMETVars(&ELI_SystVarUp[systLepES], &SmearEMI_PF_SystVarUp[systLepES]);
            SmearEDSI_SystVarDown[systLepES].SetDiLepVals(&EDSI_SystVarDown[systLepES]);
            SmearEDSI_SystVarDown[systLepES].SetMETVars(&ELI_SystVarDown[systLepES], &SmearEMI_PF_SystVarDown[systLepES]);

            
            for (int iSyst = 2; iSyst <= 6; ++iSyst) {
                SmearEDSI_SystVarUp[iSyst].SetDiLepVals(&EDSI);;
                SmearEDSI_SystVarDown[iSyst].SetDiLepVals(&EDSI);;
            }                
            SmearEDSI_SystVarUp[systJetES].SetMETVars(&ELI, &SmearEMI_PF_SystVarUp[systJetES]);
            SmearEDSI_SystVarDown[systJetES].SetMETVars(&ELI, &SmearEMI_PF_SystVarDown[systJetES]);
            
            SmearEDSI_SystVarUp[systBTagEffSF].SetMETVars(&ELI, &SmearEMI_PF);
            SmearEDSI_SystVarDown[systBTagEffSF].SetMETVars(&ELI, &SmearEMI_PF);
            
            SmearEDSI_SystVarUp[systBMisTagSF].SetMETVars(&ELI, &SmearEMI_PF);
            SmearEDSI_SystVarDown[systBMisTagSF].SetMETVars(&ELI, &SmearEMI_PF);
            
            SmearEDSI_SystVarUp[systJetSmear].SetMETVars(&ELI, &SmearEMI_PF_SystVarUp[systJetSmear]);
            SmearEDSI_SystVarDown[systJetSmear].SetMETVars(&ELI, &SmearEMI_PF_SystVarDown[systJetSmear]);
            
            SmearEDSI_SystVarUp[systUncES].SetMETVars(&ELI, &SmearEMI_PF_SystVarUp[systUncES]);
            SmearEDSI_SystVarDown[systUncES].SetMETVars(&ELI, &SmearEMI_PF_SystVarDown[systUncES]);
        
        }
        
        BEI.weight = EPI.nPUWeight1DTrue;
        BEI.preNVtxRWWeight = 1.;
        BEI.basicWeight = 1.;
        //        cout << "test 1 " << endl;
//        cout << "test 1 " << endl;
//        bool d = ELI.vecEventLeps[0].isPromptLepton && ELI.vecEventLeps[1].isPromptLepton;
//        cout << "test " << d << endl;
        
        ELI.doEvent &= FTI.EventPassTrigger(&ELI);
        if (!PMRP.SMV.doData) {
            ELI_SystVarUp[systLepES].doEvent &= FTI.EventPassTrigger(&ELI_SystVarUp[systLepES]);
            ELI_SystVarDown[systLepES].doEvent &= FTI.EventPassTrigger(&ELI_SystVarDown[systLepES]);
            
            if (PMRP.SMV.doDropFakes) {
                ELI.doEvent &= (ELI.vecEventLeps[0].isPromptLepton && ELI.vecEventLeps[1].isPromptLepton);
                ELI_SystVarUp[systLepES].doEvent &= (ELI_SystVarUp[systLepES].vecEventLeps[0].isPromptLepton && ELI_SystVarUp[systLepES].vecEventLeps[1].isPromptLepton);
                ELI_SystVarDown[systLepES].doEvent &= (ELI_SystVarDown[systLepES].vecEventLeps[0].isPromptLepton && ELI_SystVarDown[systLepES].vecEventLeps[1].isPromptLepton);  
            }
            //                ELI.doEvent &= (ELI.vecEventLeps[0].isPromptLepton && ELI.vecEventLeps[1].isPromptLepton);
            //                ELI_SystVarUp[systLepES].doEvent &= (ELI_SystVarUp[systLepES].vecEventLeps[0].isPromptLepton && ELI_SystVarUp[systLepES].vecEventLeps[1].isPromptLepton);
            //                ELI_SystVarDown[systLepES].doEvent &= (ELI_SystVarDown[systLepES].vecEventLeps[0].isPromptLepton && ELI_SystVarDown[systLepES].vecEventLeps[1].isPromptLepton);        
            //                cout << "not skipping yet " << endl;
            keepEvent = (ELI.doEvent || ELI_SystVarUp[systLepES].doEvent || ELI_SystVarDown[systLepES].doEvent);
        }
        else {
            keepEvent = ELI.doEvent;
        }
        if (!keepEvent) continue;
//        cout << "test 2 " << d << endl;
        //        cout << "test 2 " << endl;
        if (!PMRP.SMV.doData) {
            if (PMRP.SRS.isSignal) {
//                cout << "test 1 " << endl;
                if (!BEI.hasStopInfo) continue;
//                cout << "test 2 " << endl;
                
                if (!EGSPI.PassMassCutPlotMaker(PMRP.SRS.grabStopMass, PMRP.SRS.grabChi0Mass , PMRP.SRS.grabCharginoMassFrac, PMRP.SRS.massDiffThresh, PMRP.SRS.CharginoMassFracDiffThresh, PMRP.SRS.isT2tt)) continue;     
                PMRP.SRS.weightPol = EGSPI.GetStopPolWeight(PMRP.SRS.stopPolPercentRight);
                if (PMRP.PSIV.doVerbosity) {
                    cout << "weightPol " << PMRP.SRS.weightPol << endl;
                    cout << "BEI weight before pol weight " << BEI.weight << endl;
                }
//                cout << "BEI weight ber" << BEI.weight << endl;
                BEI.ScaleWeights(PMRP.SRS.weightPol, true, false);
                if (PMRP.PSIV.doVerbosity) {
                    cout << "BEI weight after pol weight " << BEI.weight << endl;
                }
            }
        }
        else if (PMRP.SMV.doData && PMRP.SMV.estFakeLep) {
//            float weightFakeLepDD = FLC.GetWeightFullChain(&ELI, h_ElecFakeRate, h_ElecPromptRate, h_MuonFakeRate, h_MuonPromptRate, true);
            float weightFakeLepDD = FLC.GetWeightFullChain(&ELI, h_ElecFakeRate, h_ElecPromptRate, h_MuonFakeRate, h_MuonPromptRate, PMRP.PSIV.doVerbosity);
            if (PMRP.PSIV.doVerbosity) {
                cout << "BEI.weight pre DD " << BEI.weight << endl;
            }
            BEI.ScaleWeights(weightFakeLepDD, true, true);
            if (PMRP.PSIV.doVerbosity) {
                cout << "BEI.weight " << BEI.weight << endl;
            }
        }
        ///****************************************
        // Call the code to set up the event information
        ///****************************************
//        cout << "test 2 " << endl;
        
//        cout << "BEI.weight 1 " << BEI.weight << endl;
        if (!PMRP.SMV.doData) {
            if (!PMRP.SMV.doGenRecoil && !PMRP.PSIV.fInName.Contains("TT")) {
                EGW.vecGenWeight[0] = 1.0;
                EGW.vecGenWeight[1] = 1.0;
                EGW.vecGenWeight[2] = 1.0;
            }
            
            BEI_SystVarUp[systGenRecoilRW].SetVars(&BEI);
            BEI_SystVarUp[systGenRecoilRW].ScaleWeights(EGW.vecGenWeight[1], true, false); // i.e. no GenRecoilReweighting systematic.
            
            
            BEI_SystVarDown[systGenRecoilRW].SetVars(&BEI);
            
            
            //FOR NOW TRY 0 Weight for GenRecoilReweight
//            BEI_SystVarDown[systGenRecoilRW].ScaleWeights(EGW.vecGenWeight[0]); // i.e. twice GenRecoilReweighting systematic.
            BEI_SystVarDown[systGenRecoilRW].ScaleWeights(EGW.vecGenWeight[2], true, false); // i.e. twice GenRecoilReweighting systematic.
            
            BEI.ScaleWeights(EGW.vecGenWeight[0], true, false);
            
            for (int iSyst = 1; iSyst <= numSystsTotal; ++iSyst) {
                if (iSyst != systGenRecoilRW) {
                    BEI_SystVarUp[iSyst].SetVars(&BEI);
                    BEI_SystVarDown[iSyst].SetVars(&BEI);
                }
            }
        }        
//        cout << "test 3 " << endl;
////        SetEventInformation(&BEI, &ELI, &EJI, EMI, EDSI);
//        if (EMI.EventMT2ll < 180) continue;
//        PrintEvent(&BEI, &EMI, &EJI, &ELI, &EDSI, 180);
        ///****************************************
        // Call the code to set up the event information
        ///****************************************
        ELI.doEvent &= ELI.PassesAdditionalLepPtCut(1, PMRP.SMV.subLepPtCut); // subLep Pt Cut -- nominal default is 10 GeV so it does nothing        
        //hasMoreThan2Leps = ELI.HasMoreThanNLeps();
        if (PMRP.SMV.doData) {
            if (!ELI.doEvent) continue;
        }
        else {            
            if (PMRP.PSIV.doVerbosity) {
                cout << "Type " << ELI.EventDiLepType << endl;
                cout << "weight pre Lepton Scale factor " << BEI.weight << endl;
            }
            
            
                        //////
            CorrectThingsEMI(&EMI_PF, &EMI_PF_SystVarUp, &EMI_PF_SystVarDown);
            
            CorrectThingsEMI(&SmearEMI_PF, &SmearEMI_PF_SystVarUp, &SmearEMI_PF_SystVarDown);
            CorrectThingsEMI(&SmearEMI_PF_noType0, &SmearEMI_PF_noType0_SystVarUp, &SmearEMI_PF_noType0_SystVarDown);
            
            
            CorrectThingsEDSI(&EDSI, &EDSI_SystVarUp, &EDSI_SystVarDown);
            CorrectThingsEDSI(&SmearEDSI, &SmearEDSI_SystVarUp, &SmearEDSI_SystVarDown);

            CorrectThingsEJI(&EJI, &EJI_SystVarUp, &EJI_SystVarDown);
            CorrectThingsEJI(&SmearEJI, &SmearEJI_SystVarUp, &SmearEJI_SystVarDown, true);
            
            //////
                                                                                                          
            //hasMoreThan2Leps_LepESUp = ELI_SystVarUp[systLepES].HasMoreThanNLeps();
            //hasMoreThan2Leps_LepESDown = ELI_SystVarDown[systLepES].HasMoreThanNLeps();
//            cout << "test 4 " << endl;
            ELI_SystVarUp[systLepES].doEvent &= ELI_SystVarUp[systLepES].PassesAdditionalLepPtCut(1, PMRP.SMV.subLepPtCut);
//            cout << "test 4a " << endl;cond
            ELI_SystVarDown[systLepES].doEvent &= ELI_SystVarDown[systLepES].PassesAdditionalLepPtCut(1, PMRP.SMV.subLepPtCut);
//            cout << "test 4b " << endl;
            if (!ELI.doEvent && !ELI_SystVarUp[systLepES].doEvent && !ELI_SystVarDown[systLepES].doEvent) continue;
//            cout << "test 4c " << endl;
            if (ELI.doEvent) {
//                cout << "setting cent val scale facs " << endl;
                ELI.SetScaleFactors(&h_TrigSFVec,  h_LepIDIsoElecSFToUse, h_LepIDIsoMuonSFToUse, PMRP.PSIV.doVerbosity);
//                ELI.SetScaleFactorsOld(PMRP.SRS.isSignal, &h_TrigSFVec,  h_LepIDIsoElecSFToUse, h_LepIDIsoMuonSFToUse);
            }
            else {
                ELI.SetScaleFactorsFailedEvent();
            }
//            cout << "test 4d " << endl;
            if (ELI_SystVarUp[systLepES].doEvent) {
//                cout << "setting shift up scale facs " << endl;
                ELI_SystVarUp[systLepES].SetScaleFactors(&h_TrigSFVec, h_LepIDIsoElecSFToUse, h_LepIDIsoMuonSFToUse);
//                ELI_SystVarUp[systLepES].SetScaleFactorsOld(PMRP.SRS.isSignal, &h_TrigSFVec, h_LepIDIsoElecSFToUse, h_LepIDIsoMuonSFToUse);
            }
            else {
                ELI_SystVarUp[systLepES].SetScaleFactorsFailedEvent();
            }
//            cout << "test 4e " << endl;
            if (ELI_SystVarDown[systLepES].doEvent) {
//                cout << "setting shift down scale facs " << endl;
                ELI_SystVarDown[systLepES].SetScaleFactors(&h_TrigSFVec, h_LepIDIsoElecSFToUse, h_LepIDIsoMuonSFToUse);
//                ELI_SystVarDown[systLepES].SetScaleFactorsOld(PMRP.SRS.isSignal, &h_TrigSFVec, h_LepIDIsoElecSFToUse, h_LepIDIsoMuonSFToUse);
            }
            else {
//                cout << "test 4eb " << endl;
                ELI_SystVarDown[systLepES].SetScaleFactorsFailedEvent();
            }
//            cout << "test 4f " << endl;
            BEI.ScaleWeights(ELI.GetSF(), true, true);
            
            for (int iSyst = 1; iSyst <= numSystsTotal; ++iSyst) {
                if (iSyst == systLepES) {
                    BEI_SystVarUp[iSyst].ScaleWeights(ELI_SystVarUp[systLepES].GetSF(), true, true);
                    BEI_SystVarDown[iSyst].ScaleWeights(ELI_SystVarDown[systLepES].GetSF(), true, true);
                }
                else if (iSyst == systLepEff) {
                    BEI_SystVarUp[iSyst].ScaleWeights(ELI.GetSF(1), true, true);
                    BEI_SystVarDown[iSyst].ScaleWeights(ELI.GetSF(-1), true, true);
                }
                else {
                    BEI_SystVarUp[iSyst].ScaleWeights(ELI.GetSF(), true, true);
                    BEI_SystVarDown[iSyst].ScaleWeights(ELI.GetSF(), true, true);
                }
            }
            //            cout << "test 5 " << endl;
            if (PMRP.SRS.isSignal) {
                BEI.ScaleWeights(EGSPI.StopXSec, true, true);
                for (int iSyst = 1; iSyst <= numSystsTotal; ++iSyst) {
                    if (iSyst == systStopXSec) {
                        BEI_SystVarUp[iSyst].ScaleWeights(EGSPI.StopXSec + 1 * EGSPI.StopXSecErr, true, true);
                        BEI_SystVarDown[iSyst].ScaleWeights(EGSPI.StopXSec - 1 * EGSPI.StopXSecErr, true, true);
                    }
                    else {
                        BEI_SystVarUp[iSyst].ScaleWeights(EGSPI.StopXSec, true, true);
                        BEI_SystVarDown[iSyst].ScaleWeights(EGSPI.StopXSec, true, true);
                    }
                }
            }
        }
        // basic condition, if running on data, only select type of events that are relevant to prevent double counting
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
        /*
        stringKeyToVar["nVtx"]      = (float) EPI.nVtx;
        stringKeyToVar["nVtxTrue"]  = (float) EPI.nVtxTrue;
        */
        

//        PMRP.PSIV.doVerbosity = true;
        if (PMRP.PSIV.doVerbosity) {
            cout << "running on event " << ievt << endl;
            cout << "BEI.weight " << BEI.weight << endl;
        }
        
        
        
        
        
        
        mapIntBEI mapIntBEI_Basic;
        mapIntELI mapIntELI_Basic;
        mapIntEMI mapIntEMI_Basic;
        mapIntEJI mapIntEJI_Basic;
        mapIntEDSI mapIntEDSI_Basic;
        
        mapIntEMI mapIntEMI_OfficialSmear;
        mapIntEJI mapIntEJI_OfficialSmear;
        mapIntEDSI mapIntEDSI_OfficialSmear;
        
        mapIntEMI mapIntEMI_NoType0;
        mapIntEDSI mapIntEDSI_NoType0;
        
        mapIntEMI mapIntEMI_NoType0_ByHandSmear;
        mapIntEDSI mapIntEDSI_NoType0_ByHandSmear;                
        
        SetEMIMap(mapIntEMI_Basic, &EMI_PF, &EMI_PF_SystVarUp, &EMI_PF_SystVarDown, numSystsTotal, &mapIntBool_EMI);
//        cout << "test " << endl;
//        cout << "about to set BEI map " << endl;
        SetBEIMap(mapIntBEI_Basic, &BEI, &BEI_SystVarUp, &BEI_SystVarDown, numSystsTotal, &mapIntBool_BEI);
        SetEJIMap(mapIntEJI_Basic, &EJI, &EJI_SystVarUp, &EJI_SystVarDown, numSystsTotal, &mapIntBool_EJI);
        SetELIMap(mapIntELI_Basic, &ELI, &ELI_SystVarUp, &ELI_SystVarDown, numSystsTotal, &mapIntBool_ELI);
        SetEDSIMap(mapIntEDSI_Basic, &EDSI, &EDSI_SystVarUp, &EDSI_SystVarDown, numSystsTotal, &mapIntBool_EDSI);
        if (!PMRP.SMV.doData) {
            SetEMIMap(mapIntEMI_OfficialSmear,   &SmearEMI_PF, &SmearEMI_PF_SystVarUp, &SmearEMI_PF_SystVarDown, numSystsTotal, &mapIntBool_SmearEMI);
            SetEJIMap(mapIntEJI_OfficialSmear,   &SmearEJI,    &SmearEJI_SystVarUp,    &SmearEJI_SystVarDown,    numSystsTotal, &mapIntBool_SmearEJI);
            SetEDSIMap(mapIntEDSI_OfficialSmear, &SmearEDSI,   &SmearEDSI_SystVarUp,   &SmearEDSI_SystVarDown,   numSystsTotal, &mapIntBool_SmearEDSI);
        }
        
        SetEMIMap(mapIntEMI_NoType0, &EMI_PF_noType0, &EMI_PF_SystVarUp, &EMI_PF_SystVarDown, numSystsTotal, &mapIntBool_EMI);        
        //        SetEDSIMap(mapIntEDSI_NoType0, &EDSI,   &EDSI_SystVarUp,   &EDSI_SystVarDown,   numSystsTotal, &mapIntBool_EDSI);
        
        if (!PMRP.SMV.doData) {
            SetEMIMap(mapIntEMI_NoType0_ByHandSmear, &SmearEMI_PF_noType0, &SmearEMI_PF_noType0_SystVarUp, &SmearEMI_PF_noType0_SystVarDown, numSystsTotal, &mapIntBool_SmearEMI);
        }
        //        SetEDSIMap(mapIntEDSI_NoType0_ByHandSmear, &SmearEDSI,   &SmearEDSI_SystVarUp,   &SmearEDSI_SystVarDown,   numSystsTotal, &mapIntBool_SmearEDSI);                
        
        ESPI.SetStructs(0, &mapIntBEI_Basic, &mapIntEMI_Basic, &mapIntEJI_Basic, &mapIntELI_Basic, &mapIntEDSI_Basic);
        if (!PMRP.SMV.doData && PMRP.PSIV.fInName) {
            for (int iSyst = 1; iSyst <= numSystsTotal; ++iSyst) {
                ESPI_SystVarUp[iSyst].SetStructs(iSyst, &mapIntBEI_Basic, &mapIntEMI_Basic, &mapIntEJI_Basic, &mapIntELI_Basic, &mapIntEDSI_Basic);
                ESPI_SystVarDown[iSyst].SetStructs(-1 * iSyst, &mapIntBEI_Basic, &mapIntEMI_Basic, &mapIntEJI_Basic, &mapIntELI_Basic, &mapIntEDSI_Basic);
            }
        }
        if (!PMRP.SMV.doData) {            
//            SmearESPI.SetStructs(0, &mapIntBEI_Basic, &mapIntEMI_OfficialSmear, &mapIntEJI_OfficialSmear, &mapIntELI_Basic, &mapIntEDSI_OfficialSmear);
            SmearESPI.SetStructs(0, &mapIntBEI_Basic, &mapIntEMI_OfficialSmear, &mapIntEJI_OfficialSmear, &mapIntELI_Basic, &mapIntEDSI_OfficialSmear);
            if (PMRP.PSIV.fInName) {
                for (int iSyst = 1; iSyst <= numSystsTotal; ++iSyst) {
//                    SmearESPI_SystVarUp[iSyst].SetStructs(iSyst, &mapIntBEI_Basic, &mapIntEMI_OfficialSmear, &mapIntEJI_OfficialSmear, &mapIntELI_Basic, &mapIntEDSI_OfficialSmear);
//                    SmearESPI_SystVarDown[iSyst].SetStructs(-1 * iSyst, &mapIntBEI_Basic, &mapIntEMI_OfficialSmear, &mapIntEJI_OfficialSmear, &mapIntELI_Basic, &mapIntEDSI_OfficialSmear);
                    SmearESPI_SystVarUp[iSyst].SetStructs(iSyst, &mapIntBEI_Basic, &mapIntEMI_OfficialSmear, &mapIntEJI_OfficialSmear, &mapIntELI_Basic, &mapIntEDSI_OfficialSmear);
                    SmearESPI_SystVarDown[iSyst].SetStructs(-1 * iSyst, &mapIntBEI_Basic, &mapIntEMI_OfficialSmear, &mapIntEJI_OfficialSmear, &mapIntELI_Basic, &mapIntEDSI_OfficialSmear);
                }
            }
        }
//        cout << "about to set NoType0 map stuff " << endl;
//        ESPI_NoType0.SetStructs(0, &mapIntBEI_Basic, &mapIntEMI_NoType0, &mapIntEJI_Basic, &mapIntELI_Basic, &mapIntEDSI_NoType0, true);        
        ESPI_NoType0.SetStructs(0, &mapIntBEI_Basic, &mapIntEMI_NoType0, &mapIntEJI_Basic, &mapIntELI_Basic, &mapIntEDSI_Basic);
        if (!PMRP.SMV.doData) { 
//            cout << "about to set NoType0 smeared map stuff " << endl;
//            SmearESPI_NoType0.SetStructs(0, &mapIntBEI_Basic, &mapIntEMI_NoType0_ByHandSmear, &mapIntEJI_OfficialSmear, &mapIntELI_Basic, &mapIntEDSI_OfficialSmear);
            SmearESPI_NoType0.SetStructs(0, &mapIntBEI_Basic, &mapIntEMI_NoType0_ByHandSmear, &mapIntEJI_Basic, &mapIntELI_Basic, &mapIntEDSI_Basic);
            if (PMRP.PSIV.fInName) { // what the hell is this for?
                for (int iSyst = 1; iSyst <= numSystsTotal; ++iSyst) {
//                    SmearESPI_NoType0_SystVarUp[iSyst].SetStructs(iSyst, &mapIntBEI_Basic, &mapIntEMI_NoType0_ByHandSmear, &mapIntEJI_OfficialSmear, &mapIntELI_Basic, &mapIntEDSI_OfficialSmear);
//                    SmearESPI_NoType0_SystVarDown[iSyst].SetStructs(-1 * iSyst, &mapIntBEI_Basic, &mapIntEMI_NoType0_ByHandSmear, &mapIntEJI_OfficialSmear, &mapIntELI_Basic, &mapIntEDSI_OfficialSmear);
                    SmearESPI_NoType0_SystVarUp[iSyst].SetStructs(iSyst, &mapIntBEI_Basic, &mapIntEMI_NoType0_ByHandSmear, &mapIntEJI_Basic, &mapIntELI_Basic, &mapIntEDSI_Basic);
                    SmearESPI_NoType0_SystVarDown[iSyst].SetStructs(-1 * iSyst, &mapIntBEI_Basic, &mapIntEMI_NoType0_ByHandSmear, &mapIntEJI_Basic, &mapIntELI_Basic, &mapIntEDSI_Basic);
                }
            }
        }

        /*
        for (unsigned int iSyst = 0; iSyst <= numKinSysts; ++iSyst) {
            cout << "for iSyst = " << iSyst << ", DphiZMET up " << EDSI_SystVarUp[iSyst].DP_ZMET.DPhiInputObjects << endl; 
            cout << "for iSyst = " << iSyst << ", DphiZMET " << EDSI_SystVarDown[iSyst].DP_ZMET.DPhiInputObjects << endl; 
        }
        */
        
        
        
        /// Clear the Maps
        
        StVM_Basic.clear();
        StVM_MET_noType0.clear();
        
        if (!PMRP.SMV.doData) {
            StVM_Smear.clear();
            StVM_Smear_MET_noType0.clear();
            if (PMRP.PSIV.fInName) {
                for (int iSyst = 1; iSyst <= numSystsTotal; ++iSyst) {
                    StVM_Basic_SystVarUp[iSyst].clear();
                    StVM_Basic_SystVarDown[iSyst].clear();
                    
                    StVM_Smear_SystVarUp[iSyst].clear();
                    StVM_Smear_SystVarDown[iSyst].clear();
                    
                    StVM_Smear_MET_noType0_SystVarUp[iSyst].clear();
                    StVM_Smear_MET_noType0_SystVarDown[iSyst].clear();
                }
            }
        }
        
        /******************************************************************************************************************************/        
        /******************************************************************************************************************************/
        ///////////////////////// BEGIN: Fill the Histograms
        /******************************************************************************************************************************/
        /******************************************************************************************************************************/
        
        ///// Basic histograms /////
        if (PMRP.PSIV.doVerbosity_Plots) {
            cout << "about to do normal plots " << endl;
            cout << "note which dilep type is " << PMRP.SMV.whichDiLepType << endl;
        }
        SetupMapsAndFillHistograms(StVM_Basic, subSampBool, subSampVec, &ESPI, &EPI, &vecVecHistT_Inclusive, &histMap_1D, &histMap_2D, &histMap_3D, METSig_Raw, systStopBook, 0, false, PMRP.SMV.whichDiLepType, PMRP.PSIV.doVerbosity_Plots);
        
        if (PMRP.PSIV.doVerbosity_Plots) {
            cout << "about to do noType0 plots " << endl;
        }
        ///// Basic histograms for MET w/0 Type0 /////
        SetupMapsAndFillHistograms(StVM_MET_noType0, subSampBool_noType0, subSampVec, &ESPI_NoType0, &EPI, &vecVecHistT_MET_noType0, &histMap_1D_METnoType0, &histMap_2D_METnoType0, &histMap_3D_METnoType0, METSig_Raw, systStopBook, 0, false, PMRP.SMV.whichDiLepType, PMRP.PSIV.doVerbosity_Plots); 
                        
        if (!PMRP.SMV.doData) {
            if (PMRP.PSIV.doVerbosity_Plots) {
                cout << "about to do Smeared plots " << endl;
            }
            ///// Histograms for smeared Jets/MET  /////            
            SetupMapsAndFillHistograms(StVM_Smear, subSampBool_Smear, subSampVec, &SmearESPI, &EPI, &vecVecHistT_Inclusive_Smear, &histMap_1D_Smear, &histMap_2D_Smear, &histMap_3D_Smear, METSig_Raw, systStopBook, 0, true, PMRP.SMV.whichDiLepType, PMRP.PSIV.doVerbosity_Plots); 
            
            ///// Histograms for smeared MET w/o Type 0  /////            
            SetupMapsAndFillHistograms(StVM_Smear_MET_noType0, subSampBool_Smear_noType0, subSampVec, &SmearESPI_NoType0, &EPI, &vecVecHistT_MET_noType0_Smear, &histMap_1D_SmearMETnoType0, &histMap_2D_SmearMETnoType0, &histMap_3D_SmearMETnoType0, METSig_Raw, systStopBook, 0, true, PMRP.SMV.whichDiLepType, PMRP.PSIV.doVerbosity_Plots);                                     
            if (PMRP.PSIV.fInName) {                
                ///// Systematic variations on histograms /////
                for (int iSyst = 1; iSyst <= numSystsTotal; ++iSyst) {                    
                    ///// Systematic variations on basic histograms /////
                    
                    if (iSyst != systUncES && iSyst != systJetSmear) {
                        /// Systematic Variations Up
                        SetupMapsAndFillHistograms(StVM_Basic_SystVarUp[iSyst], subSampBool_SystVarUp[iSyst], subSampVec, &ESPI_SystVarUp[iSyst], &EPI, &vecVecHistT_Inclusive_Syst, &histMap_1D_Syst, &histMap_2D_Syst, &histMap_3D_Syst, METSig_Raw, systStopBook, iSyst, false, PMRP.SMV.whichDiLepType, PMRP.PSIV.doVerbosity_Plots); 
                        
                        /// Systematic Variations Down
                        SetupMapsAndFillHistograms(StVM_Basic_SystVarDown[iSyst], subSampBool_SystVarDown[iSyst], subSampVec, &ESPI_SystVarDown[iSyst], &EPI, &vecVecHistT_Inclusive_Syst, &histMap_1D_Syst, &histMap_2D_Syst, &histMap_3D_Syst, METSig_Raw, systStopBook, -1 * iSyst, false, PMRP.SMV.whichDiLepType, PMRP.PSIV.doVerbosity_Plots);                        
                    }
                    
                    ///// Systematic variations on smeared histograms /////
                    
                    /// Systematic Variations Up
                    SetupMapsAndFillHistograms(StVM_Smear_SystVarUp[iSyst], subSampBool_Smear_SystVarUp[iSyst], subSampVec, &SmearESPI_SystVarUp[iSyst], &EPI, &vecVecHistT_Inclusive_Smear_Syst, &histMap_1D_Smear_Syst, &histMap_2D_Smear_Syst, &histMap_3D_Smear_Syst, METSig_Raw, systStopBook, iSyst, true, PMRP.SMV.whichDiLepType, PMRP.PSIV.doVerbosity_Plots); 
                    
                    /// Systematic Variations Down
                    SetupMapsAndFillHistograms(StVM_Smear_SystVarDown[iSyst], subSampBool_Smear_SystVarDown[iSyst], subSampVec, &SmearESPI_SystVarDown[iSyst], &EPI, &vecVecHistT_Inclusive_Smear_Syst, &histMap_1D_Smear_Syst, &histMap_2D_Smear_Syst, &histMap_3D_Smear_Syst, METSig_Raw, systStopBook, -1 * iSyst, true, PMRP.SMV.whichDiLepType, PMRP.PSIV.doVerbosity_Plots);
                    
                    
                    
                    ///// Systematic variations on histograms for MET w/o Type0 /////
                    /*
                    if (iSyst != systUncES && iSyst != systJetSmear) {
                        /// Systematic Variations Up
                        SetupMapsAndFillHistograms(StVM_Smear_MET_noType0_SystVarUp[iSyst], subSampBool_Smear_noType0_SystVarUp[iSyst], subSampVec, &SmearESPI_NoType0_SystVarUp[iSyst], &vecVecHistT_MET_noType0_Smear_Syst, &histMap_1D_SmearMETnoType0_Syst, &histMap_1D_SmearMETnoType0_Syst, &histMap_1D_SmearMETnoType0_Syst, METSig_Raw, iSyst, PMRP.PSIV.doVerbosity_Plots); 
                        
                        /// Systematic Variations Down
                        ddd_SetupMapsAndFillHistograms(StVM_Smear_MET_noType0_SystVarDown[iSyst], subSampBool_Smear_noType0_SystVarDown[iSyst], subSampVec, &SmearESPI_NoType0_SystVarDown[iSyst], &vecVecHistT_MET_noType0_Smear_Syst, &histMap_1D_SmearMETnoType0_Syst, &histMap_1D_SmearMETnoType0_Syst, &histMap_1D_SmearMETnoType0_Syst, METSig_Raw, -1 * iSyst, PMRP.PSIV.doVerbosity_Plots);                        
                    }
                    */
                    ///// Systematic variations on histograms for smeared EMT w/o Type0 /////
                    
                    /// Systematic Variations Up
                    SetupMapsAndFillHistograms(StVM_Smear_MET_noType0_SystVarUp[iSyst], subSampBool_Smear_noType0_SystVarUp[iSyst], subSampVec, &SmearESPI_NoType0_SystVarUp[iSyst], &EPI, &vecVecHistT_MET_noType0_Smear_Syst, &histMap_1D_SmearMETnoType0_Syst, &histMap_2D_SmearMETnoType0_Syst, &histMap_3D_SmearMETnoType0_Syst, METSig_Raw, systStopBook, iSyst, true, PMRP.SMV.whichDiLepType, PMRP.PSIV.doVerbosity_Plots); 
                    
                    /// Systematic Variations Down
                    SetupMapsAndFillHistograms(StVM_Smear_MET_noType0_SystVarDown[iSyst], subSampBool_Smear_noType0_SystVarDown[iSyst], subSampVec, &SmearESPI_NoType0_SystVarDown[iSyst], &EPI, &vecVecHistT_MET_noType0_Smear_Syst, &histMap_1D_SmearMETnoType0_Syst, &histMap_2D_SmearMETnoType0_Syst, &histMap_3D_SmearMETnoType0_Syst, METSig_Raw, systStopBook, -1 * iSyst, true, PMRP.SMV.whichDiLepType, PMRP.PSIV.doVerbosity_Plots);
                    
                }
            }
        }
        
        /******************************************************************************************************************************/        
        /******************************************************************************************************************************/
        ///////////////////////// END: Fill the Histograms
        /******************************************************************************************************************************/
        /******************************************************************************************************************************/
    }
    cout << "All events done" << endl;
    outputFile->cd();
    cout << "cd-ing to output directory" << endl;
    TH1F * h_numParFiles = new TH1F("h_numParFiles", "", 2, -0.5, 1.5);
    h_numParFiles->Fill(1);
    h_numParFiles->SetEntries(1);
    h_numParFiles->Write();
    h_eventCount->Write();
    h_CutFlow->Write();
    /*
    if (PMRP.PSIV.fInName.Contains("TT") || PMRP.PSIV.fInName.Contains("ttbarsignal")) {
        h_genTopPt->Write();
        h_genTopPtRW->Write();
        h_genAntiTopPt->Write();
        h_genAntiTopPtRW->Write();
    }
    */
    outputFile->Write();
    cout << "Writing of output file done" << endl;
    outputFile->Close();
    cout << "end of code" << endl;
}
StopXSec getCrossSectionStop(float stopMass){
    
    // taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections8TeVstopsbottom                                                                  
    // values are in pb                                                                                                                                             
    
    float xsecStopPairProd=0., xsecUncert=0.;
    
    if(stopMass==100) {xsecStopPairProd = 559.757;       xsecUncert = 16.1085/100.;}
    if(stopMass==105) {xsecStopPairProd = 448.456;       xsecUncert = 15.9732/100.;}
    if(stopMass==110) {xsecStopPairProd = 361.917;       xsecUncert = 16.1134/100.;}
    if(stopMass==115) {xsecStopPairProd = 293.281;       xsecUncert = 15.9763/100.;}
    if(stopMass==120) {xsecStopPairProd = 240.077;       xsecUncert = 15.9212/100.;}
    if(stopMass==125) {xsecStopPairProd = 197.122;       xsecUncert = 15.7303/100.;}
    if(stopMass==130) {xsecStopPairProd = 163.376;       xsecUncert = 15.8101/100.;}
    if(stopMass==135) {xsecStopPairProd = 135.791;       xsecUncert = 15.8086/100.;}
    if(stopMass==140) {xsecStopPairProd = 113.319;       xsecUncert = 15.7234/100.;}
    if(stopMass==145) {xsecStopPairProd =  95.0292;      xsecUncert = 15.649/100.;}
    if(stopMass==150) {xsecStopPairProd =  80.268;       xsecUncert = 15.5946/100.;}
    if(stopMass==155) {xsecStopPairProd =  68.0456;      xsecUncert = 15.5232/100.;}
    if(stopMass==160) {xsecStopPairProd =  58.01;        xsecUncert = 15.3899/100.;}
    if(stopMass==165) {xsecStopPairProd =  49.6639;      xsecUncert = 15.3711/100.;}
    if(stopMass==170) {xsecStopPairProd =  42.6441;      xsecUncert = 15.3017/100.;}
    if(stopMass==175) {xsecStopPairProd =  36.7994;      xsecUncert = 15.1749/100.;}
    if(stopMass==180) {xsecStopPairProd =  31.8695;      xsecUncert = 15.2449/100.;}
    if(stopMass==185) {xsecStopPairProd =  27.7028;      xsecUncert = 15.063/100.;}
    if(stopMass==190) {xsecStopPairProd =  24.1585;      xsecUncert = 15.16/100.;}
    if(stopMass==195) {xsecStopPairProd =  21.1597;      xsecUncert = 14.9422/100.;}
    if(stopMass==200) {xsecStopPairProd =  18.5245;      xsecUncert = 14.9147/100.;}
    if(stopMass==205) {xsecStopPairProd =  16.2439;      xsecUncert = 15.117/100.;}
    if(stopMass==210) {xsecStopPairProd =  14.3201;      xsecUncert = 14.8495/100.;}
    if(stopMass==215) {xsecStopPairProd =  12.6497;      xsecUncert = 14.8689/100.;}
    if(stopMass==220) {xsecStopPairProd =  11.1808;      xsecUncert = 14.9108/100.;}
    if(stopMass==225) {xsecStopPairProd =   9.90959;     xsecUncert = 14.9662/100.;}
    if(stopMass==230) {xsecStopPairProd =   8.78125;     xsecUncert = 14.796/100.;}
    if(stopMass==235) {xsecStopPairProd =   7.81646;     xsecUncert = 14.7983/100.;}
    if(stopMass==240) {xsecStopPairProd =   6.96892;     xsecUncert = 14.7878/100.;}
    if(stopMass==245) {xsecStopPairProd =   6.22701;     xsecUncert = 14.7897/100.;}
    if(stopMass==250) {xsecStopPairProd =   5.57596;     xsecUncert = 14.7529/100.;}
    if(stopMass==255) {xsecStopPairProd =   5.00108;     xsecUncert = 14.729/100.;}
    if(stopMass==260) {xsecStopPairProd =   4.48773;     xsecUncert = 14.6782/100.;}
    if(stopMass==265) {xsecStopPairProd =   4.03416;     xsecUncert = 14.7964/100.;}
    if(stopMass==270) {xsecStopPairProd =   3.63085;     xsecUncert = 14.6565/100.;}
    if(stopMass==275) {xsecStopPairProd =   3.2781;      xsecUncert = 14.7341/100.;}
    if(stopMass==280) {xsecStopPairProd =   2.95613;     xsecUncert = 14.7816/100.;}
    if(stopMass==285) {xsecStopPairProd =   2.67442;     xsecUncert = 14.7661/100.;}
    if(stopMass==290) {xsecStopPairProd =   2.42299;     xsecUncert = 14.6805/100.;}
    if(stopMass==295) {xsecStopPairProd =   2.19684;     xsecUncert = 14.8465/100.;}
    if(stopMass==300) {xsecStopPairProd =   1.99608;     xsecUncert = 14.6905/100.;}
    if(stopMass==305) {xsecStopPairProd =   1.81486;     xsecUncert = 14.4434/100.;}
    if(stopMass==310) {xsecStopPairProd =   1.64956;     xsecUncert = 14.4769/100.;}
    if(stopMass==315) {xsecStopPairProd =   1.50385;     xsecUncert = 14.4549/100.;}
    if(stopMass==320) {xsecStopPairProd =   1.3733;      xsecUncert = 14.7503/100.;}
    if(stopMass==325) {xsecStopPairProd =   1.25277;     xsecUncert = 14.2875/100.;}
    if(stopMass==330) {xsecStopPairProd =   1.14277;     xsecUncert = 14.578/100.;}
    if(stopMass==335) {xsecStopPairProd =   1.04713;     xsecUncert = 14.3659/100.;}
    if(stopMass==340) {xsecStopPairProd =   0.959617;    xsecUncert = 14.3896/100.;}
    if(stopMass==345) {xsecStopPairProd =   0.879793;    xsecUncert = 14.3881/100.;}
    if(stopMass==350) {xsecStopPairProd =   0.807323;    xsecUncert = 14.3597/100.;}
    if(stopMass==355) {xsecStopPairProd =   0.74141;     xsecUncert = 14.368/100.;}
    if(stopMass==360) {xsecStopPairProd =   0.681346;    xsecUncert = 14.3357/100.;}
    if(stopMass==365) {xsecStopPairProd =   0.626913;    xsecUncert = 14.3627/100.;}
    if(stopMass==370) {xsecStopPairProd =   0.576882;    xsecUncert = 14.2712/100.;}
    if(stopMass==375) {xsecStopPairProd =   0.531443;    xsecUncert = 14.266/100.;}
    if(stopMass==380) {xsecStopPairProd =   0.489973;    xsecUncert = 14.3962/100.;}
    if(stopMass==385) {xsecStopPairProd =   0.452072;    xsecUncert = 14.2234/100.;}
    if(stopMass==390) {xsecStopPairProd =   0.4176;      xsecUncert = 14.3166/100.;}
    if(stopMass==395) {xsecStopPairProd =   0.385775;    xsecUncert = 14.3112/100.;}
    if(stopMass==400) {xsecStopPairProd =   0.35683;     xsecUncert = 14.2848/100.;}
    if(stopMass==405) {xsecStopPairProd =   0.329881;    xsecUncert = 14.2072/100.;}
    if(stopMass==410) {xsecStopPairProd =   0.305512;    xsecUncert = 14.2648/100.;}
    if(stopMass==415) {xsecStopPairProd =   0.283519;    xsecUncert = 14.102/100.;}
    if(stopMass==420) {xsecStopPairProd =   0.262683;    xsecUncert = 14.3075/100.;}
    if(stopMass==425) {xsecStopPairProd =   0.243755;    xsecUncert = 14.0504/100.;}
    if(stopMass==430) {xsecStopPairProd =   0.226367;    xsecUncert = 14.0494/100.;}
    if(stopMass==435) {xsecStopPairProd =   0.209966;    xsecUncert = 14.0334/100.;}
    if(stopMass==440) {xsecStopPairProd =   0.195812;    xsecUncert = 14.0772/100.;}
    if(stopMass==445) {xsecStopPairProd =   0.181783;    xsecUncert = 14.1771/100.;}
    if(stopMass==450) {xsecStopPairProd =   0.169668;    xsecUncert = 14.2368/100.;}
    if(stopMass==455) {xsecStopPairProd =   0.158567;    xsecUncert = 14.2609/100.;}
    if(stopMass==460) {xsecStopPairProd =   0.147492;    xsecUncert = 14.4105/100.;}
    if(stopMass==465) {xsecStopPairProd =   0.137392;    xsecUncert = 14.4772/100.;}
    if(stopMass==470) {xsecStopPairProd =   0.128326;    xsecUncert = 14.5144/100.;}
    if(stopMass==475) {xsecStopPairProd =   0.119275;    xsecUncert = 14.6664/100.;}
    if(stopMass==480) {xsecStopPairProd =   0.112241;    xsecUncert = 14.6307/100.;}
    if(stopMass==485) {xsecStopPairProd =   0.104155;    xsecUncert = 14.7581/100.;}
    if(stopMass==490) {xsecStopPairProd =   0.0977878;   xsecUncert = 14.7977/100.;}
    if(stopMass==495) {xsecStopPairProd =   0.091451;    xsecUncert = 14.8963/100.;}
    if(stopMass==500) {xsecStopPairProd =   0.0855847;   xsecUncert = 14.9611/100.;}
    if(stopMass==505) {xsecStopPairProd =   0.0801322;   xsecUncert = 15.0389/100.;}
    if(stopMass==510) {xsecStopPairProd =   0.0751004;   xsecUncert = 15.1402/100.;}
    if(stopMass==515) {xsecStopPairProd =   0.0703432;   xsecUncert = 15.2139/100.;}
    if(stopMass==520) {xsecStopPairProd =   0.0660189;   xsecUncert = 15.3368/100.;}
    if(stopMass==525) {xsecStopPairProd =   0.0618641;   xsecUncert = 15.4135/100.;}
    if(stopMass==530) {xsecStopPairProd =   0.0580348;   xsecUncert = 15.4422/100.;}
    if(stopMass==535) {xsecStopPairProd =   0.0545113;   xsecUncert = 15.5446/100.;}
    if(stopMass==540) {xsecStopPairProd =   0.0511747;   xsecUncert = 15.6283/100.;}
    if(stopMass==545) {xsecStopPairProd =   0.0481537;   xsecUncert = 15.726/100.;}
    if(stopMass==550) {xsecStopPairProd =   0.0452067;   xsecUncert = 15.8177/100.;}
    if(stopMass==555) {xsecStopPairProd =   0.0424781;   xsecUncert = 15.9022/100.;}
    if(stopMass==560) {xsecStopPairProd =   0.0399591;   xsecUncert = 16.0067/100.;}
    if(stopMass==565) {xsecStopPairProd =   0.0376398;   xsecUncert = 16.0367/100.;}
    if(stopMass==570) {xsecStopPairProd =   0.0354242;   xsecUncert = 16.137/100.;}
    if(stopMass==575) {xsecStopPairProd =   0.0333988;   xsecUncert = 16.2132/100.;}
    if(stopMass==580) {xsecStopPairProd =   0.0313654;   xsecUncert = 16.3135/100.;}
    if(stopMass==585) {xsecStopPairProd =   0.0295471;   xsecUncert = 16.4264/100.;}
    if(stopMass==590) {xsecStopPairProd =   0.0279395;   xsecUncert = 16.4546/100.;}
    if(stopMass==595) {xsecStopPairProd =   0.0263263;   xsecUncert = 16.567/100.;}
    if(stopMass==600) {xsecStopPairProd =   0.0248009;   xsecUncert = 16.6406/100.;}
    if(stopMass==605) {xsecStopPairProd =   0.0233806;   xsecUncert = 16.7295/100.;}
    if(stopMass==610) {xsecStopPairProd =   0.0220672;   xsecUncert = 16.8447/100.;}
    if(stopMass==615) {xsecStopPairProd =   0.0208461;   xsecUncert = 16.9276/100.;}
    if(stopMass==620) {xsecStopPairProd =   0.0196331;   xsecUncert = 17.0459/100.;}
    if(stopMass==625) {xsecStopPairProd =   0.0185257;   xsecUncert = 17.0835/100.;}
    if(stopMass==630) {xsecStopPairProd =   0.0175075;   xsecUncert = 17.1478/100.;}
    if(stopMass==635) {xsecStopPairProd =   0.0164955;   xsecUncert = 17.2753/100.;}
    if(stopMass==640) {xsecStopPairProd =   0.0155809;   xsecUncert = 17.3814/100.;}
    if(stopMass==645) {xsecStopPairProd =   0.0147721;   xsecUncert = 17.4885/100.;}
    if(stopMass==650) {xsecStopPairProd =   0.0139566;   xsecUncert = 17.56/100.;}
    if(stopMass==655) {xsecStopPairProd =   0.0132456;   xsecUncert = 17.6129/100.;}
    if(stopMass==660) {xsecStopPairProd =   0.0125393;   xsecUncert = 17.7363/100.;}
    if(stopMass==665) {xsecStopPairProd =   0.0118287;   xsecUncert = 17.7959/100.;}
    if(stopMass==670) {xsecStopPairProd =   0.0112223;   xsecUncert = 17.8974/100.;}
    if(stopMass==675) {xsecStopPairProd =   0.0106123;   xsecUncert = 17.9891/100.;}
    if(stopMass==680) {xsecStopPairProd =   0.0100516;   xsecUncert = 18.0618/100.;}
    if(stopMass==685) {xsecStopPairProd =   0.0095256;   xsecUncert = 18.1714/100.;}
    if(stopMass==690) {xsecStopPairProd =   0.0090306;   xsecUncert = 18.2108/100.;}
    if(stopMass==695) {xsecStopPairProd =   0.00856339;  xsecUncert = 18.3365/100.;}
    if(stopMass==700) {xsecStopPairProd =   0.0081141;   xsecUncert = 18.4146/100.;}
    if(stopMass==705) {xsecStopPairProd =   0.00769525;  xsecUncert = 18.4937/100.;}
    if(stopMass==710) {xsecStopPairProd =   0.00730084;  xsecUncert = 18.6195/100.;}
    if(stopMass==715) {xsecStopPairProd =   0.00692243;  xsecUncert = 18.7005/100.;}
    if(stopMass==720) {xsecStopPairProd =   0.00656729;  xsecUncert = 18.819/100.;}
    if(stopMass==725) {xsecStopPairProd =   0.00623244;  xsecUncert = 18.8796/100.;}
    if(stopMass==730) {xsecStopPairProd =   0.00591771;  xsecUncert = 18.996/100.;}
    if(stopMass==735) {xsecStopPairProd =   0.00561049;  xsecUncert = 19.0787/100.;}
    if(stopMass==740) {xsecStopPairProd =   0.00532605;  xsecUncert = 19.1995/100.;}
    if(stopMass==745) {xsecStopPairProd =   0.00506044;  xsecUncert = 19.2916/100.;}
    if(stopMass==750) {xsecStopPairProd =   0.00480639;  xsecUncert = 19.4088/100.;}
    if(stopMass==755) {xsecStopPairProd =   0.00455979;  xsecUncert = 19.508/100.;}
    if(stopMass==760) {xsecStopPairProd =   0.00433688;  xsecUncert = 19.632/100.;}
    if(stopMass==765) {xsecStopPairProd =   0.00412174;  xsecUncert = 19.7141/100.;}
    if(stopMass==770) {xsecStopPairProd =   0.00391839;  xsecUncert = 19.8299/100.;}
    if(stopMass==775) {xsecStopPairProd =   0.00372717;  xsecUncert = 19.9097/100.;}
    if(stopMass==780) {xsecStopPairProd =   0.00354211;  xsecUncert = 20.0016/100.;}
    if(stopMass==785) {xsecStopPairProd =   0.00336904;  xsecUncert = 20.123/100.;}
    if(stopMass==790) {xsecStopPairProd =   0.00320476;  xsecUncert = 20.2271/100.;}
    if(stopMass==795) {xsecStopPairProd =   0.00304935;  xsecUncert = 20.4479/100.;}
    if(stopMass==800) {xsecStopPairProd =   0.00289588;  xsecUncert = 20.516/100.;}
    if(stopMass==805) {xsecStopPairProd =   0.00275424;  xsecUncert = 20.5444/100.;}
    if(stopMass==810) {xsecStopPairProd =   0.0026184;   xsecUncert = 20.8204/100.;}
    if(stopMass==815) {xsecStopPairProd =   0.00249291;  xsecUncert = 21.0063/100.;}
    if(stopMass==820) {xsecStopPairProd =   0.00237168;  xsecUncert = 21.0865/100.;}
    if(stopMass==825) {xsecStopPairProd =   0.00226163;  xsecUncert = 21.0511/100.;}
    if(stopMass==830) {xsecStopPairProd =   0.00214607;  xsecUncert = 21.3705/100.;}
    if(stopMass==835) {xsecStopPairProd =   0.00204589;  xsecUncert = 21.3026/100.;}
    if(stopMass==840) {xsecStopPairProd =   0.00195172;  xsecUncert = 21.6053/100.;}
    if(stopMass==845) {xsecStopPairProd =   0.0018573;   xsecUncert = 21.8177/100.;}
    if(stopMass==850) {xsecStopPairProd =   0.00176742;  xsecUncert = 21.836/100.;}
    if(stopMass==855) {xsecStopPairProd =   0.00168383;  xsecUncert = 22.1411/100.;}
    if(stopMass==860) {xsecStopPairProd =   0.00160403;  xsecUncert = 22.0506/100.;}
    if(stopMass==865) {xsecStopPairProd =   0.00153063;  xsecUncert = 22.3461/100.;}
    if(stopMass==870) {xsecStopPairProd =   0.00145772;  xsecUncert = 22.6206/100.;}
    if(stopMass==875) {xsecStopPairProd =   0.0013878;   xsecUncert = 22.5422/100.;}
    if(stopMass==880) {xsecStopPairProd =   0.00132077;  xsecUncert = 23.2161/100.;}
    if(stopMass==885) {xsecStopPairProd =   0.00126234;  xsecUncert = 23.1283/100.;}
    if(stopMass==890) {xsecStopPairProd =   0.00120568;  xsecUncert = 23.8404/100.;}
    if(stopMass==895) {xsecStopPairProd =   0.00114627;  xsecUncert = 23.7327/100.;}
    if(stopMass==900) {xsecStopPairProd =   0.00109501;  xsecUncert = 23.9439/100.;}
    if(stopMass==905) {xsecStopPairProd =   0.001044;    xsecUncert = 24.1049/100.;}
    if(stopMass==910) {xsecStopPairProd =   0.000996193; xsecUncert = 24.2789/100.;}
    if(stopMass==915) {xsecStopPairProd =   0.00095071;  xsecUncert = 24.5443/100.;}
    if(stopMass==920) {xsecStopPairProd =   0.000907494; xsecUncert = 24.7597/100.;}
    if(stopMass==925) {xsecStopPairProd =   0.000866391; xsecUncert = 24.877/100.;}
    if(stopMass==930) {xsecStopPairProd =   0.000826533; xsecUncert = 25.0849/100.;}
    if(stopMass==935) {xsecStopPairProd =   0.000789573; xsecUncert = 25.2885/100.;}
    if(stopMass==940) {xsecStopPairProd =   0.000753768; xsecUncert = 25.4768/100.;}
    if(stopMass==945) {xsecStopPairProd =   0.000719675; xsecUncert = 25.6582/100.;}
    if(stopMass==950) {xsecStopPairProd =   0.000687022; xsecUncert = 25.8341/100.;}
    if(stopMass==955) {xsecStopPairProd =   0.000656279; xsecUncert = 26.0372/100.;}
    if(stopMass==960) {xsecStopPairProd =   0.000626876; xsecUncert = 26.2059/100.;}
    if(stopMass==965) {xsecStopPairProd =   0.000598955; xsecUncert = 26.3653/100.;}
    if(stopMass==970) {xsecStopPairProd =   0.000571551; xsecUncert = 26.5169/100.;}
    if(stopMass==975) {xsecStopPairProd =   0.000546728; xsecUncert = 26.7985/100.;}
    if(stopMass==980) {xsecStopPairProd =   0.000522495; xsecUncert = 26.9218/100.;}
    if(stopMass==985) {xsecStopPairProd =   0.000499017; xsecUncert = 27.1036/100.;}
    if(stopMass==990) {xsecStopPairProd =   0.000476255; xsecUncert = 27.3032/100.;}
    if(stopMass==995) {xsecStopPairProd =   0.000455959; xsecUncert = 27.4544/100.;}
    if(stopMass==1000) {xsecStopPairProd =  0.000435488; xsecUncert = 27.6595/100.;}
    StopXSec stopCrossSection;
    stopCrossSection.stopProdXsec = xsecStopPairProd;
    stopCrossSection.stopProdXsecUncert = xsecUncert;
    
    if(xsecStopPairProd==0. || xsecUncert==0.)
        std::cout << "no xsec available for this mass point - choose another one!" << std::endl;
    
    return stopCrossSection;
}
