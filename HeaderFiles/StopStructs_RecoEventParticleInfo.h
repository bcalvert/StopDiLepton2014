// Structs used for containing event info about the reco particles
#include "TVector2.h"
using namespace std;
typedef struct BasicEventInfo {
    // Contains very basic info -- weights, run number, event number, etc
    int   nVtx, nVtxTrue;
    bool  passTrigDoubleMu, passTrigDoubleEl, passTrigElMu;
    float weight, preNVtxRWWeight, basicWeight;
    int   runNumber, lumiBlock, eventNumber;
    
    // will drop these dudes eventually
    bool  doData, doPURWOviToDESY, doHackPURW, isSignal, doPURW, doReReco, doPhiCorr;
    
    bool blindData;
    
    bool hasMETInfo, hasStopInfo, hasTopInfo;
    
    void ScaleWeights(float inSF, bool scalePreNVtxRWWeight = false, bool scaleBasicWeight = false) {
        weight *= inSF;
        if (scalePreNVtxRWWeight) {
            preNVtxRWWeight *= inSF;
        }
        if (scaleBasicWeight) {
            basicWeight *= inSF;
        }
    }
    void SetWeights(BasicEventInfo * inBEI) {
        weight              = inBEI->weight;
        preNVtxRWWeight     = inBEI->preNVtxRWWeight;
        basicWeight         = inBEI->basicWeight;
    }
    void DefaultWeights() {
        weight = 1.0;
        preNVtxRWWeight = 1.0;
        basicWeight = 1.0;
    }
    void  SetVars(BasicEventInfo * inBEI) {
        doData              = inBEI->doData;
        blindData           = inBEI->blindData;
        doReReco            = inBEI->doReReco;
        doPhiCorr           = inBEI->doPhiCorr;
        isSignal            = inBEI->isSignal;
        doPURW              = inBEI->doPURW;
        doPURWOviToDESY     = inBEI->doPURWOviToDESY;
        doHackPURW          = inBEI->doHackPURW;
        nVtx                = inBEI->nVtx;
        nVtxTrue            = inBEI->nVtxTrue;
        passTrigDoubleEl    = inBEI->passTrigDoubleEl;
        passTrigDoubleMu    = inBEI->passTrigDoubleMu;
        passTrigElMu        = inBEI->passTrigElMu;
        weight              = inBEI->weight;
        preNVtxRWWeight     = inBEI->preNVtxRWWeight;
        basicWeight         = inBEI->basicWeight;
        runNumber           = inBEI->runNumber;
        lumiBlock           = inBEI->lumiBlock;
        eventNumber         = inBEI->eventNumber;
        hasMETInfo          = inBEI->hasMETInfo;
        hasStopInfo         = inBEI->hasStopInfo;
        hasTopInfo          = inBEI->hasTopInfo;
    }
} BasicEventInfo;

typedef struct EventPileUpInfo {
    // contains information about the event's pileup and related variables
    int nVtx, nVtxTrue;
    int nPUBX0, nPUBXP, nPUBXM;
    float nPUTrue;
    float nPUWeight1DTrue, nPUWeight1D, nPUWeight3D;
    float eventRhoIso;
    void DefaultWeights() {
        nPUWeight1DTrue = 1.;
        nPUWeight1D     = 1.;
        nPUWeight3D     = 1.;
    }
    void Set1DWeightTrue(TH1F * inputHist) {
        nPUWeight1DTrue = inputHist->GetBinContent(inputHist->GetXaxis()->FindBin(nPUTrue));
    }
    void Set1DWeight(TH1F * inputHist) {
        nPUWeight1D = inputHist->GetBinContent(nPUBX0 + 1);
    }
    void Set3DWeight(TH3F * inputHist) {
        nPUWeight3D = inputHist->GetBinContent(nPUBX0 + 1, nPUBXP + 1, nPUBXM + 1);
    }
} EventPileUpInfo;

typedef struct EventLepInfo_LepCount {
    // bunch of random counts of different leptons
    
    int   EventNIsoElecs_pT20, EventNIsoMuons_pT20, EventNIsoPosits_pT20, EventNIsoMubars_pT20;
    int   EventNIsoElecs_pT10to20, EventNIsoMuons_pT10to20, EventNIsoPosits_pT10to20, EventNIsoMubars_pT10to20;
    int   EventNViableLepPairsPreMassCut, EventNViableLepPairsPostMassCut;
    
    void ClearVals() {
        EventNIsoElecs_pT20 = 0; EventNIsoMuons_pT20 = 0; EventNIsoPosits_pT20 = 0; EventNIsoMubars_pT20 = 0;
        EventNIsoElecs_pT10to20 = 0; EventNIsoMuons_pT10to20 = 0; EventNIsoPosits_pT10to20 = 0; EventNIsoMubars_pT10to20 = 0;
        EventNViableLepPairsPreMassCut = 0;
        EventNViableLepPairsPostMassCut = 0;
    }
    void BadVals() {
        EventNIsoElecs_pT20 = -1; EventNIsoMuons_pT20 = -1; EventNIsoPosits_pT20 = -1; EventNIsoMubars_pT20 = -1;
        EventNIsoElecs_pT10to20 = -1; EventNIsoMuons_pT10to20 = -1; EventNIsoPosits_pT10to20 = -1; EventNIsoMubars_pT10to20 = -1;
        EventNViableLepPairsPreMassCut = -1;
        EventNViableLepPairsPostMassCut = -1;
    }
} EventLepInfo_LepCount;

typedef struct EventLepInfo_SF {
    // ignore for now
    
    std::vector<float> DiLeptonTrigSF;
    std::vector<float> DiLeptonIDIsoSF;
    std::vector<float> DiLeptonTotSF;
} EventLepInfo_SF;

