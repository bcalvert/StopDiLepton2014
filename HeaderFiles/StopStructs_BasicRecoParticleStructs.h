// Structs for very basic reco-level particles -- leptons, jets, etc.

using namespace std;
typedef struct Lepton {
    TLorentzVector  P4;
    BasicVecComponents BVC;
    GenParticle GP;
    bool isGenMatched;
    bool isPromptLepton;
    int  lepQuality;
    int             PDGID;
    float           relPFLepIso;
    void ClearVars() {
        DefaultLorentzVector(&P4);
        BVC.ExtractParams(&P4);
        PDGID = 0;
        relPFLepIso = -1;
    }
    void isBadLep() {
        DefaultLorentzVector(&P4);
        BVC.ExtractParams(&P4);
        PDGID = 0;
        relPFLepIso = -99999;
        lepQuality = -1;
    }
    bool isElec() {
        return (abs(PDGID) == 11);
    }
    bool isMuon() {
        return (abs(PDGID) == 13);
    }
    void SetPromptLeptonStatus() {
        isPromptLepton = isGenMatched;
        isPromptLepton &= (abs(GP.MomPDGID) == 15 || abs(GP.MomPDGID) == 23 || abs(GP.MomPDGID) == 24);
    }
    void PrintVals() {
        cout << "Lepton Vec Components: " << endl;
        BVC.PrintVals(1);
        cout << "Lepton PDGID " << PDGID << endl;
        cout << "Gen Particle Matched? " << isGenMatched << endl;
        if (isGenMatched) {
            cout << "printing matched genLep info " << endl;
            GP.BVC.PrintVals(1);
        }
        else {
            cout << "not Gen Matched" << endl;
        }
        cout << "lepQuality " << lepQuality << endl;
    }
    float TempEtaFixElectron() {
        float EtaEBEnd = 1.4442;
        float EtaEEBegin = 1.566;
        float EtaEEEnd = 2.5;
        float safetyFactor = 1E-3;
        float absLepEta = fabs(BVC.Vec_Eta);
        if (absLepEta > EtaEEEnd) {
            return BVC.Vec_Eta < 0 ? -1 * EtaEEEnd : EtaEEEnd;
        }
        return BVC.Vec_Eta > 0 ? TruncateNum(BVC.Vec_Eta, EtaEBEnd, EtaEEBegin, safetyFactor) : TruncateNum(BVC.Vec_Eta, -1 * EtaEEBegin, -1 * EtaEBEnd, safetyFactor);
    }
} Lepton;

inline bool operator<(const Lepton &a, const Lepton &b)
{
    return (a.BVC.Vec_Pt < b.BVC.Vec_Pt);
}

inline bool operator>(const Lepton &a, const Lepton &b)
{
    return (a.BVC.Vec_Pt > b.BVC.Vec_Pt);
}

