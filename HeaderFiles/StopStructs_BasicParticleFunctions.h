// Set of very basic functions used as part of skimming and/or plot filling

#include "TMath.h"

inline void MetPhiCorrect(bool doData, float &MetX, float &MetY, int nVtx, int METType) {
    int c0 = 0;
    int c1 = 1;
    if (!doData) {
        c0 = 2;
        c1 = 3;
    }
    float CorrType1PFMETX[4] = {-2.09720e-01, 2.50634e-01, 1.70969e-01, -2.66011e-02};
    float CorrType1PFMETY[4] = {4.97289e-02, -1.30547e-01, 3.61567e-01, -1.52030e-01};
    
    float CorrType1PFMETX_Error[4] = {1.22617e-02, 8.66910e-04, 5.44773e-02, 3.65571e-03};
    float CorrType1PFMETY_Error[4] = {1.22985e-02, 8.70019e-04, 5.56118e-02, 3.71861e-03};
    
    
    float CorrType1CaloMETX[4] = {-9.39018e-01, 1.41941e-01, 7.11905e-01, -2.88627e-01};
    float CorrType1CaloMETY[4] = {5.83075e-01, -4.54582e-02, 1.45326e+00, -3.12096e-01};
    
    float CorrType1CaloMETX_Error[4] = {1.36453e-02, 9.54727e-04, 6.62481e-02, 4.42074e-03};
    float CorrType1CaloMETY_Error[4] = {1.37454e-02, 9.62002e-04, 6.74509e-02, 4.49338e-03};
    
    
    //  float CorrType1PFMETX_ReReco[4] = {-0.254, 0.309, 0.110, -0.005};
    //  float CorrType1PFMETY_ReReco[4] = {-0.158, -0.160, 0.321, -0.214};
    if (METType == 0) {
        MetX = MetX - CorrType1PFMETX[c0] - nVtx * CorrType1PFMETX[c1];
        MetY = MetY - CorrType1PFMETY[c0] - nVtx * CorrType1PFMETY[c1];
        
    }
    else {
        MetX = MetX - CorrType1CaloMETX[c0] - nVtx * CorrType1CaloMETX[c1];
        MetY = MetY - CorrType1CaloMETY[c0] - nVtx * CorrType1CaloMETY[c1];
        
    }
}

inline void MetPhiCorrectSmear(float &MetX, float &MetY, int nVtx) {
    int c0 = 0;
    int c1 = 1;
    float CorrType1PFMETX[4] = {4.71194e-02, -2.56750e-02};
    float CorrType1PFMETY[4] = {1.52731e-01, -7.42056e-02};
    float CorrType1PFMETX_Error[4] = {5.49905e-02, 3.73960e-03};
    float CorrType1PFMETY_Error[4] = {5.65045e-02, 3.83885e-03};
    
    MetX = MetX - CorrType1PFMETX[c0] - nVtx * CorrType1PFMETX[c1];
    MetY = MetY - CorrType1PFMETY[c0] - nVtx * CorrType1PFMETY[c1];
}

inline void MetPhiCorrect_noType0(bool doData, float &MetX, float &MetY, int nVtx) {
    int c0 = 0;
    int c1 = 1;
    if (!doData) {
        c0 = 2;
        c1 = 3;
    }
    /*
     deprecated paramters (from legacy nTruples, not usable right now
     float CorrType1PFMETX_ReReco[4] = {-0.254, 0.309, 0.110, -0.005};
     float CorrType1PFMETY_ReReco[4] = {-0.158, -0.160, 0.321, -0.214};
     */
    
    float CorrType1PFMETX_ReReco[4] = {-2.35119e-01, 3.07387e-01, 1.38959e-01, -7.37727e-03};
    float CorrType1PFMETY_ReReco[4] = {-1.70399e-01, -1.60497e-01, 3.63978e-01, -2.17595e-01};
    
    float CorrType1PFMETX_ReReco_Error[4] = {1.28320e-02, 9.29808e-04, 1.62513e-02, 1.11343e-03};
    float CorrType1PFMETY_ReReco_Error[4] = {1.3828532e-02, 9.90921e-04, 1.64292e-02, 1.12429e-03};
    
    MetX = MetX - CorrType1PFMETX_ReReco[c0] - nVtx * CorrType1PFMETX_ReReco[c1];
    MetY = MetY - CorrType1PFMETY_ReReco[c0] - nVtx * CorrType1PFMETY_ReReco[c1];
}