typedef struct EventLepInfo {
    // contains info about the leptons in the event, in particular the two we use for our dilepton selection
    
    
    float EventDiLepMass, EventLepST;
    bool  EventDiLepinZMass; //When true, means the diLep Mass is inside the ZMassWindow (76:106) GeV
    int   numSavedLeps;
    vector<Lepton> vecEventLeps;
    /*
     float DiLeptonTrigSF, DiLeptonTrigSFUpErr, DiLeptonTrigSFDownErr;
     float DiLeptonIDIsoSF, DiLeptonIDIsoSFUpErr, DiLeptonIDIsoSFDownErr;
     */
    std::vector<float> DiLeptonTrigSF;
    std::vector<float> DiLeptonIDIsoSF;
    std::vector<float> DiLeptonTotSF;
    EventLepInfo_LepCount ELI_LC;
    int   EventDiLepType;
    bool  doEvent;
    int DiLeptonEventType(int Lep0PDGID, int Lep1PDGID) {
        int productPdgId = Lep0PDGID * Lep1PDGID;
        if (productPdgId == -169) {
            return 0;
        }
        else if (productPdgId == -121) {
            return 1;
        }
        else if (productPdgId == -143) {
            return 2;
        }
        else {
            std::cout << "funky business in productPdgId " << productPdgId << std::endl;
        }
        return -1;
    }
    void ELIDefaultVarVals(int setNumSavedLeps = 2) {
        numSavedLeps = setNumSavedLeps;
        EventDiLepMass = 0.; EventDiLepType = -1; EventLepST = 0.;
        vecEventLeps.resize(numSavedLeps);
        for (int i = 0; i < numSavedLeps; ++i) {
            vecEventLeps[i].ClearVars();
        }
        ELI_LC.ClearVals();
        doEvent = false;
        EventDiLepinZMass = false;
        DiLeptonTrigSF.clear();
        DiLeptonIDIsoSF.clear();
        DiLeptonTotSF.clear();
    }
    void EventFails() {
        EventDiLepMass = -99999.; EventDiLepType = -1; EventLepST = -1.;
        for (int iLep = 0; iLep < numSavedLeps; ++iLep) {
            vecEventLeps[iLep].isBadLep();
        }
        ELI_LC.BadVals();
        doEvent = false;
        EventDiLepinZMass = false;
        DiLeptonTrigSF.clear();
        DiLeptonIDIsoSF.clear();
        DiLeptonTotSF.clear();
    }
    void SetLepST() {
        EventLepST = 0;
        for (int iLep = 0; iLep < 2; ++iLep) {
            EventLepST += vecEventLeps[iLep].BVC.Vec_Pt;
        }
    }
    void EventPasses(vector<int> * indexLeps, std::vector<Lepton> * inputLepVec) {
        for (int iLep = 0; iLep < numSavedLeps; ++iLep) {
            vecEventLeps[iLep] = inputLepVec->at(indexLeps->at(iLep));
        }
        EventDiLepType = DiLeptonEventType(vecEventLeps[0].PDGID, vecEventLeps[1].PDGID);
        EventDiLepMass = (vecEventLeps[0].P4 + vecEventLeps[1].P4).M();
        EventDiLepinZMass = EventDiLepMass > 76 && EventDiLepMass < 106;
        SetLepST();
        doEvent = true;
    }
    void GrabbedFromTree() {
        for (int iLep = 0; iLep < numSavedLeps; ++iLep) {
            vecEventLeps[iLep].BVC.SetP4Vals(&vecEventLeps[iLep].P4);
        }
        SetLepST();
        EventDiLepinZMass = EventDiLepMass > 76 && EventDiLepMass < 106;
    }
    void ScaleFactorTrigMC(vector<TH2F *> * vecTrigSF, bool doVerbosity = false) {
        /*
        float EtaEBEnd = 1.4442;
        float EtaEEBegin = 1.566;
        float safetyFactor = 1E-3; //safety factor to shift the eta past a given bound to avoid bin issues
        */
        //    float DiLeptonTrigSFCentVal, DiLeptonTrigSFErr;
        int XBin, YBin;
        float LepEtaX, LepEtaY;
        float tempLepEta0, tempLepEta1;
        
        tempLepEta0 = vecEventLeps[0].isElec() ? vecEventLeps[0].TempEtaFixElectron() : vecEventLeps[0].BVC.Vec_Eta;
        tempLepEta1 = vecEventLeps[1].isElec() ? vecEventLeps[1].TempEtaFixElectron() : vecEventLeps[1].BVC.Vec_Eta;
        
        /*
         tempLepEta0 = vecEventLeps[0].isElec() ? TruncateNum(vecEventLeps[0].BVC.Vec_Eta, EtaEBEnd, EtaEEBegin, safetyFactor) : vecEventLeps[0].BVC.Vec_Eta;
         tempLepEta1 = vecEventLeps[1].isElec() ? TruncateNum(vecEventLeps[1].BVC.Vec_Eta, EtaEBEnd, EtaEEBegin, safetyFactor) : vecEventLeps[1].BVC.Vec_Eta;
         */
        
        if (abs(vecEventLeps[0].PDGID) == abs(vecEventLeps[1].PDGID)) {
            //same flavor leptons -- utlize lead lepton for etaX
            LepEtaX = tempLepEta0;
            LepEtaY = tempLepEta1;
        }
        else if (abs(vecEventLeps[0].PDGID) < abs(vecEventLeps[1].PDGID)) {
            // lead lepton is electron but sub-lead is muon, utilize electron as etaX
            LepEtaX = tempLepEta0;
            LepEtaY = tempLepEta1;
        }
        else if (abs(vecEventLeps[0].PDGID) > abs(vecEventLeps[1].PDGID)) {
            // lead lepton is muon, utilize muon as etaY
            LepEtaX = tempLepEta1;
            LepEtaY = tempLepEta0;
        }
        else {
            cout << "issue with setting LepEtaX and LepEtaY" << endl;
        }
        
        XBin = vecTrigSF->at(EventDiLepType)->GetXaxis()->FindBin(fabs(LepEtaX));
        YBin = vecTrigSF->at(EventDiLepType)->GetXaxis()->FindBin(fabs(LepEtaY));
        if (XBin > vecTrigSF->at(EventDiLepType)->GetNbinsX()) XBin = vecTrigSF->at(EventDiLepType)->GetNbinsX();
        if (XBin < 1) XBin = 1;
        if (YBin > vecTrigSF->at(EventDiLepType)->GetNbinsY()) YBin = vecTrigSF->at(EventDiLepType)->GetNbinsY();
        if (YBin < 1) YBin = 1;
        //        cout << "TrigMC test 5 " << endl;
        //    DiLeptonTrigSFCentVal = vecTrigSF->at(EventDiLepType)->GetBinContent(XBin, YBin);
        //    DiLeptonTrigSFErr = vecTrigSF->at(EventDiLepType)->GetBinError(XBin, YBin);
        //    return DiLeptonTrigSFCentVal + Syst * DiLeptonTrigSFErr;
        DiLeptonTrigSF.resize(3);
        DiLeptonTrigSF[0] = vecTrigSF->at(EventDiLepType)->GetBinContent(XBin, YBin);
        DiLeptonTrigSF[1] = vecTrigSF->at(EventDiLepType)->GetBinError(XBin, YBin);
        DiLeptonTrigSF[2] = vecTrigSF->at(EventDiLepType)->GetBinError(XBin, YBin);
        if (DiLeptonTrigSF[0] < 1E-3 ) {
            cout << "issue with Trigger SF! -- very close to 0! " << DiLeptonTrigSF[0] << endl;
        }
        if (doVerbosity) {
            cout << "LepEtaX used " << LepEtaX << endl;
            cout << "LepEtaY used " << LepEtaY << endl;
            cout << "Trig SF " << DiLeptonTrigSF[0] << endl;
        }
    }
    void SingLepIDIsoSF(float * SFArray, Lepton * inputLep, TH2F * histElecSF, TH2F * histMuonSF, bool doVerbosity = false) {
        /*
        float EtaEBEnd = 1.4442;
        float EtaEEBegin = 1.566;
        float safetyFactor = 1E-3; //safety factor to shift the eta past a given bound to avoid bin issues
        */
        TH2F * histToUse = inputLep->isElec() ? histElecSF : histMuonSF;
        int nXbins = histToUse->GetNbinsX();
        int nYbins = histToUse->GetNbinsY();
        float tempLepEta = inputLep->BVC.Vec_Eta;
        
        if (inputLep->isElec()) {
            tempLepEta = inputLep->TempEtaFixElectron();
            /*
             if (tempLepEta > EtaEBEnd && tempLepEta < EtaEEBegin) {
             tempLepEta = TruncateNum(tempLepEta, EtaEBEnd, EtaEEBegin, safetyFactor);
             }
             if (tempLepEta > -1 * EtaEEBegin && tempLepEta < -1 * EtaEBEnd) {
             tempLepEta = TruncateNum(tempLepEta, -1 * EtaEEBegin, -1 * EtaEBEnd, safetyFactor);
             }
             */
        }
        int XBin = histToUse->GetXaxis()->FindBin(tempLepEta);
        int YBin = histToUse->GetYaxis()->FindBin(inputLep->BVC.Vec_Pt);
        if (XBin > nXbins) XBin = nXbins;
        if (XBin < 1) XBin = 1;
        if (YBin > nYbins) YBin = nYbins;
        if (YBin < 1) YBin = 1;
        SFArray[0] = histToUse->GetBinContent(XBin, YBin);
        SFArray[1] = histToUse->GetBinError(XBin, YBin);
        if (SFArray[0] < 1E-3) {
            cout << "issue with ID/Iso SF! -- very close to 0! " << SFArray[0] << endl;
            inputLep->PrintVals();
        }
        if (doVerbosity) {
            cout << "Lepton Pt " << inputLep->BVC.Vec_Pt << endl;
            cout << "YBin " << YBin << endl;
            cout << "Lepton Eta " << tempLepEta << endl;
            cout << "XBin " << XBin << endl;
            cout << "hist name " << histToUse->GetName() << endl;
            cout << "scale fac " << histToUse->GetBinContent(XBin, YBin) << endl;
        }
    }
    void ScaleFactorIDIsoMC(TH2F * histElecSF, TH2F * histMuonSF, bool doVerbosity = false) {
        float Lep0SF[2], Lep1SF[2];
        //        cout << "setting lep 0 " << endl;
        SingLepIDIsoSF(Lep0SF, &vecEventLeps[0], histElecSF, histMuonSF, doVerbosity);
        //        cout << "setting lep 1 " << endl;
        SingLepIDIsoSF(Lep1SF, &vecEventLeps[1], histElecSF, histMuonSF, doVerbosity);
        DiLeptonIDIsoSF.resize(3);
        DiLeptonIDIsoSF[0] = Lep0SF[0] * Lep1SF[0];
        
        DiLeptonIDIsoSF[1] = (Lep0SF[0] + Lep0SF[1]) * (Lep1SF[0] + Lep1SF[1]);
        DiLeptonIDIsoSF[1] -= DiLeptonIDIsoSF[0];
        DiLeptonIDIsoSF[1] = fabs(DiLeptonIDIsoSF[1]);
        
        DiLeptonIDIsoSF[2] = (Lep0SF[0] - Lep0SF[1]) * (Lep1SF[0] - Lep1SF[1]);
        DiLeptonIDIsoSF[2] -= DiLeptonIDIsoSF[0];
        DiLeptonIDIsoSF[2] = fabs(DiLeptonIDIsoSF[2]);
        
        if (doVerbosity) {
            cout << "Lepton 0 characteristics " << endl;
            vecEventLeps[0].PrintVals();
            cout << "Lep 0 ID/Iso SF " << Lep0SF[0] << endl;
            cout << "Lepton 1 characteristics " << endl;
            vecEventLeps[1].PrintVals();
            cout << "Lep 1 ID/Iso SF " << Lep1SF[0] << endl;
        }
    }
    void ScaleFactorIDIsoMCPieter(bool doSignal, TH2F * histElecFastSimSF, TH2F * histMuonFastSimSF) {
        float Lep0FastSimSF, Lep1FastSimSF;
        float Lep0FastSimSFErr = 0, Lep1FastSimSFErr = 0;
        float TotLepFastSimSF, TotLepFastSimSFUpErr = 0, TotLepFastSimSFDownErr = 0;
        float FastSimSystErrBase = 0.02;
        TH2F * Lep0FastSimHist, * Lep1FastSimHist;
        float SF_IDISO[3]           = {0.997, 0.975, 0.986};
        float SF_IDISOSystUp[3]     = {0.00085, 0.006, 0.007};
        float SF_IDISOSystDown[3]   = {0.00085, 0.006, 0.007};
        
        float baseSF = SF_IDISO[EventDiLepType];
        float baseSFUpErr = 0, baseSFDownErr = 0;
        int XBin, YBin;
        baseSFUpErr = SF_IDISOSystUp[EventDiLepType];
        baseSFDownErr = SF_IDISOSystDown[EventDiLepType];
        
        DiLeptonIDIsoSF.resize(3);
        if (doSignal) {
            Lep0FastSimHist = vecEventLeps[0].isElec() ? histElecFastSimSF : histMuonFastSimSF;
            XBin = Lep0FastSimHist->GetXaxis()->FindBin(vecEventLeps[0].BVC.Vec_Pt);
            YBin = Lep0FastSimHist->GetYaxis()->FindBin(fabs(vecEventLeps[0].BVC.Vec_Eta));
            if (XBin > Lep0FastSimHist->GetNbinsX()) XBin = Lep0FastSimHist->GetNbinsX();
            if (YBin > Lep0FastSimHist->GetNbinsY()) YBin = Lep0FastSimHist->GetNbinsY();
            Lep0FastSimSF = Lep0FastSimHist->GetBinContent(XBin, YBin);
            
            Lep1FastSimHist = vecEventLeps[1].isElec() ? histElecFastSimSF : histMuonFastSimSF;
            XBin = Lep1FastSimHist->GetXaxis()->FindBin(vecEventLeps[1].BVC.Vec_Pt);
            YBin = Lep1FastSimHist->GetYaxis()->FindBin(fabs(vecEventLeps[1].BVC.Vec_Eta));
            if (XBin > Lep1FastSimHist->GetNbinsX()) XBin = Lep1FastSimHist->GetNbinsX();
            if (YBin > Lep1FastSimHist->GetNbinsY()) YBin = Lep1FastSimHist->GetNbinsY();
            Lep1FastSimSF = Lep1FastSimHist->GetBinContent(XBin, YBin);
            
            TotLepFastSimSF = Lep0FastSimSF * Lep1FastSimSF;
            
            Lep0FastSimSFErr = FastSimSystErrBase * Lep0FastSimSF;
            Lep1FastSimSFErr = FastSimSystErrBase * Lep1FastSimSF;
            TotLepFastSimSFUpErr = Lep0FastSimSFErr * Lep1FastSimSFErr;
            TotLepFastSimSFUpErr += Lep0FastSimSF * Lep1FastSimSFErr + Lep1FastSimSF * Lep0FastSimSFErr;
            TotLepFastSimSFDownErr = Lep0FastSimSFErr * Lep1FastSimSFErr;
            TotLepFastSimSFDownErr -= Lep0FastSimSF * Lep1FastSimSFErr + Lep1FastSimSF * Lep0FastSimSFErr;
            TotLepFastSimSFDownErr = fabs(TotLepFastSimSFDownErr);
            
            DiLeptonIDIsoSF[0] = baseSF * TotLepFastSimSF;
            DiLeptonIDIsoSF[1] = TotLepFastSimSFUpErr * baseSFUpErr;
            DiLeptonIDIsoSF[1] += TotLepFastSimSF * baseSFUpErr + TotLepFastSimSFUpErr * baseSF;
            DiLeptonIDIsoSF[2] = TotLepFastSimSFDownErr * baseSFDownErr;
            DiLeptonIDIsoSF[2] -= TotLepFastSimSF * baseSFDownErr + TotLepFastSimSFDownErr * baseSF;
            DiLeptonIDIsoSF[2] = fabs(DiLeptonIDIsoSF[2]);
        }
        else {
            DiLeptonIDIsoSF[0] = baseSF;
            DiLeptonIDIsoSF[1] = baseSFUpErr;
            DiLeptonIDIsoSF[2] = baseSFDownErr;
        }
    }
    //    void SetScaleFactors(bool doSignal, std::vector<TH2F *> * vecTrigSFBkg, std::vector<TH2F *> * vecTrigSFSig, TH2F * histElecSF, TH2F * histMuonSF) {
    void SetScaleFactors(std::vector<TH2F *> * vecTrigSF, TH2F * histElecSF, TH2F * histMuonSF, bool doVerbosity = false) {
        ScaleFactorTrigMC(vecTrigSF, doVerbosity);
        ScaleFactorIDIsoMC(histElecSF, histMuonSF, doVerbosity);
        DiLeptonTotSF.resize(3);
        if (DiLeptonIDIsoSF.size() < 3 || DiLeptonTrigSF.size() < 3) {
            cout << "issue: sizes are less than 3! check that they got set " << endl;
            cout << "DiLeptonIDIsoSF.size()  " << DiLeptonIDIsoSF.size() << endl;
            cout << "DiLeptonTrigSF.size()  " << DiLeptonTrigSF.size() << endl;
        }
        else {
            /*
             cout << "DiLeptonTrigSF[0] " << DiLeptonTrigSF[0] << endl;
             cout << "DiLeptonTrigSF[1] " << DiLeptonTrigSF[1] << endl;
             cout << "DiLeptonTrigSF[2] " << DiLeptonTrigSF[2] << endl;
             cout << "DiLeptonIDIsoSF[0] " << DiLeptonIDIsoSF[0] << endl;
             cout << "DiLeptonIDIsoSF[1] " << DiLeptonIDIsoSF[1] << endl;
             cout << "DiLeptonIDIsoSF[2] " << DiLeptonIDIsoSF[2] << endl;
             cout << endl;
             */
            DiLeptonTotSF[0] = DiLeptonTrigSF[0] * DiLeptonIDIsoSF[0];
            DiLeptonTotSF[1] = TMath::Sqrt(DiLeptonTrigSF[1] * DiLeptonTrigSF[1] + DiLeptonIDIsoSF[1] * DiLeptonIDIsoSF[1]);
            DiLeptonTotSF[2] = TMath::Sqrt(DiLeptonTrigSF[2] * DiLeptonTrigSF[2] + DiLeptonIDIsoSF[2] * DiLeptonIDIsoSF[2]);
        }
    }
    
    
    void SetScaleFactorsOld(bool doSignal, std::vector<TH2F *> * vecTrigSF, TH2F * histElecSF, TH2F * histMuonSF) {
        //        cout << "SF Old test 1 " << endl;
        ScaleFactorTrigMC(vecTrigSF);
        //        cout << "SF Old test 2 " << endl;
        ScaleFactorIDIsoMCPieter(doSignal, histElecSF, histMuonSF);
        //        cout << "SF Old test 3 " << endl;
        DiLeptonTotSF.resize(3);
        if (DiLeptonIDIsoSF.size() < 3 || DiLeptonTrigSF.size() < 3) {
            cout << "issue: sizes are less than 3! check that they got set " << endl;
            cout << "DiLeptonIDIsoSF.size()  " << DiLeptonIDIsoSF.size() << endl;
            cout << "DiLeptonTrigSF.size()  " << DiLeptonTrigSF.size() << endl;
        }
        else {
            /*
             cout << "DiLeptonTrigSF[0] " << DiLeptonTrigSF[0] << endl;
             cout << "DiLeptonTrigSF[1] " << DiLeptonTrigSF[1] << endl;
             cout << "DiLeptonTrigSF[2] " << DiLeptonTrigSF[2] << endl;
             cout << "DiLeptonIDIsoSF[0] " << DiLeptonIDIsoSF[0] << endl;
             cout << "DiLeptonIDIsoSF[1] " << DiLeptonIDIsoSF[1] << endl;
             cout << "DiLeptonIDIsoSF[2] " << DiLeptonIDIsoSF[2] << endl;
             cout << endl;
             */
            DiLeptonTotSF[0] = DiLeptonTrigSF[0] * DiLeptonIDIsoSF[0];
            DiLeptonTotSF[1] = TMath::Sqrt(DiLeptonTrigSF[1] * DiLeptonTrigSF[1] + DiLeptonIDIsoSF[1] * DiLeptonIDIsoSF[1]);
            DiLeptonTotSF[2] = TMath::Sqrt(DiLeptonTrigSF[2] * DiLeptonTrigSF[2] + DiLeptonIDIsoSF[2] * DiLeptonIDIsoSF[2]);
        }
        //        cout << "SF Old test 4 " << endl;
    }
    
    void SetScaleFactorsFailedEvent() {
        DiLeptonTrigSF.resize(3);
        DiLeptonTrigSF[0] = 0.0;
        DiLeptonTrigSF[1] = 0.0;
        DiLeptonTrigSF[1] = 0.0;
        DiLeptonIDIsoSF.resize(3);
        DiLeptonIDIsoSF[0] = 0.0;
        DiLeptonIDIsoSF[1] = 0.0;
        DiLeptonIDIsoSF[2] = 0.0;
        DiLeptonTotSF.resize(3);
        DiLeptonTotSF[0] = 0.0;
        DiLeptonTotSF[1] = 0.0;
        DiLeptonTotSF[2] = 0.0;
    }
    float GetSF(int Syst = 0) {
        if (DiLeptonTotSF.size() < 3) cout << "issue with DiLeptonTotSF.size(): " << DiLeptonTotSF.size() << endl;
        float outSF = DiLeptonTotSF[0];
        //        cout << "DiLeptonTotSF[0] " << DiLeptonTotSF[0] << endl;
        if (Syst > 0) {
            outSF += DiLeptonTotSF[1];
            //           cout << "DiLeptonTotSF[1] " << DiLeptonTotSF[1] << endl;
        }
        else if (Syst < 0) {
            outSF -= DiLeptonTotSF[2];
            //           cout << "DiLeptonTotSF[2] " << DiLeptonTotSF[1] << endl;
        }
        return outSF;
    }
    void FindMatchingGenLeps(GenLeptonEventPointers * inGLEPs) {
        for (int iLep = 0; iLep < numSavedLeps; ++iLep) {
            vecEventLeps[iLep].isGenMatched = inGLEPs->FindMatchGenLep(&vecEventLeps[iLep], &vecEventLeps[iLep].GP);
            if (!vecEventLeps[iLep].isGenMatched) {
                vecEventLeps[iLep].GP.ClearVals();
            }
            else {
                vecEventLeps[iLep].GP.BVC.ExtractParams(&vecEventLeps[iLep].GP.P4);
                //                cout << " for iLep " << iLep << endl;
                //                vecEventLeps[iLep].PrintVals();
            }
            vecEventLeps[iLep].SetPromptLeptonStatus();
            //            cout << " for iLep " << iLep << endl;
            //            vecEventLeps[iLep].PrintVals();
        }
        //        cout << endl;
    }
    void GrabbedGenParticlesFromTree() {
        for (int iLep = 0; iLep < numSavedLeps; ++iLep) {
            if (!vecEventLeps[iLep].isGenMatched) {
                vecEventLeps[iLep].GP.ClearVals();
            }
            else {
                vecEventLeps[iLep].GP.BVC.SetP4Vals(&vecEventLeps[iLep].GP.P4);
            }
        }
    }
    bool PassesAdditionalLepPtCut(int whichLep, float cutVal) {
        
        return (doEvent && vecEventLeps[whichLep].BVC.Vec_Pt > cutVal);
    }
    bool HasMoreThanNLeps(int numPairsThresh = 1) {
        return ELI_LC.EventNViableLepPairsPostMassCut > numPairsThresh;
    }
} EventLepInfo;

