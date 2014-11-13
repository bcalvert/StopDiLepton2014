// Structs to contain sets of particle pointers (pointers to vectors of information) present in the nTuples at Oviedo skim level
// should be more clear what I mean by that if you look at an individual one

using namespace std;
typedef struct PFJetEventPointers {
    // pointers for the PF jets
    
    std::vector<float> * JetPx, * JetPy, * JetPz, * JetEn, * JetNHF, * JetNEF, * JetCHF, * JetCEF, * JetBTag;
    std::vector<int> * JetNDaug, * JetCharMult, * JetPartFlav;
    unsigned int numPFJets;
    //    std::vector<float> * genJetPx, * genJetPy, * genJetPz, * genJetEn, * genJetInvisE; //* genJetEt, * genJetEta;
    //    std::vector<bool> * genJetIsGenJet;
    void InitializeVecs() {
        JetPx = new vector<float>; 
        JetPy = new vector<float>;
        JetPz = new vector<float>; 
        JetEn = new vector<float>;
        JetNHF = new vector<float>; 
        JetNEF = new vector<float>;
        JetCHF = new vector<float>; 
        JetCEF = new vector<float>;
        JetBTag = new vector<float>;
        JetPartFlav = new vector<int>;        
        JetNDaug = new vector<int>; 
        JetCharMult = new vector<int>;
    }
    void DeleteVecs() {
        delete JetPx;
        delete JetPy;
        delete JetPz;
        delete JetEn;
        delete JetNHF;
        delete JetNEF;
        delete JetCHF;
        delete JetCEF;
        delete JetBTag;
        delete JetPartFlav;
        delete JetNDaug;
        delete JetCharMult;
    }   
    void SetSize() {
        numPFJets = JetEn->size();
    }
    void SetPFJet(int iJet, PFJet * inPFJet) {
        // set the characteristics of inPFJet based on the vector values at index iJet
        
        //Set LorentzVector
        inPFJet->P4.SetPxPyPzE(JetPx->at(iJet), JetPy->at(iJet), JetPz->at(iJet), JetEn->at(iJet));
        // Set BTagging info
        inPFJet->valBTagDisc  = JetBTag->at(iJet);
        inPFJet->partonFlavor = JetPartFlav->at(iJet);
        
        //Set Jet quality cut variables
        inPFJet->NeutHadFrac   = JetNHF->at(iJet);
        inPFJet->ChargeHadFrac = JetCHF->at(iJet);
        inPFJet->NeutEMFrac    = JetNEF->at(iJet);
        inPFJet->ChargeEMFrac  = JetCEF->at(iJet);
        inPFJet->NumDaughters  = JetNDaug->at(iJet);
        inPFJet->ChargeMult    = JetCharMult->at(iJet);
    }
} PFJetEventPointers;

