// Very basic Structs used as part of Skimming and/or plot filling

typedef struct ElecCutInfo {
 // Look here for CutID definitions: https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification
    
    vector<float> vecCutDeltaEtaIn, vecCutDeltaPhiIn;
    vector<float> vecCutSigIetaIeta, vecCutHOverE;
    vector<float> vecCutDelta0, vecCutDeltaZ;
    vector<float> vecCutDeltaEP, vecCutRelIso;
    vector<float> vecCutMissHits;
    
// Look Here for Trigger ID definitions: https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification#Tight_Trigger_ID
    vector<float> vecCutDeltaEtaIn_Trigger, vecCutDeltaPhiIn_Trigger;
    vector<float> vecCutSigIetaIeta_Trigger, vecCutHOverE_Trigger;
    vector<float> vecCutRelIsoECAL_Trigger, vecCutRelIsoHCAL_Trigger, vecCutRelIsoTrack_Trigger;
    
    void SetElecCutVars(int cutStrength) {
        // cutStrength 0: Loose, 1: Medium, 2: Tight
        float cutDeltEtaBarrel[3] = {0.007, 0.004, 0.004};        
        float cutDeltPhiBarrel[3] = {0.15, 0.06, 0.03};        
        float cutSigIetaIetaBarrel[3] = {0.01, 0.01, 0.01};
        float cutHOverEBarrel[3] = {0.12, 0.12, 0.12};        
        float cutDelt0Barrel[3] = {0.02, 0.02, 0.02};        
        float cutDeltZBarrel[3] = {0.2, 0.1, 0.1};
        float cutRelIsoBarrel[3] = {1.0, 0.15, 0.10};
        float cutDeltEPBarrel[3] = {0.05, 0.05, 0.05};
        float cutMissHitsBarrel[3] = {1, 1, 0};
        
        float cutDeltEtaEndcap[3] = {0.009, 0.007, 0.005};
        float cutDeltPhiEndcap[3] = {0.10, 0.03, 0.02};
        float cutSigIetaIetaEndcap[3] = {0.03, 0.03, 0.03};
        float cutHOverEEndcap[3] = {0.10, 0.10, 0.10};
        float cutDelt0Endcap[3] = {0.02, 0.02, 0.02};
        float cutDeltZEndcap[3] = {0.2, 0.1, 0.1};
        float cutRelIsoEndcap[3] = {1.0, 0.15, 0.10};
        float cutDeltEPEndcap[3] = {0.05, 0.05, 0.05};
        float cutMissHitsEndcap[3] = {1, 1, 0};
        
        vecCutDeltaEtaIn.resize(2);
        vecCutDeltaEtaIn[0] = cutDeltEtaBarrel[cutStrength];
        vecCutDeltaEtaIn[1] = cutDeltEtaEndcap[cutStrength];
        
        vecCutDeltaPhiIn.resize(2);
        vecCutDeltaPhiIn[0] = cutDeltPhiBarrel[cutStrength];
        vecCutDeltaPhiIn[1] = cutDeltPhiEndcap[cutStrength];
        
        vecCutSigIetaIeta.resize(2);
        vecCutSigIetaIeta[0] = cutSigIetaIetaBarrel[cutStrength];
        vecCutSigIetaIeta[1] = cutSigIetaIetaEndcap[cutStrength];
        
        vecCutHOverE.resize(2);
        vecCutHOverE[0] = cutHOverEBarrel[cutStrength];
        vecCutHOverE[1] = cutHOverEEndcap[cutStrength];
        
        vecCutDelta0.resize(2);
        vecCutDelta0[0] = cutDelt0Barrel[cutStrength];
        vecCutDelta0[1] = cutDelt0Endcap[cutStrength];

        vecCutDeltaZ.resize(2);
        vecCutDeltaZ[0] = cutDeltZBarrel[cutStrength];
        vecCutDeltaZ[1] = cutDeltZEndcap[cutStrength];
        
        vecCutDeltaEP.resize(2);
        vecCutDeltaEP[0] = cutDeltEPBarrel[cutStrength];
        vecCutDeltaEP[1] = cutDeltEPEndcap[cutStrength];
        
        vecCutRelIso.resize(2);
        vecCutRelIso[0] = cutRelIsoBarrel[cutStrength];
        vecCutRelIso[1] = cutRelIsoEndcap[cutStrength];
        
        vecCutMissHits.resize(2);
        vecCutMissHits[0] = cutMissHitsBarrel[cutStrength];
        vecCutMissHits[1] = cutMissHitsEndcap[cutStrength];
        
        
        //Numbers for Trigger ID
        
        
        
        float cutDeltEtaBarrel_Trigger = 0.007;
        float cutDeltEtaEndcap_Trigger = 0.009;

        float cutDeltPhiBarrel_Trigger = 0.15;
        float cutDeltPhiEndcap_Trigger = 0.10;
        
        
        float cutSigIetaIetaBarrel_Trigger = 0.01;
        float cutSigIetaIetaEndcap_Trigger = 0.03;

        float cutHOverEBarrel_Trigger = 0.12;
        float cutHOverEEndcap_Trigger = 0.10;
        
        float cutRelECALIsoBarrel_Trigger = 0.2;
        float cutRelECALIsoEndcap_Trigger = 0.2;
        
        float cutRelHCALIsoBarrel_Trigger = 0.2;
        float cutRelHCALIsoEndcap_Trigger = 0.2;

        float cutRelTrackIsoBarrel_Trigger = 0.2;
        float cutRelTrackIsoEndcap_Trigger = 0.2;
        
        vecCutDeltaEtaIn_Trigger.resize(2);
        vecCutDeltaEtaIn_Trigger[0] = cutDeltEtaBarrel_Trigger;
        vecCutDeltaEtaIn_Trigger[1] = cutDeltEtaEndcap_Trigger;
        
        vecCutDeltaPhiIn_Trigger.resize(2);
        vecCutDeltaPhiIn_Trigger[0] = cutDeltPhiBarrel_Trigger;
        vecCutDeltaPhiIn_Trigger[1] = cutDeltPhiEndcap_Trigger;
        
        vecCutSigIetaIeta_Trigger.resize(2);
        vecCutSigIetaIeta_Trigger[0] = cutSigIetaIetaBarrel_Trigger;
        vecCutSigIetaIeta_Trigger[1] = cutSigIetaIetaEndcap_Trigger;
        
        vecCutHOverE_Trigger.resize(2);
        vecCutHOverE_Trigger[0] = cutHOverEBarrel_Trigger;
        vecCutHOverE_Trigger[1] = cutHOverEEndcap_Trigger;
        
        vecCutRelIsoECAL_Trigger.resize(2);
        vecCutRelIsoECAL_Trigger[0] = cutRelECALIsoBarrel_Trigger;
        vecCutRelIsoECAL_Trigger[1] = cutRelECALIsoEndcap_Trigger;
        
        vecCutRelIsoHCAL_Trigger.resize(2);
        vecCutRelIsoHCAL_Trigger[0] = cutRelHCALIsoBarrel_Trigger;
        vecCutRelIsoHCAL_Trigger[1] = cutRelHCALIsoEndcap_Trigger;
        
        vecCutRelIsoTrack_Trigger.resize(2);
        vecCutRelIsoTrack_Trigger[0] = cutRelTrackIsoBarrel_Trigger;
        vecCutRelIsoTrack_Trigger[1] = cutRelTrackIsoEndcap_Trigger;
    }
    void DefaultCutVarVals() {
        SetElecCutVars(1);
    }
    void SetAsFakeLepECI() {
        SetElecCutVars(0);
    }
    bool PassStage1(int indexToUse, float elecDEtaIn, float elecDPhiIn, float elecSigIetaIeta, float elecHtoE, int levelVerbosity = 0) {
        bool passCuts;
        if (levelVerbosity) {
            cout << "indexToUse " << indexToUse << endl;
            cout << endl;
            cout << "fabs(elecDEtaIn) " << fabs(elecDEtaIn) << endl;
            cout << "vecCutDeltaEtaIn[indexToUse] " << vecCutDeltaEtaIn[indexToUse] << endl;
            cout << endl;
            cout << "fabs(elecDPhiIn) " << fabs(elecDPhiIn) << endl;
            cout << "vecCutDeltaPhiIn[indexToUse] " << vecCutDeltaPhiIn[indexToUse] << endl;
            cout << endl;
            cout << "elecSigIetaIeta " << elecSigIetaIeta << endl;
            cout << "vecCutSigIetaIeta[indexToUse] " << vecCutSigIetaIeta[indexToUse] << endl;
            cout << endl;
            cout << "elecHtoE " << elecHtoE << endl;
            cout << "vecCutHOverE[indexToUse] " << vecCutHOverE[indexToUse] << endl;
        }
        if (fabs(elecDEtaIn) < vecCutDeltaEtaIn[indexToUse] && fabs(elecDPhiIn) < vecCutDeltaPhiIn[indexToUse] && elecSigIetaIeta < vecCutSigIetaIeta[indexToUse] && elecHtoE < vecCutHOverE[indexToUse]) {
            passCuts = true;
        }
        else {
            passCuts = false;
        }
        return passCuts;
    }
    bool PassStage2(int indexToUse, float elecRelIso, float elecD0, float elecDZ, float elecEtoP, float elecEn, int levelVerbosity = 0) {
        bool passCuts;
        if (levelVerbosity) {
            cout << "indexToUse " << indexToUse << endl;
            cout << endl;
            cout << "fabs(elecD0) " << fabs(elecD0) << endl;
            cout << "vecCutDelta0[indexToUse] " << vecCutDelta0[indexToUse] << endl;
            cout << endl;
            cout << "fabs(elecDZ) " << fabs(elecDZ) << endl;
            cout << "vecCutDeltaZ[indexToUse] " << vecCutDeltaZ[indexToUse] << endl;
            cout << endl;
            cout << "fabs((1 - elecEtoP)/elecEn) " << fabs((1 - elecEtoP)/elecEn) << endl;
            cout << "vecCutDeltaEP " << vecCutDeltaEP[indexToUse] << endl;
            cout << endl;
            cout << "elecRelIso " << elecRelIso << endl;
            cout << "vecCutRelIso[indexToUse] " << vecCutRelIso[indexToUse] << endl;
        }
        if (fabs(elecD0) < vecCutDelta0[indexToUse] && fabs(elecDZ) < vecCutDeltaZ[indexToUse] && fabs((1 - elecEtoP)/elecEn) < vecCutDeltaEP[indexToUse] && elecRelIso < vecCutRelIso[indexToUse]) {
            passCuts = true;
        }
        else {
            passCuts = false;
        }
        return passCuts;
    }
    
    bool PassLooseCutTrigger_Stage1(int indexToUse, float elecDEtaIn, float elecDPhiIn, float elecSigIetaIeta, float elecHtoE, int levelVerbosity = 0) {
        bool passCuts;
        if (levelVerbosity) {
            cout << "indexToUse " << indexToUse << endl;
            cout << endl;
            cout << "fabs(elecDEtaIn) " << fabs(elecDEtaIn) << endl;
            cout << "vecCutDeltaEtaIn_Trigger[indexToUse] " << vecCutDeltaEtaIn_Trigger[indexToUse] << endl;
            cout << endl;
            cout << "fabs(elecDPhiIn) " << fabs(elecDPhiIn) << endl;
            cout << "vecCutDeltaPhiIn_Trigger[indexToUse] " << vecCutDeltaPhiIn_Trigger[indexToUse] << endl;
            cout << endl;
            cout << "elecSigIetaIeta " << elecSigIetaIeta << endl;
            cout << "vecCutSigIetaIeta_Trigger[indexToUse] " << vecCutSigIetaIeta_Trigger[indexToUse] << endl;
            cout << endl;
            cout << "elecHtoE " << elecHtoE << endl;
            cout << "vecCutHOverE_Trigger[indexToUse] " << vecCutHOverE_Trigger[indexToUse] << endl;
        }
        if (fabs(elecDEtaIn) < vecCutDeltaEtaIn_Trigger[indexToUse] && fabs(elecDPhiIn) < vecCutDeltaPhiIn_Trigger[indexToUse] && elecSigIetaIeta < vecCutSigIetaIeta_Trigger[indexToUse] && elecHtoE < vecCutHOverE_Trigger[indexToUse]) {
            passCuts = true;
        }
        else {
            passCuts = false;
        }
        return passCuts;
    }
    bool PassLooseCutTrigger_Stage2(int indexToUse, float elecRelIsoECAL, float elecRelIsoHCAL, float elecRelIsoTrack, int levelVerbosity = 0) {
        bool passCuts;
        if (levelVerbosity) {
            cout << "indexToUse " << indexToUse << endl;
            cout << endl;
            cout << "elecRelIsoECAL " << elecRelIsoECAL << endl;
            cout << "vecCutRelIsoECAL_Trigger " << vecCutRelIsoECAL_Trigger[indexToUse] << endl;
            cout << endl;
            cout << "elecRelIsoHCAL " << elecRelIsoHCAL << endl;
            cout << "vecCutRelIsoHCAL_Trigger " << vecCutRelIsoHCAL_Trigger[indexToUse] << endl;
            cout << endl;
            cout << "elecRelIsoTrack " << elecRelIsoTrack << endl;
            cout << "vecCutRelIsoTrack_Trigger " << vecCutRelIsoTrack_Trigger[indexToUse] << endl;
        }
        if (elecRelIsoECAL < vecCutRelIsoECAL_Trigger[indexToUse] && elecRelIsoHCAL < vecCutRelIsoHCAL_Trigger[indexToUse] && elecRelIsoTrack < vecCutRelIsoTrack_Trigger[indexToUse]) {
            passCuts = true;
        }
        else {
            passCuts = false;
        }
        return passCuts;
    }
} ElecCutInfo;