typedef struct EventJetInfo {
    // contains information about the pf jets passing cuts in the event
    
    float EventHT, EventJetST;
    int   EventNJets, EventNBtagJets;
    int   numSavedJets, numSavedBJets;
    TLorentzVector EventSumJetP4, EventSumBJetP4;
    BasicVecComponents BVC_SumJet, BVC_SumBJet;
    
    vector<PFJet> vecEventJets;
    
    vector<PFJet> vecEventBTagJets;
    vector<unsigned int> vecEventBTagJets_Index;
    void SetJetST() {
        EventJetST = 0.;
        for (int iJet = 0; iJet < 2; ++iJet) {
            EventJetST += vecEventJets[iJet].BVC.Vec_Pt;
        }
    }
    void SetNonBTagInfo(EventJetInfo * inEJI) {
        
        EventHT = inEJI->EventHT;
        EventJetST = inEJI->EventJetST;
        
        EventSumJetP4 = inEJI->EventSumJetP4;
        BVC_SumJet.ExtractParams(&EventSumJetP4);
        
        EventNJets = inEJI->EventNJets;
        
        vecEventJets.resize(numSavedJets);
        for (int iJet = 0; iJet < numSavedJets; ++iJet) {
            vecEventJets[iJet].SetNonBJetInfo(&inEJI->vecEventJets[iJet]);
        }
    }
    void EJIDefaultVarVals(int setNumSavedJets = 3, int setNumSavedBJets = 2) {
        numSavedJets = setNumSavedJets; numSavedBJets = setNumSavedBJets;
        EventHT = 0.; EventJetST = 0.; EventNJets = 0; EventNBtagJets = 0;
        DefaultLorentzVector(&EventSumJetP4);
        BVC_SumJet.ExtractParams(&EventSumJetP4);
        
        DefaultLorentzVector(&EventSumBJetP4);
        BVC_SumBJet.ExtractParams(&EventSumBJetP4);
        
        vecEventJets.resize(numSavedJets);;
        vecEventBTagJets.resize(numSavedBJets);
        vecEventBTagJets_Index.resize(numSavedBJets);
        
        for (int iJet = 0; iJet < numSavedJets; ++iJet) {
            vecEventJets[iJet].ClearVars();
        }
        for (int iBJet = 0; iBJet < numSavedBJets; ++iBJet) {
            vecEventBTagJets[iBJet].ClearVars();
            vecEventBTagJets_Index[iBJet] = -1;
        }
    }
    void EJISetValsInput(float inputHT, TLorentzVector * inSumJetVec, TLorentzVector * inSumBJetVec, int inputNJets, vector<int> * vecInJetIndex, int inputNBtagJets, vector<int> * vecInBJetIndex, vector<int> * vecInBJetSubIndex, vector<PFJet> * inputPFJetVec) {
        EventHT = inputHT;
        BVC_SumJet.ExtractParams(inSumJetVec);
        BVC_SumJet.SetP4Vals(&EventSumJetP4);
        
        BVC_SumBJet.ExtractParams(inSumBJetVec);
        BVC_SumBJet.SetP4Vals(&EventSumBJetP4);
        
        EventNJets = inputNJets; EventNBtagJets = inputNBtagJets;
        
        for (int iJet = 0; iJet < numSavedJets; ++iJet) {
            if (vecInJetIndex->at(iJet) > -1) {
                vecEventJets[iJet] = inputPFJetVec->at(vecInJetIndex->at(iJet));
            }
            else {
                vecEventJets[iJet].ClearVars();
            }
        }
        for (int iBJet = 0; iBJet < numSavedBJets; ++iBJet) {
            if (vecInBJetIndex->at(iBJet) > -1) {
                vecEventBTagJets[iBJet] = inputPFJetVec->at(vecInBJetIndex->at(iBJet));
                vecEventBTagJets_Index[iBJet] = vecInBJetSubIndex->at(iBJet);
            }
            else {
                vecEventBTagJets[iBJet].ClearVars();
                vecEventBTagJets_Index[iBJet] = -1;
            }
        }
        SetJetST();
    }
    
    void EJISetValsInput(float inputHT, TLorentzVector * inSumJetVec, TLorentzVector * inSumBJetVec, int inputNJets, vector<int> * vecInJetIndex, int inputNBtagJets, vector<int> * vecInBJetIndex, vector<int> * vecInBJetSubIndex, vector<SmearPFJet> * inputSmearPFJetVec) {
        EventHT = inputHT;
        BVC_SumJet.ExtractParams(inSumJetVec);
        BVC_SumJet.SetP4Vals(&EventSumJetP4);
        
        BVC_SumBJet.ExtractParams(inSumBJetVec);
        BVC_SumBJet.SetP4Vals(&EventSumBJetP4);
        
        EventNJets = inputNJets; EventNBtagJets = inputNBtagJets;
        
        for (int iJet = 0; iJet < numSavedJets; ++iJet) {
            if (vecInJetIndex->at(iJet) > -1) {
                vecEventJets[iJet] = inputSmearPFJetVec->at(vecInJetIndex->at(iJet)).intPFJet;
            }
            else {
                vecEventJets[iJet].ClearVars();
            }
        }
        for (int iBJet = 0; iBJet < numSavedBJets; ++iBJet) {
            if (vecInBJetIndex->at(iBJet) > -1) {
                vecEventBTagJets[iBJet] = inputSmearPFJetVec->at(vecInBJetIndex->at(iBJet)).intPFJet;
                vecEventBTagJets_Index[iBJet] = vecInBJetSubIndex->at(iBJet);
            }
            else {
                vecEventBTagJets[iBJet].ClearVars();
                vecEventBTagJets_Index[iBJet] = -1;
            }
        }
        SetJetST();
    }
    
    
    int JetSelectorForMT2(vector<TLorentzVector> * inVecJetP4s, vector<int> * inVecJetFlavor, int levelVerbosity = 0) {
        int caseMT2bb = -1;
        if (levelVerbosity > 0) {
            cout << "EventNJets " << EventNJets << endl;
        }
        if (EventNJets > 1) {
            if (EventNBtagJets > 1) {
                inVecJetFlavor->at(0) = vecEventBTagJets[0].partonFlavor;
                inVecJetP4s->at(0) = vecEventBTagJets[0].P4;
                
                inVecJetFlavor->at(1) = vecEventBTagJets[1].partonFlavor;
                inVecJetP4s->at(1) = vecEventBTagJets[1].P4;
                caseMT2bb = 0;
            }
            else if (EventNBtagJets == 1) {
                inVecJetFlavor->at(0) = vecEventBTagJets[0].partonFlavor;
                inVecJetP4s->at(0) = vecEventBTagJets[0].P4;
                if (vecEventBTagJets_Index[0] == 0) {
                    //leading jet is b-jet so grab sub-leading jet for "second b"
                    inVecJetFlavor->at(1) = vecEventJets[1].partonFlavor;
                    inVecJetP4s->at(1) = vecEventJets[1].P4;
                    caseMT2bb = 1;
                }
                else {
                    //leading jet is not b-jet so grab leading jet for "second b"
                    inVecJetFlavor->at(1) = vecEventJets[0].partonFlavor;
                    inVecJetP4s->at(1) = vecEventJets[0].P4;
                    caseMT2bb = 2;
                }
            }
            else {
                //no b-jets so just grab leading two jets as "b"'s
                inVecJetFlavor->at(0) = vecEventJets[0].partonFlavor;
                inVecJetP4s->at(0) = vecEventJets[0].P4;
                
                inVecJetFlavor->at(1) = vecEventJets[1].partonFlavor;
                inVecJetP4s->at(1) = vecEventJets[1].P4;
                
                caseMT2bb = 3;
            }
        }
        else {
            inVecJetFlavor->resize(0);
            inVecJetP4s->resize(0);
            caseMT2bb = -1;
        }
        
        return caseMT2bb;
    }
    void GrabbedFromTree() {
        for (int iJet = 0; iJet < numSavedJets; ++iJet) {
            vecEventJets[iJet].BVC.SetP4Vals(&vecEventJets[iJet].P4);
        }
        for (int iBJet = 0; iBJet < numSavedBJets; ++iBJet) {
            vecEventBTagJets[iBJet].BVC.SetP4Vals(&vecEventBTagJets[iBJet].P4);
        }
    }
} EventJetInfo;