typedef struct PFJet {
    TLorentzVector  P4;
    BasicVecComponents BVC;
    int             partonFlavor;
    
    GenJet GJ;
    bool            isGenJetMatched;
    float           dEnRecoGen;
    
    float           valBTagDisc;
    bool            isBJet;
    
    float NeutHadFrac, ChargeHadFrac, NeutEMFrac, ChargeEMFrac;
    int   NumDaughters, ChargeMult;
    bool  passJetID;
    
    void SetBasicKinInfo(PFJet * inPFJet) {
        P4           = inPFJet->P4;
        BVC          = inPFJet->BVC;
    }
    void ExtractParams() {
        BVC.ExtractParams(&P4);
    }
    void ResetGenMatchInfo() {
        dEnRecoGen = 0.0;
        isGenJetMatched = false;
    }
    void SetGenMatchInfo(float inDeltEn, bool inGJM) {
        dEnRecoGen = inDeltEn;
        isGenJetMatched = inGJM;
    }
    void SetBJetInfo(PFJet * inPFJet) {
        partonFlavor = inPFJet->partonFlavor;
        valBTagDisc = inPFJet->valBTagDisc; 
        isBJet      = inPFJet->isBJet;
    }
    void SetGenInfo(PFJet * inPFJet) {
        GJ              = inPFJet->GJ; 
        isGenJetMatched = inPFJet->isGenJetMatched; 
        dEnRecoGen      = inPFJet->dEnRecoGen;
    }
    void SetQualityInfo(PFJet * inPFJet) {
        NeutHadFrac   = inPFJet->NeutHadFrac;
        ChargeHadFrac = inPFJet->ChargeHadFrac;
        NeutEMFrac    = inPFJet->NeutEMFrac;
        ChargeEMFrac  = inPFJet->ChargeEMFrac;
        NumDaughters  = inPFJet->NumDaughters;
        ChargeMult    = inPFJet->ChargeMult;
        passJetID     = inPFJet->ChargeMult;
    }
    void SetInfo(PFJet * inPFJet, bool dBKI, bool dBJI, bool dGI, bool dQI) {
        if (dBKI) SetBasicKinInfo(inPFJet);
        if (dBJI) SetBJetInfo(inPFJet);
        if (dGI) SetGenInfo(inPFJet);
        if (dQI) SetQualityInfo(inPFJet);
    }
    void SetNonBJetInfo(PFJet * inPFJet) {
        SetInfo(inPFJet, true, false, true, true);
    }
    void ClearVars() {
        DefaultLorentzVector(&P4);
        BVC.ExtractParams(&P4);
        valBTagDisc = 0.0;
        partonFlavor = -999999;
        isBJet = false;
        isGenJetMatched = false;
        dEnRecoGen      = 0.0;
        NeutHadFrac     = 0.0;
        ChargeHadFrac   = 0.0;
        NeutEMFrac      = 0.0;
        ChargeEMFrac    = 0.0;
        NumDaughters    = 0;
        ChargeMult      = 0;
        passJetID       = true;
    }
    void isBadJet() {
        DefaultLorentzVector(&P4);
        BVC.ExtractParams(&P4);
        valBTagDisc = -1;
        partonFlavor = -999999;
        isBJet = false;
        isGenJetMatched = false;
        dEnRecoGen = 0.0;
    }
    bool PassesID(JetCutInfo * inJCI) {
        bool passJetID = false;
        passJetID = (NeutHadFrac < inJCI->cutNHF && NeutEMFrac < inJCI->cutNEF);
        //        cout << "passJetIDStage 1? " << passJetID << endl;        
        if (fabs(P4.Eta()) < inJCI->cutJetEta) {
            //            cout << "P4.Eta() " << P4.Eta() << endl;
            passJetID &= (ChargeEMFrac < inJCI->cutCEF && ChargeHadFrac > inJCI->cutCHF);
            passJetID &= (ChargeMult > inJCI->cutChMult);
            //            cout << "passJetIDStage 2? " << passJetID << endl;
        }
        return passJetID;
    }
    /*
    void SetJetVars(PFJetEventPointers inPFJEPs, int iJet) {
        //Set LorentzVector
        P4.SetPxPyPzE(inJetPx->at(iJet), inJetPy->at(iJet), inJetPz->at(iJet), inJetE->at(iJet));
        // Set BTagging info
        valBTagDisc = inJetBTag->at(iJet);
        partonFlavor = inJetPartFlav->at(iJet);
        
        //Set Jet quality cut variables
        NeutHadFrac = inJetNHF->at(iJet);
        ChargeHadFrac = inJetCHF->at(iJet);
        NeutEMFrac = inJetNEF->at(iJet);
        ChargeEMFrac = inJetCEF->at(iJet);
        NumDaughters = inJetNDaug->at(iJet);
        ChargeMult      = inJetCharMult->at(iJet);
    }
    */
    void PrintVals() {
        BVC.PrintVals();
        
        cout << "valBTagDisc " << valBTagDisc << endl;
        cout << "partonFlavor " << partonFlavor << endl;
        
        cout << "NeutHadFrac " << NeutHadFrac << endl;        
        cout << "ChargeHadFrac " << ChargeHadFrac << endl;
        cout << "NeutEMFrac " << NeutEMFrac << endl;
        cout << "ChargeEMFrac " << ChargeEMFrac << endl;
        cout << "NumDaughters " << NumDaughters << endl;
        cout << "ChargeMult " << ChargeMult << endl;
    }
} PFJet;
inline bool operator<(const PFJet &a, const PFJet &b)
{
    return (a.BVC.Vec_Pt < b.BVC.Vec_Pt);
}
inline bool operator>(const PFJet &a, const PFJet &b)
{
    return (a.BVC.Vec_Pt > b.BVC.Vec_Pt);
}