inline void METSystShift(vector<TLorentzVector> * inputObjVec, vector<TLorentzVector> * shiftObjVec, float &newMET, float &newMETPhi, float origMET, float origMETPhi) {
    TVector3 vecMET;
    TVector3 inputObjTotThreeVec; inputObjTotThreeVec.SetPtEtaPhi(0., 0., 0.);
    TVector3 shiftObjTotThreeVec; shiftObjTotThreeVec.SetPtEtaPhi(0., 0., 0.);
    TVector3 inputObjCurrThreeVec;
    TVector3 shiftObjCurrThreeVec;
    vecMET.SetPtEtaPhi(origMET, 0., origMETPhi);
    if (inputObjVec->size() != shiftObjVec->size()) {
        cout << "inputObj size " << inputObjVec->size() << endl;
        cout << "shiftObj size " << shiftObjVec->size() << endl;
        cout << "issue: two input object vectors are not same size -- check it out" << endl;
        return;
    }
    for (unsigned int i = 0; i < inputObjVec->size(); ++i) {
        inputObjCurrThreeVec.SetPtEtaPhi(inputObjVec->at(i).Pt(), inputObjVec->at(i).Eta(), inputObjVec->at(i).Phi());
        shiftObjCurrThreeVec.SetPtEtaPhi(shiftObjVec->at(i).Pt(), shiftObjVec->at(i).Eta(), shiftObjVec->at(i).Phi());
        inputObjTotThreeVec = inputObjTotThreeVec + inputObjCurrThreeVec;
        shiftObjTotThreeVec = shiftObjTotThreeVec + shiftObjCurrThreeVec;
    }
    vecMET = vecMET - inputObjTotThreeVec;
    vecMET = vecMET + shiftObjTotThreeVec;
    newMET = vecMET.Pt();
    newMETPhi = vecMET.Phi();
    return;
}

inline void METSystShift(vector<Lepton> * inputLepVec, vector<Lepton> * shiftLepVec, float &newMET, float &newMETPhi, float origMET, float origMETPhi) {
    TVector3 vecMET;
    TVector3 inputLepTotThreeVec; inputLepTotThreeVec.SetPtEtaPhi(0., 0., 0.);
    TVector3 shiftLepTotThreeVec; shiftLepTotThreeVec.SetPtEtaPhi(0., 0., 0.);
    TVector3 inputLepCurrThreeVec;
    TVector3 shiftLepCurrThreeVec;
    vecMET.SetPtEtaPhi(origMET, 0., origMETPhi);
    if (inputLepVec->size() != shiftLepVec->size()) {
        cout << "inputLep size " << inputLepVec->size() << endl;
        cout << "shiftLep size " << shiftLepVec->size() << endl;
        cout << "issue: two input object vectors are not same size -- check it out" << endl;
        return;
    }
    for (unsigned int iLep = 0; iLep < inputLepVec->size(); ++iLep) {
        inputLepCurrThreeVec.SetXYZ(inputLepVec->at(iLep).BVC.Vec_Px, inputLepVec->at(iLep).BVC.Vec_Py, inputLepVec->at(iLep).BVC.Vec_Pz);
        shiftLepCurrThreeVec.SetXYZ(shiftLepVec->at(iLep).BVC.Vec_Px, shiftLepVec->at(iLep).BVC.Vec_Py, shiftLepVec->at(iLep).BVC.Vec_Pz);
        inputLepTotThreeVec = inputLepTotThreeVec + inputLepCurrThreeVec;
        shiftLepTotThreeVec = shiftLepTotThreeVec + shiftLepCurrThreeVec;
    }
    vecMET = vecMET - inputLepTotThreeVec;
    vecMET = vecMET + shiftLepTotThreeVec;
    newMET = vecMET.Pt();
    newMETPhi = vecMET.Phi();
    return;
}