typedef struct EventRecoilInfo {
    // contains information about the hadronic recoil of the event -- defined a la a Z->ll MET study
    
    TVector3 P3;
    float recoilMag, recoilPhi;
    float recoilUPar, recoilUPerp;
    void GrabbedFromTree() {
        P3.SetPtEtaPhi(recoilMag, 0., recoilPhi);
    }
    void SetRecoilVarsBadProbe() {
        P3.SetPtEtaPhi(1E-8, 1E-8, 1E-8);
        recoilMag = 0;
        recoilPhi = 0;
        recoilUPar = 0;
        recoilUPerp = 0;
    }
    void SetVarsfromVec(TVector3 inProbeVec) {
        recoilMag = P3.Pt();
        recoilPhi = P3.Phi();
        recoilUPar = recoilMag * TMath::Cos(recoilPhi - inProbeVec.Phi());
        recoilUPerp = recoilMag * TMath::Sin(recoilPhi - inProbeVec.Phi());
    }
    void SetRecoilVars(TVector3 inProbeVec, TVector3 METP3) {
        P3.SetXYZ(0., 0., 0.);
        P3 = P3 - inProbeVec - METP3;
        recoilMag = P3.Pt();
        recoilPhi = P3.Phi();
        recoilUPar = recoilMag * TMath::Cos(recoilPhi - inProbeVec.Phi());
        recoilUPerp = recoilMag * TMath::Sin(recoilPhi - inProbeVec.Phi());
    }
    void ERIDefaultVarVals() {
        P3.SetPtEtaPhi(1E-8, 1E-8, 1E-8);
        recoilMag   = 0.;
        recoilPhi   = 0.;
        recoilUPar  = 0.;
        recoilUPerp = 0.;
    }
    void PrintVals() {
        std::cout << "recoilMag " << recoilMag << std::endl;
        std::cout << "recoilPhi " << recoilPhi << std::endl;
        std::cout << "recoilUPar " << recoilUPar << std::endl;
        std::cout << "recoilUPerp " << recoilUPerp << std::endl;
    }
    void SetVars(EventRecoilInfo * inERI) {
        P3 = inERI->P3;
        recoilMag = inERI->recoilMag;
        recoilPhi = inERI->recoilPhi;
        recoilUPar = inERI->recoilUPar;
        recoilUPerp = inERI->recoilUPerp;
    }
} EventRecoilInfo;
typedef struct EventMT2Info {
    // Contains information about the various MT and MT2 variables in the event
    
    float EventMT2ll, EventMT2lblb, EventMT2bb_ZMET;
    float EventDeltaPhiMT2lblb_JetsUsed, EventDeltaPhiMT2lblb_BLepsUsed;
    float EventDeltaPhiMT2bb_ZMET_JetsUsed;
    float EventMassBLep0_BLepsUsed, EventMassBLep1_BLepsUsed;
    vector<TLorentzVector> EventVecBLepsMT2lb;
    int caseMT2lblb;
    vector<int> vecCorrPairValMT2lblb;
    // 0 is minMT2lb, 1 is minMaxMass, 2 is minAbsMassDiff, 3 is AND of 0 and 1, 4 is OR of 0 and 1
    // 5 is AND of 0 and 2, 6 is OR of 0 and 2, 7 is AND of 1 and 2, 8 is OR of 1 and 2, 9 is AND of 0, 1, and 2; 10 is OR of 0, 1, and 2
    float MT_Lep0, MT_Lep1, MT_HTVec, MT_BHTVec;
    void EMT2IDefaultVarVals() {
        EventMT2ll = 0.; EventMT2lblb = 0.; EventMT2bb_ZMET = 0.;
        EventDeltaPhiMT2lblb_JetsUsed = -99.; EventDeltaPhiMT2lblb_BLepsUsed= -99.;
        EventDeltaPhiMT2bb_ZMET_JetsUsed = -99.;
        EventMassBLep0_BLepsUsed = -99.; EventMassBLep1_BLepsUsed = -99.;
        EventVecBLepsMT2lb.clear();
        EventVecBLepsMT2lb.resize(2);
        caseMT2lblb = -1;
        MT_Lep0 = 0.; MT_Lep1 = 0.; MT_HTVec = 0.; MT_BHTVec = 0.;
        vecCorrPairValMT2lblb.resize(16);
        for (int iCorrPair = 0; iCorrPair < (int) vecCorrPairValMT2lblb.size(); ++iCorrPair) {
            vecCorrPairValMT2lblb[iCorrPair] = -3;
        }
    }
    void SetBadMT2lblb() {
        caseMT2lblb = -1;
        EventMT2lblb = -99.;
        EventDeltaPhiMT2bb_ZMET_JetsUsed = -99.;
        EventDeltaPhiMT2lblb_JetsUsed = -99.;
        EventDeltaPhiMT2lblb_BLepsUsed = -99.;
        for (int iCorrPair = 0; iCorrPair < (int) vecCorrPairValMT2lblb.size(); ++iCorrPair) {
            vecCorrPairValMT2lblb[iCorrPair] = -3;
        }
    }
    void SetBadMT2ll() {
        EventMT2ll = -99.;
    }
    void SetBadMT2bb_ZMET() {
        EventMT2bb_ZMET = -99;
    }
    void SetMTBadLep() {
        MT_Lep0 = -99.;
        MT_Lep1 = -99.;
    }
    void SetMTBadJet() {
        MT_HTVec = -99.;
    }
    void SetMTBadBJet() {
        MT_BHTVec = -99.;
    }
    void SetMTLep(TLorentzVector * inLepton, float EventMET, float EventMETPhi, int whichMT = 1) {
        //        float Angle = dPhi(inLepton->Phi(), EventMETPhi);
        //        float MTSq = 2*EventMET*inLepton->Pt()*(1-TMath::Cos(Angle));
        TLorentzVector METvec; METvec.SetPtEtaPhiM(EventMET, 0., EventMETPhi, 0);
        switch (whichMT) {
            case 1:
                //                MT_Lep0 = TMath::Sqrt(MTSq);
                MT_Lep0 = GetMT(inLepton, &METvec);
                break;
            case 2:
                //                MT_Lep1 = TMath::Sqrt(MTSq);
                MT_Lep1 = GetMT(inLepton, &METvec);
                break;
            default:
                cout << "need to pick which MT to set!!" << endl;
                break;
        }
    }
    
    void SetMTJet(TLorentzVector * inJet, float EventMET, float EventMETPhi) {
        //        float Angle = dPhi(inJet->Phi(), EventMETPhi);
        //        float MTSq = 2*EventMET*inJet->Pt()*(1-TMath::Cos(Angle));
        TLorentzVector METvec; METvec.SetPtEtaPhiM(EventMET, 0., EventMETPhi, 0);
        //        cout << "inJet Pt " << inJet->Pt() << endl;
        MT_HTVec = GetMT(inJet, &METvec);
    }
    
    void SetMTBJet(TLorentzVector * inBJet, float EventMET, float EventMETPhi) {
        //        float Angle = dPhi(inJet->Phi(), EventMETPhi);
        //        float MTSq = 2*EventMET*inJet->Pt()*(1-TMath::Cos(Angle));
        TLorentzVector METvec; METvec.SetPtEtaPhiM(EventMET, 0., EventMETPhi, 0);
        //        cout << "inJet Pt " << inJet->Pt() << endl;
        MT_BHTVec = GetMT(inBJet, &METvec);
    }
    void CalcMT2ll(EventLepInfo * inELI, float EventMET, float EventMETPhi) {
        EventMT2ll = getMT2(inELI->vecEventLeps[0].P4, inELI->vecEventLeps[1].P4, EventMET, EventMETPhi);
    }
    void CalcMT2bb_ZMET(EventJetInfo * inEJI, EventRecoilInfo * inERI, int levelVerbosity = 0) {

        float testMass = 80.4;
        TVector3 ZMET = -1 * inERI->P3;
        
        vector<int> vecJetPartFlavor(2);
        vector<TLorentzVector> vecJetsMT2bb(2);
        int caseMT2bb = inEJI->JetSelectorForMT2(&vecJetsMT2bb, &vecJetPartFlavor, levelVerbosity);
        if (vecJetsMT2bb.size() == 2) {
            EventMT2bb_ZMET = getMT2(vecJetsMT2bb[0], vecJetsMT2bb[1], ZMET.Pt(), ZMET.Phi(), testMass);
            EventDeltaPhiMT2bb_ZMET_JetsUsed = dPhi(vecJetsMT2bb[0].Phi(), vecJetsMT2bb[1].Phi());
        }
        else {
            if (vecJetsMT2bb.size() > 2) {
                cout << "something weird with vecJetsMT2bb size " << vecJetsMT2bb.size() << endl;
            }
            SetBadMT2bb_ZMET();
        }
    }
    void SetValsMT2lbPairing(float MT2lbPair1, float MT2lbPair2, float &returnMT2lb, vector<TLorentzVector> * vecBLeps_Pair1, vector<TLorentzVector> * vecBLeps_Pair2, vector<TLorentzVector> * vec_GoodBLeps, int corrPairCheckIndex) {
        if (corrPairCheckIndex == 0) {
            returnMT2lb = MT2lbPair1;
            vec_GoodBLeps->at(0) = vecBLeps_Pair1->at(0);
            vec_GoodBLeps->at(1) = vecBLeps_Pair1->at(1);
        }
        else if (corrPairCheckIndex == 1) {
            returnMT2lb = MT2lbPair2;
            vec_GoodBLeps->at(0) = vecBLeps_Pair2->at(0);
            vec_GoodBLeps->at(1) = vecBLeps_Pair2->at(1);
        }
        else {
            cout << "corrPairCheckIndex != 0 or 1; it is " << corrPairCheckIndex << endl;
        }
    }
    float MT2lbPairingCheck(vector<TLorentzVector> * vecBLeps_Pair1, vector<TLorentzVector> * vecBLeps_Pair2, vector<TLorentzVector> * vec_GoodBLeps, float MET, float METPhi, vector<int> * vecCorrPairCheckIndex, int whichCheck = 0) {
        
        float topMass = 176;
        
        float MT2lbPair1 = getMT2(vecBLeps_Pair1->at(0), vecBLeps_Pair1->at(1), MET, METPhi);
        float MT2lbPair2 = getMT2(vecBLeps_Pair2->at(0), vecBLeps_Pair2->at(1), MET, METPhi);
        
        bool minMT2lbPair2 = (MT2lbPair1 > MT2lbPair2);
        
        float massBLeadLep_Pair1 = vecBLeps_Pair1->at(0).M();
        float massBSubLep_Pair1 = vecBLeps_Pair1->at(1).M();
        
        float massBLeadLep_Pair2 = vecBLeps_Pair2->at(0).M();
        float massBSubLep_Pair2 = vecBLeps_Pair2->at(1).M();
        
        float maxBLepMassPair1 = TMath::Max(massBLeadLep_Pair1, massBSubLep_Pair1);
        float maxBLepMassPair2 = TMath::Max(massBLeadLep_Pair2, massBSubLep_Pair2);
        bool minMaxBLepMassPair2 = (maxBLepMassPair1 > maxBLepMassPair2);
        
        float absDiffBLepMassPair1 = abs(massBLeadLep_Pair1 - massBSubLep_Pair1);
        float absDiffBLepMassPair2 = abs(massBLeadLep_Pair2 - massBSubLep_Pair2);
        bool minAbsDiffBLepMassPair2 = (absDiffBLepMassPair1 > absDiffBLepMassPair2);
        
        float minAbsDiffBLepMassTopMassPair1 = TMath::Min(abs(massBLeadLep_Pair1 - topMass), abs(massBSubLep_Pair1 - topMass));
        float minAbsDiffBLepMassTopMassPair2 = TMath::Min(abs(massBLeadLep_Pair2 - topMass), abs(massBSubLep_Pair2 - topMass));
        bool minAbsDiffBLepMassTopMassIsPair2 = minAbsDiffBLepMassTopMassPair1 > minAbsDiffBLepMassTopMassPair2;
        float returnMT2lb = -1;
        
        // 0 is minMT2lb, 1 is minMaxMass, 2 is minAbsMassDiff, 3 is AND of 0 and 1, 4 is OR of 0 and 1
        // 5 is AND of 0 and 2, 6 is OR of 0 and 2, 7 is AND of 1 and 2, 8 is OR of 1 and 2, 9 is AND of 0, 1, and 2; 10 is OR of 0, 1, and 2
        vecCorrPairCheckIndex->at(0) = minMT2lbPair2;
        vecCorrPairCheckIndex->at(1) = minMaxBLepMassPair2;
        vecCorrPairCheckIndex->at(2) = minAbsDiffBLepMassPair2;
        vecCorrPairCheckIndex->at(3) = (minMT2lbPair2 && minMaxBLepMassPair2);
        vecCorrPairCheckIndex->at(4) = (minMT2lbPair2 || minMaxBLepMassPair2);
        vecCorrPairCheckIndex->at(5) = (minMT2lbPair2 && minAbsDiffBLepMassPair2);
        vecCorrPairCheckIndex->at(6) = (minMT2lbPair2 || minAbsDiffBLepMassPair2);
        vecCorrPairCheckIndex->at(7) = (minMaxBLepMassPair2 && minAbsDiffBLepMassPair2);
        vecCorrPairCheckIndex->at(8) = (minMaxBLepMassPair2 || minAbsDiffBLepMassPair2);
        vecCorrPairCheckIndex->at(9) = (minMT2lbPair2 && minMaxBLepMassPair2 && minAbsDiffBLepMassPair2);
        vecCorrPairCheckIndex->at(10) = (minMT2lbPair2 || minMaxBLepMassPair2 || minAbsDiffBLepMassPair2);
        vecCorrPairCheckIndex->at(11) = minAbsDiffBLepMassTopMassIsPair2;
        vecCorrPairCheckIndex->at(12) = (minAbsDiffBLepMassTopMassIsPair2 || minAbsDiffBLepMassPair2);
        vecCorrPairCheckIndex->at(13) = (minAbsDiffBLepMassTopMassIsPair2 && minAbsDiffBLepMassPair2);
        vecCorrPairCheckIndex->at(14) = (minAbsDiffBLepMassTopMassIsPair2 || minMT2lbPair2);
        vecCorrPairCheckIndex->at(15) = (minAbsDiffBLepMassTopMassIsPair2 && minMT2lbPair2);
        
        SetValsMT2lbPairing(MT2lbPair1, MT2lbPair2, returnMT2lb, vecBLeps_Pair1, vecBLeps_Pair2, vec_GoodBLeps, vecCorrPairCheckIndex->at(whichCheck));
        return returnMT2lb;
    }
    float MT2lbCalculator(vector<TLorentzVector> * vecLeps, vector<TLorentzVector> * vecJets, float MET, float METPhi, vector<TLorentzVector> &vecBLeps_Good, int whichCheck, bool doVerbosity, vector<int> * inputJetPartFlavor = 0, vector<int> * inputLepPDGIDs = 0) {
        
        vector<int> vecCorrPairCheckIndex;
        vecCorrPairCheckIndex.resize(vecCorrPairValMT2lblb.size());
        
        for (int iCorrPair = 0; iCorrPair < (int) vecCorrPairCheckIndex.size(); ++iCorrPair) {
            vecCorrPairCheckIndex[iCorrPair] = -1;
        }
        
        float returnMT2lb;
        vector<TLorentzVector> vecBLeps_Pair1(0);
        vector<TLorentzVector> vecBLeps_Pair2(0);
        if (vecLeps->size() < 2 || vecJets->size() < 2) {
            cout << "Houston, we've had a problem here: one of the two vectors is less than 2!" << endl;
            cout << "vecLep size " << vecLeps->size() << endl;
            cout << "vecJet size " << vecJets->size() << endl;
        }
        vecBLeps_Pair1.push_back(vecLeps->at(0) + vecJets->at(0));
        vecBLeps_Pair1.push_back(vecLeps->at(1) + vecJets->at(1));
        
        vecBLeps_Pair2.push_back(vecLeps->at(0) + vecJets->at(1));
        vecBLeps_Pair2.push_back(vecLeps->at(1) + vecJets->at(0));
        
        returnMT2lb = MT2lbPairingCheck(&vecBLeps_Pair1, &vecBLeps_Pair2, &vecBLeps_Good, MET, METPhi, &vecCorrPairCheckIndex, whichCheck);
        if (inputJetPartFlavor == NULL || inputLepPDGIDs == NULL) {
            for (int iCorrPair = 0; iCorrPair < (int) vecCorrPairValMT2lblb.size(); ++iCorrPair) {
                vecCorrPairValMT2lblb[iCorrPair] = -2;
            }
        }
        else {
            for (int iCorrPair = 0; iCorrPair < (int) vecCorrPairValMT2lblb.size(); ++iCorrPair) {
                if (doVerbosity) {
                    cout << "setting corr pair check for iCorrPair = " << iCorrPair << endl;
                }
                vecCorrPairValMT2lblb[iCorrPair] = CheckMT2lblbPairing(vecCorrPairCheckIndex[iCorrPair], inputJetPartFlavor, inputLepPDGIDs, doVerbosity);
                if (doVerbosity) {
                    cout << "vecCorrPairValMT2lblb[iCorrPair] " << vecCorrPairValMT2lblb[iCorrPair] << endl;
                }
            }
        }
        return returnMT2lb;
    }
    void CalcMT2lblb(EventLepInfo * inELI, EventJetInfo * inEJI, float EventMET, float EventMETPhi, int whichCheck, int levelVerbosity = 0) {
        // Set the events M_{T2}(lb)(lb)
        vector<TLorentzVector> vecLepMT2lb(2), vecJetMT2lblb(2);
        vector<int> vecJetPartFlavor(2), vecLepPDGID(2);
        caseMT2lblb = inEJI->JetSelectorForMT2(&vecJetMT2lblb, &vecJetPartFlavor, levelVerbosity);
        
        if (vecJetMT2lblb.size() == 2) {
            vecLepMT2lb[0] = inELI->vecEventLeps[0].P4;
            vecLepMT2lb[1] = inELI->vecEventLeps[1].P4;
            vecLepPDGID[0] = inELI->vecEventLeps[0].PDGID;
            vecLepPDGID[1] = inELI->vecEventLeps[1].PDGID;
            if (levelVerbosity > 0) {
                cout << "vecJetMT2lblb[0].Pt() " << vecJetMT2lblb[0].Pt() << endl;
                cout << "vecJetMT2lblb[1].Pt() " << vecJetMT2lblb[1].Pt() << endl;
            }
            
            if (vecJetPartFlavor[0] == -999999 || vecJetPartFlavor[1] == -999999) {
                EventMT2lblb = MT2lbCalculator(&vecLepMT2lb, &vecJetMT2lblb, EventMET, EventMETPhi, EventVecBLepsMT2lb, whichCheck, levelVerbosity);
            }
            else {
                EventMT2lblb = MT2lbCalculator(&vecLepMT2lb, &vecJetMT2lblb, EventMET, EventMETPhi, EventVecBLepsMT2lb, whichCheck, levelVerbosity, &vecJetPartFlavor, &vecLepPDGID);
            }
            
            EventDeltaPhiMT2lblb_JetsUsed = dPhi(vecJetMT2lblb[0].Phi(), vecJetMT2lblb[1].Phi());
            EventDeltaPhiMT2lblb_BLepsUsed = dPhi(EventVecBLepsMT2lb[0].Phi(), EventVecBLepsMT2lb[1].Phi());
            
            EventMassBLep0_BLepsUsed = EventVecBLepsMT2lb[0].M();
            EventMassBLep1_BLepsUsed = EventVecBLepsMT2lb[1].M();
            
        }
        else {
            if (vecJetMT2lblb.size() > 2) {
                cout << "something weird with vecJetMT2lblb size " << vecJetMT2lblb.size() << endl;
            }
            SetBadMT2lblb();
        }
    }
} EventMT2Info;