typedef struct DiParticle {
    TLorentzVector P4;
    BasicVecComponents BVC;
    float DPhiInputObjects;
    TString Name;
    void DPDefaultVarVals(TString inName = "") {
        DefaultLorentzVector(&P4);
        BVC.ExtractParams(&P4);
        DPhiInputObjects = 0.0;
        Name = inName;
    }
    void SetDPVals(TLorentzVector * inputVec1, TLorentzVector * inputVec2) {
        P4 = (*inputVec1 + *inputVec2);
        BVC.ExtractParams(&P4);
        DPhiInputObjects = dPhi(inputVec1->Phi(), inputVec2->Phi());
    }
} DiParticle;





typedef struct SmearPFJet {
    PFJet           intPFJet;
    TLorentzVector  NonSmearP4;
    void ExtractParams() {
        intPFJet.ExtractParams();
    }
    void SetBasicKinInfo(PFJet * inPFJet) {
        intPFJet.SetBasicKinInfo(inPFJet);
    }
    void SetBJetInfo(PFJet * inPFJet) {
        intPFJet.SetBJetInfo(inPFJet);
    }
    void SetGenInfo(PFJet * inPFJet) {
        intPFJet.SetGenInfo(inPFJet);
    }
    void SetQualityInfo(PFJet * inPFJet) {
        intPFJet.SetQualityInfo(inPFJet);
    }
    void SetInfo(PFJet * inPFJet, bool dBKI, bool dBJI, bool dGI, bool dQI) {
        if (dBKI) SetBasicKinInfo(inPFJet);
        if (dBJI) SetBJetInfo(inPFJet);
        if (dGI) SetGenInfo(inPFJet);
        if (dQI) SetQualityInfo(inPFJet);
    }
    void SetNonBJetInfo(PFJet * inPFJet) {
        SetInfo(inPFJet, true, false, true, true);
    }
    void ClearVars() {
        intPFJet.ClearVars();
    }
    void isBadJet() {
        intPFJet.isBadJet();
    }
    bool PassesID(JetCutInfo * inJCI) {
        return intPFJet.PassesID(inJCI);
    }
    void PrintVals() {
        intPFJet.PrintVals();
    }
    bool SetNonKinematicValues(vector<PFJet> * inPFJetVec) {
        Double_t dRBest = 1.0;
        Double_t dRMin = 0.001;
        float currDeltaR;
        bool foundMatch = false;
        int matchIndex = -1;
        for (unsigned int iJet = 0; iJet < inPFJetVec->size(); ++iJet) {
            currDeltaR = deltaR(&intPFJet.P4, &inPFJetVec->at(iJet).P4);
            if (currDeltaR < dRBest && currDeltaR < dRMin) {
                dRBest = currDeltaR;
                foundMatch = true;
                matchIndex = iJet;
//                cout << "iJet " << iJet << endl;
//                cout << "dRBest " << dRBest << endl;
            }
        }
        if (foundMatch && matchIndex > -1) {
            /*
            cout << "before setting " << endl;
            PrintVals();
             */
            SetInfo(&inPFJetVec->at(matchIndex), false, true, true, true);
            NonSmearP4 = inPFJetVec->at(matchIndex).P4;
            
            /*
             note that energy fractions are slightly different....
            cout << "after setting " << endl;
            cout << "printing unsmeared vals" << endl;
            PrintLorentzVector(&NonSmearP4);
            cout << "printing smeared vals" << endl;
            PrintVals();
            */
        }
        return foundMatch;
    }
} SmearPFJet;
inline bool operator<(const SmearPFJet &a, const SmearPFJet &b)
{
    return (a.intPFJet < b.intPFJet);
}
inline bool operator>(const SmearPFJet &a, const SmearPFJet &b)
{
    return (a.intPFJet > b.intPFJet);
}