inline void METSystShift(vector<PFJet> * inputJetVec, vector<PFJet> * shiftJetVec, float &newMET, float &newMETPhi, float origMET, float origMETPhi, int doVerbosity = 0) {
    TVector3 vecMET;
    TVector3 inputJetTotThreeVec; inputJetTotThreeVec.SetPtEtaPhi(0., 0., 0.);
    TVector3 shiftJetTotThreeVec; shiftJetTotThreeVec.SetPtEtaPhi(0., 0., 0.);
    TVector3 inputJetCurrThreeVec;
    TVector3 shiftJetCurrThreeVec;
    vecMET.SetPtEtaPhi(origMET, 0., origMETPhi);
    if (inputJetVec->size() != shiftJetVec->size()) {
        cout << "inputJet size " << inputJetVec->size() << endl;
        cout << "shiftJet size " << shiftJetVec->size() << endl;
        cout << "issue: two input object vectors are not same size -- check it out" << endl;
        return;
    }
    for (unsigned int iJet = 0; iJet < inputJetVec->size(); ++iJet) {
        inputJetCurrThreeVec.SetXYZ(inputJetVec->at(iJet).BVC.Vec_Px, inputJetVec->at(iJet).BVC.Vec_Py, inputJetVec->at(iJet).BVC.Vec_Pz);
        shiftJetCurrThreeVec.SetXYZ(shiftJetVec->at(iJet).BVC.Vec_Px, shiftJetVec->at(iJet).BVC.Vec_Py, shiftJetVec->at(iJet).BVC.Vec_Pz);
        if (doVerbosity > 0) {
            cout << "inputJetCurrThreeVec Pt " << inputJetCurrThreeVec.Pt() << endl;
            cout << "shiftJetCurrThreeVec Pt " << shiftJetCurrThreeVec.Pt() << endl;
            cout << "inputJetCurrThreeVec Eta " << inputJetCurrThreeVec.Eta() << endl;
            cout << "shiftJetCurrThreeVec Eta " << shiftJetCurrThreeVec.Eta() << endl;
            cout << "inputJetCurrThreeVec Phi " << inputJetCurrThreeVec.Phi() << endl;
            cout << "shiftJetCurrThreeVec Phi " << shiftJetCurrThreeVec.Phi() << endl;
        }
        inputJetTotThreeVec = inputJetTotThreeVec + inputJetCurrThreeVec;
        shiftJetTotThreeVec = shiftJetTotThreeVec + shiftJetCurrThreeVec;
    }
    vecMET = vecMET - inputJetTotThreeVec;
    vecMET = vecMET + shiftJetTotThreeVec;
    
    newMET = vecMET.Pt();
    newMETPhi = vecMET.Phi();
    if (doVerbosity > 0) {
        cout << "old Met " << origMET << endl;
        cout << "new MET " << newMET << endl;
    }
    return;
}

inline int CheckPairing(int jet0, int jet1, int lep0, int lep1, bool doVerbosity = 0) {
    int outVal = -2;
    int signPair1, signPair2;
    signPair1 = TMath::Sign(1, jet0 * lep0);
    signPair2 = TMath::Sign(1, jet1 * lep1);
    if (signPair1 < 0 && signPair2 < 0) {
        outVal = 1;
    }
    else if (signPair1 > 0 && signPair2 > 0) {
        outVal = 0;
    }
    else {
        if (doVerbosity) {
            cout << "something weird: " << endl;
            cout << "signPair1 " << signPair1 << endl;
            cout << "signPair2 " << signPair2 << endl;
            cout << "jet0 parton flavor " << jet0 << endl;
            cout << "jet1 parton flavor " << jet1 << endl;
            cout << "lep0 PDGID " << lep0 << endl;
            cout << "lep1 PDGID " << lep1 << endl;
        }
    }
    return outVal;
}