typedef struct EventMETInfo {
    // Contains information about the MET and MET related variables in the event
    
    TVector3 P3;
    float EventMET, EventMETPhi, EventMETX, EventMETY, EventMETSig, EventSumET;
    float EventBMET, EventBMETPhi;
    EventMT2Info MET_EMT2I;
    EventRecoilInfo MET_ERI;
    float EventMETdivMeff;
    float EventMETdivHT;
    int METType;
    void EMIDefaultVarVals() {
        EventMET = 0.; EventMETPhi = 0.;
        EventMETX = 0.; EventMETY = 0.;
        EventSumET = 0.; EventMETSig = 0.;
        MET_EMT2I.EMT2IDefaultVarVals();
        MET_ERI.ERIDefaultVarVals();
        EventMETdivMeff = 0.;
        METType = -1;
    }
    void PrintVals() {
        std::cout << "EventMET " << EventMET << std::endl;
        std::cout << "EventMETPhi " << EventMETPhi << std::endl;
        std::cout << "EventMETX " << EventMETX << std::endl;
        std::cout << "EventMETY " << EventMETY << std::endl;
        
        std::cout << "EventMETdivMeff " << EventMETdivMeff << std::endl;
    }
    void GrabbedFromTree(int inMETType) {
        P3.SetPtEtaPhi(EventMET, 0., EventMETPhi);
        EventMETX = P3.Px(); EventMETY = P3.Py();
        METType = inMETType;
        MET_ERI.GrabbedFromTree();
    }
    void SetVarsfromVec() {
        EventMET = P3.Pt();
        EventMETPhi = P3.Phi();
        EventMETX = P3.Px();
        EventMETY = P3.Py();
    }
    void SetVarsfromXY() {
        //        cout << "setting stuff " << endl;
        //        cout << "EventMETX " << EventMETX << endl;
        //        cout << "EventMETY " << EventMETY << endl;
        P3.SetXYZ(EventMETX, EventMETY, 0.);
        EventMET = P3.Pt();
        EventMETPhi = P3.Phi();
    }
    void SetVarsfromPtPhi() {
        P3.SetPtEtaPhi(EventMET, 0., EventMETPhi);
        EventMETX = P3.Px(); EventMETY = P3.Py();
    }
    void SetVars(EventMETInfo * inEMI, bool doMore = false) {
        EventMET = inEMI->EventMET;
        EventMETPhi = inEMI->EventMETPhi;
        METType = inEMI->METType;
        if (doMore) {
            EventSumET = inEMI->EventSumET;
            GrabbedFromTree(inEMI->METType);
            MET_ERI = inEMI->MET_ERI;
            MET_EMT2I = inEMI->MET_EMT2I;
        }
    }
    void PhiCorrection(bool doData, int nVtx, bool isSmearMET, bool isOld) {
        SetVarsfromPtPhi();
        //        cout << "EventMETX " << EventMETX << endl;
        //        cout << "EventMETY " << EventMETY << endl;
        if (isSmearMET) {
            MetPhiCorrectSmear(EventMETX, EventMETY, nVtx);
        }
        else {
            if (isOld) {
                MetPhiCorrect_noType0(doData, EventMETX, EventMETY, nVtx);
            }
            else {
                MetPhiCorrect(doData, EventMETX, EventMETY, nVtx, METType);
            }
        }
        //        cout << "EventMETX " << EventMETX << endl;
        //        cout << "EventMETY " << EventMETY << endl;
        SetVarsfromXY();
    }
    void PhiCorrection(BasicEventInfo * inBEI, EventPileUpInfo * inEPUI, bool isSmearMET, bool isOld) {
        //        MetPhiCorrect(METType, inBEI->doData, EventMETX, EventMETY, inEPUI->nVtx);
        PhiCorrection(inBEI->doData, inEPUI->nVtx, isSmearMET, isOld);
        SetVarsfromXY();
    }
    void SystShiftMET(vector<TLorentzVector> * inputObjVec, vector<TLorentzVector> * shiftObjVec, EventMETInfo * inEMI) {
        METSystShift(inputObjVec, shiftObjVec, EventMET, EventMETPhi, inEMI->EventMET, inEMI->EventMETPhi);
    }
    void SystShiftMET(vector<Lepton> * inputLepVec, vector<Lepton> * shiftLepVec, EventMETInfo * inEMI) {
        METSystShift(inputLepVec, shiftLepVec, EventMET, EventMETPhi, inEMI->EventMET, inEMI->EventMETPhi);
    }
    void SystShiftMET(vector<PFJet> * inputJetVec, vector<PFJet> * shiftJetVec, EventMETInfo * inEMI, int doVerbosity = 0) {
        METSystShift(inputJetVec, shiftJetVec, EventMET, EventMETPhi, inEMI->EventMET, inEMI->EventMETPhi, doVerbosity);
    }
    void SetERIBadProbe() {
        MET_ERI.SetRecoilVarsBadProbe();
    }
    void SetERI(TVector3 inProbeVec) {
        SetVarsfromPtPhi();
        MET_ERI.SetRecoilVars(inProbeVec, P3);
    }
    void SetERI(EventLepInfo * inELI) {
        if (inELI->doEvent) {
            TVector3 DiLepVec3 = (inELI->vecEventLeps[0].P4 + inELI->vecEventLeps[1].P4).Vect();
            //            cout << "DiLepVec3.Pt " << DiLepVec3.Pt() << endl;
            SetERI(DiLepVec3);
        }
        else {
            SetERIBadProbe();
        }
    }
    void SetMTLep(EventLepInfo * inELI) {
        if (inELI->doEvent) {
            MET_EMT2I.SetMTLep(&inELI->vecEventLeps[0].P4, EventMET, EventMETPhi, 1);
            MET_EMT2I.SetMTLep(&inELI->vecEventLeps[1].P4, EventMET, EventMETPhi, 2);
        }
        else {
            MET_EMT2I.SetMTBadLep();
        }
    }
    void SetMTJet(EventLepInfo * inELI, EventJetInfo * inEJI) {
        if (inELI->doEvent && inEJI->EventNJets > 0) {
            MET_EMT2I.SetMTJet(&inEJI->EventSumJetP4, EventMET, EventMETPhi);
            //            cout << "MET_EMT2I " << MET_EMT2I.MT_HTVec << endl;
        }
        else {
            MET_EMT2I.SetMTBadJet();
        }
    }
    void SetMTBJet(EventLepInfo * inELI, EventJetInfo * inEJI) {
        if (inELI->doEvent && inEJI->EventNBtagJets > 0) {
            MET_EMT2I.SetMTBJet(&inEJI->EventSumBJetP4, EventMET, EventMETPhi);
        }
        else {
            MET_EMT2I.SetMTBadBJet();
        }
    }
    void CalcMT2ll(EventLepInfo * inELI) {
        if (inELI->doEvent) {
            MET_EMT2I.CalcMT2ll(inELI, EventMET, EventMETPhi);
        }
        else {
            MET_EMT2I.SetBadMT2ll();
        }
    }
    void CalcMT2bb_ZMET(EventLepInfo * inELI, EventJetInfo * inEJI) {
        if (inELI->doEvent) {
            MET_EMT2I.CalcMT2bb_ZMET(inEJI, &MET_ERI);
        }
        else {
            MET_EMT2I.SetBadMT2bb_ZMET();
        }
    }
    void CalcMT2lblb(EventLepInfo * inELI, EventJetInfo * inEJI, int whichCheck, int levelVerbosity = 0) {
        if (inELI->doEvent) {
            MET_EMT2I.CalcMT2lblb(inELI, inEJI, EventMET, EventMETPhi, whichCheck, levelVerbosity);
        }
        else {
            MET_EMT2I.SetBadMT2lblb();
        }
    }
    void CalcBMET(EventJetInfo * inEJI) {
        TVector2 vecMET; vecMET.SetMagPhi(EventMET, EventMETPhi);
        TVector2 vecBJetTrans;
        if (inEJI->EventNBtagJets > 0) {
            vecBJetTrans.SetMagPhi(inEJI->BVC_SumBJet.Vec_Pt, inEJI->BVC_SumBJet.Vec_Phi);
            vecMET += vecBJetTrans;
            EventBMET = vecMET.Mod();
            EventBMETPhi = vecMET.Phi();
        }
        else {
            EventBMET = EventMET;
            EventBMETPhi = EventMETPhi;
        }
    }
    void CalcMETVariations(EventLepInfo * inELI, EventJetInfo * inEJI, int whichCheck, int levelVerbosity = 0) {
        MET_EMT2I.EMT2IDefaultVarVals();
        CalcBMET(inEJI);
        SetERI(inELI);
        SetMTLep(inELI);
        SetMTJet(inELI, inEJI);
        SetMTBJet(inELI, inEJI);
        CalcMT2ll(inELI);
        CalcMT2lblb(inELI, inEJI, whichCheck, levelVerbosity);
        CalcMT2bb_ZMET(inELI, inEJI);
        EventMETdivMeff = EventMET / (inEJI->EventJetST + EventMET + inELI->EventLepST);
        EventMETdivHT = EventMET / (inEJI->EventHT);
    }
} EventMETInfo;