typedef struct MuonEventPointers {
    // contains variables and functions related to the pointers relevant for our muons
    
    std::vector<float> * MuonPt, * MuonPx, * MuonPy, * MuonPz, * MuonEn, * MuonPFCharHadIso, * MuonPFNeutHadIso, * MuonPFPhotIso, * MuonSumPUPt, * MuonD0, * MuonVertZ;
    std::vector<float> * PFMuonPt;
    std::vector<bool> * isPFMuon, * isGMPTMuon, * isGlobMuon; //, * isTrackArbitMuon;
    std::vector<int> * MuonNumMatchStations, * MuonNumLayers, * MuonNumValidPixHitsinTrack;
    std::vector<int> * MuonCharge;
    unsigned int numMuons;
    //ambiguity in which Muon Pt...rolling with PFMuonPt...never mind ambiguity resolved see https://twiki.cern.ch/twiki/bin/viewauth/CMS/SingleLepton2012#Muon_Selection
    void InitializeVecs() {
        MuonPt = new vector<float>;
        MuonPx = new vector<float>; 
        MuonPy = new vector<float>; 
        MuonPz = new vector<float>; 
        MuonEn = new vector<float>;
        MuonCharge = new vector<int>;
        MuonPFCharHadIso = new vector<float>; 
        MuonPFNeutHadIso = new vector<float>; 
        MuonPFPhotIso = new vector<float>; 
        MuonSumPUPt = new vector<float>;
        MuonD0 = new vector<float>; 
        MuonVertZ = new vector<float>;
        PFMuonPt = new vector<float>;
        
        isPFMuon = new vector<bool>; 
        isGMPTMuon = new vector<bool>; 
        isGlobMuon = new vector<bool>; 
        //isTrackArbitMuon = new vector<bool>;
        
        MuonNumMatchStations = new vector<int>; 
        MuonNumLayers = new vector<int>;
        MuonNumValidPixHitsinTrack = new vector<int>;
    }
    void SetMuonP4(int whichMu, TLorentzVector * inP4) {
        inP4->SetPxPyPzE(MuonPx->at(whichMu), MuonPy->at(whichMu), MuonPz->at(whichMu), MuonEn->at(whichMu));
    }
    bool MuonPassCutStage0_MEP(float relPFMuonIso, int cutQualityMu = 1, bool doVerb = false) {    
        // function that says if muon passes cuts we put on it
        
        bool muonPassCut;
        float muonIsoRatioCut = cutQualityMu >= 1 ? 0.15 : 0.5;
        if (!(relPFMuonIso < muonIsoRatioCut)) {
            muonPassCut = false;
        }
        else {
            muonPassCut = true;
        }
        if (doVerb) {
            cout << "muonIsoRatio Cut " << muonIsoRatioCut << endl;
            cout << "muon passes cut? " << muonPassCut << endl;
        }
        return muonPassCut;
    }
    bool MuonPassCutStage1_MEP(int whichMu, float muonEta) {    
        // function that says if muon passes cuts we put on it
                        
        bool muonPassCut;
        float muonEtaCut = 2.4;
        
        if (!isGMPTMuon->at(whichMu) || !isPFMuon->at(whichMu) || !isGlobMuon->at(whichMu) || !(fabs(muonEta) < muonEtaCut)) {
            muonPassCut = false;
        }
        else {
            muonPassCut = true;
        }
        return muonPassCut;
    }
    bool MuonPassCutStage2_MEP(float muonD0, float muonDZ, int muonNumMatchStations, int muonNumValidPixHitsinTrack, int muonNumLayers) {
        // function that says if muon passes cuts we put on it
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
    void SetMuon(int whichMu, Lepton * inMu, float firstGoodVertZ, bool keepLoose = false, int levelVerbosity = 0, int whichSystCase = 0) {
        TLorentzVector patsyVec;        
        int currMuonNumMatchStations, currMuonNumValidPixHitsinTrack, currMuonNumLayers;
        float currMuPt, currMuonEta;
        float currMuonRelPFIso;
        float currMuonDZ, currMuonD0;
        bool  currMuonPassCutTight, currMuonPassCutLoose = false;
        
        if (whichMu >= (int) numMuons) {
            cout << "trying to grab a muon outside of range! " << endl;
            inMu->lepQuality = -1;
            return;
        }
        SetMuonP4(whichMu, &patsyVec);
        currMuPt = patsyVec.Pt();
        if (levelVerbosity > 0) {
            cout << "muon " << whichMu << " has:" << endl;
            cout << "pT with no scaling is " << currMuPt << endl;
            cout << "Eta is " << patsyVec.Eta() << endl;
            cout << "Phi is " << patsyVec.Phi() << endl;
            cout << "PF pT is " << PFMuonPt->at(whichMu) << endl;
        }
        if (fabs(MuonPt->at(whichMu) - PFMuonPt->at(whichMu)) >= 5.) {
            inMu->lepQuality = -1;
            return;
        }
        if (whichSystCase != 0) {
            patsyVec = LeptonScaleSystShift(patsyVec, 13, whichSystCase);            
            currMuPt = patsyVec.Pt();
        }
        currMuonEta = patsyVec.Eta();
        currMuonRelPFIso = (MuonPFCharHadIso->at(whichMu) + TMath::Max(0., MuonPFPhotIso->at(whichMu) + MuonPFNeutHadIso->at(whichMu) - 0.5 * MuonSumPUPt->at(whichMu)))/currMuPt;
        //        currMuonRelPFIso /= currMuPt;
        currMuonDZ = fabs(firstGoodVertZ - MuonVertZ->at(whichMu));        
        currMuonD0 = MuonD0->at(whichMu);
        currMuonNumMatchStations = MuonNumMatchStations->at(whichMu);
        currMuonNumLayers = MuonNumLayers->at(whichMu);
        currMuonNumValidPixHitsinTrack = MuonNumValidPixHitsinTrack->at(whichMu);
        
        if (levelVerbosity > 0) {
            cout << "continuing for muon " << whichMu << endl;
            cout << "currMuonRelPFIso " << currMuonRelPFIso << endl;
            cout << "currMuonDZ " << currMuonDZ << endl;
            cout << "^^ coming from firstGoodVertZ = " << firstGoodVertZ << " and MuonVertZ->at(i) = " << MuonVertZ->at(whichMu) << endl;
            cout << "currMuonD0 " << currMuonD0 << endl;
            cout << "currMuonNumMatchStations " << currMuonNumMatchStations << endl;
            cout << "currMuonNumLayers " << currMuonNumLayers << endl;
            cout << "currMuonNumValidPixHitsinTrack " << currMuonNumValidPixHitsinTrack << endl;
            cout << "isGMPTMuon->at(i) " << isGMPTMuon->at(whichMu) << endl;
            cout << "isPFMuon->at(i) " << isPFMuon->at(whichMu) << endl;
            cout << "isGlobMuon->at(i) " << isGlobMuon->at(whichMu) << endl;
        }
        if (keepLoose) {
            currMuonPassCutLoose = MuonPassCutStage0_MEP(currMuonRelPFIso, 0, levelVerbosity);
            currMuonPassCutLoose &= MuonPassCutStage1_MEP(whichMu, currMuonEta);
            currMuonPassCutLoose &= MuonPassCutStage2_MEP(currMuonD0, currMuonDZ, currMuonNumMatchStations, currMuonNumValidPixHitsinTrack, currMuonNumLayers);
        }
        currMuonPassCutTight = MuonPassCutStage0_MEP(currMuonRelPFIso, 1, levelVerbosity);
        if (levelVerbosity > 0) {
            cout << "continuing for muon " << whichMu << endl;
            cout << "pass stage 0? " << currMuonPassCutTight << endl;
        }
        currMuonPassCutTight &= MuonPassCutStage1_MEP(whichMu, currMuonEta);
        if (levelVerbosity > 0) {
            cout << "continuing for muon " << whichMu << endl;
            cout << "pass stage 1? " << currMuonPassCutTight << endl;
        }
        currMuonPassCutTight &= MuonPassCutStage2_MEP(currMuonD0, currMuonDZ, currMuonNumMatchStations, currMuonNumValidPixHitsinTrack, currMuonNumLayers);
        if (levelVerbosity > 0) {
            cout << "continuing for muon " << whichMu << endl;
            cout << "pass stage 1 & 2? " << currMuonPassCutTight << endl;
        }
        if (currMuonPassCutTight){
            inMu->P4 = patsyVec;
            inMu->PDGID = MuonCharge->at(whichMu) > 0 ? -13 : 13;
            inMu->relPFLepIso = currMuonRelPFIso;
            inMu->BVC.ExtractParams(&inMu->P4);
            inMu->lepQuality = 1;
        }
        else {
            if (keepLoose && currMuonPassCutLoose) {
                inMu->P4 = patsyVec;
                inMu->PDGID = MuonCharge->at(whichMu) > 0 ? -13 : 13;
                inMu->relPFLepIso = currMuonRelPFIso;
                inMu->BVC.ExtractParams(&inMu->P4);
                inMu->lepQuality = 0;                
            }
            else {
                inMu->lepQuality = -1;
            }
            return;
        }        
    }
    
    void DeleteVecs() {
        delete MuonPt;
        delete MuonPx;
        delete MuonPy;
        delete MuonPz;
        delete MuonEn;
        delete MuonCharge;
        delete MuonPFCharHadIso;
        delete MuonPFNeutHadIso;
        delete MuonPFPhotIso;
        delete MuonSumPUPt;
        delete MuonD0;
        delete MuonVertZ;
        delete PFMuonPt;
        
        delete isPFMuon;
        delete isGMPTMuon;
        delete isGlobMuon;
        //delete isTrackArbitMuon;
        
        delete MuonNumMatchStations;
        delete MuonNumLayers;
        delete MuonNumValidPixHitsinTrack;
    }                
    void SetSize() {
        numMuons = MuonEn->size();
    }
} MuonEventPointers;

typedef struct ElectronEventPointers {
    // contains variables and functions related to the pointers relevant for our electrons
    
    std::vector<float> * ElecPt, * ElecPx, * ElecPy, * ElecPz, * ElecEn, * ElecPFCharHadIso, * ElecPFNeutHadIso, * ElecPFPhotIso; 
    std::vector<float> * PFElecPt;
    std::vector<float> * ElecSCEta, * ElecDeltaPhiIn, * ElecDeltaEtaIn, * ElecSigIetaIeta, * ElecHtoERatio, * ElecIP, * ElecDZ, * ElecECalE, * ElecSCEOverP;
    std::vector<int> * ElecNumMissHits;
    std::vector<bool> * ElecisEB, * ElecisEE;
    std::vector<int> * ElecCharge;
    //ambiguity in which Electron Pt...rolling with PFElecPt...never mind ambiguity resolved    
    std::vector<bool> * isPFElectron, * passConvVeto;
    unsigned int numElectrons;
    
    void InitializeVecs() {
        ElecPt = new vector<float>;
        ElecPx = new vector<float>; 
        ElecPy = new vector<float>;
        ElecPz = new vector<float>; 
        ElecEn = new vector<float>;
        ElecCharge = new vector<int>;
        ElecPFCharHadIso = new vector<float>; 
        ElecPFNeutHadIso = new vector<float>;
        ElecPFPhotIso = new vector<float>;
        PFElecPt = new vector<float>;
        
        ElecSCEta = new vector<float>; 
        ElecSCEOverP = new vector<float>; 
        ElecECalE = new vector<float>;
        ElecDeltaPhiIn = new vector<float>; 
        ElecDeltaEtaIn = new vector<float>;
        ElecSigIetaIeta = new vector<float>; 
        ElecHtoERatio = new vector<float>;
        ElecIP = new vector<float>; 
        ElecDZ = new vector<float>;
        ElecNumMissHits = new vector<int>;
        isPFElectron = new vector<bool>; 
        passConvVeto = new vector<bool>;
        ElecisEB = new vector<bool>; 
        ElecisEE = new vector<bool>;
    }
    void SetElecP4(int whichEl, TLorentzVector * inP4) {
        inP4->SetPxPyPzE(ElecPx->at(whichEl), ElecPy->at(whichEl), ElecPz->at(whichEl), ElecEn->at(whichEl));
    }
    int ElectronPassCutStage0_EEPs(int whichEl, bool doVerb = false) {
        
        float elecSCEtaMax = 2.5, elecSCEtaBarrelCut = 1.4442, elecSCEtaEndcapCut = 1.566;
        float currElecSCEta = ElecSCEta->at(whichEl);
        
        int indexToUse = -99;
        
        if (doVerb) {
            cout << "currElecSCEta " << currElecSCEta << endl;
            cout << "ElecisEB? " << ElecisEB->at(whichEl) << endl;
            cout << "ElecisEE? " << ElecisEE->at(whichEl) << endl;
        }        
        if (fabs(currElecSCEta) >= elecSCEtaMax || ((fabs(currElecSCEta) >= elecSCEtaBarrelCut ) && (fabs(currElecSCEta) <= elecSCEtaEndcapCut))) {
            indexToUse = -99;
            return indexToUse;
        }        
        
        if (ElecisEB->at(whichEl)) {
            indexToUse = 0;
        }
        else if (ElecisEE->at(whichEl)) {
            indexToUse = 1;
        }
        else {
            indexToUse = -99;
            return indexToUse;
        }
                
        int elecNumMissHitsCut = 1;        
        if (doVerb) {
            cout << "isPFElec? " << isPFElectron->at(whichEl) << endl;
            cout << "passConvVeto? " << passConvVeto->at(whichEl) << endl;
            cout << "numMissHits? " << ElecNumMissHits->at(whichEl) << endl;
        }
        if (!isPFElectron->at(whichEl) || !passConvVeto->at(whichEl) || ElecNumMissHits->at(whichEl) > elecNumMissHitsCut) {            
            indexToUse = -99;
            return indexToUse;
        }        
        return indexToUse;
    }
    void SetElec(int whichEl, Lepton * inEl, float eventRhoIso, ElecCutInfo * inECITight, bool keepLoose = false, ElecCutInfo * inECILoose = 0, int levelVerbosity = 0, int whichSystCase = 0) {
        TLorentzVector patsyVec;        
        bool currElecPassCutTight, currElecPassCutLoose = false;
        float currElecPt;
        float currElecRelPFElecIso;
        float currElecECalE, currElecSCEOverP;
        float currElecIP, currElecDZ;
        float currElecDeltaPhiIn, currElecDeltaEtaIn, currElecSigIetaIeta, currElecHtoERatio;        
        float currElecSCEta;
        int currElecNumMissHits;
        
        if (whichEl >= (int) numElectrons) {
            cout << "trying to grab a electron outside of range! " << endl;
            inEl->lepQuality = -1;
            return;
        }
        SetElecP4(whichEl, &patsyVec);
        currElecPt = patsyVec.Pt();
        if (levelVerbosity > 0) {
            cout << "electron " << whichEl << " has:" << endl;
            cout << "pT is " << patsyVec.Pt() << endl;
            cout << "Eta is " << patsyVec.Eta() << endl;
            cout << "Phi is " << patsyVec.Phi() << endl;
            cout << "PF pT is " << PFElecPt->at(whichEl) << endl;
        }
        
        currElecPt = patsyVec.Pt();
        if (fabs(currElecPt - PFElecPt->at(whichEl)) >= 10) {
            inEl->lepQuality = -1;
            return;
        }
        if (whichSystCase != 0) {
            patsyVec = LeptonScaleSystShift(patsyVec, 11, whichSystCase);            
            currElecPt = patsyVec.Pt();
        }
        
        int indexToUse = ElectronPassCutStage0_EEPs(whichEl, levelVerbosity);
        if (levelVerbosity > 0){
            cout << "indexToUse " << indexToUse << endl;
        }
        if (indexToUse < 0) {
            inEl->lepQuality = -1;
            return;
        }
        
        currElecECalE = ElecECalE->at(whichEl); 
        currElecSCEOverP = ElecSCEOverP->at(whichEl);        
        currElecSCEta = ElecSCEta->at(whichEl);
        currElecRelPFElecIso = (ElecPFCharHadIso->at(whichEl) + TMath::Max(0., ElecPFNeutHadIso->at(whichEl) +  ElecPFPhotIso->at(whichEl) - (eventRhoIso * ElectronEffectiveArea(currElecSCEta))))/currElecPt;
        currElecDZ = ElecDZ->at(whichEl); 
        currElecIP = ElecIP->at(whichEl);
        currElecDeltaPhiIn = ElecDeltaPhiIn->at(whichEl); 
        currElecDeltaEtaIn = ElecDeltaEtaIn->at(whichEl);
        currElecSigIetaIeta = ElecSigIetaIeta->at(whichEl); 
        currElecHtoERatio = ElecHtoERatio->at(whichEl);
        currElecNumMissHits = ElecNumMissHits->at(whichEl); 
        
        if (levelVerbosity > 0) {
            cout << "continuing for electron " << whichEl << endl;
            cout << "currElecECalE " << currElecECalE << endl;
            cout << "currElecSCEOverP " << currElecSCEOverP << endl;
            cout << "currElecRelPFElecIso " << currElecRelPFElecIso << endl;
            cout << "currElecDZ " << currElecDZ << endl;
            cout << "currElecIP " << currElecIP << endl;
            cout << "currElecDeltaPhiIn " << currElecDeltaPhiIn << endl;
            cout << "currElecDeltaEtaIn " << currElecDeltaEtaIn << endl;
            cout << "currElecSigIetaIeta " << currElecSigIetaIeta << endl;
            cout << "currElecHtoERatio " << currElecHtoERatio << endl;
            cout << "currElecNumMissHits " << currElecNumMissHits << endl;
        }
        
        if (keepLoose) {
            currElecPassCutLoose = inECILoose->PassStage1(indexToUse, currElecDeltaEtaIn, currElecDeltaPhiIn, currElecSigIetaIeta, currElecHtoERatio);
            currElecPassCutLoose &= inECILoose->PassStage2(indexToUse, currElecRelPFElecIso, currElecIP, currElecDZ, currElecSCEOverP, currElecECalE);
        }
        currElecPassCutTight = inECITight->PassStage1(indexToUse, currElecDeltaEtaIn, currElecDeltaPhiIn, currElecSigIetaIeta, currElecHtoERatio);
        if (levelVerbosity > 0) {
            cout << "continuing for electron " << whichEl << endl;
            cout << "pass stage 1? " << currElecPassCutTight << endl;
        }
        currElecPassCutTight &= inECITight->PassStage2(indexToUse, currElecRelPFElecIso, currElecIP, currElecDZ, currElecSCEOverP, currElecECalE);
        if (levelVerbosity > 0) {
            cout << "continuing for electron " << whichEl << endl;
            cout << "pass stage 1 & 2? " << currElecPassCutTight << endl;
        }
        if (currElecPassCutTight){
            inEl->P4 = patsyVec;
            inEl->PDGID = ElecCharge->at(whichEl) > 0 ? -11 : 11;
            inEl->relPFLepIso = currElecRelPFElecIso;
            inEl->BVC.ExtractParams(&inEl->P4);
            inEl->lepQuality = 1;
        }
        else {
            if (keepLoose && currElecPassCutLoose) {
                inEl->P4 = patsyVec;
                inEl->PDGID = ElecCharge->at(whichEl) > 0 ? -11 : 11;
                inEl->relPFLepIso = currElecRelPFElecIso;
                inEl->BVC.ExtractParams(&inEl->P4);
                inEl->lepQuality = 0;
            }
            else {
                inEl->lepQuality = -1;
            }
            return;
        } 
    }
    
    void DeleteVecs() {
        delete ElecPt;
        delete ElecPx;
        delete ElecPy;
        delete ElecPz;
        delete ElecEn;
        delete ElecCharge;
        delete ElecPFCharHadIso;
        delete ElecPFNeutHadIso;
        delete ElecPFPhotIso;
        delete PFElecPt;
        
        delete ElecSCEta;
        delete ElecSCEOverP;
        delete ElecECalE;
        delete ElecDeltaPhiIn;
        delete ElecDeltaEtaIn;
        delete ElecSigIetaIeta;
        delete ElecHtoERatio;
        delete ElecIP;
        delete ElecDZ;
        delete ElecNumMissHits;
        delete isPFElectron;
        delete passConvVeto;
        delete ElecisEB;
        delete ElecisEE;
    }
    
    void SetSize() {
        numElectrons = ElecEn->size();
    }
} ElectronEventPointers;

typedef struct GenParticlePointers_St3 {
    // contains variables and functions related to the pointers relevant for our Status 3 Gen Particles
    
    vector<float> * genPartSt3En, * genPartSt3Pt, * genPartSt3Eta, * genPartSt3Phi, * genPartSt3_firstMotherPt,;
    vector<int> * genPartSt3_i, * genPartSt3_firstMom, * genPartSt3_firstMotherPDGid,  * genPartSt3_pdgId;
    unsigned int numGenPartSt3;
    void InitializeVecs() {
        genPartSt3En    = new vector<float>;
        genPartSt3Eta   = new vector<float>;
        genPartSt3Phi   = new vector<float>;
        genPartSt3Pt    = new vector<float>;                
        genPartSt3_i     = new vector<int>;
        genPartSt3_pdgId = new vector<int>;
        
        genPartSt3_firstMotherPt = new vector<float>;
        genPartSt3_firstMom         = new vector<int>;
        genPartSt3_firstMotherPDGid = new vector<int>;
    }
    void DeleteVecs() {
        delete genPartSt3En;
        delete genPartSt3Eta;
        delete genPartSt3Phi;
        delete genPartSt3Pt;
        delete genPartSt3_i;
        delete genPartSt3_pdgId;
        
        delete genPartSt3_firstMotherPt;
        delete genPartSt3_firstMom;
        delete genPartSt3_firstMotherPDGid;
    }
    void SetSize() {
        numGenPartSt3 = genPartSt3En->size();
    }
    void NoGenParticle(GenParticle * inGenPart) {
        inGenPart->P4.SetPxPyPzE(-1., -1., -1., -1.);
        inGenPart->PDGID = -1;
        inGenPart->PDGStatus = -1;
        inGenPart->Mass = -1;
    }
    void DebugGrabFailure(int indexGenParticle) {
        cout << "issue with the attempted grab index compared with number of available genParticles of this particular kind " << endl;
        cout << "attempted grab index " << indexGenParticle << endl;
        cout << "number of available genParticles " << numGenPartSt3 << endl;
    }
    void SetGenParticleInfo(int indexGenParticle, GenParticle * inGenPart) {
        if (indexGenParticle >= (int) numGenPartSt3) {
            //DebugGrabFailure(indexGenParticle);
            inGenPart->ClearVals();
        }
        else {
            inGenPart->P4.SetPtEtaPhiE(genPartSt3Pt->at(indexGenParticle), genPartSt3Eta->at(indexGenParticle), genPartSt3Phi->at(indexGenParticle), genPartSt3En->at(indexGenParticle));
            inGenPart->PDGID = genPartSt3_pdgId->at(indexGenParticle);
            inGenPart->PDGStatus = 3;
            inGenPart->Mass = inGenPart->P4.M();
        }
    }
    
    void SetGenParticleInfo_Full(int indexGenParticle, GenParticleSt3Info * inGPISt3) {
        if (indexGenParticle >= (int) numGenPartSt3) {
            //DebugGrabFailure(indexGenParticle);
            inGPISt3->BadParticle();
        }
        else {
            SetGenParticleInfo(indexGenParticle, &inGPISt3->GP);
            inGPISt3->ExtractP4Params();
            //inGPISt3->ExtractNonKinParams();
            inGPISt3->GP_Index = genPartSt3_i->at(indexGenParticle);
            inGPISt3->GP_FirstMom = genPartSt3_firstMom->at(indexGenParticle);
        }
    }
    void SetBranchAddresses(TTree * inTree, TString baseName) {
        inTree->SetBranchAddress( baseName + TString("St3_pdgId"),       &genPartSt3_pdgId );
        inTree->SetBranchAddress( baseName + TString("St3_i"),           &genPartSt3_i  );
        inTree->SetBranchAddress( baseName + TString("St3_energy"),      &genPartSt3En );
        inTree->SetBranchAddress( baseName + TString("St3_pt"),          &genPartSt3Pt );
        inTree->SetBranchAddress( baseName + TString("St3_eta"),         &genPartSt3Eta );
        inTree->SetBranchAddress( baseName + TString("St3_phi"),         &genPartSt3Phi );        
        inTree->SetBranchAddress( baseName + TString("St3_firstMother"),        &genPartSt3_firstMom );
        inTree->SetBranchAddress( baseName + TString("St3_firstMotherPDGid"),   &genPartSt3_firstMotherPDGid );
        inTree->SetBranchAddress( baseName + TString("St3_firstMotherPt"),      &genPartSt3_firstMotherPt );
    }
} GenParticlePointers_St3;
typedef struct GenParticlePointers_St1 {
        // contains variables and functions related to the pointers relevant for our Status 1 Gen Particles
    vector<float> * genPartSt1En, * genPartSt1Px, * genPartSt1Py, * genPartSt1Pz;
    vector<float> * genPartSt1MomEn, * genPartSt1MomPx, * genPartSt1MomPy, * genPartSt1MomPz;
    vector<int> * genPartSt1MomPID, * genPartSt1_pdgId, * genPartSt1MomStatus;
    unsigned int numGenPartSt1;
    void InitializeVecs() {
        genPartSt1En = new vector<float>;
        genPartSt1Px = new vector<float>;
        genPartSt1Py = new vector<float>;
        genPartSt1Pz = new vector<float>;
        genPartSt1_pdgId = new vector<int>;
        genPartSt1MomEn = new vector<float>;
        genPartSt1MomPx = new vector<float>;
        genPartSt1MomPy = new vector<float>;
        genPartSt1MomPz = new vector<float>;
        genPartSt1MomPID = new vector<int>;
        genPartSt1MomStatus = new vector<int>;   
    }
    void DeleteVecs() {
        delete genPartSt1En;
        delete genPartSt1Px;
        delete genPartSt1Py;
        delete genPartSt1Pz;
        delete genPartSt1_pdgId;
        delete genPartSt1MomEn;
        delete genPartSt1MomPx;
        delete genPartSt1MomPy;
        delete genPartSt1MomPz;
        delete genPartSt1MomPID;
        delete genPartSt1MomStatus;
    }
    void SetSize() {
        numGenPartSt1 = genPartSt1En->size();
    }
    void DebugGrabFailure(int indexGenParticle) {
        cout << "issue with the attempted grab index compared with number of available genParticles of this particular kind " << endl;
        cout << "attempted grab index " << indexGenParticle << endl;
        cout << "number of available genParticles " << numGenPartSt1 << endl;
    }
    void SetGenParticleInfo(int indexGenParticle, GenParticle * inGenPart) {
        if (indexGenParticle >= (int) numGenPartSt1) {
            //DebugGrabFailure(indexGenParticle);
            inGenPart->ClearVals();
        }
        else {            
            inGenPart->P4.SetPxPyPzE(genPartSt1Px->at(indexGenParticle), genPartSt1Py->at(indexGenParticle), genPartSt1Pz->at(indexGenParticle), genPartSt1En->at(indexGenParticle));
            inGenPart->PDGID = genPartSt1_pdgId->at(indexGenParticle);
            inGenPart->PDGStatus = 1;
            inGenPart->Mass = inGenPart->P4.M();
        }
    }
    void SetGenParticleMomInfo(int indexGenParticle, GenParticle * inGenPart) {
        if (indexGenParticle >= (int) numGenPartSt1) {
            //DebugGrabFailure(indexGenParticle);
            inGenPart->ClearVals();
        }
        else {
            inGenPart->P4.SetPxPyPzE(genPartSt1MomPx->at(indexGenParticle), genPartSt1MomPy->at(indexGenParticle), genPartSt1MomPz->at(indexGenParticle), genPartSt1MomEn->at(indexGenParticle));
            inGenPart->PDGID = genPartSt1MomPID->at(indexGenParticle);
            inGenPart->PDGStatus = genPartSt1MomStatus->at(indexGenParticle);
            inGenPart->Mass = inGenPart->P4.M();
        }
    }
    void SetGenParticleMomInfo_Part2(GenParticle * inGenPart, GenParticle * inGenPartMom) {
        inGenPart->MomPDGID = inGenPartMom->PDGID;
        inGenPart->MomPDGStatus = inGenPartMom->PDGStatus;
    }
    void SetGenParticleInfo_Full(int indexGenParticle, GenParticleSt1Info * inGPISt1) {
        if (indexGenParticle >= (int) numGenPartSt1) {
            //DebugGrabFailure(indexGenParticle);
            inGPISt1->BadParticle();
        }
        else {
            SetGenParticleInfo(indexGenParticle, &inGPISt1->GP);
            SetGenParticleMomInfo(indexGenParticle, &inGPISt1->GP_Mom);
            SetGenParticleMomInfo_Part2(&inGPISt1->GP, &inGPISt1->GP_Mom);
            inGPISt1->ExtractP4Params();
//            inGPISt1->ExtractNonKinParams();
        }
    }
    void SetBranchAddresses(TTree * inTree, TString baseName) {
        
        inTree->SetBranchAddress( baseName + TString("_PID"),     &genPartSt1_pdgId );
        inTree->SetBranchAddress( baseName + TString("_Px"),      &genPartSt1Px );
        inTree->SetBranchAddress( baseName + TString("_Py"),      &genPartSt1Py );
        inTree->SetBranchAddress( baseName + TString("_Pz"),      &genPartSt1Pz );
        inTree->SetBranchAddress( baseName + TString("_Energy"),  &genPartSt1En );
        inTree->SetBranchAddress( baseName + TString("_MPID"),    &genPartSt1MomPID );
        inTree->SetBranchAddress( baseName + TString("_MPx"),     &genPartSt1MomPx );
        inTree->SetBranchAddress( baseName + TString("_MPy"),     &genPartSt1MomPy );
        inTree->SetBranchAddress( baseName + TString("_MPz"),     &genPartSt1MomPz );
        inTree->SetBranchAddress( baseName + TString("_MEnergy"), &genPartSt1MomEn );
        inTree->SetBranchAddress( baseName + TString("_MSt"),     &genPartSt1MomStatus );
    }
} GenParticlePointers_St1;


typedef struct GenEWKEventPointers {
    // contains variables and functions related to the pointers relevant for our Status 3 EWK Vector Bosons
    vector<GenParticlePointers_St3> vecGPP_St3_EWKVecBoson;
    int numBosons;
    //currently just W and Z; 0 is W and 1 is Z
    void InitializeVecs() {
        numBosons = 2;
        for (int iBoson = 0; iBoson < numBosons; ++iBoson) {
            vecGPP_St3_EWKVecBoson[iBoson].InitializeVecs();
        }
    }    
    void DeleteVecs() {
        for (int iBoson = 0; iBoson < numBosons; ++iBoson) {
            vecGPP_St3_EWKVecBoson[iBoson].DeleteVecs();
        }       
    }    
    void SetSize() {
        for (int iBoson = 0; iBoson < numBosons; ++iBoson) {
            vecGPP_St3_EWKVecBoson[iBoson].SetSize();
        }
    }
    void SetGenParticleInfo_Full(int whichParticleType, int indexGenParticle, GenParticleSt3Info * inGPISt3) {
        vecGPP_St3_EWKVecBoson[whichParticleType].SetGenParticleInfo_Full(indexGenParticle, inGPISt3);
    }
} GenEWKEventPointers;

typedef struct GenSUSYEventPointers {
    // contains variables and functions related to the pointers relevant for our Status 3 SUSY particles
    vector<double> * genStopPolarizationWeights;
    // Gen info
    vector<float> * genStopMass, * genChi0Mass, * genCharginoMass;
    vector<double> * genPolWeights;
    
    GenParticlePointers_St3 GPP_St3_Stop, GPP_St3_Chi0, GPP_St3_Chargino;
    
    void InitializeVecs() {
        genStopPolarizationWeights = new vector<double>;
        genStopMass = new vector<float>;
        genChi0Mass = new vector<float>;
        genCharginoMass = new vector<float>;
        genPolWeights = new vector<double>; 
        
        GPP_St3_Stop.InitializeVecs();
        GPP_St3_Chi0.InitializeVecs();
        GPP_St3_Chargino.InitializeVecs();        
    }
    
    void DeleteVecs() {
        delete genStopPolarizationWeights;
        delete genStopMass;
        delete genChi0Mass;
        delete genCharginoMass;
        delete genPolWeights;
        
        GPP_St3_Stop.DeleteVecs();
        GPP_St3_Chi0.DeleteVecs();
        GPP_St3_Chargino.DeleteVecs();        
    }
    
    void SetSize() {
        GPP_St3_Stop.SetSize();
        GPP_St3_Chi0.SetSize();
        GPP_St3_Chargino.SetSize();
    }
    void SetGenParticleInfo_Full(int whichParticleType, int indexGenParticle, GenParticleSt3Info * inGPISt3) {
        GenParticlePointers_St3 * GPP_St3ToUse;
        switch (whichParticleType) {
            case 0:
                GPP_St3ToUse = &GPP_St3_Stop;
                break;
            case 1:
                GPP_St3ToUse = &GPP_St3_Chi0;
                break;
            case 2:
                GPP_St3ToUse = &GPP_St3_Chargino;
                break;
        }
        GPP_St3ToUse->SetGenParticleInfo_Full(indexGenParticle, inGPISt3);
    }
} GenSUSYEventPointers;

typedef struct GenQuarkEventPointers {
    // contains variables and functions related to the pointers relevant for our Status 3 and Status 1 heavy quarks
    GenParticlePointers_St3 GPP_St3_Top, GPP_St3_B;
    GenParticlePointers_St1 GPP_St1_B;
    void InitializeVecs() {
        GPP_St3_Top.InitializeVecs();
        GPP_St3_B.InitializeVecs();
        GPP_St1_B.InitializeVecs();
    }
    void DeleteVecs() {
        GPP_St3_Top.DeleteVecs();
        GPP_St3_B.DeleteVecs();
        GPP_St1_B.DeleteVecs();
    }
    void SetSize() {
        GPP_St3_Top.SetSize();
        GPP_St3_B.SetSize();
        GPP_St1_B.SetSize();
    }
    void SetGenParticleInfo_Full(int whichParticleType, int indexGenParticle, GenParticleSt3Info * inGPISt3) {
        GenParticlePointers_St3 * GPP_St3ToUse;
        switch (whichParticleType) {
            case 0:
                GPP_St3ToUse = &GPP_St3_Top;
                break;
            case 1:
                GPP_St3ToUse = &GPP_St3_B;
                break;
        }
        GPP_St3ToUse->SetGenParticleInfo_Full(indexGenParticle, inGPISt3);
    }
    void SetGenParticleInfo_Full(int whichParticleType, int indexGenParticle, GenParticleSt1Info * inGPISt1) {
        GenParticlePointers_St1 * GPP_St1ToUse;
        switch (whichParticleType) {
            case 1:
                GPP_St1ToUse = &GPP_St1_B;
                break;
        }
        GPP_St1ToUse->SetGenParticleInfo_Full(indexGenParticle, inGPISt1);
    }
} GenQuarkEventPointers;

typedef struct GenLeptonEventPointers {
    // contains variables and functions related to the pointers relevant for our Status 3 and Status 1 Gen leptons
    GenParticlePointers_St1 GPP_St1_Elec, GPP_St1_Muon;
    GenParticlePointers_St3 GPP_St3_Elec, GPP_St3_Muon;
    void InitializeVecs() {
        GPP_St1_Elec.InitializeVecs();
        GPP_St1_Muon.InitializeVecs();
        GPP_St3_Elec.InitializeVecs();
        GPP_St3_Muon.InitializeVecs();
    }
    void DeleteVecs() {
        GPP_St1_Elec.DeleteVecs();
        GPP_St1_Muon.DeleteVecs();
        GPP_St3_Elec.DeleteVecs();
        GPP_St3_Muon.DeleteVecs();
    }
    void SetSize() {
        GPP_St1_Elec.SetSize();
        GPP_St1_Muon.SetSize();
        GPP_St3_Elec.SetSize();
        GPP_St3_Muon.SetSize();
    }
    void SetGenParticleInfo_Full(int whichParticleType, int indexGenParticle, GenParticleSt3Info * inGPISt3) {
        GenParticlePointers_St3 * GPP_St3ToUse;
        switch (whichParticleType) {
            case 0:
                GPP_St3ToUse = &GPP_St3_Elec;
                break;
            case 1:
                GPP_St3ToUse = &GPP_St3_Muon;
                break;
        }
        GPP_St3ToUse->SetGenParticleInfo_Full(indexGenParticle, inGPISt3);
    }
    void SetGenParticleInfo_Full(int whichParticleType, int indexGenParticle, GenParticleSt1Info * inGPISt1) {
        GenParticlePointers_St1 * GPP_St1ToUse;
        switch (whichParticleType) {
            case 0:
                GPP_St1ToUse = &GPP_St1_Elec;
                break;
            case 1:
                GPP_St1ToUse = &GPP_St1_Muon;
                break;
            default:
                break;
        }
        GPP_St1ToUse->SetGenParticleInfo_Full(indexGenParticle, inGPISt1);
    }
    bool FindMatchGenLep(Lepton * inLep, GenParticle * matchGenPart) {
        // given input PF Reco lepton, find matching genLepton
        bool foundMatch = false;
        float maxDeltaR = 0.1;
        float bestDeltaR = maxDeltaR;
        float currDeltaR;
        float matchPtLB, matchPtUB;
        GenParticle MomTemp;
        int matchIndex;
        GenParticlePointers_St1 * GPP_St1_ToUse;
        switch (abs(inLep->PDGID)) {
            case 11:
                GPP_St1_ToUse = &GPP_St1_Elec;
                break;
            case 13:
                GPP_St1_ToUse = &GPP_St1_Muon;
                break;
            default:
                return false;
                break;
        }
        for (unsigned int iGenLep = 0; iGenLep < GPP_St1_ToUse->numGenPartSt1; ++iGenLep) {
            GPP_St1_ToUse->SetGenParticleInfo(iGenLep, matchGenPart);
            
            // require consistent pT -- very generous cuts right now
            matchPtLB = 0.5 * matchGenPart->P4.Pt();
            matchPtUB = 2.0 * matchGenPart->P4.Pt();
//            matchPtLB = 0.0;// 0.5 * matchGenPart->BVC.Vec_Pt;
//            matchPtUB = 1E5;//2.0 * matchGenPart->BVC.Vec_Pt;
            
            // require consistent PDGID
            if (matchGenPart->PDGID != inLep->PDGID) continue;
            if (inLep->BVC.Vec_Pt < matchPtLB || inLep->BVC.Vec_Pt > matchPtUB) continue;
            currDeltaR = deltaR(&inLep->P4, &matchGenPart->P4);
            // require deltaR matching
            if (currDeltaR < maxDeltaR && currDeltaR < bestDeltaR) {
                foundMatch = true;
                bestDeltaR = currDeltaR;
                matchIndex = iGenLep;
            }            
        }
        if (foundMatch) {
            GPP_St1_ToUse->SetGenParticleInfo(matchIndex, matchGenPart);
            GPP_St1_ToUse->SetGenParticleMomInfo(matchIndex, &MomTemp);
            GPP_St1_ToUse->SetGenParticleMomInfo_Part2(matchGenPart, &MomTemp);
        }
        return foundMatch;
    }
} GenLeptonEventPointers;


typedef struct GenJetEventPointers {
    // contains variables and functions related to the pointers relevant for our genJets
    
    std::vector<float> * genJetPx, * genJetPy, * genJetPz, * genJetEn, * genJetInvisE; //* genJetEt, * genJetEta;
    std::vector<bool> * genJetIsGenJet;
    unsigned int numGenJets;
    
    void InitializeVecs() {
        genJetPx = new vector<float>;
        genJetPy = new vector<float>;
        genJetPz = new vector<float>;
        genJetEn = new vector<float>;
        genJetInvisE = new vector<float>;
        genJetIsGenJet = new vector<bool>;
    }
    void DeleteVecs() {
        delete genJetPx;
        delete genJetPy;
        delete genJetPz;
        delete genJetEn;
        delete genJetInvisE;
        delete genJetIsGenJet;
    }
    void SetSize() {
        numGenJets = genJetEn->size();
    }
} GenJetEventPointers;