typedef struct JetCutInfo {
    // Look here for CutID definitions: https://twiki.cern.ch/twiki/bin/view/CMS/JetID
    float cutNHF, cutNEF, cutCHF, cutCEF;
    int cutNConst, cutChMult;
    float cutBTagDisc, cutJetPt, cutJetEta;
    void SetJetCutVars(int cutStrength) {
        // cutStrength 0: Loose, 1: Medium, 2: Tight
        float cutNHFStrength[3] = {0.99, 0.95, 0.90};
        float cutNEFStrength[3] = {0.99, 0.95, 0.90};
        float cutCHFStrength[3] = {0., 0., 0.};
        float cutCEFStrength[3] = {0.99, 0.99, 0.99};
        int cutNConstStrength[3] = {1, 1, 1};
        int cutChMultStrength[3] = {0, 0, 0};
        cutNHF = cutNHFStrength[cutStrength];
        cutNEF = cutNEFStrength[cutStrength];
        cutCHF = cutCHFStrength[cutStrength];
        cutCEF = cutCEFStrength[cutStrength];
        cutNConst = cutNConstStrength[cutStrength];
        cutChMult = cutChMultStrength[cutStrength];
    }
    void DefaultCutVarVals() {
        SetJetCutVars(0);
        cutJetPt = 30.;
        cutJetEta = 2.4;
        cutBTagDisc = 0.679;  //CSV Middle working point, see (remove underscore in address): h_ttps://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP
    }
    void PrintVals() {
        cout << "cutNHF " << cutNHF << endl;
        cout << "cutNEF " << cutNEF << endl;
        cout << "cutCHF " << cutCHF << endl;
        cout << "cutCEF " << cutCEF << endl;
        cout << "cutNConst " << cutNConst << endl;
        cout << "cutChMult " << cutChMult << endl;
        cout << "cutBTagDisc " << cutBTagDisc << endl;
        cout << "cutJetPt " << cutJetPt << endl;
        cout << "cutJetEta " << cutJetEta << endl;
    }
} JetCutInfo;