typedef struct EventDiStructureInfo {
    // Contains information about relevant "di-structures", so two "particle" systems
    // information includes the combination vector sum of the chosen "particles"
    // relative angles between them, etc.
    
    DiParticle DP_DiLepton;
    DiParticle DP_DiJet;
    DiParticle DP_DiBJet;
    DiParticle DP_ZMET;
    DiParticle DP_DiBLeps;
    
    vector<DiParticle> vecDP_LepMET;
    
    vector< vector<DiParticle> > vecVecDP_LepJet;
    vector< vector<DiParticle> > vecVecDP_LepBJet;
    vector< vector<DiParticle> > vecVecDP_JetBJet;
    
    int numLeps, numJets, numBJets;
    
    float ELepMinEJet;
    void EDSIDefaultVarVals() {
        TString nameLep[2] = {"Lep0", "Lep1"};
        TString nameJet[2] = {"Jet0", "Jet1"};
        TString nameBJet[2] = {"BJet0", "BJet1"};
        
        DP_DiLepton.DPDefaultVarVals(TString("DiLep"));
        DP_DiJet.DPDefaultVarVals(TString("DiJet"));
        DP_DiBJet.DPDefaultVarVals(TString("DiBJet"));
        DP_DiBLeps.DPDefaultVarVals(TString("DiBLeps"));
        DP_ZMET.DPDefaultVarVals(TString("DiZMET"));
        
        numLeps = 2;
        numJets = 2;
        numBJets = 2;
        
        vecDP_LepMET.resize(numLeps);
        
        vecVecDP_LepJet.resize(numLeps);
        vecVecDP_LepBJet.resize(numLeps);
        vecVecDP_JetBJet.resize(numJets);
        
        for (int iLep = 0; iLep < numLeps; ++iLep) {
            vecDP_LepMET[iLep].DPDefaultVarVals(nameLep[iLep] + "MET");
            vecVecDP_LepJet[iLep].resize(numJets);
            vecVecDP_LepBJet[iLep].resize(numBJets);
            for (int iJet = 0; iJet < numJets; ++iJet) {
                vecVecDP_LepJet[iLep][iJet].DPDefaultVarVals(nameLep[iLep] + nameJet[iJet]);
            }
            for (int iBJet = 0; iBJet < numBJets; ++iBJet) {
                vecVecDP_LepBJet[iLep][iBJet].DPDefaultVarVals(nameLep[iLep] + nameBJet[iBJet]);
            }
        }
        for (int iJet = 0; iJet < numJets; ++iJet) {
            vecVecDP_JetBJet[iJet].resize(numBJets);
            for (int iBJet = 0; iBJet < numBJets; ++iBJet) {
                vecVecDP_JetBJet[iJet][iBJet].DPDefaultVarVals(nameJet[iJet] + nameBJet[iBJet]);
            }
        }
        ELepMinEJet = 0.;
    }
    void SetVars(EventLepInfo * inELI, EventJetInfo * inEJI, EventMETInfo * inEMI, int levelVerbosity = 0) {
        if (levelVerbosity > 0) {
            cout << "setting vecZ " << endl;
        }
        TLorentzVector vecZ = inELI->vecEventLeps[0].P4+inELI->vecEventLeps[1].P4;
        if (levelVerbosity > 0) {
            cout << "setting vecMET " << endl;
        }
        TLorentzVector METP4; METP4.SetPtEtaPhiM(inEMI->EventMET, 0., inEMI->EventMETPhi, 0.);
        if (levelVerbosity > 0) {
            cout << "setting DiLepton DP " << endl;
        }
        DP_DiLepton.SetDPVals(&inELI->vecEventLeps[0].P4, &inELI->vecEventLeps[1].P4);
        if (levelVerbosity > 0) {
            cout << "setting DiJet DP " << endl;
        }
        DP_DiJet.SetDPVals(&inEJI->vecEventJets[0].P4, &inEJI->vecEventJets[1].P4);
        if (levelVerbosity > 0) {
            cout << "setting DiBJet DP " << endl;
        }
        DP_DiBJet.SetDPVals(&inEJI->vecEventBTagJets[0].P4, &inEJI->vecEventBTagJets[1].P4);
        if (inEJI->EventNJets > 1) {
            if (levelVerbosity > 0) {
                cout << "setting DiBLep DP " << endl;
            }
            DP_DiBLeps.SetDPVals(&inEMI->MET_EMT2I.EventVecBLepsMT2lb[0], &inEMI->MET_EMT2I.EventVecBLepsMT2lb[1]);
        }
        if (levelVerbosity > 0) {
            cout << "setting DiZMET DP " << endl;
        }
        DP_ZMET.SetDPVals(&vecZ, &METP4);
        
        for (int iLep = 0; iLep < numLeps; ++iLep) {
            if (levelVerbosity > 0) {
                cout << "setting LepMET DP for iLep = " << iLep << endl;
            }
            vecDP_LepMET[iLep].SetDPVals(&inELI->vecEventLeps[iLep].P4, &METP4);
            for (int iJet = 0; iJet < numJets; ++iJet) {
                if (levelVerbosity > 0) {
                    cout << "setting LepJet DP for iLep = " << iLep << " and iJet = " << iJet << endl;
                }
                vecVecDP_LepJet[iLep][iJet].SetDPVals(&inELI->vecEventLeps[iLep].P4, &inEJI->vecEventJets[iJet].P4);
            }
            for (int iBJet = 0; iBJet < numBJets; ++iBJet) {
                if (levelVerbosity > 0) {
                    cout << "setting LepBJet DP for iLep = " << iLep << " and iBJet = " << iBJet << endl;
                }
                vecVecDP_LepBJet[iLep][iBJet].SetDPVals(&inELI->vecEventLeps[iLep].P4, &inEJI->vecEventBTagJets[iBJet].P4);
            }
        }
        for (int iJet = 0; iJet < numJets; ++iJet) {
            for (int iBJet = 0; iBJet < numBJets; ++iBJet) {
                if (levelVerbosity > 0) {
                    cout << "setting JetBJet DP for iJet = " << iJet << " and iBJet = " << iBJet << endl;
                }
                vecVecDP_JetBJet[iJet][iBJet].SetDPVals(&inEJI->vecEventJets[iJet].P4, &inEJI->vecEventBTagJets[iBJet].P4);
            }
        }
        ELepMinEJet = 0.;
        ELepMinEJet += inELI->vecEventLeps[0].BVC.Vec_En;
        ELepMinEJet += inELI->vecEventLeps[1].BVC.Vec_En;
        if (inEJI->EventNJets > 0) {
            ELepMinEJet += -1 * inEJI->vecEventJets[0].BVC.Vec_En;
            if (inEJI->EventNJets > 1) {
                ELepMinEJet += -1 * inEJI->vecEventJets[1].BVC.Vec_En;
            }
        }
    }
    void SetMETVars(EventLepInfo * inELI, EventMETInfo * inEMI) {
        for (int iLep = 0; iLep < numLeps; ++iLep) {
            vecDP_LepMET[iLep].DPhiInputObjects = dPhi(inELI->vecEventLeps[iLep].BVC.Vec_Phi, inEMI->EventMETPhi);
        }
        DP_ZMET.DPhiInputObjects = dPhi(DP_DiLepton.BVC.Vec_Phi, inEMI->EventMETPhi);
    }
    void SetDiLepVals(EventDiStructureInfo * inEDSI) {
        DP_DiLepton = inEDSI->DP_DiLepton;
    }
} EventDiStructureInfo;

