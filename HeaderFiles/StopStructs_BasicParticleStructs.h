// Very basic Structs used as part of Skimming and/or plot filling

typedef struct ElecCutInfo {
 // Look here for CutID definitions: https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification
    
    vector<float> vecCutDeltaEtaIn, vecCutDeltaPhiIn;
    vector<float> vecCutSigIetaIeta, vecCutHOverE;
    vector<float> vecCutDelta0, vecCutDeltaZ;
    vector<float> vecCutDeltaEP, vecCutRelIso;
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
        
        
        float cutDeltEtaEndcap[3] = {0.009, 0.007, 0.005};
        float cutDeltPhiEndcap[3] = {0.10, 0.03, 0.02};
        float cutSigIetaIetaEndcap[3] = {0.03, 0.03, 0.03};
        float cutHOverEEndcap[3] = {0.10, 0.10, 0.10};
        float cutDelt0Endcap[3] = {0.02, 0.02, 0.02};
        float cutDeltZEndcap[3] = {0.2, 0.1, 0.1};
        float cutRelIsoEndcap[3] = {1.0, 0.15, 0.10};
        float cutDeltEPEndcap[3] = {0.05, 0.05, 0.05};
        
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
    }
    void DefaultCutVarVals() {
        SetElecCutVars(1);
    }
    void SetAsFakeLepECI() {
        SetElecCutVars(0);
    }
    bool PassStage1(int indexToUse, float elecDEtaIn, float elecDPhiIn, float elecSigIetaIeta, float elecHtoE) {
        bool passCuts;
        if (fabs(elecDEtaIn) < vecCutDeltaEtaIn[indexToUse] && fabs(elecDPhiIn) < vecCutDeltaPhiIn[indexToUse] && elecSigIetaIeta < vecCutSigIetaIeta[indexToUse] && elecHtoE < vecCutHOverE[indexToUse]) {
            passCuts = true;
        }
        else {
            passCuts = false;
        }
        return passCuts;
    }
    
    bool PassStage2(int indexToUse, float elecRelIso, float elecD0, float elecDZ, float elecEtoP, float elecEn) {
        bool passCuts;
        if (fabs(elecD0) < vecCutDelta0[indexToUse] && fabs(elecDZ) < vecCutDeltaZ[indexToUse] && fabs((1 - elecEtoP)/elecEn) < vecCutDeltaEP[indexToUse] && elecRelIso < vecCutRelIso[indexToUse]) {
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