typedef struct BasicVecComponents {
    float Vec_Pt, Vec_Eta, Vec_Phi, Vec_Mass;
    float Vec_Px, Vec_Py, Vec_Pz, Vec_En;
    void ExtractParams(TLorentzVector * inP4) {
        Vec_Pt    = inP4->Pt();
        if (Vec_Pt == 0) {
            Vec_Eta = 1E9;
        }
        else {
            Vec_Eta   = inP4->Eta();   
        }
        Vec_Phi   = inP4->Phi();
        Vec_Mass  = inP4->M();
        Vec_Px    = inP4->Px();
        Vec_Py    = inP4->Py();
        Vec_Pz    = inP4->Pz();
        Vec_En    = inP4->E();
    }
    void PrintVals(int whichSet = 1) {
        switch (whichSet) {
            case 1:
                cout << "Vec_Pt " << Vec_Pt << endl;
                cout << "Vec_Eta " << Vec_Eta << endl;
                cout << "Vec_Phi " << Vec_Phi << endl;
                cout << "Vec_Mass " << Vec_Mass << endl;
                break;
            case 2:
                cout << "Vec_Px " << Vec_Px << endl;
                cout << "Vec_Py " << Vec_Py << endl;
                cout << "Vec_Pz " << Vec_Pz << endl;
                cout << "Vec_En " << Vec_En << endl;
                break;
        }
    }
    void SetP4Vals(TLorentzVector * inP4) {
        inP4->SetPxPyPzE(Vec_Px, Vec_Py, Vec_Pz, Vec_En);
    }
} BasicVecComponents;

#include "StopStructs_BasicGenParticleStructs.h"
#include "StopStructs_BasicRecoParticleStructs.h"