typedef struct FilterTriggerInfo {
    // Contains MET filter and lepton trigger information
    
    bool filterECalDeadCell, filterLogErrorTooManyClusters, filterTrackingFailure, filterHCalLaser, filterECalLaserCorr, filterTooManyStripClus, filterManyStripClus, filterEEBadSC;
    vector<bool> regularMETFilterVec;
    vector<bool> oppositeMETFilterVec;
    
    void SetVecs() {
        regularMETFilterVec.resize(0);
        oppositeMETFilterVec.resize(0);
        regularMETFilterVec.push_back(filterECalDeadCell);
        regularMETFilterVec.push_back(filterTrackingFailure);
        regularMETFilterVec.push_back(filterHCalLaser);
        regularMETFilterVec.push_back(filterECalLaserCorr);
        regularMETFilterVec.push_back(filterEEBadSC);
        
        oppositeMETFilterVec.push_back(filterLogErrorTooManyClusters);
        oppositeMETFilterVec.push_back(filterTooManyStripClus);
        oppositeMETFilterVec.push_back(filterManyStripClus);
    }
    
    bool passTrigDoubleEl, passTrigDoubleMu, passTrigElMu;
    
    bool EventPassTrigger(EventLepInfo * inELI) {
        bool stillDoEvent = false;
        if (inELI->doEvent) {
            switch (inELI->EventDiLepType) {
                case 0:
                    stillDoEvent = passTrigDoubleMu;
                    break;
                case 1:
                    stillDoEvent = passTrigDoubleEl;
                    break;
                case 2:
                    stillDoEvent = passTrigElMu;
                    break;
                default:
                    cout << "oddity with EventDiLepType for input ELI" << endl;
                    break;
            }
        }
        return stillDoEvent;
    }
} FilterTriggerInfo;