inline int CheckMT2lblbPairing(int whichPair, vector<int> * inputJetPartFlavor, vector<int> * inputLepPDGIDs, bool doVerbosity = 0) {
    int outVal = -1;
    if (abs(inputJetPartFlavor->at(0)) != 5 || abs(inputJetPartFlavor->at(1)) != 5) {
        return outVal;
    }
    else {
        switch (whichPair) {
            case 0:
                //first lep with first jet
                outVal = CheckPairing(inputJetPartFlavor->at(0), inputJetPartFlavor->at(1), inputLepPDGIDs->at(0), inputLepPDGIDs->at(1), doVerbosity);
                break;
            case 1:
                //first lep with second jet
                outVal = CheckPairing(inputJetPartFlavor->at(1), inputJetPartFlavor->at(0), inputLepPDGIDs->at(0), inputLepPDGIDs->at(1), doVerbosity);
                break;
            default:
                cout << "whichPair needs to be either 0 or 1 -- currently is " << whichPair << endl;
                break;
        }
    }
    if (doVerbosity) {
        cout << "case: " << whichPair <<  endl;
        cout << "inputJetPartFlavor->at(0) " << inputJetPartFlavor->at(0) << endl;
        cout << "inputJetPartFlavor->at(1) " << inputJetPartFlavor->at(1) << endl;
        cout << "inputLepPDGIDs->at(0) " << inputLepPDGIDs->at(0) << endl;
        cout << "inputLepPDGIDs->at(1) " << inputLepPDGIDs->at(1) << endl;
        cout << "outVal: " << outVal <<  endl;
    }
    return outVal;
}

inline float MT2lbCalculator(vector<TLorentzVector> * vecLeps, vector<TLorentzVector> * vecJets, float MET, float METPhi, vector<TLorentzVector> &vecBLeps, int &corrPairVal, vector<int> * inputJetPartFlavor = 0, vector<int> * inputLepPDGIDs = 0) {
    float MT2lbPair1;
    float MT2lbPair2;
    TLorentzVector vecBLeadLep, vecBSubLep;
    if (vecLeps->size() < 2 || vecJets->size() < 2) {
        cout << "Houston, we've had a problem here: one of the two vectors is less than 2!" << endl;
        cout << "vecLep size " << vecLeps->size() << endl;
        cout << "vecJet size " << vecJets->size() << endl;
    }
    MT2lbPair1 = getMT2(vecLeps->at(0) + vecJets->at(0), vecLeps->at(1) + vecJets->at(1), MET, METPhi);
    MT2lbPair2 = getMT2(vecLeps->at(0) + vecJets->at(1), vecLeps->at(1) + vecJets->at(0), MET, METPhi);
    if (MT2lbPair1 > MT2lbPair2) {
        vecBLeadLep = vecLeps->at(0) + vecJets->at(1);
        vecBSubLep = vecLeps->at(1) + vecJets->at(0);
        if (inputJetPartFlavor == NULL || inputLepPDGIDs == NULL) {
            corrPairVal = -2;
        }
        else {
            corrPairVal = CheckMT2lblbPairing(0, inputJetPartFlavor, inputLepPDGIDs);
        }
    }
    else {
        vecBLeadLep = vecLeps->at(0) + vecJets->at(0);
        vecBSubLep = vecLeps->at(1) + vecJets->at(1);
        if (inputJetPartFlavor == NULL || inputLepPDGIDs == NULL) {
            corrPairVal = -2;
        }
        else {
            corrPairVal = CheckMT2lblbPairing(1, inputJetPartFlavor, inputLepPDGIDs);
        }
    }
    vecBLeps[0] = vecBLeadLep;
    vecBLeps[1] = vecBSubLep;
    return TMath::Min(MT2lbPair1, MT2lbPair2);
}