#include <unordered_map>
typedef unordered_map<int, BasicEventInfo *> mapIntBEI;
typedef unordered_map<int, EventMETInfo *> mapIntEMI;
typedef unordered_map<int, EventLepInfo *> mapIntELI;
typedef unordered_map<int, EventJetInfo *> mapIntEJI;
typedef unordered_map<int, EventDiStructureInfo *> mapIntEDSI;
typedef unordered_map<int, bool> mIB;

typedef struct EventStructPointerInfo {
    // Used as part of filling histograms
    // Event by event contains pointers to the correct Reco Info structs -- all defined above -- that
    // should be used based on the given systematic
    
    BasicEventInfo * addBEI;
    EventLepInfo * addELI;
    EventMETInfo * addEMI;
    EventJetInfo * addEJI;
    EventDiStructureInfo * addEDSI;
    void SetBEI(int iSyst, mapIntBEI * inMapIntBEI) {
        mapIntBEI::iterator xIterBEI;
        xIterBEI = inMapIntBEI->find(iSyst);
        if (xIterBEI != inMapIntBEI->end()) {
            addBEI = xIterBEI->second;
        }
        else {
            cout << "something wiggedy: not in the map, hombre!!" << endl;
        }
    }
    void SetEMI(int iSyst, mapIntEMI * inMapIntEMI) {
        mapIntEMI::iterator xIterEMI;
        xIterEMI = inMapIntEMI->find(iSyst);
        if (xIterEMI != inMapIntEMI->end()) {
            addEMI = xIterEMI->second;
        }
        else {
            cout << "something wiggedy: not in the map, hombre!!" << endl;
        }
    }
    void SetEJI(int iSyst, mapIntEJI * inMapIntEJI) {
        mapIntEJI::iterator xIterEJI;
        xIterEJI = inMapIntEJI->find(iSyst);
        if (xIterEJI != inMapIntEJI->end()) {
            addEJI = xIterEJI->second;
        }
        else {
            cout << "something wiggedy: not in the map, hombre!!" << endl;
        }
    }
    void SetELI(int iSyst, mapIntELI * inMapIntELI) {
        mapIntELI::iterator xIterELI;
        xIterELI = inMapIntELI->find(iSyst);
        if (xIterELI != inMapIntELI->end()) {
            addELI = xIterELI->second;
        }
        else {
            cout << "something wiggedy: not in the map, hombre!!" << endl;
        }
    }
    void SetEDSI(int iSyst, mapIntEDSI * inMapIntEDSI) {
        mapIntEDSI::iterator xIterEDSI;
        xIterEDSI = inMapIntEDSI->find(iSyst);
        if (xIterEDSI != inMapIntEDSI->end()) {
            addEDSI = xIterEDSI->second;
        }
        else {
            cout << "something wiggedy: not in the map, hombre!!" << endl;
        }
    }
    void SetStructs(int iSyst, mapIntBEI * inMapIntBEI, mapIntEMI * inMapIntEMI, mapIntEJI * inMapIntEJI, mapIntELI * inMapIntELI, mapIntEDSI * inMapIntEDSI, bool doVerbosity = false) {
        if (doVerbosity) cout << "trying to set BEI " << endl;
        SetBEI(iSyst, inMapIntBEI);
        if (doVerbosity) cout << "trying to set EMI " << endl;
        SetEMI(iSyst, inMapIntEMI);
        if (doVerbosity) cout << "trying to set EJI " << endl;
        SetEJI(iSyst, inMapIntEJI);
        if (doVerbosity) cout << "trying to set ELI " << endl;
        SetELI(iSyst, inMapIntELI);
        if (doVerbosity) cout << "trying to set EDSI " << endl;
        SetEDSI(iSyst, inMapIntEDSI);
    }
    void ZeroPointers() {
        addBEI = NULL;
        addELI = NULL;
        addEMI = NULL;
        addEJI = NULL;
        addEDSI = NULL;
    }
} EventStructPointerInfo;

